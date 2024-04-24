cmake_minimum_required(VERSION 3.20)
project(scenario_design_for_cpp)

include_directories(include)

# 自动收集src目录下的所有源文件到变量SOURCE_FILES中
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# 创建一个可执行文件
add_executable(scenario_design_for_cpp ${SOURCE_FILES})

# 设置变量，指向easyx.lib的路径
set(EASYX_LIB_PATH "D:/codes/ku/easyx/lib64")

# 包含EasyX的头文件目录
include_directories("D:/codes/ku/easyx/include")

# 包含第三方库
#[[target_link_libraries(scenario_design_for_cpp
PUBLIC
easyx
)]]

# 将easyx.lib库链接到项目中
target_link_libraries(${PROJECT_NAME} "${EASYX_LIB_PATH}/libeasyx.a")
