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
#include "lab_m1/tema01/bumper.h"
#include "lab_m1/tema01/line.h"
#include "lab_m1/tema01/object.h"
#include "lab_m1/tema01/square.h"
#include "lab_m1/tema01/startbutton.h"
#include "lab_m1/tema01/transform2D.h"

#define LOGIC_SPACE_HEIGHT 500
#define LOGIC_SPACE_WIDTH 500

#define BOTTOM_LEFT_CORNER glm::vec3(0, 0, 0)

#define GRID_SQUARE_LENGTH 12
#define GRID_ROW_NUMBER 9
#define GRID_COLUMN_NUMBER 15
#define GRID_TOP_LEFT glm::vec3(205, 176, 0)
#define GRID_HORIZONTAL_OFFSET glm::vec3(18, 0, 0)
#define GRID_VERTICAL_OFFSET glm::vec3(0, 18, 0)

#define SPACESHIP_SQUARE_LENGTH 18
#define SPACESHIP_MAX_COMPONENTS 10

#define COUNTER_NUMBER SPACESHIP_MAX_COMPONENTS
#define COUNTER_SQUARE_LENGTH 20
#define COUNTER_HORIZONTAL_OFFSET glm::vec3(27, 0, 0)
#define COUNTER_TOP_LEFT glm::vec3(210, 205, 0)

#define START_BUTTON_LENGTH 25
#define START_BUTTON_POSITION glm::vec3(455, 245, 0)

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
     * @brief Adds object to spaceship if grid square available.
     *
     * Checks if player releases left mouse button on grid square, checks if the
     * grid square selected is available and if it is adds holding object to
     * spaceship.
     *
     * @param mousePositionLogicSpace - Position of mouse in logic space
     * coordinates.
     */
    void PlaceObjectInGrid(const glm::vec3& mousePositionLogicSpace);

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
     * @brief Removes an object from the spaceship based on its coordinates.
     */
    void RemoveFromSpaceship(const glm::vec3& position);

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
     * @brief Checks if the grid square is used.
     *
     * @param position - Check if object has this position.
     *
     * @return True if inside, false otherwise.
     */
    bool InSpaceShip(const glm::vec3& position);

    /**
     * @brief Based on position given, add or remove object position to grid
     * matrix.
     *
     * @param position - Position of mesh in logic space coordinates.
     */
    void ChangeGridMatrixPositionValue(const glm::vec3& position,
                                       const bool& value);

    /**
     * @brief Checks if adjacent node can be added to BFS search.
     *
     * Checks if row and column are inside matrix boundaries, if
     * visitedMatrix[row][column] is already visited and if
     * gridMatrix[row][column] contains a valid node.
     *
     * @param visitedMatrix - All visited nodes in BFS search.
     * @param row - Row that contains node to check.
     * @param column - Column that contains node to check.
     *
     * @return True if node can be added to BFS search, false otherwise.
     */
    bool IsBFSNodeValid(bool visitedMatrix[GRID_ROW_NUMBER][GRID_COLUMN_NUMBER],
                        int row, int column);
    /**
     * @brief Checks is spaceship is connected.
     *
     * Performs a BFS on the grid matrix starting from the first non false
     * value found in grid matrix. If visited matrix is equal to the grid
     * matrix, then the spaceship is connected.
     *
     * @return True if spaceship is connected, false otherwise.
     */
    bool IsSpaceShipConnected();
    /**
     * @brief Returns true if spaceship configuration is valid. If valid,
     * game can start.
     *
     * @return Spaceship config corectness.
     */
    bool IsSpaceShipConfig();

   protected:
    std::vector<Square> grid;
    std::vector<Square> componentsCounter;
    std::vector<Object> blocksToChoose;

    StartButton* startButton;

    gfxc::TextRenderer textRenderer;

    std::vector<Line> delimiters;
    std::vector<BorderCorners> borders;

    std::vector<Object> spaceship;

    bool isLeftButtonHold;
    Object* buttonHoldObject;

    LogicSpace logicSpace;
    ViewSpace viewSpace;

    bool gridMatrix[GRID_ROW_NUMBER][GRID_COLUMN_NUMBER];

    glm::mat3 visMatrix;
};

}  // namespace hw1
