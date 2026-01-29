#ifndef SOFRE_CAMERA_HPP
#define SOFRE_CAMERA_HPP

#include <sofre/math.hpp>
#include <sofre/enums.hpp>

namespace sofre {

struct CameraParams {
    union {
        struct {
            // Perspective 3D
            vec3  eye;
            vec3  center;
            vec3  up;
            float fovY;
            float zNear;
            float zFar;
        } cam3d;

        struct {
            // Cam2D
            float left;
            float right;
            float bottom;
            float top;
            float zNear;
            float zFar;
        } cam2d;
    };

    CameraMode type;

    /* default is cam3D */
    CameraParams() : type(CameraMode::Cam3D) {
        cam3d.eye    = {0, 0, 0};
        cam3d.center = {0, 0, -1};
        cam3d.up     = {0, 1, 0};
        cam3d.fovY   = 45.0f;
        cam3d.zNear  = 0.1f;
        cam3d.zFar   = 100.0f;
    }

    // Cam3D constructor
    CameraParams(const vec3& eye_, const vec3& center_, const vec3& up_,
                 float fovY_, float zNear_, float zFar_)
        : type(CameraMode::Cam3D) {
        cam3d.eye = eye_;
        cam3d.center = center_;
        cam3d.up = up_;
        cam3d.fovY = fovY_;
        cam3d.zNear = zNear_;
        cam3d.zFar = zFar_;
    }

    // Cam2D constructor
    CameraParams(float left_, float right_, float bottom_, float top_,
                 float zNear_, float zFar_)
        : type(CameraMode::Cam2D) {
        cam2d.left = left_;
        cam2d.right = right_;
        cam2d.bottom = bottom_;
        cam2d.top = top_;
        cam2d.zNear = zNear_;
        cam2d.zFar = zFar_;
    }

    // None (clip-space) constructor
    static CameraParams None() {
        CameraParams p;
        p.type = CameraMode::None;
        return p;
    }

    // compute view/projection matrices (renderer-side implementation)
    void computeView(mat4& outView) const;
    void computeProj(mat4& outProj, float aspect) const;
};

} // namespace sofre

#endif