cmake_minimum_required(VERSION 3.22)
# 设定项目名称
project(dots_and_boxes)
# 设定语言标准
set(CMAKE_CXX_STANDARD 17)
# 定义第三方库路径
set(SFML_DIR 3rdparty/SFML/lib/cmake/SFML)
# 查找第三方库
find_package(SFML 2.5 COMPONENTS
        system window graphics REQUIRED)
# 声明 SOURCES
file(GLOB_RECURSE SOURCES "src/*.cpp")
# 如果想要编译出来的程序不带控制台，使用这个
#add_executable(dots_and_boxes WIN32 ${SOURCES})
# 如果想要编译出来的程序带控制台，使用这个
add_executable(dots_and_boxes ${SOURCES})
# 链接头文件查找目录
target_include_directories(dots_and_boxes PUBLIC 3rdparty/SFML/include)
#如需静态链接
#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
# 链接库 如需静态链接在PRIVATE后加上-static
target_link_libraries(dots_and_boxes PRIVATE sfml-system sfml-window sfml-graphics)
# 复制dll
file(GLOB_RECURSE PROJECT_DLL 3rdparty/*.dll)
add_custom_command(TARGET dots_and_boxes POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_DLL}
        $<TARGET_FILE_DIR:dots_and_boxes>
        )
# 复制资源文件
add_custom_command(TARGET dots_and_boxes POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:dots_and_boxes>/res
        )
