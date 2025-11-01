/**
 * @file editor.h
 * @brief Declares and describes the Editor class, used to implement the
 * spaceship editor logic.
 * @author Grigoras Vlad Andrei
 */

#pragma once

#include <queue>
#include <string>
#include <utility>

#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "lab_m1/tema01/include/bumper.h"
#include "lab_m1/tema01/include/line.h"
#include "lab_m1/tema01/include/object.h"
#include "lab_m1/tema01/include/spaceship.h"
#include "lab_m1/tema01/include/square.h"
#include "lab_m1/tema01/include/startbutton.h"
#include "lab_m1/tema01/utils/transform2D.h"

#define LOGIC_SPACE_HEIGHT 500
#define LOGIC_SPACE_WIDTH 500

#define BOTTOM_LEFT_CORNER glm::vec3(0, 0, 0)

#define GRID_SQUARE_LENGTH 12
#define GRID_ROW_NUMBER 9
#define GRID_COLUMN_NUMBER 15
#define GRID_TOP_LEFT glm::vec3(205, 176, 0)
#define GRID_HORIZONTAL_OFFSET glm::vec3(18, 0, 0)
#define GRID_VERTICAL_OFFSET glm::vec3(0, 18, 0)

#define SPACESHIP_MAX_COMPONENTS 10

#define COUNTER_NUMBER SPACESHIP_MAX_COMPONENTS
#define COUNTER_SQUARE_LENGTH 20
#define COUNTER_HORIZONTAL_OFFSET glm::vec3(27, 0, 0)
#define COUNTER_TOP_LEFT glm::vec3(210, 205, 0)

#define START_BUTTON_LENGTH 25
#define START_BUTTON_POSITION glm::vec3(455, 235, 0)

#define GAME_STARTING_POSITION glm::vec3(250, 28, 0)
#define GAME_PALLET_SPEED 110.0f

#define VEC3_RED glm::vec3(1, 0, 0)
#define VEC3_GREEN glm::vec3(0, 1, 0)
#define VEC3_BLUE glm::vec3(0, 0, 1)
#define VEC3_LIGHT_GRAY glm::vec3(0.75f, 0.75f, 0.75f)

/**
 * @namespace hw1
 * @brief Contains all classes and methods related to the first assignment.
 */
namespace hw1 {

/**
 * @class Editor
 * @brief Represents the spaceship editor.
 *
 * Used to implement logic behind creating editor scene + creation.
 * Inherits all properties and methods from SimpleScene.
 * @see SimpleScene.
 */
class Editor : public gfxc::SimpleScene {
   public:
    /**
     * @brief Default constructor.
     */
    Editor();

    /**
     * @brief Default destructor.
     */
    ~Editor();

   private:
    /**
     * @struct ViewSpace
     * @brief Structure used to implement ViewSpace logic.
     *
     * Translates logic space to actual screen resolution.
     *
     * @see LogicSpace
     */
    struct ViewSpace {
        ViewSpace() : x(0), y(0), width(800), height(600) {}
        ViewSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}

        int x;
        int y;
        int width;
        int height;
    };

    /**
     * @struct LogicSpace.
     * @brief Structure used to implement LogicSpace logic.
     *
     * Used to overcome limitations of using screen resolution.
     *
     * @see ViewportSpace.
     */
    struct LogicSpace {
        LogicSpace() : x(0), y(0), width(500), height(500) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}

        float x;
        float y;
        float width;
        float height;
    };

    /**
     * @struct BorderCorners.
     * @brief Defines corners of borders designed to delimit editor features
     * from one another.
     */
    struct BorderCorners {
        BorderCorners(std::string name, glm::vec3 bottomLeft, glm::vec3 topLeft,
                      glm::vec3 topRight, glm::vec3 bottomRight)
            : name(name),
              bottomLeft(bottomLeft),
              topLeft(topLeft),
              topRight(topRight),
              bottomRight(bottomRight) {}

        std::string name;

        glm::vec3 bottomLeft;
        glm::vec3 topLeft;
        glm::vec3 topRight;
        glm::vec3 bottomRight;
    };

   public:
    /**
     * @brief Initializes class fields.
     *
     * Inits camera and other class fields.
     */
    void Init() override;

   private:
    /**
     * @brief Creates grid filled with squares.
     *
     * Loads a simple square mesh into GPU memory and uses it to render a 15 x 9
     * square grid.
     */
    void CreateGrid();

    /**
     * @brief Creates component counter section.
     *
     * Loads 10 simple squares into memory and prints number of components left
     * that player can place in spaceship.
     */
    void CreateComponentsCounter();

    /**
     * @brief Creates lines that will separate features.
     *
     * With these lines, every feature, such as the grid, block selection,
     * number of blocks remaining, will be clearly separated.
     */
    void CreateEditorBorders();

    /**
     * @brief Creates all blocks that a player can choose to build their
     * spaceship with.
     */
    void CreateChoosingBlocks();

    /**
     * @brief Creates the start button mesh.
     */
    void CreateStartButton();

    /**
     * @brief Get matrix conversion for translating logic space to view space.
     * @return Conversion matrix.
     */
    glm::mat3 GetSpaceConversionMatrix();

    /**
     * @brief Sets up scene inside window.
     *
     * @param colorColor - Background color.
     * @param clear - Clear color buffer bool.
     *
     * Defines where and how screen will be drawn.
     */
    void SetViewportArea(glm::vec3 colorColor = glm::vec3(0),
                         bool clear = true);

    /**
     * @brief Clears screen at the start of every frame.
     */
    void FrameStart() override;

    /**
     * @brief Draws scene every frame.
     *
     * Gets window resolution, creates view space based by resolution and draws
     * the scene frame by frame.
     *
     * @param deltaTimeSeconds - Time passed since previous frame.
     */
    void Update(float deltaTimeSeconds) override;

    /**
     * @brief Checks what mouse button was clicked and where it was clicked.
     * @param mouseX - Mouse X position.
     * @param mouseY - Mouse Y position.
     * @param button - Button clicked.
     * @param mods - Modifiers (not used).
     */

    /**
     * @brief Handles key hold inputs.
     *
     * @param deltaTimeSeconds - Time passed since previous frame.
     * @param mods - Modifiers (not used).
     */
    void OnInputUpdate(float deltaTime, int mods) override;

    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

    /**
     * @brief Checks what mouse button was released and where it was released.
     * @param mouseX - Mouse X position.
     * @param mouseY - Mouse Y position.
     * @param button - Button clicked.
     * @param mods - Modifiers (not used).
     */
    void OnMouseBtnRelease(int mouseX, int mouseY, int button,
                           int mods) override;

    /**
     * @brief Event for mouse movement. Renders mesh if the player chose a block
     * to place in grid.
     * @param mouseX - Mouse X position.
     * @param mouseY - Mouse Y position.
     * @param deltaX - Change in mouse X coordinate since last frame.
     * @param deltaY - Change in mouse Y coordinate since last frame.
     */
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;

    /**
     * @brief Logic for handling frame end.
     */
    void FrameEnd() override;

    /**
     * @brief Adds object to spaceship if grid square available.
     *
     * Checks if player releases left mouse button on grid square, checks if the
     * grid square selected is available and if it is adds holding object to
     * spaceship.
     *
     * @param mousePositionLogicSpace - Position of mouse in logic space
     * coordinates.
     */
    void PlaceObjectInSpaceShip(const glm::vec3& mousePositionLogicSpace);

    /**
     * @brief Draws all lines that define all borders in current frame.
     */
    void DrawBorders();

    /**
     * @brief Draws grid in current frame.
     */
    void DrawGrid();

    /**
     * @brief Draws all choosing blocks in current frame.
     */
    void DrawChoosingBlocks();

    /**
     * @brief Draws object being held with mouse in current frame.
     */
    void DrawHoldObject();

    /**
     * @brief Draws spaceship in grid in current frame.
     */
    void DrawSpaceShip();

    /**
     * @brief Draws counter section by number of components left for spaceship
     * in current frame.
     */
    void DrawCounterSection();

    /**
     * @brief Draws text in current frame.
     */
    void DrawText();

    /**
     * @brief Draws start button.
     */
    void DrawStartButton();

    /**
     * @brief Draws scene in current frame.
     */
    void DrawScene();

    /**
     * @brief Gets the grid square from which mouse was released.
     *
     * @param mousePositionLogicSpace - Position of mouse in logic space
     * coordinates.
     *
     * @return Center coordinates of square from inside grid, (-1, -1, 0) if not
     * inside grid.
     */
    glm::vec3 GetSquareFromGrid(const glm::vec3& mousePositionLogicSpace);

    glm::vec2 GetPositionFromGrid(const glm::vec3& squarePosition);

    /**
     * @brief Converts screen coordinates (pixels) to logic space coordinates.
     *
     * @param mouseX - Mouse X position in pixels.
     * @param mouseY - Mouse Y position in pixels.
     *
     * @return Position in logic space
     */
    glm::vec3 ConvertScreenToLogicSpace(int mouseX, int mouseY);

    /**
     * @brief Checks in which border the mouse was clicked.
     *
     * @param mousePositionLogicSpace - Position of mouse in logic space
     * coordinates.
     * @param border - Check if mouse inside this border.
     *
     * @return True if inside, false otherwise.
     */
    bool IsInsideBorder(const glm::vec3& mousePositionLogicSpace,
                        const BorderCorners& border) const;

    /**
     * @brief Clears all editor objects and initializes all game fields.
     */
    void InitGame();

    /**
     * @brief Moves spaceship to starting position.
     */
    void PlaceSpaceShipStartPosition();

   protected:
    // ==================================================

    // EDITOR OBJECTS.
    std::vector<Square> grid;
    std::vector<Square> componentsCounter;
    std::vector<Object> blocksToChoose;

    StartButton* startButton;

    std::vector<Line> delimiters;
    std::vector<BorderCorners> borders;

    bool isLeftButtonHold;
    Object* buttonHoldObject;
    // EDITOR OBJECTS.

    // ==================================================

    // EDITOR + GAME OBJECTS.
    SpaceShip* spaceship;

    LogicSpace logicSpace;
    ViewSpace viewSpace;

    gfxc::TextRenderer textRenderer;

    glm::mat3 visMatrix;
    // EDITOR + GAME OBJECTS.

    // ==================================================

    // GAME OBJECTS
    bool isGameRunning;
    bool hasGameStarted;

    int gameScore;
    int numberOfLives;
    // GAME OBJECTS
};

}  // namespace hw1
