#include <CameraOrbit.hpp>
using namespace godot;
void CameraOrbit::_register_methods(){
    register_method("_ready", &CameraOrbit::_ready);
    register_method("_physics_process", &CameraOrbit::_physics_process);
    register_method("_input", &CameraOrbit::_input);
}
CameraOrbit::CameraOrbit(){}
CameraOrbit::~CameraOrbit(){}
void CameraOrbit::_init(){
    lookSensitivity = 15;
	minLookAngle = -20;
	maxLookAngle = 75;
	player = nullptr;
	mouseDelta = Vector2();
}
void CameraOrbit::_ready(){
    set_process(false);
    set_physics_process(true);
   player = Object::cast_to<Spatial>(get_parent());
//    Input* input = Input::get_singleton();
//    input->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
}
void CameraOrbit::_physics_process(float delta){
    Vector3 rot = Vector3(mouseDelta.y ,mouseDelta.x , 0) * lookSensitivity * delta;
    Vector3 rotationDeg = get_rotation_degrees();
    Vector3 playerRotDeg = player->get_rotation_degrees();
	rotationDeg.x -=  rot.x;
	rotationDeg.x = Math::clamp(rotationDeg.x ,minLookAngle ,maxLookAngle );
	playerRotDeg.y -= rot.y;
	set_rotation_degrees(rotationDeg);
	player->set_rotation_degrees(playerRotDeg);
	mouseDelta = Vector2();
}
void CameraOrbit::_input(Ref<InputEvent> event){
    if (event->is_class("InputEventMouseMotion")) {
        Ref<InputEventMouseMotion> mouse_motion = event;
    	mouseDelta = mouse_motion->get_relative();
    }
}