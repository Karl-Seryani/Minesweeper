#include "Minesweeper.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Minesweeper game(10, 30, 40);
    game.show();

    return app.exec();
}
