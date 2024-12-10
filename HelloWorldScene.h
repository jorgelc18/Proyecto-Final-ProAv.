#ifndef __HELLO_WORLD_SCENE_H__
#define __HELLO_WORLD_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "Mushroom.h"
#include "Centripide.h"
#include <vector>
#include <unordered_set>

class HelloWorld : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // Método para dibujar la cuadrícula
    void drawGrid(float cellWidth, float cellHeight, cocos2d::Vec2 origin);

    // Método para inicializar la matriz del tablero
    void initMatrix();

    // Método para generar hongos
    void generateMushrooms();

    // Método para crear los botones de control
    void createButtons(float cellWidth, float cellHeight, cocos2d::Vec2 origin);

    // Método de actualización del nivel
    void update(float delta) override;

    // Métodos para mover al jugador
    void movePlayerLeft();
    void movePlayerRight();

    // Método para disparar
    void createBullet();

    // Implementación de create() manual
    CREATE_FUNC(HelloWorld);

private:
    Player* player;                          // Jugador (nave)
    Centipede* centipede;                    // Ciempiés
    cocos2d::Label* scoreLabel;              // Etiqueta para el puntaje
    std::vector<std::vector<int>> matrix;    // Matriz del tablero
    std::vector<Mushroom*> mushrooms;        // Lista de hongos
    std::vector<cocos2d::Sprite*> bullets;   // Vector para almacenar balas activas
    std::vector<Centipede*> centipedes;
    int lives;                               // Número de vidas restantes
    int score;                               // Puntaje actual
    float cellWidth;                         // Ancho de una celda de la cuadrícula
    float cellHeight;                        // Altura de una celda de la cuadrícula

    // Estructura para rastrear posiciones previas de hongos
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            auto hash1 = std::hash<T1>{}(pair.first);
            auto hash2 = std::hash<T2>{}(pair.second);
            return hash1 ^ hash2;
        }
    };

    static std::unordered_set<std::pair<int, int>, pair_hash> previousMushrooms;
};

#endif // __HELLO_WORLD_SCENE_H__
