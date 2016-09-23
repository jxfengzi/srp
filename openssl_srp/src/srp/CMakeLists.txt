#-----------------------------------------------------------------------------
#
# srp
#
#-----------------------------------------------------------------------------

SET(SRC_Header
    srp_api.h
    SrpCommon.h
    SrpClient.h
    SrpServer.h
    )

SET(SRC_Source
    SrpCommon.c
    SrpClient.c
    SrpServer.c
    )

#----------------------------------------------------------------------------
# source
#----------------------------------------------------------------------------
SET(SRC
    ${SRC_Header}
    ${SRC_Source}
    )

#----------------------------------------------------------------------------
# shared library
#----------------------------------------------------------------------------
ADD_LIBRARY(srp_shared SHARED ${SRC})
SET_TARGET_PROPERTIES(srp_shared PROPERTIES OUTPUT_NAME "srp")
SET_TARGET_PROPERTIES(srp_shared PROPERTIES LINKER_LANGUAGE C)
TARGET_LINK_LIBRARIES(srp_shared ${OPENSSL_LIB} ${OS_LIB} tiny_static)

#-----------------------------------------------------------------------------
#
# test
#
#-----------------------------------------------------------------------------
ADD_EXECUTABLE(test_srp test.c)
SET_TARGET_PROPERTIES(test_srp PROPERTIES LINKER_LANGUAGE C)
TARGET_LINK_LIBRARIES(test_srp srp_shared ${OS_LIB})
