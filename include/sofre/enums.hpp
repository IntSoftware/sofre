
#ifndef SOFRE_ENUMS_HPP
#define SOFRE_ENUMS_HPP

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

#endif //SOFRE_ENUMS_HPP
