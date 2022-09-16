#include "../include/gui.hpp"

GUI::GUI(const char* winTitle, int width, int height){
    window = glfwCreateWindow(width, height, winTitle, NULL, NULL);
    if(window == NULL){
        cout << "Window creation failed!" << endl;
        errCode = 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Loading GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to load GLAD!" << endl;
        errCode = 1;
    }
}

void GUI::activateWindow(){
    glfwMakeContextCurrent(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void GUI::processInput(){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

void GUI::run(){
    while(!glfwWindowShouldClose(window)){
        processInput();
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
}