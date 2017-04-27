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
#include "texture.h"
// 定义程序常量
const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 800;
// 用于相机交互参数
GLfloat lastX = WINDOW_WIDTH / 2.0f, lastY = WINDOW_HEIGHT / 2.0f;

bool firstMouseMove = true;

MyObjLoader objloader;
Camera camera;

//旋转
GLfloat angley = 0.0f;
GLfloat anglex = 0.0f;

int mode = 0;

//光源
vec3 lightspot = vec3(0.0f, 0.0f, -2.0f);
GLint texturewallfront, texturewallfront1;
vector<Vertex> wallDatafront;


// 鼠标按下事件
//void cursor_position_callback(GLFWwindow* window, double x, double y);
// 鼠标移动回调函数原型声明
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
// 鼠标滚轮回调函数原型声明
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 左右按键
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void keyFun(GLFWwindow* pWnd, int nKey, int nScanCode, int nAction, int nMode);
vector<Vertex> getvase();
mat4 shadowmat();


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
	glfwSetKeyCallback(window, keyFun);
	// 注册鼠标移动事件回调函数
	//glfwSetCursorPosCallback(window, mouse_move_callback);
	// 注册鼠标滚轮事件回调函数
	//glfwSetScrollCallback(window, mouse_scroll_callback);
	//// 注册按下事件回调函数
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// 鼠标拖动事件
	//glfwSetCursorPosCallback(window, cursor_position_callback);

	void keyFun(GLFWwindow* pWnd, int nKey, int nScanCode, int nAction, int nMode);
	

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

	vector<Vertex> VaseData = getvase();
	GLint textureVase = TextureHelper::load2DTexture("vase.jpg");

	//桌子
	vector<Vertex> vertData;
	if (!objloader.loadFromFile("file.obj", vertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	GLint textureTable = TextureHelper::load2DTexture("table.jpg");

	//椅子
	vector<Vertex> ChairBackvertData;
	if (!objloader.loadFromFile("chairBack.obj", ChairBackvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	GLint textureChairBack = TextureHelper::load2DTexture("chair.jpg");

	vector<Vertex> ChairFrontvertData;
	if (!objloader.loadFromFile("chairFront.obj", ChairFrontvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	//GLint textureChairFront = TextureHelper::load2DTexture("chair.jpg");

	//灯
	vector<Vertex> vertDataLight;
	if (!objloader.loadFromFile("light.obj", vertDataLight))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}

	//地球
	vector<Vertex> earthvertData;
	if (!objloader.loadFromFile("earthMain.obj", earthvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	GLint textureEarth = TextureHelper::load2DTexture("earth.jpg");

	vector<Vertex> earthvertData2;
	if (!objloader.loadFromFile("earthBack.obj", earthvertData2))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}

	//墙壁
	vector<Vertex> wallDataleft;
	if (!objloader.loadFromFile("wallleft.obj", wallDataleft))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	GLint texturewallround = TextureHelper::load2DTexture("wallround.jpg");

	vector<Vertex> wallDataright;
	if (!objloader.loadFromFile("wallright.obj", wallDataright))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	

	if (!objloader.loadFromFile("wallfront.obj", wallDatafront))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	texturewallfront = TextureHelper::load2DTexture("window.png");
	texturewallfront1 = TextureHelper::load2DTexture("windownight.png");

	vector<Vertex> wallDataup;
	if (!objloader.loadFromFile("wallup.obj", wallDataup))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	GLint texturewallupdown = TextureHelper::load2DTexture("wallupdown.jpg");


	vector<Vertex> wallDatadown;
	if (!objloader.loadFromFile("walldown.obj", wallDatadown))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	


	Mesh meshdragon(dragonvertData);
	Mesh meshdragonshadow(dragonvertData);

	Mesh meshwallup(wallDataup, texturewallupdown);
	Mesh meshwalldown(wallDatadown, texturewallupdown);
	Mesh meshwallleft(wallDataleft, texturewallround);
	Mesh meshwallright(wallDataright, texturewallround);
	
	Mesh meshtable(vertData, textureTable);
	Mesh meshtableshadow(vertData, textureTable);

	Mesh meshLight(vertDataLight);
	Mesh meshChairB(ChairBackvertData, textureChairBack);
	Mesh meshChairF(ChairFrontvertData);
	Mesh meshEarth(earthvertData, textureEarth);
	Mesh meshEarthshadow(earthvertData, textureEarth);
	Mesh meshEarth2(earthvertData2);
	Mesh meshEarth2shadow(earthvertData2);
	Mesh meshvase(VaseData, textureVase);


	Shader shaderdragon("cube.vertex", "dragon.frag");
	Shader shaderdragonshadow("shadow.vertex", "shadow.frag");
	
	Shader shadertableshadow("shadow.vertex", "shadow.frag");
	Shader shadertable("cube.vertex", "cube.frag");
	
	Shader shaderLight("cube.vertex", "light.frag");
	
	Shader shaderearthshadow("shadow.vertex", "shadow.frag");
	Shader shaderearth("cube.vertex", "cube.frag");
	Shader shaderearthBack("cube.vertex", "earthBack.frag");
	Shader shaderearthBackshadow("shadow.vertex", "shadow.frag");

	Shader shaderwallleft("cube.vertex", "cube.frag");
	Shader shaderwallright("cube.vertex", "cube.frag");

	//Shader shaderChairB("cube.vertex", "cube.frag");
	Shader shaderChairB("cube.vertex", "cube.frag");
	Shader shaderChairF("cube.vertex", "chairF.frag");
	//Shader shaderChairF("cube.vertex", "chairF.frag");
	Shader shaderwallfront("cube.vertex", "cube.frag");
	Shader shaderwallup("cube.vertex", "cube.frag");
	Shader shaderwalldown("cube.vertex", "cube.frag");

	Shader shadervase("cube.vertex", "cube.frag");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//透明
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //指定混合函数  
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);     //启用混合状态  
	glEnable(GL_LIGHTING);      //启用光照  
	glEnable(GL_LIGHT0);        //打开光源0  
	glEnable(GL_DEPTH_TEST);    //启用深度检测  
	glEnable(GL_COLOR_MATERIAL);//材质跟踪当前绘图色  

	// 开始主循环
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // 处理例如鼠标 键盘等事件
		//do_movement(); // 根据用户操作情况 更新相机属性

		// 清除颜色缓冲区 重置为指定颜色
		glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//glm::mat4 projection = glm::perspective(camera.mouse_zoom, //→缩放
		//	(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // 投影矩阵
		mat4 projection = glm::perspective(45.0f, //→缩放
			(GLfloat)(WINDOW_WIDTH) / WINDOW_HEIGHT, 1.0f, 100.0f); // 投影矩阵
		mat4 view = camera.getViewMatrix(); // 视变换矩阵 →lookAt  主要移动旋转在这里
		mat4 projection_view = projection*view;
		mat4 shadowModel = shadowmat();

		if (mode == 0)
		{
			Mesh meshwallfront(wallDatafront, texturewallfront);
			shaderwallfront.use();
			glUniformMatrix4fv(glGetUniformLocation(shaderwallfront.programId, "projection_view"),
				1, GL_FALSE, glm::value_ptr(projection_view));
			glm::mat4 modelwallfront;
			//modelwall = glm::rotate(modelwall, angley, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shaderwallfront.programId, "model"),
				1, GL_FALSE, glm::value_ptr(modelwallfront));
			glUniform1i(glGetUniformLocation(shaderwallfront.programId, "tex"), 0); // 设置纹理单元为0号
			GLint lightPosLocwall3 = glGetUniformLocation(shaderwallfront.programId, "lightPos");
			glUniform3f(lightPosLocwall3, lightspot.x, lightspot.y, lightspot.z);
			/*glUniformMatrix4fv(glGetUniformLocation(shaderwallfront.programId, "shadowmodel"),
				1, GL_TRUE, glm::value_ptr(mat4(1.0)));*/
			meshwallfront.draw(shaderwallfront);
		}
		else
		{
			Mesh meshwallfront(wallDatafront, texturewallfront1);
			shaderwallfront.use();
			glUniformMatrix4fv(glGetUniformLocation(shaderwallfront.programId, "projection_view"),
				1, GL_FALSE, glm::value_ptr(projection_view));
			glm::mat4 modelwallfront;
			//modelwall = glm::rotate(modelwall, angley, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shaderwallfront.programId, "model"),
				1, GL_FALSE, glm::value_ptr(modelwallfront));
			glUniform1i(glGetUniformLocation(shaderwallfront.programId, "tex"), 0); // 设置纹理单元为0号
			GLint lightPosLocwall3 = glGetUniformLocation(shaderwallfront.programId, "lightPos");
			glUniform3f(lightPosLocwall3, lightspot.x, lightspot.y, lightspot.z);
			/*glUniformMatrix4fv(glGetUniformLocation(shaderwallfront.programId, "shadowmodel"),
				1, GL_TRUE, glm::value_ptr(mat4(1.0)));*/
			meshwallfront.draw(shaderwallfront);
		}

		shaderwallleft.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderwallleft.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 modelwallleft;
		//modelwallleft = glm::rotate(modelwallleft, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderwallleft.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modelwallleft));
		glUniform1i(glGetUniformLocation(shaderwallleft.programId, "tex"), 0); // 设置纹理单元为0号
		GLint lightPosLocwall1 = glGetUniformLocation(shaderwallleft.programId, "lightPos");
		glUniform3f(lightPosLocwall1, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderwallleft.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(mat4(1.0)));*/
		meshwallleft.draw(shaderwallleft);

		shadervase.use();
		glUniformMatrix4fv(glGetUniformLocation(shadervase.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 modelvase;
		modelvase = glm::translate(modelvase, glm::vec3(0.2f, -0.25f, -0.1f));
		modelvase = glm::scale(modelvase, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(shadervase.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modelvase));
		glUniform1i(glGetUniformLocation(shadervase.programId, "tex"), 0); // 设置纹理单元为0号
		GLint lightPosLocvase = glGetUniformLocation(shadervase.programId, "lightPos");
		glUniform3f(lightPosLocwall1, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderwallleft.programId, "shadowmodel"),
		1, GL_TRUE, glm::value_ptr(mat4(1.0)));*/
		meshvase.draw(shadervase);


		shaderwallright.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderwallright.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 modelwallright;
		//modelwall = glm::rotate(modelwall, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderwallright.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modelwallright));
		glUniform1i(glGetUniformLocation(shaderwallright.programId, "tex"), 0); // 设置纹理单元为0号
		GLint lightPosLocwall2 = glGetUniformLocation(shaderwallright.programId, "lightPos");
		glUniform3f(lightPosLocwall2, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderwallright.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(mat4(1.0)));*/
		meshwallright.draw(shaderwallright);
		

		shaderwallup.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderwallup.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 modelwallup;
		//modelwall = glm::rotate(modelwall, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderwallup.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modelwallup));
		glUniform1i(glGetUniformLocation(shaderwallup.programId, "tex"), 0); // 设置纹理单元为0号
		GLint lightPosLocwall4 = glGetUniformLocation(shaderwallup.programId, "lightPos");
		glUniform3f(lightPosLocwall4, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderwallup.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(mat4(1.0)));*/
		meshwallup.draw(shaderwallup);


		shaderwalldown.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderwalldown.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 modelwalldown;
		//modelwall = glm::rotate(modelwall, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderwalldown.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modelwalldown));
		glUniform1i(glGetUniformLocation(shaderwalldown.programId, "tex"), 0); // 设置纹理单元为0号
		GLint lightPosLocwall5 = glGetUniformLocation(shaderwalldown.programId, "lightPos");
		glUniform3f(lightPosLocwall5, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderwalldown.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(mat4(1.0)));*/
		meshwalldown.draw(shaderwalldown);


		shaderdragon.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderdragon.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 modeldragon;
		modeldragon = glm::translate(modeldragon, glm::vec3(-0.2f, -0.2f, -0.3f));
		modeldragon = glm::scale(modeldragon, glm::vec3(0.1f, 0.1f, 0.1f));
		modeldragon = glm::rotate(modeldragon, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderdragon.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modeldragon));
		GLint lightPosLocDragon = glGetUniformLocation(shaderdragon.programId, "lightPos");
		glUniform3f(lightPosLocDragon, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderdragon.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(shadowModel));*/
		meshdragon.draw(shaderdragon);

		shaderdragonshadow.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderdragonshadow.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glUniformMatrix4fv(glGetUniformLocation(shaderdragonshadow.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(shadowModel));
		glUniformMatrix4fv(glGetUniformLocation(shaderdragonshadow.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modeldragon));
		meshdragonshadow.draw(shaderdragonshadow);

		shadertable.use();
		glUniformMatrix4fv(glGetUniformLocation(shadertable.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, -0.3f));
		model = glm::scale(model, glm::vec3(0.0005f, 0.0005f, 0.0005f));
		model = glm::rotate(model, angley + 0.55f, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, anglex, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shadertable.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model));
		GLint lightPosLoc = glGetUniformLocation(shadertable.programId, "lightPos");
		glUniform3f(lightPosLoc, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shadertable.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(shadowModel));*/
		meshtable.draw(shadertable); // 绘制物体


		shaderLight.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderLight.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 modelLight;
		modelLight = glm::translate(modelLight, glm::vec3(0.0f, 0.8f, -0.3f));
		modelLight = glm::scale(modelLight, glm::vec3(0.1f, 0.1f, 0.1f));
		modelLight = glm::rotate(modelLight, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		//modelLight = glm::rotate(modelLight, anglex, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderLight.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modelLight));
		GLint lightPosLocLight = glGetUniformLocation(shaderLight.programId, "lightPos");
		glUniform3f(lightPosLocLight, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderLight.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(mat4(1.0)));*/
		meshLight.draw(shaderLight); // 绘制物体


		shaderChairB.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderChairB.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 modelB;
		modelB = glm::translate(modelB, glm::vec3(-0.6f, -0.6f, -0.3f));
		modelB = glm::scale(modelB, glm::vec3(0.0006f, 0.0006f, 0.0006f));
		modelB = glm::rotate(modelB, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderChairB.programId, "model"),
			1, GL_FALSE, glm::value_ptr(modelB));
		GLint lightPosLocB = glGetUniformLocation(shaderChairB.programId, "lightPos");
		glUniform3f(lightPosLocB, lightspot.x, lightspot.y, lightspot.z);
		glUniformMatrix4fv(glGetUniformLocation(shaderChairB.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(shadowModel));
		meshChairB.draw(shaderChairB); // 绘制物体

		shaderChairF.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderChairF.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 model2;
		model2 = glm::translate(model2, glm::vec3(-0.6f, -0.6f, -0.3f));
		model2 = glm::scale(model2, glm::vec3(0.0006f, 0.0006f, 0.0006f));
		model2 = glm::rotate(model2, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderChairF.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model2));
		GLint lightPosLoc2 = glGetUniformLocation(shaderChairF.programId, "lightPos");
		glUniform3f(lightPosLoc2, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderChairF.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(shadowmat()));*/
		meshChairF.draw(shaderChairF); // 绘制物体


		shaderearth.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderearth.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 model3;
		model3 = glm::translate(model3, glm::vec3(0.0f, -0.23f, -0.3f));
		model3 = glm::scale(model3, glm::vec3(0.005f, 0.005f, 0.005f));
		model3 = glm::rotate(model3, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderearth.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model3));
		GLint lightPosLoc3 = glGetUniformLocation(shaderearth.programId, "lightPos");
		glUniform3f(lightPosLoc3, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderearth.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(shadowModel));*/
		meshEarth.draw(shaderearth); // 绘制物体
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

		shaderearthBack.use();
		glUniformMatrix4fv(glGetUniformLocation(shaderearthBack.programId, "projection_view"),
			1, GL_FALSE, glm::value_ptr(projection_view));
		glm::mat4 model3B;
		model3B = glm::translate(model3B, glm::vec3(0.0f, -0.23f, -0.3f));
		model3B = glm::scale(model3B, glm::vec3(0.005f, 0.005f, 0.005f));
		model3B = glm::rotate(model3B, angley, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderearthBack.programId, "model"),
			1, GL_FALSE, glm::value_ptr(model3B));
		GLint lightPosLoc3B = glGetUniformLocation(shaderearthBack.programId, "lightPos");
		glUniform3f(lightPosLoc3B, lightspot.x, lightspot.y, lightspot.z);
		/*glUniformMatrix4fv(glGetUniformLocation(shaderearthBack.programId, "shadowmodel"),
			1, GL_TRUE, glm::value_ptr(shadowModel));*/
		meshEarth2.draw(shaderearthBack); // 绘制物体


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

//void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	double x = (2 * xpos) / WINDOW_WIDTH - 1;
//	double y = 1 - (2 * ypos) / WINDOW_HEIGHT;
//	if (mode == 0)
//	{
//		camera.handleMouseTranslation(x, y);
//	}
//	else if (mode == 1)
//	{
//		//物体本身旋转
//		//GLfloat xoffset = lastX - xpos;
//		//GLfloat yoffset = lastY - ypos;
//
//		//xoffset *= 0.05;
//		//yoffset *= 0.05;
//
//		//angley += 360 * (GLfloat)xoffset / (GLfloat)WINDOW_WIDTH;//根据屏幕上鼠标滑动的距离来设置旋转的角度  
//		//anglex += 360 * (GLfloat)yoffset / (GLfloat)WINDOW_HEIGHT;//根据屏幕上鼠标滑动的距离来设置旋转的角度  
//
//		//lastX = xpos;
//		//lastY = ypos;
//
//		//视点移动旋转
//		camera.handleRotation(x);
//
//
//	}
//}

// 由相机辅助类处理鼠标滚轮控制
//void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	camera.handleMouseScroll(yoffset);
//}

mat4 shadowmat()
{
	mat4 model;
	vec3 lightdir;
	vec4 normalfloor = vec4(0.0f, 1.0f, 0.0f, 0.6f);
	if (mode == 0) // light 0.0,-0.5,-1.0 normalfloor x=0,y=0,z=1,w=0.6
	{
		lightdir = vec3(0.0f, 0.5f, -1.0f);
	}
	else// light 0.0,1.0,0.0 normalfloor a=0,b=0,c=1,d=0.6  //光线需要反过来
	{
		lightdir = vec3(0.0f, 1.0f, 0.0f);
	}

	model[0] = vec4(normalfloor.y*lightdir.y + normalfloor.z*lightdir.z,
		-normalfloor.x*lightdir.y, -normalfloor.x * lightdir.z, 0.0f);
	model[1] = vec4(-normalfloor.y*lightdir.x, normalfloor.x*lightdir.x + normalfloor.z*lightdir.z,
		-normalfloor.y*lightdir.z, 0.0f);
	model[2] = vec4(-normalfloor.z*lightdir.x, -normalfloor.z*lightdir.y, 
		normalfloor.x*lightdir.x + normalfloor.y * lightdir.y, 0.0f);
	model[3] = vec4(-normalfloor.w*lightdir.x, -normalfloor.w*lightdir.y, -normalfloor.w*lightdir.z, 
		normalfloor.x*lightdir.x + normalfloor.y*lightdir.y + normalfloor.z*lightdir.z);
	return model;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	//case GLFW_MOUSE_BUTTON_LEFT:
	//	//拉普拉斯
	//	//Rightclick = true;
	//	break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		//切换灯光
		if (mode == 0) //日光
		{
			//texturewallfront = TextureHelper::load2DTexture("window.png");
			lightspot = vec3(0.0f, 0.0f, -2.0f);
			//meshwallfront = new Mesh(wallDatafront, texturewallfront);
			mode = 1;
		}
			
		else if (mode == 1) //灯光
		{
			//texturewallfront = TextureHelper::load2DTexture("windownight.png");
			lightspot = vec3(0.0f, 1.0f, 0.0f);
			//meshwallfront(wallDatafront, texturewallfront);
			mode = 0;
		}

		break;
	default:
		return;
	}
	return;
}


void keyFun(GLFWwindow* pWnd, int nKey, int nScanCode, int nAction, int nMode){

	if (nAction == GLFW_PRESS)
	{
		//平移
		if (nKey == GLFW_KEY_Q)
		{
			camera.handleTranslation(FORWARD);
		}
		else if (nKey == GLFW_KEY_E)
		{
			camera.handleTranslation(BACKWARD);
		}
		else if (nKey == GLFW_KEY_A)
		{
			camera.handleTranslation(RIGHT);
		}
		else if (nKey == GLFW_KEY_D)
		{
			camera.handleTranslation(LEFT);
		}
		else if (nKey == GLFW_KEY_W)
		{
			camera.handleTranslation(DOWN);
		}
		else if (nKey == GLFW_KEY_S)
		{
			camera.handleTranslation(UP);
		}

		//旋转
		else if (nKey == GLFW_KEY_Z) //y轴顺时针
		{
			camera.handleTranslation(yAnticlockwise);
		}
		else if (nKey == GLFW_KEY_C)//y轴逆时针
		{
			camera.handleTranslation(yClockwise);
		}
		else if (nKey == GLFW_KEY_X)    // 还原视图
		{
			camera.handleTranslation(REBOUND);
		}
	}
}

vector<Vertex> getvase() 
{
	vector<Vertex> vase;
	for (int i = 0; i<30; i++) {
		Vertex point;
		point.position = vec3(0.5 * sin(0.2 * i) + 1, 0.1 * i, 0);
		point.texCoords = vec2(0, 0.033*i);
		vase.push_back(point);
	}
	mat3 rotate(cos(0.314), 0, -sin(0.314), 0, 1, 0, sin(0.314), 0, cos(0.314));
	for (int i = 1; i < 20; i++) {
		for (int a = 0; a < 30; a++) {
			Vertex point;
			point.position = vase[30 * i + a - 30].position * rotate;
			point.texCoords = vase[30 * i + a - 30].texCoords + vec2(0.05, 0);
			vase.push_back(point);
		}
	}
	for (int i = 0; i < vase.size() - 30; i++) {
		if (i < vase.size() - 30) {
			if (i % 30 != 0) {
				vec3 normal = cross(vase[i].position - vase[i + 29].position, vase[i + 30].position - vase[i + 29].position);
				vase[i].normal += normal;
				vase[i + 29].normal += normal;
				vase[i + 30].normal += normal;
			}
			if (i % 30 != 29) {
				vec3 normal = cross(vase[i].position - vase[i + 30].position, vase[i + 1].position - vase[i + 30].position);
				vase[i].normal += normal;
				vase[i + 1].normal += normal;
				vase[i + 30].normal += normal;
			}
		}
		else {
			if (i % 30 != 0) {
				vec3 normal = cross(vase[i].position - vase[i % 30 - 1].position, vase[i % 30].position - vase[i % 30 - 1].position);
				vase[i].normal += normal;
				vase[i % 30 - 1].normal += normal;
				vase[i % 30].normal += normal;
			}
			if (i % 30 != 29) {
				vec3 normal = cross(vase[i].position - vase[i % 30].position, vase[i + 1].position - vase[i % 30].position);
				vase[i].normal += normal;
				vase[i + 1].normal += normal;
				vase[i % 30].normal += normal;
			}
		}
	}

	for (int i = 0; i < vase.size(); i++) {
		if (i % 30 != 0 && i % 30 != 29) {
			vase[i].normal /= 6;
		}
		else {
			vase[i].normal /= 3;
		}
	}
	vector<Vertex> vese;
	for (int i = 0; i < vase.size(); i++) {
		if (i<vase.size() - 30) {
			if (i % 30 != 0) {
				vese.push_back(vase[i + 29]);
				vese.push_back(vase[i]);
				vese.push_back(vase[i + 30]);
			}
			if (i % 30 != 29) {
				vese.push_back(vase[i + 30]);
				vese.push_back(vase[i]);
				vese.push_back(vase[i + 1]);
			}
		}
		else {
			if (i % 30 != 0) {
				vese.push_back(vase[i % 30 - 1]);
				vese.push_back(vase[i]);
				vese.push_back(vase[i % 30]);
			}
			if (i % 30 != 29) {
				vese.push_back(vase[i % 30]);
				vese.push_back(vase[i]);
				vese.push_back(vase[i + 1]);

			}
		}
	}
	return vese;
}
