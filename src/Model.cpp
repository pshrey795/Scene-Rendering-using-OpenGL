#include "../include/Model.hpp"

Model::Model(){
}

Model::Model(ModelType modelType){
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures; 
    this->modelType = modelType;

    //Terrain
    switch(modelType){
        case GRASS:{
            buildSquare(vertices, indices);
            Texture texture;
            texture.id = getTextureFromFile("grass.jpeg");
            texture.type = DIFFUSE;
            textures.push_back(texture);
            break;
        }case LAKE:{
            buildSquare(vertices, indices);
            Texture texture;
            texture.id = getTextureFromFile("water.jpeg");
            texture.type = DIFFUSE;
            textures.push_back(texture);
            Texture texture2;
            texture2.id = getTextureFromFile("lake_floor.jpeg");
            texture2.type = DIFFUSE;
            textures.push_back(texture2);
            Texture texture3;
            setSTBIFlip(false);
            texture3.id = getCubeMap("SkyBox");  
            setSTBIFlip(false);
            texture3.type = CUBE_MAP;
            textures.push_back(texture3);
            break;
        }case ROAD:{
            buildSquare(vertices, indices);
            Texture texture;
            texture.id = getTextureFromFile("road.jpeg");
            texture.type = DIFFUSE;
            textures.push_back(texture);
            Texture texture2;
            texture2.id = getTextureFromFile("road_normal.jpeg");
            texture2.type = NORMAL_MAP;
            textures.push_back(texture2);
            Texture texture3;
            texture3.id = getTextureFromFile("road_disp.jpeg");
            texture3.type = DEPTH_MAP;
            textures.push_back(texture3);
            break;
        }case TEST: {
            buildSquare(vertices, indices);
            break;
        }
    }

    meshes.push_back(Mesh(vertices, indices, textures));
}

Model::Model(string obj_path, ModelType modelType){
    this->modelType = modelType;
    loadModel(objDir + obj_path); 

    //Loading Textures separately 
    switch(modelType){
        case SKYBOX: {
            Texture texture;
            setSTBIFlip(false);
            texture.id = getCubeMap("SkyBox");  
            setSTBIFlip(false);
            texture.type = CUBE_MAP;
            addTexture(texture);
            break;
        }case BASE: {
            Texture texture; 
            texture.id = getTextureFromFile("pedestal.jpg");
            texture.type = DIFFUSE;
            addTexture(texture);
            Texture texture2;
            texture2.id = getTextureFromFile("pedestal_normal.jpg");
            texture2.type = NORMAL_MAP;
            addTexture(texture2);
            break;
        }
    }
}

void Model::loadModel(string obj_path){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(obj_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
    vector<Vertex> vertices; 
    vector<unsigned int> indices;

    //Vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        Vertex vertex;
        vec3 v; 
        // positions
        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;
        vertex.pos = v;
        // normals
        if (mesh->HasNormals()){
            v.x = mesh->mNormals[i].x;
            v.y = mesh->mNormals[i].y;
            v.z = mesh->mNormals[i].z;
            vertex.normal = v;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]){
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
            // tangent
            v.x = mesh->mTangents[i].x;
            v.y = mesh->mTangents[i].y;
            v.z = mesh->mTangents[i].z;
            vertex.tangent = v;
            // bitangent
            v.x = mesh->mBitangents[i].x;
            v.y = mesh->mBitangents[i].y;
            v.z = mesh->mBitangents[i].z;
            vertex.bitangent = v;
        }else{
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    //Indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    if(modelType == TREE){
        Material mat;
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color(0.0f, 0.0f, 0.0f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        mat.diffuse = vec3(color.r, color.g, color.b);
        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        mat.specular = vec3(color.r, color.g, color.b);
        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        mat.ambient = vec3(color.r, color.g, color.b);
        material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        mat.emission = vec3(color.r, color.g, color.b);
        float specularity;
        material->Get(AI_MATKEY_SHININESS, specularity);
        mat.specularity = specularity;

        return Mesh(vertices, indices, mat);
    }else {
        return Mesh(vertices, indices);
    }
}

void Model::draw(Shader &shader, ShaderType shaderType) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader, modelType, shaderType);
    }
}

void Model::addTransform(mat4 model){
    for(int i = 0; i < meshes.size(); i++){
        meshes[i].addTransform(model);
    }
}

void Model::addTexture(Texture texture){
    for(int i = 0; i < meshes.size(); i++){
        meshes[i].addTexture(texture);
    }
}