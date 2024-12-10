//
//  Scorpion.cpp
//  Centripides
//
//  Created by Jorge Lopez Carretero on 09/12/24.
//

#include "Scorpion.h"

USING_NS_CC;

Scorpion* Scorpion::create(const std::string& filename) {
    Scorpion* scorpion = new Scorpion();
    if (scorpion && scorpion->initWithFile(filename)) {
        scorpion->autorelease();
        return scorpion;
    }
    CC_SAFE_DELETE(scorpion);
    return nullptr;
}

void Scorpion::moveAcrossScreen(float cellWidth, int rows, float speed) {
    Vec2 startPosition = Vec2(0, rows * cellWidth);
    Vec2 endPosition = Vec2(Director::getInstance()->getVisibleSize().width, rows * cellWidth);

    setPosition(startPosition);

    auto moveAction = MoveTo::create(speed, endPosition);
    auto resetPosition = CallFunc::create([this, startPosition]() {
        this->setPosition(startPosition);
    });

    auto sequence = Sequence::create(moveAction, resetPosition, nullptr);
    runAction(RepeatForever::create(sequence));
}
