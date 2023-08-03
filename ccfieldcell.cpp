#include "ccbomb.h"
#include "ccfieldcell.h"

#include "../GameGridLibrary/gridcell.h"
#include "../GameGridLibrary/gamegrid.h"

#include <QList>
#include <QPoint>

CCFieldCell::CCFieldCell()
    : CellComponent{}
    , m_neighboringBombs(-1)
{

}

int CCFieldCell::evaluate(GameGrid &grid)
{
    if (!m_cell)
        return -1;

    QList<std::shared_ptr<GridCell>> emptyNeighbors;
    int bombCount = 0;
    int x = m_cell->x();
    int y = m_cell->y();
    QList<QPoint> points {
        {x - 1, y + 1}, {x, y + 1}, {x + 1, y + 1},
        {x - 1, y    },             {x + 1, y    },
        {x - 1, y - 1}, {x, y - 1}, {x + 1, y - 1}
    };

    for (auto pt : points) {
        auto neighbor = grid.getCell(pt.x(), pt.y());
        if (!neighbor)
            continue;

        auto bomb = neighbor->getComponent<CCBomb>();
        if (bomb) {
            ++bombCount;
            continue;
        }

        auto fieldCell = neighbor->getComponent<CCFieldCell>();
        if (fieldCell) {
            continue;
        }

        // Save empty neighbors for later
        emptyNeighbors << neighbor;
    }

    m_neighboringBombs = bombCount;

    if (m_neighboringBombs == 0) {
        // No bombs nearby so we can reveal all empty neighbors
        for (auto neighbor : emptyNeighbors) {
            auto fieldCell = neighbor->getComponent<CCFieldCell>();
            if (fieldCell)
                continue; // SKIP: Must have become a field since it was added to this list

            fieldCell = std::make_shared<CCFieldCell>();
            neighbor->addComponent(fieldCell);
            fieldCell->setCell(neighbor);
            fieldCell->evaluate(grid);
        }
    }

    return m_neighboringBombs;
}

bool CCFieldCell::paint(QPainter &paint, const QRect &rect)
{
    paint.save();

    // Draw background
    paint.setBrush(QColor(199, 255, 199));
    paint.drawRect(rect);

    if (neighboringBombs() > 0) {
        // Draw number of nearby hazards
        QColor hintColor;
        hintColor.setHslF((6 + neighboringBombs()) / 14.0, 1.0, 0.3);
        paint.setPen(hintColor);
        paint.setFont(QFont("helvetica", rect.height() * 2/3, QFont::Bold));
        paint.drawText(rect, Qt::AlignCenter, QString::number(neighboringBombs()));
    }

    paint.restore();
    return true;
}

std::shared_ptr<GridCell> CCFieldCell::cell() const
{
    return m_cell;
}

void CCFieldCell::setCell(const std::shared_ptr<GridCell> &newCell)
{
    m_cell = newCell;
}


