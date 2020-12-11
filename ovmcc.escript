%% -*- erlang -*-
%%! -smp enable
-include_lib("kernel/include/file.hrl").
-mode(compile).

strip_new_line(String) ->
    Index = string:str(String, "\n"),
    case Index > 0 of
        true -> string:strip(String, both, $\n);
        _ -> String
    end.
strip_comment(File) ->
    case io:get_line(File, <<>>) of
        eof -> "";
        Line ->
            Index = string:str(Line, ";;"),
            [strip_new_line(L) || L <- [case Index > 0 of
                                             true -> string:sub_string(Line, 1, Index - 1);
                                              _ -> Line
                                        end | strip_comment(File)], length(L) > 0]
    end.

%% main
read_file(Path) ->
    R = file:open(Path, [read]),
    case R of
        {ok, File} ->
            Program = strip_comment(File),
            io:format("> Program:~p~n", [Program]);
        _ ->
            io:format("> File open failed~n")
    end.

main([]) ->
    io:setopts([{encoding, unicode}]),
    help();
main(["--v"]) ->
    io:format("------------------------------\n"),
    io:format("|OpenVM Compile Version 0.0.1|\n"),
    io:format("------------------------------\n");

main(Args) ->
    [FilePath | _] = Args,
    io:format("> Compile file:~p\n", [FilePath]),
    read_file(FilePath).

help() ->
    io:format("Help:\n    vmcc [filename]\n"),
    halt(1).