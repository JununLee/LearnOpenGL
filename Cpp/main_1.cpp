#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Include/Shader.h"
#include "../Include/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Include/Camera.h"
#include "../Include/Model.h"
#include <direct.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scoll_callback(GLFWwindow* window, double xoffset, double yoffset);
std::string getFullpath(std::string path);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(std::vector<std::string> faces);

bool firstMouse = true;

Camera camera(glm::vec3(0.f, 0.f, 3.f));

float lastX = 400.f;
float lastY = 300.f;
float deltaTime = 0.f;
float lastFrame = 0.f;

glm::vec3 lightPos(1.2f, 1.f, 2.f);
//glm::vec3 lightPos(1.2f, 1.f, 2.f);

int viewport[4];

int main() {
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow* window = glfwCreateWindow(800, 600, " LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scoll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize glad" << std::endl;
		return -1;
	}

	Shader shader("Shader/shader.vs","Shader/shader.fs");
	Shader lightShader("Shader/lightShader.vs", "Shader/lightShader.fs");
	Shader nanosuitShader("Shader/nanosuitShader.vs", "Shader/nanosuitShader.fs");
	Shader transparentShader("Shader/transparentShader.vs", "Shader/transparentShader.fs");
	Shader screenShader("Shader/screenShader.vs", "Shader/screenShader.fs");
	Shader skyboxShader("Shader/skyboxShader.vs", "Shader/skyboxShader.fs");
	Shader geometryShader("Shader/geometryShader.vs", "Shader/geometryShader.fs", "Shader/geometryShader.gs");
	Shader instancedShader("Shader/instancedShader.vs", "Shader/instancedShader.fs");
	Shader depthShader("Shader/shadowdepthShader.vs", "Shader/shadowdepthShader.fs");
	Shader shadowShader("Shader/shadowShader.vs", "Shader/shadowShader.fs");
	Shader normalShader("Shader/normalShader.vs", "Shader/normalShader.fs");

	float vertices[] = {
	
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
	};

	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	glm::vec3 edge1 = glm::vec3(-1.0f, -0.5f, -1.0f) - glm::vec3(1.0f, -0.5f,1.0f);
	glm::vec3 edge2 = glm::vec3(-1.0f, -0.5f, 1.0f) - glm::vec3(1.0f, -0.5f, 1.0f);
	glm::vec2 deltaUV1 = glm::vec2(0.0f, 1.0f) - glm::vec2(1.0f, 0.0f);
	glm::vec2 deltaUV2 = glm::vec2(0.0f, 0.0f) - glm::vec2(1.0f, 0.0f);

	float f = 1.f / (deltaUV1.x*deltaUV2.y - deltaUV1.y*deltaUV2.x);

	tangent1.x = f * (deltaUV2.y*edge1.x - deltaUV1.y*edge2.x);
	tangent1.y = f * (deltaUV2.y*edge1.y - deltaUV1.y*edge2.y);
	tangent1.z = f * (deltaUV2.y*edge1.z - deltaUV1.y*edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (deltaUV1.x*edge2.x - deltaUV2.x*edge1.x);
	bitangent1.x = f * (deltaUV1.x*edge2.y - deltaUV2.x*edge1.y);
	bitangent1.x = f * (deltaUV1.x*edge2.z - deltaUV2.x*edge1.z);
	bitangent1 = glm::normalize(bitangent1);

	edge1 = glm::vec3(1.0f, -0.5f, -1.0f) - glm::vec3(1.0f, -0.5f, 1.0f);
	edge2 = glm::vec3(-1.0f, -0.5f, -1.0f) - glm::vec3(1.0f, -0.5f, 1.0f);
	deltaUV1 = glm::vec2(1.0f, 1.0f) - glm::vec2(1.0f, 0.0f);
	deltaUV2 = glm::vec2(0.0f, 1.0f) - glm::vec2(1.0f, 0.0f);

	f = 1.f / (deltaUV1.x*deltaUV2.y - deltaUV1.y*deltaUV2.x);

	tangent2.x = f * (deltaUV2.y*edge1.x - deltaUV1.y*edge2.x);
	tangent2.y = f * (deltaUV2.y*edge1.y - deltaUV1.y*edge2.y);
	tangent2.z = f * (deltaUV2.y*edge1.z - deltaUV1.y*edge2.z);
	tangent2 = glm::normalize(tangent2);

	bitangent2.x = f * (deltaUV1.x*edge2.x - deltaUV2.x*edge1.x);
	bitangent2.x = f * (deltaUV1.x*edge2.y - deltaUV2.x*edge1.y);
	bitangent2.x = f * (deltaUV1.x*edge2.z - deltaUV2.x*edge1.z);
	bitangent2 = glm::normalize(bitangent2);

	float planeVertices[]{

		1.0f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
		-1.0f, -0.5f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
		-1.0f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,

		1.0f, -0.5f,  1.0f,0.0f,  0.0f, 1.0f,  1.0f, 0.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
		1.0f, -0.5f, -1.0f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
		-1.0f, -0.5f, -1.0f,0.0f,  0.0f, 1.0f,  0.0f, 1.0f,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z
	};
	float transparentVertices[]{
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	float screenVertices[]{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	float skyboxVertices[]{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	float points[]{
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};


	unsigned int cubeVAO, VBO, EBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	//VAO处于活跃状态不能解绑EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);					
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);					
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glBindVertexArray(0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int transparentVAO, transparentVBO;
	glGenVertexArrays(1, &transparentVAO);
	glBindVertexArray(transparentVAO);
	glGenBuffers(1, &transparentVBO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	
	unsigned int screenVAO, screenVBO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glGenBuffers(1, &screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int screentTexture;
	glGenTextures(1, &screentTexture);
	glBindTexture(GL_TEXTURE_2D, screentTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screentTexture, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: framebuffer is not complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int msaaframebuffer;
	glGenFramebuffers(1, &msaaframebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaframebuffer);

	unsigned int msaatexture;
	glGenTextures(1, &msaatexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaatexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 800, 600, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaatexture, 0);

	unsigned int msaarbo;
	glGenRenderbuffers(1, &msaarbo);
	glBindRenderbuffer(GL_RENDERBUFFER, msaarbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msaarbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER::framebuffer is not complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	unsigned int uniformBlockIndexShader = glGetUniformBlockIndex(shader.ID, "Matrices");
	unsigned int uniformBlockIndexLightShader = glGetUniformBlockIndex(lightShader.ID, "Matrices");
	unsigned int uniformBlockIndexNanosuitShader = glGetUniformBlockIndex(nanosuitShader.ID, "Matrices");
	unsigned int uniformBlockIndexTransparentShader = glGetUniformBlockIndex(transparentShader.ID, "Matrices");
	unsigned int uniformBlockIndexSkyboxShader = glGetUniformBlockIndex(skyboxShader.ID, "Matrices");

	glUniformBlockBinding(shader.ID, uniformBlockIndexShader, 0);
	glUniformBlockBinding(lightShader.ID, uniformBlockIndexLightShader, 0);
	glUniformBlockBinding(nanosuitShader.ID, uniformBlockIndexNanosuitShader, 0);
	glUniformBlockBinding(transparentShader.ID, uniformBlockIndexTransparentShader, 0);
	glUniformBlockBinding(skyboxShader.ID, uniformBlockIndexSkyboxShader, 0);

	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	unsigned int pointVAO, pointVBO;
	glGenVertexArrays(1, &pointVAO);
	glBindVertexArray(pointVAO);
	glGenBuffers(1, &pointVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	unsigned int texture1 = loadTexture("Image/container2.png");
	unsigned int texture2 = loadTexture("Image/container2_specular.png");
	unsigned int transparentTex = loadTexture("Image/blending_transparent_window.png");
	std::vector<std::string> faces{
		getFullpath("Image/skybox/right.jpg"),
		getFullpath("Image/skybox/left.jpg"),
		getFullpath("Image/skybox/top.jpg"),
		getFullpath("Image/skybox/bottom.jpg"),
		getFullpath("Image/skybox/front.jpg"),
		getFullpath("Image/skybox/back.jpg")
	};
	unsigned int skyboxTex = loadCubemap(faces);
	unsigned int texture3 = loadTexture("Image/brickwall.jpg");
	unsigned int texture4 = loadTexture("Image/brickwall_normal.jpg");

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	shadowShader.use();
	shadowShader.setInt("texture1", 0);
	shadowShader.setInt("texture2", 1);
	shadowShader.setInt("shadowMap", 2);

	normalShader.use();
	normalShader.setInt("texture1", 0);
	normalShader.setInt("texture2", 1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Model nanosuit(getFullpath("Model/nanosuit/nanosuit.obj").c_str());
	//Model planet(getFullpath("Model/planet/planet.obj").c_str());
	//Model rock(getFullpath("Model/rock/rock.obj").c_str());

// instanced
	/*unsigned int amount = 10000;
	glm::mat4* matrices;
	matrices = new glm::mat4[amount];
	srand(glfwGetTime());
	float radius = 50.0f;
	float offset = 2.5f;
	for (unsigned int i = 0;  i < amount;  i++)
	{
		glm::mat4 model = glm::mat4(1);
		float angle = 1.f * i / amount * 360.f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.f - offset;
		float x = sin(angle)*radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.f - offset;
		float z = cos(angle)*radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		float scale = (rand() % 20)/100.f + 0.05f;
		model = glm::scale(model, glm::vec3(scale));

		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		matrices[i] = model;

	}
	unsigned int instancedBuffer;
	glGenBuffers(1, &instancedBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instancedBuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rock.meshs.size(); i++)
	{
		unsigned int VAO = rock.meshs[i].VAO;
		glBindVertexArray(VAO);
		GLsizei vec4size = sizeof(glm::vec4);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)(vec4size));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)(2 * vec4size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)(3 * vec4size));
		glEnableVertexAttribArray(6);

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}*/
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	//glDepthFunc(GL_ALWAYS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		processInput(window);

		//glViewport(0, 0, 800, 600);
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//glBindFramebuffer(GL_FRAMEBUFFER, msaaframebuffer);

	/*	glm::mat4 lightProjection, lightView;
		glm::mat4 lightSapceMatrix;
		float nearplane = 0.1f, farplane = 10.f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearplane, farplane);
		lightView = glm::lookAt(lightPos, glm::vec3(0), glm::vec3(0, 1, 0));
		lightSapceMatrix = lightProjection * lightView;
		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSapceMatrix);
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);*/

		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);
		view = camera.GetViewMatrix();
		glm::mat4 proj = glm::mat4(1);
		proj = glm::perspective(glm::radians(camera.Zoom), 800.f / 600, 0.1f, 100.f);

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		shader.use();
		//depthShader.use();
		//normalShader.use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture3);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture4);

		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f); 
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightDir", -0.2f, 0.0f, 1.f);

		normalShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		normalShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		normalShader.setVec3("lightPos", lightPos);
		normalShader.setVec3("viewPos", camera.Position);
		normalShader.setVec3("lightDir", -0.2f, 0.0f, 1.f);

	/*	glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);*/
//cube
	model = glm::rotate(model, glm::radians(25.f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(0, 0.2f, 0));

		shader.setMat4("model", model);
		depthShader.setMat4("model", model);
		normalShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//shader.use();
		////glStencilMask(0x00);

//floor		
		/*glBindVertexArray(planeVAO);
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(0.f), glm::vec3(1, 0, 0));
		shader.setMat4("model", glm::mat4(1));
		depthShader.setMat4("model", glm::mat4(1));
		normalShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);*/


	
//stencil
		/*lightShader.use();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		model = glm::scale(model, glm::vec3(1.05f));
		lightShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);*/


//nanosuit	
		/*model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(0.f, -1.75f, 0.f));
		//model = glm::scale(model, glm::vec3(0.2f));

		//nanosuitShader.use();
		//nanosuitShader.setMat4("model", model);
		//nanosuitShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//nanosuitShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		//nanosuitShader.setVec3("lightPos", lightPos);
		//nanosuitShader.setVec3("viewPos", camera.Position);
		//nanosuitShader.setVec3("lightDir", -0.2f, 0.0f, 1.f);
		//nanosuit.Draw(nanosuitShader);*/


//light
		/*lightShader.use();
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

//skybox		
		/*glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
		glBindVertexArray(skyboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);*/

//transparent blend
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, transparentTex);
		transparentShader.use();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0, 0, 2));
		transparentShader.setMat4("model", model);
		glBindVertexArray(transparentVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/


		//glStencilMask(0xFF);

//msaaframebuffer
		/*glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaframebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		*/
	
//framebuffser
		//glViewport(0, 0, 800, 600);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//screenShader.use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, screentTexture);
		//glBindVertexArray(screenVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

//geometryShader
		/*geometryShader.use();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.f, -1.75f, 0.f));
		model = glm::scale(model, glm::vec3(0.2f));
		geometryShader.setMat4("model", model);
		geometryShader.setFloat("time", glfwGetTime());
		nanosuit.Draw(geometryShader);*/

//planet
	/*	nanosuitShader.use();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0, -3, 0));
		model = glm::scale(model, glm::vec3(4));
		nanosuitShader.setMat4("model", model);

		planet.Draw(nanosuitShader);

		instancedShader.use();

		//rock.Draw(nanosuitShader)
		//for (unsigned int i = 0; i < amount; i++)
		//{
			//instancedShader.setMat4("model", matrices[i]);
			//rock.Draw(instancedShader);
		//}
		for (unsigned int i = 0; i < rock.meshs.size(); i++)
		{
			glBindVertexArray(rock.meshs[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, rock.meshs[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		};*/

//shadow
		/*glCullFace(GL_BACK);
		glViewport(0, 0, 800, 600);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lightShader.use();
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		shadowShader.use();
		shadowShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shadowShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shadowShader.setVec3("lightPos", lightPos);
		shadowShader.setVec3("viewPos", camera.Position);
		shadowShader.setVec3("lightDir", -0.2f, 0.0f, 1.f);
		shadowShader.setMat4("lightSpaceMatrix", lightSapceMatrix);
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(25.f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(0, 0.2f, 0));
		shadowShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(planeVAO);
		shadowShader.setMat4("model", glm::mat4(1));
		glDrawArrays(GL_TRIANGLES, 0, 6);*/



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		glGetIntegerv(GL_VIEWPORT, viewport);
		GLbyte* colorArr;
		colorArr = new GLbyte[viewport[2] * viewport[3] * 3];
		glReadPixels(viewport[0], viewport[1], viewport[2], viewport[3], GL_RGB, GL_UNSIGNED_BYTE, colorArr);
		for (int i = 0; i < viewport[2] * viewport[3] * 3; i++)
		{
			GLbyte byt = *(colorArr + i);
			GLint temp = (GLint) (byt);
			int b = 0;
		}
		delete[] colorArr;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		camera.ProcessMouseMovement(xoffset, yoffset);

	}
}

void scoll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
	camera.ProcessMouseScroll(yoffset);
}

std::string getFullpath(std::string path) {
	char* buffer;
	buffer = _getcwd(NULL, 0);
	std::string fullPath(buffer);
	fullPath = fullPath + "\\" + path;
	return fullPath;
}

unsigned int loadTexture(const char* path) {
	unsigned int textureId;
	glGenTextures(1, &textureId);
	int width, heigth, nrChannel;
	unsigned char* data = stbi_load(path, &width, &heigth, &nrChannel, 0);
	if (data) {
		GLenum format;
		if (nrChannel == 1)
			format = GL_RED;
		else if (nrChannel == 3)
			format = GL_RGB;
		else if (nrChannel == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, heigth, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed load:: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureId;
}

unsigned int loadCubemap(std::vector<std::string> faces) {

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, heigth, nrChannels;
	for ( int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &heigth, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "failed load cubeMap:: " << faces[i] << std::endl;
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}