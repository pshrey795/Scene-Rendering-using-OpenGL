#include "../include/Scene.hpp"

Scene::Scene(int argc, char** argv){
    //Essential initializations
    this->init();
    gui = new GUI("Test Window", stoi(argv[1]), stoi(argv[2]));
    currentTime = 0.0f;
    frameCounter = 0;

    //Loading Shaders
    shaders[BASIC] = new Shader("Basic.vs", "Basic.fs");
    shaders[TEXTURE] = new Shader("Textured.vs", "Textured.fs");
    shaders[CUBEMAP] = new Shader("CubeMap.vs", "CubeMap.fs");
    shaders[LIGHT] = new Shader("Light.vs", "Light.fs");
    shaders[SHADOW] = new Shader("Shadow.vs", "Shadow.fs");

    //User defined initializations
    createTerrain();
    createLampPosts();
    createStatues();
    createSkyBox();

    //Setup for Shadow Mapping
    //Framebuffer
    shadowMap = FBuffer(gui->width, gui->height, SHADOW_MAP);

    //For testing purposes
    // testModel = Model("david.obj");
    // testModel.updateTransform(translate(mat4(1.0f), vec3(0.0f, 10.0f, 0.0f)));
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

        //Update time
        currentTime = glfwGetTime();
        frameCounter = (frameCounter + 1) % 60;

        //Drawing Space
        sunDirection = getSunDir();
        this->draw();

        gui->swapBuffers();
        glfwPollEvents();
    }
    this->end();
}

//By default, we will model the sun as a directional light
//Other light sources will be point lights
void Scene::fillShadowMap(vec3 lightPos, bool directional){
    Shader*& shadowShader = shaders[SHADOW];

    //Setting Up Light Transforms 
    mat4 lightProjection, lightView, lightSpaceMatrix;
    float nearPlane = 0.1f;
    float farPlane = 500.0f;
    if(directional){
        lightProjection = ortho(-200.0f, 200.0f, -200.0f, 200.0f, nearPlane, farPlane);
        lightPos = convertDirToPos(lightPos, 200.0f);
    }else{
        lightProjection = perspective(radians(45.0f), (float)shadowMap.width / (float)shadowMap.height, nearPlane, farPlane);
    }
    lightView = lookAt(lightPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;

    //Setting Up Shader
    shadowShader->use();
    shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    //Drawing to Shadow Map
    glViewport(0, 0, shadowMap.width, shadowMap.height);
    shadowMap.activateBuffer();
    glClear(GL_DEPTH_BUFFER_BIT);

    //Drawing Space
    grass.draw(*shadowShader, SHADOW);
    lake.draw(*shadowShader, SHADOW);
    road.draw(*shadowShader, SHADOW);
    lampPost.draw(*shadowShader, SHADOW);
    lamp.draw(*shadowShader, SHADOW);
    statueBase.draw(*shadowShader, SHADOW);
    bunny.draw(*shadowShader, SHADOW);
    armadillo.draw(*shadowShader, SHADOW);
    dragon.draw(*shadowShader, SHADOW);
    david.draw(*shadowShader, SHADOW);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::draw(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader*& textureShader = shaders[TEXTURE];
    Shader*& basicShader = shaders[BASIC];
    Shader*& cubeMapShader = shaders[CUBEMAP];
    Shader*& lightShader = shaders[LIGHT];

    mat4 viewMatrix = gui->camera->getViewMatrix();
    mat4 perspectiveMatrix = gui->camera->getPerspectiveMatrix();

    bool isDay = (sunDirection[1] < 0.0f);

    //Terrain
    textureShader->use();
    textureShader->setMat4("view", viewMatrix);
    textureShader->setMat4("projection", perspectiveMatrix); 

    //Grass
    grass.draw(*textureShader, TEXTURE);

    //Lake
    lake.draw(*textureShader, TEXTURE);

    //Roads
    road.draw(*textureShader, TEXTURE);

    //Lamp Posts
    basicShader->use();
    basicShader->setBool("isDay", isDay);
    basicShader->setVec3("viewPos", gui->camera->Position);
    basicShader->setVec3("sunDir", sunDirection);
    for(int i = 0; i < 24; i++){
        string lampName = "lampPos[" + to_string(i) + "]";
        basicShader->setVec3(lampName, lampPos[i]);
    }
    basicShader->setMat4("view", viewMatrix);
    basicShader->setMat4("projection", perspectiveMatrix);
    lampPost.draw(*basicShader, BASIC);

    //Lamp Lights
    lightShader->use();
    lightShader->setBool("isDay", isDay);
    lightShader->setMat4("view", viewMatrix);
    lightShader->setMat4("projection", perspectiveMatrix);
    lamp.draw(*lightShader, LIGHT);

    //Statues
    //Statue Bases
    basicShader->use();
    statueBase.draw(*basicShader, BASIC);

    //Statue Heads
    //Bunny
    bunny.draw(*basicShader, BASIC);

    //Armadillo
    armadillo.draw(*basicShader, BASIC);

    //Dragon
    dragon.draw(*basicShader, BASIC);

    //David
    david.draw(*basicShader, BASIC);

    //Skybox and Billboarded Trees
    glDepthFunc(GL_LEQUAL);
    cubeMapShader->use();
    cubeMapShader->setBool("isDay", isDay);
    //Any translation from the camera should not affect the skybox
    cubeMapShader->setMat4("projection", gui->camera->getPerspectiveMatrix());
    mat4 newViewMatrix = mat4(mat3(gui->camera->getViewMatrix()));
    cubeMapShader->setMat4("view", newViewMatrix);
    tree1.draw(*cubeMapShader, CUBEMAP);
    tree2.draw(*cubeMapShader, CUBEMAP);
    skyBox.draw(*cubeMapShader, CUBEMAP);
    glDepthFunc(GL_LESS);

    //For testing only
    // Shader*& testShader = shaders[BASIC];
    // testShader->use();
    // testShader->setMat4("view", viewMatrix);
    // testShader->setMat4("normalViewMatrix", normalViewMatrix);
    // testShader->setMat4("projection", perspectiveMatrix);
    // testShader->setMat4("model", testModel.localTransform);
    // testModel.draw(*testShader, BASIC);
}

//Drawing routines
void Scene::createTerrain(){
    //Grass
    grass = Model(GRASS, texUnit);
    grass.addTransform(getTransform(vec3(200.0f, 200.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(40.0f, 0.0f, -100.0f)));

    //Lake 
    lake = Model(LAKE, texUnit);
    lake.addTransform(getTransform(vec3(20.0f, 60.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(40.0f, 0.1f, -100.0f)));

    //Roads
    road = Model(ROAD, texUnit);
    mat4 transform; 
    road.addTransform(getTransform(vec3(20.0f, 80.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(0.0f, 0.1f, -80.0f)));
    road.addTransform(getTransform(vec3(40.0f, 20.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(20.0f, 0.1f, -180.0f)));
    road.addTransform(getTransform(vec3(20.0f, 80.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(80.0f, 0.1f, -120.0f)));
    road.addTransform(getTransform(vec3(40.0f, 20.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), -90.0f, vec3(60.0f, 0.1f, -20.0f)));
}   

void Scene::createLampPosts(){
    //24 Cylinders in a 6 * 4 grid
    lampPost = Model("cylinder.obj", texUnit); 
    lamp = Model("sphere.obj", texUnit);

    //Create an array of vec3 of size 24
    lampPos = new vec3[24];

    for(int i = 0; i < 6;i++){
        for(int j = 0; j < 4;j++){
            vec3 newLightPos = vec3(j * 40.0f - 20.0f, 16.5f, i * -40.0f);
            lampPost.addTransform(getTransform(vec3(1.0f, 10.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), 0.0f, vec3(j * 40.0f - 20.0f, 5.0f, i * -40.0f)));
            lamp.addTransform(getTransform(vec3(1.5f, 1.5f, 1.5f), vec3(1.0f, 0.0f, 0.0f), 0.0f, newLightPos));
            lampPos[i * 4 + j] = newLightPos;
        }
    }
}

void Scene::createStatues(){
    statueBase = Model("cuboid.obj", texUnit);
    bunny = Model("bunny.obj", texUnit, BUNNY);
    armadillo = Model("armadillo.obj", texUnit, ARMADILLO);
    dragon = Model("dragon.obj", texUnit, DRAGON);
    david = Model("david.obj", texUnit, DAVID);
    mat4 transform;
    for(int i = 0; i < 7; i++){
        if(i==0 || i==6){
            double zCoord = (i==0) ? 0.0f : -200.0f;
            for(int j=0;j<3;j++){
                statueBase.addTransform(getTransform(vec3(3.0f, 10.0f, 3.0f), vec3(0.0f, 1.0f, 0.0f), 45.0f, vec3(j * 40.0f, 5.0f, zCoord)));
                createStatueHead(i, j);
            }
        }else if(i==1 || i==3 || i==5){
            double zCoord = ((i==1) ? (-20.0f) : ((i==3) ? (-100.0f) : (-180.0f)));
            for(int j=0;j<2;j++){
                statueBase.addTransform(getTransform(vec3(3.0f, 10.0f, 3.0f), vec3(0.0f, 1.0f, 0.0f), 45.0f, vec3(j * 120.0f - 20.0f, 5.0f, zCoord)));
                createStatueHead(i, j);
            }
        }else{
            double zCoord = (i==2) ? -60.0f : -140.0f;
            for(int j=0;j<4;j++){
                statueBase.addTransform(getTransform(vec3(3.0f, 10.0f, 3.0f), vec3(0.0f, 1.0f, 0.0f), 45.0f, vec3(j * 40.0f - 20.0f, 5.0f, zCoord)));
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
            scaling = vec3(3.0f, 3.0f, 3.0f);
            translation += vec3(0.0f, 1.0f, 0.0f);
            bunny.addTransform(getTransform(scaling, rotateAxis, rotateAngle, translation));
            break;
        } case 1: {
            mat4 preTransform = translate(mat4(1.0f), vec3(0.2f, 0.0f, -0.3f));
            scaling = vec3(4.0f, 4.0f, 4.0f);
            if(i == 0 || i == 6){
                rotateAngle = (i==0) ? 0.0f : 180.0f;
            }else{
                rotateAngle *= (-1.0f);
            }
            translation += vec3(0.0f, 4.2f, 0.0f);
            armadillo.addTransform(getTransform(scaling, rotateAxis, rotateAngle, translation) * preTransform);
            break;
        } case 2: {
            scaling = vec3(0.75f, 0.75f, 0.75f);
            dragon.addTransform(getTransform(scaling, rotateAxis, rotateAngle, translation));
            break;
        } case 3: {
            mat4 preTransform = translate(mat4(1.0f), vec3(-1.5f, 0.0f, -15.0f));
            scaling = vec3(2.0f, 2.0f, 2.0f);
            translation += vec3(0.0f, -1.0f, 0.0f);
            david.addTransform(getTransform(scaling, rotateAxis, rotateAngle, translation) * preTransform);
        }
    }
}

void Scene::createSkyBox(){
    skyBox = Model("cuboid.obj", texUnit, SKYBOX);
    skyBox.addTransform(getTransform(vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f, vec3(0.0f, 0.0f, 0.0f)));

    tree1 = Model("Trees/tree1.obj", texUnit, TREE);
    tree2 = Model("Trees/tree2.obj", texUnit, TREE);

    for(unsigned int i = 0; i < 12; i++){
        float theta = (float)i * 30.0f; 
        int k = rand();
        double k_double = (double)k / (double)(RAND_MAX);
        if(k%2==0){
            //Tree 1
            mat4 transform = mat4(1.0f);
            transform = rotate(transform, radians(theta), vec3(0.0f, 1.0f, 0.0f));
            transform = translate(transform, vec3(0.0f, -20.0f + i * k_double * 5.0f, -500.0f));
            transform = rotate(transform, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
            transform = scale(transform, vec3(1.0f, 2.0f, 2.0f));
            tree1.addTransform(transform);
        }else{
            //Tree 2
            mat4 transform = mat4(1.0f);
            transform = rotate(transform, radians(theta), vec3(0.0f, 1.0f, 0.0f));
            transform = translate(transform, vec3(0.0f, -20.0f + i * k_double * 5.0f, -500.0f));
            transform = scale(transform, vec3(4.0f, 4.0f, 1.0f));
            transform = rotate(transform, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
            tree2.addTransform(transform);
        }
    }
}