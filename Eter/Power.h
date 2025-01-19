#pragma once
#include <QString>
#include <stdexcept>

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
    switch (power)
    {
    case Power::PowerEarthquake: return QString("PowerEarthquake");
    case Power::PowerTide: return QString("PowerTide");
    case Power::PowerStorm: return QString("PowerStorm");
    case Power::PowerSquall: return QString("PowerSquall");
    case Power::PowerGale: return QString("PowerGale");
    }
    throw std::invalid_argument("Invalid Power enum value");
}

inline Power fromQStringToPower(const QString& power)
{
    if (power == QString("PowerEarthquake")) return Power::PowerEarthquake;
    if (power == QString("PowerTide")) return Power::PowerTide;
    if (power == QString("PowerStorm")) return Power::PowerStorm;
    if (power == QString("PowerSquall")) return Power::PowerSquall;
    if (power == QString("PowerGale")) return Power::PowerGale;
    throw std::invalid_argument("Invalid Power string: " + power.toStdString());
}
