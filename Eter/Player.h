#pragma once
#include <string_view>
#include<vector>
#include<algorithm>
#include <random>
#include <ranges>
#include "SimpleCard.h"
#include "Mages.h"
#include "Board.h"
#include "AirMageVelora.h"
#include "AirMageZephyraCrow.h"
#include "EarthMageBumbleroot.h"
#include "EarthMageElderbranch.h"
#include "FireMageIgnara.h"
#include "FireMagePyrofang.h"
#include "WaterMageAqualon.h"
#include "WaterMageChillThoughts.h"
#include "Power.h"

class Player
{
private:
#pragma region Player
	std::string m_name;
	Mages m_mage;
	Power m_power;
	std::vector<SimpleCard> m_simpleCardsVector;
	std::vector<SimpleCard> m_pastSimpleCardsVector;
	std::vector<SimpleCard> m_restrictedCards;
	using Position = std::tuple<int16_t, int16_t>;

#pragma endregion

	

public:
#pragma region Player Methods
	Player();
	Player(std::string_view name, std::vector <SimpleCard> simpleCards, std::vector <SimpleCard> simplePastCards);
	Player(std::string_view name, std::vector <SimpleCard> simpleCards, std::vector <SimpleCard> simplepastCards, bool checkMage);
	~Player();
	void setName(std::string_view name);
	std::string_view getName();
#pragma endregion

#pragma region Mage/Power Methods
	void setMage(Mages mage);
	void setPower(Power power);
	int getMageAssignment();
	void reasignMage();
	int asignMage();
	void playMage(Mages mage, Board& game_board);
	std::string getMage();
	void assignPower();
	void reassignPower();
	Power getPower() const;
	Mages getMageEnum() const;
#pragma endregion

	
#pragma region Vector Methods
	void printSimpleCards();
	void printPastSimpleCards();
	void setVector(std::vector<SimpleCard>& simpleCardsVector);
	void setPastVector(std::vector<SimpleCard>& pastsimpleCardsVector);
	const std::vector<SimpleCard>& getVector();
	std::vector<SimpleCard>& getPastVector();
	void ResetVector();
	std::string GetVectorColor();
	void deleteCardFromPastVector(SimpleCard& cardToDelete);
	void makeCardInvalid(SimpleCard card);
	void makeCardValid(SimpleCard& card);

	void addRestrictedCard(const SimpleCard& card);
	void clearRestrictedCards();
	bool isCardRestricted(const SimpleCard& card) const;
#pragma endregion


#pragma region Extras Methods
	SimpleCard chooseCard();
	int numberofValidCards();

	void playCard(SimpleCard& card, Board& game_board, std::vector<SimpleCard>& m_pastSimpleCardsVecto, std::optional<std::pair<bool, bool>>& canPlayIllusion);

	void initiateBoard(Board& board, Position& pos);
#pragma endregion

	

};

