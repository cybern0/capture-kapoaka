#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <Godot.hpp>
#include <Spatial.hpp>
#include <PacketPeerUDP.hpp>
#include <Button.hpp>
#include <SceneTree.hpp>
#include <Input.hpp>
#include <IP.hpp>

namespace godot {

class MainMenu : public Spatial 
{
    GODOT_CLASS(MainMenu, Spatial)
private:
    Ref<PacketPeerUDP> UDP;
    const int UDP_PORT = 54321;
    const String MOT_MAGIC = "gl0bal";
    const String MULTICAST_GROUP = "224.1.1.1";
    const String BIND_ADRESS = "0.0.0.0";
    String serverIP;
    Button* join;
    Button* host;
    Button* left;
    Button* right;
    Node* nodes;
    Node* global;
    Spatial* camera;
    Array charcter;
    int index;
public:
    static void _register_methods();

    MainMenu();
    ~MainMenu();

    void _init();
    void _ready();
    void _physics_process(float);
    void OnJoinPressed();
    void OnHostPressed();
    void OnLeftPressed();
    void OnRightPressed();
    void OnAnimationFinished();
    String GetInterfaceName();
};

}

#endif
