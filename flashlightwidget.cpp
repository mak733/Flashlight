#include "flashlightwidget.h"

#include <QSvgRenderer>

#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>
#include <QPaintEvent>
#include <qmath.h>

#include <QDebug>


FlashlightWidget::FlashlightWidget(QString flashLightFileName,
                                   QString lightFileName,
                                   QString errorFileName,
                                   QWidget *parent)
        : QGraphicsView(parent), _flashlightSvgItem(new QGraphicsSvgItem(flashLightFileName)),
          _lightSvgItem(new QGraphicsSvgItem(lightFileName)), _errorSvgItem(new QGraphicsSvgItem(errorFileName)),
          _backgroundItem(nullptr), _outlineItem(nullptr) {
    setScene(new QGraphicsScene(this));

    if (!_flashlightSvgItem->renderer()->isValid())
        throw std::runtime_error("Can't render flashlight svg file");
    if (!_lightSvgItem->renderer()->isValid())
        throw std::runtime_error("Can't render light svg file");
    if (!_errorSvgItem->renderer()->isValid())
        throw std::runtime_error("Can't render error svg file");
    draw();
}

FlashlightWidget::~FlashlightWidget() {
    qDebug() << "~FlashlightWidget";
}

void FlashlightWidget::drawBackground(QPainter *p, const QRectF &) {
    p->save();
    p->resetTransform();
    p->drawTiledPixmap(viewport()->rect(), backgroundBrush().texture());
    p->restore();
}

bool FlashlightWidget::draw() {

    QGraphicsScene *s = scene();

    const bool drawBackground = (_backgroundItem ? _backgroundItem->isVisible() : false);
    const bool drawOutline = (_outlineItem ? _outlineItem->isVisible() : true);

    s->clear();
    resetTransform();

    _flashlightSvgItem->setFlags(QGraphicsItem::ItemClipsToShape);
    _flashlightSvgItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    _flashlightSvgItem->setZValue(2);

    _lightSvgItem->setFlags(QGraphicsItem::ItemClipsToShape);
    _lightSvgItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    _lightSvgItem->setZValue(1);

    _errorSvgItem->setFlags(QGraphicsItem::ItemClipsToShape);
    _errorSvgItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    _errorSvgItem->setZValue(0);

    _backgroundItem = new QGraphicsRectItem(_flashlightSvgItem->boundingRect());
    _backgroundItem->setBrush(Qt::white);
    _backgroundItem->setPen(Qt::NoPen);
    _backgroundItem->setVisible(drawBackground);
    _backgroundItem->setZValue(-1);

    _outlineItem = new QGraphicsRectItem(_flashlightSvgItem->boundingRect());
    QPen outline(Qt::black, 2, Qt::DashLine);
    outline.setCosmetic(true);
    _outlineItem->setPen(outline);
    _outlineItem->setBrush(Qt::NoBrush);
    _outlineItem->setVisible(drawOutline);
    _outlineItem->setZValue(1);

    s->addItem(_backgroundItem);
    s->addItem(_flashlightSvgItem);
    s->addItem(_lightSvgItem);
    s->addItem(_errorSvgItem);
    s->addItem(_outlineItem);

    s->setSceneRect(_outlineItem->boundingRect().adjusted(-10, -10, 10, 10));
    setError(false);
    return true;
}

bool FlashlightWidget::state() const {
    return _state;
}

void FlashlightWidget::setState(bool state) {
    _state = state;

    if ((!_backgroundItem) || (!_lightSvgItem)) {
        qDebug() << "Bad _backgroundItem or _lightSvgItem";
        return;
    }
    qDebug() << "Widget state:" << _state;
    _backgroundItem->setVisible(_state);
    _lightSvgItem->setVisible(_state);
    setError(false);
}

void FlashlightWidget::setError(bool state) {
    _errorSvgItem->setVisible(state);
}

QColor FlashlightWidget::color() const {
    return _color;
}

void FlashlightWidget::setColor(const QColor &color) {
    _color = color;
    _backgroundItem->setBrush(_color);
    qDebug() << "Flashlight color chnged to:" << color;
    setError(false);
}


void FlashlightWidget::setViewBackground(bool enable) {
    if (!_backgroundItem)
        return;

    _backgroundItem->setVisible(enable);
}

void FlashlightWidget::setBackgroundColor(const quint32 color) {
    if (!_backgroundItem)
        return;

    setColor(QColor(color));
}

void FlashlightWidget::setBackgroundColor(const QColor color) {
    setColor(color);
}

void FlashlightWidget::setViewOutline(bool enable) {
    if (!_outlineItem)
        return;

    _outlineItem->setVisible(enable);
}

void FlashlightWidget::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);
}

void FlashlightWidget::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

