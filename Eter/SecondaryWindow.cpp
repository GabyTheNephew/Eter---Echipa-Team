#include "SecondaryWindow.h"

void SecondaryWindow::clearCardSelection() {
    selectedCardIndex = -1;
    selectedCard = SimpleCard();
    selectedCardPlayer = Color::Red;

    if (!game) {
        qDebug() << "ERROR: Game instance is null in clearCardSelection";
        return;
    }

    game->clearSelectedCard();

    try {
        if (currentPlayer == Color::Red) {
            const auto& playerVector = game->getCurrentPlayer().getVector();
            setPlayer1Cards(playerVector);
        }
        else {
            const auto& playerVector = game->getCurrentPlayer().getVector();
            setPlayer2Cards(playerVector);
        }
    }
    catch (const std::exception& e) {
        qDebug() << "ERROR: Exception in clearCardSelection: " << e.what();
    }
    catch (...) {
        qDebug() << "ERROR: Unknown exception in clearCardSelection";
    }

    qDebug() << "Card selection cleared in SecondaryWindow AND Game";
}
void SecondaryWindow::showWinner(const QString& winnerName) {
    static bool winnerShown = false;
    if (winnerShown) {
        qDebug() << "Winner already shown, ignoring duplicate call";
        return;
    }
    winnerShown = true;

    qDebug() << "=== SHOWING WINNER DIALOG ===";
    qDebug() << "Winner:" << winnerName;

    this->blockSignals(true);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Meci Terminat");
    msgBox.setText("Câștigătorul este: " + winnerName);
    msgBox.setInformativeText("Felicitări!");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setModal(true);

    msgBox.setStyleSheet(
        "QMessageBox { "
        "   background-color: #2b2b2b; "
        "   color: white; "
        "   font-size: 16px; "
        "   min-width: 400px; "
        "   min-height: 200px; "
        "}"
        "QMessageBox QPushButton { "
        "   background-color: #4CAF50; "
        "   color: white; "
        "   border: none; "
        "   padding: 15px 30px; "
        "   font-size: 14px; "
        "   border-radius: 5px; "
        "   min-width: 80px; "
        "}"
        "QMessageBox QPushButton:hover { "
        "   background-color: #45a049; "
        "}"
    );

    int result = msgBox.exec();

    this->blockSignals(false);

    winnerShown = false;

    qDebug() << "Message box closed with result:" << result;
    qDebug() << "Game completed, emitting returnToMainMenu";

    QTimer::singleShot(100, this, [this]() {
        emit returnToMainMenu();
        });
}
void SecondaryWindow::setupMatchInfoUI() {
    matchInfoLayout = new QHBoxLayout();

    roundInfoLabel = new QLabel("Round 1", this);
    roundInfoLabel->setStyleSheet(
        "QLabel { "
        "   color: white; "
        "   font-size: 18px; "
        "   font-weight: bold; "
        "   background-color: rgba(0, 0, 0, 150); "
        "   padding: 10px; "
        "   border-radius: 10px; "
        "}"
    );
    roundInfoLabel->setAlignment(Qt::AlignCenter);

    matchInfoLabel = new QLabel("Player 1: 0 - Player 2: 0", this);
    matchInfoLabel->setStyleSheet(
        "QLabel { "
        "   color: white; "
        "   font-size: 18px; "
        "   font-weight: bold; "
        "   background-color: rgba(0, 0, 0, 150); "
        "   padding: 10px; "
        "   border-radius: 10px; "
        "}"
    );
    matchInfoLabel->setAlignment(Qt::AlignCenter);

    matchInfoLayout->addWidget(roundInfoLabel);
    matchInfoLayout->addStretch();
    matchInfoLayout->addWidget(matchInfoLabel);

    mainLayout->insertLayout(0, matchInfoLayout);
}

void SecondaryWindow::updateMatchInfo(int currentRound, int player1Score, int player2Score, int roundsToWin) {
    if (!roundInfoLabel || !matchInfoLabel) {
        return;
    }

    roundInfoLabel->setText(QString("Round %1").arg(currentRound));
    matchInfoLabel->setText(QString("Player 1: %1 - Player 2: %2 (First to %3)")
        .arg(player1Score)
        .arg(player2Score)
        .arg(roundsToWin));
}

void SecondaryWindow::showRoundWinner(const QString& winnerName, int currentRound) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Round Complete");
    msgBox.setText(QString("%1 wins Round %2!").arg(winnerName).arg(currentRound));
    msgBox.setInformativeText("Starting next round...");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setModal(true);

    msgBox.setStyleSheet(
        "QMessageBox { "
        "   background-color: #2b2b2b; "
        "   color: white; "
        "}"
        "QMessageBox QPushButton { "
        "   background-color: #4CAF50; "
        "   color: white; "
        "   border: none; "
        "   padding: 10px 20px; "
        "   font-size: 14px; "
        "   border-radius: 5px; "
        "}"
    );

    msgBox.exec();
}
void SecondaryWindow::onBoardClicked(int row, int col) {
    qDebug() << "Board clicked at (" << row << ", " << col << ")";

    if (!game) {
        qDebug() << "ERROR: No game instance!";
        return;
    }

    if (!m_boardView) {
        qDebug() << "ERROR: No board view!";
        return;
    }

    const Board& board = m_boardView->getBoard();
    if (row < 0 || row >= board.getRowSize() || col < 0 || col >= board.getColumnSize()) {
        qDebug() << "ERROR: Invalid board coordinates: (" << row << ", " << col << ")";
        return;
    }

    int playerNumber = (currentPlayer == Color::Red) ? 1 : 2;
    qDebug() << "Emitting boardClicked signal for player " << playerNumber;
    emit boardClicked(row, col, playerNumber);
}
void SecondaryWindow::setBoard(Board& board, int setMaxSize) {
    if (m_boardView) {
        qDebug() << "BoardView already exists, updating existing one instead";
        m_boardView->updateView();
        return;
    }

    try {
        m_boardView = std::make_unique<BoardView>(board, this, setMaxSize);

        if (!m_boardView) {
            qDebug() << "ERROR: Failed to create BoardView";
            return;
        }

        m_boardView->setFixedSize(350, 350);

        if (!mainLayout) {
            qDebug() << "ERROR: mainLayout is null";
            return;
        }

        mainLayout->insertWidget(1, m_boardView.get(), 0, Qt::AlignHCenter | Qt::AlignVCenter);
        connect(m_boardView.get(), &BoardView::cellClicked, this, &SecondaryWindow::onBoardClicked);

        m_boardView->updateView();

        m_boardView->setVisible(true);

        qDebug() << "Board initialized for dynamic Eter gameplay";
    }
    catch (const std::exception& e) {
        qDebug() << "ERROR: Exception in setBoard: " << e.what();
        m_boardView.reset();
    }
    catch (...) {
        qDebug() << "ERROR: Unknown exception in setBoard";
        m_boardView.reset();
    }
}
void SecondaryWindow::cleanupEmptyBorders() {
    Board& board = m_boardView->getBoard();

    qDebug() << "Cleaning up borders...";

    while (board.getRowSize() > m_boardView->getMaxSize()) {
        if (board.isFirstRowEmpty() && !board.isLastRowEmpty()) {
            board.removeRow(0);
            qDebug() << "Removed first row";
        }
        else if (board.isLastRowEmpty() && !board.isFirstRowEmpty()) {
            board.removeRow(board.getRowSize() - 1);
            qDebug() << "Removed last row";
        }
        else {
            break;
        }
    }

    while (board.getColumnSize() > m_boardView->getMaxSize()) {
        if (board.isFirstColumnEmpty() && !board.isLastColumnEmpty()) {
            board.removeColumn(0);
            qDebug() << "Removed first column";
        }
        else if (board.isLastColumnEmpty() && !board.isFirstColumnEmpty()) {
            board.removeColumn(board.getColumnSize() - 1);
            qDebug() << "Removed last column";
        }
        else {
            break;
        }
    }

    qDebug() << "Cleanup finished. Final size: "
        << board.getRowSize() << "x" << board.getColumnSize();
}
SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, Game* gameInstance,
    const QString& mage1Name, const QString& mage2Name, const QString& power1Name, const QString& power2Name,
    bool checkMage, bool checkPower, QWidget* parent)
    : QWidget(parent), imagePath(imagePath), game(gameInstance),
    selectedCardIndex(-1), selectedCardPlayer(Color::Red), currentPlayer(Color::Red),
    menu(nullptr), m_boardView(nullptr), mainLayout(nullptr),
    player1CardsLayout(nullptr), player2CardsLayout(nullptr),
    matchInfoLabel(nullptr), roundInfoLabel(nullptr), matchInfoLayout(nullptr) {

    setWindowTitle(title);

    if (!gameInstance) {
        qDebug() << "ERROR: Game instance is null in constructor!";
    }


    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    qDebug() << "Screen resolution:" << screenGeometry.size();

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    setupMatchInfoUI();

    player2CardsLayout = new QHBoxLayout();
    player2CardsLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(player2CardsLayout);

    mainLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Minimum, QSizePolicy::Fixed));

    m_boardView = nullptr;

    mainLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Minimum, QSizePolicy::Fixed));

    player1CardsLayout = new QHBoxLayout();
    player1CardsLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(player1CardsLayout);

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
        QBrush(QPixmap(imagePath).scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    if (checkMage && checkPower) {
        setMagesAndPowersCompact(mage1Name, mage2Name, power1Name, power2Name);
    }
    else if (checkMage) {
        setMagesCompact(mage1Name, mage2Name);
    }
    else if (checkPower) {
        setPowersCompact(power1Name, power2Name);
    }

    this->showFullScreen();
}


void SecondaryWindow::setMagesCompact(const QString& mage1Name, const QString& mage2Name) {
    QHBoxLayout* mageContainerLayout = new QHBoxLayout();
    mageContainerLayout->setContentsMargins(50, 10, 50, 10);

    QString mage2ImagePath = mage2Name + ".jpg";
    QPixmap mage2Pixmap(mage2ImagePath);

    if (!mage2Pixmap.isNull()) {
        QIcon mage2Icon(mage2Pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QPushButton* mage2Button = new QPushButton(this);
        mage2Button->setFixedSize(120, 120);
        mage2Button->setIcon(mage2Icon);
        mage2Button->setIconSize(QSize(120, 120));
        mage2Button->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(255, 255, 255, 50); "
            "    border: 5px solid #0000FF; "
            "    border-radius: 15px; "
            "    padding: 5px;"
            "}"
            "QPushButton:hover {"
            "    border: 6px solid #0000FF; "
            "    background-color: rgba(0, 0, 255, 70);"
            "    transform: scale(1.05);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(0, 0, 255, 90);"
            "}"
        );

        connect(mage2Button, &QPushButton::clicked, this, [this, mage2Name]() {
            onMageClicked(mage2Name, Color::Blue);
            });

        mageContainerLayout->addWidget(mage2Button, 0, Qt::AlignLeft | Qt::AlignVCenter);

        qDebug() << "Added Player 2 mage:" << mage2Name << "with blue border on LEFT";
    }
    else {
        qDebug() << "Failed to load mage2 image:" << mage2ImagePath;
    }

    mageContainerLayout->addStretch(1);

    QString mage1ImagePath = mage1Name + ".jpg";
    QPixmap mage1Pixmap(mage1ImagePath);

    if (!mage1Pixmap.isNull()) {
        QIcon mage1Icon(mage1Pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QPushButton* mage1Button = new QPushButton(this);
        mage1Button->setFixedSize(120, 120);
        mage1Button->setIcon(mage1Icon);
        mage1Button->setIconSize(QSize(120, 120));
        mage1Button->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(255, 255, 255, 50); "
            "    border: 5px solid #FF0000; "
            "    border-radius: 15px; "
            "    padding: 5px;"
            "}"
            "QPushButton:hover {"
            "    border: 6px solid #FF0000; "
            "    background-color: rgba(255, 0, 0, 70);"
            "    transform: scale(1.05);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(255, 0, 0, 90);"
            "}"
        );

        connect(mage1Button, &QPushButton::clicked, this, [this, mage1Name]() {
            onMageClicked(mage1Name, Color::Red);
            });

        mageContainerLayout->addWidget(mage1Button, 0, Qt::AlignRight | Qt::AlignVCenter);

        qDebug() << "Added Player 1 mage:" << mage1Name << "with red border on RIGHT";
    }
    else {
        qDebug() << "Failed to load mage1 image:" << mage1ImagePath;
    }

    mainLayout->insertLayout(2, mageContainerLayout);

    qDebug() << "Mages positioned: " << mage2Name << " (LEFT, blue border) and " << mage1Name << " (RIGHT, red border)";
}
void SecondaryWindow::setPowersCompact(const QString& power1Name, const QString& power2Name) {
    QHBoxLayout* powerContainerLayout = new QHBoxLayout();
    powerContainerLayout->setContentsMargins(50, 0, 50, 0);

    QString power1ImagePath = power1Name + ".jpg";
    QPixmap power1Pixmap(power1ImagePath);

    if (!power1Pixmap.isNull()) {
        QIcon power1Icon(power1Pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QPushButton* power1Button = new QPushButton(this);
        power1Button->setFixedSize(100, 100);
        power1Button->setIcon(power1Icon);
        power1Button->setIconSize(QSize(100, 100));
        power1Button->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent; "
            "    border: 3px solid red; "
            "    border-radius: 8px; "
            "    padding: 2px;"
            "}"
            "QPushButton:hover {"
            "    border: 4px solid red; "
            "    background-color: rgba(255, 0, 0, 30);"
            "}"
        );

        connect(power1Button, &QPushButton::clicked, this, [this, power1Name]() {
            onPowerClicked(power1Name, Color::Red);
            });

        powerContainerLayout->addWidget(power1Button, 0, Qt::AlignLeft | Qt::AlignVCenter);
    }

    powerContainerLayout->addStretch(1);

    QString power2ImagePath = power2Name + ".jpg";
    QPixmap power2Pixmap(power2ImagePath);

    if (!power2Pixmap.isNull()) {
        QIcon power2Icon(power2Pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QPushButton* power2Button = new QPushButton(this);
        power2Button->setFixedSize(100, 100);
        power2Button->setIcon(power2Icon);
        power2Button->setIconSize(QSize(100, 100));
        power2Button->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent; "
            "    border: 3px solid blue; "
            "    border-radius: 8px; "
            "    padding: 2px;"
            "}"
            "QPushButton:hover {"
            "    border: 4px solid blue; "
            "    background-color: rgba(0, 0, 255, 30);"
            "}"
        );

        connect(power2Button, &QPushButton::clicked, this, [this, power2Name]() {
            onPowerClicked(power2Name, Color::Blue);
            });

        powerContainerLayout->addWidget(power2Button, 0, Qt::AlignRight | Qt::AlignVCenter);
    }

    mainLayout->insertLayout(1, powerContainerLayout);
    qDebug() << "Powers positioned: " << power1Name << " (left) and " << power2Name << " (right)";
}
void SecondaryWindow::setMagesAndPowersCompact(const QString& mage1Name, const QString& mage2Name,
    const QString& power1Name, const QString& power2Name) {

    QVBoxLayout* leftSideLayout = new QVBoxLayout();

    QString mage1ImagePath = mage1Name + ".jpg";
    QPixmap mage1Pixmap(mage1ImagePath);
    QIcon mage1Icon(mage1Pixmap.scaled(80, 80, Qt::KeepAspectRatio));
    QPushButton* mage1Button = new QPushButton(this);
    mage1Button->setIcon(mage1Icon);
    mage1Button->setIconSize(QSize(80, 80));
    mage1Button->setFixedSize(80, 80);
    mage1Button->setStyleSheet(
        "background-color: transparent; "
        "border: 2px solid red; "
        "border-radius: 5px;"
    );
    connect(mage1Button, &QPushButton::clicked, this, [this, mage1Name]() {
        onMageClicked(mage1Name, Color::Red);
        });

    QString power1ImagePath = power1Name + ".jpg";
    QPixmap power1Pixmap(power1ImagePath);
    QIcon power1Icon(power1Pixmap.scaled(80, 80, Qt::KeepAspectRatio));
    QPushButton* power1Button = new QPushButton(this);
    power1Button->setIcon(power1Icon);
    power1Button->setIconSize(QSize(80, 80));
    power1Button->setFixedSize(80, 80);
    power1Button->setStyleSheet(
        "background-color: transparent; "
        "border: 2px solid red; "
        "border-radius: 5px;"
    );
    connect(power1Button, &QPushButton::clicked, this, [this, power1Name]() {
        onPowerClicked(power1Name, Color::Red);
        });

    leftSideLayout->addWidget(mage1Button, 0, Qt::AlignLeft);
    leftSideLayout->addSpacing(10);
    leftSideLayout->addWidget(power1Button, 0, Qt::AlignLeft);

    QVBoxLayout* rightSideLayout = new QVBoxLayout();

    QString mage2ImagePath = mage2Name + ".jpg";
    QPixmap mage2Pixmap(mage2ImagePath);
    QIcon mage2Icon(mage2Pixmap.scaled(80, 80, Qt::KeepAspectRatio));
    QPushButton* mage2Button = new QPushButton(this);
    mage2Button->setIcon(mage2Icon);
    mage2Button->setIconSize(QSize(80, 80));
    mage2Button->setFixedSize(80, 80);
    mage2Button->setStyleSheet(
        "background-color: transparent; "
        "border: 2px solid blue; "
        "border-radius: 5px;"
    );
    connect(mage2Button, &QPushButton::clicked, this, [this, mage2Name]() {
        onMageClicked(mage2Name, Color::Blue);
        });

    QString power2ImagePath = power2Name + ".jpg";
    QPixmap power2Pixmap(power2ImagePath);
    QIcon power2Icon(power2Pixmap.scaled(80, 80, Qt::KeepAspectRatio));
    QPushButton* power2Button = new QPushButton(this);
    power2Button->setIcon(power2Icon);
    power2Button->setIconSize(QSize(80, 80));
    power2Button->setFixedSize(80, 80);
    power2Button->setStyleSheet(
        "background-color: transparent; "
        "border: 2px solid blue; "
        "border-radius: 5px;"
    );
    connect(power2Button, &QPushButton::clicked, this, [this, power2Name]() {
        onPowerClicked(power2Name, Color::Blue);
        });

    rightSideLayout->addWidget(mage2Button, 0, Qt::AlignRight);
    rightSideLayout->addSpacing(10);
    rightSideLayout->addWidget(power2Button, 0, Qt::AlignRight);

    QHBoxLayout* combinedContainerLayout = new QHBoxLayout();
    combinedContainerLayout->addLayout(leftSideLayout);
    combinedContainerLayout->addStretch();
    combinedContainerLayout->addLayout(rightSideLayout);

    mainLayout->insertLayout(2, combinedContainerLayout);
}

void SecondaryWindow::closeEvent(QCloseEvent* event) {
    emit closed();
    event->accept();
    QWidget::closeEvent(event);
}

void SecondaryWindow::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    QPixmap backgroundPixmap(imagePath);
    if (!backgroundPixmap.isNull()) {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        this->setPalette(palette);
    }
}

void SecondaryWindow::keyPressEvent(QKeyEvent* event) {
    if (!event) {
        return;
    }

    qDebug() << "Key pressed, menu visible:" << (menu && menu->isVisible());

    if (event->key() == Qt::Key_Escape) {
        if (!menu) {
            try {
                menu = std::make_unique<MenuWindow>(this);
                menu->hide();

                connect(menu.get(), &MenuWindow::goToHome, this, [this]() {
                    if (menu) {
                        menu->hide();
                    }
                    this->close();
                    emit closed();
                    });

                connect(menu.get(), &MenuWindow::exitApp, [this]() {
                    QApplication::closeAllWindows();
                    QApplication::processEvents();
                    QApplication::quit();

                    QTimer::singleShot(1000, []() {
                        exit(0);
                        });
                    });
            }
            catch (const std::exception& e) {
                qDebug() << "ERROR: Failed to create MenuWindow: " << e.what();
                return;
            }
            catch (...) {
                qDebug() << "ERROR: Unknown exception creating MenuWindow";
                return;
            }
        }

        if (menu) {
            if (menu->isVisible()) {
                menu->hide();
            }
            else {
                menu->adjustSize();
                menu->show();
                menu->raise();
                menu->activateWindow();
            }
        }
    }
    else {
        QWidget::keyPressEvent(event);
    }
}
void SecondaryWindow::onMageClicked(const QString& mageName, const Color& color) {
    qDebug() << "Mage clicked:" << mageName;

    if (!m_boardView) {
        qDebug() << "ERROR: BoardView is null";
        return;
    }

    Board& board = m_boardView->getBoard();
    if (board.getNumberOfRowsWithCards() < 3 || board.getNumberOfColumnsWithCards() < 3) {
        QMessageBox::information(this, "Mage Clicked",
            "You cannot use a mage power until the board has been fixed!");
        return;
    }

    try {
        Mages mage = fromQStringToMages(mageName);

        switch (mage) {
        case Mages::AirMageVelora: {
            bool returnedValue;
            do {
                bool ok;

                int startRow = QInputDialog::getInt(this, "Input Start Row", "Enter start row:",
                    0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
                if (!ok) break;

                int startCol = QInputDialog::getInt(this, "Input Start Column", "Enter start column:",
                    0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
                if (!ok) break;

                int endRow = QInputDialog::getInt(this, "Input End Row", "Enter end row:",
                    0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
                if (!ok) break;

                int endCol = QInputDialog::getInt(this, "Input End Column", "Enter end column:",
                    0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
                if (!ok) break;

                AirMageVelora airMageVelora;
                returnedValue = airMageVelora.playMageVelora(m_boardView->getBoard(), color,
                    startRow, startCol, endRow, endCol);
            } while (returnedValue == false);
            break;
        }
        default:
            qDebug() << "Unknown mage:" << mageName;
            break;
        }

        if (m_boardView) {
            m_boardView->updateView();
        }
    }
    catch (const std::exception& e) {
        qDebug() << "ERROR: Exception in onMageClicked: " << e.what();
        QMessageBox::warning(this, "Error", "An error occurred while using the mage power.");
    }
    catch (...) {
        qDebug() << "ERROR: Unknown exception in onMageClicked";
        QMessageBox::warning(this, "Error", "An unknown error occurred while using the mage power.");
    }
}
void SecondaryWindow::onPowerClicked(const QString& powerName, const Color& color)
{
    qDebug() << "Power clicked:" << powerName;

    Board& board = m_boardView->getBoard();
    if (board.getNumberOfRowsWithCards() < 3 || board.getNumberOfColumnsWithCards() < 3) {
        QMessageBox::information(this, "Power Clicked",
            "You cannot use a power until the board has been fixed!");
        return;
    }

    Power power = fromQStringToPower(powerName);

    switch (power) {
    case Power::PowerEarthquake: {
        PowerEarthquake earthquakePower;
        if (earthquakePower.checkEarthquakePower(m_boardView->getBoard())) {
            earthquakePower.playEarthquakePower(m_boardView->getBoard());
            QMessageBox::information(this, "Power Activated", "Earthquake has been used!");
        }
        else {
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
        if (tidePower.checkTidePower(m_boardView->getBoard(), x1, y1, x2, y2)) {
            tidePower.playTidePower(m_boardView->getBoard(), x1, y1, x2, y2);
            QMessageBox::information(this, "Power Activated", "Tide has swapped two stacks!");
        }
        else {
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
        if (stormPower.checkStormPower(m_boardView->getBoard(), x, y)) {
            stormPower.playStormPower(m_boardView->getBoard(), x, y);
            QMessageBox::information(this, "Power Activated", "Storm has cleared the stack!");
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
        squallPower.playSquallPower(m_boardView->getBoard(), game->getCurrentPlayer(), game->getCurrentPlayer(), x, y);
        QMessageBox::information(this, "Power Activated", "Squall has returned the opponent's card to their hand!");
        break;
    }
    case Power::PowerGale: {
        PowerGale galePower;
        galePower.playGalePower(m_boardView->getBoard(), game->getCurrentPlayer(), game->getCurrentPlayer());
        QMessageBox::information(this, "Power Activated", "Gale has removed covered cards!");

        auto& currentPlayer = game->getCurrentPlayer();

        qDebug() << "Active cards for current player:";
        for (const auto& card : currentPlayer.getVector()) {
            qDebug() << "Value:" << card.getValue() << ", Color:" << (card.getColor() == Color::Red ? "Red" : "Blue");
        }

        qDebug() << "Past cards for current player:";
        for (const auto& card : currentPlayer.getPastVector()) {
            qDebug() << "Value:" << card.getValue() << ", Color:" << (card.getColor() == Color::usedRed ? "UsedRed" : "UsedBlue");
        }

        break;
    }

    default:
        QMessageBox::information(this, "Power Clicked", "This power is not implemented yet!");
        break;
    }

    m_boardView->updateView();
}



void SecondaryWindow::updateBoardView() {
    static bool boardUpdateInProgress = false;

    if (boardUpdateInProgress) {
        qDebug() << "Board update already in progress, skipping";
        return;
    }

    boardUpdateInProgress = true;

    if (m_boardView) {
        try {
            qDebug() << "Updating board view...";

            m_boardView->updateView();

            qDebug() << "Board view update completed";

        }
        catch (const std::exception& e) {
            qDebug() << "Exception in updateBoardView: " << e.what();
        }
        catch (...) {
            qDebug() << "Unknown exception in updateBoardView";
        }
    }

    boardUpdateInProgress = false;
}
void SecondaryWindow::resetView()
{
    m_boardView->setIsMaxSize(false);
}



void SecondaryWindow::setCurrentPlayer(Color player) {
    selectedCardIndex = -1;
    selectedCard = SimpleCard();
    selectedCardPlayer = Color::Red;
    currentPlayer = player;

    if (!game) {
        qDebug() << "ERROR: Game instance is null in setCurrentPlayer";
        return;
    }

    game->clearSelectedCard();

    try {
        if (currentPlayer == Color::Red) {
            const auto& playerVector = game->getCurrentPlayer().getVector();
            setPlayer1Cards(playerVector);
        }
        else {
            const auto& playerVector = game->getCurrentPlayer().getVector();
            setPlayer2Cards(playerVector);
        }
    }
    catch (const std::exception& e) {
        qDebug() << "ERROR: Exception in setCurrentPlayer: " << e.what();
    }
    catch (...) {
        qDebug() << "ERROR: Unknown exception in setCurrentPlayer";
    }

    qDebug() << "Current player changed to " << (player == Color::Red ? "Red" : "Blue");
}

void SecondaryWindow::onCardSelected(const SimpleCard& card, int cardIndex) {
    if (card.getColor() != currentPlayer) {
        qDebug() << "Cannot select opponent's card!";
        return;
    }

    if (cardIndex < 0) {
        qDebug() << "ERROR: Invalid card index: " << cardIndex;
        return;
    }

    selectedCard = card;
    selectedCardIndex = cardIndex;
    selectedCardPlayer = currentPlayer;

    if (!game) {
        qDebug() << "ERROR: Game instance is null in onCardSelected";
        return;
    }

    game->setSelectedCard(card);

    qDebug() << "Card selected: Color ="
        << (card.getColor() == Color::Red ? "Red" : "Blue")
        << ", Value =" << card.getValue()
        << ", Index =" << cardIndex;

    try {
        if (currentPlayer == Color::Red) {
            const auto& playerVector = game->getCurrentPlayer().getVector();
            setPlayer1Cards(playerVector);
        }
        else {
            const auto& playerVector = game->getCurrentPlayer().getVector();
            setPlayer2Cards(playerVector);
        }
    }
    catch (const std::exception& e) {
        qDebug() << "ERROR: Exception in onCardSelected: " << e.what();
    }
    catch (...) {
        qDebug() << "ERROR: Unknown exception in onCardSelected";
    }
}
void SecondaryWindow::refreshCardDisplays() {
    if (!game) return;
    game->getCurrentPlayer();
}


void SecondaryWindow::setPlayer1Cards(const std::vector<SimpleCard>& cards) {
    if (!player1CardsLayout) {
        qDebug() << "ERROR: player1CardsLayout is null";
        return;
    }

    QLayoutItem* child;
    while ((child = player1CardsLayout->takeAt(0)) != nullptr) {
        if (QWidget* widget = child->widget()) {
            widget->setParent(nullptr);
            widget->deleteLater();
        }
        delete child;
    }

    const int cardWidth = 120;
    const int cardHeight = 120;

    for (size_t cardIndex = 0; cardIndex < cards.size(); ++cardIndex) {
        const auto& card = cards[cardIndex];

        if (card.getColor() == Color::usedRed) {
            continue;
        }

        auto cardButton = new QPushButton(this);
        if (!cardButton) {
            qDebug() << "ERROR: Failed to create card button";
            continue;
        }

        cardButton->setFixedSize(cardWidth, cardHeight);

        QString imagePath = "red" + QString::number(card.getValue()) + ".jpg";
        QPixmap pixmap(imagePath);

        if (!pixmap.isNull()) {
            QIcon buttonIcon(pixmap.scaled(cardWidth - 10, cardHeight - 10,
                Qt::KeepAspectRatio, Qt::SmoothTransformation));
            cardButton->setIcon(buttonIcon);
            cardButton->setIconSize(QSize(cardWidth - 10, cardHeight - 10));
        }
        else {
            cardButton->setText(QString::number(card.getValue()));
            qDebug() << "Image not found:" << imagePath;
        }

        bool isSelected = (selectedCardIndex == static_cast<int>(cardIndex) &&
            selectedCardPlayer == Color::Red &&
            currentPlayer == Color::Red);

        if (isSelected) {
            cardButton->setStyleSheet(
                "QPushButton {"
                "    border: 8px solid #FFD700; "
                "    background-color: rgba(255, 215, 0, 120); "
                "    border-radius: 15px; "
                "    margin: 2px; "
                "}"
                "QPushButton:hover {"
                "    border: 10px solid #FFD700; "
                "    background-color: rgba(255, 215, 0, 150); "
                "    transform: scale(1.08); "
                "}"
                "QPushButton:pressed {"
                "    background-color: rgba(255, 215, 0, 180); "
                "    border: 8px solid #FFA500; "
                "}"
            );
        }
        else {
            cardButton->setStyleSheet(
                "QPushButton {"
                "    border: 2px solid rgba(255, 255, 255, 150); "
                "    background-color: rgba(255, 255, 255, 40); "
                "    border-radius: 8px; "
                "    margin: 2px; "
                "}"
                "QPushButton:hover {"
                "    border: 3px solid rgba(255, 255, 255, 200); "
                "    background-color: rgba(255, 255, 255, 60); "
                "    transform: scale(1.03); "
                "}"
            );
        }

        player1CardsLayout->addWidget(cardButton);

        connect(cardButton, &QPushButton::clicked, this, [this, card, cardIndex]() {
            onCardSelected(card, static_cast<int>(cardIndex));
            });
    }
}

void SecondaryWindow::setPlayer2Cards(const std::vector<SimpleCard>& cards) {
    if (!player2CardsLayout) {
        qDebug() << "ERROR: player2CardsLayout is null";
        return;
    }

    QLayoutItem* child;
    while ((child = player2CardsLayout->takeAt(0)) != nullptr) {
        if (QWidget* widget = child->widget()) {
            widget->setParent(nullptr);
            widget->deleteLater();
        }
        delete child;
    }

    const int cardWidth = 120;
    const int cardHeight = 120;
    const int spacing = 15;
    player2CardsLayout->setSpacing(spacing);

    for (size_t cardIndex = 0; cardIndex < cards.size(); ++cardIndex) {
        const auto& card = cards[cardIndex];

        if (card.getColor() == Color::usedBlue) {
            continue;
        }

        auto cardButton = new QPushButton(this);
        if (!cardButton) {
            qDebug() << "ERROR: Failed to create card button";
            continue;
        }

        cardButton->setFixedSize(cardWidth, cardHeight);

        QString imagePath = "blue" + QString::number(card.getValue()) + ".jpg";
        QPixmap pixmap(imagePath);

        if (!pixmap.isNull()) {
            QIcon buttonIcon(pixmap.scaled(cardWidth - 10, cardHeight - 10,
                Qt::KeepAspectRatio, Qt::SmoothTransformation));
            cardButton->setIcon(buttonIcon);
            cardButton->setIconSize(QSize(cardWidth - 10, cardHeight - 10));
        }
        else {
            cardButton->setText(QString::number(card.getValue()));
            qDebug() << "Image not found:" << imagePath;
        }

        bool isSelected = (selectedCardIndex == static_cast<int>(cardIndex) &&
            selectedCardPlayer == Color::Blue &&
            currentPlayer == Color::Blue);

        if (isSelected) {
            cardButton->setStyleSheet(
                "QPushButton {"
                "    border: 8px solid #FFD700; "
                "    background-color: rgba(255, 215, 0, 120); "
                "    border-radius: 15px; "
                "    margin: 2px; "
                "}"
                "QPushButton:hover {"
                "    border: 10px solid #FFD700; "
                "    background-color: rgba(255, 215, 0, 150); "
                "    transform: scale(1.08); "
                "}"
                "QPushButton:pressed {"
                "    background-color: rgba(255, 215, 0, 180); "
                "    border: 8px solid #FFA500; "
                "}"
            );
        }
        else {
            cardButton->setStyleSheet(
                "QPushButton {"
                "    border: 2px solid rgba(255, 255, 255, 150); "
                "    background-color: rgba(255, 255, 255, 40); "
                "    border-radius: 8px; "
                "    margin: 2px; "
                "}"
                "QPushButton:hover {"
                "    border: 3px solid rgba(255, 255, 255, 200); "
                "    background-color: rgba(255, 255, 255, 60); "
                "    transform: scale(1.03); "
                "}"
            );
        }

        player2CardsLayout->addWidget(cardButton);

        connect(cardButton, &QPushButton::clicked, this, [this, card, cardIndex]() {
            onCardSelected(card, static_cast<int>(cardIndex));
            });
    }
}

void SecondaryWindow::setMages(const QString& mage1Name, const QString& mage2Name) {


    QString mage1ImagePath = mage1Name + ".jpg";
    QPixmap mage1Pixmap(mage1ImagePath);
    QIcon mage1Icon(mage1Pixmap.scaled(150, 150, Qt::KeepAspectRatioByExpanding));

    QPushButton* mage1Button = new QPushButton(this);
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


    QPushButton* mage2Button = new QPushButton(this);

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
}

void SecondaryWindow::setPowers(const QString& mage1Name, const QString& mage2Name) {


    QString mage1ImagePath = mage1Name + ".jpg";
    QPixmap mage1Pixmap(mage1ImagePath);
    QIcon mage1Icon(mage1Pixmap.scaled(150, 150, Qt::KeepAspectRatioByExpanding));


    QPushButton* mage1Button = new QPushButton(this);
    mage1Button->setStyleSheet("background-color: transparent; border: none;");
    mage1Button->setFixedSize(150, 150);
    mage1Button->setIcon(mage1Icon);
    mage1Button->setIconSize(QSize(150, 150));
    connect(mage1Button, &QPushButton::clicked, this, [this, mage1Name]() {
        onPowerClicked(mage1Name, Color::Red);
        });




    QVBoxLayout* player1MageLayout = new QVBoxLayout();
    player1MageLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Fixed));
    player1MageLayout->addWidget(mage1Button, 0, Qt::AlignLeft);
    player1CardsLayout->addLayout(player1MageLayout);


    QString mage2ImagePath = mage2Name + ".jpg";
    QPixmap mage2Pixmap(mage2ImagePath);

    QIcon mage2Icon(mage2Pixmap.scaled(150, 150, Qt::KeepAspectRatioByExpanding));


    QPushButton* mage2Button = new QPushButton(this);

    mage2Button->setFixedSize(150, 150);
    mage2Button->setIcon(mage2Icon);
    mage2Button->setIconSize(QSize(150, 150));

    connect(mage2Button, &QPushButton::clicked, this, [this, mage2Name]() {
        onPowerClicked(mage2Name, Color::Blue);
        });

    mage2Button->setStyleSheet("background-color: rgba(0, 255, 0, 0.3); border: 10px;");


    QVBoxLayout* player2MageLayout = new QVBoxLayout();


    player2MageLayout->setContentsMargins(0, 400, 0, 0);


    player2MageLayout->addWidget(mage2Button);

    player2MageLayout->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));


    player1CardsLayout->addLayout(player2MageLayout);
}

void SecondaryWindow::setMagesAndPowers(const QString& mage1Name, const QString& mage2Name,
    const QString& power1Name, const QString& power2Name) {
    QVBoxLayout* player1CombinedLayout = new QVBoxLayout();


    QString mage1ImagePath = mage1Name + ".jpg";
    QPixmap mage1Pixmap(mage1ImagePath);
    QIcon mage1Icon(mage1Pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    QPushButton* mage1Button = new QPushButton(this);
    mage1Button->setIcon(mage1Icon);
    mage1Button->setIconSize(QSize(100, 100));
    mage1Button->setStyleSheet("background-color: transparent; border: none;");
    connect(mage1Button, &QPushButton::clicked, this, [this, mage1Name]() {
        onMageClicked(mage1Name, Color::Red);
        });


    QString power1ImagePath = power1Name + ".jpg";
    QPixmap power1Pixmap(power1ImagePath);
    QIcon power1Icon(power1Pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    QPushButton* power1Button = new QPushButton(this);
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
    QPushButton* mage2Button = new QPushButton(this);
    mage2Button->setIcon(mage2Icon);
    mage2Button->setIconSize(QSize(100, 100));
    mage2Button->setStyleSheet("background-color: transparent; border: none;");
    connect(mage2Button, &QPushButton::clicked, this, [this, mage2Name]() {
        onMageClicked(mage2Name, Color::Blue);
        });


    QString power2ImagePath = power2Name + ".jpg";
    QPixmap power2Pixmap(power2ImagePath);
    QIcon power2Icon(power2Pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    QPushButton* power2Button = new QPushButton(this);
    power2Button->setIcon(power2Icon);
    power2Button->setIconSize(QSize(100, 100));
    power2Button->setStyleSheet("background-color: transparent; border: none;");
    connect(power2Button, &QPushButton::clicked, this, [this, power2Name]() {
        onPowerClicked(power2Name, Color::Blue);
        });

    player2CombinedLayout->addWidget(mage2Button, 0, Qt::AlignRight);
    player2CombinedLayout->addWidget(power2Button, 0, Qt::AlignRight);

    player1CardsLayout->addLayout(player2CombinedLayout);
}
