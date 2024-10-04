#ifndef IKEYBOARD_HH
#define IKEYBOARD_HH

#include "ceCore/csCoreExport.hh"
#include "eKey.hh"
#include "ceCore/csClass.hh"


namespace cryo
{

struct iKeyboard
{
    virtual ~iKeyboard() { }

    virtual bool IsKeyDown(eKey key) const = 0;
    virtual bool IsKeyUp(eKey key) const = 0;

    virtual bool IsKeyPressed(eKey key) const = 0;
    virtual bool IsKeyReleased(eKey key) const = 0;

};

}

#endif // IKEYBOARD_HH
