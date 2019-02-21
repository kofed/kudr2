#ifndef POSITION_H
#define POSITION_H

#include "QMetaType"

enum Position{
    LEFT,
    RIGHT,
    NOTSET
};

extern Position positions[2];

Q_DECLARE_METATYPE(Position)

#endif // POSITION_H
