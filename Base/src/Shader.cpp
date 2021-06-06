#include "Shader.h"
#include "Renderer.h"


ShaderSource Shader::ParseShader(const std::string& filePath) {


	std::ifstream stream(filePath);

	enum class ShaderType {

		NONE = -1, VERTEX = 0, FRAGMENT = 1, COMPUTE = 2

	};
	std::string line;
	std::stringstream ss[3];
	ShaderType type = ShaderType::NONE;


	while (getline(stream, line)) {

		if (line.find("#shader") != std::string::npos) {

			if (line.find("compute") != std::string::npos) {
				type = ShaderType::COMPUTE;
			}
			else if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str(), ss[2].str() };
}

Shader::Shader(const std::string & shaderFilepath, const std::string & computeFilepath)
	: m_FilePath(shaderFilepath), m_RenderID(0), m_ComputeRenderID(0)
{
	ShaderSource source = ParseShader(shaderFilepath);
	//m_RenderID = CreateShader(source.vertexSource, source.fragmentSource);
	m_ComputeRenderID = CreateComputeShader(computeFilepath);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RenderID));
	GLCall(glDeleteProgram(m_ComputeRenderID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RenderID));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));
}

void Shader::BindCompute() const
{
	GLCall(glUseProgram(m_ComputeRenderID));
}

void Shader::UnBindCompute() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniform1f(const std::string & name, float v0)
{
	GLCall(glUniform1f(GetUniformLocation(name), v0));
}
void Shader::SetUniform2f(const std::string & name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}
 unsigned int Shader::CreateShader(const std::string& vs, const std::string& fs) {

	GLCall(unsigned int program = glCreateProgram());

	unsigned int vertex = CompileShader(vs, GL_VERTEX_SHADER);
	unsigned int fragment = CompileShader(fs, GL_FRAGMENT_SHADER);
	GLCall(glAttachShader(program, vertex));
	GLCall(glAttachShader(program, fragment));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vertex));
	GLCall(glDeleteShader(fragment));

	return program;
}

 unsigned int Shader::CreateComputeShader(const std::string& filepath)
 {
	 ShaderSource source = ParseShader(filepath);
	 const char* compute_shader_source = source.computeSource.c_str();
	 unsigned int compute_shader = CompileShader(compute_shader_source, GL_COMPUTE_SHADER);

	 GLCall(unsigned int compute_program = glCreateProgram());
	 GLCall(glAttachShader(compute_program, compute_shader));
	 GLCall(glLinkProgram(compute_program));
	 GLCall(glValidateProgram(compute_program));

	 GLCall(glDeleteShader(compute_shader));

	 return compute_program;

 }

unsigned int Shader::CompileShader(const std::string& source, unsigned int type) {

	std::string shaderType;
	switch (type) {
	case GL_VERTEX_SHADER:
		shaderType = "Vertex";
		break;
	case GL_FRAGMENT_SHADER:
		shaderType = "Fragment";
		break;
	case GL_COMPUTE_SHADER:
		shaderType = "Compute";
		break;
	default:
		break;
	}
	unsigned int shader;
	GLCall(shader = glCreateShader(type));
	const char* sc = source.c_str();
	GLCall(glShaderSource(shader, 1, &sc, nullptr));
	GLCall(glCompileShader(shader));

	int result;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(shader, length, &length, message));
		std::cout << "Failed to compile " << shaderType << " shader" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(shader));
		return 0;

	}

	return shader;

}

int Shader::GetUniformLocation(const std::string & name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	
	GLCall(int location = glGetUniformLocation(m_ComputeRenderID, name.c_str()));
	//GLCall(int location = glGetUniformLocation(m_RenderID, name.c_str()));
	if (location == -1) {
		//GLCall(location = glGetUniformLocation(m_RenderID, name.c_str()));
		if(location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}
	m_UniformLocationCache[name] = location;
	return location;
}
