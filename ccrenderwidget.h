#ifndef CCRENDERWIDGET_H
#define CCRENDERWIDGET_H

#include "../GameGridLibrary/cellcomponent.h"

#include "cellwidget.h"

#include <QWidget>

class CCRenderWidget : public CellComponent
{
public:
    CCRenderWidget(const int size, QWidget *parent);

    CellWidget* widget() const { return m_widget; }

private:
    CellWidget* m_widget;

};

#endif // CCRENDERWIDGET_H
