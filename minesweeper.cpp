#include "Minesweeper.h"
#include <QGridLayout>
#include <QMessageBox>
#include <algorithm>
#include <random>
#include <QApplication>

Minesweeper::Minesweeper(int rows, int cols, int mines, QWidget *parent)
    : QWidget(parent), rows(rows), cols(cols), mineCount(mines) {
    setupGrid();
    placeMines();
}

void Minesweeper::setupGrid() {
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);

    for (int row = 0; row < rows; ++row) {
        std::vector<CellButton*> rowButtons;
        for (int col = 0; col < cols; ++col) {
            CellButton *button = new CellButton(row, col, this);
            button->setFixedSize(30, 30);
            layout->addWidget(button, row, col);

            // Connect custom signals
            connect(button, &CellButton::leftClicked, this, &Minesweeper::handleLeftClick);
            connect(button, &CellButton::rightClicked, this, &Minesweeper::handleRightClick);

            rowButtons.push_back(button);
        }
        grid.push_back(rowButtons);
    }

    setLayout(layout);
    setWindowTitle("Minesweeper");
}

void Minesweeper::placeMines() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, rows - 1);
    std::uniform_int_distribution<> colDist(0, cols - 1);

    while (mines.size() < static_cast<size_t>(mineCount)) {
        int row = rowDist(gen);
        int col = colDist(gen);

        if (std::find(mines.begin(), mines.end(), std::make_pair(row, col)) == mines.end()) {
            mines.emplace_back(row, col);
            grid[row][col]->setMine(true);
        }
    }
}

int Minesweeper::countAdjacentMines(int row, int col) {
    int count = 0;
    for (int r = -1; r <= 1; ++r) {
        for (int c = -1; c <= 1; ++c) {
            int newRow = row + r;
            int newCol = col + c;

            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                if (grid[newRow][newCol]->isMine()) {
                    count++;
                }
            }
        }
    }
    return count;
}

void Minesweeper::handleLeftClick() {
    CellButton *button = qobject_cast<CellButton*>(sender());
    if (!button || button->isRevealed()) return;

    if (button->markingState() == CellButton::Unmarked) {
        if (button->isMine()) {
            revealAllMines();
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Game Over", "You hit a mine! Try again?",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                resetGame();
            } else {
                close();
            }
            return;
        } else {
            revealCell(button->row(), button->col());
            checkWinCondition();
        }
    }
}

void Minesweeper::handleRightClick() {
    CellButton *button = qobject_cast<CellButton*>(sender());
    if (button && !button->isRevealed()) {
        button->cycleMarkingState();
    }
}

void Minesweeper::revealCell(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols || grid[row][col]->isRevealed())
        return;

    // Reveal the cell even if it's marked
    grid[row][col]->reveal();
    int adjacentMines = countAdjacentMines(row, col);

    if (adjacentMines > 0) {
        grid[row][col]->setText(QString::number(adjacentMines));
    } else {
        for (int r = -1; r <= 1; ++r) {
            for (int c = -1; c <= 1; ++c) {
                revealCell(row + r, col + c);
            }
        }
    }
}
void Minesweeper::revealAllMines() {
    for (const auto &mine : mines) {
        grid[mine.first][mine.second]->reveal();
        grid[mine.first][mine.second]->setText("M");
    }
}

void Minesweeper::resetGame() {
    // Reset all cells
    for (auto &row : grid) {
        for (auto &cell : row) {
            cell->reset(); // Reset each cell
        }
    }

    // Clear the mines and place new ones
    mines.clear();
    placeMines();
}

void Minesweeper::checkWinCondition() {
    int revealedCells = 0;

    for (const auto &row : grid) {
        for (const auto &cell : row) {
            if (cell->isRevealed() && !cell->isMine()) {
                revealedCells++;
            }
        }
    }

    if (revealedCells == (rows * cols - mineCount)) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Congratulations", "You won! Try again?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            resetGame();
        } else {
            close();
        }
    }
}
