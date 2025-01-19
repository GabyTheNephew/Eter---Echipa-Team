#pragma once
#include <string_view>
#include<vector>
#include <random>
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
	std::string m_name;
	Mages m_mage;
	Power m_power;
	std::vector<SimpleCard> m_simpleCardsVector;
	std::vector<SimpleCard> m_pastSimpleCardsVector;
	

	using Position = std::tuple<int16_t, int16_t>;
	

public:
	Player();
	Player(std::string_view name, std::vector <SimpleCard> simpleCards, std::vector <SimpleCard> simplePastCards);
	Player(std::string_view name, std::vector <SimpleCard> simpleCards, std::vector <SimpleCard> simplepastCards, bool checkMage);
	~Player();

	void setName(std::string_view name);
	std::string_view getName();
	int getMageAssignment();
	void printSimpleCards();
	void printPastSimpleCards();
	void reasignMage();
	void setVector(std::vector<SimpleCard>& simpleCardsVector);
	void setPastVector(std::vector<SimpleCard>& pastsimpleCardsVector);
	const std::vector<SimpleCard>& getVector();
	std::vector<SimpleCard>& getPastVector(); 

	void ResetVector();

	void makeCardInvalid(SimpleCard card);
	void makeCardValid(SimpleCard& card);
	std::string GetVectorColor();
	void deleteCardFromPastVector(SimpleCard& cardToDelete);
	int asignMage();

	SimpleCard chooseCard();
	int numberofValidCards();
	
	void playCard(SimpleCard& card, Board& game_board, std::vector<SimpleCard>& m_pastSimpleCardsVecto, std::optional<std::pair<bool, bool>>& canPlayIllusion);
	void playMage(Mages mage, Board& game_board);
	std::string getMage();
	void initiateBoard(Board& board, Position& pos);
	void assignPower();
	void reassignPower();
	Power getPower() const;

};

