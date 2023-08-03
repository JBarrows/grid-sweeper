#include "ccrenderwidget.h"

#include <iostream>

CCRenderWidget::CCRenderWidget(const int size, QWidget *parent)
    : CellComponent{}
{
    m_widget = new CellWidget(size, parent);
}
