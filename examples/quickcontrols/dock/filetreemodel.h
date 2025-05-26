// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <QFileSystemModel>
#include <QQmlEngine>

class FileTreeModel : public QFileSystemModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    FileTreeModel(QObject *parent = nullptr);
};
