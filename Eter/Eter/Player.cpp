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

void Player::printSimpleCards()
{
	for (auto card : m_simpleCardsVector)
	{
		if(card.getColor() == "red" || card.getColor() == "blue")
				std::cout << card<<'\n';
	}
}

void Player::setVector(std::vector<SimpleCard>& simpleCardsVector)
{
	m_simpleCardsVector = simpleCardsVector;
}

const std::vector<SimpleCard>& Player::getVector()
{
	return m_simpleCardsVector;
}

void Player::ResetVector()
{
	for (auto& card : m_simpleCardsVector)
	{
		if (card.getColor() == "usedRed")
			card.setColor("red");
		else
			if (card.getColor() == "usedBlue")
				card.setColor("blue");
	}
}

void Player::makeCardInvalid(uint8_t card_value)
{
	for (auto& card : m_simpleCardsVector)
	{
		if (card.getValue() == card_value)
		{
			if (card.getColor() == "red")
			{
				card.setColor("usedRed");
				break;
			}
			else
				if (card.getColor() == "blue")
				{
					card.setColor("usedBlue");
					break;
				}
		}
	}
}

void Player::makeCardValid(uint8_t card_value)
{
	for (auto& card : m_simpleCardsVector)
	{
		if (card.getValue() == card_value)
		{
			if (card.getColor() == "usedRed")
			{
				card.setColor("red");
				break;
			}
			else
				if (card.getColor() == "usedBlue")
				{
					card.setColor("blue");
					break;
				}
		}
	}

}

void Player::chooseCard()
{
	uint8_t chosen_card;
	std::cout <<getName() << " select a card";
	printSimpleCards();
	std::cout << "\nPick a card\n";
	std::cin >> chosen_card;
	makeCardInvalid(chosen_card);

}
