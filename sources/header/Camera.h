#pragma once

#include "Matrix4.h"
#include "Transform.h"

enum CameraType
{
    Perspective,
    Orhographic
};

class Camera
{
public:
    ~Camera();

    Transform transform;

    static Camera* createPerspectiveCamera(float aspectRatio);
    static Camera* createOrhographicCamera(float aspectRatio);

    void setFieldOfView(float fov);
    float getFieldOfView();

    void setAspectRatio(float aspectRatio);
    float getAspectRatio();

    void setZoom(float zoom);
    float getZoom();

    Matrix4& getViewProjectionMatrix();

    bool viewModified;

private:

    Camera();

    void updateView();
    void updateProjection();
    void updateProjectionPersp();
    void updateProjectionOrtho();

    CameraType type;

    float fieldOfView;
    float aspectRatio;

    float nearPlane;
    float farPlane;

    float zoomOrtho;

    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
    Matrix4 viewProjectionMatrix;

    bool projectionModified;
};