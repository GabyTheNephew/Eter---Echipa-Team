#pragma once
#include <string>

enum class Color {
	Red,
	usedRed,
	Blue,
	usedBlue,
	IlusionBlue,
	IlusionRed,
	increasedRed,
	increasedBlue,
	decreasedRed,
	decreasedBlue,
	Hole,
};

std::string ColorToString(const Color& color);
Color StringToColor(std::string_view color);