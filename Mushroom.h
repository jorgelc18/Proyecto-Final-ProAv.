// Mushroom.h
#ifndef __MUSHROOM_H__
#define __MUSHROOM_H__

#include "cocos2d.h"

class Mushroom : public cocos2d::Sprite {
private:
    int impactCount;
    int impactLimit;

public:
    static Mushroom* create(const std::string& filename);
    void setImpactLimit(int limit);
    void hit();
    bool isDestroyed() const;
};

#endif

