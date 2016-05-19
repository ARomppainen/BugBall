#include "Material.h"

Material::Material()
    : texture(0),
    color(0)
{
}

Material::~Material()
{
    // AssetFactory owns and handles texture creation / deletion
    /*
    if (texture) {
        delete texture;
        texture = 0;
    }
    */

    if (color) {
        delete color;
        color = 0;
    }
}

Material::Material(const Material& other)
    : texture(0),
    color(0)
{
    if (other.texture) {
        // AssetFactory owns all textures, just copy pointer
        texture = other.texture;
    }

    if (other.color) {
        color = new vec3(
            other.color->x,
            other.color->y,
            other.color->z);
    }
}

void Material::setColor(vec3 color)
{
    if (this->color) {
        delete this->color;
    }

    this->color = new vec3(color);
}

Material& Material::operator=(const Material& other)
{
    if (this == &other)
        return *this;

    if (other.texture) {
        this->texture = other.texture;
    } else {
        this->texture = 0;
    }

    if (this->color) {
        delete this->color;
    }

    if (other.color) {
        this->color = new vec3(*other.color);
    } else {
        this->color = 0;
    }

    return *this;
}