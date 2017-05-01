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
		:Position(glm::vec3(0.0, 0.0, 2.0)), TargetPos(glm::vec3(0.0, 0.0, 0.0)),
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
		Position = glm::vec3(-x, -y, 2);
		TargetPos = glm::vec3(-x, -y, 0);
	}

	void handleRotation(GLdouble x)
	{
		Position = glm::vec3(glm::sin(x * 90) * 2, 0, glm::cos(x * 90) * 2);
		//Viewup = ;
		//std::cout << "x : " << Position.x << "y : " << Position.y << std::endl;
	}

	//相机属性
public:
	glm::vec3 Position, Viewup, TargetPos;
	GLfloat fRotateAngle = 0.0f;
	GLfloat mouse_zoom;
	//GLfloat xAngle, yAngle;
};


#endif