#pragma once

#include "Shape.h"
#include "Texture.h"
#include "Program.h"
#include "Animation.h"
#include "tinyxml2.h"

#include <iostream>
#include <map>

/*
const std::string GRASS_HEX         = "TEX_GRASS_HEX";
const std::string SAND_HEX          = "TEX_SAND_HEX";
const std::string TILES_GRASS       = "TEX_TILES_GRASS";
const std::string UI_SELECT_TACKLE  = "TEX_UI_SELECT_TACKLE";

const std::string SIMPLE = "PROG_SIMPLE";
const std::string ATLAS = "PROG_ATLAS";

const std::string SPRITE = "SHAPE_SPRITE";
*/

class AssetFactory
{
public:

	~AssetFactory();

	static AssetFactory& getInstance();

    void init();

    GLuint getActiveProgram() const;
    GLuint getActiveTexture() const;

    void setActiveProgram(GLuint program);
    void setActiveTexture(GLuint texture);

    Shape* getShape(const std::string& name);
    Texture* getTexture(const std::string& name);
    Program* getProgram(const std::string& name);
    Animation *getAnimation(const std::string& animation);

private:
	AssetFactory();

	static AssetFactory* instance;

    GLuint activeProgram;
    GLuint activeTexture;

    void loadShapes();
    void loadTextures();
    void loadPrograms();
    void loadAnimations();
    void loadAnimations(const char* xmlName);

	std::map<std::string, Shape*> shapes;
    std::map<std::string, Texture*> textures;
    std::map<std::string, Program*> programs;
    std::map<std::string, Animation*> animations;
};