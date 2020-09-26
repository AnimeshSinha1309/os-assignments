cmake_minimum_required(VERSION 3.16)
project(anishell C)

set(CMAKE_C_STANDARD 11)

add_executable(anishell main.c
        utils/string.c utils/string.h
        utils/strmat.c utils/strmat.h
        utils/io.c utils/io.h
        utils/sysinfo.c utils/sysinfo.h
        utils/terminate.c utils/terminate.h
        commands/cd.c commands/cd.h
        commands/echo.c commands/echo.h
        commands/pwd.c commands/pwd.h
        commands/ls.c commands/ls.h
        commands/pinfo.c commands/pinfo.h
        commands/execvp.c commands/execvp.h
        commands/history.c commands/history.h
        commands/nightswatch.c commands/nightswatch.h
        commands/syscom.c commands/syscom.h
        processor/prompt.c processor/prompt.h
        processor/parser.c processor/parser.h
        processor/tokenizer.c processor/tokenizer.h
        processor/pipeline.c processor/pipeline.h
        globals.h utils/joblist.c utils/joblist.h)

target_link_libraries(anishell m)
