#include "pch.h"
#include "Bitmap.h"
#include <stack>

Bitmap::Bitmap(const char* input)
{
    open(std::ifstream(input, std::ios::binary));
}

Bitmap::Bitmap(const unsigned width, const unsigned height)
{
    _header.bfOffBits = sizeof(Header) + 2;

    _header.biSize = 40;
    _header.biWidth = width;
    _header.biHeight = height;
        
    _header.biPlanes = 1;
    _header.biBitcount = 24;

    _header.biSizeImage = height * (width + (width & 3)) * sizeof(Pixel);

    _header.bfSize = _header.bfOffBits + _header.biSizeImage;

    _buffer.resize(_header.biSizeImage, '\xff');
}

Bitmap::Bitmap(Bitmap& input)
{
    operator=(input);
}

Bitmap::Bitmap(Bitmap&& input)
{
    operator=(std::move(input));
}

Bitmap& Bitmap::operator=(Bitmap& input)
{
    _header = input._header;
    _buffer = input._buffer;
    return *this;
}

Bitmap& Bitmap::operator=(Bitmap&& input)
{
    _header = input._header;
    _buffer = std::move(input._buffer);
    return *this;
}

void Bitmap::open(std::istream& input)
{
    if (input.get() == 'B' && input.get() == 'M')
    {
        input.read(reinterpret_cast<char*>(&_header), sizeof(_header));

        if (_header.biBitcount != 0x18)
            throw std::runtime_error("Unsupported bitmap file");

        _buffer.resize(_header.biSizeImage);
        input.read(_buffer.data(), _header.biSizeImage);
    }
}

auto Bitmap::operator[](unsigned index) -> Pixel*
{
    index = _header.biHeight - index - 1;

    auto tm = _buffer.data() + index * _header.biWidth * sizeof(Pixel);

    if (index)
        tm += index * (_header.biWidth & 3);

    return reinterpret_cast<Pixel*>(tm);
}

auto Bitmap::operator[](unsigned index) const -> const Pixel*
{
    index = _header.biHeight - index - 1;

    auto tm = _buffer.data() + index * _header.biWidth * sizeof(Pixel);

    if (index)
        tm += index * (_header.biWidth & 3);

    return reinterpret_cast<const Pixel*>(tm);
}

void Bitmap::save(std::ostream& output) const
{
    (output << "BM")
        .write(reinterpret_cast<const char*>(&_header), sizeof(_header))
        .write(_buffer.data(), _header.biSizeImage);
}

unsigned Bitmap::width() const
{
    return _header.biWidth;
}

unsigned Bitmap::height() const
{
    return _header.biHeight;
}

unsigned Bitmap::size() const
{
    return _header.biSizeImage;
}

auto Bitmap::getHeader() const -> Header
{
    return _header;
}

// https://stackoverflow.com/questions/17615963/standard-rgb-to-grayscale-conversion
unsigned char Bitmap::grayscale(const unsigned char r, const unsigned char g, const unsigned char b)
{
    auto cLinear = r / 255.f * .2126f + g / 255.f * .7152f + b / 255.f * .0722f;
    return static_cast<unsigned char>(255 * (cLinear > 0.0031308f ?
        1.055f * powf(cLinear, 1 / 2.4f) - 0.055f :
        12.92f * cLinear));
}

std::string Bitmap::ascii() const
{
    std::string tm;
    tm.reserve(_header.biHeight * (_header.biWidth + 1));

    for (unsigned j, i = 0; i < _header.biHeight; ++i)
    {
        for (j = 0; j < _header.biWidth; ++j)
        {
            auto gray = int(grayscale((*this)[i][j].r, (*this)[i][j].r, (*this)[i][j].b) / 255.f * 17);
            tm.push_back("@@B##&80o+i;:,.'` "[gray]);
        }

        tm.push_back('\n');
    }

    return tm;
}