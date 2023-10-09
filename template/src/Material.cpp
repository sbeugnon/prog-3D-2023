// Local includes
#include "Material.h"
#include "Shader.h"
#include "Context.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>

Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

void Material::init() {
	// TODO : Change shader by your
	switch (Context::rendererType)
	{
		case RendererType::Unlit:
			{
				m_program = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
			}
			break;
		case RendererType::Reflective:
			{
				m_program = load_shaders("shaders/reflective/vertex.glsl", "shaders/reflective/fragment.glsl");
			}
			break;
		case RendererType::PBR:
			{
				m_program = load_shaders("shaders/pbr/vertex.glsl", "shaders/pbr/fragment.glsl");
			}
			break;
		case RendererType::Phong:
			{
				m_program = load_shaders("shaders/phong/vertex.glsl", "shaders/phong/fragment.glsl");
			}
			break;
		default:
			break;
	}

	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
	m_texture = -1;
	m_textureBump = -1;
	m_pbr.albedo = {1.0, 1.0, 1.0};
	m_pbr.ao = 1.0;
	m_pbr.metalness = 0.0;
	m_pbr.roughness = 0.5;
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
	if (Context::rendererType == RendererType::Unlit)
	{
		GLint color = getUniform("color");
		glUniform4fv(color, 1, glm::value_ptr(m_color));
		if (m_texture != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glUniform1i(getUniform("colorTexture"), 0);
		}
		if (m_textureBump != -1)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_textureBump);
			glUniform1i(getUniform("bumpTexture"), 1);
		}
	}

	if (Context::rendererType == RendererType::Reflective)
	{
		glUniform3fv(getUniform("cameraPos"), 1, glm::value_ptr(Context::camera.position));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Context::skyboxTexture);
	}

	if (Context::rendererType == RendererType::PBR)
	{
		glUniform3fv(getUniform("params.albedo"), 1, glm::value_ptr(m_pbr.albedo));
		glUniform1f(getUniform("params.roughness"), m_pbr.roughness);
		glUniform1f(getUniform("params.metalness"), m_pbr.metalness);
		glUniform1f(getUniform("params.ao"), m_pbr.ao);
	}

	if (Context::rendererType == RendererType::Phong)
	{
		if (m_textureBump != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textureBump);
			glUniform1i(getUniform("bumpTexture"), 0);
		}
		glUniform3fv(getUniform("light"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glUniform3fv(getUniform("lightColor"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glUniform3fv(getUniform("material.ambient"), 1, glm::value_ptr(m_phong.ambient));
		glUniform3fv(getUniform("material.diffuse"), 1, glm::value_ptr(m_phong.diffuse));
		glUniform3fv(getUniform("material.specular"), 1, glm::value_ptr(m_phong.specular));
		glUniform1f(getUniform("material.shininess"), m_phong.shininess);
	}

	// TODO : Add your custom parameters here
}

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	GLint i = glGetUniformLocation(m_program, in_uniformName.c_str());
	return i;
}
