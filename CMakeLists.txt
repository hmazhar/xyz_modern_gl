cmake_minimum_required(VERSION 2.8)
project(xyz_display)

# Additional packages
  
find_package(GLUT)
find_package(OpenGL)
find_package(GLEW)
find_package(OPENMP)
#find_package(GLM)
find_package(ARMADILLO)
find_package(PCL)
include_directories( ${FREEGLUT_INC_PATH})
include_directories( ${GLEW_INCLUDE_DIRS} )
include_directories( ${GLM_INCLUDE_DIRS} )
include_directories( ${ARMADILLO_INCLUDE_DIRS})
include_directories(${PCL_INCLUDE_DIRS})
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ")

link_directories(${PCL_LIBRARY_DIRS})


# Add executables

SET(CODE_FILES 
xyz_io/xyz_io.cpp
rendering/camera.cpp
rendering/object.cpp
rendering/shader.cpp
rendering/cloud.cpp
rendering/vertexattributes.cpp
)
add_definitions(${PCL_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")

    ADD_EXECUTABLE(xyz_display main.cpp  ${CODE_FILES} )
		
    TARGET_LINK_LIBRARIES(xyz_display	
	${FREEGLUT_LIB}
	${GLUT_LIBRARY}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	#${GLFW_LIBRARIES}
	${ARMADILLO_LIBRARIES}
	 ${PCL_LIBRARIES}
	)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

