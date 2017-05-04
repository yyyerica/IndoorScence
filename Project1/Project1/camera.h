#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include <fstream>
#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iomanip>      // std::setprecision

//定义移动方向
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	yClockwise,
	yAnticlockwise,
	REBOUND
};

const GLfloat MOUSE_ZOOM = 45.0f; //传给perspective矩阵

class Camera
{
public:
	//Camera(glm::vec3 pos = glm::vec3(0.0, 0.0, 2.0),
	//	glm::vec3 viewup = glm::vec3(0.0, 1.0, 0.0),
	//	glm::vec3 targetpos = glm::vec3(0.0, 0.0, 0.0))
	//	:Position(pos), TargetPos(targetpos), Viewup(viewup), mouse_zoom(MOUSE_ZOOM)
	//{

	//}

	Camera()
		:Position(glm::vec3(0.0, 0.0, radius)), TargetPos(glm::vec3(0.0, 0.0, 0.0)),
		Viewup(glm::vec3(0.0, 1.0, 0.0)), mouse_zoom(MOUSE_ZOOM)
	{

	}


public:

	// 获取视变换矩阵
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->TargetPos, this->Viewup);
	}

	void handleMouseScroll(GLfloat yoffset)
	{
		if (this->mouse_zoom >= 1.0f && this->mouse_zoom <= MOUSE_ZOOM)
			this->mouse_zoom -= 0.05 * yoffset;
		if (this->mouse_zoom <= 1.0f)
			this->mouse_zoom = 1.0f;
		if (this->mouse_zoom >= 45.0f)
			this->mouse_zoom = 45.0f;
	}

	void handleMouseTranslation(GLdouble x, GLdouble y)
	{
		Position = glm::vec3(-x, -y, radius);
		TargetPos = glm::vec3(-x, -y, 0);
	}

	void handleRotation(GLdouble x)
	{
		Position = glm::vec3(glm::sin(x * 90) * radius, 0, glm::cos(x * 90) * radius);
		//Viewup = ;
		//std::cout << "x : " << Position.x << "y : " << Position.y << std::endl;
	}

	glm::vec3 direction, vertical;
	GLfloat camX, camZ;

	void handleTranslation(Camera_Movement Transdirection)
	{
		switch (Transdirection)
		{
		case BACKWARD:
			// 物体到相机的单位向量
			direction = glm::normalize(TargetPos - Position);
			direction *= 0.2;   // 移动0.2个单位向量
			Position += direction;
			break;
		case FORWARD:
			direction = glm::normalize(TargetPos - Position);
			direction *= 0.2;
			Position -= direction;
			break;
		case LEFT:
			// 物体到相机的单位向量
			direction = glm::normalize(TargetPos - Position);
			// 物体到相机的单位向量 与 相机的向上向量相乘,得到垂直向量,即平移向量
			vertical = glm::normalize(glm::cross(direction, Viewup));
			vertical *= 0.2;
			Position += vertical;  // 移动相机位置
			TargetPos += vertical; //相机的指向位置也一起平衡(不平移则以原来的目标转圈)
			break;
		case RIGHT:
			direction = glm::normalize(TargetPos - Position);
			vertical = glm::normalize(glm::cross(direction, Viewup));
			vertical *= 0.2;
			Position -= vertical;
			TargetPos -= vertical;
			break;
		case UP:
			vertical = Viewup;
			vertical *= 0.2;
			Position -= vertical;//相机下移
			TargetPos -= vertical;
			break;
		case DOWN:
			vertical = Viewup;
			vertical *= 0.2;
			Position += vertical;
			TargetPos += vertical;
			break;
		case yClockwise:
			radius = glm::distance(Position, TargetPos);
			fRotateAngle += 0.2;

			camX = sin(fRotateAngle) * radius + TargetPos.x;
			camZ = cos(fRotateAngle) * radius + TargetPos.z;

			Position = glm::vec3(camX, 0.0, camZ);
			break;
		case yAnticlockwise:
			radius = glm::distance(Position, TargetPos);
			fRotateAngle -= 0.2;

			camX = sin(fRotateAngle) * radius + TargetPos.x;
			camZ = cos(fRotateAngle) * radius + TargetPos.z;

			Position = glm::vec3(camX, 0.0, camZ);
			break;
		case REBOUND:
			Position = glm::vec3(0.0f, 0.0f, 2.0f);
			TargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
			break;
		default:
			break;
		}
	}

	//相机属性
public:
	glm::vec3 Position, Viewup, TargetPos;
	GLfloat fRotateAngle = 0.0f;
	GLfloat mouse_zoom;
	GLfloat radius = 4;
	//GLfloat xAngle, yAngle;
};


#endif