#include "board.h"
#include "mainwindow.h"

#include <QMenuBar>

Board* m_board;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar* menu = new QMenuBar(this);
    menu->addAction("Reset", this, &MainWindow::reset);
    setMenuWidget(menu);

    reset();
}

MainWindow::~MainWindow()
{
}

void MainWindow::reset()
{
    if (m_board)
        m_board->deleteLater();

    m_board = new Board(this);
    setCentralWidget(m_board);
}

