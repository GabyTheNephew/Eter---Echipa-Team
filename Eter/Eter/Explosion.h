#pragma once
#include "SpecialCards.h"
#include <iostream>
#include <string_view>
#include <cstdint>
#include <vector>
#include <pair>
//to do - make this class single tone , and default / params constructor
enum Class ActionType {
	explode,
	giveBack,
	hole
};
class Explosion
{
private:

	std::vector<std::pair<uint8_t, uint8_t>> positions;
	static Explosion* m_current_Instance;

public:
	static Explosion* get_Instance();

	void vectorInstantiation(uint8_t size);
	
	void rotationLeft(uint8_t size);
	void rotationRight(uint8_t size);
	void rotationDown(uint8_t size);


	

};

