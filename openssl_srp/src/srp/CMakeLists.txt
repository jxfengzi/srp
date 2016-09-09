#-----------------------------------------------------------------------------
#
# srp
#
#-----------------------------------------------------------------------------

SET(SRC_Header
    srp_api.h
    SrpClient.h
    SrpServer.h
    )

SET(SRC_Source
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
ADD_LIBRARY(src_shared SHARED ${SRC})
SET_TARGET_PROPERTIES(src_shared PROPERTIES OUTPUT_NAME "srp")
SET_TARGET_PROPERTIES(src_shared PROPERTIES LINKER_LANGUAGE C)
TARGET_LINK_LIBRARIES(src_shared ${OPENSSL_LIB} ${OS_LIB})
