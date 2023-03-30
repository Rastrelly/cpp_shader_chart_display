#include <iostream>
#include <ctime>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "shader.h"

using std::vector;
using std::cout;
using std::cin;
using std::string;
using std::to_string;

Shader * shad;

typedef vector<float> flArr;

// X, Y, Z, R, G, B
vector<float> vertices = {};

vector<float> triVertices = {
-1,-1,0,1,0,0,
1,-1,0,0,1,0,
0,1,0,0,0,1
};

float xc=1, yc=1;

float func(float a, float f, float x)
{
	return a * sin(f*x*glm::pi<float>() / 180.0f);
}

void generateVertexArr(float x1, float x2, int ns, vector<float> &verts)
{
	verts.clear();
	float step = (x2 - x1) / (float)ns;
	float cx = x1;
	float cy = 0.0f;
	float cz = 0.0f;
	float cr = 1.0f;
	float cg = 1.0f;
	float cb = 1.0f;
	
	//calc scale coeffs
	float scaleCoeffX = 2.0f/abs(x2 - x1);
	float scaleCoeffY = 2.0f / 20.0f;

	for (int i = 0; i < ns; i++)
	{
		//push coordinates
		cx = x1 + step * (float)i;
		verts.push_back(cx*scaleCoeffX);
		cy = func(10, 20,cx);
		verts.push_back(cy*scaleCoeffY);
		cz = 0;
		verts.push_back(cz);

		//push colors
		cr = 2*(cy / 20.0f);
		cb = -2*(cy / 20.0f);
		cg = 1-2*cr-2*cb;

		verts.push_back(cr);
		verts.push_back(cg);
		verts.push_back(cb);

		cout << verts[verts.size() - 6] << " " << verts[verts.size() - 5] << " " << verts[verts.size() - 4] << " " << verts[verts.size() - 3] << " " << verts[verts.size() - 2] << " " << verts[verts.size() - 1] << " " << std::endl;

	}
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	xc = 1 / (0.5f*(float)width);
	yc = 1 / (0.5f*(float)height);
}

void prepShaders()
{
	std::cout << "Preparing shaders..." << std::endl;
	shad = new Shader("shader_vert.gls", "shader_frag.gls");
}


void drawChartLine(flArr verts)
{

	//buffers
	unsigned int VBO; //vertex buffer
	unsigned int VAO; //vertex array

	glGenBuffers(1, &VBO); //generate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //assign data type to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(), verts.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_LINE_STRIP, 0, trunc(verts.size()/6));

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void drawTriangle(flArr verts)
{

	//buffers
	unsigned int VBO; //vertex buffer
	unsigned int VAO; //vertex array

	glGenBuffers(1, &VBO); //generate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //assign data type to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(), verts.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Main Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//INITIALIZTION OF GLAD
	bool gladLoaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!gladLoaded)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Renderer setup
	//main loop screen refresh callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//ready shaders for use
	prepShaders();

	//generate vertices
	generateVertexArr(-90.0f,90.0f,361*10,vertices);

	//MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shad->use();

		drawChartLine(vertices);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//kill GLFW
	glfwTerminate();

	return 0;
}