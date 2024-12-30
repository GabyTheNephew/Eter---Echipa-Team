#pragma once
#include <string_view>
#include<vector>
#include "SimpleCard.h"
#include "Board.h"
class Player
{
private:
	std::string m_name;
	std::vector<SimpleCard> m_simpleCardsVector;
	std::vector<SimpleCard> m_pastSimpleCardsVector;
	//vector for power
	//vector for mage

	using Position = std::tuple<int16_t, int16_t>;
	// TODO: check int8_t, why is it converted to ascii/ char?

public:
	Player();
	Player(std::string_view name,std::vector <SimpleCard> simpleCards,std::vector <SimpleCard> simplepastCards);
	~Player();

	void setName(std::string_view name);
	std::string_view getName();

	void printSimpleCards();
	void printPastSimpleCards();

	void setVector(std::vector<SimpleCard>& simpleCardsVector);
	void setPastVector(std::vector<SimpleCard>& pastsimpleCardsVector);
	const std::vector<SimpleCard>& getVector();
	std::vector<SimpleCard>& getPastVector(); /// we need to use push back on it

	void ResetVector();

	void makeCardInvalid(SimpleCard card);
	void makeCardValid(SimpleCard& card);
	std::string GetVectorColor();
	void deleteCardFromPastVector(SimpleCard& cardToDelete);

	SimpleCard chooseCard();
	int numberofValidCards();
	//void playCard(int16_t card_value,Board& game_board,Color card_color);
	void playCard(SimpleCard& card, Board& game_board,std::vector<SimpleCard>& m_pastSimpleCardsVecto, std::optional<std::pair<bool, bool>>& canPlayIllusion);
	//void playCardandExtend(SimpleCard& card, Board& game_board, std::vector<SimpleCard>& m_pastSimpleCardsVecto, bool& canPlayIllusion);
	void initiateBoard(Board& board, Position& pos);
	//void playIllusion(SimpleCard& card, Board& board, Position pos);
};

