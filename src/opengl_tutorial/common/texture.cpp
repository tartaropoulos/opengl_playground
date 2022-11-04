#include "texture.h"

#include <array>
#include <fstream>
#include <iostream>
#include <tuple>

struct BMPHeader
{
    uint32_t dataPosition;
    uint32_t width;
    uint32_t height;
    uint32_t imageSize;

private:
    enum class BMPHEADER_ADRESSES : int8_t {
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
        std::string BM{2, ' '};
        is.read(&BM[0], 2);

        if (BM != mc_BM)
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        std::array< std::tuple< uint32_t&, BMPHEADER_ADRESSES >, 4 > valuesWithAdresses
        {
            std::make_tuple(std::ref(header.dataPosition), BMPHEADER_ADRESSES::DATA_POSITION),
            std::make_tuple(std::ref(header.width), BMPHEADER_ADRESSES::WIDTH),
            std::make_tuple(std::ref(header.height), BMPHEADER_ADRESSES::HEIGHT),
            std::make_tuple(std::ref(header.imageSize), BMPHEADER_ADRESSES::IMAGE_SIZE)
        };

        for (auto& [value, adress] : valuesWithAdresses)
        {
            is.seekg( static_cast<long long>(adress), is.beg );
            is.read( reinterpret_cast<char*>(&value), 4 );
        }

        is.seekg( static_cast<long long>(BMPHEADER_ADRESSES::HEADER_END) );

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

    std::ifstream textureFile{texturePath, std::ios::binary};

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

    std::vector<std::uint8_t> data(header.imageSize, 0);
    textureFile.read(reinterpret_cast<char*>( data.data() ), header.imageSize);

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
        reinterpret_cast<void*>( data.data() ) // указатель на данные
    );

    // Включение повтора текстуры по координатам s(x) и t(y)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // При увеличении текстуры используется линейная фильтрация
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // При уменьшении текстуры используется смешение двух ближайших мипмап
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Генерация мипмап
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

struct DDSHeader
{
    uint32_t height;
    uint32_t width;
    uint32_t linearSize;
    uint32_t mipMapCount;
    uint32_t fourCC;

private:
    enum class DDSHEADER_ADRESSES : int8_t {
        HEIGHT         = 8,
        WIDTH          = 12,
        LINEAR_SIZE    = 16,
        MIP_MAP_COUNT  = 24,
        FOURCC         = 80,
        HEADER_END     = 124
    };

public:
    friend std::istream& operator>>(std::istream& is, DDSHeader& header)
    {
        std::array< std::tuple< uint32_t&, DDSHEADER_ADRESSES >, 5 > valuesWithAdresses
        {
            std::make_tuple(std::ref(header.height), DDSHEADER_ADRESSES::HEIGHT),
            std::make_tuple(std::ref(header.width), DDSHEADER_ADRESSES::WIDTH),
            std::make_tuple(std::ref(header.linearSize), DDSHEADER_ADRESSES::LINEAR_SIZE),
            std::make_tuple(std::ref(header.mipMapCount), DDSHEADER_ADRESSES::MIP_MAP_COUNT),
            std::make_tuple(std::ref(header.fourCC), DDSHEADER_ADRESSES::FOURCC)
        };

        for (auto& [value, adress] : valuesWithAdresses)
        {
            is.seekg( static_cast<long long>(adress), is.beg );
            is.read( reinterpret_cast<char*>(&value), 4 );
        }

        is.seekg( static_cast<long long>(DDSHEADER_ADRESSES::HEADER_END) );

        return is;
    }
};

enum class FOURCC : uint32_t {
    DXT1 = 0x31545844,
    DXT3 = 0x33545844,
    DXT5 = 0x35545844
};

GLuint loadDDS(std::filesystem::path texturePath)
{
    if ( texturePath.extension() != ".DDS" )
    {
        std::cout << "Wrong extension" << std::endl;
        return 0;
    }

    std::ifstream textureFile{texturePath, std::ios::binary};

    DDSHeader header;
    textureFile >> header;

    uint32_t dataSize{header.mipMapCount > 1 ? header.linearSize * 2 : header.linearSize};
    std::vector data(dataSize, 0);

    textureFile.read(reinterpret_cast<char*>( data.data() ), dataSize);

    textureFile.close();

    uint32_t format;
    switch ( static_cast<FOURCC>(header.fourCC) )
    {
    case FOURCC::DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;

    case FOURCC::DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;

    case FOURCC::DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    
    default:
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    uint32_t blockSize{format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8u : 16u};

    for 
    (
        uint32_t level{0}, offset{0}, width{header.width}, height{header.height};
        level < header.mipMapCount && (width || height);
        ++level
    )
    {
        uint32_t size{ ( (width + 3) / 4 ) * ( (height + 3) / 4 ) * blockSize };
        glCompressedTexImage2D
        (
            GL_TEXTURE_2D, 
            level, 
            format, 
            width, 
            height,
            0, 
            size, 
            reinterpret_cast<void*>( data.data() + offset )
        );

        offset += size;
        width /= 2;
        height /= 2;
    }

    return textureID;
}