#ifndef ONPLUGIN_H
#define ONPLUGIN_H
#include <QDataStream>
#include <QDebug>
#include "core_interface.h"

class OnPlugin: public QObject, CoreInterface{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "Ivideon.OnPlugin")
#endif
    Q_INTERFACES(CoreInterface)

public:
    unsigned char codogrammType() const;
    QVariant value(const QString &member, const QByteArray &data) const;
    QString name() const {return "Включить фонарь";}
};

#endif // ONPLUGIN_H
