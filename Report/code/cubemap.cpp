renderingProgramCubeMap = 
    Utils::createShaderProgram("vertCubeShader.glsl", "fragCubeShader.glsl");
//load map to cube as background
backgroundTexture = Utils::loadCubeMap("background");
glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); 