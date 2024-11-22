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

void Player::makeCardInvalid(uint8_t card_value,Color card_color)
{
	for (auto& card : m_simpleCardsVector)
	{
		if (card.getValue() == card_value && card.getColor() == card_color)
		{
			if (card_color == "red")
				card.setColor("usedRed");
			else
				if (card_color == "blue")
					card.setColor("usedBlue");
		}
	}
}

void Player::makeCardValid(uint8_t card_value, Color card_color)
{
	for (auto& card : m_simpleCardsVector)
	{
		if (card.getValue() == card_value && card.getColor() == "usedRed")
		{
			card.setColor("red");
		}
		else
			if (card.getValue() == card_value && card.getColor() == "usedBlue")
			{
				card.setColor("blue");
			}
	}

}


uint8_t Player::chooseCard()
{
	char chosen_card;
	std::cout <<getName() << " select a card\n";
	printSimpleCards();
	std::cout << "\nPick a card\n";
	std::cin >> chosen_card;
	
	return chosen_card;

}

int Player::numberofValidCards()
{
	int count = 0;
	for (auto& card : m_simpleCardsVector)
	{
		if (card.getColor() == "red" || card.getColor() == "blue")
			count++;
	}
	return count;
}

void Player::playCard(uint8_t card_value, Board& game_board,Color card_color)
{
	uint8_t x, y;
	std::cout << "Enter the coordinates of the card\n";
	while (true)
	{
		std::cin >> x >> y;

		if (game_board.canBePlaced(x,y))
		{ 
			
			makeCardInvalid(card_value);
			break;
		}
		else
		{
			std::cout << "Invalid coordinates\n";
		}

	}
	
	

}
