cmake_minimum_required(VERSION 3.10)
project (bump C CXX )

if(APPLE)
    enable_language(OBJC)
    enable_language(OBJCXX)
endif()

if (NOT HDLL_DESTINATION) 
    set(HDLL_DESTINATION "/usr/local/lib")
endif()

if (NOT HL_INCLUDE_DIR) 
    set(HL_INCLUDE_DIR "/usr/local/include")
endif()

if (NOT COMMON_LIB_DIR) 
    set(COMMON_LIB_DIR "/usr/local/lib")
endif()

#### Project definitions



add_library(bump.hdll SHARED
#src/hl-idl-helpers.cpp
src/bump.cpp
)

#set_source_files_properties(${FORGE_RENDERER_SRCS} ${FORGE_MW_SRCS} ${FORGE_OS_SRCS} src/forge.cpp src/hl-forge.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")


set_target_properties(bump.hdll
PROPERTIES
PREFIX ""
OUTPUT_NAME bump
SUFFIX .hdll
)

set_property(TARGET bump.hdll PROPERTY CXX_STANDARD 17)

cmake_policy(SET CMP0015 NEW)

target_include_directories(bump.hdll
PRIVATE
${HL_INCLUDE_DIR}
)


if (UNIX)
    # Some special flags are needed for GNU GCC compiler
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC -fpermissive")
    #not sure why the ${HL_LIB_DIR} is necessary given the above.
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -shared  ")

    find_library(LIBHL NAMES hl HINTS ${HL_LIB_DIR} )
else()
    find_library(LIBHL NAMES libhl HINTS ${HL_LIB_DIR} )
endif (UNIX)

set_property (TARGET bump.hdll APPEND_STRING PROPERTY COMPILE_FLAGS )
 
target_link_libraries(bump.hdll ${LIBHL})

install(TARGETS bump.hdll DESTINATION ${HDLL_DESTINATION})
