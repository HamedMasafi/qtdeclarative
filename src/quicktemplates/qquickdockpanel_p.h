// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKPANEL_P_H
#define QQUICKDOCKPANEL_P_H

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
#include <QtQuickTemplates2/private/qquickicon_p.h>

QT_BEGIN_NAMESPACE

class QQuickDockWindow;
class QQuickDockContainer;
class QQuickDockPanelPrivate;
class QQuickDockGroup;

class Q_QUICKTEMPLATES2_EXPORT QQuickDockPanel : public QQuickControl
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DockPanel)
    QML_ADDED_IN_VERSION(6, 13)
    Q_CLASSINFO("DefaultProperty", "contentData")

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QQuickItem *titleBar READ titleBar WRITE setTitleBar NOTIFY titleBarChanged)
    Q_PROPERTY(QQuickIcon icon READ icon WRITE setIcon NOTIFY iconChanged FINAL)
    Q_PROPERTY(bool isOpen READ isOpened WRITE setIsOpened NOTIFY isOpenChanged FINAL)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged FINAL)
    Q_PROPERTY(int resizeEdgeSize READ resizeEdgeSize WRITE setResizeEdgeSize NOTIFY
                       resizeEdgeSizeChanged FINAL)
    Q_PROPERTY(Visibility visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)
    Q_PROPERTY(Qt::Edge edge READ edge WRITE setEdge NOTIFY edgeChanged FINAL)
    Q_PROPERTY(bool pinned READ pinned NOTIFY visibilityChanged)
    Q_PROPERTY(bool opened READ isOpened WRITE setIsOpened NOTIFY isOpenChanged)

    // Q_CLASSINFO("DeferredPropertyNames", "background,contentItem,titleBar")

public:
    enum Visibility { Closed, Unpinned, Pinned, Detached };
    Q_ENUM(Visibility)

    explicit QQuickDockPanel(QQuickItem *parent = nullptr);
    virtual ~QQuickDockPanel();

    QString title() const;
    void setTitle(QString title);

    QQuickItem *titleBar() const;
    void setTitleBar(QQuickItem *titleBar);

    QQuickIcon icon() const;
    void setIcon(const QQuickIcon &newIcon);

    int resizeEdgeSize() const;
    void setResizeEdgeSize(int newResizeEdgeSize);

    QQuickDockWindow *dockWindow() const;

    void componentComplete() override;

    Visibility visibility() const;
    void setVisibility(const Visibility &newVisibility);

    Qt::Edge edge() const;
    void setEdge(Qt::Edge newEdge);

    bool isActive() const;

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void togglePin();
    Q_INVOKABLE void pinTo(QQuickDockGroup *g);

    void restoreSize();

    bool isOpened() const;
    void setIsOpened(bool newIsOpen);

    bool floating() const;
    bool pinned() const;

protected:
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

Q_SIGNALS:
    void titleChanged(QString title);
    void titleBarChanged(QQuickItem *titleBar);
    void resizeEdgeSizeChanged();
    void visibilityChanged();
    void isActiveChanged(bool isActive);
    void iconChanged();

    void beginMove();
    void moving(const QPointF &pt);
    void moved();

    void isOpenChanged();
    void closed();
    void opened();
    void detached();
    void isOpenedChanged();

    void edgeChanged();

private:
    void setIsActive(bool isActive);

    Q_DISABLE_COPY(QQuickDockPanel)
    Q_DECLARE_PRIVATE(QQuickDockPanel)
};

QT_END_NAMESPACE

#endif // QQUICKDOCKPANEL_P_H
