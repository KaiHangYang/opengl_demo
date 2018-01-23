#include "../include/mMeshRender.h"
#include <string.h>

MeshEntry::~MeshEntry() {
    glBindVertexArray(VAO);
    if (VBO[VERTEX_BUFFER]) {
        glDeleteBuffers(1, &VBO[VERTEX_BUFFER]);
    }

    if (VBO[TEXCOORD_BUFFER]) {
        glDeleteBuffers(1, &VBO[TEXCOORD_BUFFER]);
    }

    if (VBO[NORMAL_BUFFER]) {
        glDeleteBuffers(1, &VBO[NORMAL_BUFFER]);
    }

    if (VBO[INDEX_BUFFER]) {
        glDeleteBuffers(1, &VBO[INDEX_BUFFER]);
    }
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}

MeshEntry::MeshEntry(aiMesh * mesh, GLuint vao) {
    VAO = vao;
    VBO[VERTEX_BUFFER] = 0;
    VBO[TEXCOORD_BUFFER] = 0;
    VBO[NORMAL_BUFFER] = 0;
    VBO[INDEX_BUFFER] = 0;

    glBindVertexArray(VAO);

    if (mesh->HasPositions()) {
        std::vector<GLfloat> vertexs;
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            vertexs.push_back(mesh->mVertices[i].x);
            vertexs.push_back(mesh->mVertices[i].y);
            vertexs.push_back(mesh->mVertices[i].z);
        }
        
        glGenBuffers(1, &VBO[VERTEX_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, vertexs.size()*sizeof(GLfloat), &vertexs[0], GL_STATIC_DRAW);
    }

    if (mesh->HasFaces()) {
        std::vector<unsigned int> indices;
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }
        glGenBuffers(1, &VBO[INDEX_BUFFER]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        elmNum = indices.size();
    }

    if (mesh->HasNormals()) {
        std::vector<GLfloat> normals;
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            normals.push_back(mesh->mNormals[i].x);
            normals.push_back(mesh->mNormals[i].y);
            normals.push_back(mesh->mNormals[i].z);
        }
        glGenBuffers(1, &VBO[NORMAL_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MeshEntry::render() {
    glBindVertexArray(VAO);

    if (0 != VBO[VERTEX_BUFFER]) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
        glEnableVertexAttribArray(0);
    }
    if (0 != VBO[NORMAL_BUFFER]) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL_BUFFER]);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(2);
    }
    if (0 != VBO[INDEX_BUFFER]) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFFER]);
        glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(1);
    }

    glDrawElements(GL_TRIANGLES, elmNum, GL_UNSIGNED_INT, NULL); 
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void mMeshRender::addMesh(std::string mesh_path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(mesh_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR: ASSIMP::" << importer.GetErrorString() << std::endl;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        meshEntries.push_back(new MeshEntry(scene->mMeshes[i], VAO));
    }
}

mMeshRender::mMeshRender(mShader * shader) {
    glGenVertexArrays(1, &VAO);
    this->shader = shader;
}
mMeshRender::~mMeshRender() {
    for (unsigned int i = 0; i < meshEntries.size(); ++i) {
        delete meshEntries[i];
    }
    meshEntries.clear();
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
}
 
// The fixed light and parameter
void mMeshRender::setLightParams(int type) {
    bool sign = 1;
    // set the light at very far place
    this->shader->setVal("pointLights[0].position", glm::vec3(0, sign * 4000, 4000));
    this->shader->setVal("pointLights[1].position", glm::vec3(0, sign * 4000, -4000));
    this->shader->setVal("pointLights[2].position", glm::vec3(-4000, sign*4000, 0));
    this->shader->setVal("pointLights[3].position", glm::vec3(4000, sign*4000, 0));

    for (int i = 0; i < 4; ++i) {
        this->shader->setVal(("pointLights["+std::to_string(i)+"].ambient").c_str(), glm::vec3(0.1f, 0.1f, 0.1f));
        this->shader->setVal(("pointLights["+std::to_string(i)+"].diffuse").c_str(), glm::vec3(0.5f, 0.5f, 0.5f));
        this->shader->setVal(("pointLights["+std::to_string(i)+"].specular").c_str(), glm::vec3(0.7f, 0.7f, 0.7f));
    }
}

void mMeshRender::renderOne(int mesh_num, glm::mat4 model_mat, glm::mat4 view_mat, glm::mat4 proj_mat) {
    if (meshEntries.size() <= mesh_num) {
        std::cout << "(mMeshRender.cpp):Mesh Num is not valid!" << std::endl;
        exit(-1);
    }

    glBindVertexArray(VAO);
    shader->use();

    glm::mat4 view_r_mat = glm::mat4(glm::mat3(view_mat));
    glm::mat4 view_t_mat = glm::inverse(view_r_mat) * view_mat;
    // Calculate the view pose
    shader->setVal("viewPos", glm::vec3(view_t_mat[3][0], view_t_mat[3][1], view_t_mat[3][2]));
    // Set the fixed lightpos
    this->setLightParams(0);

    shader->setVal("fragColor", glm::vec3(0.9882352941176471, 0.788235294117647, 0.5294117647058824));
    shader->setVal("projection", proj_mat);
    shader->setVal("view", view_mat);
    shader->setVal("model", model_mat);
    shader->setVal("normMat", glm::transpose(glm::inverse(model_mat)));
    meshEntries[mesh_num]->render();
}
