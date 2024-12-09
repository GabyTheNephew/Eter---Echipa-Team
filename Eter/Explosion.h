#pragma once

#include <cstdlib>
#include <ctime>
#include <set>
#include <iostream>
#include <string_view>
#include <cstdint>
#include <vector>
#include <tuple>
//to do - 10 % chance for hole 
enum class ActionType {
	explode,
	giveBack,
	hole
};
class Explosion
{
private:

	std::vector<std::tuple<int, int, ActionType>> positions;
	static Explosion* m_current_Instance;

public:
	static Explosion* get_Instance();

	void vectorInstantiation(int16_t size);

	void rotationLeft(int16_t size);
	void rotationRight(int16_t size);
	void rotationDown(int16_t size);

	const std::vector<std::tuple<int, int, ActionType>>& getPositions() const;

};

