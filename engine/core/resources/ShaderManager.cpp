#include "ShaderManager.h"
#include "graphics/Shader.h"
#include "graphics/ShaderProgram.h"

std::unordered_map<const char*, Shader> ShaderManager::shaders = std::unordered_map<const char*, Shader>();
std::unordered_map<ShaderGroup, ShaderProgram> ShaderManager::programs = std::unordered_map<ShaderGroup, ShaderProgram>();

const std::string ShaderManager::shaderPath = "resources/shaders/";

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)


ShaderGroup::ShaderGroup()
{
	vert = 0;
	tessCont = 0;
	tessEval = 0;
	geom = 0;
	frag = 0;
}


bool ShaderGroup::operator==(const ShaderGroup& s) const
{
	if(vert != s.vert)
		return false;

	if(tessCont != s.tessCont)
		return false;
	
	if(tessEval != s.tessEval)
		return false;
	
	if(geom != s.geom)
		return false;
	
	if(frag != s.frag)
		return false;
	
	return true;
}

const ShaderProgram& ShaderManager::GetShaders(const char* vert, const char* frag)
{
	ShaderGroup shadersToLink;

	//Find the appropriate shaders
	if(shaders.find(vert) != shaders.end())
	{
		shadersToLink.vert = shaders[vert].GetID();
	} else
	{
		const Shader vertex((shaderPath + vert).c_str());
		shaders[vert] = vertex;
		shadersToLink.vert = shaders[vert].GetID();
	}

	if(shaders.find(frag) != shaders.end())
	{
		shadersToLink.frag = shaders[frag].GetID();
	} else
	{
		const Shader fragment((shaderPath + frag).c_str());
		shaders[frag] = fragment;
		shadersToLink.frag = shaders[frag].GetID();
	}


	
	if(programs.find(shadersToLink) != programs.end())
	{
		//if this program exists return it
		return programs[shadersToLink];
	} else
	{
		std::vector<GLint> shaderIDs;
		//put all of the shaders that are needed into an array so that they can be linked		
		if(shadersToLink.vert != 0)
		{
			shaderIDs.push_back(shadersToLink.vert);
		}
		if(shadersToLink.tessCont != 0)
		{
			shaderIDs.push_back(shadersToLink.tessCont);
		}
		if(shadersToLink.tessEval != 0)
		{
			shaderIDs.push_back(shadersToLink.tessEval);
		}
		if(shadersToLink.geom != 0)
		{
			shaderIDs.push_back(shadersToLink.geom);
		}
		if(shadersToLink.frag != 0)
		{
			shaderIDs.push_back(shadersToLink.frag);
		}

		ShaderProgram resultProgram;
		resultProgram.LinkShaders(shaderIDs);
		programs[shadersToLink] = resultProgram;

		return  programs[shadersToLink];
	}
}

void ShaderManager::DestroyAllShaders()
{
	for (auto& pair : programs)
	{
		pair.second.DeleteProgram();
	}
	programs.clear();

	for (auto& pair : shaders)
	{
		pair.second.DeleteShader();
	}
	shaders.clear();
}
