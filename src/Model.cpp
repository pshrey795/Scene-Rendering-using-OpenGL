#include "../include/Model.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

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
                stbi_set_flip_vertically_on_load(false);
                texture.id = getCubeMap("SkyBox", n);  
                stbi_set_flip_vertically_on_load(false);
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

unsigned int Model::getTextureFromFile(string fileName, int texUnit){
    string path = texDir + fileName;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if(data){
        GLenum format;
        if (nrComponents == 1){
            format = GL_RED;
        }else if (nrComponents == 3){
            format = GL_RGB;
        }else if (nrComponents == 4){
            format = GL_RGBA;
        }
        glActiveTexture(GL_TEXTURE0 + texUnit);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }else{
        cout << "Texture failed to load at path: " << path << endl;
    }
    stbi_image_free(data);
    return textureID;
}

unsigned int Model::getCubeMap(string fileDir, int texUnit){
    string pathDir = texDir + fileDir;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    unsigned char *data;
    for(unsigned int i = 0; i < 6; i++){
        string filePath = pathDir + "/" + to_string(i+1) + ".jpg";
        data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
        GLenum format;
        if (nrComponents == 1){
            format = GL_RED;
        }else if (nrComponents == 3){
            format = GL_RGB;
        }else if (nrComponents == 4){
            format = GL_RGBA;
        }
        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }else{
            cout << "Cubemap tex failed to load at path: " << filePath << endl;
        }
        stbi_image_free(data);
    }

    //Setting Parameters
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID; 
}