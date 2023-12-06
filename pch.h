#pragma once

// Add Opengl headers
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GL/GLM/glm.hpp>
#include <GL/GLM/ext.hpp>

// Add STL headers
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <list>

#include <Windows.h>

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

struct _TAG_WINDOWINFORMATION {
	GLFWwindow* window{ };
	int32		x{ };
	int32		y{ };
	int32		width{ };
	int32		height{ };
	std::string windowTitle{ };
};

struct _TAG_FLOAT2 {
	float x{ };
	float y{ };
};

struct _TAG_FLOAT3 {
	float x{ };
	float y{ };
	float z{ };
};

struct _TAG_FLOAT4 {
	float x{ };
	float y{ };
	float z{ };
	float w{ };
};

struct _TAG_VERTEX {
	glm::vec3 position{ };
	glm::vec3 normal{ };
	glm::vec2 texture{ };
};

struct Meterials {
	glm::vec3 specular{ };
	float shininess{ };
};

struct _TAG_LIGHTOPTION {

};

using float2 = _TAG_FLOAT2;
using float3 = _TAG_FLOAT3;
using float4 = _TAG_FLOAT4;
using vertex = _TAG_VERTEX;

using LightOption = _TAG_LIGHTOPTION;

using WindowInformation = _TAG_WINDOWINFORMATION;

extern std::unique_ptr<class Engine> engine;