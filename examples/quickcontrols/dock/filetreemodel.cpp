// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "filetreemodel.h"

FileTreeModel::FileTreeModel(QObject *parent) : QFileSystemModel{ parent }
{
    setRootPath("");
}
