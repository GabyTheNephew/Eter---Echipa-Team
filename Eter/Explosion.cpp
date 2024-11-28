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

	ActionType action;

	while (positions.size() < static_cast<size_t>(numActions)) {
		int x = std::rand() % 3;
		int y = std::rand() % 3;

		// Verificăm dacă poziția este unică
		int chance = std::rand() % 10;
		if (chance == 0) {
			action = ActionType::hole;
		}
		else {
			// 90% șansă pentru celelalte două tipuri de acțiuni
			action = (std::rand() % 2 == 0) ? ActionType::explode : ActionType::giveBack;
		}

		positions.emplace_back(x, y, action);
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


