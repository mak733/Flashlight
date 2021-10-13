#include <QDateTime>
#include <functional>
#include <unordered_map>
#include "codogramm.h"
#include "onplugin.h"

using namespace std;

QVariant OnPlugin::value(const QString &member, const QByteArray &data) const {
    static const unordered_map<string, function<QVariant(const Codogramm *)>> map{
            {"тип",     [](const Codogramm *data) { return data->header.type; }},
            {"размер",  [](const Codogramm *data) { return data->header.length; }},
            {"питание", [](const Codogramm *data) { return (data->header.type == Flashlight::On); }},
    };
    auto codogramm = reinterpret_cast<const Codogramm *>(data.constData());
    auto iter = map.find(member.toStdString());
    return iter == map.end() ? QVariant{} : iter->second(codogramm);
}

