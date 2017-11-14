/*
 * newtemplatedialog.cpp
 * Copyright 2017, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2017, Mohamed Thabet <thabetx@gmail.com>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "newtemplatedialog.h"
#include "preferences.h"
#include "templatemanager.h"
#include "tmxmapformat.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

namespace Tiled {
namespace Internal {

QString saveObjectTemplate(const MapObject *mapObject)
{
    FormatHelper<ObjectTemplateFormat> helper(FileFormat::ReadWrite);
    QString filter = helper.filter();
    QString selectedFilter = XmlObjectTemplateFormat().nameFilter();

    Preferences *prefs = Preferences::instance();
    QString suggestedFileName = prefs->lastPath(Preferences::ObjectTemplateFile);
    suggestedFileName += QLatin1Char('/');
    if (!mapObject->name().isEmpty())
        suggestedFileName += mapObject->name();
    else
        suggestedFileName += QCoreApplication::translate("Tiled::Internal::MainWindow", "untitled");
    suggestedFileName += QLatin1String(".tx");

    QString fileName = QFileDialog::getSaveFileName(nullptr, QCoreApplication::translate("Tiled::Internal::MainWindow", "Save Template"),
                                                    suggestedFileName,
                                                    filter,
                                                    &selectedFilter);

    if (fileName.isEmpty())
        return QString();

    ObjectTemplateFormat *format = helper.formatByNameFilter(selectedFilter);

    ObjectTemplate objectTemplate;
    objectTemplate.setObject(mapObject);

    if (!format->write(&objectTemplate, fileName)) {
        QMessageBox::critical(nullptr, QCoreApplication::translate("Tiled::Internal::MainWindow", "Error Saving Template"),
                              format->errorString());
        return QString();
    }

    prefs->setLastPath(Preferences::ObjectTemplateFile,
                       QFileInfo(fileName).path());

    return fileName;
}

} // namespace Internal
} // namespace Tiled
