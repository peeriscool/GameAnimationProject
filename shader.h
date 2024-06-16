#ifndef _H_SHADER_
#define _H_SHADER_

#include <map>
#include <string>

class shader
{
private:
	unsigned int mHandle;
	std::map<std::string, unsigned int> mAttributes;
	std::map<std::string, unsigned int> mUniforms;
private:
	std::string ReadFile(const std::string& path);
	unsigned int CompileVertexShader(
		const std::string& vertex);
	unsigned int CompileFragmentShader(
		const std::string& fragment);
	bool LinkShaders(unsigned int vertex,
		unsigned int fragment);
	void PopulateAttributes();
	void PopulateUniforms();
private:
	shader(const shader&);
	shader& operator=(const shader&);
public:
	shader();
	shader(const std::string& vertex, const std::string& fragment);
	~shader();

	void Load(const std::string& vertex, const std::string& fragment);

	void Bind();
	void UnBind();

	unsigned int GetAttribute(const std::string& name);
	unsigned int GetUniform(const std::string& name);
	unsigned int GetHandle();
};
#endif //h_shader
