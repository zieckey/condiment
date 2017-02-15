cmake_minimum_required(VERSION 2.8)

project (safe-hello)

message(STATUS "This is BINARY dir " ${CMAKE_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${CMAKE_SOURCE_DIR})

add_subdirectory(lib)
add_subdirectory(src bin)

# ----------------------------------------------------------------------------
# package information
set (CPACK_PACKAGE_VERSION_MAJOR    1)
set (CPACK_PACKAGE_VERSION_MINOR    2)
set (CPACK_PACKAGE_VERSION_PATCH    3)
set (CPACK_PACKAGE_CONTACT          "weizili@360.cn")
set (CPACK_GENERATOR                "RPM;TGZ;DEB")
set (CPACK_PACKAGING_INSTALL_PREFIX "/home/weizili/hello")

include (CPack)

