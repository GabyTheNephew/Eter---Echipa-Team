#pragma once
#include <QString>

enum class Mages
{
	AirMageVelora,
	AirMageZephyraCrow,
	EarthMageBumbleroot,
	EarthMageElderbranch,
	FireMageIgnara,
	FireMagePyrofang,
	WaterMageAqualon,
	WaterMageChillThoughts
};

inline QString fromMagesToQString(Mages mage)
{
	switch (mage)
	{
		case Mages::AirMageVelora:
			return QString("AirMageVelora");
		case Mages::AirMageZephyraCrow:
			return QString("AirMageZephyraCrow");
		case Mages::EarthMageBumbleroot:
			return QString("EarthMageBumbleroot");
		case Mages::EarthMageElderbranch:
			return QString("EarthMageElderbranch");
		case Mages::FireMageIgnara:
			return QString("FireMageIgnara");
		case Mages::FireMagePyrofang:
			return QString("FireMagePyrofang");
		case Mages::WaterMageAqualon:
			return QString("WaterMageAqualon");
		case Mages::WaterMageChillThoughts:
			return QString("WaterMageChillThoughts");
	}
	return QString();
}
inline Mages fromQStringToMages(const QString& mage)
{
	if (mage == QString("AirMageVelora"))
		return Mages::AirMageVelora;
	if (mage == QString("AirMageZephyraCrow"))
		return Mages::AirMageZephyraCrow;
	if (mage == QString("EarthMageBumbleroot"))
		return Mages::EarthMageBumbleroot;
	if (mage == QString("EarthMageElderbranch"))
		return Mages::EarthMageElderbranch;
	if (mage == QString("FireMageIgnara"))
		return Mages::FireMageIgnara;
	if (mage == QString("FireMagePyrofang"))
		return Mages::FireMagePyrofang;
	if (mage == QString("WaterMageAqualon"))
		return Mages::WaterMageAqualon;
	if (mage == QString("WaterMageChillThoughts"))
		return Mages::WaterMageChillThoughts;
	throw std::invalid_argument("Invalid mage string: " + mage.toStdString());
}