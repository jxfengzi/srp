ADD_EXECUTABLE(test_cetty test.c)

TARGET_LINK_LIBRARIES(test_cetty
                                 sc_cetty_static
                                 sc_property_static
                                 sc_container_static 
                                 sc_memory_static 
                                 sc_common_static
                                 ${OS_LIB})
