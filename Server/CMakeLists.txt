cmake_minimum_required(VERSION 3.18)
project(Server C)

set(CMAKE_C_STANDARD 11)

add_executable(Server main.c buffer.h
        active_socket.c active_socket.h
        char_buffer.c char_buffer.h
        passive_socket.c passive_socket.h
        linked_list.h)
