#include "../include/gui.hpp"

float lastX = 400, lastY = 300;
float xOffset = 0, yOffset = 0;
bool firstMouse = true;
bool checkMouse = false;

GUI::GUI(const char* winTitle, int width, int height){
    window = glfwCreateWindow(width, height, winTitle, NULL, NULL);
    if(window == NULL){
        cout << "Window creation failed!" << endl;
        errCode = 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Loading GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to load GLAD!" << endl;
        errCode = 2;
    }

    // stbi_set_flip_vertically_on_load(true);

    this->width = width;
    this->height = height;

    //Loading Camera
    camera = new Camera(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), width, height);

    allowMouse = false; 
}

void GUI::activateWindow(){
    glfwMakeContextCurrent(window);
}

bool GUI::shouldClose(){
    return (glfwWindowShouldClose(window) && (errCode == 0));
}

void GUI::swapBuffers(){
    glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    if(firstMouse){
        lastX = xposIn;
        lastY = yposIn;
        firstMouse = false;
    }else{
        xOffset = xposIn - lastX;
        yOffset = lastY - yposIn;
        lastX = xposIn;
        lastY = yposIn;
        checkMouse = true;
    }
}

void GUI::processInput(){
    //Keyboard Input
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }else if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera->processKeyInput(FORWARD, DELTA_TIME);
    }else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera->processKeyInput(LEFT, DELTA_TIME);
    }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera->processKeyInput(BACKWARD, DELTA_TIME);
    }else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera->processKeyInput(RIGHT, DELTA_TIME);
    }else if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        camera->processKeyInput(UP, DELTA_TIME);
    }else if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        camera->processKeyInput(DOWN, DELTA_TIME);
    }else if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        allowMouse = true;
    }

    //Mouse Input 
    if(allowMouse && checkMouse){
        camera->processMouseInput(xOffset, yOffset);
        checkMouse = false;
    }
}