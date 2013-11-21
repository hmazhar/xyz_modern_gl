/*	Based on code provided by: Perry Kivolowitz
	P = position
	C = color
	N = normal
	T = texture coordinates

	A = Ambient
	D = Diffuse
	S = Specular
*/

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//This class supports ADS lighting with glow and texture coordinates
//Nothing too interesting here, note that the order for the members is important
class VertexAttributesPADSNT
{
public:
	VertexAttributesPADSNT();
	VertexAttributesPADSNT(const glm::vec3 & p, const glm::vec3 & c_a, const glm::vec3 & c_d, const glm::vec3 & c_s, const glm::vec3 & c_g, const glm::vec3 & n, const glm::vec2 & t);
	VertexAttributesPADSNT(const VertexAttributesPADSNT & other);
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color_ambient;
	glm::vec3 color_diffuse;
	glm::vec3 color_specular;
	glm::vec3 color_glow;
	glm::vec2 texture_coordinate;
};

class VertexAttributesPCNT
{
public:
	VertexAttributesPCNT();
	VertexAttributesPCNT(const glm::vec3 & p, const glm::vec3 & c, const glm::vec3 & n, const glm::vec2 & t);
	VertexAttributesPCNT(const VertexAttributesPCNT & other);
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texture_coordinate;
};

class VertexAttributesPCN
{
public:
	VertexAttributesPCN();
	VertexAttributesPCN(const glm::vec3 & p, const glm::vec3 & c, const glm::vec3 & n);
	VertexAttributesPCN(const VertexAttributesPCN & other);
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	
};

class VertexAttributesPN
{
public:
	VertexAttributesPN();
	VertexAttributesPN(const glm::vec3 & p, const glm::vec3 & n);
	VertexAttributesPN(const VertexAttributesPN & other);
	glm::vec3 position;
	glm::vec3 normal;
};

class VertexAttributesP
{
public:
	VertexAttributesP();
	VertexAttributesP(const glm::vec3 & p);
	VertexAttributesP(const VertexAttributesP & other);
	glm::vec3 position;
};
