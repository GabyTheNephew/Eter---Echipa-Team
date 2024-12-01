#include "Explosion.h"

Explosion* Explosion::m_current_Instance = nullptr;



Explosion* Explosion::get_Instance()
{
	if (m_current_Instance == nullptr)
	{
		m_current_Instance = new Explosion();
	}

	return m_current_Instance;
}

#include <random>
#include <set>
#include <ctime>

#include <random>
#include <set>
#include <ctime>

void Explosion::vectorInstantiation(uint8_t size)
{
	
	std::mt19937 generator(static_cast<unsigned>(std::time(nullptr)));

	
	std::uniform_int_distribution<int> numActionsDist(2, 3);  
	std::uniform_int_distribution<int> positionDist(0, 2);    
	
	std::discrete_distribution<int> actionDist({ 10, 45, 45 });

	int numActions = numActionsDist(generator);

	std::set<std::pair<int, int>> uniquePositions;

	while (positions.size() < static_cast<size_t>(numActions)) {
		int x = positionDist(generator);
		int y = positionDist(generator);

		ActionType action = static_cast<ActionType>(actionDist(generator));

		if (uniquePositions.insert({ x, y }).second) {
			positions.emplace_back(x, y, action);
		}
	}
}




void Explosion::rotationLeft(uint8_t size)
{
	for (int i = 0; i < positions.size(); i++)
	{
		uint8_t save = std::get<0>(positions[i]);
		std::get<0>(positions[i]) = size - 1 - std::get<1>(positions[i]);
		std::get<1>(positions[i]) = save;
	}
}

void Explosion::rotationRight(uint8_t size)
{
	for (int i = 0; i < positions.size(); i++)
	{
		uint8_t save = std::get<0>(positions[i]);
		std::get<0>(positions[i]) = std::get<1>(positions[i]);
		std::get<1>(positions[i]) = size - 1 - save;
	}
}

void Explosion::rotationDown(uint8_t size)
{
	for (int i = 0; i < positions.size(); i++)
	{
		std::get<0>(positions[i]) = size - 1 - std::get<0>(positions[i]);
		std::get<1>(positions[i]) = size - 1 - std::get<1>(positions[i]);
	}
}

const std::vector<std::tuple<int, int, ActionType>>& Explosion::getPositions() const
{
	return positions;
}


