#pragma once
#include"Player.h"
#include<iostream>
#include<string>

class PowerBorder
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerBorder();

	std::string_view getName()const;
	std::string_view getDescription()const;
	bool checkBorderPower(Board& board, int16_t borderX, int16_t borderY);
	bool playBorderPower(Board& board, int16_t borderX, int16_t borderY, int16_t cardX, int16_t cardY, const SimpleCard& cardToPlace);

};

