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
#include "gsfstreamwriter.h"
#include "scgnode.h"
#include "scgpair.h"
#include "scgcontour.h"
#include "scgbus.h"
#include "scgtextitem.h"

#include <QTextCodec>

GsfStreamWriter::GsfStreamWriter(): QXmlStreamWriter(),
                                    isWritingStarted(false)
{
    createTypesMap();
}

GsfStreamWriter::GsfStreamWriter(QIODevice* device):QXmlStreamWriter(device),
                                                    isWritingStarted(false)
{
    createTypesMap();
}

GsfStreamWriter::GsfStreamWriter(QByteArray* array):QXmlStreamWriter(array),
                                                    isWritingStarted(false)
{
    createTypesMap();
}

GsfStreamWriter::~GsfStreamWriter()
{

}

void GsfStreamWriter::createTypesMap()
{
    // nodes
    mTypeAlias2GSFType["node/-/not_define"] = "node/-/not_define";

    mTypeAlias2GSFType["node/const/general"] = "node/const/general_node";
    mTypeAlias2GSFType["node/const/abstract"] = "node/const/predmet";
    mTypeAlias2GSFType["node/const/material"] = "node/const/material";
    mTypeAlias2GSFType["node/const/struct"] = "node/const/nopredmet";
    mTypeAlias2GSFType["node/const/symmetry"] = "node/const/symmetry";
    mTypeAlias2GSFType["node/const/tuple"] = "node/const/asymmetry";
    mTypeAlias2GSFType["node/const/role"] = "node/const/attribute";
    mTypeAlias2GSFType["node/const/relation"] = "node/const/relation";
    mTypeAlias2GSFType["node/const/atom"] = "node/const/atom";
    mTypeAlias2GSFType["node/const/group"] = "node/const/group";

    mTypeAlias2GSFType["node/var/not_define"] = "node/var/not_define";
    mTypeAlias2GSFType["node/var/general"] = "node/var/general_node";
    mTypeAlias2GSFType["node/var/abstract"] = "node/var/predmet";
    mTypeAlias2GSFType["node/var/material"] = "node/var/material";
    mTypeAlias2GSFType["node/var/struct"] = "node/var/nopredmet";
    mTypeAlias2GSFType["node/var/tuple"] = "node/var/symmetry";
    mTypeAlias2GSFType["node/var/asymmetry"] = "node/var/asymmetry";
    mTypeAlias2GSFType["node/var/role"] = "node/var/attribute";
    mTypeAlias2GSFType["node/var/relation"] = "node/var/relation";
    mTypeAlias2GSFType["node/var/atom"] = "node/var/atom";
    mTypeAlias2GSFType["node/var/group"] = "node/var/group";

    mTypeAlias2GSFType["node/meta/not_define"] = "node/meta/not_define";
    mTypeAlias2GSFType["node/meta/general"] = "node/meta/general_node";
    mTypeAlias2GSFType["node/meta/abstract"] = "node/meta/predmet";
    mTypeAlias2GSFType["node/meta/nopredmet"] = "node/meta/nopredmet";
    mTypeAlias2GSFType["node/meta/tuple"] = "node/meta/symmetry";
    mTypeAlias2GSFType["node/meta/asymmetry"] = "node/meta/asymmetry";
    mTypeAlias2GSFType["node/meta/role"] = "node/meta/attribute";
    mTypeAlias2GSFType["node/meta/relation"] = "node/meta/relation";
    mTypeAlias2GSFType["node/meta/atom"] = "node/meta/atom";
    mTypeAlias2GSFType["node/meta/group"] = "node/meta/group";

    // pairs
    mTypeAlias2GSFType["pair/-/-/-/orient/accessory"] = "arc/-/-";
    mTypeAlias2GSFType["pair/-/-/-/noorient"] = "pair/noorient";
    mTypeAlias2GSFType["pair/-/-/-/orient"] = "pair/orient";

    mTypeAlias2GSFType["pair/const/pos/perm/orient/accessory"] = "arc/const/pos";
    mTypeAlias2GSFType["pair/var/pos/perm/orient/accessory"] = "arc/var/pos";
    mTypeAlias2GSFType["pair/meta/pos/perm/orient/accessory"] = "arc/meta/pos";

    mTypeAlias2GSFType["pair/const/neg/perm/orient/accessory"] = "arc/const/neg";
    mTypeAlias2GSFType["pair/var/neg/perm/orient/accessory"] = "arc/var/neg";
    mTypeAlias2GSFType["pair/meta/neg/perm/orient/accessory"] = "arc/meta/neg";

    mTypeAlias2GSFType["pair/const/fuz/perm/orient/accessory"] = "arc/const/fuz" ;
    mTypeAlias2GSFType["pair/var/fuz/perm/orient/accessory"] = "arc/var/fuz";
    mTypeAlias2GSFType["pair/meta/fuz/perm/orient/accessory"] = "arc/meta/fuz";

    mTypeAlias2GSFType["pair/const/pos/temp/orient/accessory"] = "arc/const/pos/temp";
    mTypeAlias2GSFType["pair/var/pos/temp/orient/accessory"] = "arc/var/pos/temp";
    mTypeAlias2GSFType["pair/meta/pos/temp/orient/accessory"] = "arc/meta/pos/temp";

    mTypeAlias2GSFType["pair/const/neg/temp/orient/accessory"] = "arc/const/neg/temp";
    mTypeAlias2GSFType["pair/var/neg/temp/orient/accessory"] = "arc/var/neg/temp";
    mTypeAlias2GSFType["pair/meta/neg/temp/orient/accessory"] = "arc/meta/neg/temp";

    mTypeAlias2GSFType["pair/const/fuz/temp/orient/accessory"] = "arc/const/fuz/temp";
    mTypeAlias2GSFType["pair/var/fuz/temp/orient/accessory"] = "arc/var/fuz/temp";
    mTypeAlias2GSFType["pair/meta/fuz/temp/orient/accessory"] = "arc/meta/fuz/temp";


    mTypeAlias2GSFType["pair/const/-/-/noorien"] = "pair/const/synonym";
    mTypeAlias2GSFType["pair/var/-/-/noorien"] = "pair/var/noorient";
    mTypeAlias2GSFType["pair/meta/-/-/noorien"] = "pair/meta/noorient";

    mTypeAlias2GSFType["pair/const/-/-/orient"] = "pair/const/orient";
    mTypeAlias2GSFType["pair/var/-/-/orient"] = "pair/var/orient";
    mTypeAlias2GSFType["pair/meta/-/-/orient"] = "pair/meta/orient";
}

void GsfStreamWriter::setDevice(QIODevice* device)
{
    QXmlStreamWriter::setDevice(device);
}

void GsfStreamWriter::startWriting(const char* encoding)
{
    QTextCodec *codec = QTextCodec::codecForName(encoding);
    setCodec(codec);
    setAutoFormatting(true);
    writeStartDocument();
    writeStartElement("GSF");
    writeAttribute("version", "2.0");
    writeStartElement("scsSector");
    isWritingStarted = true;
}

void GsfStreamWriter::endScsWriting()
{
    Q_ASSERT(isWritingStarted);

    writeEndElement(); /*scsSector*/
    writeStartElement("sintaxSector");
}

void GsfStreamWriter::finishWriting()
{
    Q_ASSERT(isWritingStarted);

    writeEndElement(); /*sintaxSector*/
    writeEndElement(); /*GSF*/
    writeEndDocument();
}

void GsfStreamWriter::writeScs(QString scs)
{
    Q_ASSERT(isWritingStarted);

    writeCharacters(scs);
}

QString GsfStreamWriter::writeScsObject(SCgObject *object)
{
    Q_ASSERT(isWritingStarted);
        QString scs;
        SCgPair *pair = static_cast<SCgPair*>(object);
        //1
        if (mTypeAlias2GSFType[pair->beginObject()->typeAlias()].mid(0,3)=="arc") {
            scs += "sc_arc_main#";
        }
        else if (mTypeAlias2GSFType[pair->beginObject()->typeAlias()].mid(0,3)=="pai")
             scs += "sc_arc_common#";

        if (pair->beginObject()->type() == SCgNode::Type) {
            scs += "sc_node#";
        }
        scs += pair->beginObject()->idtfValue() + "|";

        //2
        if (mTypeAlias2GSFType[pair->typeAlias()].mid(0,3)=="arc") {
            scs += "sc_arc_main#";
        }
        else if (mTypeAlias2GSFType[pair->typeAlias()].mid(0,3)=="pai")
             scs += "sc_arc_common#";
        scs += pair->idtfValue() + "|";

        //3
        if (mTypeAlias2GSFType[pair->endObject()->typeAlias()].mid(0,3)=="arc") {
            scs += "sc_arc_main#";
        }
        else if (mTypeAlias2GSFType[pair->endObject()->typeAlias()].mid(0,3)=="pai")
             scs += "sc_arc_common#";

        if (pair->endObject()->type() == SCgNode::Type) {
             scs += "sc_node#";
        }
        scs += pair->endObject()->idtfValue() + ";;\n";
    return scs;
}

void GsfStreamWriter::writeObject(SCgObject *object)
{
    Q_ASSERT(isWritingStarted);

    switch (object->type())
    {
    case SCgNode::Type:
        writeNode(object);
        break;
    case SCgPair::Type:
        writePair(object);
        break;
    case SCgBus::Type:
        writeBus(object);
        break;
    case SCgContour::Type:
        writeContour(object);
        break;
    }
}

void GsfStreamWriter::writeObjectAttributes(SCgObject *obj)
{
    writeAttribute("type", mTypeAlias2GSFType[obj->typeAlias()]);
    writeAttribute("idtf", obj->idtfValue());
    writeAttribute("shapeColor", QString::number(obj->color().value()));
    writeAttribute("id", QString::number( obj->id() ));
    writeAttribute("parent", QString::number( obj->parentId() ));
    writeText(obj);
}

void GsfStreamWriter::writeNode( SCgObject *obj)
{
    writeStartElement("node");
    writeObjectAttributes(obj);
    writePosition(obj, "x", "y");

    SCgNode *node = static_cast<SCgNode*>(obj);

    writeAttribute("haveBus", node->bus() ? "true" : "false");
    writeAttribute("idtf_pos", QString::number((int)node->idtfPos()));

    writeContent(node);

    writeEndElement();//node
}

void GsfStreamWriter::writeContent(SCgNode *node)
{
    writeStartElement("content");

    int cType = node->contentType();
    QString mimeType = node->contentMimeType();

    writeAttribute("type",QString::number(cType));
    writeAttribute("mime_type", node->contentMimeType());
    writeAttribute("content_visibility", node->isContentVisible() ? "true" : "false");
    writeAttribute("file_name", node->contentFileName());

    switch(cType)
    {
        case 1:
        case 2:
        case 3:
        {
            writeCDATA((node->contentData()).toString());
            break;
        }
        case 4:
        {
            QByteArray arr = node->contentData().toByteArray().toBase64();
            writeCDATA(arr);
            break;
        }
    }

    writeEndElement();//content
}

void GsfStreamWriter::writePair(SCgObject *obj)
{
    QString type = mTypeAlias2GSFType[obj->typeAlias()].mid(0,3);
    if(type=="arc")
        writeStartElement(type);
    else
        writeStartElement("pair");
    writeObjectAttributes(obj);
    SCgPair* pair = static_cast<SCgPair*>(obj);
    SCgObject* b = pair->beginObject();
    SCgObject* e = pair->endObject();
//    writeAttribute("id_b", QString::number(b->id()));
//    writeAttribute("id_e", QString::number(e->id()));

    writePosition(b,"b_x","b_y");
    writePosition(e,"e_x","e_y");

    writeAttribute("dotBBalance", QString::number(pair->beginDot()));
    writeAttribute("dotEBalance", QString::number(pair->endDot()));
    QVector<QPointF> points = pair->scenePoints();
    points.pop_back();
    points.pop_front();
    writePoints(points);
    writeEndElement();
}

void GsfStreamWriter::writeBus(SCgObject *obj)
{
    writeStartElement("bus");
    writeObjectAttributes(obj);
    SCgBus* bus = static_cast<SCgBus*>(obj);

    writeAttribute("owner", QString::number(bus->owner()->id()));

    QVector<QPointF> points = bus->scenePoints();
    writeAttribute("b_x", QString::number(points.first().x()));
    writeAttribute("b_y", QString::number(points.first().y()));
    writeAttribute("e_x", QString::number(points.last().x()));
    writeAttribute("e_y", QString::number(points.last().y()));

    // do not save begin and end points
    points.pop_back();
    points.pop_front();
    writePoints(points);

    writeEndElement();
}

void GsfStreamWriter::writeContour(SCgObject *obj)
{
    writeStartElement("contour");
    writeObjectAttributes(obj);
    SCgContour* contour = static_cast<SCgContour*>(obj);

    QVector<QPointF> points(contour->scenePoints());
    writePoints(points);

    writeEndElement();
}

void GsfStreamWriter::writePosition(SCgObject *obj,const QString& x, const QString& y)
{
    QPointF globPos = obj->scenePos();

    writeAttribute(x, QString::number(globPos.x()));
    writeAttribute(y, QString::number(globPos.y()));
}

void GsfStreamWriter::writePoints(const QVector<QPointF>& points)
{
    writeStartElement("points");

    foreach(const QPointF& point,points)
    {
        writeStartElement("point");
        writeAttribute("x", QString::number(point.x()));
        writeAttribute("y", QString::number(point.y()));
        writeEndElement();
    }
    writeEndElement();
}

void GsfStreamWriter::writeText(SCgObject *obj)
{
    if(obj->textItem())
    {
        writeAttribute("left", QString::number(obj->textItem()->boundingRect().left()));
        writeAttribute("top", QString::number(obj->textItem()->boundingRect().top()));
        writeAttribute("right", QString::number(obj->textItem()->boundingRect().right()));
        writeAttribute("bottom", QString::number(obj->textItem()->boundingRect().bottom()));

        writeAttribute("textColor", QString::number(obj->textItem()->defaultTextColor().value()));
        writeAttribute("text_angle", QString::number(obj->textItem()->rotation()));

        writeAttribute("text_font", obj->textItem()->font().family());
        writeAttribute("font_size", QString::number(obj->textItem()->font().pointSize()));
    }
}
