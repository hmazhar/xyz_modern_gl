/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#include "shader.h"
#include <assert.h>

using namespace std;
using namespace glm;

#define BAD_GL_VALUE GLuint(-1)

Shader::Shader()
{
	this->vertex_shader_id = BAD_GL_VALUE;
	this->fragment_shader_id = BAD_GL_VALUE;
	this->program_id = BAD_GL_VALUE;
	this->modelview_matrix_handle = BAD_GL_VALUE;
	this->projection_matrix_handle = BAD_GL_VALUE;
	this->normal_matrix_handle = BAD_GL_VALUE;
	this->use_texture_handle = BAD_GL_VALUE;
	this->time_handle = BAD_GL_VALUE;
	time = 0;

}

/*	This Shader() class implements or assumes a basic set of uniforms will be
	provided to all shaders derived from it. These are listed below. 
	Shader::CommonSetup() can be used by call derived classes to send the 
	common values to the shader. Values unique to the derived class can be
	loaded with the CustomShader() function.
*/

void Shader::CommonSetup(const GLfloat * projection, const GLfloat * modelview, const GLfloat * mvp, const GLfloat * nm)
{
	if (this->projection_matrix_handle != BAD_GL_VALUE)
		glUniformMatrix4fv(this->projection_matrix_handle, 1, GL_FALSE, projection);
	this->GLReturnedError("Draw - after projection_matrix_handle");
	if (this->modelview_matrix_handle != BAD_GL_VALUE)
		glUniformMatrix4fv(this->modelview_matrix_handle, 1, GL_FALSE, modelview);
	this->GLReturnedError("Draw - after modelview_matrix_handle");
	if (this->mvp_handle != BAD_GL_VALUE)
		glUniformMatrix4fv(this->mvp_handle, 1, GL_FALSE, mvp);
	this->GLReturnedError("Draw - after mvp_handle");
	if (this->normal_matrix_handle != BAD_GL_VALUE)
		glUniformMatrix3fv(this->normal_matrix_handle, 1, GL_FALSE, nm);
	this->GLReturnedError("Draw - after normal_matrix_handle");
}
//Turn the texture use flag on or off
void Shader::ToggleTexture(bool use_texture)
{
	if (this->use_texture_handle != BAD_GL_VALUE)
		glUniform1i(this->use_texture_handle,  use_texture);
	this->GLReturnedError("Draw - after use_texture_handle");
}

void Shader::SetTime(float _time)
{
	time = _time;
}

void Shader::Use()
{
	assert(this->program_id != BAD_GL_VALUE);
	glUseProgram(this->program_id);

	if (this->time_handle != BAD_GL_VALUE)
		glUniform1f(this->time_handle,  time);
	this->GLReturnedError("Draw - after time_handle");


}

/*	The shader initialization code is lifted liberally from the GLSL 4.0 Cookbook.
	Added extra hangles as needed
*/

bool Shader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	GLint check_value;

	stringstream ss;
	ss<<vertex_shader_file<<"Initialize - on entrance";

	if (GLReturnedError(ss.str().c_str()))
		return false;
	//Create our vertex shader, read it from the file and compile it. 
	//Check if compilation was successful
	this->vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	this->LoadShader(vertex_shader_file, this->vertex_shader_id);
	glCompileShader(this->vertex_shader_id);
	glGetShaderiv(this->vertex_shader_id, GL_COMPILE_STATUS, &check_value);
	if (check_value != GL_TRUE)
	{
		cerr << this->GetShaderLog(vertex_shader_id);
		cerr << "GLSL compilation failed - vertex shader: " << vertex_shader_file << endl;
		return false;
	}

	if (GLReturnedError("Shader::Initialize - after processing vertex shader"))
		return false;
	//Create our fragment shader, read it from the file and compile it. 
	//Check if compilation was successful
	this->fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	this->LoadShader(fragment_shader_file, this->fragment_shader_id);
	glCompileShader(this->fragment_shader_id);
	glGetShaderiv(this->fragment_shader_id, GL_COMPILE_STATUS, &check_value);
	if (check_value != GL_TRUE)
	{
		cerr << this->GetShaderLog(fragment_shader_id);
		cerr << "GLSL compilation failed - fragment shader: " << fragment_shader_file << endl;
		return false;
	}
	//Attach the shaders to the program and link
	this->program_id = glCreateProgram();
	glAttachShader(this->program_id, this->vertex_shader_id);
	glAttachShader(this->program_id, this->fragment_shader_id);
	glLinkProgram(program_id);

	//Individual shaders not needed anymore
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	glUseProgram(this->program_id);

	//Locate all of the possible default handles that we can have


	this->modelview_matrix_handle = GetUniformLocation("modelview_matrix");
	this->projection_matrix_handle = GetUniformLocation("projection_matrix");
	this->normal_matrix_handle = GetUniformLocation("normal_matrix");
	this->mvp_handle = GetUniformLocation("mvp");
	this->use_texture_handle = GetUniformLocation("use_texture");
	this->time_handle = GetUniformLocation("time");

	glUseProgram(0);

	return !GLReturnedError("Shader::Initialize - on exit");
}

void Shader::CustomSetup()
{
}


void Shader::TakeDown()
{
	//Cleanup teh shader

	GLint temp;
	GLsizei size;
	//If already taken down, do not do again
	if (this->program_id == (GLuint) -1)
		return;
	//Get the shader program
	glGetProgramiv(this->program_id, GL_ATTACHED_SHADERS, &temp);
	if (temp > 0)
	{
		//Get the attached shaders
		GLuint * shader_list = new GLuint[temp];
		glGetAttachedShaders(this->program_id, temp, &size, shader_list);
		for (GLsizei i = 0; i < size; i++)
		{
			// detach each shader
			glDetachShader(this->program_id, shader_list[i]);
			// The shaders were marked for deletion
			// immediately after they were created.
		}
		delete [] shader_list;
	}
	//finally delete the shader program and then finish
	glDeleteProgram(this->program_id);
	this->program_id = (GLuint) -1;
}

/*
	This function is adapted from OpenGL 4.0 Shading Language Cookbook by David Wolff.
*/

bool Shader::LoadShader(const char * file_name, GLuint shader_id)
{
	assert(file_name != NULL);
	if (GLReturnedError("Shader::LoadShader - on entrance"))
		return false;

	FILE * file_handle = NULL;
	file_handle = fopen( file_name, "rb");
	if (file_handle == NULL)
	{
		cerr <<  "Cannot open shader: " << file_name << endl;
		return false;
	}
	fseek(file_handle, 0, SEEK_END);
	int length = ftell(file_handle);
	fseek(file_handle, 0, SEEK_SET);
	GLubyte * buffer = new GLubyte[length + 1];
	fread(buffer, sizeof(GLubyte), length, file_handle);
	fclose(file_handle);
	buffer[length] = 0;
	glShaderSource(shader_id, 1, (const char **) &buffer, NULL);
	delete [] buffer;

	return !GLReturnedError("Shader::LoadShader - on exit");
}

/*
	This function is adapted from OpenGL 4.0 Shading Language Cookbook by David Wolff.
*/

string Shader::GetShaderLog(GLuint shader_id)
{
	stringstream s;
	GLint log_length;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length <= 0)
		s << "No shader log information available." << endl;
	else
	{
		GLchar * buffer = new GLchar[log_length];
		glGetShaderInfoLog(shader_id, log_length, NULL, buffer);
		s << "Shader log:" << endl;
		s << buffer << endl;
		delete [] buffer;
	}
	return s.str();
}
GLuint Shader::GetUniformLocation(string name){
	return glGetUniformLocation(program_id, (const GLchar *) name.c_str());
}

