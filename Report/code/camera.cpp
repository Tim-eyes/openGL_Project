void cameraMove(double currentTime) {
	//GLfloat curFrameTime = glfwGetTime();
	deltaTime = currentTime - lastFrameTime;
	lastFrameTime = currentTime;
	GLfloat cameraSpeed = 5.0f * deltaTime;
	//WASD
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos += glm::normalize
			(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos -= glm::normalize
			(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
