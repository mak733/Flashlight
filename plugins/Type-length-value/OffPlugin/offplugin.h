#ifndef OFFPLUGIN_H
#define OFFPLUGIN_H
#include <QDataStream>
#include <QDebug>
#include "core_interface.h"
#include "defines.h"

class OffPlugin: public QObject, CoreInterface{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "Ivideon.Flashlight.Tlv.OffPlugin")
#endif
    Q_INTERFACES(CoreInterface)

public:
    unsigned char codogrammType() const {return Command::Off;}
    unsigned char protocol() const {return Protocol::TLV;}
    QVariant value(const QString &member,
                   const QByteArray &data) const;
    QString name() const {return "Выключить фонарь";}
};

#endif // OFFPLUGIN_H
