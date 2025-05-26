// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <QObject>
#include <QQuickItem>
#include <QMap>
#include <QString>
#include <QStringList>
#include <functional>

class Factory : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QQuickItem *parentItem READ parentItem WRITE setParentItem NOTIFY parentItemChanged)
    Q_PROPERTY(QStringList registeredTypes READ registeredTypes NOTIFY registeredTypesChanged)

public:
    explicit Factory(QObject *parent = nullptr);

    template <typename T>
    static void registerType()
    {
        static_assert(std::is_base_of<QQuickItem, T>::value, "Type must inherit QQuickItem");
        const QString typeName = QString::fromUtf8(T::staticMetaObject.className());
        s_creators()[typeName] = []() -> QQuickItem * { return new T(); };
        s_typeNames().append(typeName);
    }

    static QStringList registeredTypeNames() { return s_typeNames(); }

    Q_INVOKABLE QQuickItem *create(const QString &typeName, const QPointF &pos = QPointF());
    Q_INVOKABLE QQuickItem *create(QQuickItem *parent, const QString &typeName,
                                   const QPointF &pos = QPointF());
    Q_INVOKABLE QQuickItem *create(QQuickItem *parent, const QString &typeName,
                                   const QVariantMap &props);

    static QQuickItem *createItem(QQuickItem *parent, const QString &typeName,
                                  const QVariantMap &props);

    QQuickItem *parentItem() const;
    void setParentItem(QQuickItem *item);

    QStringList registeredTypes() const;

signals:
    void parentItemChanged();
    void registeredTypesChanged();

private:
    QQuickItem *m_parentItem = nullptr;

    static QMap<QString, std::function<QQuickItem *()>> &s_creators();
    static QStringList &s_typeNames();
};
