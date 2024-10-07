#ifndef IKEYBOARD_HH
#define IKEYBOARD_HH

#include <csCore/csCoreExport.hh>
#include <csCore/input/eKey.hh>
#include <csCore/csClass.hh>


namespace cs
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
