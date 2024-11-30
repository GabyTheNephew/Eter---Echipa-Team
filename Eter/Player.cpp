#include "Player.h"

Player::Player()
{
	m_name = "Player";
}

Player::Player(std::string_view name, std::vector <SimpleCard> simpleCards,std::vector <SimpleCard> simplePastCards):
	m_name{ name }, m_simpleCardsVector{ simpleCards }, m_pastSimpleCardsVector(simplePastCards)
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
	for (auto& card : m_simpleCardsVector)
	{
		if(card.getColor() == Color::Red || card.getColor() == Color::Blue)
				std::cout << card<<'\n';
	}
}

void Player::printPastSimpleCards()
{
	for (auto& card : m_pastSimpleCardsVector)
	{
		if (card.getColor() == Color::usedBlue || card.getColor() == Color::usedRed)
			std::cout << card << '\n';
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

const std::vector<SimpleCard>& Player::getPastVector()
{
	return m_pastSimpleCardsVector;
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

void Player::makeCardInvalid(SimpleCard card)
{
	for (auto& curCard : m_simpleCardsVector)
	{
		if (curCard.getValue() == card.getValue() && curCard.getColor() == card.getColor())
		{
			if (card.getColor() == Color::Red)
			{
				curCard.setColor(Color::usedRed);
				
				break;
			}
			else
				if (card.getColor() == Color::Blue)
				{
					curCard.setColor(Color::usedBlue);
					break;
		}
		}
	}
	
}

void Player::makeCardValid(SimpleCard& card)
{
	for (auto& curCard : m_simpleCardsVector)
	{
		if (curCard.getValue() == card.getValue() && curCard.getColor() == Color::usedRed)
		{
			curCard.setColor(Color::Red);
		}
		else
			if (curCard.getValue() == card.getValue() && curCard.getColor() == Color::usedBlue)
			{
				curCard.setColor(Color::Blue);
			}
	}

}

std::string Player::GetVectorColor()
{
		for (auto& elemnt : m_simpleCardsVector)
		{
			if (elemnt.getColor() == Color::Red)
			{
				return "Red";
			}
			else
				if (elemnt.getColor() == Color::Blue)
				{
					return "Blue";
				}
		}
		throw "All cards of the player are used";
	
}

void Player::deleteCardFromPastVectro(SimpleCard& card)
{
	for (auto& card : m_pastSimpleCardsVector)
	{
		if (card.getValue() == card.getValue() && card.getColor() == card.getColor())
		{
			m_pastSimpleCardsVector.erase(std::remove(m_pastSimpleCardsVector.begin(), m_pastSimpleCardsVector.end(), card), m_pastSimpleCardsVector.end());
			break;
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
		if (m_simpleCardsVector[i].getValue()+48 == chosen_card && (ColorToString(m_simpleCardsVector[i].getColor())=="Red" || ColorToString(m_simpleCardsVector[i].getColor())=="Blue"))
		{
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

void Player::playCard(SimpleCard& card, Board& game_board, std::vector<SimpleCard>& pastcards)
{
	uint8_t x, y;
	std::cout << "Enter the coordinates of the card\n";
	while (true)
	{
		std::cin >> x >> y;
		
		if (game_board.canBePlaced(x,y))
		{ 
			game_board.pushCard(card, { x,y });
			makeCardInvalid(card);
			pastcards.push_back(card);
			break;
		}
		else
		{
			std::cout << "Invalid coordinates\n";
		}

	}
}

void Player::playCardandExtend(SimpleCard& card, Board& game_board, std::vector<SimpleCard>& pastcards)
{
	Position pos;
	auto& [line, column] = pos;
	std::cout << "Enter the coordinates of the card\n";
	while (true)
	{
		std::cin >> line >> column;


		/*if (game_board.getSize() == 1)
		{
			game_board.pushCard(card, { 0, 0});
			makeCardInvalid(card);
			break;
		}*/
		
		if (game_board.canBePlaced(line, column))
		{
			initiateBoard(game_board, pos);
			game_board.pushCard(card, { line, column });
			makeCardInvalid(card);
			pastcards.push_back(card);
			break;
		}
		else
		{
			std::cout << "Invalid coordinates\n";
		}

	}
}

void Player::initiateBoard(Board& board, Position pos)
{
	auto& [line, column] = pos;

	uint8_t colSize = board.getColumnSize();
	uint8_t rowSize = board.getRowSize();

	if (line == -1)
	{
		board.expandRow(Board::RowExpandDirection::Up);

		if (column == -1)
		{
			board.expandColumn(Board::ColumnExpandDirection::Left);
		}
		else
		{
			if (column == colSize)
			{
				board.expandColumn(Board::ColumnExpandDirection::Right);
			}
		}
		return;
	}
	else
	{
		if (line == rowSize)
		{
			board.expandRow(Board::RowExpandDirection::Down);

			if (column == -1)
			{
				board.expandColumn(Board::ColumnExpandDirection::Left);
			}
			else
			{
				if (column == colSize)
				{
					board.expandColumn(Board::ColumnExpandDirection::Right);
				}
			}
			return;
		}
	}

	if (column == -1)
	{
		board.expandColumn(Board::ColumnExpandDirection::Left);

		if (line == -1)
		{
			board.expandRow(Board::RowExpandDirection::Up);
		}
		else
		{
			if (line == rowSize)
			{
				board.expandRow(Board::RowExpandDirection::Down);
			}
		}
	}
	else
	{
		if (column == colSize)
		{
			board.expandColumn(Board::ColumnExpandDirection::Right);

			if (line == -1)
			{
				board.expandRow(Board::RowExpandDirection::Up);
			}
			else
			{
				if (line == rowSize)
				{
					board.expandRow(Board::RowExpandDirection::Down);
				}
			}
		}
	}
}
