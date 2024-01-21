#include "pch.h"
#include "Form1.h"
#include "AsciiForm.h"
#include <omp.h>
#include <msclr/marshal_cppstd.h>
#include <complex>

static double mandel_value(const Bitmap& image, const double x, const double y, const int it)
{
    std::complex<double> point(x / (image.width() / 2.8) - 2.1, y / (image.height() / 2.5) - 1.24), z;

    for (auto i = 0; ; ++i)
    {
        if (i >= it || abs(z) >= 2)
            return double(i) / it;
        
        z = z * z + point;
    }
}

::Bitmap Grafica::Form1::GenerateBitmap()
{
    ::Bitmap f(unsigned(width->Value), unsigned(height->Value));

    auto r = button2->BackColor.R, g = button2->BackColor.G, b = button2->BackColor.B;
    auto it = int(iterations->Value);

#pragma omp parallel for
    for (int i = 0; i < int(f.height()); ++i)
        for (auto j = 0u; j < f.width(); ++j)
        {
            auto val = mandel_value(f, j, i, it);
            f[i][j].set(Byte(r * val), Byte(g * val), Byte(b * val));
        }

    return f;
}

inline void Grafica::Form1::button1_Click(Object^, EventArgs^)
{
    std::string fileName;
    SaveFileDialog sfd;
    sfd.Filter = L"BMP file (*.bmp)|*.bmp";
    sfd.Title = L"Exportă BMP";

    if (sfd.ShowDialog() != Windows::Forms::DialogResult::OK)
        return;

    fileName = msclr::interop::marshal_as<std::string, String^>(sfd.FileName);
    
    GenerateBitmap().save(std::ofstream(fileName.c_str(), std::ios::binary));
    pictureBox1->ImageLocation = sfd.FileName;
    Windows::Forms::MessageBox::Show(this, "Exportat cu succes.", "Succes", MessageBoxButtons::OK, MessageBoxIcon::Information);
}

inline void Grafica::Form1::button2_Click(Object^, EventArgs^)
{
    ColorDialog c;

    if (c.ShowDialog() == Windows::Forms::DialogResult::OK)
        button2->BackColor = c.Color;
}

inline void Grafica::Form1::button3_Click(Object^, EventArgs^)
{
    auto bmp = GenerateBitmap();

    std::ostringstream out;
    bmp.save(out);

    auto str = out.str();

    delete image;
    pictureBox1->Image = image = Image::FromStream(%IO::UnmanagedMemoryStream((Byte*)str.data(), str.size()));
}

inline void Grafica::Form1::button4_Click(Object^, EventArgs^)
{
    auto str = (gcnew String(GenerateBitmap().ascii().c_str()))->Replace("\n", "\r\n");
    Grafica_LI::AsciiForm(str, button2->BackColor).ShowDialog();
}

inline void Grafica::Form1::pictureBox1_Click(Object^, EventArgs^)
{
    OpenFileDialog ofd;
    ofd.Filter = L"BMP file (*.bmp)|*.bmp";
    ofd.Title = L"Importă BMP";

    if (ofd.ShowDialog() != Windows::Forms::DialogResult::OK)
        return;

    auto fileName = msclr::interop::marshal_as<std::string, String^>(ofd.FileName);

    try
    {
        ::Bitmap bmp(fileName.c_str());
        auto gdiBmp = gcnew Drawing::Bitmap(bmp.width(), bmp.height());

        for (auto i = 0u; i < bmp.height(); ++i)
            for (auto j = 0u; j < bmp.width(); ++j)
            {
                auto pixel = bmp[i][j];
                gdiBmp->SetPixel(j, i, Color::FromArgb(pixel.r, pixel.g, pixel.b));
            }

        delete image;
        pictureBox1->Image = image = gdiBmp;
    }
    catch (std::runtime_error& ex)
    {
        Windows::Forms::MessageBox::Show(this, gcnew String(ex.what()), "Eroare", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }    
}
