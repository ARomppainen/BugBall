#pragma once

#include "Shape.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Animation.h"
#include "AtlasCoords.h"
#include "AssetFactory.h"

#include <deque>

class Drawable
{
public:

    Drawable() :
        render(true),
        flipX(false),
        flipY(false),
        useAtlas(false),
        material(),
        transform(),
        coords(),
        shape(NULL)
    {
        this->scale = 1.0f;
    }

	virtual ~Drawable()
	{
	}

    virtual void draw(Camera& camera) {

        if (render) {
            if (useAtlas) {
                shape->renderAtlas(camera, transform, material, coords, flipX, flipY);
            } else {
                shape->render(camera, transform, material, flipX, flipY);
            }
        }
    }

    static bool compareDepth(const Drawable *d1, const Drawable *d2)
    {
        return d1->transform.position.z > d2->transform.position.z;
    }

	void update(uint64 deltaTime) {
		if (animations.size() > 0) {
			bool updated = animations.front().update(deltaTime, coords);

			if (animations.front().isOver()) {
				uint64 overflow = animations.front().timeElapsed - animations.front().duration;

				if (animations.size() > 1) {
					animations.pop_front();
					animations.front().update(overflow, coords);
				} else {
					std::string next = animations.front().nextAnimation;
					animations.pop_front();

					if (next.compare("") != 0) {
						setAnimation(next);
						animations.front().update(overflow, coords);
					}
				}
			}

			if (updated) {
                float mysterySize = 128.0f;

                transform.setScale(
                    coords.width / mysterySize * scale,
                    coords.height / mysterySize * scale,
                    1.0f);

                updateOffset();
            }
		}
	}

    virtual void updateOffset()
    {
        // äijä on kohdistettu original width / 2 kohtaan
        // transleittaa:
        // (original width / 2 - offset + width / 2) / mysterySize * skaala???

        float mysterySize = 128.0f;

        float offsetX = (coords.originalWidth / 2 - coords.offsetX - coords.width / 2) / mysterySize * scale;
        float offsetY = (coords.originalHeight / 2 - coords.offsetY - coords.height / 2) / mysterySize * scale;

        if (!flipX) {
            offsetX = -offsetX;
        }

        transform.setOffset(offsetX, offsetY, 0);
    }

	void setAnimation(std::string name) {
		if (animations.size() > 0) {
			animations.clear();
		}

		Animation a(*AssetFactory::getInstance().getAnimation(name));
		animations.push_back(a);
	}

	void setAnimation(std::string name, uint64 length) {
		if (animations.size() > 0) {
			animations.clear();
		}

		Animation a(*AssetFactory::getInstance().getAnimation(name));
		a.duration = length;
		animations.push_back(a);
	}

    void setAnimation(std::string name, uint64 length, uint64 delay)
    {
        if (animations.size() > 0) {
            animations.clear();
        }

        Animation a(*AssetFactory::getInstance().getAnimation(name));
        a.duration = length;
        a.startTime = delay;
        animations.push_back(a);
    }

	void addFollowingAnimation(std::string name) {
		Animation a(*AssetFactory::getInstance().getAnimation(name));
		animations.push_back(a);
	}

	void addFollowingAnimation(std::string name, uint64 length) {
		Animation a(*AssetFactory::getInstance().getAnimation(name));
		a.duration = length;
		animations.push_back(a);
	}

public:

    bool render;

    bool flipX;
    bool flipY;

    bool useAtlas;

    float scale;

    Shape* shape; // AssetFactory owns all shapes
    Material material;
    Transform transform;
    AtlasCoords coords;
	std::deque<Animation> animations;
};