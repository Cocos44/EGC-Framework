/**
 * @file game.cpp
 *
 * @brief Implements game logic and every method related to the game
 * functionality of Editor class.
 *
 * @see Editor
 *
 * @author Grigoras Vlad Andrei
 */

#include <iostream>

#include "lab_m1/tema01/include/editor.h"

void hw1::Editor::InitGame() {
    this->grid.clear();
    this->componentsCounter.clear();
    this->blocksToChoose.clear();

    delete this->startButton;
    this->startButton = nullptr;

    this->delimiters.clear();
    this->borders.clear();

    this->isGameRunning = true;
    this->PlaceSpaceShipStartPosition();
}

void hw1::Editor::PlaceSpaceShipStartPosition() {
    glm::vec3 offsetToStart =
        this->spaceship->centerPosition - GAME_STARTING_POSITION;

    for (auto& object : this->spaceship->components) {
        object.SetPosition(object.GetPosition() - offsetToStart);
    }
}
