#include "Player.h"
#include <iostream>

Player::Player()
{
	if (!m_texture.loadFromFile("gfx/dino1.png"))
		std::cerr << "No dino texture. sad";

	setTexture(&m_texture);
	setSize({ 36,36 });
	setCollisionBox({ {6,6}, { 24,25 } });
	setPosition({ 50,0 });
	m_isOnGround = false;
	setTextureRect({ { 0,0 }, { 24,24 } });
}

void Player::handleInput(float dt)
{
	m_acceleration = { 0,0 };

	if (m_input->isKeyDown(sf::Keyboard::Scancode::A))
		m_acceleration.x -= SPEED;
	if (m_input->isKeyDown(sf::Keyboard::Scancode::D))
		m_acceleration.x += SPEED;

	if (m_input->isKeyDown(sf::Keyboard::Scancode::R))	// Reset (for debugging)
	{
		setPosition({ 50,0 });
		m_velocity = { 0,0 };
	}
	
	if (m_input->isKeyDown(sf::Keyboard::Scancode::Space) && m_isOnGround)	
	{
		m_velocity.y = -JUMP_FORCE;
		move(m_velocity);
		m_isOnGround = false;
	}
}

void Player::update(float dt)
{
	
	// newtonian model
	
	m_velocity = dt * m_acceleration;
	if (!m_isOnGround) {
		m_velocity.y += GRAVITY * dt;
	}
	m_oldPosition = getPosition();
	move(m_velocity);
	
	m_isOnGround = false;
		
}

void Player::collisionResponse(GameObject& collider)
{
	
	auto overlap = getCollisionBox().findIntersection(collider.getCollisionBox());
	if (!overlap) return;

	float oldBottom = m_oldPosition.y + getCollisionBox().size.y;
	float tileTop = collider.getCollisionBox().position.y;

	if (oldBottom <= tileTop) {
		if (m_velocity.y > 0) {
				m_velocity.y = 0;
				setPosition({ getPosition().x, getPosition().y - overlap->size.y });
				m_isOnGround = true;							
		}			
	}
	else
	{
		m_velocity.x *= -COEFF_RESTITUTION;

		if (getCollisionBox().position.x < collider.getCollisionBox().position.x)
			setPosition({ getPosition().x - overlap->size.x, getPosition().y });
		else
			setPosition({ getPosition().x + overlap->size.x, getPosition().y });
	}
		
	
}
