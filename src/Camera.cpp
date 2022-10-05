#include "../include/Camera.hpp"

Camera::Camera(vec3 position, vec3 target, vec3 up, float width, float height, float near, float far, float yaw, float pitch) :
MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY) {
    Position = position;
    Depth = (target - position).length();
    Front = normalize(target - position);
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();

    screenWidth = width;
    screenHeight = height;
    screenNearDepth = near;
    screenFarDepth = far;
}

mat4 Camera::getViewMatrix(){
    return lookAt(Position, Position + Depth * Front, Up);
}

mat4 Camera::getPerspectiveMatrix(){
    return perspective(radians(ZOOM), screenWidth / screenHeight, screenNearDepth, screenFarDepth);
}

void Camera::processKeyInput(Camera_Movement direction, float deltaTime){
    float velocity = MovementSpeed * deltaTime;
    if(direction == FORWARD){
        Position += Front * velocity;
    }else if(direction == BACKWARD){
        Position -= Front * velocity;
    }else if(direction == LEFT){
        Position -= Right * velocity;
    }else if(direction == RIGHT){
        Position += Right * velocity;
    }else if(direction == UP){
        Position += Up * velocity;
    }else if(direction == DOWN){
        Position -= Up * velocity;
    }
}

void Camera::processMouseInput(float xoffset, float yoffset, GLboolean constrainPitch){
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    //Keeping pitch in bounds
    if(constrainPitch){
        if(Pitch > 89.0f){
            Pitch = 89.0f;
        }
        if(Pitch < -89.0f){
            Pitch = -89.0f;
        }
    }

    //Update camera vectors based on the new Euler angles
    updateCameraVectors();
}

void Camera::updateCameraVectors(){
    vec3 front;
    front.x = cos(radians(Yaw)) * cos(radians(Pitch));
    front.y = sin(radians(Pitch));
    front.z = sin(radians(Yaw)) * cos(radians(Pitch));
    Front = normalize(front);
    Right = normalize(cross(Front, WorldUp));  
    Up    = normalize(cross(Right, Front));
}

