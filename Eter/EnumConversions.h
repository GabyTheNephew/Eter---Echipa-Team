#pragma once
#include <string>
#include <stdexcept>
#include <QString>


template<typename EnumType>
std::string enumToString(EnumType value) {
	static_assert(false,"enumToString not implemented for this type");
}

template<typename EnumType>
EnumType stringToEnum(const std::string& str) {
	static_assert(false, "stringToEnum not implemented for this enum type");
}

template<typename EnumType>
QString enumToQString(EnumType value) {
	return QString::fromStdString(enumToString(value));
}

template<typename EnumType>
EnumType qStringToEnum(const QString& str) {
	return stringToEnum<EnumType>(str.toStdString());
}


enum class Color;
enum class Mages;
enum class Power;
enum class ActionType;


template<> std::string enumToString<Color>(Color color);
template<> Color stringToEnum<Color>(const std::string& str);


template<> std::string enumToString<Mages>(Mages mage);
template<> Mages stringToEnum<Mages>(const std::string& str);


template<> std::string enumToString<Power>(Power power);
template<> Power stringToEnum<Power>(const std::string& str);


template<> std::string enumToString<ActionType>(ActionType action);
template<> ActionType stringToEnum<ActionType>(const std::string& str);