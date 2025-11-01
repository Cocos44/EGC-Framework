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
}
