#include "Mushroom.h"

USING_NS_CC;

Mushroom* Mushroom::create(const std::string& filename) {
    Mushroom* mushroom = new (std::nothrow) Mushroom();
    if (mushroom && mushroom->initWithFile(filename)) {
        mushroom->autorelease();
        mushroom->impactCount = 0;  // Inicializar contador de impactos
        return mushroom;
    }
    CC_SAFE_DELETE(mushroom);
    return nullptr;
}

void Mushroom::setImpactLimit(int limit) {
    impactLimit = limit;
}

void Mushroom::hit() {
    impactCount++;
    CCLOG("Hongo impactado. Impactos: %d de %d", impactCount, impactLimit);
    if (isDestroyed()) {
        CCLOG("Hongo destruido.");
    }
}

bool Mushroom::isDestroyed() const {
    return impactCount >= impactLimit;
}
