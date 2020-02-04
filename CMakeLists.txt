project(test)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Protobuf REQUIRED)


include_directories(
        ${PROTOBUF_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/base
        ${PROJECT_SOURCE_DIR}/net
)
add_subdirectory(net)
add_subdirectory(base)



protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/test.proto)

add_executable(test main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(test ${PROTOBUF_LIBRARIES})
target_link_libraries(test mnet)
