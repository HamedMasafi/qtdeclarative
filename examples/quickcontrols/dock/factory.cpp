// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "factory.h"

QMap<QString, std::function<QQuickItem *()>> &Factory::s_creators()
{
    static QMap<QString, std::function<QQuickItem *()>> creators;
    return creators;
}

QStringList &Factory::s_typeNames()
{
    static QStringList types;
    return types;
}

Factory::Factory(QObject *parent) : QObject(parent) { }

QQuickItem *Factory::create(const QString &typeName, const QPointF &pos)
{
    return create(m_parentItem, typeName, pos);
}

QQuickItem *Factory::create(QQuickItem *parent, const QString &typeName, const QPointF &pos)
{
    auto &creators = s_creators();
    if (!creators.contains(typeName)) {
        qWarning() << "Unknown type:" << typeName;
        return nullptr;
    }
    QQuickItem *item = creators[typeName]();
    item->setParentItem(parent);
    item->setPosition(pos);
    return item;
}

QQuickItem *Factory::create(QQuickItem *parent, const QString &typeName, const QVariantMap &props)
{
    auto &creators = s_creators();
    if (!creators.contains(typeName)) {
        qWarning() << "Unknown type:" << typeName;
        return nullptr;
    }
    QQuickItem *item = creators[typeName]();
    item->setParentItem(parent);
    for (auto it = props.constBegin(); it != props.constEnd(); ++it)
        item->setProperty(it.key().toUtf8().constData(), it.value());

    return item;
}

QQuickItem *Factory::createItem(QQuickItem *parent, const QString &typeName,
                                const QVariantMap &props)
{
    auto &creators = s_creators();
    if (!creators.contains(typeName)) {
        qWarning() << "Unknown type:" << typeName;
        return nullptr;
    }
    QQuickItem *item = creators[typeName]();
    item->setParentItem(parent);
    for (auto it = props.constBegin(); it != props.constEnd(); ++it)
        item->setProperty(it.key().toUtf8().constData(), it.value());

    return item;
}

QQuickItem *Factory::parentItem() const
{
    return m_parentItem;
}

void Factory::setParentItem(QQuickItem *item)
{
    if (m_parentItem != item) {
        m_parentItem = item;
        emit parentItemChanged();
    }
}

QStringList Factory::registeredTypes() const
{
    return s_typeNames();
}
