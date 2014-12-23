/*
 * =====================================================================================
 *
 *       Filename:  GameState.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/12/2014 03:51:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "GameState.hpp"

GameState::GameState() {
	m_shader.loadFromFile("shaders/game.v.glsl", "shaders/game.f.glsl");
	
	Shader::bind(&m_shader);
	
	m_projectionMatrix = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, float(World::renderDistance * Chunk::width));
	m_viewMatrix = m_camera.update();
	
	m_shader.setUniform("u_tex", 0);
	
	Shader::bind(nullptr);
}

GameState::~GameState() {
}

void GameState::update() {
	m_viewMatrix = m_camera.processInputs();
}

void GameState::draw() {
	Shader::bind(&m_shader);
	
	m_world.draw(m_shader, m_projectionMatrix * m_viewMatrix);
	
	Shader::bind(nullptr);
}

