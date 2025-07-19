#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include <QString>
#include <QDir>
#include <QDebug>
#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "EnumConversion.h"


class GameSaver
{
private:
	static QString generateSaveFileName(const QString& email);

public:
#pragma region Main Methods
	static bool saveGame(const QString& email, const QString& password, const Game& game, const QString& filename = "");

	static bool loadGame(const QString& filename, Game& game, QString& email, QString& password);
#pragma endregion

	
#pragma region Convertors Methods
	static QJsonObject boardToJson(const Board& board);
	static void jsonToBoard(const QJsonObject& json, Board& board);


	static QJsonObject playerToJson(const Player& player);
	static void jsonToPlayer(const QJsonObject& json, Player& player);


	static QJsonArray cardVectorToJson(const std::vector<SimpleCard>& cards);
	static std::vector<SimpleCard> jsonToCardVector(const QJsonArray& json);

	static QJsonObject cardToJson(const SimpleCard& card);
	static SimpleCard jsonToCard(const QJsonObject& json);

	static QJsonObject explosionToJson(const Explosion& explosion);
	static std::unique_ptr<Explosion> jsonToExplosion(const QJsonObject& json);
#pragma endregion

	
};

