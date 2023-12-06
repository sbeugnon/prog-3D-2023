// Local includes
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "stb_image.h"

Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

void Material::init() {
	if(m_type==0){
		m_program = load_shaders("shaders/PBR/vertex.glsl", "shaders/PBR/fragment.glsl");
	}
	if(m_type==1)m_program = load_shaders("shaders/skybox/vertex.glsl", "shaders/skybox/fragment.glsl");
	//std::cout<<"m_program "<<m_program<<std::endl;
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
}

void Material::clear() {
	
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind() {
	check();
	glUseProgram(m_program);
	internalBind();
}

void Material::internalBind() {
	// bind parameters
	GLint color = getUniform("color");
	if(color != -1) glUniform4fv(color, 1, glm::value_ptr(m_color));
	GLint specular = getUniform("specular");
	if(specular != -1) glUniform1fv(specular, 1, &m_specular);



	if (m_texture != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		
		glUniform1i(getUniform("colorTexture"), 0);
	}
	if (m_normal != -1) {
		glActiveTexture(GL_TEXTURE0+1);
		glBindTexture(GL_TEXTURE_2D, m_normal);
		
		glUniform1i(getUniform("normalMap"), 1);
	}	
	if (m_roughness != -1) {
		glActiveTexture(GL_TEXTURE0+2);
		glBindTexture(GL_TEXTURE_2D, m_roughness);
		
		glUniform1i(getUniform("roughnessMap"), 2);
	}		
	// TODO : Add your custom parameters here
}

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
	//glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(normalMatrix));
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
