#include "SecondaryWindow.h"


SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, Game* gameInstance,const QString& mage1Name, const QString& mage2Name, bool checkMage, QWidget* parent)
    : QWidget(parent), imagePath(imagePath), game(gameInstance) {
    setWindowTitle(title);

    // Creăm layout-ul principal
    mainLayout = new QVBoxLayout(this);

    // Layout pentru cărțile albastre (sus)
    player2CardsLayout = new QHBoxLayout();
    mainLayout->addLayout(player2CardsLayout);

    // Spacer între cărțile albastre și tabla de joc
    mainLayout->addSpacerItem(new QSpacerItem(0, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Placeholder pentru tabla de joc
    m_boardView = nullptr;

    // Spacer între tabla de joc și cărțile roșii
    mainLayout->addSpacerItem(new QSpacerItem(0, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // Layout pentru cărțile roșii (jos)
    player1CardsLayout = new QHBoxLayout();
    mainLayout->addLayout(player1CardsLayout);

    // Setăm fundalul
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
        QBrush(QPixmap(imagePath).scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    // Afișăm magii doar pentru tipul de joc `MageDuel`
    if (checkMage) {
        setMages(mage1Name, mage2Name);
    }

    this->showFullScreen();
}




void SecondaryWindow::closeEvent(QCloseEvent* event) {
    emit closed();
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
    qDebug() << "Key pressed, menu visible:" << (menu && menu->isVisible());

    if (event->key() == Qt::Key_Escape) {
        if (!menu) {
            menu = new MenuWindow(this);

            connect(menu, &MenuWindow::goToHome, this, [this]() {
                menu->hide();
                this->close();
                emit closed();
                });

            connect(menu, &MenuWindow::exitApp, []() {
                QApplication::quit();
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

void SecondaryWindow::setBoard(Board& board, int setMaxSize) {
    if (!m_boardView) { // Creăm `m_boardView` doar dacă nu există deja
        m_boardView = new BoardView(board, this, setMaxSize);
        m_boardView->setFixedSize(350, 350);

        // Adăugăm tabla în layout-ul principal
        mainLayout->insertWidget(1, m_boardView, 0, Qt::AlignHCenter | Qt::AlignVCenter);

        // Conectăm semnalul `cellClicked` la slotul `onBoardClicked`
        connect(m_boardView, &BoardView::cellClicked, this, &SecondaryWindow::onBoardClicked);

        // Actualizăm vizualizarea tablei
        m_boardView->updateView();
    }
}

void SecondaryWindow::showWinner(const QString& winnerName) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Game Over");
    msgBox.setText("The winner is: " + winnerName);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    emit closed(); // Semnal pentru a închide fereastra curentă
}




void SecondaryWindow::setPlayer1Cards(const std::vector<SimpleCard>& cards) {
    QLayoutItem* child;
    while ((child = player1CardsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    for (const auto& card : cards) {
        if (card.getColor() == Color::usedRed) {
            continue; // Sărim peste cărțile roșii folosite
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

        // Conectăm clicul pe buton la selecția cărții
        connect(cardButton, &QPushButton::clicked, this, [this, card]() {
            onCardSelected(card);
            });
    }
}


void SecondaryWindow::setPlayer2Cards(const std::vector<SimpleCard>& cards) {
    QLayoutItem* child;
    while ((child = player2CardsLayout->takeAt(0)) != nullptr) {
        delete child->widget(); // Eliminăm toate widget-urile existente
        delete child;
    }

    int imageWidth = 150;  // Lățimea imaginii
    int imageHeight = 200; // Înălțimea imaginii
    int spacing = 20;      // Spațiul dintre imagini
    player2CardsLayout->setSpacing(spacing);

    for (const auto& card : cards) {
        if (card.getColor() == Color::usedBlue) {
            continue; // Sărim peste cărțile roșii folosite
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

        // Conectăm clicul pe buton la selecția cărții albastre
        connect(cardButton, &QPushButton::clicked, this, [this, card]() {
            onCardSelected(card);
            });

    }
}

void SecondaryWindow::setCurrentPlayer(Color player) {
    currentPlayer = player; 
}







void SecondaryWindow::onBoardClicked(int row, int col) {
    if (!selectedCard.getValue()) {
        qDebug() << "No card selected!";
        return;
    }

    qDebug() << "Attempting to place card at (" << row << ", " << col << "):"
        << "Color =" << (selectedCard.getColor() == Color::Red ? "Red" : "Blue")
        << ", Value =" << selectedCard.getValue();

    // Obține ultima carte de pe poziție

    Board::Position pos = { row, col };



    if (selectedCard.getColor() == currentPlayer) {

        if (!m_boardView->canPlaceCard(selectedCard, row, col)) {
            qDebug() << "Position is not valid for placement.";
            return;
        }

        // Plasăm cartea
        m_boardView->placeCard(selectedCard, row, col);

        // Apelăm metoda makeCardInvalid pe jucătorul curent
        game->getCurrentPlayer().makeCardInvalid(selectedCard);
        game->getCurrentPlayer().getPastVector().push_back(selectedCard);


        // Actualizăm cărțile jucătorului curent
        if (currentPlayer == Color::Red) {
            setPlayer1Cards(game->getCurrentPlayer().getVector());
        }
        else {
            setPlayer2Cards(game->getCurrentPlayer().getVector());
        }

        selectedCard = SimpleCard(); // Resetăm selecția
        qDebug() << "Card placed successfully.";

        game->setPlayerMoveCompleted(true); // Indică faptul că mutarea a fost completă

        int rowSizeBeforeChange = m_boardView->getBoard().getRowSize() - 1;
        int colSizeBeforeChange = m_boardView->getBoard().getColumnSize() - 1;

        if(m_boardView->getMaxSize() > m_boardView->getBoard().getRowSize() - 1)
        {
            if (m_boardView->getIsMaxSize() == false)
            {
                if (row == 0)
                {
                    m_boardView->getBoard().expandRow(Board::RowExpandDirection::Up);
                }
                if (row == rowSizeBeforeChange)
                {
                    m_boardView->getBoard().expandRow(Board::RowExpandDirection::Down);
                }
            }
        }
        if (m_boardView->getMaxSize() > m_boardView->getBoard().getColumnSize() - 1)
        {
            if(m_boardView->getIsMaxSize() == false)
            {
                if (col == 0)
                {
                    m_boardView->getBoard().expandColumn(Board::ColumnExpandDirection::Left);
                }
                if (col == colSizeBeforeChange)
                {
                    m_boardView->getBoard().expandColumn(Board::ColumnExpandDirection::Right);
                }
            }
        }

        /*if (m_boardView->getBoard().getNumberOfRowsWithCards() >= m_boardView->getMaxSize() ||
            m_boardView->getBoard().getNumberOfColumnsWithCards() >= m_boardView->getMaxSize())
                m_boardView->setIsMaxSize(true);*/

        if (m_boardView->getBoard().getNumberOfRowsWithCards() == m_boardView->getMaxSize())
        {
            if (m_boardView->getBoard().isFirstRowEmpty())
            {
                if (!m_boardView->getBoard().isLastRowEmpty())
                {
                    m_boardView->getBoard().removeRow(0);
                    m_boardView->getBoard().print();
                }
            }
            if (m_boardView->getBoard().isLastRowEmpty())
            {
                if (!m_boardView->getBoard().isFirstRowEmpty())
                {
                    m_boardView->getBoard().removeRow(m_boardView->getBoard().getRowSize() - 1);
                    m_boardView->getBoard().print();
                }
            }
        }

        if (m_boardView->getBoard().getNumberOfColumnsWithCards() == m_boardView->getMaxSize())
        {
            if (m_boardView->getBoard().isFirstColumnEmpty())
            {
                if (!m_boardView->getBoard().isLastColumnEmpty())
                {
                    m_boardView->getBoard().removeColumn(0);
                    m_boardView->getBoard().print();
                }
            }
            if (m_boardView->getBoard().isLastColumnEmpty())
            {
                if (!m_boardView->getBoard().isFirstColumnEmpty())
                {
                    m_boardView->getBoard().removeColumn(m_boardView->getBoard().getColumnSize() - 1);
                    m_boardView->getBoard().print();
                }
            }
        }

        if (m_boardView->getBoard().getNumberOfRowsWithCards() >= m_boardView->getMaxSize() && 
            m_boardView->getBoard().getNumberOfColumnsWithCards() >= m_boardView->getMaxSize() && m_boardView->getIsMaxSize() == false)
        {
            m_boardView->setIsMaxSize(true);
            m_boardView->getBoard().print();

            if (m_boardView->getBoard().isFirstRowEmpty())
            {
                if(!m_boardView->getBoard().isLastRowEmpty())
                {
                    m_boardView->getBoard().removeRow(0);
                    m_boardView->getBoard().print();
                }
            }
            if (m_boardView->getBoard().isLastRowEmpty())
            {
                if(!m_boardView->getBoard().isFirstRowEmpty())
                {
                    m_boardView->getBoard().removeRow(m_boardView->getBoard().getRowSize() - 1);
                    m_boardView->getBoard().print();
                }
            }
            if (m_boardView->getBoard().isFirstColumnEmpty())
            {
                if(!m_boardView->getBoard().isLastColumnEmpty())
                {
                    m_boardView->getBoard().removeColumn(0);
                    m_boardView->getBoard().print();
                }
            }
            if (m_boardView->getBoard().isLastColumnEmpty())
            {
                if(!m_boardView->getBoard().isFirstColumnEmpty())
                {
                    m_boardView->getBoard().removeColumn(m_boardView->getBoard().getColumnSize() - 1);
                    m_boardView->getBoard().print();
                }
            }

        }
    }
    m_boardView->getBoard().print();
    m_boardView->updateView();
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
    selectedCard = card;
    qDebug() << "Card selected: Color ="
        << (card.getColor() == Color::Red ? "Red" : "Blue")
        << ", Value =" << card.getValue();
}

void SecondaryWindow::setMages(const QString& mage1Name, const QString& mage2Name) {
    // Layout pentru Player 1 (stânga jos)
    QLabel* mage1Label = new QLabel(this);
    QString mage1ImagePath = mage1Name + ".jpg";
    QPixmap mage1Pixmap(mage1ImagePath);
    if (!mage1Pixmap.isNull()) {
        mage1Label->setPixmap(mage1Pixmap.scaled(150, 150, Qt::KeepAspectRatio));
    }
    else {
        mage1Label->setText("Mage 1 Image not found");
    }
    mage1Label->setAlignment(Qt::AlignCenter);

    // Creăm layout-ul pentru magul 1
    QVBoxLayout* player1MageLayout = new QVBoxLayout();
    player1MageLayout->addSpacerItem(new QSpacerItem(0, 420, QSizePolicy::Minimum, QSizePolicy::Fixed)); // Adăugăm spațiu sub mag
    player1MageLayout->addWidget(mage1Label, 0, Qt::AlignLeft);
    player1CardsLayout->addLayout(player1MageLayout);

    // Layout pentru Player 2 (dreapta jos)
    QLabel* mage2Label = new QLabel(this);
    QString mage2ImagePath = mage2Name + ".jpg";
    QPixmap mage2Pixmap(mage2ImagePath);
    if (!mage2Pixmap.isNull()) {
        mage2Label->setPixmap(mage2Pixmap.scaled(150, 150, Qt::KeepAspectRatio));
    }
    else {
        mage2Label->setText("Mage 2 Image not found");
    }
    mage2Label->setAlignment(Qt::AlignCenter);

    // Creăm layout-ul pentru magul 2
    QVBoxLayout* player2MageLayout = new QVBoxLayout();

    // Setăm margini personalizate pentru a controla poziționarea
    player2MageLayout->setContentsMargins(0, 170, 0, 0); // Margini: stânga, sus, dreapta, jos

    // Adăugăm magul în layout
    player2MageLayout->addWidget(mage2Label, 0, Qt::AlignRight);

    // Adăugăm un spacer flexibil (opțional, dacă mai e nevoie)
    player2MageLayout->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Adăugăm layout-ul magului în layout-ul cărților roșii
    player1CardsLayout->addLayout(player2MageLayout);
}
