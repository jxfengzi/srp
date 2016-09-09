#-----------------------------------------------------------------------------
#
# Tiny
#
#-----------------------------------------------------------------------------

#-----------------------
# Base
#-----------------------
SET(Base_Header
    Base/tiny_api.h
    Base/tiny_base.h
    Base/tiny_define.h
    Base/tiny_debug.h
    Base/tiny_ret.h
    Base/tiny_time.h
    Base/tiny_typedef.h
    )

SET(Base_Source
    Base/tiny_ret.c
    Base/tiny_time.c
    )

SOURCE_GROUP(TinyBase\\headers              FILES       ${Base_Header})
SOURCE_GROUP(TinyBase\\sources              FILES       ${Base_Source})

#-----------------------
# Log
#-----------------------
SET(Log_Header
    Log/tiny_log.h
    Log/tiny_log_print.h
    )

SET(Log_Source
    Log/tiny_log_print.c
    )

SOURCE_GROUP(TinyLog\\headers               FILES       ${Log_Header})
SOURCE_GROUP(TinyLog\\sources               FILES       ${Log_Source})


#----------------------------------------------------------------------------
# source
#----------------------------------------------------------------------------
SET(SRC
    ${Base_Header}
    ${Base_Source}
    ${Log_Header}
    ${Log_Source}
    )

#----------------------------------------------------------------------------
# static library
#----------------------------------------------------------------------------
ADD_LIBRARY(tiny_static STATIC ${SRC})
SET_TARGET_PROPERTIES(tiny_static PROPERTIES OUTPUT_NAME "tiny_static")
SET_TARGET_PROPERTIES(tiny_static PROPERTIES LINKER_LANGUAGE C)
