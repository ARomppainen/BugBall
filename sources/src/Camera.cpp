#include "Camera.h"

// private constructor
Camera::Camera()
    : transform()
{

}

Camera::~Camera()
{

}

Camera* Camera::createPerspectiveCamera(float aspectRatio)
{
    Camera* c = new Camera();
    c->type = Perspective;

    c->fieldOfView = 70;
    c->aspectRatio = aspectRatio;
    c->nearPlane = 0.01f;
    c->farPlane = 100.0f;

    c->viewModified = true;
    c->projectionModified = true;

    c->zoomOrtho = 0.0f;

    return c;
}

Camera* Camera::createOrhographicCamera(float aspectRatio)
{
    Camera* c = new Camera();
    c->type = Orhographic;

    c->fieldOfView = -1;
    c->aspectRatio = aspectRatio;

    c->viewModified = true;
    c->projectionModified = true;

    c->zoomOrtho = 1.0f;

    return c;
}

void Camera::setFieldOfView(float fov)
{
    fieldOfView = fov;

    if (fieldOfView < 0.1f)
        fieldOfView = 0.01f;
    else if (fieldOfView > 179.9f)
        fieldOfView = 179.9f;

    if (type == Perspective)
        projectionModified = true;
}

float Camera::getFieldOfView()
{
    return fieldOfView;
}

void Camera::setAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    projectionModified = true;
}

float Camera::getAspectRatio()
{
    return aspectRatio;
}

void Camera::setZoom(float zoom)
{
    if (this->type == Orhographic) {
        this->zoomOrtho = zoom;
        projectionModified = true;
    } else {
        this->zoomOrtho = 0.0f;
    }
}

float Camera::getZoom()
{
    return zoomOrtho;
}

Matrix4& Camera::getViewProjectionMatrix()
{
    bool updated = false;

    if (viewModified) {
        updateView();

        viewModified = false;
        updated = true;
    }

    if (projectionModified) {
        updateProjection();

        projectionModified = false;
        updated = true;
    }

    if (updated) {
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    return viewProjectionMatrix;
}

void Camera::updateView()
{
    Matrix4 rotation    = Matrix4::RotationMatrix   (  this->transform.rotation );
    Matrix4 translation = Matrix4::TranslationMatrix( -this->transform.position );

    viewMatrix = rotation * translation;
}

void Camera::updateProjection()
{
    if (type == Perspective) {
        updateProjectionPersp();
    } else if (type == Orhographic) {
        updateProjectionOrtho();
    }
}

void Camera::updateProjectionPersp()
{
    projectionMatrix = Matrix4::PerspectiveM(
        fieldOfView,
        aspectRatio,
        nearPlane,
        farPlane);
}

void Camera::updateProjectionOrtho()
{
    projectionMatrix = Matrix4::OrthoM(
        -aspectRatio * zoomOrtho,
        aspectRatio * zoomOrtho,
        zoomOrtho,
        -zoomOrtho,
        10.0f,
        -10.0f);
}