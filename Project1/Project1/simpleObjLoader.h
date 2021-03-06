#ifndef SIMPLE_OBJ_LOADER_H_
#define SIMPLE_OBJ_LOADER_H_
/*
* 这只是一个示例的简单obj加载类
* 更实用的obj类可以查看https://github.com/syoyo/tinyobjloader
*/
#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>       
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include "shader.h"

using namespace glm;
using namespace std;

// 表示一个顶点属性
// 表示一个顶点属性

struct Vertex
{
	vec3 position;
	vec2 texCoords;
	vec3 normal;
};

struct VertexCombineIndex
{
	GLuint posIndex;
	GLuint textCoordIndex;
	GLuint normIndex;
};

struct SumNorStruct
{
	vec3 position;
	vec2 texCoords;
	vec3 normal;
	vec3 sumnormal;
	GLint sum = 0;
};



class MyObjLoader
{
	vector<vec2> temp_textCoords;
	vector<vec3> temp_normals;

	vector<SumNorStruct> Vertex_tempVector;
	vector<Vertex> Vertex_Vector;

	vector<int> position_indecies;

	int v1, v2, v3;
	int faceCount = 0;
	vec3 normal;

public:
	bool loadFromFile(const char* path,
		vector<Vertex>& vertData)
	{	
		temp_textCoords.clear();
		temp_normals.clear();
		Vertex_tempVector.clear();
		Vertex_Vector.clear();
		position_indecies.clear();

		std::ifstream file(path);
		if (!file)
		{
			std::cerr << "Error::ObjLoader, could not open obj file:"
				<< path << " for reading." << std::endl;
			return false;
		}
		std::string line;

		
		while (getline(file, line))
		{
			if (line.substr(0, 2) == "vt") // 顶点纹理坐标数据
			{
				std::istringstream s(line.substr(2));
				glm::vec2 v;
				s >> v.x;
				s >> v.y;
				v.y = -v.y;  // 注意这里加载的dds纹理 要对y进行反转
				temp_textCoords.push_back(v);
			}
			else if (line.substr(0, 2) == "vn") // 顶点法向量数据
			{
				std::istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
				temp_normals.push_back(v);
			}
			else if (line.substr(0, 1) == "v") // 顶点位置数据
			{
				std::istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
			
				SumNorStruct sumnorStruct;
				sumnorStruct.position = v;
				Vertex_tempVector.push_back(sumnorStruct);
				
				Vertex vertex;
				vertex.position = v;
				Vertex_Vector.push_back(vertex);
			}
			else if (line.substr(0, 1) == "f") // 面数据
			{
				if (!strchr(line.c_str(), '/'))
				{	
					std::istringstream vtns(line.substr(2));
					std::string vtn;
					while (vtns >> vtn) // 只有顶点坐标
					{   	
						int vertComIndex;
						//std::replace(vtn.begin(), vtn.end(), '/', ' ');
						std::istringstream ivtn(vtn);
						if (vtn.find("  ") != std::string::npos) // 没有纹理数据
						{
							std::cerr << "Error:ObjLoader, no texture data found within file:"
								<< path << std::endl;
							return false;
						}
						ivtn >> vertComIndex;
						vertComIndex--;

						position_indecies.push_back(vertComIndex);
						
						switch (faceCount % 3)
						{
						case 0:
							v1 = vertComIndex;
							break;
						case 1:
							v2 = vertComIndex;
							break;
						case 2:
							v3 = vertComIndex;
							normal = cross(Vertex_tempVector[v2].position - Vertex_tempVector[v1].position, Vertex_tempVector[v3].position - Vertex_tempVector[v1].position);
							Vertex_tempVector[v1].sumnormal += normal;
							Vertex_tempVector[v2].sumnormal += normal;
							Vertex_tempVector[v3].sumnormal += normal;
							Vertex_tempVector[v1].sum++;
							Vertex_tempVector[v2].sum++;
							Vertex_tempVector[v3].sum++;
							break;
						default:
							break;
						}

						faceCount++;
					}
				} else {
					std::istringstream vtns(line.substr(2));
					std::string vtn;
					while (vtns >> vtn) // 处理一行中多个顶点属性
					{
						int positionIndex, textcoorIndex, normIndex;
						std::replace(vtn.begin(), vtn.end(), '/', ' ');
						std::istringstream ivtn(vtn);
						if (vtn.find("  ") != std::string::npos) // 没有纹理数据
						{
							std::cerr << "Error:ObjLoader, no texture data found within file:"
								<< path << std::endl;
							return false;
						}
						
						ivtn >> positionIndex
						>> textcoorIndex
						>> normIndex;
					
						positionIndex--;
						textcoorIndex--;
						normIndex--;
						Vertex_Vector[positionIndex].normal = temp_normals[normIndex];
						Vertex_Vector[positionIndex].texCoords = temp_textCoords[textcoorIndex];
						
						position_indecies.push_back(positionIndex);
					}
				}
				
			}
		}

		if (temp_normals.size() > 0)
		{
			for (int i = 0; i < position_indecies.size(); i++)
			{
				vertData.push_back(Vertex_Vector[position_indecies[i]]);
			}
				
		}
		else {
			Vertex vertex;
			for (int i = 0; i < position_indecies.size(); i++)
			{
				SumNorStruct sumnorstruct = Vertex_tempVector[position_indecies[i]];
				vertex.position = sumnorstruct.position;
				int sum = sumnorstruct.sum;
				vec3 sumnormal = sumnorstruct.sumnormal;
				sumnormal /= sum;

				vertex.normal = sumnormal;

				vertData.push_back(vertex);
			}
		}

		return true;
	}


};
#endif