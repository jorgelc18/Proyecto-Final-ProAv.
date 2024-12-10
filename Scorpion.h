//
//  Scorpion.h
//  Centripides
//
//  Created by Jorge Lopez Carretero on 09/12/24.
//

#ifndef SCORPION_H
#define SCORPION_H

#include "cocos2d.h"

class Scorpion : public cocos2d::Sprite {
public:
    static Scorpion* create(const std::string& filename);

    void moveAcrossScreen(float cellWidth, int rows, float speed);

private:
    Scorpion() = default;
    float speed;
};

#endif // SCORPION_H
