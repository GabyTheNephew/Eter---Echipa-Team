#include "GameSaver.h"

QString GameSaver::generateSaveFileName(const QString& email)
{
	QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString cleanEmail = email;
    return QString("save_%1_%2.json").arg(cleanEmail, timestamp);
}

bool GameSaver::saveGame(const QString& email, const QString& password, const Game& game, const QString& filename)
{
    QString saveFileName = filename.isEmpty() ? generateSaveFileName(email) : filename;

    QJsonObject savedData;

    Game& nonConstGame = const_cast<Game&>(game);

    savedData["email"] = email;
    savedData["password"] = password;  
    savedData["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    savedData["gameType"] = enumToString(nonConstGame.getCurrentGameType()).c_str();
    savedData["roundCounter"] = nonConstGame.getRoundCounter();
    savedData["currentPlayer"] = enumToString(nonConstGame.getCurrentPlayerColor()).c_str();

    savedData["player1Score"] = nonConstGame.getPlayer1Score();
    savedData["player2Score"] = nonConstGame.getPlayer2Score();


    savedData["board"] = boardToJson(nonConstGame.getBoard());
    savedData["boardMaxSize"] = nonConstGame.getBoardMaxSize();

    savedData["player1"] = playerToJson(nonConstGame.getPlayer1());
    savedData["player2"] = playerToJson(nonConstGame.getPlayer2());


    savedData["illusionsEnabled"] = nonConstGame.areIllusionsEnabled();
    savedData["explosionsEnabled"] = nonConstGame.areExplosionsEnabled();
    savedData["timerEnabled"] = nonConstGame.isTimerEnabled();

    savedData["player1MageUsed"] = nonConstGame.isPlayer1MageUsed();
    savedData["player2MageUsed"] = nonConstGame.isPlayer2MageUsed();
    savedData["player1PowerUsed"] = nonConstGame.isPlayer1PowerUsed();
    savedData["player2PowerUsed"] = nonConstGame.isPlayer2PowerUsed();


	QJsonDocument doc(savedData);
    QDir saveDir;
	saveDir.mkpath("saves");


    QFile file("saves/" + saveFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open file for writing:" << file.errorString();
        return false;
    }

    file.write(doc.toJson());
    file.close();

    qDebug() << "Game saved successfully to:" << saveFileName;
    return true;
}

bool GameSaver::loadGame(const QString& filename, Game& game, QString& email, QString& password)
{
    return false;
}

QJsonObject GameSaver::boardToJson(const Board& board)
{
    QJsonObject boardObj;
    QJsonArray boardArray;

    boardObj["rowSize"] = board.getRowSize();
    boardObj["columnSize"] = board.getColumnSize();

	for (int i = 0; i < board.getRowSize(); ++i) 
    {
        for (int j = 0; j < board.getColumnSize(); ++j) 
        {
            QJsonObject positionObj;
			positionObj["row"] = i;
			positionObj["column"] = j;
			
			QJsonArray cardsAtPosition;
            const auto& stack = board[{i,j}];
            for (const auto& card : stack)
            {
				cardsAtPosition.append(cardToJson(card));
            }

			positionObj["cards"] = cardsAtPosition;
			boardArray.append(positionObj);
        }
	}
	boardObj["positions"] = boardArray;
	return boardObj;
}

void GameSaver::jsonToBoard(const QJsonObject& json, Board& board)
{
	int rowSize = json["rowSize"].toInt();
	int columnSize = json["columnSize"].toInt();

    board.resizeBoard(std::max(rowSize, columnSize));

	QJsonArray positionsArray = json["positions"].toArray();

    for (const auto& posValues : positionsArray)
    {
        QJsonObject pos = posValues.toObject();
        int row = pos["row"].toInt();
        int col = pos["column"].toInt();

        QJsonArray cards = pos["cards"].toArray();

        for (const auto& cardValue : cards)
        {
            SimpleCard card = jsonToCard(cardValue.toObject());
            board.pushCard(card, { row, col });
        }
    }
}

QJsonObject GameSaver::playerToJson(const Player& player)
{
    QJsonObject playerObj;

    Player& nonConstPlayer = const_cast<Player&>(player);

    playerObj["name"] = QString::fromStdString(std::string(nonConstPlayer.getName()));
    try {
        playerObj["mage"] = enumToString(nonConstPlayer.getMageEnum()).c_str();
    }
    catch (...) {
        playerObj["mage"] = "None";
    }
    try {
        playerObj["power"] = enumToString(nonConstPlayer.getPower()).c_str();
    }
    catch (...) {
        playerObj["power"] = "None"; 
    }
    playerObj["color"] = nonConstPlayer.GetVectorColor().c_str();

    playerObj["activeCards"] = cardVectorToJson(nonConstPlayer.getVector());
    playerObj["usedCards"] = cardVectorToJson(nonConstPlayer.getPastVector());

    return playerObj;
}

void GameSaver::jsonToPlayer(const QJsonObject& json, Player& player)
{
    player.setName(json["name"].toString().toStdString());
    auto activeCards = jsonToCardVector(json["activeCards"].toArray());
    auto usedCards = jsonToCardVector(json["usedCards"].toArray());

    player.setVector(activeCards);
    player.setPastVector(usedCards);
}

QJsonArray GameSaver::cardVectorToJson(const std::vector<SimpleCard>& cards)
{
    QJsonArray array;
    for (const auto& card : cards) {
        array.append(cardToJson(card));
    }
    return array;
}

std::vector<SimpleCard> GameSaver::jsonToCardVector(const QJsonArray& json)
{
    std::vector<SimpleCard> cards;
    for (const auto& cardValue : json) {
        cards.push_back(jsonToCard(cardValue.toObject()));
    }
    return cards;
}

QJsonObject GameSaver::cardToJson(const SimpleCard& card)
{
    QJsonObject cardObj;
    cardObj["value"] = card.getValue();
    cardObj["color"] = enumToString(card.getColor()).c_str();
    return cardObj;
}

SimpleCard GameSaver::jsonToCard(const QJsonObject& json)
{
    int value = json["value"].toInt();
    Color color = stringToEnum<Color>(json["color"].toString().toStdString());
    return SimpleCard(value, color);
}
