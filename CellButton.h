#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#include <QPushButton>

class CellButton : public QPushButton {
    Q_OBJECT

public:
    enum MarkingState {
        Unmarked,
        Flagged,
        QuestionMark
    };

    CellButton(int row, int col, QWidget *parent = nullptr);
    int row() const { return m_row; }
    int col() const { return m_col; }
    bool isMine() const { return m_isMine; }
    void setMine(bool mine) { m_isMine = mine; }
    bool isRevealed() const { return m_isRevealed; }
    void reveal();
    void cycleMarkingState();
    void reset();
    MarkingState markingState() const { return m_markingState; }

signals:
    void leftClicked();
    void rightClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    int m_row, m_col;
    bool m_isMine;
    bool m_isRevealed;
    MarkingState m_markingState;
};

#endif // CELLBUTTON_H
