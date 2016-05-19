#include "Ship.h"

#include "AssetFactory.h"
#include "Util.h"

Ship::Ship(vec3 pos) :
    position(pos.x - 0.5f, pos.y + 0.5f, pos.z)
{
    this->width = 2;
    this->height = 4;

    for (int i = 0; i < width; ++i) {
        Drawable tile;
        tile.shape = AssetFactory::getInstance().getShape("SPRITE");
        tile.material.texture = AssetFactory::getInstance().getTexture("BOAT_WATER");
        tile.useAtlas = true;
        tile.coords.width = 128;
        tile.coords.height = 128;
        tile.coords.x = 0;
        tile.coords.y = 128;
        tile.transform.setPosition(pos.x + i, pos.y, pos.z);
        tiles.push_back(tile);
    }

    for (int i = 1; i <= height - 2; ++i) {
        for (int j = 0; j < width; ++j) {
            Drawable tile;
            tile.shape = AssetFactory::getInstance().getShape("SPRITE");
            tile.material.texture = AssetFactory::getInstance().getTexture("BOAT_WATER");
            tile.useAtlas = true;
            tile.coords.width = 128;
            tile.coords.height = 128;
            tile.coords.x = 0;
            tile.coords.y = 0;
            tile.transform.setPosition(pos.x + j, pos.y - i, pos.z);
            tiles.push_back(tile);
        }
    }

    for (int i = 0; i < width; ++i) {
        Drawable tile;
        tile.shape = AssetFactory::getInstance().getShape("SPRITE");
        tile.material.texture = AssetFactory::getInstance().getTexture("BOAT_WATER");
        tile.useAtlas = true;
        tile.coords.width = 128;
        tile.coords.height = 128;
        tile.coords.x = 128;
        tile.coords.y = 0;
        tile.transform.setPosition(pos.x + i, pos.y - (height - 1), pos.z);
        tiles.push_back(tile);
    }
}

Ship::~Ship()
{

}

void Ship::draw(Camera& c)
{
    for (unsigned i = 0; i < tiles.size(); ++i) {
        tiles[i].draw(c);
    }
}

int Ship::pointIsInside(vec2 point)
{
    int ret = -1;

    for (unsigned i = 0; i < tiles.size(); ++i) {
        if (Util::pointInsideSprite(point, tiles[i].transform)) {
            ret = i;
            break;
        }
    }

    return ret;
}

Drawable& Ship::getSprite(int index)
{
    return tiles[index];
}