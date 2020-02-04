set (base_SRCS
    date.cpp
    )

add_library(mbase ${base_SRCS})
target_link_libraries(mbase pthread)
