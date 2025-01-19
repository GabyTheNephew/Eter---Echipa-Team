#include "SecondaryWindow.h"


SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, Game* gameInstance,const QString& mage1Name, const QString& mage2Name, const QString& power1Name, const QString& power2Name, bool checkMage,bool checkPower, QWidget* parent)
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
    if (!m_boardView) { 
        m_boardView = new BoardView(board, this, setMaxSize);
        m_boardView->setFixedSize(350, 350);

        
        mainLayout->insertWidget(1, m_boardView, 0, Qt::AlignHCenter | Qt::AlignVCenter);

        
        connect(m_boardView, &BoardView::cellClicked, this, &SecondaryWindow::onBoardClicked);

        
        m_boardView->updateView();
    }
}

void SecondaryWindow::showWinner(const QString& winnerName) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Game Over");
    msgBox.setText("The winner is: " + winnerName);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    emit closed();
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
}







void SecondaryWindow::onBoardClicked(int row, int col) {
    if (!selectedCard.getValue()) {
        qDebug() << "No card selected!";
        return;
    }

    qDebug() << "Attempting to place card at (" << row << ", " << col << "):"
        << "Color =" << (selectedCard.getColor() == Color::Red ? "Red" : "Blue")
        << ", Value =" << selectedCard.getValue();



    Board::Position pos = { row, col };



    if (selectedCard.getColor() == currentPlayer) {

        if (!m_boardView->canPlaceCard(selectedCard, row, col)) {
            qDebug() << "Position is not valid for placement.";
            return;
        }


        m_boardView->placeCard(selectedCard, row, col);


        game->getCurrentPlayer().makeCardInvalid(selectedCard);
        game->getCurrentPlayer().getPastVector().push_back(selectedCard);



        if (currentPlayer == Color::Red) {
            setPlayer1Cards(game->getCurrentPlayer().getVector());
        }
        else {
            setPlayer2Cards(game->getCurrentPlayer().getVector());
        }

        selectedCard = SimpleCard(); 
        qDebug() << "Card placed successfully.";

        game->setPlayerMoveCompleted(true); 

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

void SecondaryWindow::onMageClicked(const QString& mageName, const Color& color)
{
    qDebug() << "Mage clicked:" << mageName;

    if (m_boardView->getBoard().getSize() < m_boardView->getMaxSize())
    {
        QMessageBox::information(this, "Mage Clicked", "You cannot use a mage power if the board is not defined yet!");
        return;
    }

    Mages mage = fromQStringToMages(mageName);

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


            AirMageVelora AirMageVelora;
            returnedValue = AirMageVelora.playMageVelora(m_boardView->getBoard(), color, startRow, startCol, endRow, endCol);
        } while (returnedValue == false);
        break;
    }
    case Mages::AirMageZephyraCrow: {
        bool ok;

        int row = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break; 

        int col = QInputDialog::getInt(this, "Input Start Column", "Enter start column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break; 

        AirMageZephyraCrow AirMageZephyraCrow;
        AirMageZephyraCrow.playMageZephyraCrow(m_boardView->getBoard(), color, row, col);
        break;
    }
    case Mages::EarthMageBumbleroot: {
        EarthMageBumbleroot EarthMageBumbleroot;

        bool ok;


        int row = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break; 

        int col = QInputDialog::getInt(this, "Input Start Column", "Enter start column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break; 

        EarthMageBumbleroot.playMageBumbleroot(m_boardView->getBoard(), row, col);
        break;
    }
    case Mages::EarthMageElderbranch: {
        EarthMageElderbranch EarthMageElderbranch;

        break;
    }
    case Mages::FireMageIgnara: {

        bool ok;


        int row = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break; 

        int col = QInputDialog::getInt(this, "Input Start Column", "Enter start column:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break; 

        FireMageIgnara FireMageIgnara;
        FireMageIgnara.playMageIgnara(m_boardView->getBoard(), color, row, col);
        break;
    }
    case Mages::FireMagePyrofang: {

        bool ok;


        int row = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break; 

        bool rowOrColumn = QInputDialog::getInt(this, "Input Row Or Column", "Enter 0 for column or 1 for row:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break; 


        FireMagePyrofang FireMagePyrofang;
        FireMagePyrofang.playMagePyrofang(m_boardView->getBoard(), color, rowOrColumn, row);
        break;
    }
    case Mages::WaterMageAqualon: {

        bool ok;


        int row = QInputDialog::getInt(this, "Input Start Row", "Enter start row:", 0, 0, m_boardView->getBoard().getRowSize() - 1, 1, &ok);
        if (!ok) break; 

        bool rowOrColumn = QInputDialog::getInt(this, "Input Row Or Column", "Enter 0 for column or 1 for row:", 0, 0, m_boardView->getBoard().getColumnSize() - 1, 1, &ok);
        if (!ok) break; 

        WaterMageAqualon WaterMageAqualon;
        WaterMageAqualon.playMageAqualon(m_boardView->getBoard(),rowOrColumn, row);
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

        WaterMageChillThoughts WaterMageChillThoughts;
        WaterMageChillThoughts.playMageChillThoughts(m_boardView->getBoard(), color, startRow, startCol, endRow, endCol);
        break;
    }
    default:
        break;
    }

    m_boardView->updateView();
}

void SecondaryWindow::onPowerClicked(const QString& powerName, const Color& color)
{
    qDebug() << "Power clicked:" << powerName;

    if (m_boardView->getBoard().getSize() < m_boardView->getMaxSize()) {
        QMessageBox::information(this, "Power Clicked", "You cannot use a power if the board is not fully defined yet!");
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
