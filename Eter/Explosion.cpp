#include "Explosion.h"



#include <random>
#include <set>
#include <ctime>

#include <random>
#include <set>
#include <ctime>

Explosion::Explosion()
{
	this->vectorInstantiation(3);
}

void Explosion::vectorInstantiation(int16_t size)
{
	
	std::mt19937 generator(static_cast<unsigned>(std::time(nullptr)));

	
	std::uniform_int_distribution<int> numActionsDist(2, 3);  
	std::uniform_int_distribution<int> positionDist(0, 2);    
	
	std::discrete_distribution<int> actionDist({ 10, 45, 45 });

	int16_t numActions = numActionsDist(generator);

	std::set<std::pair<int16_t, int16_t>> uniquePositions;

	while (positions.size() < static_cast<size_t>(numActions)) {
		int16_t x = positionDist(generator);
		int16_t y = positionDist(generator);

		ActionType action = static_cast<ActionType>(actionDist(generator));

		if (uniquePositions.insert({ x, y }).second) {
			positions.emplace_back(x, y, action);
		}
	}
}




void Explosion::rotationLeft(int16_t size)
{
	for (int16_t i = 0; i < positions.size(); i++)
	{
		int16_t save = std::get<0>(positions[i]);
		std::get<0>(positions[i]) = size - 1 - std::get<1>(positions[i]);
		std::get<1>(positions[i]) = save;
	}
}

void Explosion::rotationRight(int16_t size)
{
	for (int16_t i = 0; i < positions.size(); i++)
	{
		int16_t save = std::get<0>(positions[i]);
		std::get<0>(positions[i]) = std::get<1>(positions[i]);
		std::get<1>(positions[i]) = size - 1 - save;
	}
}

void Explosion::rotationDown(int16_t size)
{
	for (int16_t i = 0; i < positions.size(); i++)
	{
		std::get<0>(positions[i]) = size - 1 - std::get<0>(positions[i]);
		std::get<1>(positions[i]) = size - 1 - std::get<1>(positions[i]);
	}
}

const std::vector<std::tuple<int16_t, int16_t, ActionType>>& Explosion::getPositions() const
{
	return positions;
}

std::string ActionTypeToString(const ActionType& actionType)
{
	if (actionType == ActionType::explode)
		return "explode";
	if (actionType == ActionType::giveBack)
		return "giveBack";
	if (actionType == ActionType::hole)
		return "hole";
}

std::ostream& operator<<(std::ostream& os, const Explosion& explosion)
{
	os << "The explosion has these effects:\n";

	for (const auto& exp : explosion.getPositions())
	{
		os << std::get<0>(exp) << " " << std::get<1>(exp) << " " << ActionTypeToString(std::get<2>(exp)) << '\n';
	}

	return os;
}
