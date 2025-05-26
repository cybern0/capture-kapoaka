#include <Nodes.hpp>

using namespace godot;
void Nodes::_register_methods(){
    register_method("_ready", &Nodes::_ready);
    register_signal<Nodes>("ChangeState",
         "newState", GODOT_VARIANT_TYPE_INT,
          "rotationDeg", GODOT_VARIANT_TYPE_VECTOR3,
          "translationDeg", GODOT_VARIANT_TYPE_VECTOR3
    );
    register_signal<Nodes>("Die");
    register_signal<Nodes>("ServerIP",
        "ip", GODOT_VARIANT_TYPE_STRING,
        "index", GODOT_VARIANT_TYPE_INT
    );
    register_signal<Nodes>("Hoster", "index", GODOT_VARIANT_TYPE_INT);
    register_signal<Nodes>("ReqCoin",
        "i", GODOT_VARIANT_TYPE_INT,
        "location", GODOT_VARIANT_TYPE_VECTOR3
    );
    register_signal<Nodes>("RespawnPlayer");
    register_signal<Nodes>("ExitTree");
    register_signal<Nodes>("GetCoin", "nbrCoin", GODOT_VARIANT_TYPE_INT);
    register_signal<Nodes>("End");
    register_signal<Nodes>("IsWinner", "isWin", GODOT_VARIANT_TYPE_BOOL);
    register_signal<Nodes>("Next");
    register_signal<Nodes>("Delete", "name", GODOT_VARIANT_TYPE_STRING);
}
Nodes::Nodes(){

}
Nodes::~Nodes(){

}
void Nodes::_init(){

}
void Nodes::_ready(){
    set_process(false);
    set_physics_process(false);
}