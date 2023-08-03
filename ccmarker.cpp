#include "ccmarker.h"
#include <iostream>

CCMarker::CCMarker()
    : CellComponent {}
    , m_currentState(State::None)
{

}

CCMarker::State CCMarker::cycle()
{
    switch (m_currentState) {
    case State::None:
        setCurrentState(State::Flag);
        break;
    case State::Flag:
        setCurrentState(State::Question);
        break;
    case State::Question:
        setCurrentState(State::None);
        break;
    default:
        break;
    }

    return m_currentState;
}

bool CCMarker::paint(QPainter &paint, const QRect &rect)
{
    paint.save();

    int w = rect.width();
    int h = rect.height();

    switch (currentState()) {
    case Flag:
        // Draw flag
        paint.setBrush(QColor(255, 127, 0));
        paint.drawPolygon(QVector<QPoint>{
                              { w * 2/10, h * 3/10 },
                              { w * 6/10, h * 1/10 },
                              { w * 6/10, h * 5/10 },
                          });

        // Draw pole
        paint.setBrush(Qt::black);
        paint.drawRect(w * 6/10, h/10, w/10, h * 8/10);

        break;
    case Question:
        paint.setPen(Qt::darkBlue);
        paint.setFont(QFont("helvetica", h * 2/3, QFont::Bold));
        paint.drawText(rect, Qt::AlignCenter, "?");

        break;
    default:
        break;
    }

    paint.restore();
    return true;
}

void CCMarker::setCurrentState(CCMarker::State newCurrentState)
{
    m_currentState = newCurrentState;
}


