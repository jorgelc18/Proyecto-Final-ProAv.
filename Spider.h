//
//  Spider.h
//  Centripides
//
//  Created by Jorge Lopez Carretero on 05/12/24.
//

#ifndef SPIDER_H
#define SPIDER_H

#include "cocos2d.h"

class Spider : public cocos2d::Sprite {
public:
    static Spider* create(const std::string& filename);

    void jump(float cellWidth, float cellHeight, int rows, int columns);
};

#endif // SPIDER_H




