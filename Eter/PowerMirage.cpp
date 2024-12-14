#include "PowerMirage.h"



const std::string PowerMirage::m_name = "Mirage";
const std::string PowerMirage::m_description = " Exchange your Illusion (face-down card) with a different card from your hand (face-down).";

PowerMirage::PowerMirage()
{
}

std::string PowerMirage::getName() const
{
	return m_name;
}

std::string PowerMirage::getDescription() const
{
	return m_description;
}

void PowerMirage::playCrumblePower(Board& board,Player player, int16_t x, int16_t y)
{
	if (player.GetVectorColor() == "Red") 
	{
		SimpleCard card = player.chooseCard();
		card.setColor(Color::IlusionRed);
		
		player.makeCardValid(board[{x, y}].back());
		player.getPastVector().erase(std::find(player.getPastVector().begin(), player.getPastVector().end(),board[{x, y}].back()));
		player.getPastVector().push_back(card);
		board[{x, y}].pop_back();
		board[{x, y}].push_back(card);
		

	}
	if (player.GetVectorColor() == "Blue") 
	{
		SimpleCard card = player.chooseCard();
		card.setColor(Color::IlusionBlue);
		
		player.makeCardValid(board[{x, y}].back());
		player.getPastVector().erase(std::find(player.getPastVector().begin(), player.getPastVector().end(), board[{x, y}].back()));
		player.getPastVector().push_back(card);
		board[{x, y}].pop_back();
		board[{x, y}].push_back(card);
	}
	
	
}

bool PowerMirage::checkCrumblePower(Board& board,Player& player, int16_t x, int16_t y)
{
	if (player.GetVectorColor() == "Red")
	{
		for(int i=0;i<board.getSize();i++)
			for (int j = 0; j < board.getSize(); j++)
			{
				if (board[{i, j}].back().getColor() == Color::IlusionRed)
				{
					return true;
				}
			}
	}
	if (player.GetVectorColor() == "Blue")
	{
		for (int i = 0; i < board.getSize(); i++)
			for (int j = 0; j < board.getSize(); j++)
			{
				if (board[{i, j}].back().getColor() == Color::IlusionBlue)
				{
					return true;
				}
			}
	}
	return false;
}
