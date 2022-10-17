#include "../include/Framebuffer.hpp"

FBuffer::FBuffer(){
    
}

FBuffer::FBuffer(float width, float height, FBUF_TYPE type){
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    textureID = getEmptyTexture(width, height, type);

    switch(type){
        case COLOR:{
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
            break;
        }case SHADOW_MAP:{
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            break;
        }
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        cout << "Framebuffer not complete!" << endl;
    }else{
        cout << "Framebuffer complete!" << endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBuffer::activateBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void FBuffer::deactivateBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FBuffer::getTextureID(){
    return textureID;
}

unsigned int FBuffer::getRenderID(){
    return renderID;
}

//Extracting Textures
unsigned int getEmptyTexture(float width, float height, FBUF_TYPE type){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    switch(type){
        case COLOR: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        } case SHADOW_MAP: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
            break;
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0); 
    return textureID;
}

//Extracting Render Buffer
unsigned int getRenderBuffer(float width, float height){
    unsigned int renderID;
    glGenRenderbuffers(1, &renderID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return renderID;
}