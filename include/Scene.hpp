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
        //Light Sources(Sun + 25 * Over each lamp post)
        vec3 sunDirection; 
        Model lamp;
        vector<vec3> lampPos;
        vector<bool> lampState;

        //Statues
        //Cuboidal Boxes * 20
        Model statueBase;
        //Obj Models
        Model bunny; 
        Model armadillo; 
        Model dragon; 
        Model david; 

        //Terrain
        //Road + Grass + Lake
        Model grass;
        Model lake;
        Model road;

        //Sky Box 
        //6 * Cube Faces
        Model skyBox; 
        Model tree1;
        Model tree2; 

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
        void createSkyBox();
         
    public:
        Scene();
        Scene(int argc, char** argv);
        void init();
        void run(); 
        void end(); 
        void draw();

};

#endif