#include "EyerFrontChar.hpp"

namespace Eyer
{
    EyerFrontChar::EyerFrontChar(int _w, int _h, int _x, int _y, uint8_t * _buffer)
    {
        w = _w;
        h = _h;
        x = _x;
        y = _y;
        buffer.Clear();
        buffer.Append(_buffer, w * h);
    }

    EyerFrontChar::EyerFrontChar(const EyerFrontChar & frontChar)
    {
        *this = frontChar;
    }

    EyerFrontChar::~EyerFrontChar()
    {

    }

    EyerFrontChar & EyerFrontChar::operator = (const EyerFrontChar & frontChar)
    {
        buffer = frontChar.buffer;
        w = frontChar.w;
        h = frontChar.h;
        x = frontChar.x;
        y = frontChar.y;
        return *this;
    }
}