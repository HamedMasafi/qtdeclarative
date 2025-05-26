// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKLAYOUT_P_P_H
#define QQUICKDOCKLAYOUT_P_P_H

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

#include <QtQuick/private/qquickitem_p.h>
#include <QtQuickTemplates2/private/qquickdocklayout_p.h>
#include <QtQuickTemplates2/private/qquickcontainer_p.h>
#include <QtQuickTemplates2/private/qquickcontainer_p_p.h>

QT_BEGIN_NAMESPACE

class QQuickDockPanel;
class QQuickDockGroup;
class QQuickDockContainer;
class QQuickDockAreaResizeHandler;
class Q_QUICKTEMPLATES2_EXPORT QQuickDockLayoutPrivate : public QQuickContainerPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockLayout)

public:
    static QQuickDockLayoutPrivate *get(QQuickDockLayout *item) { return item->d_func(); }

    struct ItemSizeData
    {
        qreal currentSize = 0;
        qreal percentageSize = 0;
    };

    QQmlComponent *handleComponent;
    QList<QQuickItem *> visibleItems;
    QList<QQuickItem *> handleItems;
    int stretchIndex{ -1 };
    Qt::Orientation orientation{ Qt::Horizontal };
    QHash<QQuickItem *, ItemSizeData> sizeHash;

    ItemSizeData *sizeDataItem1;
    ItemSizeData *sizeDataItem2;

    qreal resizeHandleItem1Size{};
    qreal resizeHandleItem2Size{};
    QQuickItem *resizeHandleItem1;
    QQuickItem *resizeHandleItem2;
    QRectF resizeArea;
    QPointF handleMouseClickPos;
    int handlePressPos;
    int pressedHandleIndex{ -1 };
    int hoveredHandleIndex{ -1 };

    qreal handleItemsSize{};
    QQuickDockContainer *container;

    void itemVisibilityChanged(QQuickItem *item) override;
    void setItemGeometry(QQuickItem *item, qreal pos, qreal size = 0) const;
    qreal itemSize(const QQuickItem *item) const;
    void ensureThereIsEnoughHandles();
    void layout();
    bool isMainItem(QQuickItem *item);

    QQuickItem *createHandle();
    void updateSizeDataFromState();

    void itemAdded(int index, QQuickItem *item);
    void itemRemoved(int index, QQuickItem *item);

    struct DropAreaData
    {
        enum class AreaType { Group, MainItem };
        QQuickItem *item{};
        QRectF rect;
        QQuickDockLayout *layout{};
        AreaType areaType{ AreaType::MainItem };
        QQuickDockGroup *group;

        DropAreaData(QQuickItem *item, const QRectF &rect, QQuickDockLayout *layout,
                     AreaType areaType = AreaType::MainItem);

        DropAreaData(const QRectF &rect, QQuickDockGroup *group);
    };
    void iterateForDropArea(QQuickDockContainer *container, QList<DropAreaData> &list);

    bool containsMainItem(); // TODO: remove this method

    void updatePolish() override;
    QQuickItem *getContentItem() override;

    bool handlePress(const QPointF &point, ulong timestamp) override;
    bool handleMove(const QPointF &point, ulong timestamp) override;
    bool handleRelease(const QPointF &point, ulong timestamp) override;
    void updateHoveredHandle(QQuickItem *hoveredItem);

    // Add new item methods
    enum InsertChildType { AfterCurrent, BeforeCurrent };
    QQuickDockGroup *createLayout(int index, InsertChildType insertType);
    QQuickDockGroup *splitGroup(int index, InsertChildType insertType);
    void addDockPanel(QQuickItem *item, QQuickDockPanel *dockPanel, Qt::Edge edge);
};

class QQuickDockLayoutAttachedPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockLayoutAttached)

public:
    static constexpr int defaultSize{ 200 };

    QQuickDockLayoutAttachedPrivate();

    static QQuickDockLayoutAttachedPrivate *get(QQuickDockLayoutAttached *attached)
    {
        return attached->d_func();
    }
    static const QQuickDockLayoutAttachedPrivate *get(const QQuickDockLayoutAttached *attached)
    {
        return attached->d_func();
    }

    QQuickDockLayout *view = nullptr;
    qreal minimumSize;
    qreal maximumSize{ std::numeric_limits<qreal>::max() };
    qreal preferredSize{ defaultSize };
};

class QQuickDockLayoutHandleAttachedPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockLayoutHandleAttached)

public:
    QQuickDockLayoutHandleAttachedPrivate();

    void setHovered(bool hovered);
    void setPressed(bool pressed);

    static QQuickDockLayoutHandleAttachedPrivate *get(QQuickDockLayoutHandleAttached *attached);
    static const QQuickDockLayoutHandleAttachedPrivate *
    get(const QQuickDockLayoutHandleAttached *attached);

    unsigned m_hovered : 1;
    unsigned m_pressed : 1;
};

QT_END_NAMESPACE

#endif // QQUICKDOCKLAYOUT_P_P_H
