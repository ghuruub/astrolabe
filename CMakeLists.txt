cmake_minimum_required(VERSION 3.12)
project(astrolabe)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat -Wno-c++98-compat-pedantic")
endif()

set(IMGUI_DIR lib/imgui/)

include_directories(lib/imgui/)
include_directories(lib/imgui/backends/)

add_executable(lightning
  src/main.cpp

  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

include_directories(include/)
include_directories(lib/stb/)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLLW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILDS_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glad EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)


target_link_libraries(lightning
  PRIVATE glfw
  PRIVATE glad
  PRIVATE glm::glm
)

target_link_libraries(lightning
  PRIVATE "-framework Cocoa"
  PRIVATE "-framework IOKit"
  PRIVATE "-framework CoreVideo"
)

target_link_directories(lightning
    PRIVATE lib/glad/src
)

target_include_directories(lightning
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)