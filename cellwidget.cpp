#include "ccbomb.h"
#include "ccfieldcell.h"
#include "ccmarker.h"
#include "cellwidget.h"

#include "../GameGridLibrary/gridcell.h"

#include <iostream>
#include <QPainter>

CellWidget::CellWidget(QWidget *parent)
    : QAbstractButton{parent}
    , m_size(80)
{
    setFixedSize(m_size, m_size);
    connect(this, SIGNAL(pressed()), SLOT(update()));
}

CellWidget::CellWidget(const int size, QWidget *parent)
    : QAbstractButton{parent}
    , m_size(size)
{
    setFixedSize(m_size, m_size);
}

CellWidget::~CellWidget()
{

}

void CellWidget::paintUnrevealedCell(QPainter &paint, const QRect &rect)
{
    paint.save();

    QColor backgroundColor(Qt::white);
    if (this->isDown())
        backgroundColor = Qt::lightGray;

    // Draw background
    paint.setBrush(backgroundColor);
    paint.drawRect(this->rect());

    if (this->isDown()) {
        paint.restore();
        return; // EXIT: Don't render the edges to give a "pressed" appearance
    }

    // Draw button borders
    int depth = 3;
    // Left border
    paint.setBrush(Qt::lightGray);
    paint.drawPolygon(QVector<QPoint>{
                          { 0, 0 },
                          { depth, depth },
                          { depth, height() - depth },
                          { 0, height() }
                      });
    // Right Border
    paint.setBrush(Qt::darkGray);
    paint.drawPolygon(QVector<QPoint>{
                          { 0, height() },
                          { depth, height() - depth },
                          { width() - depth, height() - depth },
                          { width(), height() }
                      });

    paint.restore();
}

void CellWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event && event->button() == Qt::RightButton
            && this->rect().contains(event->pos())) {
        emit rightClicked();
        update();
    }

    QAbstractButton::mouseReleaseEvent(event);
}

void CellWidget::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    if (!cell()) {
        paint.setBrush(Qt::magenta);
        paint.drawRect(this->rect());
        return;
    }

    paint.setPen(Qt::transparent);

    auto field = cell()->getComponent<CCFieldCell>();
    if (field && field->paint(paint, this->rect())) {
        return;
    }

    auto bomb = cell()->getComponent<CCBomb>();
    if (bomb && bomb->paint(paint, this->rect())) {
        return;
    }

    // Draw unrevealed cell
    paintUnrevealedCell(paint, this->rect());

    auto marker = cell()->getComponent<CCMarker>();
    if (marker) {
         marker->paint(paint, this->rect());
    } else if (bomb) {
        // Debug bomb preview
//        paint.setBrush(QColor(255, 199, 199));
//        paint.drawEllipse(rect().center(), width()/4, height()/4);
    }
}

void CellWidget::setCell(std::shared_ptr<GridCell> newCell)
{
    m_cell = newCell;
}
