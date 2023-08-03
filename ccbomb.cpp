#include "ccbomb.h"

CCBomb::CCBomb()
    : CellComponent {}
    , m_detonated(false)
    , m_disarmed(false)
{

}

void CCBomb::detonate()
{
    if (m_detonated)
        return;

    m_detonated = true;
}

void CCBomb::disarm()
{
    if (m_disarmed)
        return;

    m_disarmed = true;
}

bool CCBomb::paint(QPainter &paint, const QRect &rect)
{
    QColor backgroundColor(Qt::white);
    QColor bombColor(Qt::black);
    // Draw bomb if detonated (or disarmed)
    if (isDetonated()) {
        backgroundColor = QColor(255, 199, 199);
        bombColor = QColor(159, 31, 0);
    } else if (isDisarmed()) {
        backgroundColor = QColor(199, 255, 199);
        bombColor = QColor(31, 159, 63);
    } else {
        return false;
    }

    paint.setBrush(backgroundColor);
    paint.drawRect(rect);

    paint.setBrush(bombColor);
    paint.drawEllipse(rect.center(), rect.width()/4, rect.height()/4);

    return true;
}




