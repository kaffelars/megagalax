#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H


namespace cameracontroller
{
    glm::mat4& getpvmatrix();
    void movecamera(glm::vec3 movement);
    void rotatecamera(glm::vec2 deltarotation);
    void changezoom(float change);
    void initialize();

    void rendercentervao();
    void render();

    void setfreelook(bool freelook);
    void togglecameramode();
    void changeshowcenter(bool onoff);
    bool& getshowcenter();
    void updateprojection();

    void resetcamera();

    glm::vec3 getcampos();
};

#endif // CAMERACONTROLLER_H
