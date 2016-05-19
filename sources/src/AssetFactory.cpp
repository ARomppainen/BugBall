#include "AssetFactory.h"
#include "tinyxml2.h"
#include "IwGL.h"

AssetFactory* AssetFactory::instance = 0;

AssetFactory::AssetFactory() :
    shapes(),
    textures()
{
    activeProgram = -1;
    activeTexture = -1;
}

AssetFactory::~AssetFactory()
{

}

void AssetFactory::init()
{
    loadPrograms();
    loadTextures();
    loadShapes();
    loadAnimations();
}

GLuint AssetFactory::getActiveProgram() const
{
    return activeProgram;
}

GLuint AssetFactory::getActiveTexture() const
{
    return activeTexture;
}

void AssetFactory::setActiveProgram(GLuint program)
{
    this->activeProgram = program;
}

void AssetFactory::setActiveTexture(GLuint texture)
{
    this->activeTexture = texture;
}

Shape* AssetFactory::getShape(const std::string& name)
{
    return shapes[name];
}

Texture* AssetFactory::getTexture(const std::string& name)
{
    return textures[name];
}

Program* AssetFactory::getProgram(const std::string& name)
{
    return programs[name];
}

Animation* AssetFactory::getAnimation(const std::string& name)
{
    return animations[name];
}

void AssetFactory::loadShapes()
{
    shapes["SPRITE"] = new Sprite();
}

void AssetFactory::loadTextures()
{
    textures["TILES_GRASS"] = new Texture("./textures/hex_tiles_128_grassland.png", GL_RGBA);
    textures["BALL"] = new Texture("./textures/BallBug.png", GL_RGBA);
    textures["BOAT_WATER"] = new Texture("./textures/Boat_Water_128.png", GL_RGBA);
    textures["UI_FLAG"] = new Texture("./textures/UI_Flag_64.png", GL_RGBA);
    textures["MENU_BUTTONS"] = new Texture("./textures/TheMenu.png", GL_RGBA);
	textures["UI_HEXATLAS"] = new Texture("./textures/UI_hexAtlas02.png", GL_RGBA);
    //textures["WATER"] = new Texture("./textures/asdf.png", GL_RGBA);
    //textures["WATER"] = new Texture("./textures/Uudempi_seatile.png", GL_PALETTE8_RGBA8_OES);
    //textures["WATER"] = new Texture("./textures/One_More_Water.png", GL_RGBA);
    textures["WATER"] = new Texture("./textures/Water.png", GL_RGBA);
	textures["ISLE_ELVES"] = new Texture("./textures/IsleElves_15p.png", GL_RGBA);
}

void AssetFactory::loadPrograms()
{
    programs["SIMPLE"] = new Program("shaders/vs_simple", "shaders/fs_simple");
    programs["ATLAS"] = new Program("shaders/vs_atlas", "shaders/fs_atlas");
    //programs["BATCH"] = new Program("shaders/vs_batch", "shaders/fs_batch");
}

void AssetFactory::loadAnimations()
{
	loadAnimations("animations/IsleElves_15p.xml");
	loadAnimations("animations/BallBug.xml");
	//loadAnimations("animations/UI_hexAttlas01")
}

void AssetFactory::loadAnimations(const char* xmlName)
{
	tinyxml2::XMLDocument doc;

	if(doc.LoadFile(xmlName) == tinyxml2::XML_SUCCESS)
	{
        tinyxml2::XMLNode* root = doc.FirstChildElement("TextureAtlas");

		//std::cout << xmlName << " file loaded" << std::endl;
		for(tinyxml2::XMLElement* e = root->FirstChildElement("animation"); e ; e = e->NextSiblingElement()){
			//Animation variables
            Animation* a = new Animation();
            a->loop = true; // TODO: kaikki animaatiot looppaa nyt =)

            a->name = e->Attribute("n");
            a->nextAnimation = e->Attribute("f");

            a->startTime = e->IntAttribute("i");
            a->animationLength = e->IntAttribute("d");
            a->duration = a->animationLength;

            int loopType = e->IntAttribute("l");

            if (loopType == 1) {
                a->reverse = true;
            } else if (loopType == 3) {
                a->loop = false;
            }

            //std::cout << "Animation information:" << std::endl;
            //std::cout  << a->name << std::endl << a->loopType << std::endl << a->startTime << std::endl;
            //std::cout << a->animationLength << std::endl << a->nextAnimation << std::endl;

            for (tinyxml2::XMLElement* e2 = e->FirstChildElement("sprite"); e2 ; e2 = e2->NextSiblingElement()) {
                AtlasCoords ac;

                //Sprite variables
                ac.x = e2->IntAttribute("x");
                ac.y = e2->IntAttribute("y");
                ac.width = e2->IntAttribute("w");
                ac.height = e2->IntAttribute("h");
                ac.offsetX = e2->IntAttribute("oX");
                ac.offsetY = e2->IntAttribute("oY");
                ac.originalWidth = e2->IntAttribute("oW");
                ac.originalHeight = e2->IntAttribute("oH");

                a->frames.push_back(ac);
            }

            if (loopType == 2) { // pingpong
                a->loop = true;

                for (int i = a->frames.size() - 2; i >= 1; --i) {
                    a->frames.push_back(a->frames[i]);
                }
            }

			animations[a->name] = a;
		}
	}
}

AssetFactory& AssetFactory::getInstance()
{
	if ( !instance ) {
		instance = new AssetFactory();
	}

	return *instance;
}