/*
Material Class 
Stores ambient, diffuse, specular and glow colors
Usefull when sending material info to a renderable object
*/
#pragma once
#include "shader.h"

class Material: public BasicGL
{
public:
	//constructor accepts 4 colors and sets them.
	Material(glm::vec3 a,glm::vec3 d, glm::vec3 s,  glm::vec3 g){
		ambient_color = a;
		diffuse_color = d;
		specular_color = s;
		glow_color = g;
	}
	//Dont need to take anything down so this function is empty
	void TakeDown(){}
	glm::vec3 ambient_color;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	glm::vec3 glow_color;

};
