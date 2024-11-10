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

}

void Explosion::rotationLeft(uint8_t size)
{
	for (int i = 0; i < positions.size(); i++)
	{
		uint8_t save = positions[i].first;
		positions[i].first = size - 1 - positions[i].second;
		positions[i].second =save;
	}
}

void Explosion::rotationRight(uint8_t size)
{
	for (int i = 0; i < positions.size(); i++)
	{
		uint8_t save = positions[i].first;
		positions[i].first = positions[i].second;
		positions[i].second = size - 1 - save;
	}
}

void Explosion::rotationDown(uint8_t size)
{
	for (int i = 0; i < positions.size(); i++)
	{
		positions[i].first = size - 1 - positions[i].first;
		positions[i].second = size - 1 - positions[i].second;
	}
}


