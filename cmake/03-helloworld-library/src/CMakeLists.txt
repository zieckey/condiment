message(STATUS "This is SOURCE dir " ${CMAKE_SOURCE_DIR})
SET(MYINCLUDEDIR )
message(STATUS "This is MYINCLUDEDIR " ${MYINCLUDEDIR})
include_directories(${CMAKE_SOURCE_DIR}/lib)
add_executable(main main.c)
target_link_libraries(main hello)

add_executable(main_static main.c)
target_link_libraries(main_static hello_static)
