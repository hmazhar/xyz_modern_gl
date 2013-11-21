/*  Based on code provided by: Perry Kivolowitz
    Class for handling shaders
    Has default bindings for several uniforms including the modelview, projection, normal and mvp matricies

*/

#pragma once
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "basicgl.h"
class Shader : public BasicGL
{
    public:
        Shader();
        void TakeDown();
        void Use();
        virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
        virtual void CustomSetup();
        void CommonSetup(const GLfloat * projection, const GLfloat * modelview, const GLfloat * mvp, const GLfloat * nm);
        void ToggleTexture(bool use_texture);
        void SetTime(float _time);
		GLuint GetUniformLocation(std::string name);
        GLuint modelview_matrix_handle;
        GLuint projection_matrix_handle;
        GLuint normal_matrix_handle;
        GLuint mvp_handle;
        GLuint use_texture_handle;
        GLuint time_handle;

        GLuint vertex_shader_id;
        GLuint fragment_shader_id;
        GLuint program_id;
        bool LoadShader(const char * file_name, GLuint shader_id);
        std::string GetShaderLog(GLuint shader_id);
        float time;
};
