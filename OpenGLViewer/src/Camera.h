#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class ProjectionMode { Perspective, Orthographic };

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw        {-90.f};
    float Pitch      {  0.f};
    float MovSpeed   {  5.f};
    float MouseSens  {  0.1f};
    float Zoom       { 45.f};
    float OrthoScale {  5.f};

    ProjectionMode projMode{ProjectionMode::Perspective};

    Camera(glm::vec3 pos = {0,0,3}, glm::vec3 up = {0,1,0});

    glm::mat4 getView() const;
    glm::mat4 getProjection(float aspect) const;

    // Keyboard movement
    void processKeyboard(int direction, float dt);
    // Mouse look
    void processMouseMovement(float xoff, float yoff, bool constrainPitch = true);
    // Scroll zoom
    void processScroll(float yoff);

    void toggleProjection();

private:
    void updateVectors();
};

// Direction enum helpers
enum CameraDirection { FORWARD=0, BACKWARD, LEFT, RIGHT_DIR, UP_DIR, DOWN_DIR };
