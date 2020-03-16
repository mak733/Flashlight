#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H

#include <QtPlugin>

class CoreInterface {
public:
    virtual ~CoreInterface() {}
    virtual unsigned char codogrammType() const = 0;
    virtual QVariant value(const QString &member,
                           const QByteArray &data) const = 0;
    virtual QString name() const = 0;
};
Q_DECLARE_INTERFACE(CoreInterface, "CoreInterface")

#endif // CORE_INTERFACE_H
