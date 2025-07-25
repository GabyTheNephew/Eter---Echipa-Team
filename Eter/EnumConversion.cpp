#include "EnumConversion.h"
#include "CardColor.h"
#include "Mages.h"
#include "Power.h"
#include "Explosion.h"
#include "GameTypes.h"


template<>
std::string enumToString<Color>(Color color) {
    switch (color) {
    case Color::Red:
        return "Red";
    case Color::Blue:
        return "Blue";
    case Color::usedRed:
        return "usedRed";
    case Color::usedBlue:
        return "usedBlue";
    case Color::IlusionBlue:
        return "IlusionBlue";
    case Color::IlusionRed:
        return "IlusionRed";
    case Color::increasedRed:
        return "increasedRed";
    case Color::increasedBlue:
        return "increasedBlue";
    case Color::decreasedRed:
        return "decreasedRed";
    case Color::decreasedBlue:
        return "decreasedBlue";
    case Color::Hole: 
        return "Hole";
	case Color::Border:
		return "Border";
    default: throw std::invalid_argument("Unknown Color enum value");
    }
}


template<>
Color stringToEnum<Color>(const std::string& str) {
    if (str == "Red")
        return Color::Red;
    if (str == "Blue")
        return Color::Blue;
    if (str == "usedRed")
        return Color::usedRed;
    if (str == "usedBlue")
        return Color::usedBlue;
    if (str == "IlusionBlue")
        return Color::IlusionBlue;
    if (str == "IlusionRed")
        return Color::IlusionRed;
    if (str == "increasedRed")
        return Color::increasedRed;
    if (str == "increasedBlue")
        return Color::increasedBlue;
    if (str == "decreasedRed")
        return Color::decreasedRed;
    if (str == "decreasedBlue")
        return Color::decreasedBlue;
    if (str == "Hole")
        return Color::Hole;
	if (str == "Border")
		return Color::Border;
    throw std::invalid_argument("Invalid Color string: " + str);
}




template<>
std::string enumToString<Mages>(Mages mage) {
    switch (mage) {
    case Mages::AirMageVelora:
        return "AirMageVelora";
    case Mages::AirMageZephyraCrow:
        return "AirMageZephyraCrow";
    case Mages::EarthMageBumbleroot:
        return "EarthMageBumbleroot";
    case Mages::EarthMageElderbranch:
        return "EarthMageElderbranch";
    case Mages::FireMageIgnara:
        return "FireMageIgnara";
    case Mages::FireMagePyrofang:
        return "FireMagePyrofang";
    case Mages::WaterMageAqualon:
        return "WaterMageAqualon";
    case Mages::WaterMageChillThoughts:
        return "WaterMageChillThoughts";
    default:
        throw std::invalid_argument("Unknown Mages enum value");
    }
}

template<>
Mages stringToEnum<Mages>(const std::string& str) {
    if (str == "AirMageVelora")
        return Mages::AirMageVelora;
    if (str == "AirMageZephyraCrow")
        return Mages::AirMageZephyraCrow;
    if (str == "EarthMageBumbleroot")
        return Mages::EarthMageBumbleroot;
    if (str == "EarthMageElderbranch")
        return Mages::EarthMageElderbranch;
    if (str == "FireMageIgnara")
        return Mages::FireMageIgnara;
    if (str == "FireMagePyrofang")
        return Mages::FireMagePyrofang;
    if (str == "WaterMageAqualon")
        return Mages::WaterMageAqualon;
    if (str == "WaterMageChillThoughts")
        return Mages::WaterMageChillThoughts;
    throw std::invalid_argument("Invalid Mages string: " + str);
}


template<>
std::string enumToString<Power>(Power power) {
    switch (power) {
    case Power::PowerEarthquake:
        return "PowerEarthquake";
	case Power::PowerWaterfall:
		return "PowerWaterfall";
    case Power::PowerTide:
        return "PowerTide";
    case Power::PowerStorm:
        return "PowerStorm";
    case Power::PowerSquall:
        return "PowerSquall";
    case Power::PowerGale:
        return "PowerGale";
	case Power::PowerWave:
		return "PowerWave";
    case Power::PowerGust:
		return "PowerGust";
    case Power::PowerWhirlpool:
		return "PowerWhirlpool";
    case Power::PowerAsh:
		return "PowerAsh";
    case Power::PowerBlizzard:
		return "PowerBlizzard";
	case Power::PowerMist:
		return "PowerMist";
    case Power::PowerMirage:
		return "PowerMirage";
    case Power::PowerRock:
		return "PowerRock";
    case Power::PowerSupport:
		return "PowerSupport";
    case Power::PowerCrumble:
		return "PowerCrumble";
    case Power::PowerControlledExplosion:
		return "PowerControlledExplosion";
    case Power::PowerDestruction:
		return "PowerDestruction";
    case Power::PowerFlame:
		return "PowerFlame";
    case Power::PowerFire:
		return "PowerFire";
    case Power::PowerSpark:
		return "PowerSpark";
	case Power::PowerHurricane:
		return "PowerHurricane";
    case Power::PowerAvalanche:
		return "PowerAvalanche";
    case Power::PowerBorder:
		return "PowerBorder";
    default:
        throw std::invalid_argument("Unknown Power enum value");
    }
}

template<>
Power stringToEnum<Power>(const std::string& str) {
    if (str == "PowerEarthquake")
        return Power::PowerEarthquake;
    if (str == "PowerTide")
        return Power::PowerTide;
	if (str == "PowerWaterfall")
		return Power::PowerWaterfall;
    if (str == "PowerStorm")
        return Power::PowerStorm;
    if (str == "PowerSquall")
        return Power::PowerSquall;
    if (str == "PowerGale")
        return Power::PowerGale;
	if (str == "PowerWave")
		return Power::PowerWave;
    if (str == "PowerGust")
		return Power::PowerGust;
    if (str == "PowerWhirlpool")
		return Power::PowerWhirlpool;
	if (str == "PowerAsh")
		return Power::PowerAsh;
	if (str == "PowerBlizzard")
		return Power::PowerBlizzard;
    if(str == "PowerMist")
		return Power::PowerMist;
    if (str == "PowerMirage")
		return Power::PowerMirage;
	if (str == "PowerRock")
		return Power::PowerRock;
    if (str == "PowerSupport")
        return Power::PowerSupport;
	if (str == "PowerCrumble")
		return Power::PowerCrumble;
    if (str == "PowerControlledExplosion")
		return Power::PowerControlledExplosion;
    if (str == "PowerDestruction")
		return Power::PowerDestruction;
	if (str == "PowerFlame")
		return Power::PowerFlame;
	if (str == "PowerFire")
		return Power::PowerFire;
	if (str == "PowerSpark")
		return Power::PowerSpark;
	if (str == "PowerHurricane")
		return Power::PowerHurricane;
	if (str == "PowerAvalanche")
		return Power::PowerAvalanche;
	if (str == "PowerBorder")
		return Power::PowerBorder;

    throw std::invalid_argument("Invalid Power string: " + str);
}


template<>
std::string enumToString<ActionType>(ActionType action) {
    switch (action) {
    case ActionType::explode:
        return "explode";
    case ActionType::giveBack:
        return "giveBack";
    case ActionType::hole:
        return "hole";
    default: throw std::invalid_argument("Unknown ActionType enum value");
    }
}

template<>
ActionType stringToEnum<ActionType>(const std::string& str) {
    if (str == "explode")
        return ActionType::explode;
    if (str == "giveBack")
        return ActionType::giveBack;
    if (str == "hole")
        return ActionType::hole;
    throw std::invalid_argument("Invalid ActionType string: " + str);
}

template<>
std::string enumToString<GameType>(GameType gameType) {
    switch (gameType) {
    case GameType::Training:
        return "Training";
    case GameType::MageDuel:
        return "MageDuel";
    case GameType::Power:
        return "Power";
    case GameType::MageDuelAndPower:
        return "MageDuelAndPower";
    case GameType::Tournament:
        return "Tournament";
    default:
        throw std::invalid_argument("Unknown GameType enum value");
    }
}

template<>
GameType stringToEnum<GameType>(const std::string& str) {
    if (str == "Training")
        return GameType::Training;
    if (str == "MageDuel")
        return GameType::MageDuel;
    if (str == "Power")
        return GameType::Power;
    if (str == "MageDuelAndPower")
        return GameType::MageDuelAndPower;
    if (str == "Tournament")
        return GameType::Tournament;
    throw std::invalid_argument("Invalid GameType string: " + str);
}