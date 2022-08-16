#include "texture.h"

#include <fstream>

#include <GL/glew.h>

struct BMPHeader
{
    std::uint32_t dataPosition;
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t imageSize;

    friend std::istream& operator>>(std::istream& is, BMPHeader& header)
    {
        std::string BM;
        is.get(BM, 2);

        if (BM != mc_BM)
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        std::array< std::tuple< std::uint32_t&, BMPHEADER_ADRESSES > > valuesWithAdresses
        {
            std::make_tuple(header.dataPosition, BMPHEADER_ADRESSES::DATA_POSITION),
            std::make_tuple(header.width, BMPHEADER_ADRESSES::WIDTH),
            std::make_tuple(header.height, BMPHEADER_ADRESSES::HEIGHT),
            std::make_tuple(header.imageSize, BMPHEADER_ADRESSES::IMAGE_SIZE)
        }

        for (auto [auto& value, auto adress] : valuesWithAdresses)
        {
            is.seekg(adress);
            is >> value;
        }

        is.seekg(BMPHEADER_ADRESSES::HEADER_END);

        return is;
    }

private:
    enum class BMPHEADER_ADRESSES{
        DATA_POSITION  = 0x0A,
        WIDTH          = 0x12,
        HEIGHT         = 0x16,
        IMAGE_SIZE     = 0x22,
        HEADER_END     = 0x36
    }

    static const std::string mc_BM{"BM"};
};

GLuint loadBMP_custom(std::filesystem::path texturePath)
{
    std::ifstream textureFile{texturePath};

    BMPHeader header;
    textureFile >> header;

    if ( textureFile.fail() )
    {
        return 0;
    }

    std::istream_iterator<unsigned char> begin{textureFile};
    std::istream_iterator<unsigned char> end
    {
        textureFile.seekg
        (
            textureFile.tellg() + header.imageSize
        )
    };

    std::vector data(begin, end);

    textureFile.close();

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    // Отправка изображения в OpenGL
    glTexImage2D
    (
        GL_TEXTURE_2D, // цель
        0, // количевство LODов, 0 просто изображение
        GL_RGB, // 
        header.width, // ширина изображения
        header.height, // высота изображения
        0, // всегда 0 (https://docs.gl/gl4/glTexImage2D)
        GL_BGR, // формат данных пикселя
        GL_UNSIGNED_BYTE, // тип данных пикселя
        data.data() // указатель на данные
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return textureID;
}