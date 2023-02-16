#ifndef EYERLIB_EYERFRONTBOXPRIVATE_HPP
#define EYERLIB_EYERFRONTBOXPRIVATE_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Eyer
{
    class EyerFrontBoxPrivate
    {
    public:
        FT_Library ft;
        FT_Face face;
    };
}

#endif //EYERLIB_EYERFRONTBOXPRIVATE_HPP
