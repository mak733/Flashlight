#ifndef GRAPHICSCOLORSVGITEM_H
#define GRAPHICSCOLORSVGITEM_H

#include <QGraphicsSvgItem>
#include <QDomDocument>
#include <QList>

class GraphicsColorSvgItem : public QGraphicsSvgItem
{
public:
    GraphicsColorSvgItem(QString svgContent, QGraphicsItem *parent = 0);
    virtual ~GraphicsColorSvgItem();

    void setColor(QColor c);

private:
    QDomDocument _svgXML;

    void changeAttributes(QString attName, QString attValue);
    void recursiveChangeAttributes(QDomNode node, QString attName, QString attValue);
};

#endif // GRAPHICSCOLORSVGITEM_H
