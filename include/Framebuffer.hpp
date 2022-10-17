#ifndef FBFR_HPP
#define FBFR_HPP

#include "common.hpp"

enum FBUF_TYPE {
    COLOR, 
    DEPTH,
    SHADOW_MAP
};

class FBuffer {
    public:
        FBuffer();
        FBuffer(float width, float height, FBUF_TYPE type = COLOR);
        void activateBuffer();
        void deactivateBuffer();
        unsigned int getTextureID();
        unsigned int getRenderID();
        float width, height;

    private:
        unsigned int FBO, textureID, renderID;
};

unsigned int getEmptyTexture(float width, float height, FBUF_TYPE type = COLOR);
unsigned int getRenderBuffer(float width, float height);

#endif