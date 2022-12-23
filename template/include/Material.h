#ifndef MATERIAL_H_DEFINED
#define MATERIAL_H_DEFINED
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <stdexcept>

struct Material {
	// Shader program
	GLuint m_program=0;
	GLuint m_type = 0; //0 = default 1 = skybox 2=mirror
	// Material parameters
	glm::vec4 m_color;
	glm::vec4 ambiant;
	GLfloat m_specular;
	glm::vec4 emmisive;
	glm::vec4 transparent;
	glm::vec4 reflective;

	GLuint m_texture=-1;
	GLuint m_normal=-1;
	GLuint m_roughness=-1;
	std::string name;
	std::string diffusePath;
	std::string normalMapPath;


	inline void check() {
		if (m_program == 0) {
			throw std::runtime_error("Shader program not initialized");
		}
	}

	Material(): m_program(0) {

	}

	virtual ~Material();

	virtual void init();

	virtual void clear();

	void bind();

	virtual void internalBind();

	void setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix);

	GLint getAttribute(const std::string& in_attributeName);

	GLint getUniform(const std::string& in_uniformName);
};

#endif
