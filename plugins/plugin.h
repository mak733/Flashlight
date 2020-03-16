#ifndef TLVPLUGIN_H
#define TLVPLUGIN_H

#include <unordered_map>

class QMutex;
class QObject;

class Plugin {
public:
    static Plugin *instance();
    void insert(int id, QObject *plugin);
    void lock(int type, int protocol = -1) const;
    void unlock(int type, int protocol = -1) const;
    QObject *plugin(int type = -1, int protocol = -1) const;

private:
    using Hash = std::unordered_map<int, QObject*>;
    static Plugin *_instance;
    Hash _plugins;
    Plugin();
    QObject *plugin(int protocol, int type, const Hash &plugins) const;
};

#endif // TLVPLUGIN_H
