#ifndef GUI_HPP
#define GUI_HPP

#include "common.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class GUI {
    public:
        unsigned int errCode = 0;

        GUI(const char* winTitle, int width, int height);
        void activateWindow();
        void run();

    private:
        GLFWwindow* window;
        void processInput();

};

#endif