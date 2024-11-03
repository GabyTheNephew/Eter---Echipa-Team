#include "FireMageIgnara.h"

FireMageIgnara::FireMageIgnara(std::string_view name, std::string_view description):
	Mage::Mage({ "Ignara" }, { "Remove from play an opponent’s card that covers one of your cards." })
{
}

void FireMageIgnara::playMage() const
{
	//remove a card that is coverd
}
