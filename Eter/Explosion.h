#pragma once

#include <cstdlib>
#include <ctime>
#include <set>
#include <iostream>
#include <string_view>
#include <cstdint>
#include <vector>
#include <tuple>

enum class ActionType {
	explode,
	giveBack,
	hole
};

std::string ActionTypeToString(const ActionType& actionType);

class Explosion
{
private:
	std::vector<std::tuple<int16_t, int16_t, ActionType>> positions;


public:


	Explosion();

	void vectorInstantiation(int16_t size);

	void rotationLeft(int16_t size);
	void rotationRight(int16_t size);
	void rotationDown(int16_t size);

	const std::vector<std::tuple<int16_t, int16_t, ActionType>>& getPositions() const;

	friend std::ostream& operator<<(std::ostream& os, const Explosion& explosion);

};

