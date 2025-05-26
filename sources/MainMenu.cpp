#include <MainMenu.hpp>
using namespace godot;

void MainMenu::_register_methods(){
    register_method("_ready", &MainMenu::_ready);
    register_method("OnJoinPressed", &MainMenu::OnJoinPressed);
    register_method("OnHostPressed", &MainMenu::OnHostPressed);
    register_method("OnLeftPressed", &MainMenu::OnLeftPressed);
    register_method("OnRightPressed", &MainMenu::OnRightPressed);
    register_method("_physics_process", &MainMenu::_physics_process);
    register_method("OnAnimationFinished", &MainMenu::OnAnimationFinished);
    register_method("GetInterfaceName", &MainMenu::GetInterfaceName);
}

MainMenu::MainMenu(){

}
MainMenu::~MainMenu(){

}
void MainMenu::_init(){
    serverIP = "";
    join = nullptr;
    host = nullptr;
    left = nullptr;
    right = nullptr;
    nodes = nullptr;
    global = nullptr;
    camera = nullptr;
    index = 0;
}
void MainMenu::_ready(){
    set_process(false);
    set_physics_process(false);
    Input* input = Input::get_singleton();
    input->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);

    join = Object::cast_to<Button>(get_node("CanvasLayer/Join"));
    join->connect("pressed", this, "OnJoinPressed");
    host = Object::cast_to<Button>(get_node("CanvasLayer/Host"));
    host->connect("pressed", this, "OnHostPressed");
    left = Object::cast_to<Button>(get_node("CanvasLayer/Left"));
    left->connect("pressed", this, "OnLeftPressed");
    right = Object::cast_to<Button>(get_node("CanvasLayer/Right"));
    right->connect("pressed", this, "OnRightPressed");

    
    camera = Object::cast_to<Spatial>(get_node("CameraOrbit"));

    nodes = Object::cast_to<Node>(get_node("/root/Nodes"));
    global = Object::cast_to<Node>(get_node("/root/Global"));

    Node* Char = global->call("LoadAndInstance", String("res://Blend/Char.tscn"), this, Vector3());
    charcter = Char->get_children();
    Spatial* child = Object::cast_to<Spatial>(charcter[index]);
    camera->set_global_translation(child->get_global_translation());

    get_tree()->call_group("Dance", "connect", "animation_finished", this, "OnAnimationFinished");
    get_tree()->call_group("Dance", "play", "Armature|mixamocom|Layer0");
}
void MainMenu::_physics_process(float delta){
    while(UDP->get_available_packet_count() > 0){
        PoolByteArray packetData = UDP->get_packet();
        String msg;
        for(int i = 0; i < packetData.size(); i++)
            msg += static_cast<char>(packetData[i]);
        if(msg.empty())
            continue;
        if(msg.begins_with(MOT_MAGIC)){
            serverIP = UDP->get_packet_ip();
            UDP->close();
            global = Object::cast_to<Node>(get_node("/root/Global"));
            nodes = Object::cast_to<Node>(get_node("/root/Nodes"));
            if (global) {
                if(!nodes->has_node("Server"))
                    Node* client = global->call("LoadAndInstance", String("res://Game/Networking/Server.tscn"), nodes, Vector3(0,0,0));
                nodes->emit_signal("ServerIP", serverIP, index);
                global->call("NewGame");
            }
            set_physics_process(false);
        }
    }
}
void MainMenu::OnJoinPressed(){
    UDP.instance();
    host->set_disabled(true);
    left->set_disabled(true);
    right->set_disabled(true);
    const String INTERFACE_NAME =  GetInterfaceName();
    Error err = UDP->listen(UDP_PORT, BIND_ADRESS);
    if (err != Error::OK) {
        return;
    }
    err = UDP->join_multicast_group(MULTICAST_GROUP, INTERFACE_NAME);
    if (err != Error::OK) {
        UDP->close();
        return;
    }else{
        set_physics_process(true);
    }
}
String MainMenu::GetInterfaceName() {
    Array interfaces = IP::get_singleton()->get_local_interfaces();
    String lastInterfaceName = "";
    int countIP = 0;
    for (int i = 0; i < interfaces.size(); i++) {
        Dictionary iface = interfaces[i];
        String name = iface["name"];
        Array addresses = iface["addresses"];
        for (int j = 0; j < addresses.size(); j++) {
            String ip = addresses[j];
            if (ip.begins_with("192.168.") || ip.begins_with("10.") || ip.begins_with("172.")) {
                if(ip.begins_with("192.168.")){
                    lastInterfaceName = name;
                    countIP++;
                    continue;
                }
                return name;
            }
        }
    }
    if(countIP >= 1){
        return lastInterfaceName;
    }else{
        get_tree()->quit();
    }
    return "";
}
void MainMenu::OnHostPressed(){
    join->set_disabled(true);
    left->set_disabled(true);
    right->set_disabled(true);
    global = Object::cast_to<Node>(get_node("/root/Global"));
    nodes = Object::cast_to<Node>(get_node("/root/Nodes"));
    if (global) {
        if(!nodes->has_node("Server"))
            Node* server = global->call("LoadAndInstance", String("res://Game/Networking/Server.tscn"), nodes, Vector3(0,0,0));
        nodes->emit_signal("Hoster", index);
        global->call("NewGame");
    }
}
void MainMenu::OnAnimationFinished(){
    get_tree()->call_group("Dance", "play", "Armature|mixamocom|Layer0");
}
void MainMenu::OnLeftPressed(){
    index--;
    if(index < 0){
        index = charcter.size() - 1;
    }
    Spatial* child = Object::cast_to<Spatial>(charcter[index]);
    camera->set_global_translation(child->get_global_translation());
}
void MainMenu::OnRightPressed(){
    index++;
    if(index >= charcter.size())
        index = 0;
    Spatial* child = Object::cast_to<Spatial>(charcter[index]);
    camera->set_global_translation(child->get_global_translation());
}