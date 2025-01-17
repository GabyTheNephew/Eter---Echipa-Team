#include "SecondaryWindow.h"

SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, QWidget* parent)
    : QWidget(parent), imagePath(imagePath) {
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

        // Setăm dimensiunea tablei de joc
        m_boardView->setFixedSize(350, 350);

        // Eliminăm orice widget sau spacer existent pe poziția tablei
        QLayoutItem* item = mainLayout->itemAt(1); // Poziția 1 în layout
        if (item) {
            mainLayout->removeItem(item);
            delete item;
        }

        // Adăugăm tabla de joc, centrată
        mainLayout->insertWidget(1, m_boardView, 0, Qt::AlignHCenter | Qt::AlignVCenter);

        m_boardView->updateView();
    }
}




void SecondaryWindow::setPlayer1Cards(const std::vector<SimpleCard>& cards) {
    QLayoutItem* child;
    while ((child = player1CardsLayout->takeAt(0)) != nullptr) {
        delete child->widget(); // Eliminăm toate widget-urile existente
        delete child;
    }

    for (const auto& card : cards) {
        auto cardLabel = new QLabel(this);

        // Determinăm imaginea corespunzătoare cărții pe baza valorii și culorii
        QString imagePath;
        if (card.getColor() != Color::usedRed) {
            imagePath += (card.getColor() == Color::Red ? "red" : "blue");
            imagePath += QString::number(card.getValue()) + ".jpg";


            // Setăm imaginea ca fundal al QLabel-ului
            QPixmap pixmap(imagePath);
            if (!pixmap.isNull()) {
                cardLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatioByExpanding));
            }
            else {
                cardLabel->setText("Image not found");
                cardLabel->setStyleSheet("border: 1px solid black; background-color: white;");
            }


            cardLabel->setAlignment(Qt::AlignCenter);
            player1CardsLayout->addWidget(cardLabel);
        }
    }
}

void SecondaryWindow::setPlayer2Cards(const std::vector<SimpleCard>& cards) {
    QLayoutItem* child;
    while ((child = player2CardsLayout->takeAt(0)) != nullptr) {
        delete child->widget(); // Eliminăm toate widget-urile existente
        delete child;
    }

    // Configurare spațiere și dimensiune imagini
    int imageWidth = 150; // Lățimea imaginii
    int imageHeight = 200; // Înălțimea imaginii
    int spacing = 20; // Spațiul dintre imagini
    player2CardsLayout->setSpacing(spacing);

    for (const auto& card : cards) {
        auto cardLabel = new QLabel(this);

        // Determinăm imaginea corespunzătoare cărții albastre
        QString imagePath = "blue_";
        imagePath += QString::number(card.getValue()) + ".jpg";

        // Setăm imaginea ca fundal al QLabel-ului
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            cardLabel->setPixmap(pixmap.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio));
        }
        else {
            cardLabel->setText("Image not found");
            cardLabel->setStyleSheet("border: 1px solid black; background-color: white;");
        }

        cardLabel->setAlignment(Qt::AlignCenter);
        player2CardsLayout->addWidget(cardLabel);
    }
}



