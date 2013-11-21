#include "vertexattributes.h"

using namespace glm;

//This file contains the constructors for each vertex attribute type
//nothing really important here worth commenting....
VertexAttributesPADSNT::VertexAttributesPADSNT()
{
    this->position = vec3(0.0f);
    this->color_ambient = vec3(0.0f);
    this->color_diffuse = vec3(0.0f);
    this->color_specular = vec3(0.0f);
    this->color_glow = vec3(0.0f);
    this->normal = vec3(0.0f);
    this->texture_coordinate = vec2(0.0f);


}
VertexAttributesPADSNT::VertexAttributesPADSNT(const glm::vec3 & p, const glm::vec3 & c_a, const glm::vec3 & c_d, const glm::vec3 & c_s, const glm::vec3 & c_g, const glm::vec3 & n, const glm::vec2 & t)
{
    this->position = p;
    this->color_ambient = c_a;
    this->color_diffuse = c_d;
    this->color_specular = c_s;
    this->color_glow = c_g;
    this->normal = n;
    this->texture_coordinate = t;


}
VertexAttributesPADSNT::VertexAttributesPADSNT(const VertexAttributesPADSNT & other)
{

    this->position = other.position;
    this->color_ambient = other.color_ambient;
    this->color_diffuse = other.color_diffuse;
    this->color_specular = other.color_specular;
    this->color_glow = other.color_glow;
    this->normal = other.normal;
    this->texture_coordinate = other.texture_coordinate;

}


VertexAttributesPCNT::VertexAttributesPCNT()
{
    this->position = vec3(0.0f);
    this->color = vec3(0.0f);
    this->normal = vec3(0.0f);
    this->texture_coordinate = vec2(0.0f);
}

VertexAttributesPCNT::VertexAttributesPCNT(const vec3 & p, const vec3 & c, const vec3 & n, const vec2 & t)
{
    this->position = p;
    this->color = c;
    this->normal = n;
    this->texture_coordinate = t;
}

VertexAttributesPCNT::VertexAttributesPCNT(const VertexAttributesPCNT & other)
{
    this->position = other.position;
    this->color = other.color;
    this->normal = other.normal;
    this->texture_coordinate = other.texture_coordinate;
}

VertexAttributesP::VertexAttributesP(const vec3 & p)
{
    this->position = p;
}

VertexAttributesP::VertexAttributesP(const VertexAttributesP & other)
{
    this->position = other.position;
}

VertexAttributesP::VertexAttributesP()
{
    this->position = vec3(0.0f);
}

VertexAttributesPN::VertexAttributesPN(const vec3 & p, const vec3 & n)
{
    this->position = p;
    this->normal = n;
}

VertexAttributesPN::VertexAttributesPN(const VertexAttributesPN & other)
{
    this->position = other.position;
    this->normal = other.normal;
}

VertexAttributesPN::VertexAttributesPN()
{
    this->position = vec3(0.0f);
    this->normal = vec3(0.0f, 0.0f, 1.0f);
}


VertexAttributesPCN::VertexAttributesPCN()
{
    this->position = vec3(0.0f);
    this->color = vec3(0.0f);
    this->normal = vec3(0.0f);
}

VertexAttributesPCN::VertexAttributesPCN(const vec3 & p, const vec3 & c, const vec3 & n)
{
    this->position = p;
    this->color = c;
    this->normal = n;
}

VertexAttributesPCN::VertexAttributesPCN(const VertexAttributesPCN & other)
{
    this->position = other.position;
    this->color = other.color;
    this->normal = other.normal;
}
