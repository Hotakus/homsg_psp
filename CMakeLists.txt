cmake_minimum_required(VERSION 3.20)
project(homsg_psp
        LANGUAGES C
        VERSION 1.0.0.0
        DESCRIPTION "Publish-Subscribe Pattern"
        HOMEPAGE_URL https://github.com/Hotakus/hhtl
)
set(CMAKE_C_STANDARD 99)

# 指定版本号的配置文件
configure_file(${PROJECT_SOURCE_DIR}/version.h.in ${PROJECT_SOURCE_DIR}/version.h)

#设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib) # 设置静态库文件目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib) # 动态库文件目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin) # 可执行文件目录

#设置编译、链接参数等
#set(LINK_FLAGS      "${LINK_FLAGS} -Wl,-Map=object.map,--cref,--gc-section")
#set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fdata-sections -ffunction-sections -Wall")

# main add
add_definitions(-DDUMMY_DEFINE)
add_compile_options(-fdata-sections -ffunction-sections -Wall)
add_link_options(-Wl,-Map=object.map,--cref,--gc-section)

include_directories(./inc ./)
include_directories(./src/)
include_directories(./src/chain)

#file(GLOB_RECURSE SOURCES "./*.c" "./*.h")
add_subdirectory(./src/chain)

#message(WARNING     "${LINK_FLAGS}")
#message(WARNING     "${CMAKE_C_FLAGS}")

#add_executable(homsg_psp src/homsg_psp.c)
add_library(homsg_psp src/homsg_psp.c)
target_link_libraries(homsg_psp chain)
