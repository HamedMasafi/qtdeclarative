// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QQuickPaintedItem>

#ifndef QQUICKBORDERLINE_P_H
#  define QQUICKBORDERLINE_P_H

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

QT_BEGIN_NAMESPACE

class QQuickBorderLinePrivate;

class QQuickBorderLine : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor FINAL)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor FINAL)
    Q_PROPERTY(bool topLine READ topLine WRITE setTopLine FINAL)
    Q_PROPERTY(bool leftLine READ leftLine WRITE setLeftLine FINAL)
    Q_PROPERTY(bool rightLine READ rightLine WRITE setRightLine FINAL)
    Q_PROPERTY(bool bottomLine READ bottomLine WRITE setBottomLine FINAL)
    Q_PROPERTY(qreal topPadding READ topPadding WRITE setTopPadding RESET resetTopPadding FINAL)
    Q_PROPERTY(qreal leftPadding READ leftPadding WRITE setLeftPadding RESET resetLeftPadding FINAL)
    Q_PROPERTY(qreal rightPadding READ rightPadding WRITE setRightPadding RESET resetRightPadding
                       FINAL)
    Q_PROPERTY(qreal bottomPadding READ bottomPadding WRITE setBottomPadding RESET
                       resetBottomPadding FINAL)
    QML_ADDED_IN_VERSION(6, 11)
    QML_NAMED_ELEMENT(BorderLine)

public:
    explicit QQuickBorderLine(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    QColor color() const;
    void setColor(const QColor &newColor);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newColor);

    bool topLine() const;
    void setTopLine(bool newTopLine);

    bool leftLine() const;
    void setLeftLine(bool newLeftLine);

    bool rightLine() const;
    void setRightLine(bool newRightLine);

    bool bottomLine() const;
    void setBottomLine(bool newBottomLine);

    qreal topPadding() const;
    void setTopPadding(qreal newTopPadding);
    void resetTopPadding();

    qreal leftPadding() const;
    void setLeftPadding(qreal newLeftPadding);
    void resetLeftPadding();

    qreal rightPadding() const;
    void setRightPadding(qreal newRightPadding);
    void resetRightPadding();

    qreal bottomPadding() const;
    void setBottomPadding(qreal newBottomPadding);
    void resetBottomPadding();

private:
    Q_DISABLE_COPY(QQuickBorderLine)
    Q_DECLARE_PRIVATE(QQuickBorderLine)
};

QT_END_NAMESPACE

#endif // !QQUICKBORDERLINE_P_H
