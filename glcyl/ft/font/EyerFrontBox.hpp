#ifndef EYERLIB_EYERFRONTBOX_HPP
#define EYERLIB_EYERFRONTBOX_HPP

#include <string>

#include "EyerBuffer.hpp"

namespace Eyer
{
    class EyerFrontBoxPrivate;
    class EyerFrontBox
    {
    public:
        EyerFrontBox(unsigned char *_fontData, int _dataSize, int _fontSize);
        EyerFrontBox(const std::string & _frontPath, int _frontSize);
        ~EyerFrontBox();

        /**
         * @upgrade github.com/numberwolf
         * @param outBuffer
         * @param width
         * @param height
         * @param str
         * @return
         */
        int GenText(EyerBuffer & outBuffer, int & width, int & height, wchar_t * str);
        /**
         * @upgrade github.com/numberwolf
         * @param outBuffer
         * @param width
         * @param height
         * @param str
         * @param align
         * @return
         */
        int GenText(EyerBuffer & outBuffer, int & width, int & height, wchar_t * str, size_t align);

        EyerFrontBoxPrivate * piml = nullptr;
        std::string frontPath;
        int frontSize = 100;
    };
}

#endif //EYERLIB_EYERFRONTBOX_HPP
