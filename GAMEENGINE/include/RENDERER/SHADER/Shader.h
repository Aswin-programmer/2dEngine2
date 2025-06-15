#pragma once

class Shader
{
public:
	Shader();
	Shader( const char* vertexPath, const char* fragmentPath );
	~Shader();
	void checkCompileErrors( unsigned int shader, std::string type );
	void use();
	void unuse();
	unsigned int getID() { return ID; }
	void setBool( const std::string& name, bool value ) const;
	void setInt( const std::string& name, int value ) const;
	void setFloat( const std::string& name, float value ) const;
	void setMat4( const std::string& name, glm::mat4 mat ) const;
	void setTexture( const std::string& name, GLsizei count, const GLint* value );

private:
	unsigned int ID;
};
