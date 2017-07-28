cmake_minimum_required (VERSION 2.8)

project (libuv-examples C CXX)

if (WIN32)
	set (C_FLAGS -DWIN32 -D_WIN32 -EHsc)
    set(VcpkgRoot ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
    include_directories(${VcpkgRoot}/include
                        ${PROJECT_SOURCE_DIR}
                        ${PROJECT_SOURCE_DIR}/3rdparty/wininclude)

    # On windows, Debug mode
    link_directories(${VcpkgRoot}/debug/lib ${VcpkgRoot}/debug/lib/manual-link)

    # On Windows, Release mode
    # link_directories(${VcpkgRoot}/lib ${VcpkgRoot}/lib/manual-link)
endif ()

add_executable(libuv_example_cgi cgi/main.c cgi/tick.c)
target_link_libraries(libuv_example_cgi libuv)

add_executable(libuv_example_detach detach/main.c)
target_link_libraries(libuv_example_detach libuv)

add_executable(libuv_example_dns dns/main.c)
target_link_libraries(libuv_example_dns libuv)

add_executable(libuv_example_helloworld helloworld/main.c)
target_link_libraries(libuv_example_helloworld libuv)

add_executable(libuv_example_idle-basic idle-basic/main.c)
target_link_libraries(libuv_example_idle-basic libuv)

add_executable(libuv_example_idle-compute idle-compute/main.c)
target_link_libraries(libuv_example_idle-compute libuv)

add_executable(libuv_example_interfaces interfaces/main.c)
target_link_libraries(libuv_example_interfaces libuv)

add_executable(libuv_example_locks locks/main.c)
target_link_libraries(libuv_example_locks libuv)

add_executable(libuv_example_multi-echo-server multi-echo-server/main.c multi-echo-server/worker.c)
target_link_libraries(libuv_example_multi-echo-server libuv)

add_executable(libuv_example_onchange onchange/main.c)
target_link_libraries(libuv_example_onchange libuv)

add_executable(libuv_example_pipe-echo-server pipe-echo-server/main.c)
target_link_libraries(libuv_example_pipe-echo-server libuv)

add_executable(libuv_example_plugin plugin/main.c plugin/hello.c)
target_link_libraries(libuv_example_plugin libuv)

add_executable(libuv_example_proc-streams proc-streams/main.c proc-streams/test.c)
target_link_libraries(libuv_example_proc-streams libuv)

add_executable(libuv_example_progress progress/main.c)
target_link_libraries(libuv_example_progress libuv)

add_executable(libuv_example_tcp-echo-server tcp-echo-server/main.c)
target_link_libraries(libuv_example_tcp-echo-server libuv)

#queue-cancel/main.c
#queue-work/main.c
#ref-timer/main.c
#signal/main.c
#spawn/main.c
#thread-create/main.c
#tty-gravity/main.c
#tty/main.c
#udp-dhcp/main.c
#uvcat/main.c
#uvstop/main.c
#uvtee/main.c
#uvwget/main.c
