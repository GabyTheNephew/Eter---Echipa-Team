#pragma once

#include <cstdlib>
#include <ctime>
#include <set>
#include <iostream>
#include <string_view>
#include <cstdint>
#include <vector>
#include <tuple>
#include "EnumConversion.h"
enum class ActionType {
	explode,
	giveBack,
	hole
};

std::string ActionTypeToString(const ActionType& actionType);

class Explosion
{


public:
	std::vector<std::tuple<int16_t, int16_t, ActionType>> positions;

	Explosion();
	Explosion(bool empty);

	void vectorInstantiation(int16_t size);

	void rotationLeft(int16_t size);
	void rotationRight(int16_t size);
	void rotationDown(int16_t size);

	const std::vector<std::tuple<int16_t, int16_t, ActionType>>& getPositions() const;

	friend std::ostream& operator<<(std::ostream& os, const Explosion& explosion);

};

