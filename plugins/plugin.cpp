#include <QMutex>
#include <QPluginLoader>
#include "plugin.h"
#include <QDebug>

Plugin *Plugin::_instance = nullptr;

Plugin::Plugin() {
    QPluginLoader laoder;
    foreach(QObject *instance, laoder.staticInstances()) {
//        CoreInterface *core = qobject_cast<CoreInterface *>(instance);
//        if(qobject_cast<StreamInterface *>(instance) != nullptr)
//            _streamPlugins[core->protocol()][core->type()] = {instance, new QMutex {}};
//        else
//            _dataPlugins[core->protocol()][core->type()] = {instance, new QMutex {}};
    }
}

void Plugin::insert(int id, QObject *plugin) {
//    CoreInterface *core = qobject_cast<CoreInterface *>(plugin);
//    Hash &hash = plugin->inherits("StreamInterface") ? _streamPlugins : _dataPlugins;
//    hash[core->protocol()].insert({id,  {plugin, new QMutex {}}});
}

void Plugin::lock(int type, int protocol) const {
//    auto i = _dataPlugins.find(protocol);
//    if(i != _dataPlugins.end()) {
//        auto j = i->second.find(type);
//        if(j != i->second.end())
//            j->second.second->lock();
//    }
}

void Plugin::unlock(int type, int protocol) const {
//    auto i = _dataPlugins.find(protocol);
//    if(i != _dataPlugins.end()) {
//        auto j = i->second.find(type);
//        if(j != i->second.end())
//            j->second.second->unlock();
//    }
}

//QObject *TlvPlugin::streamPlugin(int type, int protocol) const {
//    return plugin(protocol, type, _streamPlugins);
//}

//QObject *TlvPlugin::dataPlugin(int type, int protocol) const {
//    return plugin(protocol, abs(type), _dataPlugins);
//}

QObject *Plugin::plugin(int protocol, int type, const Hash &plugins) const {
    if(plugins.size() == 0)
        return nullptr;
    auto i = protocol < 0 ? plugins.begin() : plugins.find(protocol);
    if(i != plugins.end()) {
//        auto j = type < 0 ? i->second.begin() : i->second.find(type);
//        return j == i->second.end() ? nullptr : j->second.first;
    }
    return nullptr;
}

Plugin *Plugin::instance() {
    static QMutex mutex;
    if(_instance == nullptr) {
        QMutexLocker locker(&mutex);
        _instance = new Plugin();
    }
    return _instance;
}
