#include "CardColor.h"
std::string ColorToString(const Color& color)
{
	return enumToString(color);
}

Color StringToColor(std::string_view color)
{
	return stringToEnum<Color>(std::string(color));
}