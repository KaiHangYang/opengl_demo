#include "../include/mOpenGLUtils.h"

mOpenGLUtils::~mOpenGLUtils() {
    this->terminate();
}
mOpenGLUtils::mOpenGLUtils(std::string win_title, int wnd_width, int wnd_height) {
    glfwSetErrorCallback(this->error_callback);
    this->win_title = win_title;
    this->wnd_width = wnd_width;
    this->wnd_height = wnd_height;
    this->window = initWindow();
}
GLFWwindow * mOpenGLUtils::initWindow() {
    if (false == glfwInit()) {
        std::cout << "glfwInit failed!" << std::endl;
        return nullptr;
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(this->wnd_width, this->wnd_height, this->win_title.c_str(), nullptr, nullptr);
    if (!this->window) {
        std::cout << "Window create failed!" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW\n" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return window;
}

bool mOpenGLUtils::checkInited() {
    return this->window != nullptr;
}

void mOpenGLUtils::setCallback(void (mouse_button_callback)(GLFWwindow *, int, int, int), void (mouse_move_callback)(GLFWwindow *, double, double), void(keybutton_callback) (GLFWwindow *, int, int, int, int), void(scroll_callback)(GLFWwindow*,double,double)) {
    if (mouse_button_callback) {
        glfwSetMouseButtonCallback(window, mouse_button_callback);
    }

    if (mouse_move_callback) {
        glfwSetCursorPosCallback(window, mouse_move_callback);
    }

    if (keybutton_callback) {
        glfwSetKeyCallback(window, keybutton_callback);
    }

    if (scroll_callback) {
        glfwSetScrollCallback(window, scroll_callback);
    }
}

void mOpenGLUtils::drawStart() {
    if (!checkInited()) {
        std::cout << "Glfw init failed!" << std::endl;
        return;
    }

    this->makeCurCtx();
    glClearColor(0.4627450980392157f, 0.5882352941176471f, 0.8980392156862745f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void mOpenGLUtils::makeCurCtx() {
    glfwMakeContextCurrent(this->window);
}

void mOpenGLUtils::drawEnd() {
    if (!checkInited()) {
        std::cout << "Glfw init failed!" << std::endl;
        return;
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool mOpenGLUtils::checkShouldClose() {
    if (!checkInited()) {
        std::cout << "Glfw init failed!" << std::endl;
        return false;
    }
    return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
}

void mOpenGLUtils::terminate() {
    glfwTerminate();
}
