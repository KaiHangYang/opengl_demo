#ifndef M_ROTATE_UTILS
#define M_ROTATE_UTILS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace mArcBall {

    extern float initX, initY, curX, curY;
    extern bool isMousePressed;

    void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
    void mouse_move_callback(GLFWwindow * window, double x, double y);
    glm::vec3 getArcballVector(float x, float y, int wnd_width, int wnd_height);
    glm::mat4 getRotateMat(int wnd_width, int wnd_height, glm::mat4 view_mat);
};
#endif
