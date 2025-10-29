/**
 * @file editor.h
 * @brief Declares and describes the Editor class, used to implement the
 * spaceship editor logic.
 * @author Grigoras Vlad Andrei
 */

#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema01/object.h"
#include "lab_m1/tema01/square.h"
#include "lab_m1/tema01/transform2D.h"
#include "utils/gl_utils.h"

#define LOGIC_SPACE_HEIGHT 500
#define LOGIC_SPACE_WIDTH 500
#define BOTTOM_LEFT_CORNER glm::vec3(0, 0, 0)

#define GRID_SQUARE_LENGTH 12
#define GRID_ROW_NUMBER 15
#define GRID_COLUMN_NUMBER 9

#define GRID_TOP_LEFT glm::vec3(200, 170, 0)
#define GRID_HORIZONTAL_OFFSET glm::vec3(18, 0, 0)
#define GRID_VERTICAL_OFFSET glm::vec3(0, 18, 0)

#define VEC3_RED glm::vec3(1, 0, 0)
#define VEC3_GREEN glm::vec3(0, 1, 0)
#define VEC3_BLUE glm::vec3(0, 0, 1)
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
        ViewSpace(GLint x, GLint y, GLsizei width, GLsizei height)
            : x(x), y(y), width(width), height(height) {}
        GLint x;
        GLint y;
        GLsizei width;
        GLsizei height;
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
        GLfloat x;
        GLfloat y;
        GLfloat width;
        GLfloat height;
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
     * @brief Logic for handling frame end.
     */
    void FrameEnd() override;

    /**
     * @brief Draws grid in current frame.
     */
    void DrawGrid();

    /**
     * @brief Draws scene in current frame.
     */
    void DrawScene();

   protected:
    std::vector<Square> grid;
    std::vector<Object> objects;
    LogicSpace logicSpace;
    ViewSpace viewSpace;

    glm::mat3 visMatrix;
};

}  // namespace hw1
