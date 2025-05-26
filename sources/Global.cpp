#include <Global.hpp>

using namespace godot;
void Global::_register_methods(){
    register_method("_ready", &Global::_ready);
    register_method("NewGame", &Global::NewGame);
    register_method("MainMenu", &Global::MainMenu);
    register_method("GameOver", &Global::GameOver);
    register_method("NextLevel", &Global::NextLevel);
    register_method("LoadAndInstance", &Global::LoadAndInstance);
}
Global::Global(){

}
Global::~Global(){

}
void Global::_init(){
    levels = PoolStringArray();
    endScreen = "res://GameOver/GameOver.tscn";
    startScreen = "res://MainMenu/MainMenu.tscn";
    currentLevel = -1;
}
void Global::_ready(){
    set_process(false);
    set_physics_process(false);
    if (levels.size() == 0) {
        levels.append("res://Game/Level/Level3.tscn");
        levels.append("res://Game/Level/Level2.tscn");
        levels.append("res://Game/Level/Level4.tscn");
        levels.append("res://Game/Level/Level1.tscn");
    }
}
void Global::NewGame() {
    currentLevel = -1;
    NextLevel();
}

void Global::GameOver() {
    currentLevel -= 1;
    get_tree()->change_scene(endScreen);
}
void Global::MainMenu() {
    get_tree()->change_scene(startScreen);
}

void Global::NextLevel() {
    currentLevel += 1;
    if (currentLevel >= levels.size()) {
        GameOver();
    } else {
        get_tree()->change_scene(levels[currentLevel]);
    }
}

Node* Global::LoadAndInstance(String path, Node* parent, Vector3 position) {
    ResourceLoader* resourceLoader = ResourceLoader::get_singleton();
    Ref<PackedScene> scene = resourceLoader->load(path);
    
    if (scene.is_null()) {
        return nullptr;
    }else{
        Node* instance = scene->instance();
        Spatial* spatial = Object::cast_to<Spatial>(instance);
        if (spatial) {
            spatial->set_translation(position);
        }
        parent->add_child(instance);
        return instance;
    }
}