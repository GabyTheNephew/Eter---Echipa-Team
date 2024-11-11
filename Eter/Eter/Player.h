#pragma once
#include <string_view>
#include<vector>
#include "SimpleCard.h"
class Player
{
private:
	std::string_view m_name;
	std::vector<SimpleCard> m_simpleCardsVector;
	//vector for power
	//vector for mage

public:
	Player();
	Player(std::string_view name,std::vector <SimpleCard> simpleCards);
	~Player();

	void setName(std::string_view name);
	std::string_view getName();

	void printSimpleCards();

	void setVector(std::vector<SimpleCard>& simpleCardsVector);
	const std::vector<SimpleCard>& getVector();
	void ResetVector();

	void makeCardInvalid(uint8_t card_value);
	void makeCardValid(uint8_t card_value);

	void chooseCard();
};

