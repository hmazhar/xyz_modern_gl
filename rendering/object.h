/*	
Based on code provided by: Perry Kivolowitz
Class for renderable objects
Inherits from basicGL class
Each object contains a pointer to a shader and an image asset, these do not necessarily need to be used.

*/

#pragma once
#include "basicgl.h"
#include "shader.h"
#include "vertexattributes.h"
#include "material.h"
class Object:public BasicGL
{
public:
	Object();
	virtual ~Object();
	virtual void TakeDown();
	virtual bool Initialize();
	virtual bool PostGLInitialize(GLuint * vertex_array_handle, GLuint * vertex_coordinate_handle, GLsizeiptr sz, const GLvoid * ptr);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview) = 0;
	void AttachShader(Shader * new_shader);

protected:
	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;
	std::vector<GLuint> vertex_indices;
	Shader * shader;

	bool using_texture;

private:
	void InternalInitialize();
};
