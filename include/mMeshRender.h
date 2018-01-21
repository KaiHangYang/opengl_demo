#ifndef M_MESH
#define M_MESH

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mShader.h"

class MeshEntry {
public:
    enum BUFFERS {
        VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER,INDEX_BUFFER
    };
    GLuint VAO;
    GLuint VBO[4];
    unsigned int elmNum;

    MeshEntry(aiMesh * mesh, GLuint vao);
    ~MeshEntry();

    void render();
};

class mMeshRender {
public:
    std::vector<MeshEntry *> meshEntries;

    mMeshRender(mShader* shader);
    ~mMeshRender();
    void addMesh(std::string mesh_path);
    void renderOne(int mesh_num, glm::mat4 model_mat, glm::mat4 view_mat, glm::mat4 proj_mat);
private:

    void setLightParams(int type = 0);
    GLuint VAO;
    mShader * shader;
};

#endif
