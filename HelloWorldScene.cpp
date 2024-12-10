#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "Mushroom.h"
#include "Centripide.h"
#include "GlobalDefinitions.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
    return HelloWorld::create();
}

bool HelloWorld::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    float cellWidth = visibleSize.width / COLUMNAS;
    float cellHeight = visibleSize.height / FILAS;
    auto centipede = Centipede::create(this, 10, cellWidth, cellHeight);
    centipede->setPosition(Vec2(cellWidth, visibleSize.height - cellHeight * 2)); // Posicionar lejos del jugador
    centipedes.push_back(centipede); // Añadir al vector


    // Inicializar variables del juego
    lives = 3;  // Número de vidas inicial
    score = 0;  // Puntaje inicial

    // Dibujar la cuadrícula
    drawGrid(cellWidth, cellHeight, origin);

    // Configurar matriz del tablero
    initMatrix();

    // Crear ciempiés
    centipede = Centipede::create(this, 4, cellWidth, cellHeight);
    centipede->setPosition(Vec2(cellWidth, visibleSize.height - cellHeight * 2)); // Posicionar lejos del jugador

    // Crear jugador (nave) en la penúltima fila desde abajo
    player = Player::create("ship.png");
    player->setScale(cellWidth * 0.8f / player->getContentSize().width);
    player->setPosition(Vec2((COLUMNAS / 2) * cellWidth + cellWidth / 2 + origin.x, cellHeight * 1.5 + origin.y));
    this->addChild(player, 5);

    // Generar hongos
    generateMushrooms();

    // Crear botones de control en la parte inferior
    createButtons(cellWidth, cellHeight, origin);

    // Mostrar puntaje en la pantalla
    scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", 24);
    scoreLabel->setPosition(Vec2(origin.x + 100, origin.y + visibleSize.height - 20));
    this->addChild(scoreLabel, 10);

    // Programar la actualización del nivel
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::update), 0.1f); // Actualización cada 0.1 segundos
    return true;
}

void HelloWorld::drawGrid(float cellWidth, float cellHeight, Vec2 origin) {
    auto drawNode = DrawNode::create();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    for (int i = 0; i <= FILAS; i++) {
        drawNode->drawLine(Vec2(origin.x, origin.y + i * cellHeight),
                           Vec2(origin.x + visibleSize.width, origin.y + i * cellHeight), Color4F::BLUE);
    }

    for (int j = 0; j <= COLUMNAS; j++) {
        drawNode->drawLine(Vec2(origin.x + j * cellWidth, origin.y),
                           Vec2(origin.x + j * cellWidth, origin.y + visibleSize.height), Color4F::BLUE);
    }

    this->addChild(drawNode, 1);
}

void HelloWorld::initMatrix() {
    matrix = std::vector<std::vector<int>>(FILAS, std::vector<int>(COLUMNAS, 0));
}

void HelloWorld::generateMushrooms() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    float cellWidth = visibleSize.width / COLUMNAS;
    float cellHeight = visibleSize.height / FILAS;

    static std::unordered_set   <std::pair<int, int>, pair_hash> previousMushrooms; // Guardar posiciones previas

    for (int i = 1; i < FILAS - 1; ++i) {
        std::vector<int> validColumns;

        for (int j = 1; j < COLUMNAS - 1; ++j) {
            // Asegurar que no haya hongos consecutivos verticalmente ni en posiciones previas
            if (matrix[i - 1][j] != 2 && previousMushrooms.find({i, j}) == previousMushrooms.end()) {
                validColumns.push_back(j);
            }
        }

        // Colocar al menos un hongo por fila si es posible
        if (!validColumns.empty()) {
            int columnIndex = validColumns[rand() % validColumns.size()];
            auto mushroom = Mushroom::create("mushroom.png");
            mushroom->setScale(cellWidth * 0.5f / mushroom->getContentSize().width);
            mushroom->setPosition(Vec2(columnIndex * cellWidth + cellWidth / 2 + origin.x,
                                       i * cellHeight + cellHeight / 2 + origin.y));
            mushroom->setImpactLimit(3);
            this->addChild(mushroom, 5);
            mushrooms.push_back(mushroom);
            matrix[i][columnIndex] = 2;

            // Guardar la posición del hongo recién colocado
            previousMushrooms.insert({i, columnIndex});
        }
    }
}

// Función hash para usar std::pair en unordered_set
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};


void HelloWorld::createButtons(float cellWidth, float cellHeight, Vec2 origin) {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Botón izquierdo
    auto leftButton = ui::Button::create("left_button.png");
    leftButton->setScale(cellWidth / leftButton->getContentSize().width);
    leftButton->setPosition(Vec2(cellWidth * 1.5 + origin.x, cellHeight * 0.5 + origin.y));
    leftButton->addClickEventListener([this](Ref* sender) {
        movePlayerLeft();
    });
    this->addChild(leftButton, 15);

    // Botón derecho
    auto rightButton = ui::Button::create("right_button.png");
    rightButton->setScale(cellWidth / rightButton->getContentSize().width);
    rightButton->setPosition(Vec2(cellWidth * (COLUMNAS - 1.5) + origin.x, cellHeight * 0.5 + origin.y));
    rightButton->addClickEventListener([this](Ref* sender) {
        movePlayerRight();
    });
    this->addChild(rightButton, 15);

    // Botón de disparo
    auto fireButton = ui::Button::create("fire_button.png");
    fireButton->setScale(cellWidth / fireButton->getContentSize().width);
    fireButton->setPosition(Vec2(cellWidth * (COLUMNAS / 2) + origin.x, cellHeight * 0.5 + origin.y));
    fireButton->addClickEventListener([this](Ref* sender) {
        createBullet();
    });
    this->addChild(fireButton, 15);
}

void HelloWorld::update(float delta) {
    std::vector<Centipede*> newCentipedes;

    for (auto it = centipedes.begin(); it != centipedes.end();) {
        if (*it) {
            (*it)->move(delta, matrix, FILAS, COLUMNAS, mushrooms);
            (*it)->checkCollisionWithLaser(bullets, score, matrix, this, newCentipedes);

            // Eliminar ciempiés si no tiene segmentos
            if ((*it)->segments.empty()) {
                delete *it;
                it = centipedes.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Añadir nuevos ciempiés al vector principal
    for (auto& newCentipede : newCentipedes) {
        centipedes.push_back(newCentipede);
    }

    // Actualizar puntaje
    scoreLabel->setString("Score: " + std::to_string(score));
}


void HelloWorld::movePlayerLeft() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cellWidth = visibleSize.width / COLUMNAS;

    // Mover y centrar en la celda
    Vec2 position = player->getPosition();
    player->setPosition(Vec2(position.x - cellWidth, position.y));
}

void HelloWorld::movePlayerRight() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cellWidth = visibleSize.width / COLUMNAS;

    // Mover y centrar en la celda
    Vec2 position = player->getPosition();
    player->setPosition(Vec2(position.x + cellWidth, position.y));
}

void HelloWorld::createBullet() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cellHeight = visibleSize.height / FILAS;

    auto bullet = Sprite::create("bullet.png");
    bullet->setScale(player->getScale() * 0.5f);
    bullet->setPosition(player->getPosition());
    this->addChild(bullet, 6);

    bullets.push_back(bullet);

    auto moveAction = MoveBy::create(1.0f, Vec2(0, visibleSize.height));
    auto removeAction = CallFunc::create([=]() {
        bullet->removeFromParent();
        bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
    });

    auto sequence = Sequence::create(moveAction, removeAction, nullptr);
    bullet->runAction(sequence);
}

