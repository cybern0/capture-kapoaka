#include <OtherPlayer.hpp>
using namespace godot;
void OtherPlayer::_register_methods(){
    Personne::_register_methods();
    register_method("_ready", &OtherPlayer::_ready);
    register_method("_physics_process", &OtherPlayer::_physics_process);
    register_method("ChangeState", &OtherPlayer::ChangeState);
    register_method("OnIsLocationTimeout", &OtherPlayer::OnIsLocationTimeout);
    register_method("SetIndex", &OtherPlayer::SetIndex);
    register_method("GetIndex", &OtherPlayer::GetIndex);
}
void OtherPlayer::SetIndex(int index){
    Personne::SetIndex(index);
}
int OtherPlayer::GetIndex(){
    return Personne::GetIndex();
}
OtherPlayer::OtherPlayer(){}
OtherPlayer::~OtherPlayer(){}
void OtherPlayer::_init(){
    Personne::_init();
    isLocation = nullptr;
}
void OtherPlayer::_ready(){
    Personne::_ready();
    isLocation = Object::cast_to<Timer>(get_node("IsLocation"));
    isLocation->connect("timeout", this, "OnIsLocationTimeout");
}
void OtherPlayer::_physics_process(float delta){
    switch(state){
    case JUMPING:
        if(get_global_translation() != translation)
            set_global_translation(translation);
    break;
    case MOVING_BACK:
        vel.x += Math::sin(rotation.y);
        vel.z += Math::cos(rotation.y);
    break;
    case MOVING_FORWARD:
        vel.x -= Math::sin(rotation.y);
        vel.z -= Math::cos(rotation.y);
    break;
    case MOVING_SPEED:
        vel.x -= Math::sin(rotation.y) * moveSpeed;
        vel.z -= Math::cos(rotation.y) * moveSpeed;
    break;
    case IDLE:
    break;
    }
    if(get_rotation() != rotation)
        set_rotation(rotation);
    Personne::_physics_process(delta);
}
void OtherPlayer::ChangeState(int newState, Vector3 rotationDeg, Vector3 translationDeg){
    state = newState;
    rotation = rotationDeg;
    translation = translationDeg;
}
void OtherPlayer::OnIsLocationTimeout(){
    if(get_global_translation() != translation)
        set_global_translation(translation);
}
