#include "CardColor.h"
std::string ColorToString(const Color& color)
{
	if (color == Color::Red)return "Red";
	if (color == Color::Blue)return "Blue";
	if (color == Color::usedRed)return "usedRed";
	if (color == Color::usedBlue)return "usedBlue";
	if (color == Color::IlusionBlue)return "IlusionBlue";
	if (color == Color::IlusionRed)return "IlusionRed";
	if (color == Color::increasedRed)return "increasedRed";
	if (color == Color::increasedBlue)return "increasedBlue";
	if (color == Color::decreasedRed)return "decreasedRed";
	if (color == Color::decreasedBlue)return "decreasedBlue";
	if (color == Color::Hole)return "Hole";
}

Color StringToColor(std::string_view color)
{
	if (color == "Red")return Color::Red;
	if (color == "Blue")return Color::Blue;
	if (color == "usedRed")return Color::usedRed;
	if (color == "usedBlue")return Color::usedBlue;
	if (color == "IlusionBlue")return Color::IlusionBlue;	
	if (color == "increasedRed")return Color::increasedRed;
	if (color == "increasedBlue")return Color::increasedBlue;
	if (color == "decreasedRed")return Color::decreasedRed;
	if (color == "decreasedBlue")return Color::decreasedBlue;
	if (color == "Hole")return Color::Hole;
}