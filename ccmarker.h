#ifndef CCMARKER_H
#define CCMARKER_H

#include "../GameGridLibrary/cellcomponent.h"

#include <QPainter>

class CCMarker : public CellComponent
{
public:
    enum State {
        None,
        Flag,
        Question
    };

    CCMarker();

    CCMarker::State currentState() const { return m_currentState; }
    void setCurrentState(CCMarker::State newCurrentState);

    CCMarker::State cycle();

    bool paint(QPainter &paint, const QRect &rect);

private:
    CCMarker::State m_currentState;
};

#endif // CCMARKER_H
