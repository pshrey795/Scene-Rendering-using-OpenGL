#include "../include/Model.hpp"

Model::Model(){
    vector<Vertex> vertices; 
    vector<unsigned int> indices;

    vertices.push_back(Vertex(vec3(0.0f, 0.0f, 0.0f)));
    vertices.push_back(Vertex(vec3(1.0f, 0.0f, 0.0f)));
    vertices.push_back(Vertex(vec3(1.0f, 1.0f, 0.0f)));
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    meshes.push_back(Mesh(vertices, indices));

    this->localTransform = mat4(1.0f);
}

Model::Model(string obj_path){
    loadModel(obj_path);
    this->localTransform = mat4(1.0f);
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
    vector<Texture> textures;

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
    return Mesh(vertices, indices);
}

void Model::draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}

void Model::updateTransform(mat4 transform){
    this->localTransform *= transform;
}