#pragma once
class Game
{

private:
	int m_round_Counter;
	//Board  board
	static Game* m_current_Instance;

private:
	Game() :m_round_Counter(0) {}
	~Game() { delete m_current_Instance; }
	

public:
	Game(const Game&) = delete;//cost de copiere
	Game operator=(const Game&) = delete;//egal

	static Game* get_Instance();//instanta jocului

};

