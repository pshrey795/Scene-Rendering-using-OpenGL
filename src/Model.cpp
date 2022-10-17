#include "../include/Model.hpp"

Model::Model(){
    
}

Model::Model(ModelType modelType, unordered_map<ModelType, pair<int,unsigned int>> &texUnit){
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures; 
    this->modelType = modelType;

    //Terrain
    switch(modelType){
        case GRASS:{
            buildSquare(vertices, indices);
            Texture texture;
            if(texUnit.find(GRASS) == texUnit.end()){
                int n = texUnit.size();
                texture.texUnit = n;
                texture.id = getTextureFromFile("grass.jpeg", n);
                texUnit.insert({GRASS, make_pair(n,texture.id)});
            }else{
                auto p = texUnit[GRASS];
                texture.texUnit = p.first;
                texture.id = p.second;
            }
            texture.type = DIFFUSE;
            textures.push_back(texture);
            break;
        }case LAKE:{
            buildSquare(vertices, indices);
            Texture texture;
            if(texUnit.find(LAKE) == texUnit.end()){
                int n = texUnit.size();
                texture.texUnit = n;
                texture.id = getTextureFromFile("water.jpeg", n);
                texUnit.insert({LAKE, make_pair(n,texture.id)});
            }else{
                auto p = texUnit[LAKE];
                texture.texUnit = p.first;
                texture.id = p.second;
            }
            texture.type = DIFFUSE;
            textures.push_back(texture);
            break;
        }case ROAD:{
            buildSquare(vertices, indices);
            Texture texture;
            if(texUnit.find(ROAD) == texUnit.end()){
                int n = texUnit.size();
                texture.texUnit = n;
                texture.id = getTextureFromFile("road.jpeg", n);
                texUnit.insert({ROAD, make_pair(n,texture.id)});
            }else{
                auto p = texUnit[ROAD];
                texture.texUnit = p.first;
                texture.id = p.second;
            }
            texture.type = DIFFUSE;
            textures.push_back(texture);
            break;
        }
    }

    meshes.push_back(Mesh(vertices, indices, textures));
}

Model::Model(string obj_path, unordered_map<ModelType, pair<int,unsigned int>> &texUnit, ModelType modelType){
    this->modelType = modelType;
    loadModel(objDir + obj_path); 

    //Loading Textures separately 
    switch(modelType){
        case SKYBOX: {
            Texture texture;
            if(texUnit.find(SKYBOX) == texUnit.end()){
                int n = texUnit.size();
                texture.texUnit = n;
                setSTBIFlip(false);
                texture.id = getCubeMap("SkyBox", n);  
                setSTBIFlip(false);
                texUnit.insert({SKYBOX, make_pair(n,texture.id)});
            }else{
                auto p = texUnit[SKYBOX];
                texture.texUnit = p.first;
                texture.id = p.second;
            }
            texture.type = CUBE_MAP;
            for(int i = 0; i < meshes.size(); i++){
                meshes[i].textures.push_back(texture);
            }
            break;
        }
    }
}

void Model::loadModel(string obj_path){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(obj_path, aiProcess_Triangulate | aiProcess_FlipUVs);
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
        vec3 pos; 
        pos.x = mesh->mVertices[i].x; pos.y = mesh->mVertices[i].y; pos.z = mesh->mVertices[i].z;
        vertex.pos = pos;

        if(mesh->HasNormals()){
            vec3 norm;
            norm.x = mesh->mNormals[i].x; norm.y = mesh->mNormals[i].y; norm.z = mesh->mNormals[i].z;
            vertex.normal = norm;
        }

        if(mesh->mTextureCoords[0]){
            vec2 tex;
            tex.x = mesh->mTextureCoords[0][i].x; tex.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = tex;
        }else{
            vertex.texCoords = vec2(0.0f, 0.0f);
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