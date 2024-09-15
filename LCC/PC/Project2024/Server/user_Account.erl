-module(user_Account).
-import(dict, [new/0, store/3, erase/2, find/2, fold/3]).
-export([update_user_status/1, remove_from_file/1, top_players/0, register/2, login/2, add/3, write_to_file/2, read_from_file/1]).

add(Dict, Username, {Password, Lvl, Vics, Defs, State}) ->
    dict:store(Username, {Password, Lvl, Vics, Defs, State}, Dict).


login(Username, Password) -> %esta melhor assim
    ExistingDict = read_from_file("user.txt"),
    case dict:find(Username, ExistingDict) of
        {ok, {StoredPassword, _, _, _, State}} ->
            case Password == StoredPassword of
                true ->
                    case State /= "ON" of
                        true -> ok;
                        false -> {error, "invalid_account"}
                    end;
                false -> {error, "invalid_password"}
            end;
        error -> {error, "invalid_account"}
    end.

register(Username, Password) -> %esta melhor assim
    ExistingDict = read_from_file("user.txt"), %vai ao file user.txt e itera sobre cada linha armazenando todos os users la existentes num novo dicionario ExistingDict(output) 
    case dict:find(Username, ExistingDict) of
        {ok, _} -> 
            already_exists; %caso o user ja exista retorna %!already_exists
        error -> %caso nao exista
            NewDict = add(ExistingDict, Username, {Password, 1, 0, 0, "ON"}), %incrementa um novo user + ExistingDict = NewDict(novo dicionario)
            write_to_file(NewDict, "user.txt"), %escreve para o ficheiro "user.txt" todos os tuplos(users) existentes no dicionario(NewDict)
            ok %retorna %!ok
    end.

write_to_file(Dict, FilePath) ->
    {ok, File} = file:open(FilePath, [write]), % o que intereça saber aqui é que o ficheiro "user.txt" vai ser aberto em modo de escrita sendo o File o seu descritor de escrita
    try
        dict:fold(fun(Key, {Password, Lvl, Vics, Defs, State}, _) ->
                          Str = io_lib:format("~s|~s|~w|~w|~w|~s\n", [Key, Password, Lvl, Vics, Defs, State]),
                          file:write(File, Str)
                  end, ok, Dict)
    after
        file:close(File)
    end.

remove_from_file(Username) ->
    Dict = read_from_file("user.txt"),
    NewDict = maps:remove(Username, Dict),
    write_to_file(NewDict,"user.txt").

read_from_file(FilePath) ->
    {ok, FileContent} = file:read_file(FilePath),
    Lines = string:split(binary_to_list(FileContent), "\n", all),
    lists:foldl(
        fun(Line, Dict) ->
            case Line of
                "" -> Dict;  % ignora linhas vazias
                _ ->
                    [Username, Password, Lvl, Vics, Defs, State] = string:split(Line, "|", all), %separa os campos da string toda por "|" e coloca-os numa lista
                    NewDict = add(Dict, Username, {Password, list_to_integer(Lvl), list_to_integer(Vics), list_to_integer(Defs), State}), % começa a criar um dicionario novo(Newdict) que vai substituir o Dict 
                    NewDict
            end
        end,
        dict:new(),  % inicializa um dicionario novo
        Lines
    ).

top_players() ->
    ExistingDict = read_from_file("user.txt"),
    PlayersList = dict:fold(
        fun(Key, {Password, Lvl, Vics, Defs, State}, Acc) ->
            [{Key, Password, Lvl, Vics, Defs, State} | Acc]
        end, [], ExistingDict),
    SortedPlayers = lists:sort(
        fun({_, _, Lvl1, Vics1, Defs1, _}, {_, _, Lvl2, Vics2, Defs2, _}) ->
            if
                Lvl1 > Lvl2 -> true;
                Lvl1 < Lvl2 -> false;
                true -> (Vics1 - Defs1) > (Vics2 - Defs2)
            end
        end, PlayersList),
    Top10Players = lists:sublist(SortedPlayers, 10),
    PlayersStrings = lists:map(
        fun({Username, _, Lvl, Vics, Defs, _}) ->
            io_lib:format("~s|~w|~w|~w", [Username, Lvl, Vics, Defs])
        end, Top10Players),
    lists:flatten(io_lib:format("~s", [string:join(PlayersStrings, ",")])).  


update_user_status(Username) ->
    Dict = read_from_file("user.txt"),
    case dict:find(Username, Dict) of
        {ok, {Pass, Lvl, MaxW, MaxL, Status}} ->
            NewStatus = if 
                Status == "OFF" -> "ON"; 
                true -> "OFF" 
            end,
            UpdatedUser = {Pass, Lvl, MaxW, MaxL, NewStatus},
            UpdatedDict = dict:store(Username, UpdatedUser, Dict),
            write_to_file(UpdatedDict, "user.txt");
        error ->
            io:format("User ~p not found~n", [Username])
    end.


% ! preciso de fazer uma função para dar update ao nível/MaxW/MaxL fornecendo o Username como imput
