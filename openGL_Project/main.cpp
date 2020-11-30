#include <GLEW\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <stack>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"
#include "Utils.h"
using namespace std;

#define numVAOs 1
#define numVBOs 11

float cameraX, cameraY, cameraZ;
float sphLocX, sphLocY, sphLocZ;
GLuint renderingProgram, renderingProgramCubeMap;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
float rotAmt = 0.0f;

// variable allocation for display
GLuint vLoc, mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

stack<glm::mat4> mvStack;
GLuint sunTexture, planetTexture, moonTexture, jupiterTexture, marsTexture, 
mercuryTexture, neptuneTexture, plutoTexture, saturnTexture, uranusTexture, venusTexture,
backgroundTexture;

Sphere mySphere = Sphere(48);
void setupVertices(void) {
	float cubeVertexPositions[108] =
	{ -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	std::vector<glm::vec3> norm = mySphere.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	int numIndices = mySphere.getNumIndices();
	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}


	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size()*4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	renderingProgramCubeMap = Utils::createShaderProgram("vertCubeShader.glsl", "fragCubeShader.glsl");

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 100.0f;
	sphLocX = 0.0f; sphLocY = 0.0f; sphLocZ = -1.0f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();

	//load map to cube as background
	backgroundTexture = Utils::loadCubeMap("background");
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//load texture of each planets
	sunTexture = Utils::loadTexture("sun.bmp");
	planetTexture = Utils::loadTexture("earth.bmp");
	moonTexture = Utils::loadTexture("moon.bmp");
	jupiterTexture = Utils::loadTexture("jupiter.bmp");
	marsTexture = Utils::loadTexture("mars.bmp");
	mercuryTexture = Utils::loadTexture("mercury.bmp");
	neptuneTexture = Utils::loadTexture("neptune.bmp");
	plutoTexture = Utils::loadTexture("pluto.bmp");
	saturnTexture = Utils::loadTexture("saturn.bmp");
	uranusTexture = Utils::loadTexture("uranus.bmp");
	venusTexture = Utils::loadTexture("venus.bmp");
	
}

void push_planet(stack<glm::mat4> mvStack,double currentTime,GLuint texture){
	/*mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));*/
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();
}

void display(GLFWwindow* window, double currentTime) {


	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	
	//draw cube
	glUseProgram(renderingProgramCubeMap);
	vLoc = glGetUniformLocation(renderingProgramCubeMap, "v_matrix");
	projLoc = glGetUniformLocation(renderingProgramCubeMap, "p_matrix");

	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, backgroundTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);
	
	//draw sphere
	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");


	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(sphLocX, sphLocY, sphLocZ));
	mvMat = vMat * mMat;
	mvStack.push(mvMat);

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	//sun
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sunTexture);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();

	//Mercury 
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.38f, 0.38f, 0.38f));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 8.0, 0.0, cos((float)currentTime) * 8.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	//glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);
	//glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mercuryTexture);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();


	//venus
	mvStack.push(mvStack.top());
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(2.48f, 2.48f, 2.48f));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, venusTexture);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();

	
	//Earth
	mvStack.push(mvStack.top());
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(1.05f, 1.05f, 1.05f));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planetTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();

	//Moon
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, moonTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();

	//Mars
	mvStack.push(mvStack.top());
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.53f, 0.53f, 0.53f));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, marsTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();


	//Jupiter
	mvStack.push(mvStack.top());
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(21.05f, 21.05f, 21.05f));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, jupiterTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();

	//Saturn
	mvStack.push(mvStack.top());
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.84f, 0.84f, 0.84f));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, saturnTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();

	//Uranus
	mvStack.push(mvStack.top());
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.42f, 0.421f, 0.42f));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, uranusTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();

	//Neptune
	mvStack.push(mvStack.top());
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.42, 0.42f, 0.42f));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, neptuneTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	mvStack.pop();

	mvStack.pop(); mvStack.pop(); mvStack.pop();
	mvStack.pop();
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(int argc,char **argv) {

	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "Project", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}