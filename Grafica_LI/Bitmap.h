#pragma once
#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>

class Bitmap
{
public:
    struct Header
    {
        unsigned bfSize, bfReserved, bfOffBits,
            biSize, biWidth, biHeight;

        unsigned short biPlanes, biBitcount;

        unsigned biCompression, biSizeImage,
            biXPelsMeter, biYPelsMeter,
            biClrUsed, biClrImportant;
    };
private:
    Header _header{};
    std::vector<char> _buffer;
public:
    struct Pixel
    {
        unsigned char b, g, r;

        Pixel(const unsigned char R, const unsigned char G, const unsigned char B) : b(B), g(G), r(R) {}

        void set(const unsigned char R, const unsigned char G, const unsigned char B)
        {
            b = B;
            g = G;
            r = R;
        }
    };

    Bitmap(const char* input);
    Bitmap(const unsigned width, const unsigned height);
    Bitmap(Bitmap& input);
    Bitmap(Bitmap&& input);

    Bitmap& operator=(Bitmap& input);
    Bitmap& operator=(Bitmap&& input);

    void open(std::istream& input);

    Pixel* operator[](unsigned index);

    const Pixel* operator[](unsigned index) const;

    void save(std::ostream& output) const;

    unsigned width() const;
    unsigned height() const;
    unsigned size() const;

    Header getHeader() const;

    static unsigned char grayscale(const unsigned char R, const unsigned char G, const unsigned char B);

    std::string ascii() const;
};