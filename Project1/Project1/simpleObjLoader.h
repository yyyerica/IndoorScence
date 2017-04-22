#ifndef SIMPLE_OBJ_LOADER_H_
#define SIMPLE_OBJ_LOADER_H_
/*
* ��ֻ��һ��ʾ���ļ�obj������
* ��ʵ�õ�obj����Բ鿴https://github.com/syoyo/tinyobjloader
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "shader.h"

using namespace glm;
using std::vector;

// ��ʾһ����������
// ��ʾһ����������
struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

struct VertexCombineIndex
{
	GLuint posIndex;
	GLuint textCoordIndex;
	GLuint normIndex;
};

// ��ʾһ��������Ⱦ����Сʵ��
class Mesh
{
public:
	void draw(Shader& shader) // ����Mesh
	{
		shader.use();
		glBindVertexArray(this->VAOId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textureId);
		glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);

		/*glDrawArrays(GL_TRIANGLES, 0, this->vertData.size());*/
		glDrawArrays(GL_TRIANGLES, 0, this->vertData.size());

		glBindVertexArray(0);
		glUseProgram(0);
	}
	Mesh(){}

	Mesh(const std::vector<Vertex>& vertData, bool bShowData = false) // ����һ��Mesh
	{
		this->vertData = vertData;
		//this->textureId = textureId;
		this->setupMesh();
		if (bShowData)
		{
			const char * fileName = "vert-data.txt";
			std::ofstream file(fileName);
			for (std::vector<Vertex>::const_iterator it = this->vertData.begin(); it != this->vertData.end(); ++it)
			{
				file << glm::to_string(it->position) << ","
					<< glm::to_string(it->texCoords) << ","
					<< glm::to_string(it->normal) << std::endl;
			}
			file.close();
			std::cout << " vert data saved in file:" << fileName << std::endl;
		}
	}
	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAOId);
		glDeleteBuffers(1, &this->VBOId);
	}
public:
	std::vector<Vertex> vertData;
	GLuint VAOId, VBOId;
	GLint textureId;

	void setupMesh()  // ����VAO,VBO�Ȼ�����
	{
		glGenVertexArrays(1, &this->VAOId);
		glGenBuffers(1, &this->VBOId);

		glBindVertexArray(this->VAOId);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBOId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* this->vertData.size(),
			&this->vertData[0], GL_STATIC_DRAW);
		// ����λ������
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

class MyObjLoader
{
	std::vector<VertexCombineIndex> vertComIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_textCoords;
	std::vector<glm::vec3> temp_normals;

public:
	bool loadFromFile(const char* path,
		vector<Vertex>& vertData)
	{
		vertComIndices.clear();
		temp_vertices.clear();
		temp_textCoords.clear();
		temp_normals.clear();
		vector<Vertex> vertData1;//������˳���Ĵ��з������Ķ����б�
		vec3 normal1;

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
			if (line.substr(0, 2) == "vt") // ����������������
			{
				std::istringstream s(line.substr(2));
				glm::vec2 v;
				s >> v.x;
				s >> v.y;
				v.y = -v.y;  // ע��������ص�dds���� Ҫ��y���з�ת
				//v = normalize(v);
				temp_textCoords.push_back(v);
			}
			else if (line.substr(0, 2) == "vn") // ���㷨��������
			{
				std::istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
				v = normalize(v);
				temp_normals.push_back(v);
			}
			else if (line.substr(0, 1) == "v") // ����λ������
			{
				std::istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
				//v = normalize(v);
				temp_vertices.push_back(v);
			}
			else if (line.substr(0, 1) == "f") // ������
			{
				if (!strchr(line.c_str(), '/')){
					
					std::istringstream vtns(line.substr(2));
					std::string vtn;

					while (vtns >> vtn) // ����һ���ж����������
					{   
						
						VertexCombineIndex vertComIndex;
						///std::replace(vtn.begin(), vtn.end(), '/', ' ');
						std::istringstream ivtn(vtn);
						if (vtn.find("  ") != std::string::npos) // û����������
						{
							std::cerr << "Error:ObjLoader, no texture data found within file:"
								<< path << std::endl;
							return false;
						}
						ivtn >> vertComIndex.posIndex;
						
						//if (vertComIndex.posIndex)
						vertComIndex.posIndex--;
						
						vertComIndices.push_back(vertComIndex);
					}
				}
				else{
					std::istringstream vtns(line.substr(2));
					std::string vtn;
					while (vtns >> vtn) // ����һ���ж����������
					{
						VertexCombineIndex vertComIndex;
						std::replace(vtn.begin(), vtn.end(), '/', ' ');
						std::istringstream ivtn(vtn);
						if (vtn.find("  ") != std::string::npos) // û����������
						{
							std::cerr << "Error:ObjLoader, no texture data found within file:"
								<< path << std::endl;
							return false;
						}
						ivtn >> vertComIndex.posIndex
							>> vertComIndex.textCoordIndex
							>> vertComIndex.normIndex;
						//if (vertComIndex.posIndex)
						vertComIndex.posIndex--;
						vertComIndex.textCoordIndex--;
						vertComIndex.normIndex--;

						vertComIndices.push_back(vertComIndex);
					}
				}
				
			}
		}

		if (temp_normals.size() > 0)
		{
			for (std::vector<GLuint>::size_type i = 0; i < vertComIndices.size(); ++i)
			{
				Vertex vert;
				VertexCombineIndex comIndex = vertComIndices[i];

				vert.position = temp_vertices[comIndex.posIndex];
				vert.texCoords = temp_textCoords[comIndex.textCoordIndex];
				vert.normal = temp_normals[comIndex.normIndex];

				vertData.push_back(vert);
			}
		}
		else {
			int a = 0;
			glm::vec3 normal;
			Vertex v1, v2, v3;
			for (std::vector<GLuint>::size_type i = 0; i < vertComIndices.size(); ++i)
			{
				a++;
				if (a % 3 == 1){
					v1.position = temp_vertices[vertComIndices[i].posIndex];
				}
				else if (a % 3 == 2){
					v2.position = temp_vertices[vertComIndices[i].posIndex];
				}
					
				else if (a % 3 == 0)
				{
					v3.position = temp_vertices[vertComIndices[i].posIndex];
					normal = glm::normalize(cross(v2.position - v1.position, v3.position - v1.position));
					v1.normal = normal;
					v2.normal = normal;
					v3.normal = normal;
					vertData.push_back(v1);
					vertData.push_back(v2);
					vertData.push_back(v3);
				}

				
			}
		}
			


		
		return true;
	}

};


#endif