#include "pylonfeaturesenum.h"

QString NameForPylon::getName(int &numb, int &Type, int Column)
{
    if (numb==-1 || Column==-1 ) return nullptr;
    switch (Type){
    case PylonFeaturesEnum::DInt:
    {
        try {
            return IntName.at(static_cast<int>(numb*3+Column));
        } catch (std::out_of_range)
        {
            return nullptr;
        }
    }
    case PylonFeaturesEnum::DEnum:
    {
        try {
            return EnumName.at(static_cast<int>(numb*3+Column));
        } catch (std::out_of_range)
        {
            return nullptr;
        }
    }
    case PylonFeaturesEnum::DFloat:
    {
        try {
            return FloatName.at(static_cast<int>(numb*3+Column));
        } catch (std::out_of_range) {
            return nullptr;
        }
    }
    default:
    {
        return nullptr;
    }
    }
}
