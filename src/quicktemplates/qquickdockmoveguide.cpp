// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qquickdockmoveguide_p.h"
#include "qquickdockmoveguide_p_p.h"

#include <QtCore/QCoreApplication>
#include <QPainter>
#include <QQuickWindow>
#include <QScreen>
#include <private/qquickitem_p.h>
#include <atomic>

#include "qquickdockcontainer_p.h"
#include "qquickdeferredexecute_p_p.h"
#include "qquickdockcontainer_p_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmltype DockMoveGuide
    \instantiates QQuickDockMoveGuide
    \inqmlmodule QtQuick.Templates
    \since 6.8
    \brief Provides visual docking guides during drag operations of dock widgets.

 DockMoveGuide is used internally by \l DockContainer to display docking
 indicators when the user drags a dock widget. It shows visual hints on
 potential docking targets around the container, such as edges or panel areas.

 The guide highlights a target docking area with a \c highlightRect and shows
 interactive indicators for each dockable region: center panel, sides of the panel,
 and edges of the main window. These indicators can be customized via
 the respective \c panel* and \c window* properties, each of which accepts
 a \c QQuickItem.

 The \c detached property indicates whether the widget is currently detached
 (floating) or docked.

 The \c currentSide property indicates the currently highlighted docking
 side or panel area.

 \section2 Properties

 \qmlproperty rect DockMoveGuide::highlightRect
 The rectangle highlighting the current docking target area.

 \qmlproperty bool DockMoveGuide::detached
 Indicates whether the dragged widget is detached (floating).

 \qmlproperty Item DockMoveGuide::panelCenter
 The visual item representing the center docking panel.

 \qmlproperty Item DockMoveGuide::panelTop
 The visual item representing the top docking panel.

 \qmlproperty Item DockMoveGuide::panelRight
 The visual item representing the right docking panel.

 \qmlproperty Item DockMoveGuide::panelBottom
 The visual item representing the bottom docking panel.

 \qmlproperty Item DockMoveGuide::panelLeft
 The visual item representing the left docking panel.

 \qmlproperty Item DockMoveGuide::windowTop
 The visual item representing the top edge of the window docking area.

 \qmlproperty Item DockMoveGuide::windowRight
 The visual item representing the right edge of the window docking area.

 \qmlproperty Item DockMoveGuide::windowBottom
 The visual item representing the bottom edge of the window docking area.

 \qmlproperty Item DockMoveGuide::windowLeft
 The visual item representing the left edge of the window docking area.

 \qmlproperty enumeration DockMoveGuide::currentSide
 The current highlighted docking side or panel area.

 \section2 Example

 The following example shows how to customize DockMoveGuide indicators:

 \qml
 import QtQuick
 import QtQuick.Controls.impl
 import QtQuick.Templates as T

 T.DockMoveGuide {
     id: control
     implicitHeight: 300
     implicitWidth: 250

     Rectangle {
         x: control.highlightRect.x
         y: control.highlightRect.y
         width: control.highlightRect.width
         height: control.highlightRect.height

         Behavior on x { NumberAnimation{} }
         Behavior on y { NumberAnimation{} }
         Behavior on width { NumberAnimation{} }
         Behavior on height { NumberAnimation{} }

         color: 'blue'
     }

     panelCenter: Rectangle {
         implicitHeight: 30
         implicitWidth: 30
         Rectangle {
             height: 20
             width: 20
             anchors.centerIn: parent
         }
     }
     // Similarly customize panelLeft, panelRight, panelTop, panelBottom,
     // windowLeft, windowRight, windowTop, windowBottom
 }
 \endqml

 \sa DockContainer, DockPanel
*/

QQuickDockMoveGuide::QQuickDockMoveGuide(QQuickItem *parent)
    : QQuickItem(*(new QQuickDockMoveGuidePrivate), parent)
{
    setCursor(Qt::ArrowCursor);
    setZ(500);
}

void QQuickDockMoveGuide::componentComplete()
{
    Q_D(QQuickDockMoveGuide);

    if (!d->parentDockContainer) {
        auto p = qobject_cast<QQuickDockContainer *>(parentItem());
        if (p)
            d->parentDockContainer = p;
    }

    d->panelCenter->setParentItem(this);
    d->panelTop->setParentItem(this);
    d->panelRight->setParentItem(this);
    d->panelBottom->setParentItem(this);
    d->panelLeft->setParentItem(this);
    d->windowTop->setParentItem(this);
    d->windowRight->setParentItem(this);
    d->windowBottom->setParentItem(this);
    d->windowLeft->setParentItem(this);

    if (d->parentDockContainer) {
        auto checkAllowDetach = [d]() {
            if (QQuickDockContainerPrivate::get(d->parentDockContainer)->usePopupWindow())
                d->detach();
            else
                d->attach();
        };
        connect(d->parentDockContainer, &QQuickDockContainer::popupTypeChanged, this,
                checkAllowDetach);
        checkAllowDetach();
    }
    QQuickItem::componentComplete();
}

QRectF QQuickDockMoveGuide::areaRect() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->panelRect;
}

void QQuickDockMoveGuide::setAreaRect(const QRectF &area)
{
    Q_D(QQuickDockMoveGuide);
    d->setPanelRect(area);
}

void QQuickDockMoveGuide::begin()
{
    Q_D(QQuickDockMoveGuide);

    d->panelRect = QRectF{};
    d->begin();
}

void QQuickDockMoveGuide::end()
{
    Q_D(QQuickDockMoveGuide);
    d->hide();
}

QQuickDockMoveGuide::DockSide QQuickDockMoveGuide::currentSide() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->currentSide;
}

QPointF QQuickDockMoveGuide::mousePos() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->mousePos;
}

void QQuickDockMoveGuide::setMousePos(const QPointF &mousePos)
{
    Q_D(QQuickDockMoveGuide);
    d->setMousePos(mousePos);
}

QRectF QQuickDockMoveGuide::highlightRect() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->highlightRect;
}

bool QQuickDockMoveGuide::detached() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->detached;
}

void QQuickDockMoveGuide::setDetached(bool newDetached)
{
    Q_D(QQuickDockMoveGuide);
    if (d->detached == newDetached)
        return;
    d->detached = newDetached;

    emit detachedChanged();
}

QQuickItem *QQuickDockMoveGuide::panelCenter() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->panelCenter;
}

void QQuickDockMoveGuide::setPanelCenter(QQuickItem *newPanelCenter)
{
    Q_D(QQuickDockMoveGuide);
    if (d->panelCenter == newPanelCenter)
        return;
    d->panelCenter = newPanelCenter;
    emit panelCenterChanged();
}

QQuickItem *QQuickDockMoveGuide::panelTop() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->panelTop;
}

void QQuickDockMoveGuide::setPanelTop(QQuickItem *newPanelTop)
{
    Q_D(QQuickDockMoveGuide);
    if (d->panelTop == newPanelTop)
        return;
    d->panelTop = newPanelTop;
    emit panelTopChanged();
}

QQuickItem *QQuickDockMoveGuide::panelRight() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->panelRight;
}

void QQuickDockMoveGuide::setPanelRight(QQuickItem *newPanelRight)
{
    Q_D(QQuickDockMoveGuide);
    if (d->panelRight == newPanelRight)
        return;
    d->panelRight = newPanelRight;
    emit panelRightChanged();
}

QQuickItem *QQuickDockMoveGuide::panelBottom() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->panelBottom;
}

void QQuickDockMoveGuide::setPanelBottom(QQuickItem *newPanelBottom)
{
    Q_D(QQuickDockMoveGuide);
    if (d->panelBottom == newPanelBottom)
        return;
    d->panelBottom = newPanelBottom;
    emit panelBottomChanged();
}

QQuickItem *QQuickDockMoveGuide::panelLeft() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->panelLeft;
}

void QQuickDockMoveGuide::setPanelLeft(QQuickItem *newPanelLeft)
{
    Q_D(QQuickDockMoveGuide);
    if (d->panelLeft == newPanelLeft)
        return;
    d->panelLeft = newPanelLeft;
    emit panelLeftChanged();
}

QQuickItem *QQuickDockMoveGuide::windowTop() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->windowTop;
}

void QQuickDockMoveGuide::setWindowTop(QQuickItem *newWindowTop)
{
    Q_D(QQuickDockMoveGuide);
    if (d->windowTop == newWindowTop)
        return;
    d->windowTop = newWindowTop;
    emit windowTopChanged();
}

QQuickItem *QQuickDockMoveGuide::windowRight() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->windowRight;
}

void QQuickDockMoveGuide::setWindowRight(QQuickItem *newWindowRight)
{
    Q_D(QQuickDockMoveGuide);
    if (d->windowRight == newWindowRight)
        return;
    d->windowRight = newWindowRight;
    emit windowRightChanged();
}

QQuickItem *QQuickDockMoveGuide::windowBottom() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->windowBottom;
}

void QQuickDockMoveGuide::setWindowBottom(QQuickItem *newWindowBottom)
{
    Q_D(QQuickDockMoveGuide);
    if (d->windowBottom == newWindowBottom)
        return;
    d->windowBottom = newWindowBottom;
    emit windowBottomChanged();
}

QQuickItem *QQuickDockMoveGuide::windowLeft() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->windowLeft;
}

void QQuickDockMoveGuide::setWindowLeft(QQuickItem *newWindowLeft)
{
    Q_D(QQuickDockMoveGuide);
    if (d->windowLeft == newWindowLeft)
        return;
    d->windowLeft = newWindowLeft;
    emit windowLeftChanged();
}
bool QQuickDockMoveGuide::allowFill() const
{
    Q_D(const QQuickDockMoveGuide);
    return d->allowFill;
}

void QQuickDockMoveGuide::setAllowFill(bool allowFill)
{
    Q_D(QQuickDockMoveGuide);
    if (d->allowFill == allowFill)
        return;

    d->panelCenter->setVisible(allowFill);
    d->allowFill = allowFill;
}

void QQuickDockMoveGuidePrivate::detach()
{
    Q_Q(QQuickDockMoveGuide);
    if (!window) {
        window = new QQuickWindow;
        window->setFlags(window->flags() | Qt::FramelessWindowHint | Qt::BypassWindowManagerHint
                         | Qt::ToolTip);
        window->setColor(Qt::transparent);
        window->setTransientParent(q->window());
    }

    panelCenter->setParentItem(window->contentItem());
    panelTop->setParentItem(window->contentItem());
    panelRight->setParentItem(window->contentItem());
    panelBottom->setParentItem(window->contentItem());
    panelLeft->setParentItem(window->contentItem());
    windowTop->setParentItem(window->contentItem());
    windowRight->setParentItem(window->contentItem());
    windowBottom->setParentItem(window->contentItem());
    windowLeft->setParentItem(window->contentItem());

    detached = true;
}

void QQuickDockMoveGuidePrivate::attach()
{
    Q_Q(QQuickDockMoveGuide);

    panelCenter->setParentItem(q);
    panelTop->setParentItem(q);
    panelRight->setParentItem(q);
    panelBottom->setParentItem(q);
    panelLeft->setParentItem(q);
    windowTop->setParentItem(q);
    windowRight->setParentItem(q);
    windowBottom->setParentItem(q);
    windowLeft->setParentItem(q);

    if (window) {
        window->deleteLater();
        window = nullptr;
    }
    detached = false;
}

void QQuickDockMoveGuidePrivate::updateButtonsVisible()
{

    auto mouseInWindow{ windowRect.isValid() && windowRect.contains(mousePos) };
    auto mouseInPanel{ panelRect.isValid() && panelRect.contains(mousePos) };

    panelCenter->setVisible(mouseInPanel && allowFill);
    panelTop->setVisible(mouseInPanel);
    panelRight->setVisible(mouseInPanel);
    panelBottom->setVisible(mouseInPanel);
    panelLeft->setVisible(mouseInPanel);
    windowTop->setVisible(mouseInWindow);
    windowRight->setVisible(mouseInWindow);
    windowBottom->setVisible(mouseInWindow);
    windowLeft->setVisible(mouseInWindow);

    if (prevMouseInWindow != mouseInWindow || prevMouseInPanel != mouseInPanel)
        updateMaskNeeded = true;
    prevMouseInWindow = mouseInWindow;
    prevMouseInPanel = mouseInPanel;
}

void QQuickDockMoveGuidePrivate::movetoCenter(QQuickItem *btn, qreal x, qreal y)
{
    btn->setPosition(QPointF{ x - btn->size().width() / 2, y - btn->size().height() / 2 });
}

void QQuickDockMoveGuidePrivate::setButtonsPos2(const QRectF &rc, QQuickItem *leftBtn,
                                                QQuickItem *topBtn, QQuickItem *rightBtn,
                                                QQuickItem *bottomBtn, bool panel)
{

    auto center = rc.center();

    if (panel) {
        auto d = buttonSize * 2;
        movetoCenter(leftBtn, center.x() - d, center.y());
        movetoCenter(rightBtn, center.x() + d, center.y());
        movetoCenter(topBtn, center.x(), center.y() - d);
        movetoCenter(bottomBtn, center.x(), center.y() + d);
    } else {
        movetoCenter(leftBtn, rc.left() + buttonSize, center.y());
        movetoCenter(rightBtn, rc.right() - buttonSize, center.y());
        movetoCenter(topBtn, center.x(), rc.top() + buttonSize);
        movetoCenter(bottomBtn, center.x(), rc.bottom() - buttonSize);
    }
    updateMaskNeeded = true;
}

void QQuickDockMoveGuidePrivate::begin()
{
    Q_Q(QQuickDockMoveGuide);
    windowRect.setSize(parentDockContainer->size());
    if (detached) {
        auto windowPos = parentDockContainer->mapToGlobal(0, 0);
        window->setPosition(windowPos.toPoint());
        window->resize(parentDockContainer->size().toSize());
        setButtonsPos2(windowRect, windowLeft, windowTop, windowRight, windowBottom, false);
        updateMask();

        window->show();
    } else {
        windowRect.moveTo(0, 0);
        setButtonsPos2(windowRect, windowLeft, windowTop, windowRight, windowBottom, false);
    }
    q->setSize(parentDockContainer->size());
}

void QQuickDockMoveGuidePrivate::setPanelRect(const QRectF &rc)
{
    if (rc == panelRect)
        return;
    panelRect = rc;

    if (rc.isValid()) {
        setButtonsPos2(rc, panelLeft, panelTop, panelRight, panelBottom, true);
        movetoCenter(panelCenter, rc.center().x(), rc.center().y());
    }
    panelCenter->setVisible(allowFill);
    updateMaskNeeded = true;
}
void QQuickDockMoveGuidePrivate::setHighlightArea()
{
    Q_Q(QQuickDockMoveGuide);

    highlightRect = panelRect;

    switch (currentSide) {

    case QQuickDockMoveGuide::None:
        return;
    case QQuickDockMoveGuide::Fill:
        break;
    case QQuickDockMoveGuide::PanelTop:
        highlightRect.setHeight(std::min(highlightRect.height() / 2, highlightImplicitHeight));
        break;
    case QQuickDockMoveGuide::PanelBottom:
        highlightRect.setHeight(std::min(highlightRect.height() / 2, highlightImplicitHeight));
        highlightRect.moveBottom(panelRect.bottom());
        break;
    case QQuickDockMoveGuide::PanelLeft:
        highlightRect.setWidth(std::min(highlightRect.height() / 2, highlightImplicitHeight));
        break;
    case QQuickDockMoveGuide::PanelRight:
        highlightRect.setWidth(std::min(highlightRect.height() / 2, highlightImplicitHeight));
        highlightRect.moveRight(panelRect.right());
        break;
    case QQuickDockMoveGuide::WindowTop:
        highlightRect = windowRect;
        highlightRect.setHeight(std::min(highlightRect.height() / 2, highlightImplicitHeight));
        break;
    case QQuickDockMoveGuide::WindowBottom:
        highlightRect = windowRect;
        highlightRect.setHeight(std::min(highlightRect.height() / 2, highlightImplicitHeight));
        highlightRect.moveBottom(windowRect.bottom());
        break;
    case QQuickDockMoveGuide::WindowLeft:
        highlightRect = windowRect;
        highlightRect.setWidth(std::min(highlightRect.width() / 2, highlightImplicitWidth));
        break;
    case QQuickDockMoveGuide::WindowRight:
        highlightRect = windowRect;
        highlightRect.setWidth(std::min(highlightRect.width() / 2, highlightImplicitWidth));
        highlightRect.moveRight(panelRect.right());
        break;
    }

    emit q->highlightRectChanged();
}

void QQuickDockMoveGuidePrivate::setMousePos(const QPointF &pos)
{
    Q_Q(QQuickDockMoveGuide);
    mousePos = pos;

    if (setMousePosRunning.exchange(true))
        return;

    if (detached) {
        if (windowRect.contains(mousePos)) {
            window->setVisible(true);
            updateButtonsVisible();
            if (updateMaskNeeded) {
                updateMask();
                updateMaskNeeded = false;
            }
        } else {
            window->setVisible(false);
        }
    } else {
        updateButtonsVisible();
        q->setVisible(windowRect.contains(mousePos));
    }

    auto newSide = currentSideByMousePos();

    if (newSide != currentSide) {
        currentSide = newSide;
        setHighlightArea();
        Q_EMIT q->currentSideChanged();
        qApp->processEvents();
    }

    setMousePosRunning.store(false);
}

void QQuickDockMoveGuidePrivate::show()
{
    Q_Q(QQuickDockMoveGuide);
    if (detached) {

    } else {
        q->setVisible(true);
    }
}

void QQuickDockMoveGuidePrivate::hide()
{
    Q_Q(QQuickDockMoveGuide);
    if (detached) {
        window->hide();
    } else
        q->setVisible(false);
}

void QQuickDockMoveGuidePrivate::addToRegion(QQuickItem *item, QRegion &region)
{
    if (!item->isVisible())
        return;
    region = region.united(QRect{ static_cast<int>(item->x()), static_cast<int>(item->y()),
                                  static_cast<int>(item->width()),
                                  static_cast<int>(item->height()) });
}

void QQuickDockMoveGuidePrivate::updateMask()
{
    // QRegion region;
    // addToRegion(panelCenter, region);
    // addToRegion(panelTop, region);
    // addToRegion(panelRight, region);
    // addToRegion(panelBottom, region);
    // addToRegion(panelLeft, region);
    // addToRegion(windowTop, region);
    // addToRegion(windowRight, region);
    // addToRegion(windowBottom, region);
    // addToRegion(windowLeft, region);
    // window->setMask(region);
}

QQuickDockMoveGuide::DockSide QQuickDockMoveGuidePrivate::currentSideByMousePos()
{
    Q_Q(QQuickDockMoveGuide);
    QQuickItem *item{ nullptr };

    if (detached)
        item = window->contentItem()->childAt(mousePos.x(), mousePos.y());
    else
        item = q->childAt(mousePos.x(), mousePos.y());

    if (!item)
        return QQuickDockMoveGuide::None;

    if (item == panelCenter)
        return QQuickDockMoveGuide::Fill;
    if (item == panelTop)
        return QQuickDockMoveGuide::PanelTop;
    if (item == panelRight)
        return QQuickDockMoveGuide::PanelRight;
    if (item == panelBottom)
        return QQuickDockMoveGuide::PanelBottom;
    if (item == panelLeft)
        return QQuickDockMoveGuide::PanelLeft;
    if (item == windowTop)
        return QQuickDockMoveGuide::WindowTop;
    if (item == windowRight)
        return QQuickDockMoveGuide::WindowRight;
    if (item == windowBottom)
        return QQuickDockMoveGuide::WindowBottom;
    if (item == windowLeft)
        return QQuickDockMoveGuide::WindowLeft;

    return QQuickDockMoveGuide::None;
}

QT_END_NAMESPACE

#include "moc_qquickdockmoveguide_p.cpp"
