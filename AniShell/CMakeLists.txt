cmake_minimum_required(VERSION 3.16)
project(AniShell C)

set(CMAKE_C_STANDARD 11)

add_executable(AniShell main.c

        utils/string.c utils/string.h
        utils/sysinfo.c utils/sysinfo.h

        commands/ls.c commands/ls.h
        commands/pinfo.c commands/pinfo.h
        commands/cd.c commands/cd.h
        commands/pwd.c commands/pwd.h

        processor/prompt.c processor/prompt.h
        processor/parser.c processor/parser.h
        processor/tokenizer.c processor/tokenizer.h

        globals.h commands/execvp.c commands/execvp.h utils/strmat.c utils/strmat.h)

target_link_libraries(AniShell m)
