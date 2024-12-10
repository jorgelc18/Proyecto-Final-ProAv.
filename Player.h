//
//  Player.h
//  Centripides
//
//  Created by Jorge Lopez Carretero on 05/12/24.
//

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Mushroom.h"
#include <vector>

class Player : public cocos2d::Sprite {
public:
    static Player* create(const std::string& filename);

    // Función para mover el jugador hacia la izquierda
    void moveLeft(float distance);

    // Función para mover el jugador hacia la derecha
    void moveRight(float distance, int columns);

    // Función para verificar colisiones entre balas y hongos
    void checkBulletCollisions(std::vector<cocos2d::Sprite*>& bullets,
                               std::vector<Mushroom*>& mushrooms,
                               std::vector<std::vector<int>>& matrix);
};

#endif // __PLAYER_H__




