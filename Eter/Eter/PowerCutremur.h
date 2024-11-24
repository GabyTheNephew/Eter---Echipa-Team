#pragma once

#include <iostream>
#include "Board.h"
#include "Player.h"
#include <string_view>
class PowerCutremur
{
private:
	std::string m_name;
	std::string m_description;


public:
	PowerCutremur(std::string_view name, std::string_view description);

	std::string getName() const;
	std::string getDescription() const;

	void playPower(Board& board);
	bool checkPower();

};
