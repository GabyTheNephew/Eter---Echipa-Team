#pragma once
#include <string>

enum class Color {
	Red,
	usedRed,
	Blue,
	usedBlue
};

std::string ColorToString(const Color& color);
Color StringToColor(std::string_view color);