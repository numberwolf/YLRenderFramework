#ifndef EYERLIB_EYERFRONTCHAR_HPP
#define EYERLIB_EYERFRONTCHAR_HPP

#include "EyerBuffer.hpp"

namespace Eyer
{
    class EyerFrontChar
    {
    public:
        EyerFrontChar(int _w, int _h, int _x, int _y, uint8_t * _buffer);
        EyerFrontChar(const EyerFrontChar & frontChar);

        ~EyerFrontChar();

        EyerFrontChar & operator = (const EyerFrontChar & frontChar);

        EyerBuffer buffer;
        int w;
        int h;
        int x;
        int y;
    };
}

#endif //EYERLIB_EYERFRONTCHAR_HPP
