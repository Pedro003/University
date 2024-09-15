-module(players).
-export([start/3, loopPlayer/3]).

-define(SOL_POS, {625, 400}).
-define(SOL_MASS, 100).
-define(G, 0.05).

-record(player, {pos, vel, acc, angle, fuel, radius}).
-record(planet, {orbit_radius, orbit_speed, radius, angle = rand:uniform() * 6.28, pos}).

start(MatchPid, Players, Planets) ->
    loopPlayer(MatchPid, Players, Planets).

%![{Pid, Username, player{pos, vel, acc , angle, fuel, radius},{},...] [planet{orbit_radius, orbit_speed, radius, angle, pos},planet{},planet{},planet{},planet{}]
loopPlayer(MatchPid, Players, _Planets) ->
    receive
        {update_planet, Updated_planets} ->
            %io:format("~n~p~n~n", [Updated_planets]),
            %io:format("~n~p~n~n", [Players]),
            PlayersState = update_players(Players, Updated_planets),
            %io:format("~p~n", [PlayersState]),
            MatchPid ! {players, PlayersState},
            loopPlayer(MatchPid, PlayersState, Updated_planets)
    end.

update_players(Players, Planets) ->
    UpdatedPlayers = [{Pid, Username, apply_gravity(Player)} || {Pid, Username, Player} <- Players],
    UpdatedPlayers2 = [
        {Pid, Username, update_position(Player)}
     || {Pid, Username, Player} <- UpdatedPlayers
    ],
    %io:format("~n~p~n~n", [UpdatedPlayers2]),
    check_collisions(UpdatedPlayers2, Planets).

%! handle_player_input(State, PlayerPid, Key) ->
%!     {Planets, Players} = State,
%!     case lists:keyfind(PlayerPid, 1, Players) of
%!         false ->
%!             State; % Player not found
%!         {PlayerPid, Username, Player} ->
%!             UpdatedPlayer = update_player_physics(Player, Key),
%!             NewPlayers = lists:keyreplace(PlayerPid, 1, Players, {PlayerPid, Username, UpdatedPlayer}),
%!             {Planets, NewPlayers}
%!     end.

%! update_player_physics(Player, Key) ->
%!     case Key of
%!         "w" ->
%!             _ = {math:cos(Player#player.angle), math:sin(Player#player.angle)},
%!             Player1 = apply_force(Player, Player#player.angle, 0.1),
%!             Player1#player{fuel = Player#player.fuel - 0.1};
%!         "a" ->
%!             Player#player{angle = Player#player.angle - 0.1, fuel = Player#player.fuel - 0.1};
%!         "d" ->
%!             Player#player{angle = Player#player.angle + 0.1, fuel = Player#player.fuel - 0.1};
%!         _ -> Player
%!     end.

%! apply_force(Player, Angle, Magnitude) ->
%!     {X, Y} = Player#player.acc,
%!     {X2, Y2} = {X + Magnitude * math:cos(Angle), Y + Magnitude * math:sin(Angle)},
%!     Player#player{acc = {X2, Y2}}.

%! funciona
apply_gravity(Player) ->
    {X, Y} = Player#player.pos,
    %io:format("player~n~p~n~n", [Player]),
    {SolX, SolY} = ?SOL_POS,
    Force = {SolX - X, SolY - Y},
    %io:format("force~n~p~n~n", [Force]),

    Distance = distance({X, Y}, {SolX, SolY}),
    %io:format("distance~n~p~n~n", [Distance]),
    ConstrainedDistance = max(5, min(25, Distance)),
    %io:format("constrained~n~p~n~n", [ConstrainedDistance]),
    NormalizedForce = {
        element(1, Force) / ConstrainedDistance, element(2, Force) / ConstrainedDistance
    },
    %io:format("normalized~n~p~n~n", [NormalizedForce]),
    Strength = (?G * ?SOL_MASS) / (ConstrainedDistance * ConstrainedDistance),
    %io:format("strength~n~p~n~n", [Strength]),
    {ForceX, ForceY} = {
        element(1, NormalizedForce) * Strength, element(2, NormalizedForce) * Strength
    },
    %io:format("force~n~p~n~n", [{ForceX, ForceY}]),
    {AccX, AccY} = Player#player.acc,
    %io:format("acc~n~p~n~n", [{AccX, AccY}]),
    Player#player{acc = {AccX + ForceX, AccY + ForceY}}.

%! funciona
update_position(Player) ->
    {VelX, VelY} = Player#player.vel,
    {AccX, AccY} = Player#player.acc,
    {NewVelX, NewVelY} = {VelX + AccX, VelY + AccY},
    {PosX, PosY} = Player#player.pos,
    {NewPosX, NewPosY} = {PosX + NewVelX, PosY + NewVelY},
    Player#player{vel = {NewVelX, NewVelY}, pos = {NewPosX, NewPosY}, acc = {0, 0}}.

collides(PlayerPos, PlanetPos) ->
    distance(PlayerPos, PlanetPos) =< 2.

check_collisions(Players, Planets) ->
    lists:foldl(
        fun(Player, Collisions) ->
            lists:foldl(
                fun(Planet, Acc) ->
                    {Pid, _, _} = Player,
                    %io:format("Chega aqui"),
                    {_Pid, _Username, {player, PlayerPos, _Vel, _Acc, _Angle, _Fuel, _Radius}} =
                        Player,
                    %io:format("~p~n", [PlayerPos]),
                    {planet, _, _, _, __, PlanetPos} = Planet,
                    case collides(PlayerPos, PlanetPos) of
                        true ->
                            notify_players_of_loss(Players, Pid);
                        false ->
                            case lists:member(Player, Acc) of
                                false ->
                                    [Player | Acc];
                                true ->
                                    Acc
                            end
                    end
                end,
                Collisions,
                Planets
            )
        end,
        [],
        Players
    ).

distance({X1, Y1}, {X2, Y2}) ->
    math:sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1)).

notify_players_of_loss(Players, LostPlayerPid) ->
    lists:foreach(
        fun({Pid, _, _}) ->
            if
                Pid =/= LostPlayerPid ->
                    % Notify other players that a specific player lost
                    Pid ! random_player_lost;
                true ->
                    ok
            end,
            if
                Pid == LostPlayerPid ->
                    % Notify the specific player who lost
                    Pid ! player_lost;
                true ->
                    ok
            end
        end,
        Players
    ).
