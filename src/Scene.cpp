#include "../include/Scene.hpp"

Scene::Scene(int argc, char** argv){
    //Essential initializations
    this->init();
    gui = new GUI("Test Window", stoi(argv[1]), stoi(argv[2]));

    //User defined initializations
    createTerrain();
    createLampPosts();
    createStatues();

    //For testing purposes
    // testModel = Model("david.obj");
    // testModel.updateTransform(translate(mat4(1.0f), vec3(0.0f, 10.0f, 0.0f)));

    shaders[BASIC] = new Shader("Basic.vs", "Basic.fs");
    shaders[TEXTURE] = new Shader("Textured.vs", "Textured.fs");
    // shaders[CubeMap] = new Shader("CubeMap.vs", "CubeMap.fs");
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
    //Terrain
    Shader*& currentShader = shaders[TEXTURE];
    currentShader->use();
    currentShader->setMat4("view", gui->camera->getViewMatrix());
    currentShader->setMat4("projection", gui->camera->getPerspectiveMatrix()); 
    for(unsigned int i = 0; i < terrain.size(); i++){
        currentShader->setMat4("model", terrain[i].localTransform);
        terrain[i].draw(*currentShader, TEXTURE);
    }

    //Lamp Posts
    Shader*& currentShader2 = shaders[BASIC];
    currentShader2->use();
    currentShader2->setMat4("view", gui->camera->getViewMatrix());
    currentShader2->setMat4("projection", gui->camera->getPerspectiveMatrix());
    for(unsigned int i = 0; i < lampPosts.size(); i++){
        currentShader2->setMat4("model", lampPosts[i].localTransform);
        lampPosts[i].draw(*currentShader2, BASIC);
    }

    //Statues
    for(unsigned int i = 0; i < statueBases.size(); i++){
        currentShader2->setMat4("model", statueBases[i].localTransform);
        statueBases[i].draw(*currentShader2, BASIC);
    }
    for(unsigned int i = 0; i < statueHeads.size(); i++){
        currentShader2->setMat4("model", statueHeads[i].localTransform);
        statueHeads[i].draw(*currentShader2, BASIC);
    }

    //For testing only
    // Shader*& testShader = shaders[BASIC];
    // testShader->use();
    // testShader->setMat4("view", gui->camera->getViewMatrix());
    // testShader->setMat4("projection", gui->camera->getPerspectiveMatrix());
    // testShader->setMat4("model", testModel.localTransform);
    // testModel.draw(*testShader, BASIC);
}

//Drawing routines
void Scene::createTerrain(){
    //Grass
    Model grass = Model(GRASS, texUnit);
    grass.updateTransform(vec3(200.0f, 200.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(40.0f, 0.0f, -100.0f));
    terrain.push_back(grass);

    //Lake 
    Model lake = Model(LAKE, texUnit);
    lake.updateTransform(vec3(20.0f, 60.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(40.0f, 0.1f, -100.0f));
    terrain.push_back(lake);

    //Roads
    Model road1 = Model(ROAD, texUnit);
    road1.updateTransform(vec3(20.0f, 80.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(0.0f, 0.1f, -80.0f));
    terrain.push_back(road1);

    Model road2 = Model(ROAD, texUnit);
    road2.updateTransform(vec3(40.0f, 20.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(20.0f, 0.1f, -180.0f));
    terrain.push_back(road2);

    Model road3 = Model(ROAD, texUnit);
    road3.updateTransform(vec3(20.0f, 80.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(80.0f, 0.1f, -120.0f));
    terrain.push_back(road3);

    Model road4 = Model(ROAD, texUnit);
    road4.updateTransform(vec3(40.0f, 20.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(60.0f, 0.1f, -20.0f));
    terrain.push_back(road4);
}   

void Scene::createLampPosts(){
    //24 Cylinders in a 6 * 4 grid 
    for(int i = 0; i < 6;i++){
        for(int j = 0; j < 4;j++){
            Model lampPost = Model("cylinder.obj");
            lampPost.updateTransform(vec3(1.0f, 10.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(j * 40.0f - 20.0f, 5.0f, i * -40.0f));
            lampPosts.push_back(lampPost);
        }
    }
}

void Scene::createStatues(){
    for(int i = 0; i < 7; i++){
        if(i==0 || i==6){
            double zCoord = (i==0) ? 0.0f : -200.0f;
            for(int j=0;j<3;j++){
                Model statueBase = Model("cuboid.obj");
                statueBase.updateTransform(vec3(3.0f, 10.0f, 3.0f), vec3(0.0f, 1.0f, 0.0f), 45.0f, vec3(j * 40.0f, 5.0f, zCoord));
                statueBases.push_back(statueBase);
                createStatueHead(i, j);
            }
        }else if(i==1 || i==3 || i==5){
            double zCoord = ((i==1) ? (-20.0f) : ((i==3) ? (-100.0f) : (-180.0f)));
            for(int j=0;j<2;j++){
                Model statueBase = Model("cuboid.obj");
                statueBase.updateTransform(vec3(3.0f, 10.0f, 3.0f), vec3(0.0f, 1.0f, 0.0f), 45.0f, vec3(j * 120.0f - 20.0f, 5.0f, zCoord));
                statueBases.push_back(statueBase);
                createStatueHead(i, j);
            }
        }else{
            double zCoord = (i==2) ? -60.0f : -140.0f;
            for(int j=0;j<4;j++){
                Model statueBase = Model("cuboid.obj");
                statueBase.updateTransform(vec3(3.0f, 10.0f, 3.0f), vec3(0.0f, 1.0f, 0.0f), 45.0f, vec3(j * 40.0f - 20.0f, 5.0f, zCoord));
                statueBases.push_back(statueBase);
                createStatueHead(i, j);
            }
        }
    }
}

void Scene::createStatueHead(int i, int j){
    Model statueHead; 
    vec3 scaling; 
    vec3 translation; 
    vec3 rotateAxis = vec3(0.0f, 1.0f, 0.0f);
    float rotateAngle; 
    int randInt = (i + j) % 4;
    if(i == 0 || i == 6){
        double zCoord = (i==0) ? 0.0f : -200.0f;
        translation = vec3(j * 40.0f, 15.0f, zCoord);
        rotateAngle = (i==0) ? 180.0f : 0.0f;
    }else if(i==1 || i==3 || i==5){
        double zCoord = ((i==1) ? (-20.0f) : ((i==3) ? (-100.0f) : (-180.0f)));
        translation = vec3(j * 120.0f - 20.0f, 15.0f, zCoord);
        rotateAngle = (j==0) ? 90.0f : -90.0f;
    }else{
        double zCoord = (i==2) ? -60.0f : -140.0f;
        translation = vec3(j * 40.0f - 20.0f, 15.0f, zCoord);
        rotateAngle = (j%2==0) ? 90.0f : -90.0f;
    }
    switch(randInt){
        case 0: {
            statueHead = Model("bunny.obj", BUNNY);
            scaling = vec3(3.0f, 3.0f, 3.0f);
            translation += vec3(0.0f, 1.0f, 0.0f);
            break;
        } case 1: {
            statueHead = Model("armadillo.obj", ARMADILLO);
            statueHead.updateTransform(translate(mat4(1.0f), vec3(0.2f, 0.0f, -0.3f)));
            scaling = vec3(4.0f, 4.0f, 4.0f);
            if(i == 0 || i == 6){
                rotateAngle = (i==0) ? 0.0f : 180.0f;
            }else{
                rotateAngle *= (-1.0f);
            }
            translation += vec3(0.0f, 4.2f, 0.0f);
            break;
        } case 2: {
            statueHead = Model("dragon.obj", DRAGON);
            scaling = vec3(0.75f, 0.75f, 0.75f);
            break;
        } case 3: {
            statueHead = Model("david.obj", DAVID);
            statueHead.updateTransform(translate(mat4(1.0f), vec3(-1.5f, 0.0f, -15.0f)));
            scaling = vec3(2.0f, 2.0f, 2.0f);
            translation += vec3(0.0f, -1.0f, 0.0f);
        }
    }
    statueHead.updateTransform(scaling, rotateAxis, rotateAngle, translation);
    statueHeads.push_back(statueHead);
}