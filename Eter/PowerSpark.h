#pragma once
#include"Player.h"
#include<iostream>

class PowerSpark
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerSpark();

	std::string_view getName()const;
	std::string_view getDescription()const;
	bool checkSparkPower(Board& board, Color playerColor);
	bool playSparkPower(Board& board, Color playerColor, int16_t sourceX, int16_t sourceY, int16_t cardIndex, int16_t destX, int16_t destY);
};


