#include "Shader.h"

void Shader::checkShaderCompilationErrors(unsigned int shaderID) {
	int success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cerr << "ERROR: Failed to compile shader\n" << infoLog << std::endl;
	}
}

void Shader::checkProgramLinkingErrors() {
	int success;
	char infoLog[512];
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
		std::cerr << "ERROR::Failed to link shader program\n" << infoLog << std::endl;
	}
}

unsigned int Shader::createShader(const std::string& shaderSource, char type) {
	// returns shader id
	unsigned int shader = 0;

	if (type == 'v') shader = glCreateShader(GL_VERTEX_SHADER); // CREATE VERTEX SHADER
	else if (type == 'f') shader = glCreateShader(GL_FRAGMENT_SHADER); // CREATE FRAGMENT SHADER

	const char* shaderSourceCstr = shaderSource.c_str();
	glShaderSource(shader, 1, &shaderSourceCstr, NULL);
	glCompileShader(shader);

	return shader;
}

Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, bool filePathProvided) {

	unsigned int vertexShader = 0, fragmentShader = 0;

	if (filePathProvided) {
		std::fstream vertexFileStream(vertexShaderSource, std::ios::in);

		if (vertexFileStream.is_open()) {
			const std::string vertexShaderSourceString(
				(std::istreambuf_iterator<char>(vertexFileStream)),
				std::istreambuf_iterator<char>()
			);

			vertexShader = createShader(vertexShaderSourceString, 'v');

		}
		else {
			std::cerr << "Error in reading vertex shader source: " << vertexShaderSource << std::endl;
		}

		checkShaderCompilationErrors(vertexShader);

		std::fstream fragmentFileStream(fragmentShaderSource, std::ios::in);

		if (fragmentFileStream.is_open()) {
			const std::string fragmentShaderSourceString(
				(std::istreambuf_iterator<char>(fragmentFileStream)),
				std::istreambuf_iterator<char>()
			);

			fragmentShader = createShader(fragmentShaderSourceString, 'f');

		}
		else {
			std::cerr << "Failed to open fragment shader source: " << fragmentShaderSource << std::endl;
		}

		checkShaderCompilationErrors(fragmentShader);
	}
	else {
		vertexShader = createShader(vertexShaderSource, 'v');
		checkShaderCompilationErrors(vertexShader);
		fragmentShader = createShader(fragmentShaderSource, 'f');
		checkShaderCompilationErrors(fragmentShader);
	}

	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	checkProgramLinkingErrors();

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}



void Shader::use() {
	glUseProgram(this->ID);
}

void Shader::del() {
	glDeleteProgram(this->ID);
}

unsigned int Shader::getID() {
	return this->ID;
}

void Shader::setBoolUniform(const std::string& name, bool value) const {
	unsigned int uniformLocation = glGetUniformLocation(this->ID, name.c_str());
	if (uniformLocation == -1)
		std::cerr << "ERROR: Uniform " << name << " not found" << std::endl;
	glUniform1i(uniformLocation, (int)value);
}

void Shader::setIntUniform(const std::string& name, int value) const {
	unsigned int uniformLocation = glGetUniformLocation(this->ID, name.c_str());
	if (uniformLocation == -1) {
		std::cerr << "ERROR: Uniform " << name << " not found" << std::endl;
	}
	glUniform1i(uniformLocation, value);
}

void Shader::setFloatUniform(const std::string& name, float value) const {
	unsigned int uniformLocation = glGetUniformLocation(this->ID, name.c_str());
	if (uniformLocation == -1) {
		std::cerr << "ERROR: Uniform " << name << " not found." << std::endl;
	}
	glUniform1f(uniformLocation, value);
}

void Shader::setUniformMatrix4float(const std::string& name, glm::mat4& transform) {
	unsigned int transformLoc = glGetUniformLocation(this->ID, name.c_str());
	if (transformLoc == -1) {
		std::cerr << "Uniform matrix 4f " << name << " not found" << std::endl;
	}
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}