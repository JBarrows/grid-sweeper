#ifndef BOARD_H
#define BOARD_H

#include "../GameGridLibrary/gamegrid.h"

#include "cellwidget.h"
#include "gridbuilder.h"

#include <QWidget>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

public slots:
    void positionBoard();
    void positionCell(int x, int y);

signals:

private slots:
    void cellWidget_onClicked();

    void cellWidget_onRightClicked();

    void gridBuilder_onCellAdded(CellWidget *cell);

    void gridBuilder_onFinished();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

private:
    GridBuilder* m_builder;
    QList<CellWidget*> m_cells;
    GameGrid m_grid;

    void checkVictory();

    void revealCell(std::shared_ptr<GridCell> cell);
};

#endif // BOARD_H
