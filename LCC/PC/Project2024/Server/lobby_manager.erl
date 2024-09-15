-module(lobby_manager).
-export([start/0]).

start() ->
    spawn(fun() -> loop(dict:new()) end).

loop(Lobbies) ->
    receive
        {play, Pid, Username} ->
            Player = {Pid, Username},
            case find_lobby_with_space(Lobbies) of
                {ok, LobbyPid} ->
                    LobbyPid ! {add_player, Player},
                    loop(Lobbies);
                error ->
                    io:format("Creating a new lobby~n"),
                    LobbyPid = spawn(fun() -> lobby:start(self()) end),
                    LobbyPid ! {add_player, Player},
                    NewLobbies = dict:store(LobbyPid, true, Lobbies),
                    loop(NewLobbies)
            end;
        {lobby_full, LobbyPid} ->
            loop(dict:erase(LobbyPid, Lobbies))
    end.

find_lobby_with_space(Lobbies) ->
    case [LobbyPid || {LobbyPid, true} <- dict:to_list(Lobbies)] of
        [] -> error;
        [LobbyPid | _] -> {ok, LobbyPid}
    end.
