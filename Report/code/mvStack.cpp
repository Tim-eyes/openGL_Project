//function which convert degree to radian
float toRadians(float degrees) 
    { return (degrees * 2.0f * 3.14159f) / 360.0f; }
//Earth
mvStack.push(mvStack.top());
//scale
mvStack.top() *= scale(glm::mat4(1.0f), 
    glm::vec3(1.05f, 1.05f, 1.05f));
//orbital position and revolution
mvStack.top() *= glm::translate(glm::mat4(1.0f), 
    glm::vec3(sin((float)currentTime * 2.9) * 12.0, 
        0.0f, cos((float)currentTime * 2.9) * 12.0));
mvStack.push(mvStack.top());
//inclination
mvStack.top() *= rotate(glm::mat4(1.0f), 
    toRadians(23.43f), glm::vec3(0.0, 0.0, 1.0));
//rotation
mvStack.top() *= rotate(glm::mat4(1.0f), 
    (float)(currentTime * 2.0), 
        glm::vec3(0.0, 1.0, 0.0));
glUniformMatrix4fv(mvLoc, 1, GL_FALSE, 
    glm::value_ptr(mvStack.top()));
/*some codes which are same as sample codes can be omitted*/
mvStack.pop(); 

//Moon no rotation
mvStack.push(mvStack.top());
//orbital position
mvStack.top() *= glm::translate
    (glm::mat4(1.0f), 
        glm::vec3(sin((float)currentTime) * 2.0, 
            0.0f, cos((float)currentTime) * 2.0));
//revolution
mvStack.top() *= rotate
    (glm::mat4(1.0f), (float)(currentTime), 
        glm::vec3(0.0, 0.0, 1.0));
//scale
mvStack.top() *= scale(glm::mat4(1.0f), 
    glm::vec3(0.25f, 0.25f, 0.25f));
glUniformMatrix4fv(mvLoc, 1, GL_FALSE, 
    glm::value_ptr(mvStack.top()));
/*some codes which are same as sample codes can be omitted*/
mvStack.pop(); mvStack.pop();