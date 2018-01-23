CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(OPENGL_TEST)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)
FIND_PACKAGE(assimp REQUIRED)

SET(LINK_LIBS ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} glfw ${ASSIMP_LIBRARIES})

SET(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_FLAGS "-std=c++11")

SET(CMAKE_BUILD_TYPE Release)

LINK_DIRECTORIES (
    "/usr/lib/"
    "/usr/local/lib/"
    "/usr/local/Cellar/glew/2.1.0/lib/"
    "/usr/local/Cellar/glfw/3.2.1/lib/"
    "/usr/local/Cellar/assimp/3.2/lib/"
    )

INCLUDE_DIRECTORIES ("./include"
                    ${GLEW_INCLUDE_DIRS}
                    ${GLFW_INCLUDE_DIRS}
                    ${ASSIMP_INCLUDE_DIRS}
                    )

ADD_SUBDIRECTORY(./src)
ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES(main glutils ${LINK_LIBS})

