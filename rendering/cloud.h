/*Based on code provided by: Perry Kivolowitz
Background object class
Draws a quad without any attached shader
Shader is specified manually (used for deferred shading)
*/


#pragma once
#include "object.h"
#include "shader.h"

class Cloud : public Object
{
public:
		Cloud();
	virtual bool Initialize(const std::vector<glm::vec3>& data);
	//Not used but needed because of inheritance
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview);
	void TakeDown();

private:
	std::vector<VertexAttributesP> vertices;
	typedef Object super;
};
