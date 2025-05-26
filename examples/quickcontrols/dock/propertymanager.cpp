// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "propertymanager.h"

PropertyManager::PropertyManager(QObject *parent) : QAbstractListModel{ parent } { }

int PropertyManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _data.size();
}

QVariant PropertyManager::data(const QModelIndex &index, int role) const
{

    auto d = _data.at(index.row());

    switch (static_cast<Roles>(role)) {
    case PropertyManager::NameRole:
        return d.name;
    case PropertyManager::ValueRole:
        return d.value;
    case PropertyManager::TypeRole:
        return d.type;
    }

    return {};
}

QHash<int, QByteArray> PropertyManager::roleNames() const
{

    // clang-format off
    return {
        {NameRole, "name"},
        {ValueRole, "value"},
        {TypeRole, "type"},
    };
    // clang-format on
}

QObject *PropertyManager::object() const
{
    return m_object;
}

void PropertyManager::setObject(QObject *newObject)
{
    if (m_object == newObject)
        return;
    m_object = newObject;
    analyseObject();
    emit objectChanged();
}

void PropertyManager::setValue(const QString &name, const QVariant &value)
{
    if (!m_object)
        return;

    qDebug() << "Set" << name << "to" << value;
    m_object->setProperty(name.toUtf8().data(), value);
}

void PropertyManager::analyseObject()
{
    if (!m_object)
        return;

    beginResetModel();
    _data.clear();
    auto m = m_object->metaObject();
    for (int i = 0; i < m->propertyCount(); ++i) {
        auto objectProperty = m->property(i);
        Property p;
        p.name = objectProperty.name();
        p.value = objectProperty.read(m_object);
        p.type = QMetaType(objectProperty.typeId()).name();
        _data << p;
    }
    endResetModel();
    qDebug() << _data.size() << "properties found";
}
