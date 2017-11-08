/*
 * objecttemplatedocument.cpp
 * Copyright 2017, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2017, Mohamed Thabet <thabetx@gmail.com>
 *
 * This file is part of libtiled.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "objecttemplatedocument.h"

#include "objecttemplateformat.h"
#include "tmxmapformat.h"
#include "savefile.h"

#include <QCoreApplication>
#include <QDir>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

using namespace Tiled;
using namespace Tiled::Internal;

ObjectTemplateDocument::ObjectTemplateDocument(ObjectTemplate *objectTemplate)
    : Document(ObjectTemplateDocumentType, objectTemplate->fileName())
    , mObjectTemplate(objectTemplate)
{
    mFileName = mObjectTemplate->fileName();
}

ObjectTemplateDocument::~ObjectTemplateDocument()
{
}

bool ObjectTemplateDocument::save(const QString &fileName, QString *error)
{
    auto format = mObjectTemplate->format();

    if (!format->write(mObjectTemplate, fileName)) {
        if (error)
            *error = format->errorString();
        return false;
    }

    return true;
}

ObjectTemplateDocument *ObjectTemplateDocument::load(const QString &fileName,
                                                     QString *error)
{
    ObjectTemplate *objectTemplate = readObjectTemplate(fileName, error);

    if (!objectTemplate)
        return nullptr;

    return new ObjectTemplateDocument(objectTemplate);
}

FileFormat *ObjectTemplateDocument::writerFormat() const
{
    return mObjectTemplate->format();
}

QString ObjectTemplateDocument::displayName() const
{
    QString displayName = mObjectTemplate->object()->name();

    if (displayName.isEmpty())
        displayName = mFileName;

    return displayName;
}
