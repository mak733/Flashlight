#ifndef CODOGRAMM_H
#define CODOGRAMM_H

#include <QtGlobal>
#include "header.h"

#pragma pack(push,1)
struct Codogramm {
    Header header;
    Color color;
};
static_assert(sizeof(Codogramm) == 6, "Codogramm size != 6");
#pragma pack(pop)

#endif // CODOGRAMM_H
