cmake_minimum_required(VERSION 3.18)
project(SimOhenServer C)

set(CMAKE_CXX_STANDARD 17)

add_executable(SimOhenServer
        Server/main.c
        Server/buffer.h
        Server/passive_socket.c
        Server/passive_socket.h
        Server/char_buffer.c
        Server/char_buffer.h
        Server/active_socket.c
        Server/active_socket.h
        Server/linked_list.h)
