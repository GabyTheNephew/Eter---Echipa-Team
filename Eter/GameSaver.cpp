#include "GameSaver.h"

#pragma region Save and load logic

QString GameSaver::generateSaveFileName(const QString& email)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString cleanEmail = email;
    return QString("save_%1_%2.json").arg(cleanEmail, timestamp);
}

bool GameSaver::saveGame(const QString& email, const QString& password, const Game& game, const QString& filename)
{
    Game& nonConstGame = const_cast<Game&>(game);

    bool gameFinished = false;

    switch (nonConstGame.getCurrentGameType()) {
    case GameType::Training:
        gameFinished = (nonConstGame.getPlayer1Score() >= 2 || nonConstGame.getPlayer2Score() >= 2);
        break;

    case GameType::MageDuel:
        gameFinished = (nonConstGame.getPlayer1Score() >= 3 || nonConstGame.getPlayer2Score() >= 3);
        break;
    case GameType::Power:
        gameFinished = (nonConstGame.getPlayer1Score() >= 3 || nonConstGame.getPlayer2Score() >= 3);
        break;

    case GameType::MageDuelAndPower:
        gameFinished = (nonConstGame.getPlayer1Score() >= 2 || nonConstGame.getPlayer2Score() >= 2);
        break;
    }

    if (gameFinished) {
        qDebug() << "Game is finished - not saving completed game";
        return false;
    }

    QString saveFileName = filename.isEmpty() ? generateSaveFileName(email) : filename;

    QJsonObject savedData;

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
    savedData["player1IllusionUsed"] = nonConstGame.isPlayer1IllusionUsed();
    savedData["player2IllusionUsed"] = nonConstGame.isPlayer2IllusionUsed();

    savedData["explosionActivated"] = nonConstGame.m_explosionActivated;
    if (nonConstGame.m_explosion)
    {
        savedData["currentExplosion"] = explosionToJson(*nonConstGame.m_explosion);
    }
    else
    {
        savedData["currentExplosion"] = QJsonObject();
    }
    savedData["timerDuration"] = nonConstGame.getTimerDuration();
    savedData["player1RemainingTime"] = nonConstGame.getPlayer1RemainingTime();
    savedData["player2RemainingTime"] = nonConstGame.getPlayer2RemainingTime();


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
    QFile file("saves/" + filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for reading:" << file.errorString();
        return false;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull()) {
        qDebug() << "Invalid JSON format in file:" << filename;
        return false;
    }
    QJsonObject savedData = doc.object();

    try
    {
        email = savedData["email"].toString();
        password = savedData["password"].toString();

        Game& gameInstance = Game::get_Instance();

        gameInstance.m_currentGameType = stringToEnum<GameType>(savedData["gameType"].toString().toStdString());
        gameInstance.m_round_Counter = savedData["roundCounter"].toInt();
        gameInstance.m_boardMaxSize = savedData["boardMaxSize"].toInt();

        gameInstance.player1RoundsWon = savedData["player1Score"].toInt();
        gameInstance.player2RoundsWon = savedData["player2Score"].toInt();

        gameInstance.currentPlayer = stringToEnum<Color>(savedData["currentPlayer"].toString().toStdString());


        gameInstance.m_gameBoard = Board(1);
        jsonToBoard(savedData["board"].toObject(), gameInstance.m_gameBoard);

        jsonToPlayer(savedData["player1"].toObject(), gameInstance.player1);
        jsonToPlayer(savedData["player2"].toObject(), gameInstance.player2);


        gameInstance.m_illusionsEnabled = savedData["illusionsEnabled"].toBool();
        if (savedData["explosionsEnabled"].toBool()) {
            gameInstance.setExplosionsEnabled(true);
        }
        gameInstance.m_timerEnabled = savedData["timerEnabled"].toBool();


        gameInstance.m_player1MageUsed = savedData["player1MageUsed"].toBool();
        gameInstance.m_player2MageUsed = savedData["player2MageUsed"].toBool();
        gameInstance.m_player1PowerUsed = savedData["player1PowerUsed"].toBool();
        gameInstance.m_player2PowerUsed = savedData["player2PowerUsed"].toBool();
        gameInstance.m_player1IllusionUsed = savedData["player1IllusionUsed"].toBool();
        gameInstance.m_player2IllusionUsed = savedData["player2IllusionUsed"].toBool();

        gameInstance.m_explosionActivated = savedData["explosionActivated"].toBool();

        QJsonObject explosionObj = savedData["currentExplosion"].toObject();
        if (!explosionObj.isEmpty())
        {
            gameInstance.m_explosion = jsonToExplosion(explosionObj);
        }
        else
        {
            gameInstance.m_explosion.reset();
        }

        if (savedData.contains("timerDuration")) {
            gameInstance.setTimerDuration(savedData["timerDuration"].toInt());
        }
        if (savedData.contains("player1RemainingTime")) {
            gameInstance.player1RemainingTime = savedData["player1RemainingTime"].toInt();
        }
        if (savedData.contains("player2RemainingTime")) {
            gameInstance.player2RemainingTime = savedData["player2RemainingTime"].toInt();
        }

        gameInstance.setUserCredentials(email, password);

        qDebug() << "Game loaded successfully!";
        return true;

    }
    catch (const std::exception& e) {
        qDebug() << "Error loading game:" << e.what();
        return false;
    }
}

#pragma endregion

#pragma region ToJson Convertors
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
            const auto& stack = board[{i, j}];
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
QJsonArray GameSaver::cardVectorToJson(const std::vector<SimpleCard>& cards)
{
    QJsonArray array;
    for (const auto& card : cards) {
        array.append(cardToJson(card));
    }
    return array;
}
QJsonObject GameSaver::cardToJson(const SimpleCard& card)
{
    QJsonObject cardObj;
    cardObj["value"] = card.getValue();
    cardObj["color"] = enumToString(card.getColor()).c_str();
    return cardObj;
}
QJsonObject GameSaver::explosionToJson(const Explosion& explosion)
{
    QJsonObject explosionObj;
    QJsonArray posArray;

    for (const auto& [x, y, action] : explosion.getPositions())
    {
        QJsonObject positionObj;
        positionObj["x"] = x;
        positionObj["y"] = y;
        positionObj["action"] = enumToString(action).c_str();
        posArray.append(positionObj);
    }

    explosionObj["positions"] = posArray;
    return explosionObj;
}

#pragma endregion

#pragma region JsonTo

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


void GameSaver::jsonToPlayer(const QJsonObject& json, Player& player)
{
    player.setName(json["name"].toString().toStdString());

    QString mageStr = json["mage"].toString();
    if (!mageStr.isEmpty() && mageStr != "None") {
        try {
            Mages mage = stringToEnum<Mages>(mageStr.toStdString());
            player.setMage(mage);
        }
        catch (...) {
            qDebug() << "Failed to load mage:" << mageStr;
        }
    }

    QString powerStr = json["power"].toString();
    if (!powerStr.isEmpty() && powerStr != "None") {
        try {
            Power power = stringToEnum<Power>(powerStr.toStdString());
            player.setPower(power);
        }
        catch (...) {
            qDebug() << "Failed to load power:" << powerStr;
        }
    }
    auto activeCards = jsonToCardVector(json["activeCards"].toArray());
    auto usedCards = jsonToCardVector(json["usedCards"].toArray());

    player.setVector(activeCards);
    player.setPastVector(usedCards);
}


std::vector<SimpleCard> GameSaver::jsonToCardVector(const QJsonArray& json)
{
    std::vector<SimpleCard> cards;
    for (const auto& cardValue : json) {
        cards.push_back(jsonToCard(cardValue.toObject()));
    }
    return cards;
}


SimpleCard GameSaver::jsonToCard(const QJsonObject& json)
{
    int value = json["value"].toInt();
    Color color = stringToEnum<Color>(json["color"].toString().toStdString());
    return SimpleCard(value, color);
}


std::unique_ptr<Explosion> GameSaver::jsonToExplosion(const QJsonObject& json)
{

    auto explosion = std::make_unique<Explosion>();
    explosion->positions.clear();


    QJsonArray positionsArray = json["positions"].toArray();
    for (const auto& posValue : positionsArray)
    {
        QJsonObject pos = posValue.toObject();
        int16_t x = pos["x"].toInt();
        int16_t y = pos["y"].toInt();
        ActionType action = stringToEnum<ActionType>(pos["action"].toString().toStdString());

        explosion->positions.emplace_back(x, y, action);
    }

    return explosion;

}

#pragma endregion
