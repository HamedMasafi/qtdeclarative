// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKMOVEGUIDE_P_P_H
#define QQUICKDOCKMOVEGUIDE_P_P_H

#include <private/qquickitem_p.h>
#include <private/qquickdockmoveguide_p.h>

class QQuickDockContainer;

class QQuickDockMoveGuidePrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockMoveGuide)

public:
    enum DropIndex { First, End };

    QQuickItem *panelCenter{ nullptr };
    QQuickItem *panelTop{ nullptr };
    QQuickItem *panelRight{ nullptr };
    QQuickItem *panelBottom{ nullptr };
    QQuickItem *panelLeft{ nullptr };
    QQuickItem *windowTop{ nullptr };
    QQuickItem *windowRight{ nullptr };
    QQuickItem *windowBottom{ nullptr };
    QQuickItem *windowLeft{ nullptr };

    QPointF mousePos;
    QRectF windowRect;
    QRectF panelRect;
    QQuickWindow *window{ nullptr };
    bool detached{ false };
    QQuickDockContainer *parentDockContainer{nullptr};

    QRectF highlightRect;
    qreal highlightImplicitHeight{100};
    qreal highlightImplicitWidth{100};

    std::atomic<bool> setMousePosRunning{ false };
    QQuickDockMoveGuide::DockSide currentSide{ QQuickDockMoveGuide::None };

    int buttonSize{ 20 };
    bool updateMaskNeeded;

    bool prevMouseInWindow{ false };
    bool prevMouseInPanel{ false };
    bool allowFill{true};

    static QQuickDockMoveGuidePrivate *get(QQuickDockMoveGuide *item) { return item->d_func(); }

    void detach();

    void attach();
    void updateButtonsVisible();

    void movetoCenter(QQuickItem *btn, qreal x, qreal y);

    void setButtonsPos2(const QRectF &rc, QQuickItem *leftBtn, QQuickItem *topBtn,
                        QQuickItem *rightBtn, QQuickItem *bottomBtn, bool panel);

    void begin();
    void setPanelRect(const QRectF &rc);

    void setHighlightArea();
    void setMousePos(const QPointF &pos);

    void show();
    void hide();

    void addToRegion(QQuickItem *item, QRegion &region);

    void updateMask();

    QQuickDockMoveGuide::DockSide currentSideByMousePos();
};

#endif // QQUICKDOCKMOVEGUIDE_P_P_H
