#include "Player.h"

Player::Player()
{
	m_name = "Player";
}

Player::Player(std::string_view name, std::vector <SimpleCard> simpleCards, std::vector <SimpleCard> simplePastCards) :
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
		if (card.getColor() == Color::Red || card.getColor() == Color::Blue)
			std::cout << card << '\n';
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

void Player::setPastVector(std::vector<SimpleCard>& pastsimpleCardsVector)
{
	m_pastSimpleCardsVector = pastsimpleCardsVector;
}


const std::vector<SimpleCard>& Player::getVector()
{
	return m_simpleCardsVector;
}

std::vector<SimpleCard>& Player::getPastVector()
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
			if (card.getColor() == Color::Red || card.getColor() == Color::IlusionRed)
			{
				curCard.setColor(Color::usedRed);

				break;
			}
			else
				if (card.getColor() == Color::Blue || card.getColor() == Color::IlusionBlue)
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
			for (auto& pastCard : m_pastSimpleCardsVector)
			{
				if (pastCard.getValue() == card.getValue() && pastCard.getColor() == Color::usedRed)
				{
					curCard.setColor(Color::Red);
					m_pastSimpleCardsVector.erase(
						std::remove_if(m_pastSimpleCardsVector.begin(),
							m_pastSimpleCardsVector.end(), 
							[&pastCard](const auto& element)
							{
								return element.getValue() == pastCard.getValue();
							}),
						m_pastSimpleCardsVector.end());
					break;
				}
			}
		}
		else
			if (curCard.getValue() == card.getValue() && curCard.getColor() == Color::usedBlue)
			{
				for (auto& pastCard : m_pastSimpleCardsVector)
				{
					if (pastCard.getValue() == card.getValue() && pastCard.getColor() == Color::usedBlue)
					{
						curCard.setColor(Color::Blue);
						m_pastSimpleCardsVector.erase(
							std::remove_if(m_pastSimpleCardsVector.begin(),
								m_pastSimpleCardsVector.end(),

								[&pastCard](const auto& element)
								{
									return element.getValue() == pastCard.getValue();
								}),

							m_pastSimpleCardsVector.end());
						break;
					}
				}
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

void Player::deleteCardFromPastVector(SimpleCard& cardToDelete)
{
	for (auto& card : m_pastSimpleCardsVector)
	{
		if (card.getValue() == cardToDelete.getValue() && card.getColor() == cardToDelete.getColor())
		{
			m_pastSimpleCardsVector.erase(
				std::remove_if(m_pastSimpleCardsVector.begin(),
				m_pastSimpleCardsVector.end(), 

				[&card](const auto& element) 
				{
					return element.getValue() == card.getValue() && element.getColor()==card.getColor();
				}), 

				m_pastSimpleCardsVector.end());
			break;
		}
	}
}


SimpleCard Player::chooseCard()
{
	int16_t chosen_card;
	//std::cout << getName() << " select a card\n";
	printSimpleCards();
	//std::cout << "\nPick a card\n";
	std::cin >> chosen_card;

	for (int16_t i = 0; i < m_simpleCardsVector.size(); i++)
	{
		if (m_simpleCardsVector[i].getValue() == chosen_card && (ColorToString(m_simpleCardsVector[i].getColor()) == "Red" || ColorToString(m_simpleCardsVector[i].getColor()) == "Blue"))
		{
			return m_simpleCardsVector[i];
		}
	}

}

int Player::numberofValidCards()
{
	int16_t count = 0;
	for (auto& card : m_simpleCardsVector)
	{
		if (card.getColor() == Color::Red || card.getColor() == Color::Blue)
			count++;
	}
	return count;
}

void Player::playCard(SimpleCard& card, Board& game_board,std::vector<SimpleCard>& pastcards, std::optional<std::pair<bool, bool>>& canPlayIllusion)
{
	Position pos;

	auto& [x, y] = pos;

	if (canPlayIllusion.has_value())
	{
		std::cout << "Do you want to play it as an illusion?\n";
		std::string answer;
		std::cin >> answer;
		if (answer == "yes")
		{
			if (card.getColor() == Color::Red)
			{
				if (canPlayIllusion->first == true)
				{
					card.setColor(Color::IlusionRed);
					canPlayIllusion->first = false;
				}
				else
					std::cout << "This player already played an illusion this round!\n";
			}
			else
			{
				if (canPlayIllusion->second == true)
				{
					card.setColor(Color::IlusionBlue);
					canPlayIllusion->second = false;
				}
				else
					std::cout << "This player already played an illusion this round!\n";
			}
			if (canPlayIllusion->first == false && canPlayIllusion->second == false)
			{
				canPlayIllusion = std::nullopt;
			}
		}
	}

	std::cout << "Enter the coordinates of the card\n";
	while (true)
	{
		std::cin >> x >> y;


		/*if (game_board.getSize() == 1)
		{
			game_board.pushCard(card, { 0, 0});
			makeCardInvalid(card);
			break;
		}*/

		if (game_board.canBePlaced(x, y))
		{
			if (game_board.getSize() < 3)
			{
				initiateBoard(game_board, pos);
			}

			if (card.getColor() == Color::IlusionBlue || card.getColor() == Color::IlusionRed)
			{
				if (game_board[pos].empty())
				{
					game_board.pushCard(card, { x,y });
					makeCardInvalid(card);
					pastcards.push_back(card);
					break;
				}
				else
				{
					std::cout << "You can only play an illusion on an empty space on the board!\n";
					continue;
				}
			}
			else if (!game_board[pos].empty() && game_board[pos].back().getColor() == Color::IlusionBlue)
			{
				if (card.getColor() == Color::Red)
				{
					if (!game_board.canBePushed(card, pos))
					{
						makeCardInvalid(card);
						std::cout << "You tried to cover an illusion with a lower value card!\n";
						game_board[pos].back().setColor(Color::Blue);
						break;
					}
					else
					{
						game_board.pushCard(card, pos);
						pastcards.push_back(card);
						break;
					}
				}
			}
			else if (!game_board[pos].empty() && game_board[pos].back().getColor() == Color::IlusionRed)
			{
				if (card.getColor() == Color::Blue)
				{
					if (!game_board.canBePushed(card, pos))
					{
						makeCardInvalid(card);
						std::cout << "You tried to cover an illusion with a lower value card!\n";
						game_board[pos].back().setColor(Color::Red);
						break;
					}
					else
					{
						game_board.pushCard(card, pos);
						pastcards.push_back(card);
						break;
					}
				}
			}
			else
			{
				game_board.pushCard(card, { x,y });
				makeCardInvalid(card);
				pastcards.push_back(card);
				break;
			}
		}
		else
		{
			std::cout << "Invalid coordinates\n";
		}
	}
}

//void Player::playCardandExtend(SimpleCard& card, Board& game_board,
//	std::vector<SimpleCard>& pastcards, bool& canPlayIllusion)
//{
//	Position pos;
//	auto& [line, column] = pos;
//
//	if (canPlayIllusion)
//	{
//		std::cout << "Do you want to play it as an illusion?\n";
//		std::string answer;
//		std::cin >> answer;
//		if (answer == "yes")
//		{
//			canPlayIllusion = false;
//			if (card.getColor() == Color::Red)
//			{
//				card.setColor(Color::IlusionRed);
//			}
//			else
//			{
//				card.setColor(Color::IlusionBlue);
//			}
//		}
//	}
//
//	std::cout << "Enter the coordinates of the card\n";
//	while (true)
//	{
//		std::cin >> line >> column;
//
//
//		/*if (game_board.getSize() == 1)
//		{
//			game_board.pushCard(card, { 0, 0});
//			makeCardInvalid(card);
//			break;
//		}*/
//
//		if (game_board.canBePlaced(line, column))
//		{
//			initiateBoard(game_board, pos);
//			game_board.pushCard(card, { line, column });
//			makeCardInvalid(card);
//			pastcards.push_back(card);
//			break;
//		}
//		else
//		{
//			std::cout << "Invalid coordinates\n";
//		}
//
//	}
//}

void Player::initiateBoard(Board& board, Position& pos)
{
	auto& [line, column] = pos;

	int16_t colSize = board.getColumnSize();
	int16_t rowSize = board.getRowSize();

	if (line == -1)
	{
		board.expandRow(Board::RowExpandDirection::Up);

		if (column == -1)
		{
			board.expandColumn(Board::ColumnExpandDirection::Left);
			column = 0;
		}
		else
		{
			if (column == colSize)
			{
				board.expandColumn(Board::ColumnExpandDirection::Right);
			}
		}

		line = 0;
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
				column = 0;
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
		column = 0;

		if (line == -1)
		{
			board.expandRow(Board::RowExpandDirection::Up);
			line = 0;
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
				line = 0;
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

//void Player::playIllusion(SimpleCard& card, Board& board, Position pos)
//{
//	if (card.getIllusionState())
//	{
//
//	}
//}

bool Player::hasSelectedCard()  {
	return selectedCard.getValue() != 0; // Returnează true dacă o carte este selectată
}

SimpleCard Player::getSelectedCard()  {
	return selectedCard;
}

void Player::setSelectedCard(const SimpleCard& card) {
	selectedCard = card;
}