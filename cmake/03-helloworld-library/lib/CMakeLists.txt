add_library(hello SHARED hello.c)
add_library(hello_static STATIC hello.c)
set_target_properties(hello_static PROPERTIES OUTPUT_NAME "hello")


# ----------------------------------------------------------------------------
# package information
set (PACKAGE_NAME        "safe-hello")
set (PACKAGE_VERSION     "1.0.0")
set (PACKAGE_STRING      "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set (PACKAGE_TARNAME     "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set (PACKAGE_BUGREPORT   "https://github.com/Qihoo360/evpp/issues")
set (PACKAGE_DESCRIPTION "evpp is a modern C++ network library for developing high performance network servers in TCP/UDP/HTTP protocols.")
set (PACKAGE_URL         "http://github.com/Qihoo360/evpp")

set (PACKAGE_VERSION_MAJOR 1)
set (PACKAGE_VERSION_MINOR 0)
set (PACKAGE_VERSION_PATCH 0)
set (PACKAGE_SOVERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}")

set (CPACK_GENERATOR "RPM;TGZ")

set (CPACK_INSTALL_PREFIX "/home/weizili/hello")
set (CPACK_PACKAGING_INSTALL_PREFIX ${CPACK_INSTALL_PREFIX})

include (CPack)

install (TARGETS hello DESTINATION "lib")
install (FILES hello.h DESTINATION "include")

