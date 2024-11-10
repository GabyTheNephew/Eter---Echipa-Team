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

void Explosion::vectorInstantiation(uint8_t size)
{
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	int numActions = 2 + (std::rand() % 2);

	std::set<std::pair<int, int>> uniquePositions;

	while (positions.size() < static_cast<size_t>(numActions)) {
		int x = std::rand() % 3;
		int y = std::rand() % 3;

		// Verificăm dacă poziția este unică
		if (uniquePositions.emplace(x, y).second) { // `.second` este true dacă perechea a fost inserată cu succes
			ActionType action = static_cast<ActionType>(std::rand() % 3);
			positions.emplace_back(x, y, action);
		}
	}

	for (const auto& entry : positions) {
		int x, y;
		ActionType action;
		std::tie(x, y, action) = entry;

		std::cout << "Position: (" << x << ", " << y << "), ActionType: ";
		switch (action) {
		case ActionType::explode: std::cout << "explode"; break;
		case ActionType::giveBack: std::cout << "giveBack"; break;
		case ActionType::hole: std::cout << "hole"; break;
		}
		std::cout << std::endl;
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


