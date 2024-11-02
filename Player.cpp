#include "Player.h"

Player::Player()
{
	m_name = "Player";
}

Player::Player(std::string_view name):
	m_name{name}
{	
}

Player::~Player()
{
}

void Player::setName(std::string_view name)
{
	m_name = name;
}

std::string_view Player::getName()
{
	return m_name;
}
