#include "CardColor.h"
std::string ColorToString(const Color& color)
{
	if (color == Color::Red)return "Red";
	if (color == Color::Blue)return "Blue";
	if (color == Color::usedRed)return "usedRed";
	if (color == Color::usedBlue)return "usedBlue";
	if (color == Color::IlusionBlue)return "IlusionBlue";
	if (color == Color::IlusionRed)return "IlusionRed";
}

Color StringToColor(std::string_view color)
{
	if (color == "Red")return Color::Red;
	if (color == "Blue")return Color::Blue;
	if (color == "usedRed")return Color::usedRed;
	if (color == "usedBlue")return Color::usedBlue;
	if (color == "IlusionBlue")return Color::IlusionBlue;
}