#ifndef __CENTIPIDE_H__
#define __CENTIPIDE_H__

#include "cocos2d.h"
#include "Mushroom.h"
#include <vector>

class Centipede {
public:
    // Crea un nuevo ciempiés
    static Centipede* create(cocos2d::Node* parent, int segmentCount, float cellWidth, float cellHeight);

    // Posiciona el ciempiés en el tablero
    void setPosition(const cocos2d::Vec2& position);

    // Mueve el ciempiés en el tablero
    void move(float delta, std::vector<std::vector<int>>& matrix, int rows, int cols, std::vector<Mushroom*>& mushrooms);

    // Verifica colisiones del láser con el ciempiés
    void checkCollisionWithLaser(std::vector<cocos2d::Sprite*>& bullets, int& score, std::vector<std::vector<int>>& matrix, cocos2d::Node* parent, std::vector<Centipede*>& centipedes);

    // Segmentos que componen el ciempiés
    std::vector<cocos2d::Sprite*> segments;

private:
    // Divide el ciempiés en dos si es impactado por un láser
    void splitCentipede(int segmentIndex, cocos2d::Node* parent, std::vector<Centipede*>& centipedes);

    // Dimensiones de las celdas en el tablero
    float cellWidth;
    float cellHeight;

    // Dirección de movimiento del ciempiés
    int direction;
};

#endif // __CENTIPIDE_H__
