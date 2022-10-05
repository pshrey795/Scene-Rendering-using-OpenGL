#include "../include/Scene.hpp"

Scene::Scene(int argc, char** argv){
    //Essential initializations
    this->init();
    gui = new GUI("Test Window", stoi(argv[1]), stoi(argv[2]));

    //User defined initializations
    Model model = Model();
    sceneObjects.push_back(model);
    shader = new Shader("shaders/vert1.glsl", "shaders/frag1.glsl");
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
    glEnable(GL_DEPTH_TEST);
    while(!gui->shouldClose()){
        gui->processInput();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Drawing Space
        this->draw();

        gui->swapBuffers();
        glfwPollEvents();
    }
    this->end();
}

void Scene::draw(){
    shader->use();
    shader->setMat4("view", gui->camera->getViewMatrix());
    shader->setMat4("projection", gui->camera->getPerspectiveMatrix()); 
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        shader->setMat4("model", sceneObjects[i].localTransform);
        sceneObjects[i].draw(*shader);
    }
}