#include <Player.hpp>
using namespace godot;
void Player::_register_methods(){
    register_method("_ready", &Player::_ready);
    register_method("_physics_process", &Player::_physics_process);
    register_method("GiveGold", &Player::GiveGold);
    register_method("IsWinner", &Player::IsWinner);
    register_method("SetIndex", &Player::SetIndex);
    register_method("GetIndex", &Player::GetIndex);
}
void Player::SetIndex(int index){
    Personne::SetIndex(index);
}
int Player::GetIndex(){
    return Personne::GetIndex();
}
Player::Player(){}
Player::~Player(){}
void Player::_init(){
    Personne::_init();
    label = nullptr;
    nbrCoin = 0;
}
void Player::_ready(){
    Personne::_ready();
    label = Object::cast_to<Label>(get_node("CanvasLayer/Label"));
    
    nodes->connect("IsWinner", this, "IsWinner");
}
void Player::_physics_process(float delta){
    rotation = get_rotation();
    translation = get_global_translation();
    if(translation.y <= -5){
        nodes->emit_signal("Die");
    }
    Input* input = Input::get_singleton();
    switch(state){
    case MOVING_BACK:
        if (input->is_key_pressed(GlobalConstants::KEY_Q)){
            vel.x += Math::sin(rotation.y);
            vel.z += Math::cos(rotation.y);
        }else{
            state = IDLE;
        }
    break;
    case MOVING_SPEED:
        if (input->is_key_pressed(GlobalConstants::KEY_C)){
            vel.x -= Math::sin(rotation.y) * moveSpeed;
            vel.z -= Math::cos(rotation.y) * moveSpeed;
        }else{
            state = IDLE;
        }
        if (input->is_action_pressed("ui_accept") && is_on_floor()){
            vel.y = jumpForce * moveSpeed;
            state = JUMPING;
        }
    case MOVING_FORWARD:
        if (input->is_key_pressed(GlobalConstants::KEY_S)){
            vel.x -= Math::sin(rotation.y);
            vel.z -= Math::cos(rotation.y);
        }else{
            state = IDLE;
        }
        if (input->is_action_pressed("ui_accept") && is_on_floor()){
            vel.y = jumpForce;
            state = JUMPING;
        }else if (input->is_key_pressed(GlobalConstants::KEY_C)){
            state = MOVING_SPEED;
        }
    break;
    case JUMPING:
    
        if(is_on_floor()){
            state = IDLE;
        }else if (input->is_key_pressed(GlobalConstants::KEY_C)){
            vel.x -= Math::sin(rotation.y) * moveSpeed;
            vel.z -= Math::cos(rotation.y) * moveSpeed;
        }
    break;
    case IDLE:
        if(input->is_action_pressed("ui_accept") && is_on_floor()){
            vel.y = jumpForce;
            state = JUMPING;
        }else if (input->is_key_pressed(GlobalConstants::KEY_S)){
            state = MOVING_FORWARD;
        }else if (input->is_key_pressed(GlobalConstants::KEY_Q)){
            state = MOVING_BACK;
        }else if (input->is_key_pressed(GlobalConstants::KEY_C)){
            state = MOVING_SPEED;
        }
    break;
    }
    nodes->emit_signal("ChangeState", state, rotation, translation);
    Personne::_physics_process(delta);    
}
void Player::GiveGold(){
    label->set_text("Coin : " + String::num_int64(++nbrCoin));
    nodes->emit_signal("GetCoin", nbrCoin);
}
void Player::IsWinner(bool isWin){
    state = isWin ? WINNER : LOSER;
}
