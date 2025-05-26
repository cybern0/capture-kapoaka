#include <Level.hpp>
using namespace godot;

void Level::_register_methods(){
    register_method("_ready", &Level::_ready);
    register_method("ChooseCoinLocation", &Level::ChooseCoinLocation);
    register_method("OnAudioFinished", &Level::OnAudioFinished);
}
Level::Level(){

}
Level::~Level(){

}
void Level::_init(){
    tmp = 0;
    nodes = nullptr;
    nbrCoin = 0;
    audio = nullptr;
}
void Level::_ready(){
    audio = Object::cast_to<AudioStreamPlayer>(get_node("AudioStreamPlayer"));
    audio->connect("finished", this, "OnAudioFinished");
    audio->play();
    if(get_name() == String("Level3")){
        tmp = 20;
        nbrCoin = 1;
    }else if(get_name() == String("Level2")){
        tmp = 30;
        nbrCoin = 3;
    }else if(get_name() == String("Level1")){
        tmp = 60;
        nbrCoin = 1;
    }else if(get_name() == String("Level4")){
        tmp = 20;
        nbrCoin = 2;
    }else{
        tmp = 0;
        nbrCoin = 0;
    }
    spawnCoin.append(Vector3(tmp,5,0));
    spawnCoin.append(Vector3(0,5,tmp));
    spawnCoin.append(Vector3(-tmp,5,0));
    spawnCoin.append(Vector3(0,5,-tmp));
    nodes = Object::cast_to<Node>(get_node(static_cast<NodePath>("/root/Nodes")));
    for(int i = 0; i < nbrCoin; i++)
        nodes->emit_signal("ReqCoin", i, ChooseCoinLocation());
}
Vector3 Level::ChooseCoinLocation(){
    Ref<RandomNumberGenerator> rng = RandomNumberGenerator::_new();
    rng->randomize();
    int point = rng->randi() % spawnCoin.size();
    Vector3 loc = spawnCoin[point];
    spawnCoin.remove(point);
    return loc;
}
void Level::OnAudioFinished(){
    audio->play();
}