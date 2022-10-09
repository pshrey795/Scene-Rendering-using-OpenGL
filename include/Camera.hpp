#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "common.hpp"

//Specific camera movements
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  25.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera{
public:
    //Camera Attributes
    vec3 Position;
    double Depth;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;

    //Screen Parameters
    float screenWidth, screenHeight;
    float screenNearDepth, screenFarDepth;

    //Euler Angles
    float Yaw;
    float Pitch;

    //Camera options
    float MovementSpeed;
    float MouseSensitivity;

    //Constructors
    Camera(vec3 position, vec3 target, vec3 up, float width, float height, float near = 0.1f, float far = 1000.0f, float yaw = YAW, float pitch = PITCH);

    //Returns the view matrix for vertex shader
    mat4 getViewMatrix();

    //Returns the perspective matrix for the vertex shader 
    mat4 getPerspectiveMatrix();

    //Processing Input from keyboard
    void processKeyInput(Camera_Movement direction, float deltaTime);

    //Processing Input from mouse
    void processMouseInput(float xoffset, float yoffset, GLboolean constrainPitch = true);

private:
    //Calculate direction vectors from euler angles
    void updateCameraVectors();
};

#endif