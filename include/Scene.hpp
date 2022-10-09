#ifndef SCENE_HPP
#define SCENE_HPP

#include "gui.hpp"
#include "Model.hpp"
#include "Shader.hpp"

class Scene {

    private:
        GUI* gui;

        //Objects of the scene
        //Lamp Post(Cylinder) * 24
        vector<Model> lampPosts;
        //Cuboidal Boxes * 20
        vector<Model> statueBases;
        //Obj Models
        vector<Model> statueHeads;
        //Road + Grass + Lake
        vector<Model> terrain;
        //Sun + 25 * Over each lamp post
        vector<Model> lightSources;
        //6 * Cube Faces
        vector<Model> skyBox;

        unordered_map<ShaderType, Shader*> shaders;
        unordered_map<ModelType, pair<int, unsigned int>> texUnit;
         
        //Separate model creating functions
        void createTerrain();
        void createLampPosts();
        void createStatues();
         
    public:
        Scene();
        Scene(int argc, char** argv);
        void init();
        void run(); 
        void end(); 
        void draw();

};

#endif