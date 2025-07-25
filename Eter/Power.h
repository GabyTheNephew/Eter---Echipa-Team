#pragma once
#include <QString>
#include <stdexcept>
#include "EnumConversion.h"
enum class Power
{
    PowerEarthquake,
    PowerTide,
    PowerStorm,
    PowerSquall,
	PowerWave,
    PowerGale,
	PowerWaterfall,
    PowerGust,
	PowerWhirlpool,
    PowerAsh,
	PowerBlizzard,
    PowerMist,
	PowerMirage,
    PowerRock,
	PowerSupport,
	PowerCrumble,
    PowerControlledExplosion,
    PowerDestruction,
    PowerFlame,
    PowerFire,
    PowerSpark,
	PowerHurricane,
    PowerAvalanche, 
	PowerBorder
};

inline QString fromPowerToQString(Power power)
{
    return enumToQString(power);
}

inline Power fromQStringToPower(const QString& power)
{
    return qStringToEnum<Power>(power);
}
