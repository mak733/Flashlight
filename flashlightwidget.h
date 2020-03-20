/****************************************************************************
**
**
****************************************************************************/

#ifndef SVGVIEW_H
#define SVGVIEW_H

#include <QGraphicsColorizeEffect>
#include <QGraphicsView>
#include <QFile>


QT_BEGIN_NAMESPACE
class QGraphicsColorizeEffect;
class QGraphicsSvgItem;
class QSvgRenderer;
class QPaintEvent;
class QFile;
QT_END_NAMESPACE

class FlashlightWidget : public QGraphicsView
{
    Q_OBJECT

public:

    explicit FlashlightWidget(QString flashLightFileName,
                     QString lightFileName, QString errorFileName,
                     QWidget *parent = nullptr);
    ~FlashlightWidget();

    QColor color() const;
    bool state() const;

    void setBackgroundColor(const quint32 color);
    void setBackgroundColor(const QColor color);

public slots:
    void setViewBackground(bool enable);
    void setState(bool state);

    void setViewOutline(bool enable);
    void setError(bool state);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void drawBackground(QPainter *p, const QRectF &rect) override;

private:
    bool draw();

    QImage m_image;

    QGraphicsSvgItem *_flashlightSvgItem;
    QGraphicsSvgItem *_ligthSvgItem;
    QGraphicsSvgItem *_errorSvgItem;
    QGraphicsRectItem *_backgroundItem;
    QGraphicsRectItem *_outlineItem;

    bool _state {false};

    QColor _color {Qt::white};
    void setColor(const QColor &color);
};
#endif // SVGVIEW_H
