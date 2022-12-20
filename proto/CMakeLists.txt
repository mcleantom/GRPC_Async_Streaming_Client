cmake_minimum_required(VERSION 3.16)
project(ProtoObjects)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads)

set(
    PROTO_FILES
        ProtoObjects/helloworld.proto
)

add_library(
    ProtoObjects ${PROTO_FILES}
)

target_link_libraries(
    ProtoObjects
        PUBLIC
            protobuf::libprotobuf
            gRPC::grpc
            gRPC::grpc++
)
target_include_directories(
    ProtoObjects PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET ProtoObjects LANGUAGE cpp)
protobuf_generate(TARGET ProtoObjects LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")