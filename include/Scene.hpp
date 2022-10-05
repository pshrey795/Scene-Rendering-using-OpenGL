#ifndef SCENE_HPP
#define SCENE_HPP

#include "gui.hpp"
#include "Model.hpp"
#include "Shader.hpp"

class Scene {

    private:
        GUI* gui;
        vector<Model> sceneObjects;
        Shader* shader;
         
    public:
        Scene();
        Scene(int argc, char** argv);
        void init();
        void run(); 
        void end(); 
        void draw();

};

#endif