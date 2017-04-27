//#ifndef _CAMERA_H_
//#define _CAMERA_H_
//
//#include <iostream>
//#include <fstream>
//#include <GLEW/glew.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/string_cast.hpp>
//#include <iomanip>      // std::setprecision
//
////定义移动方向
//enum Camera_Movement
//{
//	FORWARD,
//	BACKWARD,
//	LEFT,
//	RIGHT,
//	UP,
//	DOWN,
//	yClockwise,
//	yAnticlockwise,
//	REBOUND
//};
//
//const GLfloat MOUSE_ZOOM = 45.0f; //传给perspective矩阵
//
//class Camera
//{
//public:
//	//Camera(glm::vec3 pos = glm::vec3(0.0, 0.0, 2.0),
//	//	glm::vec3 viewup = glm::vec3(0.0, 1.0, 0.0),
//	//	glm::vec3 targetpos = glm::vec3(0.0, 0.0, 0.0))
//	//	:Position(pos), TargetPos(targetpos), Viewup(viewup), mouse_zoom(MOUSE_ZOOM)
//	//{
//
//	//}
//
//	Camera()
//		:Position(glm::vec3(0.0, 0.0, 2.0)), TargetPos(glm::vec3(0.0, 0.0, 0.0)),
//		Viewup(glm::vec3(0.0, 1.0, 0.0)), mouse_zoom(MOUSE_ZOOM)
//	{
//
//	}
//
//
//public:
//
//	// 获取视变换矩阵
//	glm::mat4 getViewMatrix()
//	{
//		return glm::lookAt(this->Position, this->TargetPos, this->Viewup);
//	}
//
//	void handleMouseScroll(GLfloat yoffset)
//	{
//		if (this->mouse_zoom >= 1.0f && this->mouse_zoom <= MOUSE_ZOOM)
//			this->mouse_zoom -= 0.05 * yoffset;
//		if (this->mouse_zoom <= 1.0f)
//			this->mouse_zoom = 1.0f;
//		if (this->mouse_zoom >= 45.0f)
//			this->mouse_zoom = 45.0f;
//	}
//
//	void handleMouseTranslation(GLdouble x, GLdouble y)
//	{
//		Position = glm::vec3(-x, -y, 2);
//		TargetPos = glm::vec3(-x, -y, 0);
//	}
//
//	void handleRotation(GLdouble x)
//	{
//		Position = glm::vec3(sin(x/2 * 90)*2, 0, cos(x/2 * 90)*2);
//		//Viewup = ;
//		//std::cout << "x : " << Position.x << "y : " << Position.y << std::endl;
//	}
//
//	//相机属性
//public:
//	glm::vec3 Position, Viewup, TargetPos;
//	GLfloat fRotateAngle = 0.0f;
//	GLfloat mouse_zoom;
//	//GLfloat xAngle, yAngle;
//};
//
////// 定义移动方向
////enum Camera_Movement {
////	FORWARD,
////	BACKWARD,
////	LEFT,
////	RIGHT
////};
////// 定义预设常量
////const GLfloat YAW = 0.0f;
////const GLfloat PITCH = 0.0f;
////const GLfloat SPEED = 3.0f;
////const GLfloat MOUSE_SENSITIVTY = 0.05f;
////const GLfloat MOUSE_ZOOM = 45.0f;
////const float  MAX_PITCH_ANGLE = 89.0f; // 防止万向锁
////
////class Camera
////{
////public:
////	Camera(glm::vec3 pos = glm::vec3(0.0, 0.0, 2.0),
////		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0),
////		GLfloat yaw = YAW, GLfloat pitch = PITCH)
////		:position(pos), forward(0.0, 0.0, -1.0), viewUp(up),
////		moveSpeed(SPEED), mouse_zoom(MOUSE_ZOOM), mouse_sensitivity(MOUSE_SENSITIVTY),
////		yawAngle(yaw), pitchAngle(pitch)
////	{
////		this->updateCameraVectors();
////	}
////
////public:
////	// 获取视变换矩阵
////	glm::mat4 getViewMatrix()
////	{
////		return glm::lookAt(this->position, this->position + this->forward, this->viewUp);
////	}
////	
////	// 处理键盘按键后方向移动
////	void handleKeyPress(Camera_Movement direction, GLfloat deltaTime)
////	{
////	//	GLfloat velocity = this->moveSpeed * deltaTime;
////	//	switch (direction)
////	//	{
////	//	case FORWARD:
////	//		this->position += this->forward * velocity;
////	//		break;
////	//	case BACKWARD:
////	//		this->position -= this->forward * velocity;
////	//		break;
////	//	case LEFT:
////	//		this->position -= this->side * velocity;
////	//		break;
////	//	case RIGHT:
////	//		this->position += this->side * velocity;
////	//		break;
////	//	default:
////	//		break;
////	//	}
////	//}
////	
////	//// 处理鼠标移动
////	//void handleMouseMove(GLfloat xoffset, GLfloat yoffset)
////	//{
////	//	xoffset *= this->mouse_sensitivity; // 用鼠标灵敏度调节角度变换
////	//	yoffset *= this->mouse_sensitivity;
////
////	//	this->pitchAngle += yoffset;
////	//	this->yawAngle += xoffset;
////
////	//	this->normalizeAngle();
////	//	this->updateCameraVectors();
////	//}
////	void handleMouseMove(GLfloat x, GLfloat y)
////	{
////		glm::vec3 forward;
////		forward.x = x;
////		forward.y = y;
////		forward.z = 0;
////		this->forward = glm::normalize(forward);
////	}
////	
////	// 处理鼠标滚轮缩放 保持在[1.0, MOUSE_ZOOM]之间
////	void handleMouseScroll(GLfloat yoffset)
////	{
////		/*if (this->mouse_zoom >= 1.0f && this->mouse_zoom <= MOUSE_ZOOM)
////			this->mouse_zoom -= this->mouse_sensitivity * yoffset;
////		if (this->mouse_zoom <= 1.0f)
////			this->mouse_zoom = 1.0f;
////		if (this->mouse_zoom >= 45.0f)
////			this->mouse_zoom = 45.0f;*/
////	}
////
////	// 使pitch yaw角度保持在合理范围内
////	void normalizeAngle()
////	{
////		/*if (this->pitchAngle > MAX_PITCH_ANGLE)
////			this->pitchAngle = MAX_PITCH_ANGLE;
////		if (this->pitchAngle < -MAX_PITCH_ANGLE)
////			this->pitchAngle = -MAX_PITCH_ANGLE;
////		if (this->yawAngle < 0.0f)
////			this->yawAngle += 360.0f;*/
////	}
////	
////	// 计算forward side向量
////	void updateCameraVectors()
////	{
////		/*glm::vec3 forward;
////		forward.x = -sin(glm::radians(this->yawAngle)) * cos(glm::radians(this->pitchAngle));
////		forward.y = sin(glm::radians(this->pitchAngle));
////		forward.z = -cos(glm::radians(this->yawAngle)) * cos(glm::radians(this->pitchAngle));
////		this->forward = glm::normalize(forward);
////
////		glm::vec3 side;
////		side.x = cos(glm::radians(this->yawAngle));
////		side.y = 0;
////		side.z = -sin(glm::radians(this->yawAngle));
////		this->side = glm::normalize(side);*/
////	}
////
////public:
////	glm::vec3 forward, up, side, viewUp, position; // 相机属性
////	GLfloat yawAngle, pitchAngle; // 欧拉角
////	GLfloat moveSpeed, mouse_sensitivity, mouse_zoom; // 相机选项
////};
//
//#endif

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
enum Camera_Movement {
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

class Camera
{
public:
	Camera(glm::vec3 pos = glm::vec3(0.0, 0.0, 2.0),
		glm::vec3 viewup = glm::vec3(0.0, 1.0, 0.0),
		glm::vec3 targetpos = glm::vec3(0.0, 0.0, 0.0))
		:Position(pos), TargetPos(targetpos), Viewup(viewup)
	{

	}



public:
	// 获取视变换矩阵
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(this->Position, this->TargetPos, this->Viewup);
	}

	glm::vec3 direction, vertical;
	GLfloat camX, camZ, radius;
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
	//GLfloat xAngle, yAngle;
};
#endif