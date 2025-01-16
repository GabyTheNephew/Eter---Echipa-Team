#include "SecondaryWindow.h"


SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, QWidget* parent)
    : QWidget(parent), imagePath(imagePath) {
    setWindowTitle(title);

    // Creăm layout-ul principal pentru întreaga fereastră
    mainLayout = new QVBoxLayout(this);

    // Adăugăm un spacer extensibil la început pentru a împinge conținutul în jos
    mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Adăugăm un spacer extensibil la sfârșit pentru a trage conținutul în sus
    mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Layout pentru cărțile jucătorului 1
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
    m_boardView = new BoardView(board, this);

    // Am redus dimensiunea la 120x120
    m_boardView->setFixedSize(350, 350);

    mainLayout->insertWidget(1, m_boardView, 0, Qt::AlignCenter);

    m_boardView->updateView();
}

void SecondaryWindow::setPlayer1Cards(const std::vector<SimpleCard>& cards) {
    QLayoutItem* child;
    while ((child = player1CardsLayout->takeAt(0)) != nullptr) {
        delete child->widget(); // Eliminăm toate widget-urile existente
        delete child;
    }

    for (const auto& card : cards) {
        auto cardLabel = new QLabel(this);
        cardLabel->setText(QString::number(card.getValue())); // Afișăm valoarea cărții
        cardLabel->setAlignment(Qt::AlignCenter);
        cardLabel->setStyleSheet("border: 1px solid black; background-color: white;");
        player1CardsLayout->addWidget(cardLabel);
    }
}



