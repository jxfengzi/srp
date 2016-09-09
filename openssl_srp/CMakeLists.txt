CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(SrpKit C)

SET(PROJ_NAME "SrpKit")

#---------------------------------------------------------------------------------------
#
# c flags
#
#---------------------------------------------------------------------------------------
ADD_DEFINITIONS(-DTINY_DEBUG)

#ADD_DEFINITIONS(-DDSO_DLFCN)
#ADD_DEFINITIONS(-DHAVE_DLFCN_H)
#ADD_DEFINITIONS(-DNDEBUG)
#ADD_DEFINITIONS(-DOPENSSL_THREADS)
#ADD_DEFINITIONS(-DOPENSSL_NO_STATIC_ENGINE)
#ADD_DEFINITIONS(-DOPENSSL_PIC)
#ADD_DEFINITIONS(-DOPENSSL_IA32_SSE2)
#ADD_DEFINITIONS(-DOPENSSL_BN_ASM_MONT)
#ADD_DEFINITIONS(-DOPENSSL_BN_ASM_MONT5)
#ADD_DEFINITIONS(-DOPENSSL_BN_ASM_GF2m)
#ADD_DEFINITIONS(-DSHA1_ASM)
#ADD_DEFINITIONS(-DSHA256_ASM)
#ADD_DEFINITIONS(-DSHA512_ASM)
#ADD_DEFINITIONS(-DRC4_ASM)
#ADD_DEFINITIONS(-DMD5_ASM)
#ADD_DEFINITIONS(-DAES_ASM)
#ADD_DEFINITIONS(-DVPAES_ASM)
#ADD_DEFINITIONS(-DBSAES_ASM)
#ADD_DEFINITIONS(-DGHASH_ASM)
#ADD_DEFINITIONS(-DECP_NISTZ256_ASM)
#ADD_DEFINITIONS(-DPOLY1305_ASM)
#ADD_DEFINITIONS(-DOPENSSLDIR="\"/usr/local/ssl\"")
#ADD_DEFINITIONS(-DENGINESDIR="\"/usr/local/lib/engines-1.1\"")
#ADD_DEFINITIONS(-Wall) 
#ADD_DEFINITIONS(-O3)
#ADD_DEFINITIONS(-pthread)
#ADD_DEFINITIONS(-m64)
#ADD_DEFINITIONS(-DL_ENDIAN)
#ADD_DEFINITIONS(-Wa, --noexecstack)
#ADD_DEFINITIONS(-MMD)
#ADD_DEFINITIONS(-MF)

#---------------------------------------------------------------------------------------
# WIN32
#---------------------------------------------------------------------------------------
IF(WIN32)

    MESSAGE(STATUS "build ${PROJ_NAME}for Windows")

    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_UNICODE)

ELSE(WIN32)

    ADD_DEFINITIONS(-Wno-multichar)

    # release
    ADD_DEFINITIONS(-O3)

    # debug
    #ADD_DEFINITIONS(-g)

    ADD_DEFINITIONS(-fPIC)

    #-----------------------------------------------------------------------------------
    # APPLE MACOSX
    #-----------------------------------------------------------------------------------
    IF(APPLE)
        MESSAGE(STATUS "build ${PROJ_NAME} for osx")
        ADD_DEFINITIONS(-D__MAC_OSX__)

        # Apple LLVM Compiler
        # SET(CMAKE_CXX_COMPILER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++)

        # LLVM gcc Compiler
        SET(CMAKE_CXX_COMPILER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++)
        SET(CMAKE_CXX_FLAGS -m64)

        #SET(CMAKE_CXX_COMPILER g++)
        #SET(CMAKE_CXX_FLAGS -m64)

    #-----------------------------------------------------------------------------------
    # Linux
    #-----------------------------------------------------------------------------------
    ELSE(APPLE)
        MESSAGE(STATUS "build ${PROJ_NAME} for Linux")
        ADD_DEFINITIONS(-D__LINUX__)
    ENDIF(APPLE)
ENDIF(WIN32)

#---------------------------------------------------------------------------------------
#
# include dir
#
#---------------------------------------------------------------------------------------
SET(OPENSSL_INC ${PROJECT_SOURCE_DIR}/../../openssl/include)
MESSAGE(STATUS "openssl_inc: ${OPENSSL_INC}")
INCLUDE_DIRECTORIES(${OPENSSL_INC})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/tiny_base)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/srp)

#--------------------------------------------------------------------------------------
#
# lib
#
#--------------------------------------------------------------------------------------
IF (WIN32)
    SET(OS_LIB)
    SET(OPENSSL_LIB crypto.lib ssl.lib)
ELSE (WIN32)
    SET(OS_LIB dl)
    SET(OPENSSL_LIB ${PROJECT_SOURCE_DIR}/libs/x86_64_linux/libcrypto.a ${PROJECT_SOURCE_DIR}/libs/x86_64_linux/libssl.a)
ENDIF (WIN32)

MESSAGE(STATUS "os_lib: ${OS_LIB}")
MESSAGE(STATUS "openssl_lib: ${OPENSSL_LIB}")

#-------------------------------------------------------------------------------------
# set output dir
#-------------------------------------------------------------------------------------
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
MESSAGE(STATUS "executable output:  ${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "library output: ${LIBRARY_OUTPUT_PATH}")

#-------------------------------------------------------------------------------------
#
# project
#
#-------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(src)
