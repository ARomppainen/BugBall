#pragma once

#include "IwGL.h"

#include "Camera.h"
#include "Transform.h"
#include "Material.h"
#include "Program.h"
#include "AtlasCoords.h"

class Shape
{
public:
	Shape();
	~Shape();

	virtual void render(
        Camera& c,
        Transform& t,
        Material& m,
        bool swapX,
        bool swapY) = 0;

    virtual void renderAtlas(
        Camera& c,
        Transform& t,
        Material& m,
        AtlasCoords& a,
        bool swapX,
        bool swapY) = 0;

protected:

	GLenum drawType;
	GLint drawStart;
	GLint drawCount;
};

class Sprite : public Shape
{
public:
	Sprite();
	~Sprite();

    void render(
        Camera& c,
        Transform& t,
        Material& m,
        bool swapX,
        bool swapY);

    void renderAtlas(
        Camera& c,
        Transform& t,
        Material& m,
        AtlasCoords& a,
        bool swapX,
        bool swapY);
};