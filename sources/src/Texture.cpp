#include "Texture.h"

// http://developer.roolez.com/marmaladesdk/api/api/ExampleIwGLExt.html

Texture::Texture(const char* fileName, int format)
{
    // load image from file
    Image image;
    image.LoadFromFile(fileName);

    width  = image.GetWidth();
    height = image.GetHeight();

    // this caused some weird color changes

    // convert to OpenGL ES native format
    //Image nativeimage;
    //nativeimage.SetFormat(Image::ARGB_8888);
    //image.ConvertToImage(&nativeimage);

    // generate texture object
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // disable mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // upload
    glTexImage2D(GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        image.GetTexels());

    // unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

GLuint Texture::getId() const
{
    return id;
}

GLsizei Texture::getWidth() const
{
    return width;
}

GLsizei Texture::getHeight() const
{
    return height;
}