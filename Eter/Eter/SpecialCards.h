#pragma once
#include <iostream>
#include <string_view>

class SpecialCards
{
protected:

	std::string_view m_name;
	std::string_view m_description;
	bool m_isPlayable;

public:
	SpecialCards(std::string_view name, std::string_view description);

	void setName(std::string_view name);
	void setDescription(std::string_view description);
	void setIsPlayable(bool isPlayable);
	std::string_view getName() const;
	std::string_view getDescription() const;
	bool getIsPlayable() const;
};

