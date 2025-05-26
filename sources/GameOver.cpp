#include <GameOver.hpp>
using namespace godot;

void GameOver::_register_methods(){
    register_method("_ready", &GameOver::_ready);
    register_method("OnRespawnPressed", &GameOver::OnRespawnPressed);
    register_method("OnExitPressed", &GameOver::OnExitPressed);
}

GameOver::GameOver(){

}
GameOver::~GameOver(){

}

void GameOver::_init(){
    exit = nullptr;
    restart = nullptr;
    nodes = nullptr;
}
void GameOver::_ready(){
    set_process(false);
    set_physics_process(false);
    Input* input = Input::get_singleton();
    input->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
    exit = Object::cast_to<Button>(get_node("Exit"));
    exit->connect("pressed", this, "OnExitPressed");
    restart = Object::cast_to<Button>(get_node("Respawn"));
    restart->connect("pressed", this, "OnRespawnPressed");
    nodes = Object::cast_to<Node>(get_node("/root/Nodes"));
    nodes->emit_signal("End");
}
void GameOver::OnRespawnPressed(){
    if(nodes)
        nodes->emit_signal("RespawnPlayer");
}
void GameOver::OnExitPressed(){
    if(nodes)
        nodes->emit_signal("ExitTree");
}