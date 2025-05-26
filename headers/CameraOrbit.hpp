#ifndef CAMERA_H
#define CAMERA_H

#include <Godot.hpp>
#include <Spatial.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouseMotion.hpp>
#include <Math.hpp>
#include <Vector2.hpp>

namespace godot
{
    class CameraOrbit : public Spatial
    {
        GODOT_CLASS(CameraOrbit, Spatial);
    private:
        float lookSensitivity;
        float minLookAngle;
        float maxLookAngle;
        Vector2 mouseDelta;
        Spatial* player;
    public:
        static void _register_methods();

        CameraOrbit();
        ~CameraOrbit();
        void _init();
        void _ready();

        void _physics_process(float);
        void _input(Ref<InputEvent>);
    };
}

#endif