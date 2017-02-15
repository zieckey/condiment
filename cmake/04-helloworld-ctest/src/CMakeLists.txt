message(STATUS "This is SOURCE dir " ${CMAKE_SOURCE_DIR})
message(STATUS "This is MYINCLUDEDIR " ${MYINCLUDEDIR})
include_directories(${CMAKE_SOURCE_DIR}/lib)
add_executable(main main.c)
target_link_libraries(main hello)

add_executable(main_static main.c)
target_link_libraries(main_static hello_static)

include(CTest)
add_test(NAME main COMMAND main)
add_test(NAME main_static COMMAND main_static)
