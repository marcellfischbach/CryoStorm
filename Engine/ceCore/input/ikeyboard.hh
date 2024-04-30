#ifndef IKEYBOARD_HH
#define IKEYBOARD_HH

#include "ceCore/coreexport.hh"
#include "key.hh"
#include "ceCore/class.hh"


namespace ce
{

struct iKeyboard
{
    virtual ~iKeyboard() { }

    virtual bool IsKeyDown(Key key) const = 0;
    virtual bool IsKeyUp(Key key) const = 0;

    virtual bool IsKeyPressed(Key key) const = 0;
    virtual bool IsKeyReleased(Key key) const = 0;

};

}

#endif // IKEYBOARD_HH
