#ifndef MESH_H_
#define MESH_H_
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

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

// 表示一个用于渲染的最小实体
class Mesh
{
public:
	void draw(Shader& shader) // 绘制Mesh
	{
		glBindVertexArray(this->VAOId);
		shader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textureId);
		glUniform1i(glGetUniformLocation(shader.programId, "TexCoords"), 0);

		glDrawArrays(GL_TRIANGLES, 0, this->vertData.size());

		glBindVertexArray(0);
		glUseProgram(0);

	}
	Mesh(){}

	Mesh(const std::vector<Vertex>& vertData, bool bShowData = false) // 构造一个Mesh
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

	Mesh(const std::vector<Vertex>& vertData, GLint textureId, bool bShowData = false) // 构造一个Mesh
	{
		this->vertData = vertData;
		this->textureId = textureId;
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

	void setupMesh()  // 建立VAO,VBO等缓冲区
	{
		glGenVertexArrays(1, &this->VAOId);
		glGenBuffers(1, &this->VBOId);

		glBindVertexArray(this->VAOId);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBOId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* this->vertData.size(),
			&this->vertData[0], GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (GLvoid*)(5 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

#endif