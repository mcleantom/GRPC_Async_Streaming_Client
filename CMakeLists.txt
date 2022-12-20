cmake_minimum_required(VERSION 3.10.0)
project(gRPCAsyncStreamingClient VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party)
add_subdirectory(proto)
add_subdirectory(client)