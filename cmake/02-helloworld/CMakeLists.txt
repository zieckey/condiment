project (HELLO)
message(status "This is BINARY dir " ${HELLO_BINARY_DIR})
message(status "This is SOURCE dir " ${HELLO_SOURCE_DIR})
add_subdirectory(src bin)
