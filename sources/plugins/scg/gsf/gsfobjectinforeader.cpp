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


#include "gsfobjectinforeader.h"

#include <memory>

#include "scgobjectsinfo.h"
#include "scgnode.h"
#include "scgbus.h"
#include "scgcontour.h"
#include "scgpair.h"

#include <QRegExp>
#include <QStringList>
#include <QPair>


GsfObjectInfoReader::GsfObjectInfoReader(bool isOwner) :
    mIsOwner(isOwner),
    mVersion(qMakePair(0, 0))
{
    createTypesMap();
}

GsfObjectInfoReader::GsfObjectInfoReader(const QDomDocument& document, bool isOwner):
                                                        mIsOwner(isOwner)
{
    createTypesMap();
    read(document);
}

GsfObjectInfoReader::~GsfObjectInfoReader()
{
    if (mIsOwner)
        del();
}

void GsfObjectInfoReader::del()
{
    TypeToObjectsMap::iterator it = mObjectsInfo.begin();
    ObjectInfoList::iterator itList;

    for(;it != mObjectsInfo.end(); ++it)
    {
        for(itList = it.value().begin(); itList != it.value().end(); ++itList)
            delete *itList;
        it.value().clear();
    }
    mObjectsInfo.clear();
}

void GsfObjectInfoReader::createTypesMap()
{
    // initialize types mapping

    // nodes
    mGSFType2TypeAlias["node/-/not_define"] = "node/-/not_define";
    mGSFType2TypeAlias["node/const/not_define"] = "node/-/not_define";
    mGSFType2TypeAlias["node/const/general_node"] = "node/const/general";
    mGSFType2TypeAlias["node/const/predmet"] = "node/const/abstract";
    mGSFType2TypeAlias["node/const/material"] = "node/const/material";
    mGSFType2TypeAlias["node/const/nopredmet"] = "node/const/struct";
    mGSFType2TypeAlias["node/const/symmetry"] = "node/const/tuple";
    mGSFType2TypeAlias["node/const/asymmetry"] = "node/const/tuple";
    mGSFType2TypeAlias["node/const/attribute"] = "node/const/role";
    mGSFType2TypeAlias["node/const/relation"] = "node/const/relation";
    mGSFType2TypeAlias["node/const/atom"] = "node/const/group";
    mGSFType2TypeAlias["node/const/group"] = "node/const/group";

    mGSFType2TypeAlias["node/var/not_define"] = "node/var/general";
    mGSFType2TypeAlias["node/var/general_node"] = "node/var/general";
    mGSFType2TypeAlias["node/var/predmet"] = "node/var/abstract";
    mGSFType2TypeAlias["node/var/material"] = "node/var/material";
    mGSFType2TypeAlias["node/var/nopredmet"] = "node/var/struct";
    mGSFType2TypeAlias["node/var/symmetry"] = "node/var/tuple";
    mGSFType2TypeAlias["node/var/asymmetry"] = "node/var/tuple";
    mGSFType2TypeAlias["node/var/attribute"] = "node/var/role";
    mGSFType2TypeAlias["node/var/relation"] = "node/var/relation";
    mGSFType2TypeAlias["node/var/atom"] = "node/var/group";
    mGSFType2TypeAlias["node/var/group"] = "node/var/group";

    mGSFType2TypeAlias["node/meta/not_define"] = "node/-/not_define";
    mGSFType2TypeAlias["node/meta/general_node"] = "node/var/general";
    mGSFType2TypeAlias["node/meta/predmet"] = "node/var/abstract";
    mGSFType2TypeAlias["node/meta/nopredmet"] = "node/var/struct";
    mGSFType2TypeAlias["node/meta/symmetry"] = "node/var/tuple";
    mGSFType2TypeAlias["node/meta/asymmetry"] = "node/var/tuple";
    mGSFType2TypeAlias["node/meta/attribute"] = "node/var/role";
    mGSFType2TypeAlias["node/meta/relation"] = "node/var/relation";
    mGSFType2TypeAlias["node/meta/atom"] = "node/var/group";
    mGSFType2TypeAlias["node/meta/group"] = "node/var/group";

    // pairs
    mGSFType2TypeAlias["arc/-/-"] = "pair/-/-/-/orient/accessory";
    mGSFType2TypeAlias["pair/noorient"] = "pair/-/-/-/noorient";
    mGSFType2TypeAlias["pair/orient"] = "pair/-/-/-/orient";

    mGSFType2TypeAlias["arc/const/pos"] = "pair/const/pos/perm/orient/accessory";
    mGSFType2TypeAlias["arc/var/pos"] = "pair/var/pos/perm/orient/accessory";
    mGSFType2TypeAlias["arc/meta/pos"] = "pair/var/pos/perm/orient/accessory";

    mGSFType2TypeAlias["arc/const/neg"] = "pair/const/neg/perm/orient/accessory";
    mGSFType2TypeAlias["arc/var/neg"] = "pair/var/neg/perm/orient/accessory";
    mGSFType2TypeAlias["arc/meta/neg"] = "pair/var/neg/perm/orient/accessory";

    mGSFType2TypeAlias["arc/const/fuz"] = "pair/const/fuz/perm/orient/accessory";
    mGSFType2TypeAlias["arc/var/fuz"] = "pair/var/fuz/perm/orient/accessory";
    mGSFType2TypeAlias["arc/meta/fuz"] = "pair/var/fuz/perm/orient/accessory";

    mGSFType2TypeAlias["arc/const/pos/temp"] = "pair/const/pos/temp/orient/accessory";
    mGSFType2TypeAlias["arc/var/pos/temp"] = "pair/var/pos/temp/orient/accessory";
    mGSFType2TypeAlias["arc/meta/pos/temp"] = "pair/var/pos/temp/orient/accessory";

    mGSFType2TypeAlias["arc/const/neg/temp"] = "pair/const/neg/temp/orient/accessory";
    mGSFType2TypeAlias["arc/var/neg/temp"] = "pair/var/neg/temp/orient/accessory";
    mGSFType2TypeAlias["arc/meta/neg/temp"] = "pair/var/neg/temp/orient/accessory";

    mGSFType2TypeAlias["arc/const/fuz/temp"] = "pair/const/fuz/temp/orient/accessory";
    mGSFType2TypeAlias["arc/var/fuz/temp"] = "pair/var/fuz/temp/orient/accessory";
    mGSFType2TypeAlias["arc/meta/fuz/temp"] = "pair/var/fuz/temp/orient/accessory";

    mGSFType2TypeAlias["pair/const/synonym"] = "pair/const/-/-/noorien";
    mGSFType2TypeAlias["pair/var/noorient"] = "pair/var/-/-/noorien";
    mGSFType2TypeAlias["pair/meta/noorient"] = "pair/var/-/-/noorien";

    mGSFType2TypeAlias["pair/const/orient"] = "pair/const/-/-/orient";
    mGSFType2TypeAlias["pair/var/orient"] = "pair/var/-/-/orient";
    mGSFType2TypeAlias["pair/meta/orient"] = "pair/var/-/-/orient";

    //deprecated. For supporting old format.
    mGSFType2TypeAlias["pair/rail/noorient"] = "pair/var/-/-/noorien";
    mGSFType2TypeAlias["pair/rail/orient"] = "pair/var/-/-/orient";
    mGSFType2TypeAlias["pair/rail2/noorient"] = "pair/var/-/-/noorien";
    mGSFType2TypeAlias["pair/rail2/orient"] = "pair/var/-/-/orient";
}

bool GsfObjectInfoReader::read(const QDomDocument& document)
{
    if (mIsOwner)
        del();
    mLastError.clear();

    QDomElement root = document.documentElement();

    if (root.tagName() != "GSF")
    {
        mLastError = QString(QObject::tr("Given document has unsupported format %1").arg(root.tagName()));
        return false;
    }
    else
    {
        QStringList v_list = root.attribute("version").split(".");
        mVersion.first = v_list.first().toInt();
        mVersion.second = v_list.last().toInt();
        if (mVersion != qMakePair(1, 6) && mVersion != qMakePair(2, 0))
        {
            mLastError = QString(QObject::tr("Version %1 of GSF files not supported.\n"
                                        "Just 1.6 and 2.0 versions supported.")).arg(root.attribute("version"));
            return false;
        }
    }


    // read edges from scs sector
    QDomElement scsSector = root.firstChildElement("scsSector");

    mEdges = parseScsSector(scsSector);

    // get sintax sector
    QDomElement sintaxSector = root.firstChildElement("sintaxSector");

    // parse sintax nodes
    QDomElement element = sintaxSector.firstChildElement("node");
    while (!element.isNull())
    {
        if (!parseNode(element))
            return false;

        element = element.nextSiblingElement("node");
    }

    // parse sintax arcs
    element = sintaxSector.firstChildElement("arc");
    while (!element.isNull())
    {
        if (!parsePair(element))
            return false;

        element = element.nextSiblingElement("arc");
    }

    // parse sintax pairs
    element = sintaxSector.firstChildElement("pair");
    while (!element.isNull())
    {
        if (!parsePair(element))
            return false;

        element = element.nextSiblingElement("pair");        
    }

    // parse sintax bus
    element = sintaxSector.firstChildElement("bus");
    while (!element.isNull())
    {
        if (!parseBus(element))
            return false;

        element = element.nextSiblingElement("bus");
    }

    // parse sintax contour
    element = sintaxSector.firstChildElement("contour");
    while (!element.isNull())
    {
        if (!parseContour(element))
            return false;

        element = element.nextSiblingElement("contour");
    }

    // create links
    SCgObjectInfo *pairObject;
    foreach(pairObject, mObjectsInfo[SCgPair::Type])
    {
        SCgPairInfo* pair = static_cast<SCgPairInfo*>(pairObject);

        QString pairIdtf = pair->idtfValue();

        pair->beginObjectIdRef() = mIdtf2Id[mEdges[pairIdtf].first.idtf];
        pair->endObjectIdRef() = mIdtf2Id[mEdges[pairIdtf].second.idtf];
    }

    return true;
}

bool GsfObjectInfoReader::parseObject(const QDomElement &element, SCgObjectInfo* info)
{
    if(info->objectType() == SCgPair::Type || info->objectType() == SCgNode::Type)
    {
        QString& type = info->typeAliasRef();
        if (getAttributeString(element, "type", type))
        {
            if (!mGSFType2TypeAlias.contains(type))
            {
                errorUnknownElementType(element.tagName(), type);
                return false;
            }else
                type = mGSFType2TypeAlias[type];
        }
        else
            return false;
    }

    if (!getAttributeString(element, "id", info->idRef()))
        return false;

    if (!getAttributeString(element, "parent", info->parentIdRef()))
        return false;

    if (!getAttributeString(element, "idtf", info->idtfValueRef()))
        return false;

    mIdtf2Id.insert(info->idtfValueRef(), info->idRef());

    return true;
}

bool GsfObjectInfoReader::parseNode(const QDomElement &element)
{
    std::auto_ptr<SCgNodeInfo> nodeInfo(new SCgNodeInfo());

    if(!parseObject(element,nodeInfo.get()))
        return false;

    qreal& x = nodeInfo->posRef().rx();
    qreal& y = nodeInfo->posRef().ry();
    if (!getAttributeDouble(element, "x", x) || !getAttributeDouble(element, "y", y))
        return false;


    // get identifier position
    int& idtfPos = nodeInfo->idtfPosRef();
    if (!getAttributeInt(element, "idtf_pos", idtfPos))
        idtfPos = 0;

    // get content element
    QDomElement contEl = element.firstChildElement("content");
    if (contEl.isNull())
    {
        errorHaventContent(element.tagName());
        return false;
    }

    // get content type
    int& cType = nodeInfo->contentTypeRef();
    if (!getAttributeInt(contEl, "type", cType))
        return false;

    // get mime type
    if (!getAttributeString(contEl, "mime_type", nodeInfo->contentMimeTypeRef()))
        return false;

    // in old versions format, there wasn't content_visibility attribute, so we need to check if it exists
    if (contEl.hasAttribute("content_visibility") && !getAttributeBool(contEl, "content_visibility", nodeInfo->contentVisibleRef()))
        return false;

    // set content to nodeInfo
    if (cType > 0 && cType < 5)
    {
        if (cType == 1 || cType == 2 || cType == 3)
            nodeInfo->contentDataRef() = QVariant(contEl.firstChild().nodeValue());
        else if (cType == 4)
        {
            // get file name
            getAttributeString(contEl, "file_name", nodeInfo->contentFilenameRef());
            QString cData = contEl.firstChild().nodeValue();
            QByteArray arr = QByteArray::fromBase64(cData.toLocal8Bit());
            nodeInfo->contentDataRef() = QVariant(arr);
        }else
        {
            mLastError = QObject::tr("Content type '%1' doesn't supported for now").arg(cType);
            return false;
        }
    }else if (cType != 0)
    {
        mLastError = QObject::tr("Unknown content type '%1'").arg(cType);
        return false;
    }

    mObjectsInfo[SCgNode::Type].append(nodeInfo.release());

    return true;
}

bool GsfObjectInfoReader::parsePair(const QDomElement &element)
{
    std::auto_ptr<SCgPairInfo> pairInfo(new SCgPairInfo());

    if(!parseObject(element,pairInfo.get()))
        return false;

//    if (!getAttributeString(element, "id_b", pairInfo->beginObjectIdRef()) ||
//        !getAttributeString(element, "id_e", pairInfo->endObjectIdRef()))
//        return false;

    if (!getAttributeDouble(element, "dotBBalance", pairInfo->beginDotRef()) ||
        !getAttributeDouble(element, "dotEBalance", pairInfo->endDotRef()))
        return false;

    pairInfo->pointsRef().push_back(QPointF());
    if (!getElementPoints(element, pairInfo->pointsRef()))
        return false;
    pairInfo->pointsRef().push_back(QPointF());

    mObjectsInfo[SCgPair::Type].append(pairInfo.release());
    return true;
}


QPair<QString, QString> GsfObjectInfoReader::parseTypeIdent(const QString& str)
{
    QRegExp rxValue("\\s*(\\S+)#(\\S*)\\s*");

    if(rxValue.exactMatch(str))
    {
        QString type = rxValue.cap(1);
        QString ident  = rxValue.cap(2);

        return QPair<QString,QString>(type, ident);
    }

    return QPair<QString,QString>();
}

QMap<QString, Edge> GsfObjectInfoReader::parseScsSector(const QDomElement& scsSector)
{
    QMap<QString, Edge> result;

    // read sector value
    QString scsValue = scsSector.text();

    // remove comment
    QRegExp rxComment("/\\*.*\\*/");

    scsValue.replace(rxComment, "");

    //
    QStringList scsLines = scsValue.split(QString(";;"));
    QString scsLine;

    foreach(scsLine, scsLines)
    {
        QStringList sublines = scsLine.split(QString("|"));
        if(sublines.count()==3)
        {
            Node nodeFirst(parseTypeIdent(sublines[0]));
            Node nodeSecond(parseTypeIdent(sublines[2]));
            Edge edge(parseTypeIdent(sublines[1]), nodeFirst, nodeSecond);

            result.insert(edge.idtf, edge);
        }
    }

    return result;
}


bool GsfObjectInfoReader::parseBus(const QDomElement &element)
{
    std::auto_ptr<SCgBusInfo> busInfo(new SCgBusInfo());

    if(!parseObject(element,busInfo.get()))
        return false;

    if (!getAttributeString(element, "owner", busInfo->ownerIdRef()))
        return false;

    double bx, by;
    if (!getAttributeDouble(element, "b_x", bx) || !getAttributeDouble(element, "b_y", by))
        return false;
    double ex, ey;
    if (!getAttributeDouble(element, "e_x", ex) || !getAttributeDouble(element, "e_y", ey))
        return false;

    busInfo->pointsRef().append(QPointF(bx, by));

    if (!getElementPoints(element, busInfo->pointsRef()))
        return false;

    busInfo->pointsRef().append(QPointF(ex, ey));

    mObjectsInfo[SCgBus::Type].append(busInfo.release());
    return true;
}

bool GsfObjectInfoReader::parseContour(const QDomElement &element)
{
    std::auto_ptr<SCgContourInfo> contourInfo(new SCgContourInfo());

    if(!parseObject(element,contourInfo.get()))
        return false;

    if (!getElementPoints(element, contourInfo->pointsRef()))
        return false;

    mObjectsInfo[SCgContour::Type].append(contourInfo.release());
    return true;
}

bool GsfObjectInfoReader::getAttributeString(const QDomElement &element, QString attribute, QString &result)
{
    if (element.hasAttribute(attribute))
    {
        result = element.attribute(attribute);
        return true;
    }

    errorHaventAttribute(element.tagName(), attribute);
    return false;
}

bool GsfObjectInfoReader::getAttributeBool(const QDomElement &element, QString attribute, bool &result)
{
    QString strResult;
    if (!getAttributeString(element, attribute, strResult))
        return false;
    else
        if (strResult == "false")
            result = false;
        else if (strResult == "true")
            result = true;
        else
        {
            errorBoolParse(element.tagName(), attribute);
            return false;
        }
    return true;
}

bool GsfObjectInfoReader::getAttributeDouble(const QDomElement &element, QString attribute, double &result)
{
    if (element.hasAttribute(attribute))
    {
        bool res;
        result = element.attribute(attribute).toDouble(&res);

        if (!res) return false;

        return true;
    }

    errorHaventAttribute(element.tagName(), attribute);
    return false;
}

bool GsfObjectInfoReader::getAttributeInt(const QDomElement &element, QString attribute, int &result)
{
    if (element.hasAttribute(attribute))
    {
        bool res;
        result = element.attribute(attribute).toInt(&res);

        if (!res) return false;

        return true;
    }
    errorHaventAttribute(element.tagName(), attribute);
    return false;
}

bool GsfObjectInfoReader::getElementPoints(const QDomElement &element, QVector<QPointF> &result)
{
    QDomElement points = element.firstChildElement("points");
    if (points.isNull())
    {
        mLastError = QObject::tr("There are no points data for element '%1'").arg(element.tagName());
        return false;
    }

    QDomElement point = points.firstChildElement("point");
    while (!point.isNull())
    {
        double x, y;
        if (!getAttributeDouble(point, "x", x) || !getAttributeDouble(point, "y", y))
            return false;
        result.push_back(QPointF(x, y));
        point = point.nextSiblingElement("point");
    }

    return true;
}

void GsfObjectInfoReader::errorHaventAttribute(QString element, QString attribute)
{
    mLastError = QObject::tr("'%1' element haven't '%2' attribute").arg(element).arg(attribute);
}

void GsfObjectInfoReader::errorFloatParse(QString element, QString attribute)
{
    mLastError = QObject::tr("invalid float value in attribute '%1' of element '%2'").arg(attribute).arg(element);
}

void GsfObjectInfoReader::errorBoolParse(QString element, QString attribute)
{
    mLastError = QObject::tr("invalid bollean value in attribute '%1' of element '%2'").arg(attribute).arg(element);
}

void GsfObjectInfoReader::errorHaventContent(QString element)
{
    mLastError = QObject::tr("node element '%1' haven't content tag").arg(element);
}

void GsfObjectInfoReader::errorUnknownElementType(QString element, QString type)
{
    mLastError = QObject::tr("type '%1' is unknown for element '%2'").arg(type).arg(element);
}
