#ifndef MODEL_HPP
#define MODEL_HPP

#include "Mesh.hpp"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

class Model{
    public:
        Model();
        Model(ModelType modelType);
        Model(string obj_path, ModelType modelType = NONE);
        void draw(Shader &shader, ShaderType shaderType);

        //Model Transformations
        void addTransform(mat4 transform); 
        void addTexture(Texture texture);
    
    private:
        vector<Mesh> meshes;
        ModelType modelType;
        void loadModel(string obj_path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

};

#endif