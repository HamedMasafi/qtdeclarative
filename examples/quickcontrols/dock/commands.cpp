// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "commands.h"

#include <QQmlEngine>
#include <QQuickItem>

MoveCommand::MoveCommand(QQuickItem *item, QPointF from, QPointF to, QUndoCommand *parent)
    : QUndoCommand(parent), _item(item), _from(std::move(from)), _to(std::move(to))
{
}

void MoveCommand::undo()
{
    _item->setPosition(_from);
}

void MoveCommand::redo()
{
    _item->setPosition(_to);
}

GeometryCommand::GeometryCommand(QQuickItem *item, const QRectF &from, const QRectF &to,
                                 QUndoCommand *parent)
    : QUndoCommand(parent), _item(item), _from(from), _to(to)
{
}

void GeometryCommand::undo()
{
    _item->setPosition(_from.topLeft());
    _item->setSize(_from.size());
}

void GeometryCommand::redo()
{
    _item->setPosition(_to.topLeft());
    _item->setSize(_to.size());
}

CreateCommand::CreateCommand(QQuickItem *parentItem, QPointF pos, const QString &uri,
                             QUndoCommand *parent)
    : QUndoCommand(parent), _parentItem{ parentItem }, _pos(std::move(pos)), _uri(uri)
{
}

void CreateCommand::undo()
{
    _item->setParentItem(nullptr);
    _item->deleteLater();
    _item = nullptr;
}

void CreateCommand::redo()
{
    if (!_parentItem) {
        qWarning() << "Parent QQuickItem is null!";
        return;
    }

    QQmlEngine *engine = qmlEngine(_parentItem);
    if (!engine) {
        qWarning() << "Could not retrieve QQmlEngine from parent!";
        return;
    }

    QQmlComponent component(engine, QUrl(_uri), _parentItem);
    if (component.isError()) {
        qWarning() << "Error loading component:" << component.errors();
        return;
    }

    QObject *obj = component.create();
    _item = qobject_cast<QQuickItem *>(obj);

    if (!_item) {
        qWarning() << "Created object is not a QQuickItem!";
        delete obj;
        return;
    }

    _item->setParentItem(_parentItem);
    _item->setPosition(_pos);
}
