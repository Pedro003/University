-module(match).
-export([start/1]).

-define(SOL_POS, {625, 400}).
-define(SOL_MASS, 100).
-define(G, 0.05).

-record(player, {pos, vel, acc, angle, fuel, radius}).
%-record(planet, {orbit_radius, orbit_speed, radius, angle, pos}).

start(Players) ->
    MatchPid = self(),
    % Cria Processo para atualizar as posições dos planetas

    %! aqui
    PlanetsP = spawn(fun() -> planet:start(MatchPid) end),
    % Cria Processo para atualizar as posições dos jogadores
    PlayerPositions = [{100, 100}, {1000, 700}, {100, 700}, {1000, 100}],
    PlayerPids = assign_positions(Players, PlayerPositions, []),
    PlayersP = spawn(fun() -> players:start(MatchPid, PlayerPids, []) end),
    % Start the main loop
    loop(PlayersP, {PlanetsP, PlayerPids}).

assign_positions([], _, Acc) ->
    lists:reverse(Acc);
assign_positions([{Pid, Username} | Rest], [Pos | PosRest], Acc) ->
    % Send initial position to each player
    assign_positions(Rest, PosRest, [
        {Pid, Username, #player{
            pos = Pos, vel = {0, 0}, acc = {0, 0}, angle = 0, fuel = 100, radius = 10
        }}
        | Acc
    ]).

loop(PlayersP, State) ->
    receive
        %! {player_input, PlayerPid, Key} ->
        %!     UpdatedState = handle_player_input(State, PlayerPid, Key),
        %!     loop(UpdatedState);

        %! alterado e funcional
        {planets, PlanetsState} ->
            %io:format("~n~p~n~n~n", [PlanetsState]),
            PlayersP ! {update_planet, PlanetsState},
            UpdatedState = update_planet(State, PlanetsState),
            send_planet_updates(UpdatedState),
            loop(PlayersP, UpdatedState);
        %! ainda no cook
        {players, PlayersState} ->
            %io:format("~n~p~n~n", [PlayersState]),
            UpdatedState = update_players(State, PlayersState),
            send_players_updates(UpdatedState),
            loop(PlayersP, UpdatedState);
        stop ->
            io:format("Game Over~n"),
            ok
    end.

%! a funcionar
update_planet({_Planets, Players}, PlanetsState) ->
    {PlanetsState, Players}.

update_players({Planets, _Players}, PlayersState) ->
    {Planets, PlayersState}.

%! a funcionar
send_planet_updates({Planets, Players}) ->
    lists:foreach(
        fun({Pid, _Username, _Player}) ->
            Pid ! {planets_update, extract_planet_data(Planets)}
        end,
        Players
    ).

%! a funcionar
extract_planet_data([{planet, _, _, _, _, {X, Y}} | Rest]) ->
    [{X, Y} | extract_planet_data(Rest)];
extract_planet_data([]) ->
    [].

send_players_updates({_Planets, Players}) ->
    lists:foreach(
        fun({Pid, _Username, _Player}) ->
            Pid ! {players_update, extract_positions(Players)}
        end,
        Players
    ).
%lista = extract_positions(Players),
%lists:foreach(
%    fun({Pid, _Username, _Player}) ->
%        Pid ! {players_update, lista}
%    end,
%    Players
%).

extract_positions([
    {_, _, #player{pos = {X, Y}, vel = _, acc = _, angle = _, fuel = _, radius = _}} | Rest
]) ->
    [{X, Y} | extract_positions(Rest)];
extract_positions([]) ->
    [].
