#pragma once

spot::image diff( const spot::image &A, const spot::image &B ) {
    if( A.w == B.w && A.h == B.h ) {
        spot::image diff( A.w, A.h );
        for( auto end = A.size(), it = end - end; it < end; ++it ) {
            auto error2 = ( B[ it ] - A[ it ] ) * 255;
            auto errorN = sqrt( error2.h * error2.h + error2.s * error2.s + error2.l * error2.l + error2.a * error2.a ) / 4;
            diff[ it ] = spot::rgba( errorN, errorN, errorN, 255 );
        }
        return diff;
    }
    return spot::image();
}

#include <algorithm>
spot::image concat(const spot::image &pic1, const spot::image &pic2, const spot::image &pic3 = spot::image() ) {
    spot::image seq( pic1.w + pic2.w + pic3.w, std::max( std::max( pic1.h, pic2.h ), pic3.h ) );
    seq = seq.paste( 0, 0, pic1 );
    seq = seq.paste( pic1.w, 0, pic2 );
    seq = seq.paste( pic1.w+pic2.w, 0, pic3 );
    return seq;
}

// CImg.h code following
#include "cimg.h"
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
void display(const spot::image &pic, const std::string title = std::string(), bool checkgrid = true) {
    if (pic.size()) {
        using namespace spot;
        // check pattern that covers whole image.
        spot::image check(pic.w, pic.h);
        if (checkgrid) {
            // cell size is 10% of image, check pattern is 10 cells wide.
            //int cell_size = pic.w * 0.10f;
            // cell size is 16px
            int cell_size = 16;
            for (size_t y = 0; y < check.h; ++y) {
                for (size_t x = 0; x < check.w; ++x) {
                    int xc = x / cell_size;
                    int yc = y / cell_size;
                    float light = 0.5f + 0.5f * ((xc % 2) ^ (yc % 2));
                    check.at(x, y) = spot::hsla(0.f, 0.f, light, 1.f);
                }
            }
            // blend image
            for (size_t p = 0, pend = pic.size(); p < pend; ++p) {
                float a = pic.at(p).a;
                check.at(p) = pic.at(p) * a + check.at(p) * (1 - a);
                check.at(p).a = 1;
            }
        }
        else {
            check = pic;
        }
        // display blend
        auto &blend = check;
        cimg_library::CImg<unsigned char> ctexture(blend.w, blend.h, 1, 4, 0);
        for (size_t y = 0; y < blend.h; ++y) {
            for (size_t x = 0; x < blend.w; ++x) {
                spot::pixel pix = blend.at(x, y);
                ctexture(x, y, 0) = pix.r;
                ctexture(x, y, 1) = pix.g;
                ctexture(x, y, 2) = pix.b;
                ctexture(x, y, 3) = pix.a;
            }
        }
        ctexture.display(title.c_str(), false);
    }
}