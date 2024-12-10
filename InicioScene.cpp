/*
#include "InicioScene.h"
#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* InicioScene::createScene() {
    return InicioScene::create();
}

bool InicioScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Título adaptado
    auto title = Label::createWithSystemFont("Menu Principal", "Arial", visibleSize.height * 0.05f);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - title->getContentSize().height * 1.5f));
    this->addChild(title, 1);

    // Tamaño de los botones relativo a la pantalla
    float buttonWidth = visibleSize.width * 0.3f;
    float buttonHeight = visibleSize.height * 0.1f;
    float buttonFontSize = visibleSize.height * 0.03f;

    // Botón Iniciar
    auto startButton = ui::Button::create("button.png");
    startButton->setScale9Enabled(true);
    startButton->setContentSize(Size(buttonWidth, buttonHeight));
    startButton->setTitleText("Iniciar");
    startButton->setTitleFontSize(buttonFontSize);
    startButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + buttonHeight * 1.5f));
    startButton->addClickEventListener(CC_CALLBACK_1(InicioScene::goToHelloWorldScene, this));
    this->addChild(startButton);

    // Botón Niveles
    auto levelsButton = ui::Button::create("button.png", "button_selected.png");
    levelsButton->setScale9Enabled(true);
    levelsButton->setContentSize(Size(buttonWidth, buttonHeight));
    levelsButton->setTitleText("Niveles");
    levelsButton->setTitleFontSize(buttonFontSize);
    levelsButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    levelsButton->addClickEventListener(CC_CALLBACK_1(InicioScene::showLevelsMenu, this));
    this->addChild(levelsButton);

    // Botón Ajustes
    auto settingsButton = ui::Button::create("button.png", "button_selected.png");
    settingsButton->setScale9Enabled(true);
    settingsButton->setContentSize(Size(buttonWidth, buttonHeight));
    settingsButton->setTitleText("Ajustes");
    settingsButton->setTitleFontSize(buttonFontSize);
    settingsButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - buttonHeight * 1.5f));
    settingsButton->addClickEventListener(CC_CALLBACK_1(InicioScene::showSettingsMenu, this));
    this->addChild(settingsButton);

    return true;
}

void InicioScene::goToHelloWorldScene(Ref* sender) {
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void InicioScene::showLevelsMenu(Ref* sender) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto levelsMenu = Label::createWithSystemFont("Nivel 1\nNivel 2\nNivel 3", "Arial", visibleSize.height * 0.03f);
    levelsMenu->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(levelsMenu, 2);

    auto closeButton = ui::Button::create("button.png", "button_selected.png");
    closeButton->setScale9Enabled(true);
    closeButton->setContentSize(Size(visibleSize.width * 0.2f, visibleSize.height * 0.07f));
    closeButton->setTitleText("Cerrar");
    closeButton->setTitleFontSize(visibleSize.height * 0.025f);
    closeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - levelsMenu->getContentSize().height));
    closeButton->addClickEventListener([=](Ref* sender) {
        levelsMenu->removeFromParent();
        closeButton->removeFromParent();
    });
    this->addChild(closeButton, 2);
}

void InicioScene::showSettingsMenu(Ref* sender) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Fondo para ajustes
    auto settingsBackground = LayerColor::create(Color4B(0, 0, 0, 200));
    this->addChild(settingsBackground, 3);

    // Ajustes de Volumen
    auto volumeLabel = Label::createWithSystemFont("Volumen", "Arial", visibleSize.height * 0.03f);
    volumeLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 - visibleSize.width * 0.1f, visibleSize.height / 2 + visibleSize.height * 0.05f));
    settingsBackground->addChild(volumeLabel);

    auto volumeSlider = ui::Slider::create();
    volumeSlider->loadBarTexture("slideTrack.png");
    volumeSlider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png");
    volumeSlider->loadProgressBarTexture("sliderProgress.png");
    volumeSlider->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width * 0.1f, visibleSize.height / 2 + visibleSize.height * 0.05f));
    volumeSlider->setPercent(50);
    settingsBackground->addChild(volumeSlider);

    // Ajustes de Tamaño de Botones
    auto buttonSizeLabel = Label::createWithSystemFont("Tamaño de Botones", "Arial", visibleSize.height * 0.03f);
    buttonSizeLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 - visibleSize.width * 0.1f, visibleSize.height / 2));
    settingsBackground->addChild(buttonSizeLabel);

    auto buttonSizeSlider = ui::Slider::create();
    buttonSizeSlider->loadBarTexture("slideTrack.png");
    buttonSizeSlider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png");
    buttonSizeSlider->loadProgressBarTexture("sliderProgress.png");
    buttonSizeSlider->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width * 0.1f, visibleSize.height / 2));
    buttonSizeSlider->setPercent(50);
    settingsBackground->addChild(buttonSizeSlider);

    auto closeButton = ui::Button::create("button.png", "button_selected.png");
    closeButton->setScale9Enabled(true);
    closeButton->setContentSize(Size(visibleSize.width * 0.2f, visibleSize.height * 0.07f));
    closeButton->setTitleText("Cerrar");
    closeButton->setTitleFontSize(visibleSize.height * 0.025f);
    closeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - visibleSize.height * 0.1f));
    closeButton->addClickEventListener([=](Ref* sender) {
        settingsBackground->removeFromParent();
    });
    settingsBackground->addChild(closeButton);
}
*/
