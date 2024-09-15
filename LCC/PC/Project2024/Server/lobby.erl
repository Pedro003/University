%! Tempo de entrada para jogadores n está a renovar corretamente os 5s quando entra o 3ºjogador por isso esta versão não da uso de tempos de espera entre jogadores

-module(lobby).
-export([start/1]).

start(Lobby_manager_Pid) ->
    loop(Lobby_manager_Pid, [], undefined).

loop(Lobby_manager_Pid, Players, Timer) ->
    receive
        {add_player, Player = {Pid, Username}} ->
            io:format("Player ~p with Username ~p added to the lobby ~p.~n", [
                Pid, Username, Lobby_manager_Pid
            ]),
            UpdatedPlayers = [Player | Players],
            %Pid ! {name, Username},
            %? inform_players(UpdatedPlayers, self()),
            io:format("Timer: ~p", [Timer]),
            case length(UpdatedPlayers) of
                2 ->
                    % Define um novo temporizador para 5 segundos
                    %? NewTimer = timer:send_after(5000, self(), start_match),

                    %! retirar depois
                    start_match(UpdatedPlayers),
                    %! retirar depois
                    Lobby_manager_Pid ! {lobby_full, self()},
                    %! retirar depois
                    loop(Lobby_manager_Pid, [], undefined);
                %loop(Lobby_manager_Pid, UpdatedPlayers, Timer);
                3 ->
                    % Cancela o temporizador anterior e define um novo
                    case Timer of
                        undefined -> ok;
                        _ -> timer:cancel(Timer)
                    end,
                    %? NewTimer = timer:send_after(5000, self(), start_match),
                    loop(Lobby_manager_Pid, UpdatedPlayers, Timer);
                4 ->
                    % Cancela o temporizador, pois a sala está cheia
                    case Timer of
                        undefined -> ok;
                        _ -> timer:cancel(Timer)
                    end,
                    start_match(UpdatedPlayers),
                    Lobby_manager_Pid ! {lobby_full, self()},
                    loop(Lobby_manager_Pid, [], undefined);
                _ ->
                    loop(Lobby_manager_Pid, UpdatedPlayers, Timer)
            end;
        {remove_player, Pid} ->
            UpdatedPlayers = remove_player(Pid, Players),
            inform_players(UpdatedPlayers, self()),
            loop(Lobby_manager_Pid, UpdatedPlayers, Timer);
        %? serve para quando tivermos o temporizador a funcionar
        start_match ->
            case length(Players) >= 2 of
                true ->
                    io:format("Starting match with players: ~p~n", [Players]),
                    %!start_match(Players),
                    Lobby_manager_Pid ! {lobby_full, self()},
                    loop(Lobby_manager_Pid, [], undefined);
                false ->
                    % Apenas continue o loop, caso não haja jogadores suficientes
                    loop(Lobby_manager_Pid, Players, Timer)
            end
    end.

remove_player(_Pid, []) -> [];
remove_player(Pid, [{Pid, _} | T]) -> T;
remove_player(Pid, [H | T]) -> [H | remove_player(Pid, T)].

inform_players(Players, Lobby_Pid) ->
    lists:foreach(
        fun({Pid, _Username}) ->
            Pid ! {lobby, Players, Lobby_Pid}
        end,
        Players
    ).

inform_players_match(Players, MatchPid) ->
    lists:foreach(
        fun({Pid, _Username}) ->
            Pid ! {match_started, MatchPid}
        end,
        Players
    ).

start_match(Players) ->
    MatchPid = spawn(fun() -> match:start(Players) end),
    inform_players_match(Players, MatchPid).
