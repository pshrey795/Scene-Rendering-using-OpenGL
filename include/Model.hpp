#ifndef MODEL_HPP
#define MODEL_HPP

#include "Mesh.hpp"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

class Model{
    public:
        Model();
        Model(ModelType modelType, unordered_map<ModelType, pair<int,unsigned int>> &texUnit);
        Model(string obj_path, ModelType modelType = NONE);
        void draw(Shader &shader, ShaderType shaderType);

        //Model Transformations
        void updateTransform(mat4 transform);
        void addTransform(mat4 transform); 

    
    private:
        vector<Mesh> meshes;
        ModelType modelType;
        void loadModel(string obj_path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        //Extracting a new texture from an image file 
        unsigned int getTextureFromFile(string fileName, int texUnit);
};

#endif