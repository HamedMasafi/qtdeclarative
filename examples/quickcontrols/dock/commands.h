// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <QPointF>
#include <QRectF>
#include <QUndoCommand>

class QQuickItem;
class MoveCommand : public QUndoCommand
{
    QQuickItem *_item;
    QPointF _from;
    QPointF _to;

public:
    MoveCommand(QQuickItem *item, QPointF from, QPointF to, QUndoCommand *parent = nullptr);

    void undo();
    void redo();
};

class CreateCommand : public QUndoCommand
{
    QQuickItem *_item;
    QQuickItem *_parentItem;
    QPointF _pos;
    QString _uri;

public:
    CreateCommand(QQuickItem *parentItem, QPointF pos, const QString &uri,
                  QUndoCommand *parent = nullptr);

    void undo();
    void redo();
};

class GeometryCommand : public QUndoCommand
{
    QQuickItem *_item;
    QRectF _from;
    QRectF _to;

public:
    GeometryCommand(QQuickItem *item, const QRectF &from, const QRectF &to,
                    QUndoCommand *parent = nullptr);

    void undo();
    void redo();
};
