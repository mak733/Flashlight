#include "graphicscolorsvgitem.h"

#include <QSvgRenderer>
#include <QRgb>
#include <QDebug>

GraphicsColorSvgItem::GraphicsColorSvgItem(QString svgContent, QGraphicsItem *parent) :
    QGraphicsSvgItem(parent),
    _svgXML()
{
    bool success = false;
    QString error;
    int line;
    _svgXML.setContent(svgContent, &success, &error, &line);
    //int count = _svgXML.toByteArray().count();
    setSharedRenderer(new QSvgRenderer(_svgXML.toByteArray()));
}

GraphicsColorSvgItem::~GraphicsColorSvgItem()
{
    delete renderer();
}

void GraphicsColorSvgItem::setColor(QColor c)
{
    changeAttributes("fill", c.name().toUpper());
    changeAttributes("stroke", c.name().toUpper());
    renderer()->load(_svgXML.toByteArray());
}

void GraphicsColorSvgItem::changeAttributes(QString attName, QString attValue)
{
    QDomElement rootElem = _svgXML.documentElement();

    QDomNode n = rootElem.firstChild();
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
            if(e.hasAttribute(attName))
                e.setAttribute(attName, attValue);

            if(n.hasChildNodes())
                recursiveChangeAttributes(n.firstChild(), attName, attValue);
        }
        n = n.nextSibling();
    }
}

void GraphicsColorSvgItem::recursiveChangeAttributes(QDomNode node, QString attName, QString attValue)
{
    QDomNode n = node;
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
            if(e.hasAttribute(attName))
                e.setAttribute(attName, attValue);

            if(n.hasChildNodes())
                recursiveChangeAttributes(n.firstChild(), attName, attValue);
        }
        n = n.nextSibling();
    }
}
