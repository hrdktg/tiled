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
#include "objecttemplatemodel.h"
#include "preferences.h"
#include "templatemanager.h"
#include "tmxmapformat.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

namespace Tiled {
namespace Internal {

void saveObjectTemplate(const MapObject *mapObject)
{
    FormatHelper<ObjectTemplateFormat> helper(FileFormat::ReadWrite);
    QString filter = helper.filter();
    QString selectedFilter = XmlObjectTemplateFormat().nameFilter();

    Preferences *prefs = Preferences::instance();
    QString suggestedFileName = prefs->lastPath(Preferences::TemplateDocumentsFile);
    // todo: base suggested file name on the name of the object, when non-empty
    suggestedFileName += QCoreApplication::translate("Tiled::Internal::MainWindow", "untitled");
    suggestedFileName += QLatin1String(".tx");

    QString fileName = QFileDialog::getSaveFileName(nullptr, QCoreApplication::translate("Tiled::Internal::MainWindow", "Save Template"),
                                                    suggestedFileName,
                                                    filter,
                                                    &selectedFilter);

    if (fileName.isEmpty())
        return;

    ObjectTemplateFormat *format = helper.formatByNameFilter(selectedFilter);

    QScopedPointer<ObjectTemplate> objectTemplate(new ObjectTemplate(fileName));
    objectTemplate->setFormat(format);
    objectTemplate->setObject(mapObject);

    QScopedPointer<ObjectTemplateDocument>
        objectTemplateDocument(new ObjectTemplateDocument(objectTemplate.data()));

    QString error;
    if (!objectTemplateDocument->save(fileName, &error)) {
        QMessageBox::critical(nullptr, QCoreApplication::translate("Tiled::Internal::MainWindow", "Error Saving Template"), error);
        return;
    }

//    auto model = ObjectTemplateModel::instance();
//    model->addNewDocument(ObjectTemplateDocument.take());

    prefs->setLastPath(Preferences::TemplateDocumentsFile,
                       QFileInfo(fileName).path());
}

} // namespace Internal
} // namespace Tiled
