#pragma once
#include <QString>
#include <stdexcept>
#include "EnumConversions.h"
enum class Power
{
    PowerEarthquake,
    PowerTide,
    PowerStorm,
    PowerSquall,
    PowerGale,
};

inline QString fromPowerToQString(Power power)
{
    return enumToQString(power);
}

inline Power fromQStringToPower(const QString& power)
{
    return qStringToEnum<Power>(power);
}
