set (net_SRCS
    inetaddress.cpp
    socket_ops.cpp
    socket.cpp
    )

include_directories(../base)

add_subdirectory(../base base.out)
#file(GLOB net_srcs "./*cpp")
aux_source_directory(. CUR_SRC)

add_library(mnet ${CUR_SRC})
target_link_libraries(mnet mbase)
#install(TARGETS mnet DESTINATION  lib)

set(HEADERS
    socket.h
    endian.h
    inetaddress.h
    socket_ops.h)
install(FILES ${HEADERS} DESTINATION include/webtest/net)

