#include <GL/glew.h>
#include <iostream>

#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(); 
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
private:

public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;
	void Compute(const Shader& shader, unsigned int num_groups_x, unsigned int num_groups_y, unsigned int num_groups_z) const;
};