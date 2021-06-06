#include "Renderer.h"

void GLClearError() {

	while (glGetError() != GL_NO_ERROR);

}
bool GLLogCall(const char* function, const char* file, int line) {

	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << std::hex << error << ") in " << function 
			<< " " << file << ": (" << std::dec << line << ")" << std::endl;
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, va.GetSize()));
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Compute(const Shader& shader, unsigned int num_groups_x, unsigned int num_groups_y, unsigned int num_groups_z) const
{
	shader.BindCompute();
	GLCall(glDispatchCompute(num_groups_x, num_groups_y, num_groups_z));
	// make sure writing to image has finished before read		
	GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));
}
