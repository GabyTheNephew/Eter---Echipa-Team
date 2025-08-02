#include "SecondaryWindow.h"


#pragma region Setters
void SecondaryWindow::setBoard(Board& board, int setMaxSize) {
    if (!m_boardView) {
        m_boardView = new BoardView(board, this, setMaxSize);
        m_boardView->setFixedSize(400, 400);


        mainLayout->insertWidget(1, m_boardView, 0, Qt::AlignHCenter | Qt::AlignVCenter);


        connect(m_boardView, &BoardView::cellClicked, this, &SecondaryWindow::onBoardClicked);


        m_boardView->updateView();
    }
}

void SecondaryWindow::showWinner(const QString& winnerName) {
    QMessageBox* msgBox = new QMessageBox();
    msgBox->setWindowTitle("Game Over");
    msgBox->setText("The Winner is: " + winnerName);
    msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Close);
    msgBox->setWindowModality(Qt::ApplicationModal);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);

    connect(msgBox, &QMessageBox::finished, this, [this](int result) {
        if (result == QMessageBox::Ok) {
            this->resetView();
        }
        else {
            emit closed();
        }
        });

    msgBox->show();
}




void SecondaryWindow::setPlayer1Cards(const std::vector<SimpleCard>& cards) {
    QLayoutItem* child;
    while ((child = player1CardsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    for (const auto& card : cards) {
        if (card.getColor() == Color::usedRed) {
            continue;
        }
        auto cardButton = new QPushButton(this);

        QString imagePath = "red";
        imagePath += QString::number(card.getValue()) + ".jpg";

        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            QIcon buttonIcon(pixmap.scaled(150, 150, Qt::KeepAspectRatioByExpanding));
            cardButton->setIcon(buttonIcon);
            cardButton->setIconSize(QSize(150, 150));
        }
        else {
            cardButton->setText("Card not found");
        }

        cardButton->setStyleSheet("border: none;");
        player1CardsLayout->addWidget(cardButton);


        connect(cardButton, &QPushButton::clicked, this, [this, card]() {
            onCardSelected(card);
            });
    }
}


void SecondaryWindow::setPlayer2Cards(const std::vector<SimpleCard>& cards) {
    QLayoutItem* child;
    while ((child = player2CardsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    int imageWidth = 150;
    int imageHeight = 200;
    int spacing = 20;
    player2CardsLayout->setSpacing(spacing);

    for (const auto& card : cards) {
        if (card.getColor() == Color::usedBlue) {
            continue;
        }
        auto cardButton = new QPushButton(this);

        QString imagePath = "blue";
        imagePath += QString::number(card.getValue()) + ".jpg";

        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            QIcon buttonIcon(pixmap.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio));
            cardButton->setIcon(buttonIcon);
            cardButton->setIconSize(QSize(imageWidth, imageHeight));
        }
        else {
            cardButton->setText("Card not found");
            cardButton->setStyleSheet("border: 1px solid black; background-color: white;");
        }

        cardButton->setStyleSheet("border: none;");
        player2CardsLayout->addWidget(cardButton);

        connect(cardButton, &QPushButton::clicked, this, [this, card]() {
            onCardSelected(card);
            });

    }
}

void SecondaryWindow::setCurrentPlayer(Color player) {
    currentPlayer = player;
    if (game->isTimerEnabled()) {
        game->startPlayerTimer();
    }
    qDebug() << "Current player changed to:" << (player == Color::Red ? "Red" : "Blue");

}



void SecondaryWindow::setMages(const QString& mage1Name, const QString& mage2Name) {


    QString mage1ImagePath = mage1Name + ".jpg";
    QPixmap mage1Pixmap(mage1ImagePath);
    QIcon mage1Icon(mage1Pixmap.scaled(150, 150, Qt::KeepAspectRatioByExpanding));

    mage1Button = new QPushButton(this);
    mage1Button->setStyleSheet("background-color: transparent; border: none;");
    mage1Button->setFixedSize(150, 150);
    mage1Button->setIcon(mage1Icon);
    mage1Button->setIconSize(QSize(150, 150));
    connect(mage1Button, &QPushButton::clicked, this, [this, mage1Name]() {
        onMageClicked(mage1Name, Color::Red);
        });




    QVBoxLayout* player1MageLayout = new QVBoxLayout();
    player1MageLayout->addSpacerItem(new QSpacerItem(0, 420, QSizePolicy::Minimum, QSizePolicy::Fixed));
    player1MageLayout->addWidget(mage1Button, 0, Qt::AlignLeft);
    player1CardsLayout->addLayout(player1MageLayout);


    QString mage2ImagePath = mage2Name + ".jpg";
    QPixmap mage2Pixmap(mage2ImagePath);

    QIcon mage2Icon(mage2Pixmap.scaled(150, 150, Qt::KeepAspectRatioByExpanding));


    mage2Button = new QPushButton(this);
    mage2Button->setFixedSize(150, 150);
    mage2Button->setIcon(mage2Icon);
    mage2Button->setIconSize(QSize(150, 150));

    connect(mage2Button, &QPushButton::clicked, this, [this, mage2Name]() {
        onMageClicked(mage2Name, Color::Blue);
        });

    mage2Button->setStyleSheet("background-color: rgba(0, 255, 0, 0.3); border: 10px;");




    QVBoxLayout* player2MageLayout = new QVBoxLayout();


    player2MageLayout->setContentsMargins(0, 170, 0, 0);


    player2MageLayout->addWidget(mage2Button);

    player2MageLayout->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));


    player1CardsLayout->addLayout(player2MageLayout);

    updateMageButtons();
}

void SecondaryWindow::setPowers(const QString& power1Name, const QString& power2Name) {
    QString power1ImagePath = power1Name + ".jpg";
    QPixmap power1Pixmap(power1ImagePath);
    QIcon power1Icon(power1Pixmap.scaled(150, 150, Qt::KeepAspectRatioByExpanding));

    power1Button = new QPushButton(this);
    power1Button->setStyleSheet("background-color: transparent; border: none;");
    power1Button->setFixedSize(150, 150);
    power1Button->setIcon(power1Icon);
    power1Button->setIconSize(QSize(150, 150));
    connect(power1Button, &QPushButton::clicked, this, [this, power1Name]() {
        onPowerClicked(power1Name, Color::Red);
        });

    QVBoxLayout* player1PowerLayout = new QVBoxLayout();
    player1PowerLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Fixed));
    player1PowerLayout->addWidget(power1Button, 0, Qt::AlignLeft);
    player1CardsLayout->addLayout(player1PowerLayout);

    QString power2ImagePath = power2Name + ".jpg";
    QPixmap power2Pixmap(power2ImagePath);
    QIcon power2Icon(power2Pixmap.scaled(150, 150, Qt::KeepAspectRatioByExpanding));

    power2Button = new QPushButton(this);
    power2Button->setFixedSize(150, 150);
    power2Button->setIcon(power2Icon);
    power2Button->setIconSize(QSize(150, 150));
    connect(power2Button, &QPushButton::clicked, this, [this, power2Name]() {
        onPowerClicked(power2Name, Color::Blue);
        });
    power2Button->setStyleSheet("background-color: rgba(0, 255, 0, 0.3); border: 10px;");

    QVBoxLayout* player2PowerLayout = new QVBoxLayout();
    player2PowerLayout->setContentsMargins(0, 400, 0, 0);
    player2PowerLayout->addWidget(power2Button);
    player2PowerLayout->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    player1CardsLayout->addLayout(player2PowerLayout);


    updatePowerButtons();
}
void SecondaryWindow::setMagesAndPowers(const QString& mage1Name, const QString& mage2Name, const QString& power1Name, const QString& power2Name)
{
    QVBoxLayout* player1CombinedLayout = new QVBoxLayout();


    QString mage1ImagePath = mage1Name + ".jpg";
    QPixmap mage1Pixmap(mage1ImagePath);
    QIcon mage1Icon(mage1Pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    mage1Button = new QPushButton(this);
    mage1Button->setIcon(mage1Icon);
    mage1Button->setIconSize(QSize(100, 100));
    mage1Button->setStyleSheet("background-color: transparent; border: none;");
    connect(mage1Button, &QPushButton::clicked, this, [this, mage1Name]() {
        onMageClicked(mage1Name, Color::Red);
        });


    QString power1ImagePath = power1Name + ".jpg";
    QPixmap power1Pixmap(power1ImagePath);
    QIcon power1Icon(power1Pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    power1Button = new QPushButton(this);
    power1Button->setIcon(power1Icon);
    power1Button->setIconSize(QSize(100, 100));
    power1Button->setStyleSheet("background-color: transparent; border: none;");
    connect(power1Button, &QPushButton::clicked, this, [this, power1Name]() {
        onPowerClicked(power1Name, Color::Red);
        });


    player1CombinedLayout->addWidget(mage1Button, 0, Qt::AlignLeft);
    player1CombinedLayout->addWidget(power1Button, 0, Qt::AlignLeft);

    player1CardsLayout->addLayout(player1CombinedLayout);


    QVBoxLayout* player2CombinedLayout = new QVBoxLayout();


    QString mage2ImagePath = mage2Name + ".jpg";
    QPixmap mage2Pixmap(mage2ImagePath);
    QIcon mage2Icon(mage2Pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    mage2Button = new QPushButton(this);
    mage2Button->setIcon(mage2Icon);
    mage2Button->setIconSize(QSize(100, 100));
    mage2Button->setStyleSheet("background-color: transparent; border: none;");
    connect(mage2Button, &QPushButton::clicked, this, [this, mage2Name]() {
        onMageClicked(mage2Name, Color::Blue);
        });


    QString power2ImagePath = power2Name + ".jpg";
    QPixmap power2Pixmap(power2ImagePath);
    QIcon power2Icon(power2Pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    power2Button = new QPushButton(this);
    power2Button->setIcon(power2Icon);
    power2Button->setIconSize(QSize(100, 100));
    power2Button->setStyleSheet("background-color: transparent; border: none;");
    connect(power2Button, &QPushButton::clicked, this, [this, power2Name]() {
        onPowerClicked(power2Name, Color::Blue);
        });

    player2CombinedLayout->addWidget(mage2Button, 0, Qt::AlignRight);
    player2CombinedLayout->addWidget(power2Button, 0, Qt::AlignRight);

    player1CardsLayout->addLayout(player2CombinedLayout);

    updateMageButtons();
    updatePowerButtons();
}


#pragma endregion


SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, Game* gameInstance,
    const QString& mage1Name, const QString& mage2Name, const QString& power1Name, const QString& power2Name,
    bool checkMage, bool checkPower, QWidget* parent)
    : QWidget(parent), imagePath(imagePath), game(gameInstance) {

    setWindowTitle(title);

    mainLayout = new QVBoxLayout(this);

    player2CardsLayout = new QHBoxLayout();
    mainLayout->addLayout(player2CardsLayout);

    mainLayout->addSpacerItem(new QSpacerItem(0, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));

   
    m_boardView = nullptr;


    mainLayout->addSpacerItem(new QSpacerItem(0, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));


    player1CardsLayout = new QHBoxLayout();
    mainLayout->addLayout(player1CardsLayout);


    player2TimerLabel = new QLabel("Time: 1:30", this);
    player2TimerLabel->setStyleSheet(
        "color: white; "
        "font-size: 14px; "
        "font-weight: bold; "
        "background-color: rgba(74, 144, 226, 0.8); "
        "padding: 5px 10px; "
        "border-radius: 5px; "
        "border: 2px solid #4A90E2;"
    );
    player2TimerLabel->setAlignment(Qt::AlignCenter);
    player2TimerLabel->setFixedSize(80, 30);
    player2TimerLabel->move(20, 200);  

    
    player1TimerLabel = new QLabel("Time: 1:30", this);
    player1TimerLabel->setStyleSheet(
        "color: white; "
        "font-size: 14px; "
        "font-weight: bold; "
        "background-color: rgba(231, 76, 60, 0.8); "
        "padding: 5px 10px; "
        "border-radius: 5px; "
        "border: 2px solid #E74C3C;"
    );
    player1TimerLabel->setAlignment(Qt::AlignCenter);
    player1TimerLabel->setFixedSize(80, 30);



    uiUpdateTimer = new QTimer(this);
    connect(uiUpdateTimer, &QTimer::timeout, this, &SecondaryWindow::updateTimerDisplay);
    uiUpdateTimer->start(100);

    
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
        QBrush(QPixmap(imagePath).scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    
    if (checkMage && checkPower) {
        setMagesAndPowers(mage1Name, mage2Name, power1Name, power2Name);
    }
    else if (checkMage) {
        setMages(mage1Name, mage2Name);
    }
    else if (checkPower) {
        setPowers(power1Name, power2Name);
    }

    this->showFullScreen();
}

void SecondaryWindow::updateTimerDisplay() {
    if (!player1TimerLabel || !player2TimerLabel) {
        return;
    }

    if (!game->isTimerEnabled()) {
        player1TimerLabel->setVisible(false);
        player2TimerLabel->setVisible(false);
        return;
    }

    player1TimerLabel->setVisible(true);
    player2TimerLabel->setVisible(true);

    int p1Time = game->getPlayer1RemainingTime();
    int p2Time = game->getPlayer2RemainingTime();

    QString p1Text = QString("%1:%2").arg(p1Time / 60).arg(p1Time % 60, 2, 10, QChar('0'));
    QString p2Text = QString("%1:%2").arg(p2Time / 60).arg(p2Time % 60, 2, 10, QChar('0'));

    player1TimerLabel->setText(p1Text);
    player2TimerLabel->setText(p2Text);

    if (p1Time <= 10) {
        player1TimerLabel->setStyleSheet(
            "color: white; "
            "font-size: 14px; "
            "font-weight: bold; "
            "background-color: rgba(255, 0, 0, 0.9); "
            "padding: 5px 10px; "
            "border-radius: 5px; "
            "border: 2px solid red;"
        );
    }

    if (p2Time <= 10) {
        player2TimerLabel->setStyleSheet(
            "color: white; "
            "font-size: 14px; "
            "font-weight: bold; "
            "background-color: rgba(255, 0, 0, 0.9); "
            "padding: 5px 10px; "
            "border-radius: 5px; "
            "border: 2px solid red;"
        );
    }
}


#pragma region Evenst

void SecondaryWindow::closeEvent(QCloseEvent* event) {
    Game::forceStop();
    event->accept();
}

void SecondaryWindow::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    QPixmap backgroundPixmap(imagePath);
    if (!backgroundPixmap.isNull()) {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        this->setPalette(palette);
    }


    if (player1TimerLabel) {
        player1TimerLabel->move(20, this->height() - 200);
    }
}

void SecondaryWindow::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_Escape) {
        if (!menu) {
            menu = std::make_unique<MenuWindow>(this);

            connect(menu.get(), &MenuWindow::goToHome, this, [this]() {
                menu->hide();
                this->hide();
                emit closed();
                Game::get_Instance().emit gameEnded();
                });

            connect(menu.get(), &MenuWindow::exitApp, []() {
                Game::forceStop();
                });

            menu->hide();
        }

        if (menu->isVisible()) {
            menu->hide();
        }
        else {
            menu->show();
            menu->raise();
            menu->activateWindow();
        }
    }
    else {
        QWidget::keyPressEvent(event);
    }
}


#pragma endregion


#pragma region Board and Card things

void SecondaryWindow::onBoardClicked(int row, int col) {
    if (!selectedCard.getValue()) {
        qDebug() << "No card selected!";
        return;
    }

    qDebug() << "Attempting to place card at (" << row << ", " << col << "):";


    if (selectedCardAsIllusion && !m_boardView->getBoard()[{row, col}].empty()) {
        QMessageBox::warning(this, "Invalid Move", "Illusions can only be placed on empty spaces!");
        return;
    }


    if (game->m_restrictionRemainingTurns > 0)
    {
        for (const auto& [restrictedRow, restrictedCol] : game->m_restrictedPositions)
        {
            if (row == restrictedRow && col == restrictedCol)
            {
                QMessageBox::warning(this, "Restricted Position", "This position is restricted by Tsunami!");
                return;
            }
        }
    }
    SimpleCard cardToPlace = selectedCard;
    if (selectedCardAsIllusion) {
        if (currentPlayer == Color::Red) {
            cardToPlace.setColor(Color::IlusionRed);

        }
        else {
            cardToPlace.setColor(Color::IlusionBlue);

        }
    }

    bool placementResult = handleCardPlacement(row, col, cardToPlace);
    bool shouldEndTurn = false;

    if (placementResult)
    {

        if (selectedCardAsIllusion)
        {
            if (currentPlayer == Color::Red)
            {
                game->setPlayer1IllusionUsed(true);
            }
            else
            {
                game->setPlayer2IllusionUsed(true);
            }
        }
        shouldEndTurn = true;
    }
    else
    {
        Board& board = m_boardView->getBoard();
        if (row >= 0 && row < board.getRowSize() && col >= 0 && col < board.getColumnSize())
        {
            if (!board[{row, col}].empty())
            {
                SimpleCard topCard = board[{row, col}].back();
                if ((topCard.getColor() == Color::Red || topCard.getColor() == Color::Blue) && (cardToPlace.getColor() == Color::Red || cardToPlace.getColor() == Color::Blue) && (topCard.getColor() != cardToPlace.getColor()))
                {

                    shouldEndTurn = true;
                }
            }
        }
    }

    if (shouldEndTurn) {

        game->getCurrentPlayer().makeCardInvalid(selectedCard);
        game->getCurrentPlayer().getPastVector().push_back(selectedCard);

        if (currentPlayer == Color::Red)
        {
            setPlayer1Cards(game->getCurrentPlayer().getVector());
        }
        else
        {
            setPlayer2Cards(game->getCurrentPlayer().getVector());
        }

        selectedCard = SimpleCard();
        optimizeBoard();
        m_boardView->updateView();

        qDebug() << "Turn completed at (" << row << ", " << col << ").";
        game->setPlayerMoveCompleted(true);
    }
}




bool SecondaryWindow::handleCardPlacement(int row, int col, const SimpleCard& cardToPlace) {
    Board& board = m_boardView->getBoard();


    if (cardToPlace.getColor() == Color::IlusionRed || cardToPlace.getColor() == Color::IlusionBlue)
    {
        if (row >= 0 && row < board.getRowSize() && col >= 0 && col < board.getColumnSize())
        {
            if (!board[{row, col}].empty())
            {
                QMessageBox::warning(this, "Invalid Move", "Illusions must be placed on empty spaces!");
                return false;
            }
            board.pushCard(cardToPlace, { row, col });
            return true;
        }
        else {

            if (!expandBoardForPosition(row, col))
            {
                return false;
            }
            int newRow = row < 0 ? 0 : row;
            int newCol = col < 0 ? 0 : col;
            board.pushCard(cardToPlace, { newRow, newCol });
            return true;
        }
    }


    if (row >= 0 && row < board.getRowSize() && col >= 0 && col < board.getColumnSize()) {
        Board::Position pos = { row, col };

        if (!board[pos].empty())
        {
            SimpleCard topCard = board[pos].back();
            if (topCard.getColor() == Color::IlusionRed || topCard.getColor() == Color::IlusionBlue)
            {
                return handleIllusionCovering(row, col, cardToPlace, topCard);
            }
        }
        if (board.canBePushed(cardToPlace, pos)) {
            board.pushCard(cardToPlace, pos);
            qDebug() << "Card placed successfully at existing position (" << row << ", " << col << ")";
            return true;
        }
        else {
            QMessageBox::warning(this, "Invalid Move",
                "Cannot place card at this position!\n"
                "Card value must be higher than the top card in the stack.");
            return false;
        }
    }
    else {
        if (!board.canBePlaced(row, col)) {
            QMessageBox::warning(this, "Invalid Position",
                "Cannot place card at this position!\n"
                "Position must be adjacent to existing cards.");
            return false;
        }

        if (!expandBoardForPosition(row, col)) {
            qDebug() << "Cannot expand board for position (" << row << ", " << col << ")";
            return false;
        }


        int newRow = row;
        int newCol = col;

        if (row < 0) newRow = 0;
        if (col < 0) newCol = 0;

        Board::Position pos = { newRow, newCol };


        if (board.canBePushed(cardToPlace, pos)) {
            board.pushCard(cardToPlace, pos);
            return true;
        }
        else {
            QMessageBox::warning(this, "Invalid Move",
                "Cannot place card at this position!\n"
                "Card value must be higher than the top card in the stack.");
            return false;
        }
    }
}
bool SecondaryWindow::handleIllusionCovering(int row, int col, const SimpleCard& attackCard, SimpleCard& illusionCard)
{
    Board& board = m_boardView->getBoard();

    if ((attackCard.getColor() == Color::Red && illusionCard.getColor() == Color::IlusionRed) || (attackCard.getColor() == Color::Blue && illusionCard.getColor() == Color::IlusionBlue))
    {
        QMessageBox::warning(this, "Invalid Move", "You cannot cover your own illusion!");
        return false;
    }

    Color revealedColor = (illusionCard.getColor() == Color::IlusionRed) ? Color::Red : Color::Blue;
    illusionCard.setColor(revealedColor);

    board[{row, col}].back().setColor(revealedColor);

    QMessageBox::information(this, "Illusion Revealed!",
        QString("Illusion revealed! Value: %1").arg(illusionCard.getValue()));

    if (attackCard.getValue() > illusionCard.getValue()) {
        board.pushCard(attackCard, { row, col });
        return true;
    }
    else {
        QMessageBox::information(this, "Attack Failed!",
            "Your card was eliminated! Turn ends.");
        return false;
    }
}

bool SecondaryWindow::expandBoardForPosition(int row, int col)
{
    Board& board = m_boardView->getBoard();


    if (m_boardView->getIsMaxSize()) {
        if (row >= 0 && row < board.getRowSize() && col >= 0 && col < board.getColumnSize())
        {
            return true;
        }
        QString message = "Cannot expand board - maximum size reached!\n";
        message += "Current board size: " + QString::number(board.getRowSize()) + "x" + QString::number(board.getColumnSize());
        message += "\nMaximum allowed size: " + QString::number(m_boardView->getMaxSize()) + "x" + QString::number(m_boardView->getMaxSize());
        QMessageBox::warning(this, "Board Expansion Failed", message);
        return false;
    }

    bool expanded = false;
    QString expansionMessage;

    if (row < 0 && board.getRowSize() < m_boardView->getMaxSize())
    {
        board.expandRow(Board::RowExpandDirection::Up);
        expanded = true;
        expansionMessage += "Expanded board upwards. ";
    }
    else if (row >= board.getRowSize() && board.getRowSize() < m_boardView->getMaxSize())
    {
        board.expandRow(Board::RowExpandDirection::Down);
        expanded = true;
        qDebug() << "Expanded board downwards.";
    }
    else if (row < 0 || row >= board.getRowSize())
    {
        QMessageBox::warning(this, "Invalid Position",
            "Cannot place card at this position!\n"
            "Row expansion limit reached: " + QString::number(m_boardView->getMaxSize()));
        return false;
    }

    if (col < 0 && board.getColumnSize() < m_boardView->getMaxSize())
    {
        board.expandColumn(Board::ColumnExpandDirection::Left);
        expanded = true;
        expansionMessage += "Expanded board to the left. ";
    }
    else if (col >= board.getColumnSize() && board.getColumnSize() < m_boardView->getMaxSize())
    {
        board.expandColumn(Board::ColumnExpandDirection::Right);
        expanded = true;
        expansionMessage += "Expanded board to the right. ";
    }
    else if (col < 0 || col >= board.getColumnSize())
    {
        QMessageBox::warning(this, "Invalid Position",
            "Cannot place card at this position!\n"
            "Column expansion limit reached: " + QString::number(m_boardView->getMaxSize()));
        return false;
    }

    if (expanded) {
        if (board.getRowSize() >= m_boardView->getMaxSize() && board.getColumnSize() >= m_boardView->getMaxSize()) {
            m_boardView->setIsMaxSize(true);
            QMessageBox::information(this, "Board Size",
                "Board has reached maximum size: " + QString::number(m_boardView->getMaxSize()) + "x" + QString::number(m_boardView->getMaxSize()) + "\n"
                "No further expansion possible.");
        }
    }

    return expanded;
}


void SecondaryWindow::optimizeBoard()
{
    Board& board = m_boardView->getBoard();


    bool boardPhysicallyFull = (board.getRowSize() >= m_boardView->getMaxSize() &&
        board.getColumnSize() >= m_boardView->getMaxSize());

    if (boardPhysicallyFull && !m_boardView->getIsMaxSize()) {
        m_boardView->setIsMaxSize(true);
        qDebug() << "Board reached maximum physical size - enabling optimization";
    }


    if (m_boardView->getIsMaxSize()) {
        bool hasOptimized = false;


        if (board.isFirstRowEmpty() && !board.isLastRowEmpty()) {

            bool firstRowUseless = true;
            for (int j = 0; j < board.getColumnSize(); ++j) {
                if (board.canBePlaced(0, j)) {
                    firstRowUseless = false;
                    break;
                }
            }

            if (firstRowUseless) {
                board.removeRow(0);
                qDebug() << "Removed first empty row";
                hasOptimized = true;
            }
        }


        if (board.isLastRowEmpty() && !board.isFirstRowEmpty()) {
            int lastRow = board.getRowSize() - 1;
            bool lastRowUseless = true;
            for (int j = 0; j < board.getColumnSize(); ++j) {
                if (board.canBePlaced(lastRow, j)) {
                    lastRowUseless = false;
                    break;
                }
            }

            if (lastRowUseless) {
                board.removeRow(board.getRowSize() - 1);
                qDebug() << "Removed last empty row";
                hasOptimized = true;
            }
        }


        if (board.isFirstColumnEmpty() && !board.isLastColumnEmpty()) {
            bool firstColumnUseless = true;
            for (int i = 0; i < board.getRowSize(); ++i) {
                if (board.canBePlaced(i, 0)) {
                    firstColumnUseless = false;
                    break;
                }
            }

            if (firstColumnUseless) {
                board.removeColumn(0);
                qDebug() << "Removed first empty column";
                hasOptimized = true;
            }
        }

        if (board.isLastColumnEmpty() && !board.isFirstColumnEmpty()) {
            int lastCol = board.getColumnSize() - 1;
            bool lastColumnUseless = true;
            for (int i = 0; i < board.getRowSize(); ++i) {
                if (board.canBePlaced(i, lastCol)) {
                    lastColumnUseless = false;
                    break;
                }
            }

            if (lastColumnUseless) {
                board.removeColumn(board.getColumnSize() - 1);
                qDebug() << "Removed last empty column";
                hasOptimized = true;
            }
        }

        if (hasOptimized) {
            qDebug() << "Board optimized - new size:" << board.getRowSize() << "x" << board.getColumnSize();
        }
    }
}


void SecondaryWindow::updateBoardView()
{
    m_boardView->updateView();
}

void SecondaryWindow::resetView()
{
    m_boardView->setIsMaxSize(false);
}





void SecondaryWindow::onCardSelected(const SimpleCard& card) {
    if (currentPlayer == Color::Red && card.getColor() != Color::Red) {
        qDebug() << "Player 1 can only select red cards!";
        return;
    }
    if (currentPlayer == Color::Blue && card.getColor() != Color::Blue) {
        qDebug() << "Player 2 can only select blue cards!";
        return;
    }

    bool canPlayAsIllusion = false;
    if (game->areIllusionsEnabled()) {
        if (currentPlayer == Color::Red && !game->isPlayer1IllusionUsed()) {
            canPlayAsIllusion = true;
        }
        else if (currentPlayer == Color::Blue && !game->isPlayer2IllusionUsed()) {
            canPlayAsIllusion = true;
        }
    }

    bool playAsIllusion = false;
    if (canPlayAsIllusion && card.getValue() != 5) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Play as Illusion?",
            "Do you want to play this card as an illusion?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );
        playAsIllusion = (reply == QMessageBox::Yes);
    }

    selectedCard = card;
    selectedCardAsIllusion = playAsIllusion;

    qDebug() << "Card selected: Color ="
        << (card.getColor() == Color::Red ? "Red" : "Blue")
        << ", Value =" << card.getValue()
        << ", As Illusion =" << playAsIllusion;
}




#pragma endregion


#pragma region Mage and Powers

void SecondaryWindow::updateMageButtons() {
    if (mage1Button && game->isPlayer1MageUsed()) {
        mage1Button->setEnabled(false);
        mage1Button->setStyleSheet("background-color: rgba(128, 128, 128, 0.7); border: none; opacity: 0.5;");
    }
    if (mage2Button && game->isPlayer2MageUsed()) {
        mage2Button->setEnabled(false);
        mage2Button->setStyleSheet("background-color: rgba(128, 128, 128, 0.7); border: none; opacity: 0.5;");
    }
}

void SecondaryWindow::updatePowerButtons() {
    if (power1Button && game->isPlayer1PowerUsed()) {
        power1Button->setEnabled(false);
        power1Button->setStyleSheet("background-color: rgba(128, 128, 128, 0.7); border: none; opacity: 0.5;");
    }
    if (power2Button && game->isPlayer2PowerUsed()) {
        power2Button->setEnabled(false);
        power2Button->setStyleSheet("background-color: rgba(128, 128, 128, 0.7); border: none; opacity: 0.5;");
    }
}
void SecondaryWindow::onMageClicked(const QString& mageName, const Color& color)
{
    qDebug() << "Mage clicked:" << mageName;

    if (currentPlayer != color)
    {
        QMessageBox::information(this, "Invalid Mage", QString("Only Player %1 can use their mage!")
            .arg(currentPlayer == Color::Red ? " 2 (Blue)" : "1 (Red)"));
        return;
    }
    if (color == Color::Red && game->isPlayer1MageUsed()) {
        QMessageBox::information(this, "Mage Already Used", "Player 1 has already used their mage power this game!");
        return;
    }
    if (color == Color::Blue && game->isPlayer2MageUsed()) {
        QMessageBox::information(this, "Mage Already Used", "Player 2 has already used their mage power this game!");
        return;
    }

    if (m_boardView->getBoard().getSize() < m_boardView->getMaxSize())
    {
        QMessageBox::information(this, "Mage Clicked", "You cannot use a mage power if the board is not defined yet!");
        return;
    }

    Mages mage = fromQStringToMages(mageName);
    bool mageWasUsed = false;

    switch (mage) {
    case Mages::AirMageVelora: {
        bool returnedValue;
        do
        {
            bool ok;


            int startRow = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
            if (!ok) break;

            int startCol = QInputDialog::getInt(this, "Input Start Column", "Enter start column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
            if (!ok) break;


            int endRow = QInputDialog::getInt(this, "Input End Row", "Enter end row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
            if (!ok) break;

            int endCol = QInputDialog::getInt(this, "Input End Column", "Enter end column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
            if (!ok) break;

            Board testBoard = m_boardView->getBoard();
            if (!testBoard[{startRow, startCol}].empty() && testBoard[{startRow, startCol}].back().getColor() == color && testBoard[{endRow, endCol}].empty())
            {
                testBoard.moveSpace(startRow, startCol, endRow, endCol);
                if (game->wouldMageCreateIsolatedCards(testBoard)) {
                    QMessageBox::warning(this, "Invalid Mage Move",
                        "This move would create isolated cards! Choose different positions.");
                    returnedValue = false;
                    continue;
                }
            }

            AirMageVelora AirMageVelora;
            returnedValue = AirMageVelora.playMageVelora(m_boardView->getBoard(), color, startRow, startCol, endRow, endCol);
            if (returnedValue)
            {
                if (color == Color::Red)
                {
                    game->setPlayer1MageUsed(true);
                    QMessageBox::information(this, "Mage Used", "Player 1 mage power has been used!");
                }
                else
                {
                    game->setPlayer2MageUsed(true);
                    QMessageBox::information(this, "Mage Used", "Player 2 mage power has been used!");
                }
                mageWasUsed = true;
            }
        } while (returnedValue == false);
        break;
    }
    case Mages::AirMageZephyraCrow: {
        bool ok;

        int row = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int col = QInputDialog::getInt(this, "Input Start Column", "Enter start column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        Board testBoard = m_boardView->getBoard();
        if (testBoard[{row, col}].empty())
        {
            testBoard[{row, col}].push_back(SimpleCard(5, color));
            if (game->wouldMageCreateIsolatedCards(testBoard))
            {
                QMessageBox::warning(this, "Invalid Mage Move", "Placing Eter here would create isolated cards! Choose a different position.");
                break;
            }
        }

        AirMageZephyraCrow AirMageZephyraCrow;
        AirMageZephyraCrow.playMageZephyraCrow(m_boardView->getBoard(), color, row, col);

        if (color == Color::Red)
        {
            game->setPlayer1MageUsed(true);
            QMessageBox::information(this, "Mage Used", "Player 1 mage power has been used!");
        }
        else
        {
            game->setPlayer2MageUsed(true);
            QMessageBox::information(this, "Mage Used", "Player 2 mage power has been used!");
        }
        mageWasUsed = true;
        break;
    }
    case Mages::EarthMageBumbleroot: {


        bool ok;


        int row = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int col = QInputDialog::getInt(this, "Input Start Column", "Enter start column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        Board testBoard = m_boardView->getBoard();
        if (testBoard[{row, col}].empty())
        {
            testBoard[{row, col}].push_back({ 6, Color::Hole });
            if (game->wouldMageCreateIsolatedCards(testBoard))
            {
                QMessageBox::warning(this, "Invalid Mage Move", "Creating a hole here would create isolated cards! Choose a different position.");
                break;
            }
        }

        EarthMageBumbleroot EarthMageBumbleroot;
        EarthMageBumbleroot.playMageBumbleroot(m_boardView->getBoard(), row, col);

        if (color == Color::Red)
        {
            game->setPlayer1MageUsed(true);
            QMessageBox::information(this, "Mage Used", "Player 1 mage power has been used!");
        }
        else
        {
            game->setPlayer2MageUsed(true);
            QMessageBox::information(this, "Mage Used", "Player 2 mage power has been used!");
        }
        mageWasUsed = true;

        break;
    }
    case Mages::EarthMageElderbranch:
    {
        bool ok;
        int16_t row = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int col = QInputDialog::getInt(this, "Input Target Column", "Enter target column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (m_boardView->getBoard()[{row, col}].empty())
        {
            QMessageBox::warning(this, "Invalid Target", "Target position must have a card!");
            break;
        }

        int value = QInputDialog::getInt(this, "Input Card Value", "Choose card value from your hand (1-4):", 1, 1, 4, 1, &ok);
        if (!ok) break;

        bool hasCard = false;
        for (const auto& card : game->getCurrentPlayer().getVector())
        {
            if (card.getValue() == value && card.getColor() == color)
            {
                hasCard = true;
                break;
            }
        }

        if (!hasCard)
        {
            QMessageBox::warning(this, "Invalid Card", "You don't have this card in your hand!");
            break;
        }

        Board testBoard = m_boardView->getBoard();
        testBoard[{row, col}].push_back(SimpleCard(value, color));

        if (game->wouldMageCreateIsolatedCards(testBoard))
        {
            QMessageBox::warning(this, "Invalid Mage Move", "This move would create isolated cards! Choose a different position.");
            break;
        }

        EarthMageElderbranch elderbranch;
        if (elderbranch.playMageElderbranch(m_boardView->getBoard(), color, value, row, col))
        {
            SimpleCard usedCard(value, color);
            game->getCurrentPlayer().makeCardInvalid(usedCard);
            game->getCurrentPlayer().getPastVector().push_back(usedCard);

            QMessageBox::information(this, "Mage Success", "Successfully covered opponent's card!");

            if (color == Color::Red)
            {
                game->setPlayer1MageUsed(true);
                QMessageBox::information(this, "Mage Used", "Player 1 mage power has been used!");
            }
            else
            {
                game->setPlayer2MageUsed(true);
                QMessageBox::information(this, "Mage Used", "Player 2 mage power has been used!");
            }

            mageWasUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Invalid Move", "Cannot cover that card with the selected value!");
        }
        break;
    }
    case Mages::FireMageIgnara: {

        bool ok;


        int row = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int col = QInputDialog::getInt(this, "Input Start Column", "Enter start column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (m_boardView->getBoard()[{row, col}].size() < 2)
        {
            QMessageBox::warning(this, "Invalid Target", "Target position must have at least 2 cards (one covering yours)!");
            break;
        }

        auto& stack = m_boardView->getBoard()[{row, col}];
        if (stack[stack.size() - 2].getColor() != color)
        {
            QMessageBox::warning(this, "Invalid Target", "Your card is not being covered at this position!");
            break;
        }

        if (stack.back().getColor() == color)
        {
            QMessageBox::warning(this, "Invalid Target", "You cannot remove your own card!");
            break;
        }

        FireMageIgnara FireMageIgnara;
        if (FireMageIgnara.playMageIgnara(m_boardView->getBoard(), color, row, col))
        {
            QMessageBox::information(this, "Mage Success", "Successfully removed opponent's card covering yours!");
            if (color == Color::Red)
            {
                game->setPlayer1MageUsed(true);
                QMessageBox::information(this, "Mage Used", "Player 1 mage power has been used!");
            }
            else
            {
                game->setPlayer2MageUsed(true);
                QMessageBox::information(this, "Mage Used", "Player 2 mage power has been used!");
            }
            mageWasUsed = true;

        }
        else
        {
            QMessageBox::warning(this, "Invalid Move", "Cannot remove card at this position!");
        }
        break;
    }
    case Mages::FireMagePyrofang: {

        bool ok;

        QString availableOptions = "Available full rows/columns with your cards:\n";
        bool hasValidTargets = false;

        for (int i = 0; i < m_boardView->getBoard().getRowSize(); i++)
        {
            if (m_boardView->getBoard().checkRow(i))
            {
                bool hasPlayerCard = false;
                for (int j = 0; j < m_boardView->getBoard().getColumnSize(); j++)
                {
                    if (!m_boardView->getBoard()[{i, j}].empty() && m_boardView->getBoard()[{i, j}].back().getColor() == color)
                    {
                        hasPlayerCard = true;
                        break;
                    }
                }
                if (hasPlayerCard)
                {
                    availableOptions += QString("Row %1\n").arg(i);
                    hasValidTargets = true;
                }
            }
        }

        for (int j = 0; j < m_boardView->getBoard().getColumnSize(); j++)
        {
            if (m_boardView->getBoard().checkColumn(j))
            {
                bool hasPlayerCard = false;
                for (int i = 0; i < m_boardView->getBoard().getRowSize(); i++)
                {
                    if (!m_boardView->getBoard()[{i, j}].empty() && m_boardView->getBoard()[{i, j}].back().getColor() == color)
                    {
                        hasPlayerCard = true;
                        break;
                    }
                }
                if (hasPlayerCard)
                {
                    availableOptions += QString("Column %1\n").arg(j);
                    hasValidTargets = true;
                }
            }
        }
        if (!hasValidTargets)
        {
            QMessageBox::information(this, "No Valid Targets", "No full rows or columns contain your cards!");
            break;
        }

        QMessageBox::information(this, "Available Targets", availableOptions);

        QStringList items;
        items << "Row" << "Column";
        QString item = QInputDialog::getItem(this, "Select Type", "Choose row or column:", items, 0, false, &ok);
        if (!ok) break;

        bool isColumn = (item == "Column");

        int maxIndex = isColumn ? m_boardView->getBoard().getColumnSize() - 1 : m_boardView->getBoard().getRowSize() - 1;
        int index = QInputDialog::getInt(this, QString("Select %1").arg(item), QString("Enter %1 index:").arg(item.toLower()), 0, 0, maxIndex, 1, &ok);
        if (!ok) break;

        Board testBoard = m_boardView->getBoard();
        if (isColumn) {
            testBoard.emptyColumn(index);
        }
        else {
            testBoard.emptyRow(index);
        }


        if (game->wouldMageCreateIsolatedCards(testBoard))
        {
            QMessageBox::warning(this, "Invalid Mage Move", "Removing this row/column would create isolated cards!");
            break;
        }

        FireMagePyrofang pyrofang;
        if (pyrofang.playMagePyrofang(m_boardView->getBoard(), color, isColumn, index))
        {
            QMessageBox::information(this, "Mage Success", QString("Successfully removed %1 %2!").arg(item.toLower()).arg(index));

            if (color == Color::Red)
            {
                game->setPlayer1MageUsed(true);
                QMessageBox::information(this, "Mage Used", "Player 1 mage power has been used!");
            }
            else
            {
                game->setPlayer2MageUsed(true);
                QMessageBox::information(this, "Mage Used", "Player 2 mage power has been used!");
            }
            mageWasUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Invalid Move", "Cannot remove this row/column!");
        }
        break;
    }
    case Mages::WaterMageAqualon: {

        bool ok;

        QStringList items;
        items << "Row" << "Column";
        QString item = QInputDialog::getItem(this, "Select Type", "Choose row or column:", items, 0, false, &ok);
        if (!ok) break;

        bool isColumn = (item == "Column");

        int maxIndex = isColumn ? m_boardView->getBoard().getColumnSize() - 1 : m_boardView->getBoard().getRowSize() - 1;

        QStringList edgeOptions;
        edgeOptions << "0 (First)" << QString("%1 (Last)").arg(maxIndex);
        QString edgeChoice = QInputDialog::getItem(this, "Select Edge", QString("Choose which edge %1:").arg(item.toLower()), edgeOptions, 0, false, &ok);
        if (!ok) break;

        int index = edgeChoice.startsWith("0") ? 0 : maxIndex;


        WaterMageAqualon WaterMageAqualon;
        if (WaterMageAqualon.playMageAqualon(m_boardView->getBoard(), isColumn, index))
        {
            QMessageBox::information(this, "Mage Success", QString("Successfully moved %1 %2 to opposite edge!").arg(item.toLower()).arg(index));

            if (color == Color::Red)
            {
                game->setPlayer1MageUsed(true);
                QMessageBox::information(this, "Mage Used", "Player 1 mage power has been used!");
            }
            else
            {
                game->setPlayer2MageUsed(true);
                QMessageBox::information(this, "Mage Used", "Player 2 mage power has been used!");
            }

            mageWasUsed = true;
        }
        else {
            QMessageBox::warning(this, "Invalid Move", "Cannot move this row/column! Make sure it's at the edge and fully occupied.");
        }
        break;
    }
    case Mages::WaterMageChillThoughts: {

        bool ok;

        int startRow = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int startCol = QInputDialog::getInt(this, "Input Start Column", "Enter start column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        int endRow = QInputDialog::getInt(this, "Input End Row", "Enter end row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int endCol = QInputDialog::getInt(this, "Input End Column", "Enter end column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        Board testBoard = m_boardView->getBoard();
        if (!testBoard[{startRow, startCol}].empty())
        {
            testBoard.moveSpace(startRow, startCol, endRow, endCol);
            if (game->wouldMageCreateIsolatedCards(testBoard))
            {
                QMessageBox::warning(this, "Invalid Mage Move", "This move would create isolated cards! Choose different positions.");
                break;
            }
        }
        WaterMageChillThoughts WaterMageChillThoughts;
        WaterMageChillThoughts.playMageChillThoughts(m_boardView->getBoard(), color, startRow, startCol, endRow, endCol);


        if (color == Color::Red)
        {
            game->setPlayer1MageUsed(true);
            QMessageBox::information(this, "Mage Used", "Player 1 mage power has been used!");
        }
        else
        {
            game->setPlayer2MageUsed(true);
            QMessageBox::information(this, "Mage Used", "Player 2 mage power has been used!");
        }
        mageWasUsed = true;

        break;
    }
    default:
        break;
    }

    updateMageButtons();

    m_boardView->updateView();

    Game& gameInstance = Game::get_Instance();
    setPlayer1Cards(gameInstance.getPlayer1().getVector());
    setPlayer2Cards(gameInstance.getPlayer2().getVector());
    game->setPlayerMoveCompleted(true);

    if (mageWasUsed) 
    {
   
        game->setPlayerMoveCompleted(true);
    }
}

void SecondaryWindow::onPowerClicked(const QString& powerName, const Color& color)
{
    bool powerUsed = false;
    qDebug() << "Power clicked:" << powerName;

    

    if (color == Color::Red && game->isPlayer1PowerUsed())
    {
        QMessageBox::information(this, "Power Already Used", "Player 1 has already used their power this game!");
        return;
    }
    if (color == Color::Blue && game->isPlayer2PowerUsed())
    {
        QMessageBox::information(this, "Power Already Used", "Player 2 has already used their power this game!");
        return;
    }

    if (m_boardView->getBoard().getSize() < m_boardView->getMaxSize())
    {
        QMessageBox::information(this, "Power Clicked", "You cannot use a power if the board is not fully defined yet!");
        return;
    }

    Power power = fromQStringToPower(powerName);

    switch (power) {
    case Power::PowerWaterfall:
    {
        PowerWaterfall waterfallPower;
        if (!waterfallPower.checkWaterfallPower(m_boardView->getBoard()))
        {
            QMessageBox::information(this, "Power Failed", "No rows or columns have at least 3 occupied positions!");
            break;
        }

        bool ok;
        QStringList options;
        options << "Row" << "Column";
        QString choice = QInputDialog::getItem(this, "Select Type", "Choose row or column:", options, 0, false, &ok);
        if (!ok) break;

        bool isRow = (choice == "Row");
        int maxIndex = isRow ? m_boardView->getBoard().getRowSize() - 1 : m_boardView->getBoard().getColumnSize() - 1;

        int index = QInputDialog::getInt(this, QString("Select %1").arg(choice), QString("Enter %1 index:").arg(choice.toLower()), 0, 0, maxIndex, 1, &ok);
        if (!ok) break;

        int ocupiedPositions = 0;
        if (isRow) 
        {
            for (int col = 0; col < m_boardView->getBoard().getColumnSize(); col++)
            {
                if (!m_boardView->getBoard()[{index, col}].empty())
                {
                    ocupiedPositions++;
                }
            }
        }
        else {
            for (int row = 0; row < m_boardView->getBoard().getRowSize(); row++)
            {
                if (!m_boardView->getBoard()[{row, index}].empty())
                {
                    ocupiedPositions++;
                }
            }
        }

        if (ocupiedPositions < 3)
        {
            QMessageBox::warning(this, "Invalid Selection", QString("Selected %1 does not have at least 3 occupied positions!").arg(choice.toLower()));
            break;
        }

        QStringList directions;
        if (isRow)
        {
            directions << "Left" << "Right";
        }
        else 
        {
            directions << "Up" << "Down";
        }
        QString direction = QInputDialog::getItem(this, "Cascade Direction", "Choose cascade direction:", directions, 0, false, &ok);
        if (!ok) break;

        bool cascadeToFirst = (direction == "Left" || direction == "Up");

        Board testBoard = m_boardView->getBoard();
        waterfallPower.playWaterfallPower(testBoard, index, cascadeToFirst, isRow);

        if (game->wouldMageCreateIsolatedCards(testBoard))
        {
            QMessageBox::warning(this, "Invalid Move", "Waterfall would create isolated cards!");
            break;
        }

        waterfallPower.playWaterfallPower(m_boardView->getBoard(), index, cascadeToFirst, isRow);
        QMessageBox::information(this, "Power Activated", QString("Waterfall cascaded %1 %2 to the %3!").arg(choice.toLower()).arg(index).arg(direction.toLower()));

        if (color == Color::Red)
        {
            game->setPlayer1PowerUsed(true);
            QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this round!");
        }
        else
        {
            game->setPlayer2PowerUsed(true);
            QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this round!");
        }
        powerUsed = true;
        break;
    }
    case Power::PowerEarthquake: {
        PowerEarthquake earthquakePower;
        if (earthquakePower.checkEarthquakePower(m_boardView->getBoard()))
        {

            Board testBoard = m_boardView->getBoard();
            for (int i = 0; i < testBoard.getRowSize(); i++)
            {

                for (int j = 0; j < testBoard.getColumnSize(); j++) {
                    if (!testBoard[{i, j}].empty() && testBoard[{i, j}].back().getValue() == 1)
                    {
                        testBoard.popCard({ i, j });
                    }
                }
            }

            if (game->wouldMageCreateIsolatedCards(testBoard))
            {
                QMessageBox::warning(this, "Invalid Move", "Earthquake would create isolated cards!");
                break;
            }

            earthquakePower.playEarthquakePower(m_boardView->getBoard());
            QMessageBox::information(this, "Power Activated", "Earthquake has been used!");


            if (color == Color::Red)
            {
                game->setPlayer1PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
            }
            else
            {
                game->setPlayer2PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::information(this, "Power Failed", "No valid targets for Earthquake.");
        }

        break;
    }
    case Power::PowerTide: {
        bool ok;
        int x1 = QInputDialog::getInt(this, "Input Row 1", "Enter row 1:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int y1 = QInputDialog::getInt(this, "Input Column 1", "Enter column 1:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;
        int x2 = QInputDialog::getInt(this, "Input Row 2", "Enter row 2:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int y2 = QInputDialog::getInt(this, "Input Column 2", "Enter column 2:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        PowerTide tidePower;
        if (tidePower.checkTidePower(m_boardView->getBoard(), x1, y1, x2, y2))
        {

            Board testBoard = m_boardView->getBoard();
            std::swap(testBoard[{x1, y1}], testBoard[{x2, y2}]);

            if (game->wouldMageCreateIsolatedCards(testBoard))
            {
                QMessageBox::warning(this, "Invalid Move", "Tide would create isolated cards!");
                break;
            }

            tidePower.playTidePower(m_boardView->getBoard(), x1, y1, x2, y2);
            QMessageBox::information(this, "Power Activated", "Tide has swapped two stacks!");

            if (color == Color::Red)
            {
                game->setPlayer1PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
            }
            else
            {
                game->setPlayer2PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::information(this, "Power Failed", "Invalid stacks for Tide.");

        }
        break;
    }
    case Power::PowerStorm: {
        bool ok;
        int x = QInputDialog::getInt(this, "Input Row", "Enter row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int y = QInputDialog::getInt(this, "Input Column", "Enter column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        PowerStorm stormPower;
        if (stormPower.checkStormPower(m_boardView->getBoard(), x, y))
        {
            Board testBoard = m_boardView->getBoard();
            testBoard[{x, y}].clear();

            if (game->wouldMageCreateIsolatedCards(testBoard))
            {
                QMessageBox::warning(this, "Invalid Move", "Storm would create isolated cards!");
                break;
            }

            stormPower.playStormPower(m_boardView->getBoard(), x, y);
            QMessageBox::information(this, "Power Activated", "Storm has cleared the stack!");


            if (color == Color::Red)
            {
                game->setPlayer1PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
            }
            else
            {
                game->setPlayer2PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
            }
            powerUsed = true;
        }
        else {
            QMessageBox::information(this, "Power Failed", "Invalid stack for Storm.");
        }
        break;
    }
    case Power::PowerSquall: {
        bool ok;
        int x = QInputDialog::getInt(this, "Input Row", "Enter row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int y = QInputDialog::getInt(this, "Input Column", "Enter column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        PowerSquall squallPower;
        std::string currentPlayerColor = game->getCurrentPlayer().GetVectorColor();

        if (!squallPower.checkSquallPower(m_boardView->getBoard(), currentPlayerColor, x, y))
        {
            QMessageBox::warning(this, "Invalid Target", "Cannot use Squall on this position! Must target opponent's visible card.");
            break;
        }

        Board testBoard = m_boardView->getBoard();
        testBoard.popCard({ x, y });

        if (game->wouldMageCreateIsolatedCards(testBoard))
        {
            QMessageBox::warning(this, "Invalid Move", "This power would create isolated cards! Choose a different target.");
            break;
        }

        squallPower.playSquallPower(m_boardView->getBoard(), game->getPlayer1(), game->getPlayer2(), currentPlayerColor, x, y);
        QMessageBox::information(this, "Power Activated", "Squall returned opponent's card to their hand!");

        if (color == Color::Red)
        {
            game->setPlayer1PowerUsed(true);
            QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
        }
        else
        {
            game->setPlayer2PowerUsed(true);
            QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
        }

        powerUsed = true;
        break;
    }
    case Power::PowerGale: {
        PowerGale galePower;
        galePower.playGalePower(m_boardView->getBoard(), game->getPlayer1(), game->getPlayer2());
        QMessageBox::information(this, "Power Activated", "Gale has removed covered cards!");

        if (color == Color::Red)
        {
            game->setPlayer1PowerUsed(true);
            QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
        }
        else
        {
            game->setPlayer2PowerUsed(true);
            QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
        }
        powerUsed = true;
        break;
    }
    case Power::PowerWave:
    {
        if (!selectedCard.getValue())
        {
            QMessageBox::warning(this, "No Card Selected", "Please select a card first!");
            break;
        }

        PowerWave wavePower;
        bool ok;

        int sourceX = QInputDialog::getInt(this, "Source Row", "Enter source row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int sourceY = QInputDialog::getInt(this, "Source Column", "Enter source column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (!wavePower.checkWavePower(m_boardView->getBoard(), game->getCurrentPlayer(), sourceX, sourceY))
        {
            QMessageBox::warning(this, "Invalid Source", "Cannot use Wave on this position!");
            break;
        }

        int targetX = QInputDialog::getInt(this, "Target Row", "Enter target row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int targetY = QInputDialog::getInt(this, "Target Column", "Enter target column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (m_boardView->getBoard()[{targetX, targetY}].empty() && abs(targetX - sourceX) <= 1 && abs(targetY - sourceY) <= 1)
        {

            Board testBoard = m_boardView->getBoard();
            testBoard.moveSpace(sourceX, sourceY, targetX, targetY);
            testBoard.pushCard(selectedCard, { sourceX, sourceY });

            if (!game->wouldMageCreateIsolatedCards(testBoard))
            {
                wavePower.playWavePower(m_boardView->getBoard(), game->getCurrentPlayer(),
                    sourceX, sourceY, targetX, targetY, selectedCard);


                setPlayer1Cards(game->getPlayer1().getVector());
                setPlayer2Cards(game->getPlayer2().getVector());
                selectedCard = SimpleCard();

                QMessageBox::information(this, "Power Activated", "Wave completed!");

                if (color == Color::Red)
                {
                    game->setPlayer1PowerUsed(true);
                    QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");

                }
                else
                {
                    game->setPlayer2PowerUsed(true);
                    QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");

                }
                powerUsed = true;
            }
            else
            {
                QMessageBox::warning(this, "Invalid Move", "This move would create isolated cards! Choose a different target.");
            }
        }
        else
        {
            QMessageBox::warning(this, "Invalid Target", "Target must be adjacent and empty!");
        }
        break;
    }
    case Power::PowerGust:
    {
        PowerGust gustPower;

        bool ok;
        int sourceX = QInputDialog::getInt(this, "Source Row", "Enter source row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int sourceY = QInputDialog::getInt(this, "Source Column", "Enter source column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (!gustPower.checkGustPower(m_boardView->getBoard(), sourceX, sourceY))
        {
            QMessageBox::warning(this, "Invalid Source", "Cannot use Gust on this position!");
            break;
        }

        int targetX = QInputDialog::getInt(this, "Target Row", "Enter target row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int targetY = QInputDialog::getInt(this, "Target Column", "Enter target column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        bool validTarget = false;
        if (abs(targetX - sourceX) + abs(targetY - sourceY) == 1)
        {
            if (!m_boardView->getBoard()[{targetX, targetY}].empty())
            {
                int sourceValue = m_boardView->getBoard()[{sourceX, sourceY}].back().getValue();
                int targetValue = m_boardView->getBoard()[{targetX, targetY}].back().getValue();
                if (targetValue < sourceValue)
                {
                    validTarget = true;
                }
            }
        }

        if (!validTarget)
        {
            QMessageBox::warning(this, "Invalid Target", "Target must be adjacent horizontally/vertically with a lower value card!");
            break;
        }

        Board testBoard = m_boardView->getBoard();
        gustPower.playGustPower(testBoard, sourceX, sourceY, targetX, targetY);

        if (!game->wouldMageCreateIsolatedCards(testBoard))
        {

            gustPower.playGustPower(m_boardView->getBoard(), sourceX, sourceY, targetX, targetY);
            QMessageBox::information(this, "Power Activated", "Gust moved the card!");


            if (color == Color::Red)
            {
                game->setPlayer1PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");

            }
            else
            {
                game->setPlayer2PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");

            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Invalid Move", "Gust would create isolated cards!");
        }
        break;
    }
    case Power::PowerWhirlpool:
    {
        PowerWhirlpool whirlpoolPower;

        bool ok;
        int row = QInputDialog::getInt(this, "Row", "Enter row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int col1 = QInputDialog::getInt(this, "Column 1", "Enter first column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        int col2 = QInputDialog::getInt(this, "Column 2", "Enter second column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (!whirlpoolPower.checkWhirlpoolPower(m_boardView->getBoard(), row, col1, col2))
        {
            QMessageBox::warning(this, "Invalid", "Need two individual cards separated by empty space!");
            break;
        }

        Board testBoard = m_boardView->getBoard();
        whirlpoolPower.playWhirlpoolPower(testBoard, row, col1, col2);

        if (!game->wouldMageCreateIsolatedCards(testBoard))
        {
            whirlpoolPower.playWhirlpoolPower(m_boardView->getBoard(), row, col1, col2);
            QMessageBox::information(this, "Power Activated", "Whirlpool combined the cards!");

            if (color == Color::Red)
            {
                game->setPlayer1PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");

            }
            else
            {
                game->setPlayer2PowerUsed(true);
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");

            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Invalid Move", "Would create isolated cards!");
        }
        break;
    }
    case Power::PowerAsh:
    {
        PowerAsh ashPower;

        std::vector<int16_t> eliminatedValues = ashPower.getEliminatedCardValues(m_boardView->getBoard(), game->getCurrentPlayer());

        if (eliminatedValues.empty()) {
            QMessageBox::warning(this, "Power Failed", "No eliminated cards found! You need cards that were removed from play.");
            break;
        }


        QStringList cardOptions;
        for (int16_t value : eliminatedValues)
        {
            cardOptions << QString("Value %1").arg(value);
        }

        bool ok;
        QString selectedOption = QInputDialog::getItem(this, "Select Card", "Choose eliminated card value to replay:", cardOptions, 0, false, &ok);
        if (!ok) break;

        int16_t cardValue = selectedOption.split(" ")[1].toInt();

        int x = QInputDialog::getInt(this, "Place Card Row", "Enter row to place card:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int y = QInputDialog::getInt(this, "Place Card Column", "Enter column to place card:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (ashPower.playAshPower(m_boardView->getBoard(), game->getCurrentPlayer(), cardValue, x, y))
        {
            QMessageBox::information(this, "Power Activated", QString("Ash used! Card with value %1 restored and placed at (%2,%3)!").arg(cardValue).arg(x).arg(y));

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else {
            QMessageBox::warning(this, "Power Failed", "Cannot use Ash! Check card value and position.");
        }
        break;
    }
    case Power::PowerBlizzard:
    {
        PowerBlizzard blizzardPower;

        if (!blizzardPower.checkBlizzardPower(m_boardView->getBoard()))
        {
            QMessageBox::information(this, "Power Failed", "Opponent has no valid moves available!");
            break;
        }

        bool ok;
        QStringList options;
        options << "Row" << "Column";
        QString choice = QInputDialog::getItem(this, "Tsunami Target",
            "Choose row or column to restrict:", options, 0, false, &ok);
        if (!ok) break;

        bool isRow = (choice == "Row");
        int maxIndex = isRow ? m_boardView->getBoard().getRowSize() - 1 :
            m_boardView->getBoard().getColumnSize() - 1;

        int index = QInputDialog::getInt(this, "Select Index", QString("Enter %1 index:").arg(choice.toLower()), 0, 0, maxIndex, 1, &ok);
        if (!ok) break;

        blizzardPower.playBlizzardPower(*game, isRow, index);

        QMessageBox::information(this, "Power Activated", QString("Tsunami restricts %1 %2! Opponent cannot play there next turn.").arg(choice.toLower()).arg(index));


        if (color == Color::Red)
        {
            QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
            game->setPlayer1PowerUsed(true);
        }
        else
        {
            QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
            game->setPlayer2PowerUsed(true);
        }
        powerUsed = true;
        break;
    }
    case Power::PowerMist:
    {
        PowerMist mistPower;

        if (!mistPower.checkMistPower(m_boardView->getBoard(), game->getCurrentPlayer()))
        {
            QMessageBox::warning(this, "Power Failed", "You already have an illusion on the board! Cannot place another one.");
            break;
        }

        if (!selectedCard.getValue())
        {
            QMessageBox::warning(this, "No Card Selected", "Please select a card first!");
            break;
        }

        bool ok;
        int x = QInputDialog::getInt(this, "Row", "Enter row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int y = QInputDialog::getInt(this, "Column", "Enter column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (mistPower.playMistPower(m_boardView->getBoard(), game->getCurrentPlayer(), selectedCard, x, y))
        {
            selectedCard = SimpleCard();
            QMessageBox::information(this, "Power Activated", "Mist illusion placed!");

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot place illusion! Check position and card validity.");
        }

        break;
    }
    case Power::PowerMirage:
    {
        PowerMirage miragePower;

        if (!miragePower.checkMiragePower(m_boardView->getBoard(), game->getCurrentPlayer()))
        {
            QMessageBox::warning(this, "Power Failed", "You don't have any illusions on the board to replace!");
            break;
        }

        if (!selectedCard.getValue())
        {
            QMessageBox::warning(this, "No Card Selected", "Please select a card first!");
            break;
        }

        bool ok;
        int x = QInputDialog::getInt(this, "Illusion Row", "Enter row of your illusion:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int y = QInputDialog::getInt(this, "Illusion Column", "Enter column of your illusion:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (miragePower.playMiragePower(m_boardView->getBoard(), game->getCurrentPlayer(),
            selectedCard, x, y))
        {
            selectedCard = SimpleCard();
            QMessageBox::information(this, "Power Activated", "Mirage used! Illusion has been replaced!");

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot replace illusion! Check if you selected your own illusion and a valid card.");
        }

        break;
    }
    case Power::PowerRock:
    {
        PowerRock rockPower;

        if (!selectedCard.getValue())
        {
            QMessageBox::warning(this, "No Card Selected", "Please select a card first!");
            break;
        }

        bool ok;
        int x = QInputDialog::getInt(this, "Target Row", "Enter row of illusion to cover:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;

        int y = QInputDialog::getInt(this, "Target Column", "Enter column of illusion to cover:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (!rockPower.checkRockPower(m_boardView->getBoard(), x, y))
        {
            QMessageBox::warning(this, "Invalid Target", "No illusion found at this position!");
            break;
        }

        if (rockPower.playRockPower(m_boardView->getBoard(), game->getCurrentPlayer(), selectedCard, x, y))
        {
            selectedCard = SimpleCard();
            QMessageBox::information(this, "Power Activated", QString("Rock used! Illusion at (%1,%2) covered without revealing its value!").arg(x).arg(y));

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot cover illusion! Your card value must be higher than the illusion value.");
        }

        break;
    }
    case Power::PowerSupport:
    {
        bool ok;
        int x = QInputDialog::getInt(this, "Support Target Row", "Enter row of your card to support:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int y = QInputDialog::getInt(this, "Support Target Column", "Enter column of your card to support:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        PowerSupport supportPower;
        if (supportPower.playSupportPower(m_boardView->getBoard(), color, x, y))
        {
            QMessageBox::information(this, "Power Activated", QString("Support used! Card at (%1,%2) value increased by +1!").arg(x).arg(y));

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot support this card! Make sure it's your card, not value 4, and not already supported.");
        }
        break;
    }
    case Power::PowerCrumble:
    {
        bool ok;
        int x = QInputDialog::getInt(this, "Crumble Target Row", "Enter row of opponent's card to weaken:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int y = QInputDialog::getInt(this, "Crumble Target Column", "Enter column of opponent's card to weaken:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        PowerCrumble crumblePower;
        if (crumblePower.playCrumblePower(m_boardView->getBoard(), color, x, y))
        {
            QMessageBox::information(this, "Power Activated", QString("Crumble used! Opponent's card at (%1,%2) value decreased by -1!").arg(x).arg(y));

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot weaken this card! Make sure it's opponent's card, not value 1, and not already weakened.");
        }
        break;
    }
    case Power::PowerControlledExplosion:
    {
        PowerControlledExplosion explosionPower;

        if (!explosionPower.checkControlledExplosionPower(*game))
        {
            QMessageBox::warning(this, "Power Failed", "Cannot trigger explosion! Make sure explosions are enabled and board is large enough.");
            break;
        }

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Controlled Explosion", "Are you sure you want to trigger a controlled explosion?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            if (explosionPower.playControlledExplosionPower(*game))
            {
                QMessageBox::information(this, "Power Activated", "Controlled Explosion triggered! Choose your explosion pattern.");

                if (color == Color::Red)
                {

                    QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                    game->setPlayer1PowerUsed(true);
                }
                else
                {

                    QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                    game->setPlayer2PowerUsed(true);
                }
                powerUsed = true;
            }
            else
            {
                QMessageBox::warning(this, "Power Failed", "Failed to trigger explosion.");
            }
        }
        break;
    }
    case Power::PowerDestruction:
    {
        PowerDestruction destructionPower;
        Player& opponent = (color == Color::Red) ? game->getPlayer2() : game->getPlayer1();

        if (destructionPower.playDestructionPower(m_boardView->getBoard(), opponent))
        {
            QMessageBox::information(this, "Power Activated", "Destruction used! Opponent's last played card has been destroyed!");

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot use Destruction! No opponent cards to destroy.");
        }
        break;
    }
    case Power::PowerFlame:
    {
        PowerFlame flamePower;

        if (!flamePower.checkFlamePower(m_boardView->getBoard(), color))
        {
            QMessageBox::warning(this, "Power Failed", "No opponent illusions found on the board!");
            break;
        }

        if (!selectedCard.getValue())
        {
            QMessageBox::warning(this, "No Card Selected", "Please select a card first!");
            break;
        }

        bool ok;
        int placeX = QInputDialog::getInt(this, "Place Card Row", "Enter row to place your card:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int placeY = QInputDialog::getInt(this, "Place Card Column", "Enter column to place your card:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (flamePower.playFlamePower(m_boardView->getBoard(), color, placeX, placeY, selectedCard))
        {

            game->getCurrentPlayer().makeCardInvalid(selectedCard);
            game->getCurrentPlayer().getPastVector().push_back(selectedCard);
            selectedCard = SimpleCard();

            QMessageBox::information(this, "Power Activated", QString("Flame used! Opponent's illusion revealed and card placed at (%1,%2)!").arg(placeX).arg(placeY));

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot use Flame power!");
        }

        break;
    }
    case Power::PowerFire:
    {
        bool ok;
        int value = QInputDialog::getInt(this, "Fire Power", "Enter card value to return (1-4):", 1, 1, 4, 1, &ok);
        if (!ok) break;

        PowerFire firePower;
        if (firePower.playFirePower(m_boardView->getBoard(), game->getPlayer1(), game->getPlayer2(), value))
        {
            QMessageBox::information(this, "Power Activated", QString("Fire used! All visible cards with value %1 returned to owners!").arg(value));
            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", QString("Cannot use Fire! Need at least 2 cards with value %1 on the board.").arg(value));
        }

        break;
    }
    case Power::PowerSpark:
    {
        PowerSpark sparkPower;

        if (!sparkPower.checkSparkPower(m_boardView->getBoard(), color))
        {
            QMessageBox::warning(this, "Power Failed", "No covered cards found! You need cards covered by opponent's cards.");
            break;
        }

        bool ok;
        int sourceX = QInputDialog::getInt(this, "Source Row", "Enter row of stack with your covered card:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int sourceY = QInputDialog::getInt(this, "Source Column", "Enter column of stack with your covered card:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        int cardIndex = QInputDialog::getInt(this, "Card Index", "Enter index of your card in stack (0 = bottom):", 0, 0, 10, 1, &ok);
        if (!ok) break;

        int destX = QInputDialog::getInt(this, "Destination Row", "Enter row to place your card:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int destY = QInputDialog::getInt(this, "Destination Column", "Enter column to place your card:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        if (sparkPower.playSparkPower(m_boardView->getBoard(), color, sourceX, sourceY, cardIndex, destX, destY)) {
            QMessageBox::information(this, "Power Activated", QString("Spark used! Card moved from (%1,%2) to (%3,%4)!").arg(sourceX).arg(sourceY).arg(destX).arg(destY));

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot move card! Check positions and make sure it's your covered card.");
        }
        break;
    }
    case Power::PowerHurricane:
    {

        PowerHurricane hurricanePower;

        if (!hurricanePower.checkHurricanePower(m_boardView->getBoard()))
        {
            QMessageBox::warning(this, "Power Failed", "No complete rows or columns found!");
            break;
        }

        bool ok;
        QStringList options;
        options << "Row" << "Column";
        QString choice = QInputDialog::getItem(this, "Hurricane Target", "Move row or column?", options, 0, false, &ok);
        if (!ok) break;

        bool isRow = (choice == "Row");
        int maxIndex = isRow ? m_boardView->getBoard().getRowSize() - 1 : m_boardView->getBoard().getColumnSize() - 1;

        int index = QInputDialog::getInt(this, QString("Select %1").arg(choice), QString("Enter %1 index to move:").arg(choice.toLower()), 0, 0, maxIndex, 1, &ok);
        if (!ok) break;

        QStringList directions;
        directions << "Left" << "Right" << "Up" << "Down";

        QString dirChoice = QInputDialog::getItem(this, "Hurricane Direction", "Choose direction:", directions, 0, false, &ok);
        if (!ok) break;

        int direction = directions.indexOf(dirChoice);

        if (hurricanePower.playHurricanePower(m_boardView->getBoard(), game->getPlayer1(), game->getPlayer2(), isRow, index, direction))
        {
            QMessageBox::information(this, "Power Activated", QString("Hurricane used! %1 %2 moved %3 and cards returned to owners!").arg(choice).arg(index).arg(dirChoice.toLower()));

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot use Hurricane! Make sure the selected row/column is complete.");
        }
        break;
    }
    case Power::PowerAvalanche:
    {
        bool ok;
        int x1 = QInputDialog::getInt(this, "First Card Row", "Enter row of first card:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int y1 = QInputDialog::getInt(this, "First Card Column", "Enter column of first card:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;

        int x2 = QInputDialog::getInt(this, "Second Card Row", "Enter row of second card:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break;
        int y2 = QInputDialog::getInt(this, "Second Card Column", "Enter column of second card:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break;


        QStringList directions;
        if (x1 == x2)
        {
            directions << "Left" << "Right";
        }
        else if (y1 == y2)
        {
            directions << "Up" << "Down";
        }
        else
        {
            QMessageBox::warning(this, "Invalid Selection", "Cards must be on same row or column!");
            break;
        }

        QString dirChoice = QInputDialog::getItem(this, "Avalanche Direction", "Choose direction to shift:", directions, 0, false, &ok);
        if (!ok) break;


        int direction;
        if (dirChoice == "Up")
            direction = 0;
        else
            if (dirChoice == "Down")
                direction = 1;
            else
                if (dirChoice == "Left")
                    direction = 2;
                else
                    direction = 3;

        PowerAvalanche avalanchePower;
        if (avalanchePower.playAvalanchePower(m_boardView->getBoard(), x1, y1, x2, y2, direction))
        {
            QMessageBox::information(this, "Power Activated", QString("Avalanche used! Cards shifted %1!").arg(dirChoice.toLower()));

            if (color == Color::Red)
            {
                QMessageBox::information(this, "Power Used", "Player 1 power has been used and cannot be used again this game!");
                game->setPlayer1PowerUsed(true);
            }
            else
            {
                QMessageBox::information(this, "Power Used", "Player 2 power has been used and cannot be used again this game!");
                game->setPlayer2PowerUsed(true);
            }
            powerUsed = true;
        }
        else
        {
            QMessageBox::warning(this, "Power Failed", "Cannot use Avalanche! Make sure cards are neighbors and can shift to valid positions.");
        }
        break;
    }
    case Power::PowerBorder:
    {
        break;
    }
    default:
        QMessageBox::information(this, "Power Clicked", "This power is not implemented yet!");
        break;
    }

    if (powerUsed)
    {
        qDebug() << "Power turn completed - switching players";
        game->setPlayerMoveCompleted(true);
    }
    updatePowerButtons();

    m_boardView->updateView();
    Game& gameInstance = Game::get_Instance();
    setPlayer1Cards(gameInstance.getPlayer1().getVector());
    setPlayer2Cards(gameInstance.getPlayer2().getVector());
}



#pragma endregion

