//
//  Level2Scene.h
//  Centripides
//
//  Created by Jorge Lopez Carretero on 08/12/24.
//

#ifndef __LEVEL2_SCENE_H__
#define __LEVEL2_SCENE_H__

#include "cocos2d.h"
#include "Mushroom.h"
#include "Centripide.h"
#include "Player.h"
#include <vector>

class Level2Scene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // Declaración de funciones
    void initMatrix();
    void generateMushrooms();
    void generateSpider();
    void movePlayerLeft();
    void movePlayerRight();
    void createBullet();
    void update(float delta);

    CREATE_FUNC(Level2Scene);

private:
    // Variables miembro
    cocos2d::Label* scoreLabel;
    int score;
    int lives;

    cocos2d::Sprite* spider;
    Player* player;
    Centipede* centipede;
    std::vector<Mushroom*> mushrooms;
    std::vector<std::vector<int>> matrix;
};

#endif // __LEVEL2_SCENE_H__
