#include "SecondaryWindow.h"

SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, QWidget* parent)
    : QWidget(parent), imagePath(imagePath) {
    setWindowTitle(title);

    // Creăm layout-ul principal
    mainLayout = new QVBoxLayout(this);

    connect(m_boardView, &BoardView::cellClicked, this, &SecondaryWindow::onBoardClicked);

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

void SecondaryWindow::setBoard(Board& board) {
    if (!m_boardView) {
        m_boardView = new BoardView(board, this);
        m_boardView->setFixedSize(350, 350);

        // Adăugăm tabla în layout-ul principal
        mainLayout->insertWidget(1, m_boardView, 0, Qt::AlignHCenter | Qt::AlignVCenter);

        // Conectăm semnalul `cellClicked` la slotul `onBoardClicked`
        connect(m_boardView, &BoardView::cellClicked, this, &SecondaryWindow::onBoardClicked);

        // Actualizăm vizualizarea tablei
        m_boardView->updateView();
    }
}





void SecondaryWindow::setPlayer1Cards(const std::vector<SimpleCard>& cards) {
    QLayoutItem* child;
    while ((child = player1CardsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    for (const auto& card : cards) {
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




void SecondaryWindow::onBoardClicked(int row, int col) {
    if (!selectedCard.getValue()) {
        qDebug() << "No card selected!";
        return;
    }

    qDebug() << "Placing card: Color ="
        << (selectedCard.getColor() == Color::Red ? "Red" : "Blue")
        << ", Value =" << selectedCard.getValue();

    if (m_boardView->canPlaceCard(row, col)) {
        m_boardView->placeCard(selectedCard, row, col); // Transmitem selectedCard
        qDebug() << "Card placed: Color ="
            << (selectedCard.getColor() == Color::Red ? "Red" : "Blue")
            << ", Value =" << selectedCard.getValue() << " at row =" << row << ", col =" << col;

        selectedCard = SimpleCard(); // Resetăm selecția
    }
    else {
        qDebug() << "Cannot place card at:" << row << col;
    }
}




void SecondaryWindow::onCardSelected(const SimpleCard& card) {
    selectedCard = card;
    qDebug() << "Card selected: Color ="
        << (card.getColor() == Color::Red ? "Red" : "Blue")
        << ", Value =" << card.getValue();
}


