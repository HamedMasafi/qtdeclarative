// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKTABBAR_P_H
#define QQUICKDOCKTABBAR_P_H

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

#include <QtQuickTemplates2/private/qquickcontrol_p.h>

QT_BEGIN_NAMESPACE

class QQuickDockPanel;
class QQuickDockTabBarPrivate;

class Q_QUICKTEMPLATES2_EXPORT QQuickDockTabBar : public QQuickControl
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QQuickDockTabBar)
    QML_NAMED_ELEMENT(DockTabBar)
    QML_ADDED_IN_VERSION(6, 13)

    Q_PROPERTY(QVariant dockPanels READ dockPanels NOTIFY dockPanelsChanged FINAL)
    Q_PROPERTY(
            qreal implicitSize READ implicitSize WRITE setInplicitSize NOTIFY implicitSizeChanged)
    Q_PROPERTY(Qt::Edge position READ position WRITE setPosition NOTIFY positionChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged
                       FINAL)

public:
    QQuickDockTabBar(QQuickItem *parent = nullptr);

    bool containsPanel(QQuickDockPanel *panel) const;
    void addDockPanel(QQuickDockPanel *widget);
    void removeDockPanel(QQuickDockPanel *widget);
    QList<QQuickDockPanel *> panels() const;

    QVariant dockPanels() const;
    Qt::Edge position() const;
    void setPosition(Qt::Edge newPosition);

    int count() const;

    QQuickDockPanel *currentDockPanel() const;
    Q_INVOKABLE void setCurrentDockPanel(QQuickDockPanel *widget);

    int currentIndex() const;
    void setCurrentIndex(int newCurrentIndex);

    qreal implicitSize() const;
    void setInplicitSize(qreal newValue);

Q_SIGNALS:
    void dockPanelsChanged();
    void positionChanged();
    void countChanged();
    void currentIndexChanged();
    void implicitSizeChanged();
};

QT_END_NAMESPACE

#endif // QQUICKDOCKTABBAR_P_H
