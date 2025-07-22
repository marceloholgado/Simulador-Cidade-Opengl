cmake_minimum_required(VERSION 3.10)
project(SimuladorCidade)

set(CMAKE_CXX_STANDARD 11)
set(GLUT_ROOT "C:/MinGW/freeglut") # freeglut 3.0.0 MinGW Package at https://www.transmissionzero.co.uk/software/freeglut-devel/
set(GLUT_INCLUDE_DIR "${GLUT_ROOT}/include")
set(GLUT_glut_LIBRARY "${GLUT_ROOT}/lib/libfreeglut.a")

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${GLUT_INCLUDE_DIR})

# Add all source files
file(GLOB SRC_FILES src/*.cpp SOIL/*.cpp)

add_executable(SimuladorCidade ${SRC_FILES})

# Find OpenGL and GLUT
find_package(OpenGL REQUIRED)

target_link_libraries(SimuladorCidade PRIVATE ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY} glu32 opengl32)