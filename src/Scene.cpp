#include "../include/Scene.hpp"

Scene::Scene(int argc, char** argv){
    this->init();
    gui = new GUI("Test Window", stoi(argv[1]), stoi(argv[2]));
}

void Scene::init(){
    //Setting up GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Scene::end(){
    glfwTerminate();
}

void Scene::run(){
    gui->activateWindow();
    if(gui->errCode == 0){
        gui->run();
    }
    this->end();
}