// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qquickborderline_p.h"
#include "qquickborderline_p_p.h"

#include <QPainter>

QT_BEGIN_NAMESPACE

QQuickBorderLine::QQuickBorderLine(QQuickItem *parent)
    : QQuickPaintedItem{ *(new QQuickBorderLinePrivate), parent }
{
}

void QQuickBorderLine::paint(QPainter *painter)
{
    Q_D(QQuickBorderLine);

    if (!d->color.isValid())
        return;

    QPen pen(d->color, 1);
    painter->setPen(pen);

    const QRectF r = boundingRect();

    const qreal left = r.left() + d->leftPadding;
    const qreal right = r.right() - d->rightPadding - 1;
    const qreal top = r.top() + d->topPadding;
    const qreal bottom = r.bottom() - d->bottomPadding - 1;

    if (d->backgroundColor != Qt::transparent)
        painter->fillRect(left, top, right - left, bottom - top, d->backgroundColor);

    if (d->topLine)
        painter->drawLine(QPointF(left, top), QPointF(right, top));
    if (d->bottomLine)
        painter->drawLine(QPointF(left, bottom), QPointF(right, bottom));
    if (d->leftLine)
        painter->drawLine(QPointF(left, top), QPointF(left, bottom));
    if (d->rightLine)
        painter->drawLine(QPointF(right, top), QPointF(right, bottom));
}

QColor QQuickBorderLine::color() const
{
    Q_D(const QQuickBorderLine);
    return d->color;
}

void QQuickBorderLine::setColor(const QColor &newColor)
{
    Q_D(QQuickBorderLine);
    d->color = newColor;
    update();
}

QColor QQuickBorderLine::backgroundColor() const
{
    Q_D(const QQuickBorderLine);
    return d->backgroundColor;
}

void QQuickBorderLine::setBackgroundColor(const QColor &newColor)
{
    Q_D(QQuickBorderLine);
    d->backgroundColor = newColor;
    update();
}

bool QQuickBorderLine::topLine() const
{
    Q_D(const QQuickBorderLine);
    return d->topLine;
}

void QQuickBorderLine::setTopLine(bool newTopLine)
{
    Q_D(QQuickBorderLine);
    d->topLine = newTopLine;
    update();
}

bool QQuickBorderLine::leftLine() const
{
    Q_D(const QQuickBorderLine);
    return d->leftLine;
}

void QQuickBorderLine::setLeftLine(bool newLeftLine)
{
    Q_D(QQuickBorderLine);
    d->leftLine = newLeftLine;
    update();
}

bool QQuickBorderLine::rightLine() const
{
    Q_D(const QQuickBorderLine);
    return d->rightLine;
}

void QQuickBorderLine::setRightLine(bool newRightLine)
{
    Q_D(QQuickBorderLine);
    d->rightLine = newRightLine;
    update();
}

bool QQuickBorderLine::bottomLine() const
{
    Q_D(const QQuickBorderLine);
    return d->bottomLine;
}

void QQuickBorderLine::setBottomLine(bool newBottomLine)
{
    Q_D(QQuickBorderLine);
    d->bottomLine = newBottomLine;
    update();
}

qreal QQuickBorderLine::topPadding() const
{
    Q_D(const QQuickBorderLine);
    return d->topPadding;
}

void QQuickBorderLine::setTopPadding(qreal newTopPadding)
{
    Q_D(QQuickBorderLine);
    d->topPadding = newTopPadding;
    update();
}

void QQuickBorderLine::resetTopPadding()
{
    setTopPadding(0);
}

qreal QQuickBorderLine::leftPadding() const
{
    Q_D(const QQuickBorderLine);
    return d->leftPadding;
}

void QQuickBorderLine::setLeftPadding(qreal newLeftPadding)
{
    Q_D(QQuickBorderLine);
    d->leftPadding = newLeftPadding;
    update();
}

void QQuickBorderLine::resetLeftPadding()
{
    setLeftPadding(0);
}

qreal QQuickBorderLine::rightPadding() const
{
    Q_D(const QQuickBorderLine);
    return d->rightPadding;
}

void QQuickBorderLine::setRightPadding(qreal newRightPadding)
{
    Q_D(QQuickBorderLine);
    d->rightPadding = newRightPadding;
    update();
}

void QQuickBorderLine::resetRightPadding()
{
    setRightPadding(0);
}

qreal QQuickBorderLine::bottomPadding() const
{
    Q_D(const QQuickBorderLine);
    return d->bottomPadding;
}

void QQuickBorderLine::setBottomPadding(qreal newBottomPadding)
{
    Q_D(QQuickBorderLine);
    d->bottomPadding = newBottomPadding;
    update();
}

void QQuickBorderLine::resetBottomPadding()
{
    setBottomPadding(0);
}

QT_END_NAMESPACE

#include "moc_qquickborderline_p.cpp"
