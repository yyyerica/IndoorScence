// 引入GLEW库 定义静态链接
#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
// 引入GLFW库
#include <cstdlib>
#include <vector>
#include <iostream>  
#include <glm/vec3.hpp>// glm::vec3
#include <glm/geometric.hpp>// glm::cross, glm::normalize

// 引入SOIL库
#include <SOIL.h>
// 引入GLM库
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// 包含着色器加载库
#include "shader.h"
// 包含相机控制辅助类
#include "camera.h"
// 包含简单的obj读取文件
#include "simpleObjLoader.h"
//// 包含纹理加载类
//#include "texture.h"
// 定义程序常量
const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 800;
// 用于相机交互参数
GLfloat lastX = WINDOW_WIDTH / 2.0f, lastY = WINDOW_HEIGHT / 2.0f;

int mode = 0;//0平移，1旋转
glm::vec3 rotateVec = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouseMove = true;

MyObjLoader objloader;
Camera camera;

//旋转
GLfloat angley = 0.0f;
GLfloat anglex = 0.0f;

// 鼠标按下事件
//void cursor_position_callback(GLFWwindow* window, double x, double y);
// 鼠标移动回调函数原型声明
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
// 鼠标滚轮回调函数原型声明
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 左右按键
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int main(int argc, char** argv)
{

	if (!glfwInit())	// 初始化glfw库
	{
		std::cout << "Error::GLFW could not initialize GLFW!" << std::endl;
		return -1;
	}

	// 开启OpenGL 3.3 core profile
	std::cout << "Start OpenGL core profile version 3.3" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
		"ex3", NULL, NULL);
	if (!window)
	{
		std::cout << "Error::GLFW could not create winddow!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 创建的窗口的context指定为当前context
	glfwMakeContextCurrent(window);

	// 注册鼠标移动事件回调函数
	glfwSetCursorPosCallback(window, mouse_move_callback);
	// 注册鼠标滚轮事件回调函数
	glfwSetScrollCallback(window, mouse_scroll_callback);
	//// 注册按下事件回调函数
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// 鼠标拖动事件
	//glfwSetCursorPosCallback(window, cursor_position_callback);

	// 初始化GLEW 获取OpenGL函数
	glewExperimental = GL_TRUE; // 让glew获取所有拓展函数
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
			<< " error string:" << glewGetErrorString(status) << std::endl;
		glfwTerminate();
		return -1;
	}

	// 设置视口参数 (-1到1
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//视口变换将位于-1.0到1.0范围的坐标变换到由glViewport函数所定义的坐标范围内

	//dragon
	vector<Vertex> dragonvertData;
	if (!objloader.loadFromFile("dragon.obj", dragonvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}

	//桌子
	vector<Vertex> vertData;
	if (!objloader.loadFromFile("file.obj", vertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}

	//椅子
	vector<Vertex> ChairvertData;
	if (!objloader.loadFromFile("obj.obj", ChairvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}

	//灯
	vector<Vertex> vertData1;
	vec3 v1, v2, v3, v4;
	v1 = vec3(0.0f, 1.0f, 0.0f);
	v2 = vec3(-1.0f, 0.0f, 0.0f);
	v3 = vec3(1.0f, 0.0f, 0.0f);
	v4 = vec3(0.0f, 0.0f, 1.0f);
	vec3 normal1 = normalize(cross(v2 - v1, v3 - v1));
	vec3 normal2 = normalize(cross(v4 - v1, v3 - v1));
	vec3 normal3 = normalize(cross(v2 - v1, v4 - v1));
	vec3 normal4 = normalize(cross(v3 - v2, v4 - v2));

	Vertex ver1, ver2, ver3, ver4, ver5, ver6, ver7, ver8, ver9, ver10, ver11, ver12;
	ver1.position = v1;
	ver1.normal = normal1;
	ver2.position = v2;
	ver2.normal = normal1;
	ver3.position = v3;
	ver3.normal = normal1;
	ver4.position = v1;
	ver4.normal = normal2;
	ver5.position = v4;
	ver5.normal = normal2;
	ver6.position = v3;
	ver6.normal = normal2;
	ver7.position = v1;
	ver7.normal = normal3;
	ver8.position = v2;
	ver8.normal = normal3;
	ver9.position = v4;
	ver9.normal = normal3;
	ver10.position = v2;
	ver10.normal = normal1;
	ver11.position = v3;
	ver11.normal = normal1;
	ver12.position = v4;
	ver12.normal = normal1;

	vertData1.push_back(ver1);
	vertData1.push_back(ver2);
	vertData1.push_back(ver3);
	vertData1.push_back(ver4);
	vertData1.push_back(ver5);
	vertData1.push_back(ver6);
	vertData1.push_back(ver7);
	vertData1.push_back(ver8);
	vertData1.push_back(ver9);
	vertData1.push_back(ver10);
	vertData1.push_back(ver11);
	vertData1.push_back(ver12);


	vector<Vertex> earthvertData;
	if (!objloader.loadFromFile("earth_03.obj", earthvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}

	//扫面花瓶
	/*vector<Vertex> vertDatavase;
	vec3 v1, v2, v3, v4;
	v1 = vec3(-0.5f, 1.0f, 0.0f);
	v2 = vec3(0.0f, 1.0f, 0.0f);
	v3 = vec3(0.0f, 0.0f, 0.0f);
	v4 = vec3(-1.0f, 0.0f, 0.0f);
	vec3 normal1 = normalize(cross(v2 - v1, v3 - v1));
	vec3 normal2 = normalize(cross(v4 - v1, v3 - v1));
	
	Vertex ver1, ver2, ver3, ver4, ver5, ver6, ver7, ver8, ver9, ver10, ver11, ver12;
	ver1.position = v1;
	ver1.normal = normal1;
	ver2.position = v2;
	ver2.normal = normal1;
	ver3.position = v3;
	ver3.normal = normal1;
	ver4.position = v1;
	ver4.normal = normal2;
	ver5.position = v4;
	ver5.normal = normal2;
	ver6.position = v3;
	ver6.normal = normal2;
	ver7.position = v1;
	ver7.normal = normal3;
	ver8.position = v2;
	ver8.normal = normal3;
	ver9.position = v4;
	ver9.normal = normal3;
	ver10.position = v2;
	ver10.normal = normal1;
	ver11.position = v3;
	ver11.normal = normal1;
	ver12.position = v4;
	ver12.normal = normal1;

	vertData1.push_back(ver1);
	vertData1.push_back(ver2);
	vertData1.push_back(ver3);
	vertData1.push_back(ver4);
	vertData1.push_back(ver5);
	vertData1.push_back(ver6);
	vertData1.push_back(ver7);
	vertData1.push_back(ver8);
	vertData1.push_back(ver9);
	vertData1.push_back(ver10);
	vertData1.push_back(ver11);
	vertData1.push_back(ver12);*/

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	Shader shaderdragon("cube.vertex", "cube.frag");
	Shader shader("cube.vertex", "cube.frag");
	Shader shader1("cube.vertex", "cube.frag");
	Shader shaderChair("cube.vertex", "cube.frag");
	Shader shaderearth("cube.vertex", "cube.frag");
	// 开始主循环
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // 处理例如鼠标 键盘等事件
		//do_movement(); // 根据用户操作情况 更新相机属性

		// 清除颜色缓冲区 重置为指定颜色
		glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Mesh mesh(vertData);
		Mesh mesh1(vertData1);
		Mesh meshChair(ChairvertData);
		Mesh meshEarth(earthvertData);
		Mesh meshdragon(dragonvertData);

		shaderdragon.use();

		glm::mat4 projectiondragon = glm::perspective(camera.mouse_zoom, //→缩放
			(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // 投影矩阵
		glm::mat4 viewdragon = camera.getViewMatrix(); // 视变换矩阵 →lookAt  主要移动旋转在这里

		glm::mat4 projection_viewdragon = projectiondragon*viewdragon;
		glUniformMatrix4fv(glGetUniformLocation(shaderdragon.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_viewdragon));

		glm::mat4 modeldragon;
		modeldragon = glm::translate(modeldragon, glm::vec3(-0.2f, -0.10f, 0.0f));
		modeldragon = glm::scale(modeldragon, glm::vec3(0.1f, 0.1f, 0.1f));
		//modeldragon = glm::rotate(modeldragon, 0.55f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderdragon.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modeldragon));

		shader.use();

		glm::mat4 projection = glm::perspective(camera.mouse_zoom, //→缩放
			(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // 投影矩阵
		glm::mat4 view = camera.getViewMatrix(); // 视变换矩阵 →lookAt  主要移动旋转在这里

		glm::mat4 projection_view = projection*view;
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0005f, 0.0005f, 0.0005f));
		model = glm::rotate(model, 0.55f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model));

		shader1.use();
		glm::mat4 projection1 = glm::perspective(camera.mouse_zoom, //→缩放
			(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // 投影矩阵
		glm::mat4 view1 = camera.getViewMatrix(); // 视变换矩阵 →lookAt  主要移动旋转在这里
		glm::mat4 projection_view1 = projection1*view1;
		glUniformMatrix4fv(glGetUniformLocation(shader1.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view1));
		glm::mat4 model1;
		model1 = glm::translate(model1, glm::vec3(0.0f, 0.8f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, anglex, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader1.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model1));

		shaderChair.use(); 
		glm::mat4 projection2 = glm::perspective(camera.mouse_zoom, //→缩放
			(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // 投影矩阵
		glm::mat4 view2 = camera.getViewMatrix(); // 视变换矩阵 →lookAt  主要移动旋转在这里
		glm::mat4 projection_view2 = projection2*view2;
		glUniformMatrix4fv(glGetUniformLocation(shaderChair.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view2));
		glm::mat4 model2;
		model2 = glm::translate(model2, glm::vec3(-0.6f, -0.3f, 0.0f));
		model2 = glm::scale(model2, glm::vec3(0.0003f, 0.0003f, 0.0003f));
		//model = glm::rotate(model, anglex, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderChair.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model2));

		shaderearth.use();
		glm::mat4 projection3 = glm::perspective(camera.mouse_zoom, //→缩放
			(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // 投影矩阵
		glm::mat4 view3 = camera.getViewMatrix(); // 视变换矩阵 →lookAt  主要移动旋转在这里
		glm::mat4 projection_view3 = projection3*view3;
		glUniformMatrix4fv(glGetUniformLocation(shaderearth.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view3));
		glm::mat4 model3;
		model3 = glm::translate(model3, glm::vec3(0.0f, -0.13f, 0.0f));
		model3 = glm::scale(model3, glm::vec3(0.005f, 0.005f, 0.005f));
		//model = glm::rotate(model, anglex, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderearth.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model3));

		//光源位置
		GLint lightPosLoc = glGetUniformLocation(shader.programId, "lightPos");
		GLint lightPosLoc1 = glGetUniformLocation(shader1.programId, "lightPos");
		GLint lightPosLoc2 = glGetUniformLocation(shaderChair.programId, "lightPos");
		GLint lightPosLoc3 = glGetUniformLocation(shaderearth.programId, "lightPos");
		GLint lightPosLoc4 = glGetUniformLocation(shaderearth.programId, "lightPos");
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightPosLoc, 0, 2, 0);
		glUniform3f(lightPosLoc1, 0, 2, 0);
		glUniform3f(lightPosLoc2, 0, 2, 0);
		glUniform3f(lightPosLoc3, 0, 2, 0);
		glUniform3f(lightPosLoc4, 0, 2, 0);
		// 这里填写场景绘制代码
		mesh.draw(shader); // 绘制物体
		meshChair.draw(shaderChair); // 绘制物体
		mesh1.draw(shader1); // 绘制物体
		meshEarth.draw(shaderearth); // 绘制物体
		meshdragon.draw(shaderdragon);

		glBindVertexArray(0);
		glUseProgram(0);
		glfwSwapBuffers(window); // 交换缓存
	}
	// 释放资源
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // 关闭窗口
	}
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	double x = (2 * xpos) / WINDOW_WIDTH - 1;
	double y = 1 - (2 * ypos) / WINDOW_HEIGHT;
	if (mode == 0)
	{
		camera.handleMouseTranslation(x, y);
	}
	else if (mode == 1)
	{

		GLfloat xoffset = lastX - xpos;
		GLfloat yoffset = lastY - ypos;

		xoffset *= 0.05;
		yoffset *= 0.05;

		angley += 360 * (GLfloat)xoffset / (GLfloat)WINDOW_WIDTH;//根据屏幕上鼠标滑动的距离来设置旋转的角度  
		anglex += 360 * (GLfloat)yoffset / (GLfloat)WINDOW_HEIGHT;//根据屏幕上鼠标滑动的距离来设置旋转的角度  

		lastX = xpos;
		lastY = ypos;

	}

}

// 由相机辅助类处理鼠标滚轮控制
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.handleMouseScroll(yoffset);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		//拉普拉斯
		//Rightclick = true;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		//切换移动旋转
		if (mode == 0)
			mode = 1;
		else if (mode == 1) {
			mode = 0;
		}

		break;
	default:
		return;
	}
	return;
}
