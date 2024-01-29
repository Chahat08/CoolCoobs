#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <iostream>

class Shader {
private:
	// SHADER ID
	unsigned int ID = 0;

	unsigned int createShader(const std::string& shaderSource, char type);
	void checkShaderCompilationErrors(unsigned int shaderID);
	void checkProgramLinkingErrors();
public:
	// constructor
	Shader(const std::string& vertexShader, const std::string& fragmentShader, bool filePathProvided=true);

	// use shaderprogram
	void use();
	// delete shaderprogram
	void del();
	// get shader id
	unsigned int getID();

	// set uniform values
	void setBoolUniform(const std::string& name, bool value) const;
	void setIntUniform(const std::string& name, int value) const;
	void setFloatUniform(const std::string& name, float value) const;
	void setUniformMatrix4float(const std::string& name, glm::mat4& transform);
};