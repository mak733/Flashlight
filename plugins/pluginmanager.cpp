﻿#include <QMutex>
#include <QPluginLoader>

#include "pluginmanager.h"
#include <QDebug>
#include <QDir>
#include <QDirIterator>

PluginManager *PluginManager::_instance = nullptr;

PluginManager::PluginManager() {
    loadPlugins("./plugins/libs");
}

void PluginManager::loadPlugins(QString libDir)
{
    QDirIterator it(libDir , QStringList() << "*.so", QDir::Files, QDirIterator::Subdirectories);
    qDebug() << "search plugins in:" << libDir;
    while (it.hasNext())
    {
        it.next();
        QPluginLoader loader( it.fileInfo().absoluteFilePath() );
        qDebug() << "Found:" << it.fileInfo().absoluteFilePath();
        if( loader.load() ) {
            if( CoreInterface *core = qobject_cast<CoreInterface *>(loader.instance()) )
            {
                if(_dataPlugins[core->protocol()] == nullptr)
                {
                    _dataPlugins[core->protocol()] = new QHash<quint8, CoreInterface*>;
                }
                _dataPlugins[core->protocol()]->insert(core->codogrammType(), core);
                qDebug() << "Loaded new plugin:" << core->name();
            }
            //loader.unload();
        } else {
            qDebug() << "Failed to load new plugin" << it.filePath() << loader.errorString();
        }
    }
}

CoreInterface *PluginManager::getPlugin(quint8 protocol, quint8 type) const {
    const QHash<quint8, CoreInterface*> *curProtocol = _dataPlugins[protocol];
    if(curProtocol == nullptr)
    {
        qDebug() << "Failed to get translate plugin:" << " bad protocol type" << protocol;
        return nullptr;
    }
    if(curProtocol->value(type) == nullptr)
    {
        qDebug() << "Failed to get translate plugin:" << "protocol type" << protocol << "can't find plugin to translate type" << type;
        return nullptr;
    }
    return curProtocol->value(type);
}

PluginManager *PluginManager::instance() {
    if(_instance == nullptr) {
        _instance = new PluginManager();
    }
    return _instance;
}
