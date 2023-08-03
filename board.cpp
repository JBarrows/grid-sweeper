#include "board.h"
#include "ccbomb.h"
#include "ccfieldcell.h"
#include "ccmarker.h"
#include "ccrenderwidget.h"
#include "gridbuilder.h"

#include "../GameGridLibrary/gridcell.h"

#include <QPaintEvent>
#include <QPainter>

const int CELL_SIZE(32);

Board::Board(QWidget *parent)
    : QWidget{parent}
    , m_builder(nullptr)
    , m_grid(true)
{
    setEnabled(false);

    m_builder = new GridBuilder(&m_grid, this);
    connect(m_builder, &GridBuilder::cellAdded, this, &Board::gridBuilder_onCellAdded);
    connect(m_builder, &GridBuilder::gridUpdate, this, &Board::positionBoard);
    connect(m_builder, &GridBuilder::finished, this, &Board::gridBuilder_onFinished);
    m_builder->start();
}

void Board::positionBoard() {
    setMinimumWidth(m_grid.width() * CELL_SIZE);
    setMinimumHeight(m_grid.height() * CELL_SIZE);
    for (int x = m_grid.minX(); x <= m_grid.maxX(); ++x) {
        for (int y = m_grid.minY(); y <= m_grid.maxY(); ++y) {
            positionCell(x, y);
        }
    }
    update();
}

void Board::positionCell(int x, int y)
{
    CellWidget *widget = nullptr;
    auto component = m_grid.getComponentAt<CCRenderWidget>(x, y);
    if (component)
        widget = component->widget();

    if (!widget)
        return;

    widget->move((x - m_grid.minX()) * widget->width(),
                 (m_grid.maxY() - y) * widget->height());
    widget->show();
}

void Board::gridBuilder_onCellAdded(CellWidget* cell) {
    m_cells << cell;

    connect(cell, &CellWidget::clicked,
            this, &Board::cellWidget_onClicked);
    connect(cell, &CellWidget::rightClicked,
            this, &Board::cellWidget_onRightClicked);
}

void Board::gridBuilder_onFinished() {
    if (m_builder) {
        m_builder->deleteLater();
    }

    revealCell(m_grid.getCell(0, 0));
    this->setEnabled(true);
}

void Board::revealCell(std::shared_ptr<GridCell> cell)
{
    if (!cell)
        return;

    auto fieldCell = std::make_shared<CCFieldCell>();
    cell->addComponent(fieldCell);
    fieldCell->setCell(cell);
    fieldCell->evaluate(m_grid);
    update();
}

void Board::cellWidget_onClicked()
{
    CellWidget* widget = qobject_cast<CellWidget*>(sender());
    std::shared_ptr<GridCell> cell;
    if (widget)
        cell = widget->cell();

    if (!cell)
        return;

    auto fieldCell = cell->getComponent<CCFieldCell>();
    if (fieldCell) {
        return; // EXIT: Do nothing, it's an empty field
    }

    auto marker = cell->getComponent<CCMarker>();
    if (marker && marker->currentState() == CCMarker::Flag) {
        return; // EXIT: Don't allow revealing flagged squares
    }

    auto bomb = cell->getComponent<CCBomb>();
    if (bomb) {
        std::cout << "BOOM" << std::endl;
        bomb->detonate();
        setEnabled(false);
        update();
        return; // EXIT: Blew up a bomb
    }

    revealCell(cell);

    // Check for win
    checkVictory();

}

void Board::cellWidget_onRightClicked() {
    CellWidget* widget = qobject_cast<CellWidget*>(sender());
    std::shared_ptr<GridCell> cell;
    if (widget)
        cell = widget->cell();

    if (!cell)
        return;

    auto marker = cell->getComponent<CCMarker>();
    if (!marker) {
        marker = std::make_shared<CCMarker>();
        cell->addComponent(marker);
    }

    if (marker) {
        marker->cycle();
    }
}

void Board::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setBrush(QBrush(Qt::black));
    paint.drawRect(this->rect());
}

void Board::checkVictory()
{
    for (int x = m_grid.minX(); x <= m_grid.maxX(); ++x) {
        for (int y = m_grid.minY(); y <= m_grid.maxY(); ++y) {
            auto cell = m_grid.getCell(x, y);
            if (!cell)
                continue;

            if (cell->components().size() <= 1) {
                // Just a widget, no bomb or revealed field
                // That means this is still OK to click on, so it's not a win yet
                return;
            }
        }
    }

    std::cout << "VICTORY!" << std::endl;
    setEnabled(false);
    update();
}

