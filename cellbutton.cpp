#include "CellButton.h"
#include <QMouseEvent> // Include for mouse events

CellButton::CellButton(int row, int col, QWidget *parent)
    : QPushButton(parent), m_row(row), m_col(col), m_isMine(false), m_isRevealed(false), m_markingState(Unmarked) {
    setStyleSheet("background-color: lightgray; border: 1px solid black;");
}

void CellButton::reveal() {
    m_isRevealed = true;
    setEnabled(false);
    setStyleSheet("background-color: white; border: 1px solid black;");
}

void CellButton::reset() {
    m_isRevealed = false;
    m_isMine = false;
    m_markingState = Unmarked;
    setText("");
    setEnabled(true);
    setStyleSheet("background-color: lightgray; border: 1px solid black;");
}

void CellButton::cycleMarkingState() {
    if (m_isRevealed) return; // Do not allow marking revealed cells

    switch (m_markingState) {
    case Unmarked:
        m_markingState = Flagged;
        setText("F");
        setStyleSheet("background-color: red; color: white; border: 1px solid black;");
        break;
    case Flagged:
        m_markingState = QuestionMark;
        setText("?");
        setStyleSheet("background-color: yellow; color: black; border: 1px solid black;");
        break;
    case QuestionMark:
        m_markingState = Unmarked;
        setText("");
        setStyleSheet("background-color: lightgray; border: 1px solid black;");
        break;
    }
}

void CellButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(); // Emit signal for left-click
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(); // Emit signal for right-click
    }
    QPushButton::mousePressEvent(event); // Call base class implementation
}
