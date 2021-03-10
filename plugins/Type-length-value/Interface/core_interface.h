#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H
//Подумать над размещением этого интерфейса в иерархии плагинов.
//Следует разместить этот класс в папке plugins, и применять его к новым протоколам
#include <QtPlugin>

class CoreInterface {
public:
    virtual ~CoreInterface() {}
    virtual QVariant value(const QString &member,
                           const QByteArray &data) const = 0;
    virtual QString name() const = 0;
    virtual quint8 codogrammType() const = 0;
    virtual quint8 protocol() const = 0;
};
Q_DECLARE_INTERFACE(CoreInterface, "CoreInterface")

#endif // CORE_INTERFACE_H
