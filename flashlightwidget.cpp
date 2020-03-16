/****************************************************************************
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
    , m_renderer(Native)
    , m_flashlightSvgItem(new QGraphicsSvgItem(flashLightFileName))
    , m_ligthSvgItem(new QGraphicsSvgItem(lightFileName))
    , m_errorSvgItem(new QGraphicsSvgItem(errorFileName))
    , m_backgroundItem(nullptr)
    , m_outlineItem(nullptr)
{
    setScene(new QGraphicsScene(this));

    if (!m_flashlightSvgItem->renderer()->isValid())
        throw std::runtime_error("Can't render flashlight svg file");
    if (!m_ligthSvgItem->renderer()->isValid())
        throw std::runtime_error("Can't render light svg file");
    if (!m_errorSvgItem->renderer()->isValid())
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

    const bool drawBackground = (m_backgroundItem ? m_backgroundItem->isVisible() : false);
    const bool drawOutline = (m_outlineItem ? m_outlineItem->isVisible() : true);

    s->clear();
    resetTransform();

    m_flashlightSvgItem->setFlags(QGraphicsItem::ItemClipsToShape);
    m_flashlightSvgItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    m_flashlightSvgItem->setZValue(2);

    m_ligthSvgItem->setFlags(QGraphicsItem::ItemClipsToShape);
    m_ligthSvgItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    m_ligthSvgItem->setZValue(1);

    m_errorSvgItem->setFlags(QGraphicsItem::ItemClipsToShape);
    m_errorSvgItem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    m_errorSvgItem->setZValue(0);

    m_backgroundItem = new QGraphicsRectItem(m_flashlightSvgItem->boundingRect());
    m_backgroundItem->setBrush(Qt::white);
    m_backgroundItem->setPen(Qt::NoPen);
    m_backgroundItem->setVisible(drawBackground);
    m_backgroundItem->setZValue(-1);

    m_outlineItem = new QGraphicsRectItem(m_flashlightSvgItem->boundingRect());
    QPen outline(Qt::black, 2, Qt::DashLine);
    outline.setCosmetic(true);
    m_outlineItem->setPen(outline);
    m_outlineItem->setBrush(Qt::NoBrush);
    m_outlineItem->setVisible(drawOutline);
    m_outlineItem->setZValue(1);

    s->addItem(m_backgroundItem);
    s->addItem(m_flashlightSvgItem);
    s->addItem(m_ligthSvgItem);
    s->addItem(m_errorSvgItem);
    s->addItem(m_outlineItem);

    s->setSceneRect(m_outlineItem->boundingRect().adjusted(-10, -10, 10, 10));
    return true;
}

bool FlashlightWidget::state() const
{
    return m_state;
}

void FlashlightWidget::setState(bool state)
{
    m_state = state;

    if ((!m_backgroundItem) || (!m_ligthSvgItem))
        return;

    m_backgroundItem->setVisible(m_state);
    m_ligthSvgItem->setVisible(m_state);
    setError(false);
}

void FlashlightWidget::setError(bool state)
{
    m_errorSvgItem->setVisible(state);
}

QColor FlashlightWidget::color() const
{
    return m_color;
}

void FlashlightWidget::setColor(const QColor &color)
{
    m_color = color;
    m_backgroundItem->setBrush(m_color);
    setError(false);
}


void FlashlightWidget::setViewBackground(bool enable)
{
    if (!m_backgroundItem)
          return;

    m_backgroundItem->setVisible(enable);
}

void FlashlightWidget::setBackgroundColor(quint32 color)
{
    if (!m_backgroundItem)
          return;

    setColor(QColor(color));
}

void FlashlightWidget::setViewOutline(bool enable)
{
    if (!m_outlineItem)
        return;

    m_outlineItem->setVisible(enable);
}

void FlashlightWidget::paintEvent(QPaintEvent *event)
{
    if (m_renderer == Image) {
        if (m_image.size() != viewport()->size()) {
            m_image = QImage(viewport()->size(), QImage::Format_ARGB32_Premultiplied);
        }

        QPainter imagePainter(&m_image);
        QGraphicsView::render(&imagePainter);
        imagePainter.end();

        QPainter p(viewport());
        p.drawImage(0, 0, m_image);

    } else {
        QGraphicsView::paintEvent(event);

    }
}

void FlashlightWidget::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

