#ifndef HEADER_H
#define HEADER_H

#pragma pack(push,1)
struct Header {
    unsigned char type;
    unsigned short length;
};
static_assert(sizeof(Header) == 3, "Header size != 3");
#pragma pack(pop)

#pragma pack(push,1)
struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
static_assert(sizeof(Header) == 3, "Color size != 3");
#pragma pack(pop)

#endif // HEADER_H
