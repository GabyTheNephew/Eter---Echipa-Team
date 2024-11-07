#include "Player.h"

Player::Player()
{
	m_name = "Player";
}

Player::Player(std::string_view name, std::vector <SimpleCard> simpleCards):
	m_name{name},m_simpleCardsVector{simpleCards}
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

void Player::printCards()
{
	for (auto card : m_simpleCardsVector)
	{
		std::cout << card;
	}
}

void Player::setVector(std::vector<SimpleCard> simpleCardsVector)
{
	m_simpleCardsVector = simpleCardsVector;
}

std::vector<SimpleCard> Player::getVector()
{
	return m_simpleCardsVector;
}
