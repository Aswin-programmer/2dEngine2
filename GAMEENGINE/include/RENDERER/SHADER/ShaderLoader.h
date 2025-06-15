#pragma once

#include "Shader.h"

class ShaderLoader
{
public:
	static std::shared_ptr<Shader> createShader( const char* vertexShaderPath, const char* fragmentShaderPath )
	{
		return std::make_shared<Shader>( vertexShaderPath, fragmentShaderPath );
	}
};
