#include "GameSaveManager.h"
#include "../GameSaveLibrary/GameSaveLibrary.h"  

#include <sstream>

bool GameSaveManager::Save(const std::string& filePath, const Game& game) {
    return SaveGame(filePath, SerializeGame(game));
}

bool GameSaveManager::Load(const std::string& filePath, Game& game) {
    std::string data = LoadGame(filePath);
    if (data.empty()) return false;
    DeserializeGame(data, game);
    return true;
}

std::string GameSaveManager::SerializeGame(const Game& game) {
    std::ostringstream ss;

    Game::GameType type = game.getCurrentGameType();
    ss << game.gameTypeToString(type) << "\n";

    const Board& board = game.getBoard();
    ss << board.getRowSize() << " " << board.getColumnSize() << "\n";
    for (int i = 0; i < board.getRowSize(); i++) {
        for (int j = 0; j < board.getColumnSize(); j++) {
            auto& stack = board[{i, j}];
            ss << stack.size();
            for (auto& card : stack) {
                ss << " " << card.getValue() << " " << static_cast<int>(card.getColor());
            }
            ss << " | ";
        }
        ss << "\n";
    }

    auto savePlayer = [&](Player& player, const std::string& name) {
        ss << "Player " << name << "\n";

        auto& hand = player.getVector();
        auto& past = player.getPastVector();

        ss << "Hand " << hand.size();
        for (auto& c : hand) {
            ss << " " << c.getValue() << " " << static_cast<int>(c.getColor());
        }
        ss << "\n";

        ss << "Past " << past.size();
        for (auto& c : past) {
            ss << " " << c.getValue() << " " << static_cast<int>(c.getColor());
        }
        ss << "\n";

        if (type == Game::GameType::MageDuel || type == Game::GameType::MageDuelAndPower) {
            ss << "Mage " << player.getMageAssignment() << "\n";
        }
        if (type == Game::GameType::Power || type == Game::GameType::MageDuelAndPower) {
            ss << "Power " << static_cast<int>(player.getPower()) << "\n";
        }
        };

    savePlayer(const_cast<Game&>(game).getPlayer1(), "Player1");
    savePlayer(const_cast<Game&>(game).getPlayer2(), "Player2");

    ss << "CurrentPlayer " << static_cast<int>(game.getCurrentPlayerColor()) << "\n";

    return ss.str();
}

void GameSaveManager::DeserializeGame(const std::string& data, Game& game) {
    std::istringstream ss(data);
    std::string token;

    std::string gameMode;
    ss >> gameMode;
    Game::GameType type = game.stringToGameType(gameMode);

    game.createWindowFromLoad(type);

    int rows, cols;
    ss >> rows >> cols;
    Board board(rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int stackSize;
            ss >> stackSize;
            for (int k = 0; k < stackSize; k++) {
                int val, color;
                ss >> val >> color;
                board[{i, j}].push_back(SimpleCard(val, static_cast<Color>(color)));
            }
            std::string sep;
            ss >> sep;
        }
    }
    game.getBoard() = board;

    for (int p = 0; p < 2; p++) {
        ss >> token;
        std::string playerName;
        ss >> playerName;

        ss >> token;
        int handSize;
        ss >> handSize;
        std::vector<SimpleCard> hand;
        for (int i = 0; i < handSize; i++) {
            int val, color;
            ss >> val >> color;
            hand.push_back(SimpleCard(val, static_cast<Color>(color)));
        }

        ss >> token;
        int pastSize;
        ss >> pastSize;
        std::vector<SimpleCard> past;
        for (int i = 0; i < pastSize; i++) {
            int val, color;
            ss >> val >> color;
            past.push_back(SimpleCard(val, static_cast<Color>(color)));
        }

        Player player(playerName, hand, past);

        if (type == Game::GameType::MageDuel ||
            type == Game::GameType::MageDuelAndPower) {
            ss >> token;
            int mageId;
            ss >> mageId;
            player.setMageAssignment(mageId);
        }

        if (type == Game::GameType::Power ||
            type == Game::GameType::MageDuelAndPower) {
            ss >> token;
            int powerId;
            ss >> powerId;
            player.setPowerAssignment(powerId);
        }

        if (p == 0) {
            game.getPlayer1() = player;
        }
        else {
            game.getPlayer2() = player;
        }
    }

    ss >> token;
    int currentColorInt;
    ss >> currentColorInt;
    game.setCurrentPlayerColor(static_cast<Color>(currentColorInt));

    if (auto* win = game.getCurrentGameWindow()) {
        win->setBoard(game.getBoard(), game.getBoard().getSize());
        win->setPlayer1Cards(game.getPlayer1().getVector());
        win->setPlayer2Cards(game.getPlayer2().getVector());
        win->setCurrentPlayer(game.getCurrentPlayerColor());
        win->updateBoardView();

        win->show();
        win->raise();
        win->activateWindow();
    }
}
