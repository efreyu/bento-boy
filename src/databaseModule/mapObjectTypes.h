#ifndef BENTO_BOY_MAPOBJECTTYPES_H
#define BENTO_BOY_MAPOBJECTTYPES_H

#include <string>

namespace bt::databaseModule {

    enum class eMapObjectType {
        UNDEFINED = 0,
        HERO,
        FOOD
    };

    eMapObjectType getMapObjectTypeByString(const std::string& string);
}

#endif// BENTO_BOY_MAPOBJECTTYPES_H
