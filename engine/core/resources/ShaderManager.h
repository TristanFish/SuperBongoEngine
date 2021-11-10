#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>

#include "glew/glew.h"

struct ShaderGroup
{
	GLint vert;
	GLint tessCont;
	GLint tessEval;
	GLint geom;
	GLint frag;

	ShaderGroup();
	~ShaderGroup() = default;

	bool operator==(const ShaderGroup& s) const;
};

namespace std {

	template <>
	struct hash<ShaderGroup>
	{
		std::size_t operator()(const ShaderGroup& sg) const noexcept
		{
			using std::size_t;
      
			size_t value = sg.vert;
			value = (value << 8) + sg.tessCont;
			value = (value << 8) + sg.tessEval;
			value = (value << 8) + sg.geom;
			value = (value << 8) + sg.frag;

			return value;
		}
	};
}

class Shader;
class ShaderProgram;

class ShaderManager
{
	static const std::string shaderPath;
	
	static std::unordered_map<const char*, Shader> shaders;
	static std::unordered_map<ShaderGroup, ShaderProgram> programs;

public:
	static const ShaderProgram& GetShaders(const char* vert, const char* frag);

	static void DestroyAllShaders();
};

#endif
