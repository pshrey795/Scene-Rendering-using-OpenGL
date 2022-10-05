#ifndef MODEL_HPP
#define MODEL_HPP

#include "Mesh.hpp"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

class Model{
    public:
        Model();
        Model(string obj_path);
        void draw(Shader &shader);

        //Model Transformations
        mat4 localTransform;
        void updateTransform(mat4 transform);
    
    private:
        vector<Mesh> meshes;
        void loadModel(string obj_path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};

#endif