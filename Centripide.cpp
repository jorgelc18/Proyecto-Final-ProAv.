#include "Centripide.h"
#include "Mushroom.h" 

USING_NS_CC;

Centipede* Centipede::create(Node* parent, int segmentCount, float cellWidth, float cellHeight) {
    auto centipede = new Centipede();
    centipede->cellWidth = cellWidth;
    centipede->cellHeight = cellHeight;
    centipede->direction = 1;

    for (int i = 0; i < segmentCount; ++i) {
        auto segment = Sprite::create("CENTRIPIDE2.png");
        float scaleFactor = (cellWidth * 0.5f) / segment->getContentSize().width;
        segment->setScale(scaleFactor);
        parent->addChild(segment, 5);
        centipede->segments.push_back(segment);
    }

    return centipede;
}

void Centipede::setPosition(const Vec2& position) {
    for (size_t i = 0; i < segments.size(); ++i) {
        float xOffset = i * cellWidth/3; // Espaciar segmentos horizontalmente
        float yOffset = cellHeight / 2; // Centrar verticalmente
        segments[i]->setPosition(Vec2(position.x + xOffset, position.y + yOffset));
    }
}

void Centipede::move(float delta, std::vector<std::vector<int>>& matrix, int rows, int cols, std::vector<Mushroom*>& mushrooms) {
    if (segments.empty()) return; // No mover si no hay segmentos

    static float horizontalTimeAccumulator = 0.0f;
    horizontalTimeAccumulator += delta;

    if (horizontalTimeAccumulator < 0.8f) return;
    horizontalTimeAccumulator = 0.0f;

    Vec2 headPos = segments[0]->getPosition();
    int headRow = static_cast<int>(std::floor(headPos.y / cellHeight));
    int headCol = static_cast<int>(std::floor(headPos.x / cellWidth));
    int nextHeadCol = headCol + direction;

    // Cambiar dirección si llega al borde o encuentra un obstáculo
    if (nextHeadCol < 0 || nextHeadCol >= cols || (matrix[headRow][nextHeadCol] == 2)) {
        direction *= -1;

        // Intentar bajar una fila si hay un obstáculo
        for (auto& segment : segments) {
            float newY = segment->getPositionY() - cellHeight;
            segment->setPositionY(std::floor(newY / cellHeight) * cellHeight + cellHeight / 2);
        }
        return;
    }

    // Mover los segmentos horizontalmente
    for (auto& segment : segments) {
        float newX = segment->getPositionX() + direction * cellWidth;
        segment->setPositionX(std::floor(newX / cellWidth) * cellWidth + cellWidth / 2);
    }
}   


void Centipede::checkCollisionWithLaser(std::vector<Sprite*>& bullets, int& score, std::vector<std::vector<int>>& matrix, Node* parent, std::vector<Centipede*>& centipedes) {
    for (size_t i = 0; i < segments.size();) {
        bool segmentHit = false;

        for (auto it = bullets.begin(); it != bullets.end();) {
            if ((*it)->getBoundingBox().intersectsRect(segments[i]->getBoundingBox())) {
                auto hitSegment = segments[i];
                Vec2 pos = hitSegment->getPosition();
                int row = static_cast<int>(std::floor(pos.y / cellHeight));
                int col = static_cast<int>(std::floor(pos.x / cellWidth));

                matrix[row][col] = 0; // Liberar la celda ocupada por el segmento
                hitSegment->removeFromParent();
                segments.erase(segments.begin() + i);

                // Dividir ciempiés si tiene más de un segmento
                if (!segments.empty()) {
                    splitCentipede(i, parent, centipedes);
                }

                score += 20; // Aumentar puntaje

                (*it)->removeFromParent();
                it = bullets.erase(it);

                segmentHit = true;
                break;
            } else {
                ++it;
            }
        }

        if (!segmentHit) {
            ++i;
        }
    }
}

void Centipede::splitCentipede(int segmentIndex, Node* parent, std::vector<Centipede*>& centipedes) {
    if (segmentIndex < segments.size()) {
        // Crear un nuevo ciempiés para los segmentos posteriores
        auto newCentipede = new Centipede();
        newCentipede->cellWidth = this->cellWidth;
        newCentipede->cellHeight = this->cellHeight;
        newCentipede->direction = -this->direction; // Dirección opuesta al original

        // Mover los segmentos posteriores al nuevo ciempiés
        for (size_t i = segmentIndex; i < segments.size(); ++i) {
            newCentipede->segments.push_back(segments[i]);
        }

        // Eliminar los segmentos movidos del ciempiés original
        segments.erase(segments.begin() + segmentIndex, segments.end());

        // Centrar los segmentos del nuevo ciempiés
        for (auto& segment : newCentipede->segments) {
            Vec2 pos = segment->getPosition();
            float centeredX = std::floor(pos.x / cellWidth) * cellWidth + cellWidth / 2;
            float centeredY = std::floor(pos.y / cellHeight) * cellHeight + cellHeight / 2;
            segment->setPosition(Vec2(centeredX, centeredY));
        }

        // Centrar los segmentos restantes del ciempiés original
        for (auto& segment : segments) {
            Vec2 pos = segment->getPosition();
            float centeredX = std::floor(pos.x / cellWidth) * cellWidth + cellWidth / 2;
            float centeredY = std::floor(pos.y / cellHeight) * cellHeight + cellHeight / 2;
            segment->setPosition(Vec2(centeredX, centeredY));
        }

        // Asegurar que ambos ciempiés mantengan movimiento independiente
        centipedes.push_back(newCentipede);
    }
}





