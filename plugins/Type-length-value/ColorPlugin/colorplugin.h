#ifndef OFFPLUGIN_H
#define OFFPLUGIN_H
#include <QDataStream>
#include <QDebug>
#include "core_interface.h"

class ColorPlugin: public QObject, CoreInterface{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "Ivideon.OffPlugin")
#endif
    Q_INTERFACES(CoreInterface)

public:
    unsigned char codogrammType() const;
    QVariant value(const QString &member,
                   const QByteArray &data) const;
    QString name() const {return "Цвет";}
};

#endif // OFFPLUGIN_H
