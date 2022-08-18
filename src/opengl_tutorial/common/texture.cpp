#include "texture.h"

#include <array>
#include <fstream>
#include <iostream>
#include <tuple>

struct BMPHeader
{
    long long dataPosition;
    long long width;
    long long height;
    long long imageSize;

private:
    enum class BMPHEADER_ADRESSES : int {
        DATA_POSITION  = 0x0A,
        WIDTH          = 0x12,
        HEIGHT         = 0x16,
        IMAGE_SIZE     = 0x22,
        HEADER_END     = 0x36
    };

    static const std::string mc_BM;

public:

    friend std::istream& operator>>(std::istream& is, BMPHeader& header)
    {
        std::string BM;
        is.read(&BM[0], 2);

        if (BM != mc_BM)
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        std::array< std::tuple< long long&, BMPHEADER_ADRESSES >, 4 > valuesWithAdresses
        {
            std::make_tuple(std::ref(header.dataPosition), BMPHEADER_ADRESSES::DATA_POSITION),
            std::make_tuple(std::ref(header.width), BMPHEADER_ADRESSES::WIDTH),
            std::make_tuple(std::ref(header.height), BMPHEADER_ADRESSES::HEIGHT),
            std::make_tuple(std::ref(header.imageSize), BMPHEADER_ADRESSES::IMAGE_SIZE)
        };

        for (auto& [value, adress] : valuesWithAdresses)
        {
            is.seekg( static_cast<int>(adress) );
            is >> value;
        }

        is.seekg( static_cast<int>(BMPHEADER_ADRESSES::HEADER_END) );

        return is;
    }
};

const std::string BMPHeader::mc_BM{"BM"};

GLuint loadBMP_custom(std::filesystem::path texturePath)
{
    if ( texturePath.extension() != ".bmp" )
    {
        std::cout << "Wrong extension" << std::endl;
        return 0;
    }

    std::ifstream textureFile{texturePath};

    BMPHeader header;
    textureFile >> header;

    if ( textureFile.fail() )
    {
        std::cout << "Error reading bmp file" << std::endl;
        return 0;
    }

    // Некоторые bmp файлы могут быть неверно отформатированы (написано в оригинале)
    if ( header.imageSize == 0 )
    {
        header.imageSize = header.width * header.height * 3;
    }

    if ( header.dataPosition == 0 )
    {
        header.dataPosition = 54;
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
        GL_RGB, // количество цветовых компонентов в текстуре
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