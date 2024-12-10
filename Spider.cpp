//
//  Spider.cpp
//  Centripides
//
//  Created by Jorge Lopez Carretero on 05/12/24.
//

#include "Spider.h"

USING_NS_CC;

Spider* Spider::create(const std::string& filename) {
    Spider* spider = new Spider();
    if (spider && spider->initWithFile(filename)) {
        spider->autorelease();
        return spider;
    }
    CC_SAFE_DELETE(spider);
    return nullptr;
}

void Spider::jump(float cellWidth, float cellHeight, int rows, int columns) {
    int randomCol = cocos2d::RandomHelper::random_int(1, columns - 2);
    setPosition(Vec2(randomCol * cellWidth + cellWidth / 2, cellHeight));
}








