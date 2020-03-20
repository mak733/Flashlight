#ifndef OFFPLUGIN_H
#define OFFPLUGIN_H
#include <QDataStream>
#include <QDebug>
#include <QByteArray>
#include "core_interface.h"
#include "defines.h"
#include "codogramm.h"

class ColorPlugin: public QObject, CoreInterface{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "Ivideon.Flashlight.Tlv.ColorPlugin")
#endif
    Q_INTERFACES(CoreInterface)

public:
    unsigned char codogrammType() const {return Command::Color;}
    unsigned char protocol() const {return Protocol::TLV;}
    QVariant value(const QString &member,
                   const QByteArray &data) const;
    QString name() const {return "Цвет";}
};
Q_DECLARE_METATYPE(Color)           //needs for cast Color-type to QVariant
#endif // OFFPLUGIN_H
