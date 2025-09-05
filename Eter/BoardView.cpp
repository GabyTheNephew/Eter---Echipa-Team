#include "BoardView.h"
#include <QApplication>
#include <QVBoxLayout>
#include <chrono>
BoardView::BoardView(Board& boardInstance, QWidget* parent, int maxSize)
    : QWidget(parent), board(boardInstance), maxSize(maxSize), isMaxSize(false) {

    qDebug() << "Creating BoardView with max size:" << maxSize;

    try {
        // Create layouts with this widget as parent
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        if (!mainLayout) {
            qDebug() << "ERROR: Failed to create main layout";
            return;
        }

        gridLayout = new QGridLayout();
        if (!gridLayout) {
            qDebug() << "ERROR: Failed to create grid layout";
            return;
        }

        // Set up the layout structure
        mainLayout->addStretch();
        mainLayout->addLayout(gridLayout);
        mainLayout->addStretch();

        // Set spacing and margins for better appearance
        mainLayout->setContentsMargins(10, 10, 10, 10);
        mainLayout->setSpacing(5);
        gridLayout->setSpacing(2);

        setLayout(mainLayout);

        // Initialize the containers but don't create buttons yet
        // updateView() will be called after this constructor
        cellButtons.clear();

        // Make sure the widget is visible by default
        this->setVisible(true);

        qDebug() << "BoardView constructor completed successfully";

    }
    catch (const std::exception& e) {
        qDebug() << "Exception in BoardView constructor:" << e.what();
    }
    catch (...) {
        qDebug() << "Unknown exception in BoardView constructor";
    }
}
void BoardView::setupButtonIcon(QPushButton* button, const SimpleCard& card) {
    QString imagePath = (card.getColor() == Color::Red || card.getColor() == Color::IlusionRed) ? "red" : "blue";
    imagePath += QString::number(card.getValue()) + ".jpg";

    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        QPixmap scaledPixmap = pixmap.scaled(button->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        button->setIcon(QIcon(scaledPixmap));
        button->setIconSize(button->size());
    }
    else {
        qDebug() << "Image not found for card: Value =" << card.getValue()
            << ", Color =" << (card.getColor() == Color::Red ? "Red" : "Blue");
        button->setText(QString::number(card.getValue()));
    }
}

void BoardView::styleCellButton(QPushButton* button, bool canPlace, bool isEmpty, bool isInPlayableArea) {
    if (canPlace && isEmpty) {
        // Poziție goală unde se poate plasa o carte
        button->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(144, 238, 144, 100);" // Verde pentru pozițiile valide
            "    border: 2px solid green;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(144, 238, 144, 150);"
            "}"
        );
    }
    else if (isEmpty && isInPlayableArea) {
        // Poziție în zona jucabilă dar nu adiacentă
        button->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(255, 255, 220, 70);" // Galben pal pentru zona jucabilă
            "    border: 1px solid #DDD;"
            "}"
        );
        button->setEnabled(false);
    }
    else if (isEmpty) {
        // Poziție în afara zonei jucabile - aproape invizibilă
        button->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(240, 240, 240, 30);" // Foarte pal
            "    border: 1px solid rgba(200, 200, 200, 50);"
            "}"
        );
        button->setEnabled(false);
    }
    else {
        // Poziție ocupată
        button->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(255, 255, 255, 80);"
            "    border: 2px solid black;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(255, 255, 255, 120);"
            "}"
        );
    }
}

void BoardView::createCellButton(int row, int col) {
    try {
        // Validate indices
        if (row < 0 || row >= board.getRowSize() || col < 0 || col >= board.getColumnSize()) {
            qDebug() << "Invalid indices for createCellButton:" << row << col;
            return;
        }

        // Ensure the container is properly sized
        if (row >= static_cast<int>(cellButtons.size()) || col >= static_cast<int>(cellButtons[row].size())) {
            qDebug() << "Container not properly sized for button at (" << row << "," << col << ")";
            return;
        }

        qDebug() << "Creating button at (" << row << "," << col << ")";

        // Create button using make_unique - start it hidden to prevent flash
        auto cellButton = std::make_unique<QPushButton>(this);
        if (!cellButton) {
            qDebug() << "Failed to create button for position (" << row << "," << col << ")";
            return;
        }

        // Set fixed size immediately
        cellButton->setFixedSize(100, 100);

        // Start hidden to prevent flicker during setup
        cellButton->setVisible(false);

        // Block signals during setup
        cellButton->blockSignals(true);

        // Calculate playable area logic
        bool isInPlayableArea = true;
        auto [actualRows, actualCols] = board.getActualBoardBounds();

        if (!isMaxSize && actualRows > 0 && actualCols > 0) {
            int16_t minRow = board.getRowSize(), maxRow = -1;
            int16_t minCol = board.getColumnSize(), maxCol = -1;

            bool hasCards = false;
            for (int16_t i = 0; i < board.getRowSize(); ++i) {
                for (int16_t j = 0; j < board.getColumnSize(); ++j) {
                    if (!board[{i, j}].empty()) {
                        hasCards = true;
                        minRow = std::min(minRow, i);
                        maxRow = std::max(maxRow, i);
                        minCol = std::min(minCol, j);
                        maxCol = std::max(maxCol, j);
                    }
                }
            }

            if (hasCards) {
                isInPlayableArea = (row >= minRow - 1 && row <= maxRow + 1 &&
                    col >= minCol - 1 && col <= maxCol + 1);
            }
        }

        bool canPlace = board.canBePlaced(row, col) && isInPlayableArea;
        bool isEmpty = board[{row, col}].empty();

        // Style the button
        styleCellButton(cellButton.get(), canPlace, isEmpty, isInPlayableArea);

        // Setup icon if cell contains a card
        if (!isEmpty) {
            setupButtonIcon(cellButton.get(), board[{row, col}].back());
        }

        // Get raw pointer before moving to container
        QPushButton* rawPtr = cellButton.get();

        // Store in smart pointer container
        cellButtons[row][col] = std::move(cellButton);

        // Add to Qt layout system - this automatically manages the widget
        if (gridLayout) {
            gridLayout->addWidget(rawPtr, row, col);
            qDebug() << "Added button to grid layout at (" << row << "," << col << ")";
        }
        else {
            qDebug() << "ERROR: gridLayout is null!";
            return;
        }

        // Connect signal for relevant positions AFTER adding to layout
        if (canPlace || !isEmpty) {
            connect(rawPtr, &QPushButton::clicked, [this, row, col]() {
                qDebug() << "Button clicked at (" << row << "," << col << ")";
                emit cellClicked(row, col);
                });
        }

        // Re-enable signals and show the button ONLY after everything is ready
        rawPtr->blockSignals(false);
        rawPtr->setVisible(true);

        qDebug() << "Successfully created and configured button at (" << row << "," << col << ")";

    }
    catch (const std::exception& e) {
        qDebug() << "Exception in createCellButton for (" << row << "," << col << "):" << e.what();
    }
    catch (...) {
        qDebug() << "Unknown exception in createCellButton for (" << row << "," << col << ")";
    }
}
void BoardView::updateView() {
    // Prevent multiple simultaneous updates
    static bool isUpdating = false;
    if (isUpdating) {
        qDebug() << "BoardView update already in progress, skipping";
        return;
    }
    isUpdating = true;

    qDebug() << "Starting updateView - current board size: " << board.getRowSize() << "x" << board.getColumnSize();

    try {
        // Disable updates to prevent flashing during reconstruction
        this->setUpdatesEnabled(false);

        // First, safely remove all widgets from the layout
        QLayoutItem* item;
        while ((item = gridLayout->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->hide(); // Hide immediately to prevent flash
                widget->disconnect();
                widget->setParent(nullptr);
                widget->deleteLater();
            }
            delete item;
        }

        // Clear smart pointer containers
        for (auto& row : cellButtons) {
            for (auto& button : row) {
                if (button) {
                    button->disconnect();
                    button.reset();
                }
            }
            row.clear();
        }
        cellButtons.clear();

        qDebug() << "Cleared existing widgets and containers";

        // Resize containers for new board size
        cellButtons.resize(board.getRowSize());
        for (auto& row : cellButtons) {
            row.resize(board.getColumnSize());
        }

        // Create new buttons in batch - they start hidden
        for (int row = 0; row < board.getRowSize(); ++row) {
            for (int col = 0; col < board.getColumnSize(); ++col) {
                createCellButton(row, col);
            }
        }

        // Force layout update
        gridLayout->activate();
        this->adjustSize();

        // Re-enable updates and refresh
        this->setUpdatesEnabled(true);
        this->update();

        qDebug() << "Grid updated successfully with size: " << board.getRowSize() << "x" << board.getColumnSize();

    }
    catch (const std::exception& e) {
        qDebug() << "Exception in updateView: " << e.what();
        // Re-enable updates even if there was an error
        this->setUpdatesEnabled(true);
    }

    // Reset the updating flag
    isUpdating = false;
}
bool BoardView::canPlaceCard(const SimpleCard& card, int row, int col) const {
    Board::Position pos = { row, col };

    // Verificăm adiacența și dacă carta poate fi pusă deasupra
    return board.canBePlaced(row, col) && board.canBePushed(card, pos);
}

void BoardView::placeCard(const SimpleCard& card, int row, int col) {
    // Check bounds
    if (row < 0 || row >= static_cast<int>(cellButtons.size()) ||
        col < 0 || col >= static_cast<int>(cellButtons[row].size())) {
        qDebug() << "Invalid position for placeCard: (" << row << "," << col << ")";
        return;
    }

    // Get the button from smart pointer container
    if (cellButtons[row][col]) {
        QPushButton* cellButton = cellButtons[row][col].get();
        setupButtonIcon(cellButton, card);
    }

    qDebug() << "Card visual update completed at position (" << row << "," << col << ")";
}
Board& BoardView::getBoard()
{
    return board;
}

int BoardView::getMaxSize()
{
    return maxSize;
}

void BoardView::setIsMaxSize(bool isMaxSized)
{
    this->isMaxSize = isMaxSized;
}

bool BoardView::getIsMaxSize() const
{
    return isMaxSize;
}