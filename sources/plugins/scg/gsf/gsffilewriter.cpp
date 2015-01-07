/*
-----------------------------------------------------------------------------
This source file is part of OSTIS (Open Semantic Technology for Intelligent Systems)
For the latest info, see http://www.ostis.net

Copyright (c) 2010-2014 OSTIS

OSTIS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OSTIS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OSTIS.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/
#include "gsffilewriter.h"
#include "scgscene.h"
#include "scgnode.h"
#include "scgpair.h"
#include "scgbus.h"
#include "scgcontour.h"

#include <QMessageBox>
#include <QFile>
#include <QTextCodec>
#include <QApplication>


GSFFileWriter::GSFFileWriter()
{

}

GSFFileWriter::~GSFFileWriter()
{

}

bool GSFFileWriter::save(QString file_name, QObject *input)
{
    SCgScene *scene = qobject_cast<SCgScene*>(input);

        QFile fileOut(file_name);
        if (!fileOut.open(QFile::WriteOnly | QFile::Text)) {
                 QMessageBox::warning(0, qAppName(),
                                      QObject::tr("File saving error.\nCannot write file %1:\n%2.")
                                      .arg(file_name)
                                      .arg(fileOut.errorString()));
                 return false;
             }
        stream.setDevice(&fileOut);
        stream.startWriting("UTF-8");

        QString scs = "\n/* SCs_code0.1.0.Level 1 */\n";
        QList<QGraphicsItem *>	items_scs = scene->items ();
        QGraphicsItem * item_scs;
        foreach (item_scs, items_scs)
        {
            if(item_scs->type() == SCgPair::Type)
            {
                SCgObject *obj = static_cast<SCgObject*>(item_scs);
                scs += stream.writeScsObject(obj);
            }
        }
        stream.writeScs(scs);

        /*QList<QGraphicsItem *>	items = scene->items ();
        QGraphicsItem * item;
        foreach (item, items)
        {
            if(SCgObject::isSCgObjectType(item->type()) )
            {
                SCgObject *obj = static_cast<SCgObject*>(item);
                stream.writeObject(obj, true);
            }
        }*/

        stream.endScsWriting();

        QList<QGraphicsItem *>	items1 = scene->items ();
        QGraphicsItem * item1;
        foreach (item1, items1)
        {
            if(SCgObject::isSCgObjectType(item1->type()) )
            {
                SCgObject *obj1 = static_cast<SCgObject*>(item1);
                stream.writeObject(obj1);
            }
        }

        stream.finishWriting();

        fileOut.close();
        return true;
}
