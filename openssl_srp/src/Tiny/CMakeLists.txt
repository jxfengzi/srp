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

#-----------------------
# Memory
#-----------------------
SET(Memory_Header
    Memory/tiny_memory.h
    )

SET(Memory_Source
    Memory/tiny_memory.c
    )

SOURCE_GROUP(TinyMemory\\headers            FILES       ${Memory_Header})
SOURCE_GROUP(TinyMemory\\sources            FILES       ${Memory_Source})

#-----------------------
# Container
#-----------------------
SET(Container_Header
    Container/TinyContainerListener.h
    Container/TinyList.h
    Container/TinyMap.h
    Container/TinyQueue.h)

SET(Container_Source
    Container/TinyList.c
    Container/TinyMap.c
    Container/TinyQueue.c)

SOURCE_GROUP(TinyContainer\\headers         FILES   ${Container_Header})
SOURCE_GROUP(TinyContainer\\sources         FILES   ${Container_Source})


#----------------------------------------------------------------------------
# source
#----------------------------------------------------------------------------
SET(SRC
    ${Base_Header}
    ${Base_Source}
    ${Log_Header}
    ${Log_Source}
    ${Container_Header}
    ${Container_Source}
    ${Memory_Header}
    ${Memory_Source}
    )

#----------------------------------------------------------------------------
# static library
#----------------------------------------------------------------------------
ADD_LIBRARY(tiny_static STATIC ${SRC})
SET_TARGET_PROPERTIES(tiny_static PROPERTIES OUTPUT_NAME "tiny_static")
SET_TARGET_PROPERTIES(tiny_static PROPERTIES LINKER_LANGUAGE C)
