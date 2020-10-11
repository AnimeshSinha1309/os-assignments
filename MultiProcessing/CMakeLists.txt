cmake_minimum_required(VERSION 3.16)
project(MultiProcessing)

set(CMAKE_CXX_STANDARD 14)

add_executable(q1
        q1/main.c
        q1/normal.h q1/normal.c
        q1/processes.h q1/processes.c
        q1/threads.h q1/threads.c
        q1/functions.h q1/functions.c)
target_link_libraries(q1 m pthread)

add_executable(q2
        q2/main.c
        q2/company.c q2/company.h
        q2/zone.c q2/zone.h
        q2/student.c q2/student.h
        q2/functions.c q2/functions.h)
target_link_libraries(q2 m pthread)

add_executable(q3 q3/main.c)
target_link_libraries(q3 m pthread)
