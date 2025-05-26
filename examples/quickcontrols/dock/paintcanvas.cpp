// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "paintcanvas.h"

#include "factory.h"

#include <QCursor>

PaintCanvas::PaintCanvas(QQuickItem *parent) : QQuickItem{ parent }
{
    setAcceptHoverEvents(true);

    setAcceptedMouseButtons(Qt::LeftButton);
    setFiltersChildMouseEvents(true);
}

QQuickItem *PaintCanvas::rubberBand() const
{
    return m_rubberBand;
}

void PaintCanvas::setRubberBand(QQuickItem *newRubberBand)
{
    if (m_rubberBand == newRubberBand)
        return;
    m_rubberBand = newRubberBand;
    m_rubberBand->setParentItem(this);
    emit rubberBandChanged();
}

QQuickItem *PaintCanvas::activeItem() const
{
    return m_activeItem;
}

void PaintCanvas::setActiveItem(QQuickItem *newActiveItem)
{
    if (m_activeItem == newActiveItem)
        return;
    m_activeItem = newActiveItem;
    emit activeItemChanged();
}

void PaintCanvas::mousePressEvent(QMouseEvent *event)
{
    _clickStartPos = event->position();
    event->accept();
    // QQuickItem::mousePressEvent(event);
}

void PaintCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if (!_typeName.isEmpty()) {
        m_rubberBand->setPosition(QPointF{ qMin(event->position().x(), _clickStartPos.x()),
                                           qMin(event->position().y(), _clickStartPos.y()) });
        m_rubberBand->setSize(QSizeF{ qAbs(event->position().x() - _clickStartPos.x()),
                                      qAbs(event->position().y() - _clickStartPos.y()) });
        m_rubberBand->setVisible(true);
    }
}

void PaintCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (!_typeName.isEmpty()) {
        auto item = Factory::createItem(this, _typeName, _props);
        item->setAcceptedMouseButtons(Qt::LeftButton);
        item->setPosition(m_rubberBand->position());
        item->setSize(m_rubberBand->size());
    }

    m_rubberBand->setVisible(false);
}

void PaintCanvas::setActiveItemRect(Edges edges, const QPointF &pt)
{
    QRectF rc{ m_activeItem->position(), m_activeItem->size() };

    if (edges & Edge::Left)
        rc.setLeft(pt.x());

    if (edges & Edge::Top)
        rc.setTop(pt.y());

    if (edges & Edge::Right)
        rc.setRight(pt.x());

    if (edges & Edge::Bottom)
        rc.setBottom(pt.y());

    m_activeItem->setPosition(rc.topLeft());
    m_activeItem->setSize(rc.size());
}

QQmlComponent *PaintCanvas::handleComponent() const
{
    return m_handleComponent;
}

void PaintCanvas::setHandleComponent(QQmlComponent *component)
{
    if (m_handleComponent == component)
        return;

    m_handleComponent = component;
    emit handleComponentChanged();

    // Clean up old handles
    qDeleteAll(m_handles);
    m_handles.clear();

    // Create 8 handles
    if (m_handleComponent && m_handleComponent->isReady()) {
        for (auto &d : handleData) {
            QObject *obj = m_handleComponent->create(qmlContext(this));
            QQuickItem *handle = qobject_cast<QQuickItem *>(obj);
            if (handle) {
                handle->setParentItem(this);
                handle->setVisible(false);
                handle->setZ(30);
                handle->setAcceptedMouseButtons(Qt::LeftButton);
                d.handle = handle;
                handle->setCursor(d.cursor);
                m_handles.append(handle);
            } else {
                delete obj;
            }
        }
    }
}

void PaintCanvas::setPointer()
{
    _typeName.clear();
    _props.clear();
}

void PaintCanvas::setTool(const QString &typeName, const QVariantMap &props)
{
    qDebug() << Q_FUNC_INFO << typeName << props;
    _typeName = typeName;
    _props = props;
}

void PaintCanvas::updateHandlesGeometry()
{
    if (!m_activeItem)
        return;

    QRectF bounds = m_activeItem->boundingRect().translated(m_activeItem->x(), m_activeItem->y());
    QPointF corners[8] = { bounds.topLeft(),     QPointF(bounds.center().x(), bounds.top()),
                           bounds.topRight(),    QPointF(bounds.right(), bounds.center().y()),
                           bounds.bottomRight(), QPointF(bounds.center().x(), bounds.bottom()),
                           bounds.bottomLeft(),  QPointF(bounds.left(), bounds.center().y()) };

    for (int i = 0; i < m_handles.size(); ++i) {
        QQuickItem *handle = m_handles[i];
        if (handle) {
            handle->setVisible(true);
            handle->setPosition(corners[i] - QPointF(handle->width() / 2, handle->height() / 2));
        }
    }
}

bool PaintCanvas::childMouseEventFilter(QQuickItem *item, QEvent *e)
{
    static HandleData *currentHandleData{};

    auto me = static_cast<QMouseEvent *>(e);
    qDebug() << e->type();
    switch (e->type()) {
    case QEvent::MouseButtonPress: {
        _typeName.clear();
        _clickStartPos = me->scenePosition();
        _itemInitialPos = item->position();
        me->accept();

        auto it = std::find_if(handleData.begin(), handleData.end(),
                               [item](const HandleData &d) { return d.handle == item; });
        currentHandleData = it != handleData.end() ? &(*it) : nullptr;

        if (!currentHandleData) {
            setActiveItem(item);
            updateHandlesGeometry();
        }
        return true;
    }
    case QEvent::MouseMove:
        qDebug() << me->position();

        if (currentHandleData) {
            setActiveItemRect(currentHandleData->edge, item->mapToItem(this, me->position()));
        } else {
            m_activeItem->setPosition(_itemInitialPos + (me->scenePosition() - _clickStartPos));
        }
        updateHandlesGeometry();

        break;

    default:
        break;
    }
    return QQuickItem::childMouseEventFilter(item, e);
}
