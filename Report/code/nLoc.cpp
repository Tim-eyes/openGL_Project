cameraMove(currentTime);
vMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
vLoc = glGetUniformLocation(renderingProgramCubeMap, "v_matrix");