#include <Personne.hpp>
using namespace godot;
void Personne::_register_methods(){}
Personne::Personne(){}
Personne::~Personne(){}
void Personne::_init(){
	moveSpeed = 10.0f;
	jumpForce = 10.0f;
    gravity = 15.0f;
    GravityVector = Vector3(0, -1, 0);
    translation = Vector3();
    vel = Vector3();
    rotation = Vector3();
    state = IDLE;
    animName = "Armature|mixamocom|Layer0";
    moving = nullptr;
    jumping = nullptr;
    idle = nullptr;
    speed = nullptr;
    nodes = nullptr;
    global = nullptr;
    win = nullptr;
    lose = nullptr;
    mesh = nullptr;
    myIndex = 2;
    charPlayer = PoolStringArray();
}
void Personne::_ready(){
    set_process(false);
    set_physics_process(true);

    
    if (charPlayer.size() == 0) {
        charPlayer.append("res://Blend/Character/Lola.tscn");
        charPlayer.append("res://Blend/Character/Zelda.tscn");
        charPlayer.append("res://Blend/Character/Soldat.tscn");
        charPlayer.append("res://Blend/Character/IslamGirl.tscn");
        charPlayer.append("res://Blend/Character/Ninja.tscn");
        charPlayer.append("res://Blend/Character/Remy.tscn");
    }

    nodes = Object::cast_to<Node>(get_node("/root/Nodes"));
    global = Object::cast_to<Node>(get_node("/root/Global"));

    rotation = get_rotation();
    translation = get_global_translation();
    moving = Object::cast_to<AnimationPlayer>(get_node("Move"));
    jumping = Object::cast_to<AnimationPlayer>(get_node("Jump"));
    idle = Object::cast_to<AnimationPlayer>(get_node("IDLE"));
    speed = Object::cast_to<AnimationPlayer>(get_node("Speed"));
    win = Object::cast_to<AnimationPlayer>(get_node("Win"));
    lose = Object::cast_to<AnimationPlayer>(get_node("Lose"));
}
void Personne::_physics_process(float delta){
    if(mesh == nullptr){
        return;
    }else if(mesh->get_name() != "Armature"){
        mesh->set_name("Armature");
    }
    switch(state){
        case JUMPING:
            if(speed->is_playing()){
                speed->stop(false);
            }else if(idle->is_playing()){
                idle->stop(false);
            }else if(moving->is_playing())
                moving->stop(false);
            if(!jumping->is_playing())
                jumping->play(animName);
        break;
        case MOVING_BACK:
            if(idle->is_playing())
                idle->stop(false);
            if(!moving->is_playing())
                moving->play_backwards(animName);
        break;
        case MOVING_FORWARD: 
            if(idle->is_playing())
                idle->stop(false);
            if(!moving->is_playing())
                moving->play(animName);
        break;
        case MOVING_SPEED:
            if(idle->is_playing()){
                idle->stop(false);  
            }else if(moving->is_playing())
                moving->stop(false);
            if(!speed->is_playing())
                speed->play(animName);
        break;
        case IDLE:
            if(speed->is_playing()){
                speed->stop(false);
            }else if(jumping->is_playing()){
                jumping->stop();
            }else if(moving->is_playing())
                moving->stop(false);
            if(!idle->is_playing())
                idle->play(animName);
        break;
        case WINNER:
            if(idle->is_playing()){
                idle->stop(false);
            }else if(jumping->is_playing()){
                jumping->stop();
            }
            if(!win->is_playing())
                win->play(animName);
        break;
        case LOSER:
            if(idle->is_playing()){
                idle->stop(false);
            }else if(jumping->is_playing()){
                jumping->stop();
            }
            if(!lose->is_playing())
                lose->play(animName);
        break;
    }
    if (!is_on_floor()) {
        vel += GravityVector * gravity * delta;
    }
    vel = move_and_slide(vel, Vector3(0,1,0));
    vel.x = 0;
    vel.z = 0;
}
void Personne::SetIndex(int index){
    myIndex = index;
    mesh = global->call("LoadAndInstance", charPlayer[index], this, Vector3());
    get_node("Armature")->queue_free();
    mesh->set_name("Armature");
}
int Personne::GetIndex(){
    return myIndex;
}