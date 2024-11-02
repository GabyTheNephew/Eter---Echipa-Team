#include "Game.h"

Game* Game::get_Instance()
{
	if (m_current_Instance == nullptr)
	{
		m_current_Instance = new Game();
	}

	return m_current_Instance;
}
