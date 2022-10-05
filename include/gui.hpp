#ifndef GUI_HPP
#define GUI_HPP

#include "common.hpp"
#include "Camera.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

//Global Mouse Parameters
extern float lastX, lastY;
extern float xOffset, yOffset;
extern bool firstMouse;
extern bool checkMouse;

class GUI {
    public:
        GUI(const char* winTitle, int width, int height);
        void activateWindow();
        void processInput();
        bool shouldClose();
        void swapBuffers();

        //Camera
        Camera* camera;

        //Window Parameters
        float width, height;        
    private:
        GLFWwindow* window;
        unsigned int errCode = 0;

        //Mouse 
        bool allowMouse;
};

#endif