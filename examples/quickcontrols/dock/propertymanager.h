#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>

class PropertyManager : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QObject* object READ object WRITE setObject NOTIFY objectChanged FINAL)

public:
    explicit PropertyManager(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QObject *object() const;
    void setObject(QObject *newObject);

    Q_INVOKABLE void setValue(const QString &name, const QVariant &value);
signals:
    void objectChanged();

private:
    enum Roles {
        NameRole = Qt::UserRole,
        ValueRole,
        TypeRole
    };
    struct Property {
        QString name;
        QVariant value;
        QString type;
    };

    void analyseObject();
    QList<Property> _data;
    QObject *m_object = nullptr;
};
