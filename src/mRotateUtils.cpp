#include "../include/mRotateUtils.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace mArcBall {

    float initX, initY, curX, curY, globalX, globalY;
    bool isMousePressed = false;

    void mouse_button_callback(GLFWwindow * window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                //std::cout << "mouse down" << std::endl;
                initX = curX = globalX;
                initY = curY = globalY;
                isMousePressed = true;
                break;
            }
        }
        else {
            switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                //std::cout << "mouse release" << std::endl;
                isMousePressed = false;
                break;
            }
        }
    }
    
    void mouse_move_callback(GLFWwindow * window, double x, double y) {
        globalX = x;
        globalY = y;
        if (isMousePressed) {
            curX = x;
            curY = y;
            //std::cout << "cursor is at (" << curX << ", " << curY << ")" << std::endl;
        }
    }

    glm::vec3 getArcballVector(float x, float y, int wnd_width, int wnd_height) {
        glm::vec3 P = glm::vec3(1.0*x/wnd_width*2 - 1.0,
			  1.0*y/wnd_height*2 - 1.0,
			  0);
        P.y = -P.y;
        float OP_squared = P.x * P.x + P.y * P.y;
        if (OP_squared <= 1*1)
            P.z = sqrt(1*1 - OP_squared);  // Pythagore
        else
            P = glm::normalize(P);  // nearest point
        return P;
    }

    // set the camera rotate
    glm::mat4 getRotateMat(int wnd_width, int wnd_height, glm::mat4 view_mat) {
        glm::mat4 mat_in_obj_coord = glm::mat4(1.f);
        if (initX != curX || initY != curY) {
            // -1 is for rotate the camera
            glm::vec3 va = getArcballVector(initX, initY, wnd_width, wnd_height);
            glm::vec3 vb = getArcballVector(curX, curY, wnd_width, wnd_height);

            float angle = acos(fmin(1.0f, glm::dot(va, vb)));
            glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
            glm::vec3 axis_in_obj_coord = glm::inverse(glm::mat3(view_mat)) * axis_in_camera_coord;

            mat_in_obj_coord = glm::rotate(mat_in_obj_coord, angle, axis_in_obj_coord);

            initX = curX;
            initY = curY;
        }

        return mat_in_obj_coord;
    }

}
