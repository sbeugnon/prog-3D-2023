// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>
// STD includes
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <float.h>
#include <queue>
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
// Local includes
#include "Camera.h"
#include "Shader.h"
#include "Material.h"
#include "Context.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "SceneLoader.h"
#include "Texture.h"




// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
GLuint skyboxTexture;
Material skyboxM;
GLuint cubemapBuf;
GLuint VBO;
void init() {
	// Context::camera.initPos();
	Context::camera.resize(SCREENWIDTH, SCREENHEIGHT);
	//glCullFace (GL_BACK);
	//glEnable (GL_CULL_FACE);
	glEnable (GL_DEPTH_TEST);
	glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
}

void beforeLoop() {
	// Take first instance center as target
	if (!Context::instances.empty()) {
		glm::vec3 centerOfInstance0 = Context::instances.at(0).mesh->center;
		std::cerr << glm::to_string(centerOfInstance0) << std::endl;

		glm::vec3 target(Context::instances.at(0).matrix * glm::vec4(centerOfInstance0, 1.0));
		Context::camera.position = glm::vec3(0.5,0.5,0.5);
		Context::camera.target = target;//lookat
		Context::camera.forward = glm::normalize(target - Context::camera.position);
		Context::camera.view = Context::camera.getViewMatrix();
	}

	// std::cerr << "projection:" << glm::to_string(Context::camera::projection) << std::endl;
	// std::cerr << "view:" << glm::to_string(Context::camera::view) << std::endl;

}


void draw() {
	if (Context::refreshMatrices) {
		// Context::camera.refreshMatrices();
		Context::camera.view = Context::camera.getViewMatrix();
		Context::camera.projection = Context::camera.getProjectionMatrix();
		Context::refreshMatrices = false;
	}
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < Context::instances.size(); ++i) {
		Instance& inst = Context::instances[i];
		Material* material = inst.material;

		Mesh* mesh = inst.mesh;
		material->bind();
		glBindBuffer(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glUniform1i(material->getUniform("skybox"), 11);
		GLint campos = material->getUniform("cameraPos");
		glUniform3fv(campos, 1, glm::value_ptr(Context::camera.position));
		material->setMatrices(Context::camera.projection, Context::camera.view, inst.matrix);
		mesh->draw();
	}
	glDepthFunc(GL_LEQUAL);
	skyboxM.bind();
	glm::mat4 view = glm::mat4(glm::mat3(Context::camera.view)); 
	glUniformMatrix4fv(skyboxM.getUniform("view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(skyboxM.getUniform("projection"), 1, false, glm::value_ptr(Context::camera.projection));

	glBindVertexArray(cubemapBuf);
	glActiveTexture(GL_TEXTURE0+11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glUniform1i(skyboxM.getUniform("skybox"), 11);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);



}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
	glFlush();
	glutSwapBuffers();
}


int main (int argc, char ** argv) {
	if (argc < 2) {
		std::cerr << "Missing parameter: <path-to-model>" << std::endl;
		exit (EXIT_FAILURE);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
	window = glutCreateWindow("TP");

	init();
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	key('?', 0, 0);

	std::string path(argv[1]);
	loadDataWithAssimp(path);

	
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	
	glGenVertexArrays(1, &cubemapBuf);
	glGenBuffers(1,&VBO);
	
	glBindVertexArray(cubemapBuf);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	std::string folder = std::filesystem::current_path().c_str();
	folder+='/';
	//glEnableVertexAttribArray(0);
	std::vector<std::string> skyboxfaces
	{
		"cubemap/skybox1.png",
		"cubemap/skybox3.png",
		"cubemap/skybox4.png",
		"cubemap/skybox5.png",
		"cubemap/skybox0.png",
		"cubemap/skybox2.png"
	};
	skyboxTexture = loadCubemap(skyboxfaces);
	skyboxM.m_type=1;
	skyboxM.init();

	beforeLoop();
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glutMainLoop();
	clearContext();
	return EXIT_SUCCESS;
}
