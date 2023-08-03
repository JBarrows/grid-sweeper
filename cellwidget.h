#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <memory>
#include <QAbstractButton>
#include <QMouseEvent>

class GridCell;

class CellWidget : public QAbstractButton
{
    Q_OBJECT
public:
    explicit CellWidget(QWidget *parent = nullptr);
    CellWidget(const int size, QWidget *parent = nullptr);
    ~CellWidget();

    std::shared_ptr<GridCell> cell() const { return m_cell; }
    void setCell(std::shared_ptr<GridCell> newCell);

signals:
    void rightClicked();

private:
    int m_size;
    std::shared_ptr<GridCell> m_cell;

    void paintUnrevealedCell(QPainter &paint, const QRect &rect);

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);
};

#endif // CELLWIDGET_H
