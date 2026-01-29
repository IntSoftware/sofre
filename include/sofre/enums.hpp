
#ifndef SOFRE_ENUMS_HPP
#define SOFRE_ENUMS_HPP

#include <cstdint>

namespace sofre {
/**
 *  - `NORMAL` makes the cursor visible and behaving normally.
 *  - `HIDDEN` makes the cursor invisible when it is over the
 *    content area of the window but does not restrict the cursor from leaving.
 *  - `DISABLED` hides and grabs the cursor, providing virtual
 *    and unlimited cursor movement.  This is useful for implementing for
 *    example 3D camera controls.
 *  - `CAPTURED` makes the cursor visible and confines it to the
 *    content area of the window.
 */
enum class CursorMode {
    NORMAL,
    HIDDEN,
    DISABLED,
    CAPTURED
};

enum class ShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY
};

enum class VertexAttribType {
    Float,
    Int
};

enum class CameraMode {
    None,   // 2D clip-space directly
    Cam2D,
    Cam3D
};

} // namespace sofre

#endif //SOFRE_ENUMS_HPP
