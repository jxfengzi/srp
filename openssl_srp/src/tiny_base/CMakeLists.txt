#-----------------------------------------------------------------------------
#
# tiny_base
#
#-----------------------------------------------------------------------------

SET(Base_Header
    tiny_api.h
    tiny_base.h
    tiny_define.h
    tiny_debug.h
    tiny_ret.h
    tiny_time.h
    tiny_typedef.h
    )

SET(Base_Source
    tiny_ret.c
    tiny_time.c
    )

SOURCE_GROUP(TinyBase\\headers              FILES       ${Base_Header})
SOURCE_GROUP(TinyBase\\sources              FILES       ${Base_Source})

#----------------------------------------------------------------------------
# source
#----------------------------------------------------------------------------
SET(SRC
    ${Base_Header}
    ${Base_Source}
    )

#----------------------------------------------------------------------------
# static library
#----------------------------------------------------------------------------
ADD_LIBRARY(tinybase_static STATIC ${SRC})
SET_TARGET_PROPERTIES(tinybase_static PROPERTIES OUTPUT_NAME "tinybase_static")
