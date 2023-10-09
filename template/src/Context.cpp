#include "Context.h"
// GLUT includes
#include <GL/glut.h>

void idle() {
    glutPostRedisplay();
}

void reshape(int w, int h) {
    Context::camera.resize(w, h);
    // refresh
    Context::refreshMatrices = true;
}


Camera Context::camera;
GLuint Context::skyboxProgram;
GLuint Context::skyboxTexture;
GLuint Context::skyboxVAO;
GLuint Context::skyboxVBO;
RendererType Context::rendererType;
std::vector<Instance> Context::instances;
std::vector<Material*> Context::materials;
std::vector<Mesh*> Context::meshes;
bool Context::refreshMatrices = true;
bool Context::fullScreen = false;

void clearContext() {
  for (Mesh* mesh: Context::meshes) {
    delete mesh;
  }
  for (Material* mat: Context::materials) {
    delete mat;
  }
  Context::meshes.clear();
  Context::materials.clear();
}
