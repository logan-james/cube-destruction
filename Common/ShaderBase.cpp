/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "ShaderBase.h"
using namespace NCL;
using namespace Rendering;

ShaderBase::ShaderBase(const string& vertex, const string& fragment, const string& geometry, const string& domain, const string& hull)
{
	shaderFiles[ShaderStages::SHADER_VERTEX]	= vertex;
	shaderFiles[ShaderStages::SHADER_FRAGMENT]	= fragment;
	shaderFiles[ShaderStages::SHADER_GEOMETRY]	= geometry;
	shaderFiles[ShaderStages::SHADER_DOMAIN]	= domain;
	shaderFiles[ShaderStages::SHADER_HULL]		= hull;
}

ShaderBase::~ShaderBase()
{
}