#include <Nodes.hpp>
#include <MainMenu.hpp>
#include <Global.hpp>
#include <GameOver.hpp>
#include <Server.hpp>
#include <CameraOrbit.hpp>
#include <Personne.hpp>
#include <Player.hpp>
#include <OtherPlayer.hpp>
#include <Level.hpp>
#include <Coin.hpp>

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);
    godot::register_class<godot::Nodes>();
    godot::register_class<godot::MainMenu>();
    godot::register_class<godot::Global>();
    godot::register_class<godot::GameOver>();
    godot::register_class<godot::Server>();
    godot::register_class<godot::Personne>();
    godot::register_class<godot::Player>();
    godot::register_class<godot::OtherPlayer>();
    godot::register_class<godot::CameraOrbit>();
    godot::register_class<godot::Level>();
    godot::register_class<godot::Coin>();
}
