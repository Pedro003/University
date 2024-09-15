-module(server).
-export([start/0]).

start() ->
    Port = 1234,
    {ok, ListenSocket} = gen_tcp:listen(Port, [{active, false}, {reuseaddr, true}]),
    io:format("Server started on port ~p~n", [Port]),
    % Inicia o processo do lobby
    Lobby_Manager = lobby_manager:start(),
    % Initialized as an empty map
    Room = spawn(fun() -> room(#{}, Lobby_Manager) end),
    spawn(fun() -> loop(ListenSocket, Room) end).

loop(ListenSocket, Room) ->
    {ok, Socket} = gen_tcp:accept(ListenSocket),
    io:format("Accepted connection~n", []),
    spawn(fun() -> loop(ListenSocket, Room) end),
    Room ! {enter, self()},
    handle_connection(Socket, Room).

% Room that contains all connections between each client and the server
room(Users, Lobby_Manager) ->
    receive
        {enter, Pid} ->
            io:format("user entered~n", []),
            NewUsers = maps:put(Pid, undefined, Users),
            io:format("Updated Users after enter: ~p~n", [NewUsers]),
            room(NewUsers, Lobby_Manager);
        {leave, Pid} ->
            io:format("user left~n", []),
            NewUsers = maps:remove(Pid, Users),
            io:format("Updated Users after leave: ~p~n", [NewUsers]),
            room(NewUsers, Lobby_Manager);
        {logout, Pid} ->
            case maps:find(Pid, Users) of
                {ok, Username} ->
                    io:format("user logout: ~p (Username: ~p)~n", [Pid, Username]),
                    % sets status to "OFF"
                    user_Account:update_user_status(Username),
                    NewUsers = maps:put(Pid, undefined, Users),
                    io:format("Updated Users after logout: ~p~n", [NewUsers]),
                    room(NewUsers, Lobby_Manager);
                error ->
                    io:format("logout failed: ~p not found~n", [Pid]),
                    room(Users, Lobby_Manager)
            end;
        {lobby, Pid} ->
            case maps:find(Pid, Users) of
                {ok, Username} ->
                    io:format("user update: ~p (Username: ~p)~n", [Pid, Username]),
                    Lobby_Manager ! {play, Pid, Username},
                    room(Users, Lobby_Manager);
                %!
                {error} ->
                    io:format("Player: ~p not found~n", [Pid]),
                    room(Users, Lobby_Manager)
            end;
        {Username, Pid} ->
            io:format("user update: ~p (Username: ~p)~n", [Pid, Username]),
            NewUsers = maps:put(Pid, Username, Users),
            io:format("Updated Users after user update: ~p~n", [NewUsers]),
            room(NewUsers, Lobby_Manager)
    end.

handle_connection(Socket, Room) ->
    case gen_tcp:recv(Socket, 0) of
        {ok, Data} ->
            TrimmedData = string:trim(Data),
            RequestN = string:split(TrimmedData, "\n"),
            Request = string:join(RequestN, " "),
            case Request of
                "Exit" ->
                    io:format("Received data: ~p~n", [Request]),
                    gen_tcp:send(Socket, "Exiting"),
                    Room ! {leave, self()},
                    gen_tcp:close(Socket),
                    exit(normal);
                _ ->
                    io:format("Received data: ~p~n", [Request]),
                    handle_request(Socket, Request, Room)
            end;
        {error, closed} ->
            io:format("Client disconnected~n"),
            ok
    end.

handle_request(Socket, Data, Room) ->
    Tokens = string:tokens(Data, " "),
    case Tokens of
        [Command, Username, Password] ->
            Response =
                case Command of
                    "register" ->
                        case user_Account:register(Username, Password) of
                            already_exists ->
                                "user_already_exists";
                            ok ->
                                Room ! {Username, self()},
                                "register_ok"
                        end;
                    "login" ->
                        case user_Account:login(Username, Password) of
                            ok ->
                                user_Account:update_user_status(Username),
                                Room ! {Username, self()},
                                "login_ok";
                            {error, Reason} ->
                                Reason
                        end;
                    _ ->
                        "Unknown command"
                end;
        _ ->
            Response = "Invalid request format"
    end,
    gen_tcp:send(Socket, Response ++ "\n"),
    case Response of
        "user_already_exists" ->
            handle_connection(Socket, Room);
        "invalid_account" ->
            handle_connection(Socket, Room);
        "invalid_password" ->
            handle_connection(Socket, Room);
        _ ->
            user_options(Socket, Room)
    end.

user_options(Socket, Room) ->
    case gen_tcp:recv(Socket, 0) of
        {ok, Data} ->
            TrimmedData = string:trim(Data),
            case TrimmedData of
                "Play" ->
                    io:format("Received data: ~p~n", [TrimmedData]),
                    % !
                    Room ! {lobby, self()},
                    % Uncomment and implement this when ready
                    player(Socket, Room, 0);
                % Complete
                "Logout" ->
                    io:format("Received data: ~p~n", [TrimmedData]),
                    gen_tcp:send(Socket, "logout_ok\n"),
                    Room ! {logout, self()},
                    handle_connection(Socket, Room);
                % Seems complete
                "Leaderboard" ->
                    io:format("Received data: ~p~n", [TrimmedData]),
                    Players = user_Account:top_players(),
                    io:format("Received data: ~p~n", [Players]),
                    gen_tcp:send(Socket, Players ++ "\n"),
                    io:format("Passou o send~n"),
                    user_options(Socket, Room);
                _ ->
                    io:format("Unknown command received: ~p~n", [TrimmedData]),
                    user_options(Socket, Room)
            end;
        {error, closed} ->
            io:format("Client disconnected~n"),
            Room ! {logout, self()},
            ok
    end.

player(Socket, Room, Lobby_ID) ->
    inet:setopts(Socket, [{active, once}]),
    receive
        {name, Username} ->
            gen_tcp:send(Socket, Username ++ "\n"),
            player(Socket, Room, Lobby_ID);
        %?{lobby, Players, Lobby_Pid} ->
        %?    Usernames = lists:map(fun({_, Username}) -> Username end, Players),
        %?    Users = string:join(Usernames, ","),
        %?    io:format("~s~n", [Users]),
        %?    gen_tcp:send(Socket, "Update_lobby\n"),
        %?    gen_tcp:send(Socket, Users ++ "\n"),
        %?    player(Socket, Room, Lobby_Pid);
        %?{tcp, Socket, Data} -> % Receber dados por socket TCP
        %?    io:format("Received~n"),
        %?    TrimmedData = string:trim(Data),
        %?    io:format("Received data: ~p~n", [TrimmedData]),
        %?    case TrimmedData of
        %?        "Leave" ->
        %?            Lobby_ID ! {remove_player, self()},
        %?            gen_tcp:send(Socket, "Left\n"), % ultima coisa que adicionei
        %?            user_options(Socket, Room);
        %?        _ ->
        %?            player(Socket, Room, Lobby_ID)
        %?    end;
        {match_started, MatchPid} ->
            gen_tcp:send(Socket, "Start_ok\n"),
            player_playing(MatchPid, Socket, Room);
        {error, closed} ->
            io:format("Client disconnected~n"),
            Room ! {logout, self()},
            ok
    end.

player_playing(MatchPid, Socket, Room) ->
    inet:setopts(Socket, [{active, once}]),
    receive
        % as posições de todos os planetas num certo momento
        {planets_update, Positions} ->
            gen_tcp:send(Socket, "Update_planets\n"),
            StringPos = coordinates_to_string(Positions),
            %io:format("~p~n~n~n", [StringPos]),
            gen_tcp:send(Socket, StringPos),
            player_playing(MatchPid, Socket, Room);
        % as posições de todos os planetas num certo momento
        {players_update, Positions} ->
            gen_tcp:send(Socket, "Update_players\n"),
            StringPos = coordinates_to_string(Positions),
            io:format("~p~n~n~n", [StringPos]),
            gen_tcp:send(Socket, StringPos),
            player_playing(MatchPid, Socket, Room);
        %!{init_player_position, Pos} -> % Pos contem as coordenadas iniciais do jogador
        %!    io:format("~p~n", [Pos]),
        %!    gen_tcp:send(Socket, Pos),
        %!    player_playing(MatchPid, Socket, Room);
        random_player_lost ->
            gen_tcp:send(Socket, "Loser\n"),
            player_playing(MatchPid, Socket, Room);
        player_lost ->
            gen_tcp:send(Socket, "I_lost\n"),
            io:format("~n~n~n~n~n~n~n-----------A Player lost------------~n~n~n~n~n~n~n~n"),
            user_options(Socket, Room);
        % Receber dados por socket TCP
        {tcp, Socket, Data} ->
            TrimmedData = string:trim(Data),
            case TrimmedData of
                "w" ->
                    MatchPid ! {player_input, self(), "w"};
                "a" ->
                    MatchPid ! {player_input, self(), "a"};
                "d" ->
                    MatchPid ! {player_input, self(), "d"}
            end,
            player_playing(MatchPid, Socket, Room)
    end.

coordinates_to_string(Coordinates) ->
    FormattedCoordinates = lists:map(
        fun({X, Y}) -> io_lib:format("{~p, ~p}", [X, Y]) end, Coordinates
    ),
    string:join(FormattedCoordinates, ", ") ++ "\n".
