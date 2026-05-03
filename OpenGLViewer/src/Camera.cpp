#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

Camera::Camera(glm::vec3 pos, glm::vec3 up)
    : Position(pos), WorldUp(up)
{
    updateVectors();
}

glm::mat4 Camera::getView() const {
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::getProjection(float aspect) const {
    if (projMode == ProjectionMode::Perspective) {
        return glm::perspective(glm::radians(Zoom), aspect, 0.1f, 500.f);
    } else {
        float h = OrthoScale;
        float w = h * aspect;
        return glm::ortho(-w, w, -h, h, 0.1f, 500.f);
    }
}

void Camera::processKeyboard(int dir, float dt) {
    float v = MovSpeed * dt;
    if (dir == FORWARD)   Position += Front * v;
    if (dir == BACKWARD)  Position -= Front * v;
    if (dir == LEFT)      Position -= Right * v;
    if (dir == RIGHT_DIR) Position += Right * v;
    if (dir == UP_DIR)    Position += Up    * v;
    if (dir == DOWN_DIR)  Position -= Up    * v;
}

void Camera::processMouseMovement(float xoff, float yoff, bool constrainPitch) {
    Yaw   += xoff * MouseSens;
    Pitch += yoff * MouseSens;
    if (constrainPitch)
        Pitch = std::clamp(Pitch, -89.f, 89.f);
    updateVectors();
}

void Camera::processScroll(float yoff) {
    if (projMode == ProjectionMode::Perspective)
        Zoom = std::clamp(Zoom - yoff, 1.f, 90.f);
    else
        OrthoScale = std::max(0.5f, OrthoScale - (float)yoff * 0.5f);
}

void Camera::toggleProjection() {
    projMode = (projMode == ProjectionMode::Perspective)
                 ? ProjectionMode::Orthographic
                 : ProjectionMode::Perspective;
}

void Camera::updateVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    f.y = sin(glm::radians(Pitch));
    f.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(f);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}
