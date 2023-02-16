#include "EyerFrontBox.hpp"

#include <iostream>
#include <locale.h>
#include <wchar.h>
#include <math.h>

#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "EyerFrontBoxPrivate.hpp"
#include "EyerFrontChar.hpp"

namespace Eyer
{
    EyerFrontBox::EyerFrontBox(unsigned char *_fontData, int _dataSize, int _fontSize) {
        frontSize = _fontSize;
        piml = new EyerFrontBoxPrivate();

        if (FT_Init_FreeType(&piml->ft)) {
            printf("ERROR::FREETYPE: FT_New_Memory_Face Could not init FreeType Library\n");
        }

        // typedef unsigned char  FT_Byte;
        // FT_New_Memory_Face( FT_Library      library,
        //     const FT_Byte*  file_base,
        //     FT_Long         file_size,
        //     FT_Long         face_index,
        //     FT_Face        *aface );
        if (FT_New_Memory_Face(piml->ft, (const FT_Byte *)_fontData, _dataSize, 0, &piml->face)) {
            printf("ERROR::FREETYPE: FT_New_Memory_Face Failed to load font\n");
        }

        FT_Set_Pixel_Sizes(piml->face, 0, frontSize);

        FT_Long num_glyphs = piml->face->num_glyphs;
    }

    EyerFrontBox::EyerFrontBox(const std::string & _frontPath, int _frontSize)
    {
        frontPath = _frontPath;
        frontSize = _frontSize;
        piml = new EyerFrontBoxPrivate();

        if (FT_Init_FreeType(&piml->ft)) {
            printf("ERROR::FREETYPE: Could not init FreeType Library\n");
        }

        if (FT_New_Face(piml->ft, frontPath.c_str(), 0, &piml->face)) {
        //if (FT_New_Face(piml->ft, "http://localhost:8002/cpp/front.ttf", 0, &piml->face)) {
            printf("ERROR::FREETYPE: Failed to load font\n");
        }

        FT_Set_Pixel_Sizes(piml->face, 0, frontSize);

        FT_Long num_glyphs = piml->face->num_glyphs;
    }

    EyerFrontBox::~EyerFrontBox()
    {
        if(piml->face != nullptr){
            if (FT_Done_Face(piml->face)) {
                printf("ERROR::FREETYPE: Could not FT_Done_Face\n");
            }
            piml->face = nullptr;
        }

        if(piml->ft != nullptr){
            if (FT_Done_FreeType(piml->ft)) {
                printf("ERROR::FREETYPE: Could not FT_Done_FreeType\n");
            }
            piml->ft = nullptr;
        }
        if(piml != nullptr){
            delete piml;
            piml = nullptr;
        }
    }

    int EyerFrontBox::GenText(EyerBuffer & outBuffer, int & width, int & height, wchar_t * str) {
        return GenText(outBuffer, width, height, str, 4);
    }

    int EyerFrontBox::GenText(EyerBuffer & outBuffer, int & width, int & height, wchar_t * str, size_t align)
    {
        printf("start EyerFrontBox::GenText\n");
        setlocale(LC_ALL, "");

        printf("start EyerFrontBox::GenText wcslen\n");
        int num_chars = wcslen(str);
        if (num_chars <= 0) {
            return -1;
        }

        FT_GlyphSlot slot = piml->face->glyph;
        int pen_x = 0;
        int pen_y = 200;

        std::vector<EyerFrontChar> charList;
        printf("start EyerFrontBox::GenText int n = 0; n < num_chars; n++\n");
        for (int n = 0; n < num_chars; n++) {
            wchar_t t = str[n];

            FT_UInt  glyph_index;

            glyph_index = FT_Get_Char_Index(piml->face, t);

            FT_Error error = FT_Load_Glyph( piml->face, glyph_index, FT_LOAD_DEFAULT );
            if ( error ) continue;

            error = FT_Render_Glyph( piml->face->glyph, ft_render_mode_normal );
            if ( error ) continue;

            int w = slot->bitmap.width;
            int h = slot->bitmap.rows;
            int x = pen_x + slot->bitmap_left;
            int y = pen_y - slot->bitmap_top;

            EyerFrontChar frontChar(w, h, x, y, slot->bitmap.buffer);
            charList.push_back(frontChar);

            pen_x += slot->advance.x >> 6;
            pen_y += slot->advance.y >> 6;
        }

        int canvasWidth = pen_x;
        int canvasHeight = 0;
        int max_front_h = 0;
        printf("start EyerFrontBox::GenText int i=0; i<charList.size(); i++\n");
        for (int i = 0; i < charList.size(); i++) {
            const EyerFrontChar & frontChar = charList[i];
            int hh = frontChar.h + frontChar.y;
            std::cout << "frontChar.h:" << frontChar.h << ",frontChar.y:" << frontChar.y << std::endl;
            canvasHeight = std::max(hh, canvasHeight);
            max_front_h = std::max(frontChar.h, max_front_h);
        } // end for charList: compute the char's size in image

        max_front_h += 15; // 补充 line height
        width = canvasWidth + (align - canvasWidth % align);
        height = max_front_h;

        int unused_h = canvasHeight - max_front_h;
        std::cout << "unused_h:" << unused_h << ",max_front_h:" << max_front_h << std::endl;

        printf("ttf width:%d height:%d\n", width, height);
        outBuffer = EyerBuffer(width * height);

        uint8_t * canvas = outBuffer.GetPtr();
        memset(canvas, 0, width * height);

        printf("start EyerFrontBox::GenText int i=0; i<charList.size(); i++\n");
        for (int i = 0; i < charList.size(); i++) {
            printf("\n==========Char index:%d==========\n", i);
            const EyerFrontChar &frontChar = charList[i];

            for (int y = 0; y < frontChar.h; y++) {
                printf("line%03d     ", y);
                for (int x = 0; x < frontChar.w; x++) {
                    int index = y * frontChar.w + x;
                    uint8_t *src = frontChar.buffer.GetPtr();

                    int distX = frontChar.x + x;
                    int distY = frontChar.y + y;

                    if (distX < 0 || distX >= canvasWidth) {
                        continue;
                    }
                    if (distY < unused_h || distY >= canvasHeight) {
                        continue;
                    }

                    // int distIndex = distY * width + distX;
                    int distIndex = (frontChar.y + y - unused_h) * width + distX;
                    canvas[distIndex] = src[index];
                    // if (y == 0 || y == frontChar.h - 1) {
                    //     std::cout << " " << distY;
                    // }

                    for (int s_index = 0; s_index < 2; s_index++) {
                        putchar(" .:ioVM@"[src[index] >> 5]); // 2^3=8, 8char
                    }
                    // if (src[index] > 0) {
                    //     printf("@@");
                    // } else {
                    //     printf("  ");
                    // }
                }
                printf("\n");
            } // end for frontChar w h pix buf to canvas buffer
        } // end for charList

        printf("ttf result %dx%d\n", canvasWidth, canvasHeight);

        return 0;
    }
}
