/*
Common include class
provides basic includes used elsehwere
Defines bad gl value if not already defined
also provides usefull constants
*/

#ifndef INCLUDE_H
#define INCLUDE_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/constants.hpp>


//Define BAD_GL_VALUE to the maximum possible unsigned integer
#ifndef	BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif // !BAD_GL_VALUE


//Constants
#define H_PI 3.14159265359f
#define H_PI_2 M_PI/2.0

#endif
