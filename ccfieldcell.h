#ifndef CCFIELDCELL_H
#define CCFIELDCELL_H

#include "../GameGridLibrary/cellcomponent.h"

#include <QPainter>

class GameGrid;
class GridCell;

class CCFieldCell : public CellComponent
{
public:
    CCFieldCell();

    std::shared_ptr<GridCell> cell() const;
    void setCell(const std::shared_ptr<GridCell> &newCell);

    int neighboringBombs() const { return m_neighboringBombs; }

    int evaluate(GameGrid &grid);

    bool paint(QPainter &paint, const QRect &rect);

private:
    int m_neighboringBombs;
    std::shared_ptr<GridCell> m_cell;
};

#endif // CCFIELDCELL_H
