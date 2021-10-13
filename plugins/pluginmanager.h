#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QMap>
#include "core_interface.h"
#include "defines.h"

class QMutex;

class QObject;

class PluginManager {
public:
    using MapHash = QMap<quint8, QHash<quint8, CoreInterface *> *>;

    //banchmark: https://savepearlharbor.com/?p=170017
    //we probably have many codograms and few protocols
    static PluginManager *instance();

    CoreInterface *getPlugin(quint8 protocol, quint8 type) const;

private:
    static PluginManager *_instance;
    MapHash _dataPlugins;

    PluginManager();

    void loadPlugins(QString libDir);
};

#endif // TLVPLUGIN_H
