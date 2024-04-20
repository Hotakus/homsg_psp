# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. LIB_SRCS)
include_directories(include LIB_INC)
include_directories(. LIB_INC)

add_library(homsg_psp src/homsg_psp.c src/chain/chain.c)
#target_link_libraries(homsg_psp chain)
