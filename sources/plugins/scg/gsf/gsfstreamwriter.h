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

#ifndef GSFSTREAMWRITER_H_
#define GSFSTREAMWRITER_H_

#include <QXmlStreamWriter>
#include <QVector>
#include <QPointF>
#include <QMap>

class SCgObject;
class SCgNode;

//! Class for writing data in gsf format.
/*! NOTE: First of all you have to specify device for storing information \
    @see GsfStreamWriter::GsfStreamWriter(QIODevice* device),\
    @see GsfStreamWriter::GsfStreamWriter(QByteArray* array),\
    @see GsfStreamWriter::setDevice(QIODevice* device).
    Next step is to write header information needed for gwf-format.\
    @see  GsfStreamWriter::startWriting(const char* encoding = "UTF-8").
    After writing all objects by GsfStreamWriter::writeObject(SCgObject *object) \
    You have to finish writing by calling GsfStreamWriterfinishWriting().
*/
class GsfStreamWriter: private QXmlStreamWriter
{

public:
    GsfStreamWriter();
    explicit GsfStreamWriter(QIODevice* device);
    explicit GsfStreamWriter(QByteArray* array);
    virtual ~GsfStreamWriter();

    //! Sets up current writing device. @see QXmlStreamWriter::setDevice
    void setDevice(QIODevice* device);

    /*! Writes start element for GSF-document.
     * @param encoding Encoding, for writer
     */
    void startWriting(const char* encoding = "UTF-8");

    //! Finishes writing staticSector.
    void endScsWriting();

    //! Finishes writing GSF-document.
    void finishWriting();

    //! Analyzes type of object and writes it to specified device;
    void writeObject(SCgObject *object);
    void writeScs(QString scs);
    QString writeScsObject(SCgObject *object);

private:
    bool isWritingStarted;
    /*! Save sc.g-node.
      @param node Element of sc.g-node type.
      */
    void writeNode(SCgObject *node);

    /*! Save sc.g-pair.
      @param node Element of sc.g-pair.
      */
    void writePair(SCgObject *node);

    /*! Save sc.g-bus.
      @param obj Element of sc.g-bus.
      */
    void writeBus(SCgObject *obj);

    /*! Save sc.g-contour.
      @param obj Element of sc.g-contour.
      */
    void writeContour(SCgObject *obj);

    /*! Save sc.g-Object attributes.
      @param obj Element of sc.g-object.
      */
    void writeObjectAttributes(SCgObject *obj);

    /*! Save point position.
      @param obj Element of sc.g-object.
      @param x Name of attribute x-positions.
      @param y Name of attribute y-positions.
      */
    void writePosition(SCgObject *obj,const QString& x, const QString& y);

    /*! Save points array.
      @param points Vector of points
      */
    void writePoints(const QVector<QPointF>& points);

    /*! Save text attribute.
      @param obj Element of sc.g-object.
      */
    void writeText(SCgObject *obj);

    /*! Save content in node.
      @param node Element of sc.g-node type.
      */
    void writeContent(SCgNode *node);

    typedef QMap<QString, QString> GSFType2TypeAliasMap;
    //! Map for mapping gsf types to types alias
    GSFType2TypeAliasMap mTypeAlias2GSFType;

    void createTypesMap();
};

#endif /* GSFSTREAMWRITER_H_ */
