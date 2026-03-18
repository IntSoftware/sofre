#include <sofre/camera.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstring>

namespace sofre {

void CameraParams::computeView(mat4& outView) const {
    switch (type) {
    case CameraMode::Cam3D: {
         outView = glm::lookAt(
            glm::vec3(cam3d.eye.x,    cam3d.eye.y,    cam3d.eye.z),
            glm::vec3(cam3d.center.x, cam3d.center.y, cam3d.center.z),
            glm::vec3(cam3d.up.x,     cam3d.up.y,     cam3d.up.z)
        );
        break;
    }
    case CameraMode::Cam2D: {
        // view = identity for 2D
        outView = sofre::mat4();
        break;
    }
    case CameraMode::None:
    default:
        // No camera concept: do not modify outView
        break;
    }
}

void CameraParams::computeProj(mat4& outProj, float aspect) const
{
    switch (type) {
    case CameraMode::Cam3D: {
        outProj = glm::perspective(
            glm::radians(cam3d.fovY),
            aspect,
            cam3d.zNear,
            cam3d.zFar
        );
        break;
    }
    case CameraMode::Cam2D: {
        outProj = glm::ortho(
            cam2d.left,
            cam2d.right,
            cam2d.bottom,
            cam2d.top,
            cam2d.zNear,
            cam2d.zFar
        );
        break;
    }
    case CameraMode::None:
    default:
        // No camera concept: do not modify outProj
        break;
    }
}

} // namespace sofre
