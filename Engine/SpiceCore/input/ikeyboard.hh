#ifndef IKEYBOARD_HH
#define IKEYBOARD_HH

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/input/key.hh>
#include <SpiceRefl/class.hh>


namespace Spice
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
