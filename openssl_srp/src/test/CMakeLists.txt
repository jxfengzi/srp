#-----------------------------------------------------------------------------
#
# test
#
#-----------------------------------------------------------------------------
SET(SRC srptest.c)
ADD_EXECUTABLE(srptest ${SRC})
SET_TARGET_PROPERTIES(srptest PROPERTIES LINKER_LANGUAGE C)
#TARGET_LINK_LIBRARIES(srptest ${OS_LIB} ${OPENSSL_LIB})
TARGET_LINK_LIBRARIES(srptest ${OPENSSL_LIB} ${OS_LIB})
