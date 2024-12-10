//
//  Player.cpp
//  Centripides
//
//  Created by Jorge Lopez Carretero on 05/12/24.
//
#include "GlobalDefinitions.h"
#include "Player.h"
#include "Mushroom.h"
#include "Centripide.h"

USING_NS_CC;

Player* Player::create(const std::string& filename) {
    auto player = new Player();
    if (player && player->initWithFile(filename)) {
        player->autorelease();
        return player;
    }
    delete player;
    return nullptr;
}

void Player::moveLeft(float distance) {
    auto pos = getPosition();
    setPositionX(std::max(pos.x - distance, 0.0f));
}

void Player::moveRight(float distance, int columns) {
    auto pos = getPosition();
    float maxX = Director::getInstance()->getVisibleSize().width;
    setPositionX(std::min(pos.x + distance, maxX - (maxX / columns)));
}

void Player::checkBulletCollisions(std::vector<Sprite*>& bullets, std::vector<Mushroom*>& mushrooms, std::vector<std::vector<int>>& matrix) {
    for (auto it = bullets.begin(); it != bullets.end();) {
        bool bulletRemoved = false;

        // Iterar sobre los hongos y verificar colisiones
        for (auto mIt = mushrooms.begin(); mIt != mushrooms.end();) {
            if ((*it)->getBoundingBox().intersectsRect((*mIt)->getBoundingBox())) {
                (*mIt)->hit(); // Registrar impacto

                // Si el hongo está destruido, eliminarlo de forma segura
                if ((*mIt)->isDestroyed()) {
                    Vec2 mushroomPos = (*mIt)->getPosition();
                    int row = static_cast<int>(std::floor(mushroomPos.y / (Director::getInstance()->getVisibleSize().height / FILAS)));
                    int col = static_cast<int>(std::floor(mushroomPos.x / (Director::getInstance()->getVisibleSize().width / COLUMNAS)));

                    matrix[row][col] = 0; // Liberar celda
                    (*mIt)->removeFromParent(); // Eliminar del nodo padre
                    mIt = mushrooms.erase(mIt); // Remover del vector
                } else {
                    ++mIt;
                }

                // Remover la bala que impactó
                (*it)->removeFromParent();
                it = bullets.erase(it);
                bulletRemoved = true;
                break; // Salir del bucle de hongos
            } else {
                ++mIt;
            }
        }

        // Si no se eliminó la bala, continuar al siguiente iterador
        if (!bulletRemoved) {
            ++it;
        }
    }
}









