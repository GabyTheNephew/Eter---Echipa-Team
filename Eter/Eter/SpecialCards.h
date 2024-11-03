#pragma once
#include <iostream>
#include <string_view>

class SpecialCards
{
private:

	std::string_view m_name;

public:

	virtual std::string_view getName() = 0;
	virtual std::string_view Description() = 0;
	virtual bool isPlayable() = 0;

};

