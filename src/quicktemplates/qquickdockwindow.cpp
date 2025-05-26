#include "qquickdockwindow_p.h"
#include "qquickdockpanel_p.h"
#include <QDebug>

QQuickItem *QQuickDockWindow::dockPanel() const
{
    return _dockPanels;
}

void QQuickDockWindow::setDockPanel(QQuickItem *dockPanel)
{
    _dockPanels = dockPanel;

    _dockPanels->setParentItem(contentItem());
    _dockPanels->setPosition(QPointF(0, 0));
    _dockPanels->setSize(size());
    _dockPanels->setVisible(true);
}

QQuickDockWindow::QQuickDockWindow(QWindow *parent) : QQuickWindow(parent)
{
    setFlags(Qt::FramelessWindowHint | Qt::Tool);
}

void QQuickDockWindow::resizeEvent(QResizeEvent *)
{
    _dockPanels->setSize(size());
}

bool QQuickDockWindow::event(QEvent *event)
{
    // if (event->type() == QEvent::Expose)
    // if (isExposed())
    // startSystemMove();

    return QQuickWindow::event(event);
}
void QQuickDockWindow::moveEvent(QMoveEvent *move_event)
{
    qDebug() << "QQuickDockWindow::moveEvent" << move_event->pos();
    QQuickWindow::moveEvent(move_event);
}
