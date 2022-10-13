#ifndef SCENE_HPP
#define SCENE_HPP

#include "gui.hpp"
#include "Model.hpp"
#include "Shader.hpp"

class Scene {

    private:
        GUI* gui;

        //For testing purposes
        Model testModel;

        //Objects of the scene
        //Lamp Post(Cylinder) * 24
        Model lampPost;
        //Cuboidal Boxes * 20
        Model statueBase;
        //Obj Models
        Model bunny; 
        Model armadillo; 
        Model dragon; 
        Model david; 
        //Road + Grass + Lake
        Model grass;
        Model lake;
        Model road;
        //Sun + 25 * Over each lamp post
        vector<Model> lightSources;
        //6 * Cube Faces
        vector<Model> skyBox;

        //Book-keeping for Shaders/Textures
        unordered_map<ShaderType, Shader*> shaders;
        unordered_map<ModelType, pair<int, unsigned int>> texUnit;
         
        //Time keeping
        int frameCounter;
        float currentTime;

        //Separate model creating functions
        void createTerrain();
        void createLampPosts();
        void createStatues();
        void createStatueHead(int i, int j); 
         
    public:
        Scene();
        Scene(int argc, char** argv);
        void init();
        void run(); 
        void end(); 
        void draw();

};

#endif