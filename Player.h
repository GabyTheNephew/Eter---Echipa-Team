#pragma once
#include <string_view>
class Player
{
private:
	std::string_view m_name;
	//vector for simpleCard
	//vector for power
	//vector for mage

public:
	Player();
	Player(std::string_view name);
	~Player();

	void setName(std::string_view name);
	std::string_view getName();
};

