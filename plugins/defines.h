#ifndef DEFINES_H
#define DEFINES_H

namespace Command {
    enum Type {
        Undefined = -1,
        On = 0x12,
        Off = 0x13,
        Color = 0x20
    };
};

namespace Protocol {
    enum Type {
        Undefined = -1,
        TLV = 1
    };
}

namespace Flashlight {
    enum State {
        On = Command::On,
        Off = Command::Off,
    };
}

#endif // DEFINES_H
