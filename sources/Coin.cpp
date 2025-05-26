#include <Coin.hpp>

using namespace godot;

Coin::Coin(){}
Coin::~Coin(){}

void Coin::_init()
{
	rotateSpeed = 5.0f;
    nodes = nullptr;
	bip = nullptr;
}

void Coin::_register_methods()
{
	register_method("_ready", &Coin::_ready);
	register_method("_process", &Coin::_process);
	register_method("OnBodyEntered", &Coin::OnBodyEntered);
	register_method("OnBipFinished", &Coin::OnBipFinished);
}

void Coin::_process(float delta)
{
	rotate_y(rotateSpeed * delta);
}

void Coin::_ready()
{
	bip = Object::cast_to<AudioStreamPlayer3D>(get_node("Bip"));
	bip->connect("finished", this, "OnBipFinished");
	connect("body_entered", this, "OnBodyEntered");
    nodes = Object::cast_to<Node>(get_node("/root/Nodes"));
}

void Coin::OnBodyEntered(Node* body)
{
	if (body->is_in_group("Personne")){
		if(body->get_name() == String::num_int64(get_tree()->get_network_unique_id())){
			body->call("GiveGold");
			nodes->emit_signal("Next");
		}
        bip->play();
		hide();
	}
}
void Coin::OnBipFinished(){
	nodes->emit_signal("Delete", get_name());
}