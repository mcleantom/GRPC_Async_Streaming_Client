cmake_minimum_required(VERSION 3.0.0)
project(gRPCAsyncStreamingClient VERSION 0.1.0)

add_library(gRPCAsyncStreamingClient gRPCAsyncStreamingClient.cpp)
add_subdirectory(proto)