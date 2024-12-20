#include "PowerAsh.h"


const std::string PowerAsh::m_name = "ASH";
const std::string PowerAsh::m_description = "Rise like a Phoenix! If one of your cards was removed from play, you can immediately play it again.";

PowerAsh::PowerAsh()
{
}


std::string_view PowerAsh::getName()const
{
	return m_name;
}

std::string_view PowerAsh::getDescription()const
{
	return m_description;
}

bool PowerAsh::checkAshPower(Player& player)
{
	int16_t cnt1 = 0;
	int16_t cnt2 = 0;
	std::string color = player.GetVectorColor();
	if (color == "Red")
		color = "usedRed";
	else
		color = "usedBlue";

	
	for (auto& card : player.getPastVector())
	{
		if (color == ColorToString(card.getColor()))
			cnt1++;
	}

	for (auto& card : player.getVector())
	{
		if (color == ColorToString(card.getColor()))
			cnt2++;
	}

	if (cnt1 < cnt2)
		return true;
	else
		return false;

}

void PowerAsh::playAshPower(Board& board,Player& player)
{
	
	std::string usedColor = (player.GetVectorColor() == "Red") ? "usedRed" : "usedBlue";

	if (checkAshPower(player) == false)
	{
		std::cout << "You can't use this power right now!\n";
		return;
	}

	// Find playable cards
	std::vector<SimpleCard> eligibleCards;
	for (const auto& card : player.getVector()) {
		if (ColorToString(card.getColor()) == usedColor) {
			bool inPast = false;
			for (const auto& pastCard : player.getPastVector()) {
				if (card.getValue() == pastCard.getValue() && ColorToString(card.getColor()) == ColorToString(pastCard.getColor())) {
					inPast = true;
					break;
				}
			}
			if (!inPast) {
				eligibleCards.push_back(card);
			}
		}
	}

	if (eligibleCards.empty()) {
		std::cout << "There are no cards to replay!\n";
		return;
	}

	SimpleCard chosenCard;
	if (eligibleCards.size() > 1) 
	{
		std::cout << "Select a card to replay:\n";
		for (size_t i = 0; i < eligibleCards.size(); ++i) {
			std::cout << i + 1 << ". "<< "Card Value: " << eligibleCards[i].getValue()<< ", Color: " << ColorToString(eligibleCards[i].getColor())<< '\n';
	}
	int choice;
		while (true) {
			std::cout << "Enter your choice (1-" << eligibleCards.size() << "): ";
			std::cin >> choice;
			if (choice > 0 && choice <= static_cast<int>(eligibleCards.size())) {
				chosenCard = eligibleCards[choice - 1];
				break;
			}
			else {
				std::cout << "Invalid choice. Try again.\n";
			}
		}
	}
	std::optional<std::pair<bool, bool>> canPlayIllusion = std::make_pair(false, false);
	std::vector<SimpleCard> PastCards = player.getPastVector();
	player.makeCardValid(chosenCard); 
	player.playCard(chosenCard, board, PastCards, canPlayIllusion);
}

