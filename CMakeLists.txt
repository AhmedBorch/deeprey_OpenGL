cmake_minimum_required(VERSION 3.10)
project(wxGL_deeprey)
set(CMAKE_BUILD_TYPE Debug)

# set(wxWidgets_ROOT_DIR "C:/wxWidgets-3.2.7")
# set(wxWidgets_CONFIGURATION mswu)  # or "vc_x64_lib/mswu" for VS

find_package(wxWidgets REQUIRED COMPONENTS core base gl)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
include(${wxWidgets_USE_FILE})

# Set include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${wxWidgets_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/MyApp.cpp
    src/MyFrame.cpp
    src/MyGLCanvas.cpp
    src/ControlPanel.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} opengl32)