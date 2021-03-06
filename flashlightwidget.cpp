﻿/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
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
    : QGraphicsView(parent)
    , _flashlightSvgItem(new QGraphicsSvgItem(flashLightFileName))
    , _ligthSvgItem(new QGraphicsSvgItem(lightFileName))
    , _errorSvgItem(new QGraphicsSvgItem(errorFileName))
    , _backgroundItem(nullptr)
    , _outlineItem(nullptr)
{
    setScene(new QGraphicsScene(this));

    if (!_flashlightSvgItem->renderer()->isValid())
        throw std::runtime_error("Can't render flashlight svg file");
    if (!_ligthSvgItem->renderer()->isValid())
        throw std::runtime_error("Can't render light svg file");
    if (!_errorSvgItem->renderer()->isValid())
        throw std::runtime_error("Can't render error svg file");
    draw();
}

FlashlightWidget::~FlashlightWidget()
{
    qDebug() << "~FlashlightWidget";
}

void FlashlightWidget::drawBackground(QPainter *p, const QRectF &)
{
    p->save();
    p->resetTransform();
    p->drawTiledPixmap(viewport()->rect(), backgroundBrush().texture());
    p->restore();
}

bool FlashlightWidget::draw()
{

    QGraphicsScene *s = scene();

    const bool drawBackground = (_backgroundItem ? _backgroundItem->isVisible() : false);
    const bool drawOutline = (_outlineItem ? _outlineItem->isVisible() : true);

    s->clear();
    resetTransform();

    _flashlightSvgItem->setFlags(QGraphicsItem::ItemClipsToShape);
    _flashlightSvgItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    _flashlightSvgItem->setZValue(2);

    _ligthSvgItem->setFlags(QGraphicsItem::ItemClipsToShape);
    _ligthSvgItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    _ligthSvgItem->setZValue(1);

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
    s->addItem(_ligthSvgItem);
    s->addItem(_errorSvgItem);
    s->addItem(_outlineItem);

    s->setSceneRect(_outlineItem->boundingRect().adjusted(-10, -10, 10, 10));
    setError(false);
    return true;
}

bool FlashlightWidget::state() const
{
    return _state;
}

void FlashlightWidget::setState(bool state)
{
    _state = state;

    if ((!_backgroundItem) || (!_ligthSvgItem))
    {
        qDebug() << "Bad _backgroundItem or _ligthSvgItem";
        return;
    }
    qDebug() << "Widget state:" << _state;
    _backgroundItem->setVisible(_state);
    _ligthSvgItem->setVisible(_state);
    setError(false);
}

void FlashlightWidget::setError(bool state)
{
    _errorSvgItem->setVisible(state);
}

QColor FlashlightWidget::color() const
{
    return _color;
}

void FlashlightWidget::setColor(const QColor &color)
{
    _color = color;
    _backgroundItem->setBrush(_color);
    qDebug() << "Flashlight color chnged to:" << color;
    setError(false);
}


void FlashlightWidget::setViewBackground(bool enable)
{
    if (!_backgroundItem)
        return;

    _backgroundItem->setVisible(enable);
}

void FlashlightWidget::setBackgroundColor(const quint32 color)
{
    if (!_backgroundItem)
        return;

    setColor(QColor(color));
}

void FlashlightWidget::setBackgroundColor(const QColor color)
{
    setColor(color);
}

void FlashlightWidget::setViewOutline(bool enable)
{
    if (!_outlineItem)
        return;

    _outlineItem->setVisible(enable);
}

void FlashlightWidget::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

void FlashlightWidget::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

