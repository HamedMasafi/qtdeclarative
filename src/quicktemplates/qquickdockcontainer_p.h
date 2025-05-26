// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef DOCKCONTAINER_H
#define DOCKCONTAINER_H

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
#include <QtQuickTemplates2/private/qquickdockmoveguide_p.h>
#include <QtQuickTemplates2/private/qquickdockpanel_p.h>
#include <QtQuickTemplates2/private/qquickcontainer_p.h>

QT_BEGIN_NAMESPACE

class QQuickDockArea;
class QQuickDockAreaResizeHandler;
class QQuickDockContainerPrivate;
class QQuickDockContainerAttached;
class QQuickDockContainerAttachedPrivate;

// Q_DECLARE_OPAQUE_POINTER(QQuickDockMoveGuide)
Q_DECLARE_OPAQUE_POINTER(QQuickDockArea)
Q_DECLARE_OPAQUE_POINTER(QQuickDockAreaResizeHandler)
Q_DECLARE_OPAQUE_POINTER(QQuickDockContainerPrivate)

class Q_QUICKTEMPLATES2_EXPORT QQuickDockContainer : public QQuickContainer
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DockContainer)
    QML_ADDED_IN_VERSION(6, 13)
    Q_CLASSINFO("DefaultProperty", "contentData")
    QML_ATTACHED(QQuickDockContainerAttached)

    Q_PROPERTY(QList<QQuickDockPanel *> dockPanels READ dockPanels NOTIFY dockPanelsChanged)
    Q_PROPERTY(QQuickDockPanel *activeWidget READ activeWidget NOTIFY activeWidgetChanged FINAL)

    Q_PROPERTY(QQuickDockArea *topArea READ topArea WRITE setTopArea NOTIFY topAreaChanged FINAL)
    Q_PROPERTY(QQuickDockArea *bottomArea READ bottomArea WRITE setBottomArea NOTIFY
                       bottomAreaChanged FINAL)
    Q_PROPERTY(QQuickDockArea *rightArea READ rightArea WRITE setRightArea NOTIFY rightAreaChanged
                       FINAL)
    Q_PROPERTY(
            QQuickDockArea *leftArea READ leftArea WRITE setLeftArea NOTIFY leftAreaChanged FINAL)

    Q_PROPERTY(QQuickItem *dropArea READ dropArea WRITE setDropArea NOTIFY dropAreaChanged FINAL)
    Q_PROPERTY(QQuickDockMoveGuide *moveGuide READ moveGuide WRITE setMoveGuide NOTIFY
                       moveGuideChanged FINAL)
    Q_PROPERTY(QQmlComponent *splitView READ splitView WRITE setSplitView NOTIFY splitViewChanged
                       FINAL)
    Q_PROPERTY(QQmlComponent *widgetsGroup READ widgetsGroup WRITE setWidgetsGroup NOTIFY
                       widgetsGroupChanged FINAL)
    Q_PROPERTY(PopupType popupType READ popupType WRITE setPopupType NOTIFY popupTypeChanged FINAL)
    Q_PROPERTY(QQuickItem *dockIndicatorItem READ dockIndicatorItem WRITE setDockIndicatorItem
                       NOTIFY dockIndicatorItemChanged)

public:
    enum PopupType { Item, Window, Native };
    Q_ENUM(PopupType)

    explicit QQuickDockContainer(QQuickItem *parent = nullptr);

    [[nodiscard]] QList<QQuickDockPanel *> dockPanels() const;

    void removeDockPanel(QQuickDockPanel *widget);

    [[nodiscard]] QQuickDockArea *topArea() const;
    void setTopArea(QQuickDockArea *newTopArea);

    [[nodiscard]] QQuickDockArea *bottomArea() const;
    void setBottomArea(QQuickDockArea *newBottomArea);

    [[nodiscard]] QQuickDockArea *rightArea() const;
    void setRightArea(QQuickDockArea *newRightArea);

    [[nodiscard]] QQuickDockArea *leftArea() const;
    void setLeftArea(QQuickDockArea *newLeftArea);

    [[nodiscard]] QQuickItem *dropArea() const;
    void setDropArea(QQuickItem *newDropArea);

    [[nodiscard]] QQuickDockMoveGuide *moveGuide() const;
    void setMoveGuide(QQuickDockMoveGuide *newMoveGuide);

    [[nodiscard]] QQmlComponent *splitView() const;
    void setSplitView(QQmlComponent *newSplitView);

    [[nodiscard]] QQmlComponent *widgetsGroup() const;
    void setWidgetsGroup(QQmlComponent *newWidgetsGroup);

    [[nodiscard]] QQuickItem *dockIndicatorItem() const;
    void setDockIndicatorItem(QQuickItem *item);

    QQuickDockPanel *activeWidget() const;

    static QQuickDockContainerAttached *qmlAttachedProperties(QObject *object);

    PopupType popupType() const;
    void setPopupType(PopupType newPopupType);

protected:
    void itemAdded(int index, QQuickItem *item) override;
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void componentComplete() override;

Q_SIGNALS:
    void dockPanelsChanged(QList<QQuickDockPanel *> dockPanels);

    void topAreaChanged();
    void bottomAreaChanged();
    void rightAreaChanged();
    void leftAreaChanged();
    void dropAreaChanged();
    void moveGuideChanged();
    void splitViewChanged();
    void widgetsGroupChanged();
    void activeWidgetChanged();
    void dockIndicatorItemChanged();

    void popupTypeChanged();

private:
    Q_DECLARE_PRIVATE(QQuickDockContainer)

    Q_DISABLE_COPY(QQuickDockContainer)
    friend class QQuickDockMoveGuide;

    Q_PRIVATE_SLOT(d_func(), void dockPanelBeginMove())
    Q_PRIVATE_SLOT(d_func(), void dockPanelMoving(const QPointF &pt))
    Q_PRIVATE_SLOT(d_func(), void dockPanelMoved())
    Q_PRIVATE_SLOT(d_func(), void dockPanelVisibleChanged())
    Q_PRIVATE_SLOT(d_func(), void windowActiveFocusItemChanged())
    Q_PRIVATE_SLOT(d_func(), void areaCountChanged())
};

class Q_QUICKTEMPLATES2_EXPORT QQuickDockContainerAttached : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Qt::Edge edge READ edge WRITE setEdge NOTIFY edgeChanged FINAL)
    Q_PROPERTY(bool isFloating READ isFloating WRITE setIsFloating NOTIFY isFloatingChanged FINAL)

public:
    explicit QQuickDockContainerAttached(QObject *parent = nullptr);

    Qt::Edge edge() const;
    void setEdge(Qt::Edge newEdge);

    bool isFloating() const;
    void setIsFloating(bool newIsFloating);

Q_SIGNALS:
    void edgeChanged();
    void isFloatingChanged();

private:
    Q_DISABLE_COPY(QQuickDockContainerAttached)
    Q_DECLARE_PRIVATE(QQuickDockContainerAttached)
};

QT_END_NAMESPACE

#endif // DOCKCONTAINER_H
