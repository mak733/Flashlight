#ifndef STREAM_INTERFACE_H
#define STREAM_INTERFACE_H

#include <QtPlugin>

class QString;

class QIODevice;

class StreamInterface {
public:
    virtual ~StreamInterface() {}

    virtual void clear() const = 0;

    virtual QByteArray readStreamHeader(QIODevice *device, quint32 &type) const = 0;

    virtual QByteArray readBlock(QIODevice *device, QString *message = nullptr) const = 0;

    virtual QByteArray readData(QByteArray &block, quint32 &type, QString *message = nullptr) const = 0;
};

Q_DECLARE_INTERFACE(StreamInterface, "StreamInterface")

#endif // STREAM_INTERFACE_H
