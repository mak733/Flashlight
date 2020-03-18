#include <QDateTime>
#include <functional>
#include <unordered_map>
#include "codogramm.h"
#include "colorplugin.h"

using namespace std;

QVariant ColorPlugin::value(const QString &member, const QByteArray &data) const {
    static const unordered_map<string, function<QVariant(const Codogramm *)>> map {
    {"тип", [](const Codogramm *data) {return data->header.type;}},
    {"размер", [](const Codogramm *data) {return data->header.length;}},
    {"цвет", [](const Codogramm *data) {return data->color;}},
};
    auto codogramm = reinterpret_cast<const Codogramm *>(data.constData());
    auto iter = map.find(member.toStdString());
    return iter == map.end() ? QVariant {} : iter->second(codogramm);
}

