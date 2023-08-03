#ifndef CCBOMB_H
#define CCBOMB_H

#include "../GameGridLibrary/cellcomponent.h"

#include <QPainter>

/// Cell Component: Bomb
class CCBomb : public CellComponent
{
public:
    CCBomb();

    void detonate();
    bool isDetonated() const { return m_detonated; }

    void disarm();
    bool isDisarmed() const { return m_disarmed; }

    bool paint(QPainter &paint, const QRect &rect);

private:
    bool m_detonated;
    bool m_disarmed;
};

#endif // CCBOMB_H
