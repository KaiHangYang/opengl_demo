#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "include/mOpenGLUtils.h"

#include "include/mShader.h"
#include "include/mMeshRender.h"
#include "include/mRotateUtils.h"

int main() {
    // Must init opengl session first!
    int wnd_width = 400;int wnd_height = 400;
    mOpenGLUtils gl_session("test", wnd_width, wnd_height);
    gl_session.setCallback(mArcBall::mouse_button_callback, mArcBall::mouse_move_callback, nullptr);
    mShader model_shader("./shaders/multilight.vert", "./shaders/multilight.frag");
    mMeshRender mesh_render(&model_shader);
    mesh_render.addMesh("./models/test-0.6.ply");

    glm::mat4 model_mat(1.f);
    glm::mat4 rotate_mat(1.f);
    glm::mat4 proj_mat = glm::perspective(glm::radians(45.f), static_cast<float>(wnd_width) / wnd_height, 0.1f, 100.f);
    glm::mat4 view_mat = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    while (gl_session.checkShouldClose()) {
        gl_session.drawStart();
        rotate_mat = mArcBall::getRotateMat(wnd_width, wnd_height, view_mat);
        model_mat = rotate_mat * model_mat;

        mesh_render.renderOne(0, model_mat, view_mat, proj_mat);

        gl_session.drawEnd();
    }
    return 0;
}
