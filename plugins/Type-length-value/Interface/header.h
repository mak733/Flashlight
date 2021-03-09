#ifndef HEADER_H
#define HEADER_H

#pragma pack(push,1)
struct Header {
    quint8 type;
    quint16 length;
};
static_assert(sizeof(Header) == 3, "Header size != 3");
#pragma pack(pop)

#pragma pack(push,1)
struct Color {
    quint8 red;
    quint8 green;
    quint8 blue;
};
static_assert(sizeof(Header) == 3, "Color size != 3");
#pragma pack(pop)

#endif // HEADER_H
