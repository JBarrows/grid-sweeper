#include "ccbomb.h"
#include "ccrenderwidget.h"
#include "gridbuilder.h"

#include <QRandomGenerator>
#include <QTimer>

GridBuilder::GridBuilder(GameGrid *grid, QObject *parent)
    : QObject{parent}
    , m_bombChance(0.0f)
    , m_bombsToAdd(20)
    , m_grid(grid)
    , m_maxDistance(m_bombsToAdd)
{

}

void GridBuilder::start()
{
    // Make sure that the origin cell (0, 0) is empty and has empty neighbors
    addCell(0, 0);
    addCell(0, 1);
    addCell(1, 0);
    addCell(0,-1);
    addCell(-1,0);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GridBuilder::addNextCells);
    timer->start(50);
}

std::shared_ptr<GridCell> GridBuilder::addCell(int x, int y)
{
    if (!m_grid)
        return nullptr;

    auto cell = m_grid->addCell(x, y);
    if (cell) {
        m_frontier.append(cell);
        auto widgetComponent = std::make_shared<CCRenderWidget>(32, static_cast<QWidget*>(this->parent()));
        cell->addComponent(widgetComponent);
        if (widgetComponent->widget()) {
            widgetComponent->widget()->setCell(cell);
            emit cellAdded(widgetComponent->widget());
        }
    }

    return cell;
}

void GridBuilder::considerCellAt(int x, int y)
{
    auto n = m_grid->getCell(x, y);
    if (n) {
        return; // EXIT: There's already a cell there
    }

    int distance = std::max(std::abs(x), std::abs(y));
    if (QRandomGenerator::global()->bounded(m_maxDistance) < distance)
        return; // EXIT Escalating chance of not creating a cell at all

    auto cell = addCell(x, y);

    if (m_bombsToAdd > 0 && cell && QRandomGenerator::global()->generateDouble() < m_bombChance) {
        // Add BombComponent
        cell->addComponent(std::make_shared<CCBomb>());
        --m_bombsToAdd;
    }
}

void GridBuilder::addNextCells()
{
    QList<std::shared_ptr<GridCell>> borderCells = m_frontier; // Copy frontier into a new list so we can do just this layer
    m_frontier.clear();
    // For each cell in the frontier, add new neighbors
    while (!borderCells.isEmpty()) {
        auto workingCell = borderCells.takeFirst();
        if (!workingCell)
            break;

        int x = workingCell->x();
        int y = workingCell->y();
        considerCellAt(x, y + 1); // up
        considerCellAt(x + 1, y); // right
        considerCellAt(x, y - 1); // down
        considerCellAt(x - 1, y); // left
    }

    emit gridUpdate();

    if (m_frontier.isEmpty() || m_bombsToAdd == 0) {
        emit finished();
    } else {
        m_bombChance += 0.05;
    }
}
