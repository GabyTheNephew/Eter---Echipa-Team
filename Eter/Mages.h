#pragma once
#include <QString>
#include "EnumConversion.h"
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
	return enumToQString(mage);
}
inline Mages fromQStringToMages(const QString& mage)
{
	return qStringToEnum<Mages>(mage);
}