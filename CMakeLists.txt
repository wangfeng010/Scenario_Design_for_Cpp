cmake_minimum_required(VERSION 3.20)
project(scenario_design_for_cpp)

include_directories(include)

# 自动收集src目录下的所有源文件到变量SOURCE_FILES中
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# 创建一个可执行文件
add_executable(scenario_design_for_cpp ${SOURCE_FILES})

# 包含第三方库
target_link_libraries(scenario_design_for_cpp
PUBLIC
easyx
)
