// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKMOVEGUIDE_H
#define QQUICKDOCKMOVEGUIDE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtQuickTemplates2/private/qtquicktemplates2global_p.h>
#include <QtQuick/qquickitem.h>

QT_BEGIN_NAMESPACE

class QQuickWindow;
class QQuickDockMoveDropGuide;
class QQuickDockMoveGuidePrivate;

class Q_QUICKTEMPLATES2_EXPORT QQuickDockMoveGuide : public QQuickItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DockMoveGuide)
    QML_ADDED_IN_VERSION(6, 13)

    Q_PROPERTY(QRectF highlightRect READ highlightRect NOTIFY highlightRectChanged FINAL)
    Q_PROPERTY(bool detached READ detached WRITE setDetached NOTIFY detachedChanged FINAL)

    Q_PROPERTY(QQuickItem *panelCenter READ panelCenter WRITE setPanelCenter NOTIFY
                       panelCenterChanged FINAL)

    Q_PROPERTY(QQuickItem *panelTop READ panelTop WRITE setPanelTop NOTIFY panelTopChanged FINAL)
    Q_PROPERTY(QQuickItem *panelRight READ panelRight WRITE setPanelRight NOTIFY panelRightChanged
                       FINAL)
    Q_PROPERTY(QQuickItem *panelBottom READ panelBottom WRITE setPanelBottom NOTIFY
                       panelBottomChanged FINAL)
    Q_PROPERTY(
            QQuickItem *panelLeft READ panelLeft WRITE setPanelLeft NOTIFY panelLeftChanged FINAL)

    Q_PROPERTY(
            QQuickItem *windowTop READ windowTop WRITE setWindowTop NOTIFY windowTopChanged FINAL)
    Q_PROPERTY(QQuickItem *windowRight READ windowRight WRITE setWindowRight NOTIFY
                       windowRightChanged FINAL)
    Q_PROPERTY(QQuickItem *windowBottom READ windowBottom WRITE setWindowBottom NOTIFY
                       windowBottomChanged FINAL)
    Q_PROPERTY(QQuickItem *windowLeft READ windowLeft WRITE setWindowLeft NOTIFY windowLeftChanged
                       FINAL)
    Q_PROPERTY(DockSide currentSide READ currentSide NOTIFY currentSideChanged FINAL)
public:
    enum DockSide {
        None,
        Fill,
        PanelTop,
        PanelBottom,
        PanelLeft,
        PanelRight,
        WindowTop,
        WindowBottom,
        WindowLeft,
        WindowRight,
    };
    Q_ENUM(DockSide)

    using LocalAndGlobalRect = std::pair<QRectF, QRectF>;
    QQuickDockMoveGuide(QQuickItem *parent = nullptr);

    void componentComplete() override;

    QRectF areaRect() const;
    void setAreaRect(const QRectF &area);

    void begin();
    void end();

    QPointF mousePos() const;
    void setMousePos(const QPointF &mousePos);

    DockSide currentSide() const;

    [[nodiscard]] QRectF highlightRect() const;

    bool detached() const;
    void setDetached(bool newDetached);

    QQuickItem *panelCenter() const;
    void setPanelCenter(QQuickItem *newPanelCenter);

    QQuickItem *panelTop() const;
    void setPanelTop(QQuickItem *newPanelTop);

    QQuickItem *panelRight() const;
    void setPanelRight(QQuickItem *newPanelRight);

    QQuickItem *panelBottom() const;
    void setPanelBottom(QQuickItem *newPanelBottom);

    QQuickItem *panelLeft() const;
    void setPanelLeft(QQuickItem *newPanelLeft);

    QQuickItem *windowTop() const;
    void setWindowTop(QQuickItem *newWindowTop);

    QQuickItem *windowRight() const;
    void setWindowRight(QQuickItem *newWindowRight);

    QQuickItem *windowBottom() const;
    void setWindowBottom(QQuickItem *newWindowBottom);

    QQuickItem *windowLeft() const;
    void setWindowLeft(QQuickItem *newWindowLeft);

    bool allowFill() const;
    void setAllowFill(bool allowFill);

Q_SIGNALS:
    void dropped(Qt::DockWidgetArea area);

    void buttonsChanged();
    void highlightRectChanged();
    void detachedChanged();
    void panelCenterChanged();
    void panelTopChanged();
    void panelRightChanged();
    void panelBottomChanged();
    void panelLeftChanged();
    void windowTopChanged();
    void windowRightChanged();
    void windowBottomChanged();
    void windowLeftChanged();
    void currentSideChanged();

private:
    Q_DECLARE_PRIVATE(QQuickDockMoveGuide)

    Q_DISABLE_COPY(QQuickDockMoveGuide)
};

QT_END_NAMESPACE

#endif // QQUICKDOCKMOVEGUIDE_H
