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
		if(card.getColor() == Color::Red || card.getColor() == Color::Blue)
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
		if (card.getColor() == Color::usedRed)
			card.setColor(Color::Red);
		else
			if (card.getColor() == Color::usedBlue)
				card.setColor(Color::Blue);
	}
}

void Player::makeCardInvalid(SimpleCard& card)
{
	for (auto& curCard : m_simpleCardsVector)
	{
		if (curCard.getValue() == card.getValue()  && curCard.getColor() == card.getColor())
		{
			if (card.getColor() == Color::Red)
				card.setColor(Color::usedRed);
			else
				if (card.getColor() == Color::Blue)
					card.setColor(Color::usedBlue);
		}
	}
}

void Player::makeCardValid(SimpleCard& card)
{
	for (auto& curCard : m_simpleCardsVector)
	{
		if (curCard.getValue() == card.getValue() && curCard.getColor() == Color::usedRed)
		{
			card.setColor(Color::Red);
		}
		else
			if (curCard.getValue() == card.getValue() && curCard.getColor() == Color::usedBlue)
			{
				card.setColor(Color::Blue);
			}
	}

}


SimpleCard Player::chooseCard()
{
	uint8_t chosen_card;
	std::cout <<getName() << " select a card\n";
	printSimpleCards();
	std::cout << "\nPick a card\n";
	std::cin >> chosen_card;
	
	for (int8_t i = 0; i < m_simpleCardsVector.size(); i++)
	{
		if (m_simpleCardsVector[i].getValue() == chosen_card)
		{
			makeCardInvalid(m_simpleCardsVector[i]);
			return m_simpleCardsVector[i];
		}
	}

}

int Player::numberofValidCards()
{
	int count = 0;
	for (auto& card : m_simpleCardsVector)
	{
		if (card.getColor() == Color::Red || card.getColor() == Color::Blue)
			count++;
	}
	return count;
}

void Player::playCard(SimpleCard& card, Board& game_board)
{
	uint8_t x, y;
	std::cout << "Enter the coordinates of the card\n";
	while (true)
	{
		std::cin >> x >> y;

		if (game_board.canBePlaced(x,y))
		{ 
			
			makeCardInvalid(card);
			break;
		}
		else
		{
			std::cout << "Invalid coordinates\n";
		}

	}
}

void Player::initiateBoard(Board& board, int x, int y)
{
	board.resizeBoard(1);

	std::cout << "Coordinates: \n";

	std::tuple<uint8_t, uint8_t> position = std::make_tuple(x, y);

	auto chosenCard = chooseCard();
	auto& [line, column] = position;

	if (board.canBePlaced(x, y));
	{
		switch (x)
		{
		case -1:
			if (y == 0)
			{
				board.expandLeft();
			}
			else
			{
				if (y == 1)
				{
					board.expandLeftUpCorner();
				}
				else
				{
					if (y == -1)
					{
						board.expandLeftBottomCorner();
					}
					else
					{
						std::cout << "Y nu este valid!\n";
						break;
					}
				}
			}
			break;
		case 1:
			if (y == 0)
			{
				board.expandRight();
			}
			else
			{
				if (y == 1)
				{
					board.expandRightUpCorner();
				}
				else
				{
					if (y == -1)
					{
						board.expandRightBottomCorner();
					}
					else
					{
						std::cout << "Y nu este valid!\n";
						break;
					}
				}
			}
			break;
		case 0:
			if (y == 0)
			{
				break;
			}
			else
			{
				if (y == 1)
				{
					board.expandRightUpCorner();
				}
				else
				{
					if (y == -1)
					{
						board.expandRightBottomCorner();
					}
					else
					{
						std::cout << "Y nu este valid!\n";
						break;
					}
				}
			}
			break;
		}
	}
}
