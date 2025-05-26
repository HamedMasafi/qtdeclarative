// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKLAYOUT_P_H
#define QQUICKDOCKLAYOUT_P_H

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

#include <QQuickItem>
#include <QtQuickTemplates2/private/qtquicktemplates2global_p.h>
#include <QtQuickTemplates2/private/qquickcontainer_p.h>

QT_BEGIN_NAMESPACE

class QQuickDockPanel;
class QQuickDockLayoutPrivate;
class QQuickDockLayoutAttached;
class QQuickDockLayoutAttachedPrivate;
class QQuickDockLayoutHandleAttachedPrivate;
class Q_QUICKTEMPLATES2_EXPORT QQuickDockLayout : public QQuickContainer
{
private:
    Q_OBJECT
    QML_NAMED_ELEMENT(DockLayout)
    QML_ADDED_IN_VERSION(6, 13)
    QML_ATTACHED(QQuickDockLayoutAttached)

    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY
                       orientationChanged FINAL)
    Q_PROPERTY(QQmlComponent *handle READ handle WRITE setHandle NOTIFY handleChanged FINAL)

public:
    explicit QQuickDockLayout(QQuickItem *parent = nullptr);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation newOrientation);

    QQmlComponent *handle() const;
    void setHandle(QQmlComponent *newHandle);

    static QQuickDockLayoutAttached *qmlAttachedProperties(QObject *object);

signals:
    void orientationChanged();
    void handleChanged();

protected:
    void componentComplete() override;
    void itemAdded(int index, QQuickItem *item) override;
    void itemRemoved(int index, QQuickItem *item) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    bool childMouseEventFilter(QQuickItem *item, QEvent *event) override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    bool isContent(QQuickItem *item) const override;

#if QT_CONFIG(accessibility)
    QAccessible::Role accessibleRole() const override;
#endif

private:
    Q_DECLARE_PRIVATE(QQuickDockLayout)
    // Q_PRIVATE_SLOT(d_func(), void layout())
};

class Q_QUICKTEMPLATES2_EXPORT QQuickDockLayoutAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickDockLayout *view READ view NOTIFY viewChanged FINAL)
    Q_PROPERTY(
            qreal minimumSize READ minimumSize WRITE setMinimumSize NOTIFY minimumSizeChanged FINAL)
    Q_PROPERTY(
            qreal maximumSize READ maximumSize WRITE setMaximumSize NOTIFY maximumSizeChanged FINAL)
    Q_PROPERTY(qreal preferredSize READ preferredSize WRITE setPreferredSize NOTIFY
                       preferredSizeChanged FINAL)

public:
    QQuickDockLayoutAttached(QObject *parent = nullptr);

    QQuickDockLayout *view() const;

    qreal minimumSize() const;
    void setMinimumSize(qreal newMinimumSize);

    qreal maximumSize() const;
    void setMaximumSize(qreal newMaximumSize);

    qreal preferredSize() const;
    void setPreferredSize(qreal preferredSize);

Q_SIGNALS:
    void viewChanged();
    void minimumSizeChanged();
    void maximumSizeChanged();

    void preferredSizeChanged();

private:
    Q_DECLARE_PRIVATE(QQuickDockLayoutAttached)
};

class Q_QUICKTEMPLATES2_EXPORT QQuickDockLayoutHandleAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hovered READ isHovered NOTIFY hoveredChanged FINAL)
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged FINAL)
    QML_NAMED_ELEMENT(DockLayoutHandle)
    QML_ATTACHED(QQuickDockLayoutHandleAttached)
    QML_UNCREATABLE("")
    QML_ADDED_IN_VERSION(2, 13)

public:
    explicit QQuickDockLayoutHandleAttached(QObject *parent = nullptr);

    bool isHovered() const;
    bool isPressed() const;

    static QQuickDockLayoutHandleAttached *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void hoveredChanged();
    void pressedChanged();

private:
    Q_DISABLE_COPY(QQuickDockLayoutHandleAttached)
    Q_DECLARE_PRIVATE(QQuickDockLayoutHandleAttached)
};

QT_END_NAMESPACE

#endif // QQUICKDOCKLAYOUT_P_H
