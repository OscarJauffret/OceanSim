cmake_minimum_required(VERSION 3.30)
project(OceanSim)

# Use C++20 as you requested
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Tell CMake to look in Homebrew's directory for libraries
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

# 2. Find the required graphics libraries
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
# GLM is header-only, but we still find it to get the include paths
find_package(glm REQUIRED)

# 3. Define the executable (assumes main.cpp is in the same folder)
add_executable(OceanSim
        src/main.cpp
        src/shader.cpp
        src/window.cpp
        src/mesh.cpp
        )

# 4. Link the libraries
# On Mac, we also need to link the system's OpenGL and App frameworks
target_link_libraries(OceanSim
        PRIVATE
        glfw
        GLEW::GLEW
        glm::glm
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
)

# 5. Include directories (ensures headers are found)
target_include_directories(OceanSim PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        "/opt/homebrew/include")