cmake_minimum_required(VERSION 3.10)
project(wxGL_deeprey)

find_package(wxWidgets REQUIRED COMPONENTS core base gl)
find_package(OpenGL REQUIRED)
include(${wxWidgets_USE_FILE})

# Set include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${wxWidgets_INCLUDE_DIRS}
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
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES})