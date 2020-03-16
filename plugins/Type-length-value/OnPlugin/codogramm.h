#ifndef CODOGRAMM_H
#define CODOGRAMM_H

#include <QtGlobal>
#include "../header.h"

struct Codogramm {
    Header header;
};

static_assert(sizeof(Codogramm) == 4, "Codogramm size != 4");

#endif // CODOGRAMM_H
