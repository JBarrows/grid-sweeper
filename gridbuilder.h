#ifndef GRIDBUILDER_H
#define GRIDBUILDER_H

#include "../GameGridLibrary/gamegrid.h"

#include <QObject>

class CellWidget;

class GridBuilder : public QObject
{
    Q_OBJECT
public:
    explicit GridBuilder(GameGrid* grid, QObject *parent = nullptr);

    void start();

signals:
    void cellAdded(CellWidget* cell);
    void finished();
    void gridUpdate();

private slots:
    std::shared_ptr<GridCell> addCell(int x, int y);
    void addNextCells();

private:
    float m_bombChance = 0.1f;
    int m_bombsToAdd = 10;
    QList<std::shared_ptr<GridCell>> m_frontier;
    GameGrid* m_grid;
    int m_maxDistance;

    void considerCellAt(int x, int y);
};

#endif // GRIDBUILDER_H
