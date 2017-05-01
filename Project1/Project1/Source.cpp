// GLEW
#define GLEW_STATIC
#include <GLEW/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

#include "camera.h"
#include "shader.h"
#include "simpleObjLoader.h"

// Properties
const GLuint SCR_WIDTH = 1200, SCR_HEIGHT = 1000;

// Function prototypes
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

GLuint loadTexture(GLchar* path);
void RenderScene(Shader &shader);
void RenderDRAGON(Shader &shader);
void RenderCUBE(Shader &shader, vector<Vertex>);
void RenderQuad();

vector<Vertex> getvase();
// Camera
Camera camera;

// Delta
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int mode = 0;
int mode1 = 0;

glm::vec3 lightspot(0.0f, 0.0f, 3.0f);
// Options
GLboolean shadows = true;

// Global variables
GLuint woodTexture;
GLuint planeVAO;

glm::mat4 model = glm::mat4();

MyObjLoader objloader;
vector<Vertex> dragonvertData, VaseData, tableData, ChairBackvertData, ChairFrontvertData, vertDataLight, earthvertData, earthvertData2;
vector<Vertex> wallDataup, wallDatadown, wallDataleft, wallDataright, wallDatafront;
// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwMakeContextCurrent(window);

	// 注册鼠标移动事件回调函数
	glfwSetCursorPosCallback(window, mouse_move_callback);
	// 注册鼠标滚轮事件回调函数
	glfwSetScrollCallback(window, mouse_scroll_callback);
	//// 注册按下事件回调函数
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);


	if (!objloader.loadFromFile("dragon.obj", dragonvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	if (!objloader.loadFromFile("walldown.obj", wallDatadown))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	VaseData = getvase();
	if (!objloader.loadFromFile("file.obj", tableData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	if (!objloader.loadFromFile("chairBack.obj", ChairBackvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	if (!objloader.loadFromFile("chairFront.obj", ChairFrontvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	if (!objloader.loadFromFile("light.obj", vertDataLight))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	if (!objloader.loadFromFile("earthMain.obj", earthvertData))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	if (!objloader.loadFromFile("earthBack.obj", earthvertData2))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
	if (!objloader.loadFromFile("wallleft.obj", wallDataleft))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}
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
	if (!objloader.loadFromFile("wallup.obj", wallDataup))
	{
		std::cerr << "Could not load obj model, exit now.";
		std::system("pause");
		exit(-1);
	}


	// Setup and compile our shaders
	Shader shader("shadow_mapping.vertex", "shadow_mapping.frag");
	Shader simpleDepthShader("shadow_mapping_depth.vertex", "shadow_mapping_depth.frag");
	Shader debugDepthQuad("debug_quad.vertex", "debug_quad_depth.frag");

	// Set texture samples
	shader.Use();
	glUniform1i(glGetUniformLocation(shader.programId, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(shader.programId, "shadowMap"), 1);


	// Setup plane VAO
	GLuint planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* wallDatadown.size(),
		&wallDatadown[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (GLvoid*)(5 * sizeof(GL_FLOAT)));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Load textures
	woodTexture = loadTexture("wallupdown.jpg");

	// Configure depth map FBO
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// - Create depth texture
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();

		// Change light position over time
		lightspot.x = sin(glfwGetTime()) * 3.0f;
		lightspot.z = cos(glfwGetTime()) * 2.0f;
		lightspot.y = 5.0 + cos(glfwGetTime()) * 1.0f;

		// 1. Render depth of scene to texture (from light's perspective)
		// - Get light projection/view matrix.
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		GLfloat near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
		lightView = glm::lookAt(lightspot, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// - now render scene from light's point of view
		simpleDepthShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.programId, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		RenderScene(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. Render scene as normal 
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Use();
		glm::mat4 projection = glm::perspective(camera.mouse_zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// Set light uniforms
		glUniform3fv(glGetUniformLocation(shader.programId, "lightPos"), 1, &lightspot[0]);
		glUniform3fv(glGetUniformLocation(shader.programId, "viewPos"), 1, &camera.Position[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		// Enable/Disable shadows by pressing 'SPACE'
		glUniform1i(glGetUniformLocation(shader.programId, "shadows"), shadows);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderScene(shader);

		// Swap the buffers
		glfwSwapBuffers(window);
		model = glm::mat4();
	}

	glfwTerminate();
	return 0;
}

void RenderScene(Shader &shader)
{
	// Floor
	glm::mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, wallDatadown.size());
	glBindVertexArray(0);
	// Cubes
	RenderDRAGON(shader);
	RenderCUBE(shader,tableData);
}



// RenderCube() Renders a 1x1 3D cube in NDC.
GLuint DRAGONVAO = 0;
GLuint DRAGONVBO = 0;
void RenderDRAGON(Shader &shader)
{
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// Initialize (if necessary)
	if (DRAGONVAO == 0)
	{
		glGenVertexArrays(1, &DRAGONVAO);
		glGenBuffers(1, &DRAGONVBO);
		glBindVertexArray(DRAGONVAO);
		glBindBuffer(GL_ARRAY_BUFFER, DRAGONVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* dragonvertData.size(),
			&dragonvertData[0], GL_STATIC_DRAW);
		
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// Render Cube
	glBindVertexArray(DRAGONVAO);
	glDrawArrays(GL_TRIANGLES, 0, dragonvertData.size());
	glBindVertexArray(0);
}

GLuint TABLEVAO = 0;
GLuint TABLEVBO = 0;
void RenderCUBE(Shader &shader, vector<Vertex> cubeData)
{
	model = glm::mat4();
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
	if (TABLEVAO == 0)
	{
		glGenVertexArrays(1, &TABLEVAO);
		glGenBuffers(1, &TABLEVBO);
		glBindVertexArray(TABLEVAO);
		glBindBuffer(GL_ARRAY_BUFFER, TABLEVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* cubeData.size(),
			&cubeData[0], GL_STATIC_DRAW);

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
	// Render Cube
	glBindVertexArray(TABLEVAO);
	glDrawArrays(GL_TRIANGLES, 0, cubeData.size());
	glBindVertexArray(0);

}


// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
	// Generate texture ID and load texture data 
	//GLuint textureID;
	//glGenTextures(1, &textureID);
	//int width, height;
	//unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	//// Assign texture to ID
	//glBindTexture(GL_TEXTURE_2D, textureID);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D);

	//// Parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//SOIL_free_image_data(image);
	//return textureID;
	
	
	// Step1 创建并绑定纹理对象
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	// Step2 设定wrap参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Step3 设定filter参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR); // 为MipMap设定filter方法
	// Step4 加载纹理
	GLubyte *imageData = NULL;
	int picWidth, picHeight;
	imageData = SOIL_load_image(path, &picWidth, &picHeight, 0, SOIL_LOAD_RGB);
	if (imageData == NULL)
	{
		std::cerr << "Error::Texture could not load texture file:" << path << std::endl;
		return 0;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picWidth, picHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Step5 释放纹理图片资源
	SOIL_free_image_data(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

bool keys[1024];
bool keysPressed[1024];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // 关闭窗口
	}
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	double x = (2 * xpos) / SCR_WIDTH - 1;
	double y = 1 - (2 * ypos) / SCR_HEIGHT;
	if (mode == 0)
	{
		camera.handleMouseTranslation(x, y);
	}
	else if (mode == 1)
	{
		//视点移动旋转
		camera.handleRotation(x);
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
		if (mode1 == 0) //日光
		{
			lightspot = vec3(0.0f, 0.0f, -2.0f);
			mode1 = 1;
		}

		else if (mode1 == 1) //灯光
		{
			lightspot = vec3(0.0f, 1.0f, 0.0f);
			mode1 = 0;
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (mode == 0) //日光
		{
			mode = 1;
		}

		else if (mode == 1) //灯光
		{
			mode = 0;
		}

		break;
	default:
		return;
	}
	return;
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