#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QWidget>
#include <QGridLayout>
#include <QMessageBox>
#include <vector>
#include <utility>
#include "CellButton.h"

class Minesweeper : public QWidget {
    Q_OBJECT

public:
    Minesweeper(int rows, int cols, int mines, QWidget *parent = nullptr);

private:
    int rows, cols, mineCount;
    std::vector<std::vector<CellButton*>> grid;
    std::vector<std::pair<int, int>> mines;

    void setupGrid();
    void placeMines();
    int countAdjacentMines(int row, int col);
    void revealCell(int row, int col);
    void revealAllMines();
    void resetGame();
    void checkWinCondition();

private slots:
    void handleLeftClick();  // Slot for handling left-clicks
    void handleRightClick(); // Slot for handling right-clicks
};

#endif // MINESWEEPER_H
