
/*
#include "Level2Scene.h"
#include "GlobalDefinitions.h"
#include "Centripide.h"
#include "Mushroom.h"
#include "Player.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* Level2Scene::createScene() {
    return Level2Scene::create();
}

bool Level2Scene::init() {
    if (!Scene::init()) {
        return false;
    }

    // Configuración de la escena
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Inicializar matriz y elementos
    initMatrix();
    generateMushrooms();
    generateSpider();

    // Configuración inicial de puntaje y vidas
    score = 0;
    lives = 3;

    // Mostrar puntaje
    scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", 24);
    scoreLabel->setPosition(Vec2(origin.x + 100, origin.y + visibleSize.height - 20));
    this->addChild(scoreLabel, 10);

    // Crear jugador
    player = Player::create("ship.png");
    float cellWidth = visibleSize.width / COLUMNAS;
    float cellHeight = visibleSize.height / FILAS;
    player->setScale((cellWidth * 4) / player->getContentSize().width);
    player->setPosition(Vec2((COLUMNAS / 2) * cellWidth + cellWidth / 2, cellHeight * (FILAS - 2)));
    this->addChild(player, 5);

    // Generar ciempiés más rápido
    centipede = Centipede::create(this, 4, cellWidth, cellHeight);
    centipede->setSpeedMultiplier(1.5f); // Aumentar la velocidad del ciempiés en un 50%

    // Crear botones de movimiento
    float buttonSize = cellWidth * 5.0;

    auto leftButton = ui::Button::create("button.png", "button_selected.png");
    leftButton->setScale(buttonSize / leftButton->getContentSize().width);
    leftButton->setPosition(Vec2(cellWidth * 2, cellHeight * 2));
    leftButton->addClickEventListener([this](Ref* sender) {
        movePlayerLeft();
    });
    this->addChild(leftButton, 15);

    auto rightButton = ui::Button::create("button.png", "button_selected.png");
    rightButton->setScale(buttonSize / rightButton->getContentSize().width);
    rightButton->setPosition(Vec2(cellWidth * 8, cellHeight * 2));
    rightButton->addClickEventListener([this](Ref* sender) {
        movePlayerRight();
    });
    this->addChild(rightButton, 15);

    auto fireButton = ui::Button::create("button.png", "button_selected.png");
    fireButton->setScale(buttonSize / fireButton->getContentSize().width);
    fireButton->setPosition(Vec2(cellWidth * 15, cellHeight * 2));
    fireButton->addClickEventListener([this](Ref* sender) {
        createBullet();
    });
    this->addChild(fireButton, 15);

    // Programar el método update
    this->schedule(CC_SCHEDULE_SELECTOR(Level2Scene::update), 0.02f);

    return true;
}

void Level2Scene::initMatrix() {
    matrix = std::vector<std::vector<int>>(FILAS, std::vector<int>(COLUMNAS, 1));
    for (int i = 0; i < FILAS; ++i) {
        matrix[i][0] = 0;
        matrix[i][COLUMNAS - 1] = 0;
    }
    for (int j = 0; j < COLUMNAS; ++j) {
        matrix[0][j] = 0;
        matrix[FILAS - 1][j] = 0;
    }
}

void Level2Scene::generateMushrooms() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cellWidth = visibleSize.width / COLUMNAS;
    float cellHeight = visibleSize.height / FILAS;

    for (int i = 1; i < FILAS - 2; ++i) {
        for (int j = 1; j < COLUMNAS - 1; ++j) {
            if (rand() % 100 < 36) { // 36% probabilidad de generar hongos
                auto mushroom = Mushroom::create("mushroom.png");
                mushroom->setImpactLimit(5); // Hongos requieren 5 impactos para destruirse
                float scaleFactor = (cellWidth * 0.5) / mushroom->getContentSize().width;
                mushroom->setScale(scaleFactor);
                mushroom->setPosition(Vec2(j * cellWidth + cellWidth / 2, i * cellHeight + cellHeight / 2));
                this->addChild(mushroom, 5);
                mushrooms.push_back(mushroom);
                matrix[i][j] = 2;
            }
        }
    }
}

void Level2Scene::generateSpider() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cellWidth = visibleSize.width / COLUMNAS;
    float cellHeight = visibleSize.height / FILAS;

    spider = Sprite::create("spider.png");
    float scaleFactor = (cellWidth * 0.7) / spider->getContentSize().width;
    spider->setScale(scaleFactor);
    spider->setPosition(Vec2((rand() % COLUMNAS) * cellWidth + cellWidth / 2, cellHeight * (FILAS - 1)));
    this->addChild(spider, 6);

    auto jumpAction = MoveBy::create(0.5f, Vec2(0, -cellHeight));
    auto resetPosition = CallFunc::create([=]() {
        spider->setPositionX(rand() % COLUMNAS * cellWidth + cellWidth / 2);
        spider->setPositionY(cellHeight * (FILAS - 1));
    });

    auto sequence = Sequence::create(jumpAction, resetPosition, nullptr);
    spider->runAction(RepeatForever::create(sequence));
}

void Level2Scene::movePlayerLeft() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cellWidth = visibleSize.width / COLUMNAS;
    player->moveLeft(cellWidth);
}

void Level2Scene::movePlayerRight() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cellWidth = visibleSize.width / COLUMNAS;
    player->moveRight(cellWidth, COLUMNAS);
}

void Level2Scene::createBullet() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cellHeight = visibleSize.height / FILAS;
    auto bullet = Sprite::create("bullet.png");
    bullet->setScale(player->getScale() * 0.5);
    bullet->setPosition(player->getPosition());
    this->addChild(bullet, 6);

    auto moveAction = MoveBy::create(1.0, Vec2(0, cellHeight * (FILAS - 1)));
    auto checkCollision = CallFunc::create([=]() {
        for (size_t i = 0; i < mushrooms.size(); ++i) {
            if (bullet->getBoundingBox().intersectsRect(mushrooms[i]->getBoundingBox())) {
                mushrooms[i]->hit();
                if (mushrooms[i]->isDestroyed()) {
                    mushrooms[i]->removeFromParent();
                    mushrooms.erase(mushrooms.begin() + i);
                }
                bullet->removeFromParent();
                return;
            }
        }
    });

    auto sequence = Sequence::create(moveAction, checkCollision, nullptr);
    bullet->runAction(sequence);
}

void Level2Scene::update(float delta) {
    centipede->move(delta, matrix, FILAS, COLUMNAS);
}
*/
