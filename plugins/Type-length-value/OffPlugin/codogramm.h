#ifndef CODOGRAMM_H
#define CODOGRAMM_H

#include <QtGlobal>
#include "../header.h"

struct Codogramm {
    Header header;
};

static_assert(sizeof(Codogramm) == 3, "Codogramm size != 3");

#endif // CODOGRAMM_H
