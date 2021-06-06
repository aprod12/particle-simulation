#pragma once
#include <string>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

struct ShaderSource {

	std::string vertexSource;
	std::string fragmentSource;
	std::string computeSource;
};


class Shader {

private:
	std::string m_FilePath;
	unsigned int m_RenderID;
	unsigned int m_ComputeRenderID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string & shaderFilepath, const std::string & computeFilepath);
	~Shader();

	void Bind() const;
	void UnBind() const;
	void BindCompute() const;
	void UnBindCompute() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform2f(const std::string& name, float v0, float v1);
private:
	ShaderSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	int GetUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string& vs, const std::string& fs);
	unsigned int CreateComputeShader(const std::string& filename);
};