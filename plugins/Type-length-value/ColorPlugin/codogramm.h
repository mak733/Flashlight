#ifndef CODOGRAMM_H
#define CODOGRAMM_H

#include <QtGlobal>
#include "../header.h"

struct Codogramm {
    Header header;
    uint32_t color : 24;
};

static_assert(sizeof(Codogramm) == 8, "Codogramm size != 8");

#endif // CODOGRAMM_H
