#include <Server.hpp>
using namespace godot;

void Server::_register_methods(){
    register_method("_ready", &Server::_ready);
    register_method("RespawnPlayer", &Server::RespawnPlayer);
    register_method("ChooseSpawnLocation", &Server::ChooseSpawnLocation);
    register_method("ExitTree", &Server::ExitTree);
    register_method("OnChangeState", &Server::OnChangeState);
    register_method("IsNext", &Server::IsNext);
    register_method("ReLoc", &Server::ReLoc);
    register_method("OnGetCoin", &Server::OnGetCoin);
    register_method("OnEnd", &Server::OnEnd);

    register_method("StartServer", &Server::StartServer);
    register_method("JoinServer", &Server::JoinServer);
    register_method("Die", &Server::Die);

    register_method("OnStateServer", &Server::OnStateServer);
    register_method("OnBroadcastTimeout", &Server::OnBroadcastTimeout);
    register_method("OnServerIP", &Server::OnServerIP);
    register_method("OnHoster", &Server::OnHoster);
    register_method("OnConnected", &Server::OnConnected);
    register_method("OnDisconnected", &Server::OnDisconnected);
    register_method("OnConnectionFaild", &Server::OnConectionFaild);
    register_method("OnPlayerConnected", &Server::OnPlayerConnected);
    register_method("OnPlayerDisconnected", &Server::OnPlayerDisconnected);
    //
    register_method("SetWinner", &Server::SetWinner, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("RequestWinner", &Server::RequestWinner, GODOT_METHOD_RPC_MODE_MASTER);
    //
    register_method("InstanceCoin", &Server::InstanceCoin, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("RequestCoin", &Server::RequestCoin, GODOT_METHOD_RPC_MODE_MASTER);
    register_method("RequestFirstCoin", &Server::RequestFirstCoin, GODOT_METHOD_RPC_MODE_MASTER);
    //
    register_method("NextLevel", &Server::NextLevel, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("RequestLevel", &Server::RequestLevel, GODOT_METHOD_RPC_MODE_MASTER);
    //
    register_method("UpdatePlayer", &Server::UpdatePlayer, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("RequestUpdate", &Server::RequestUpdate, GODOT_METHOD_RPC_MODE_MASTER);
    //
    register_method("DeleteObj", &Server::DeleteObj, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("RequestDelete", &Server::RequestDelete, GODOT_METHOD_RPC_MODE_MASTER);
    //
    register_method("InstancePlayer", &Server::InstancePlayer, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("RequestPlayer", &Server::RequestPlayer, GODOT_METHOD_RPC_MODE_MASTER);
    //
    register_method("InstanceOtherPlayer", &Server::InstanceOtherPlayer, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("RequestOtherPlayer", &Server::RequestOtherPlayer, GODOT_METHOD_RPC_MODE_MASTER);
    //
}
Server::Server(){}
Server::~Server(){}
void Server::_init(){
    nodes = nullptr;
    global = nullptr;
    serverIP = "";
    isConnected = false;
    isServer = false;
    nbrSender = 0;
    indexPlayer = 0;
    winnerID = 1;
    winnerCoin = 0;
    state = DIE;
    broadcaster = nullptr;
    spawnPoint.append(Vector3(5,5,0));
    spawnPoint.append(Vector3(0,5,5));
    spawnPoint.append(Vector3(5,5,5));
    spawnPoint.append(Vector3(-5,5,0));
    spawnPoint.append(Vector3(0,5,-5));
    spawnPoint.append(Vector3(-5,5,-5));
    spawnPoint.append(Vector3(5,5,-5));
    spawnPoint.append(Vector3(-5,5,5));
    spawnPointClone = spawnPoint.duplicate();
}
void Server::_ready(){
    set_process(false);
    set_physics_process(false);
    global = Object::cast_to<Node>(get_node("/root/Global"));
    nodes = Object::cast_to<Node>(get_node("/root/Nodes"));
    broadcaster = Object::cast_to<Timer>(get_node("Broadcast"));
    nodes->connect("ServerIP", this, "OnServerIP");
    nodes->connect("Hoster", this, "OnHoster");
    nodes->connect("ChangeState", this, "OnChangeState");
    nodes->connect("Die", this, "ReLoc");
    nodes->connect("ReqCoin", this, "RequestCoin");
    nodes->connect("RespawnPlayer", this, "RespawnPlayer");
    nodes->connect("ExitTree", this, "ExitTree");
    nodes->connect("GetCoin", this, "OnGetCoin");
    nodes->connect("End", this, "OnEnd");
    nodes->connect("Next", this, "IsNext");
    nodes->connect("Delete", this, "RequestDelete");
}
void Server::RespawnPlayer(){
    OnStateServer(DIE);
    global->call("MainMenu");
}
void Server::ExitTree(){
    get_tree()->quit();
}
void Server::IsNext(){
    Array coinArray = get_tree()->get_nodes_in_group("Coin");
    if(coinArray.size() == 1){
        RequestLevel();
    }
}
void Server::SetWinner(int id){
    if(get_tree()->get_network_unique_id() == id){
        nodes->emit_signal("IsWinner", true);
    }else{
        nodes->emit_signal("IsWinner", false);
    }
}
void Server::RequestWinner(int id, int nbrCoin){
    if(isServer){
        if(winnerCoin < nbrCoin){
            winnerCoin = nbrCoin;
            winnerID = id;
        }
        if(get_tree()->get_network_connected_peers().size() < ++nbrSender){
            SetWinner(winnerID);
            rpc("SetWinner", winnerID);
        }
    }else{
        rpc_id(1, "RequestWinner", id, nbrCoin);
    }
}
void Server::InstanceCoin(String coinName, Vector3 location){
    if(nodes->has_node(static_cast<NodePath>(coinName)))
        nodes->get_node(static_cast<NodePath>(coinName))->queue_free();
    String scenePath = String("res://Game/Level/Coin/Coin.tscn");
    Node* coin = global->call("LoadAndInstance", scenePath, nodes, location);
    coin->set_name(coinName);
}
void Server::RequestCoin(int i, Vector3 location){
    if(isServer){
        InstanceCoin(String("Coin_") + String::num_int64(i), location);
        rpc("InstanceCoin", String("Coin_") + String::num_int64(i), location);
    }else{
        rpc_id(1, "RequestCoin", i, location);
    }
}
void Server::NextLevel(){
    global->call("NextLevel");
    Spatial* player = Object::cast_to<Spatial>(nodes->get_node(static_cast<NodePath>(String::num_int64(get_tree()->get_network_unique_id()))));
    player->set_global_translation(ChooseSpawnLocation());
}
void Server::RequestLevel(){
    if(isServer){
        if(has_node("Broadcast")){
            broadcaster->stop();
            UDP->close();
            broadcaster->queue_free();
        }
        rpc("NextLevel");
        NextLevel();
    }else{
        rpc_id(1, "RequestLevel");
    }
}
void Server::InstancePlayer(int playerID, Vector3 location, int index){
    const bool isLocalIP = playerID == get_tree()->get_network_unique_id();
    String scenePath = isLocalIP ? String("res://Game/Machine/Player/Player.tscn") : String("res://Game/Machine/OtherPlayer/OtherPlayer.tscn");
    Node* player = global->call("LoadAndInstance", scenePath, nodes, location);
    player->set_name(String::num_int64(playerID));
    player->call("SetIndex", index);
    if(isLocalIP){
        if(!isServer){
            RequestOtherPlayer();
            RequestFirstCoin();
        }
    }
}
void Server::RequestFirstCoin(){
    if(isServer){
        const int senderID = static_cast<int>(get_tree()->get_rpc_sender_id());
        Array coins = get_tree()->get_nodes_in_group("Coin");
        for (int i = 0; i < coins.size(); i++) {
            String peerStr = coins[i].operator String();
            String coin = peerStr.split(":")[0];
            Spatial* coinNode = Object::cast_to<Spatial>(nodes->get_node(static_cast<NodePath>(coin)));
            Vector3 location = coinNode ? coinNode->get_global_translation() : Vector3();
            rpc_id(senderID, "InstanceCoin", coin, location);
        }
    }else{
        rpc_id(1, "RequestFirstCoin");
    }
}
void Server::ReLoc(){
    Spatial* player = Object::cast_to<Spatial>(nodes->get_node(static_cast<NodePath>(String::num_int64(get_tree()->get_network_unique_id()))));
    player->set_global_translation(ChooseSpawnLocation());
}
void Server::OnGetCoin(int nbrCoin){
    winnerCoin = nbrCoin;
}
void Server::OnEnd(){
    const int myID = static_cast<int>(get_tree()->get_network_unique_id());
    RequestWinner(myID, winnerCoin);
}
void Server::RequestPlayer(int playerID, int index){
    Vector3 location = ChooseSpawnLocation();
    if(isServer){
        InstancePlayer(playerID, location, index);
        rpc("InstancePlayer", playerID, location, index);
    }else{
        rpc_id(1, "RequestPlayer", playerID, index);
    }
}
void Server::InstanceOtherPlayer(int playerID, Vector3 location, int index){
    if(nodes->has_node(static_cast<NodePath>(String::num_int64(playerID))))
        return;
    String scenePath = String("res://Game/Machine/OtherPlayer/OtherPlayer.tscn");
    Node* player = global->call("LoadAndInstance", scenePath, nodes, location);
    player->set_name(String::num_int64(playerID));
    player->call("SetIndex", index);
}
void Server::RequestOtherPlayer(){
    if(isServer){
        const int senderID = static_cast<int>(get_tree()->get_rpc_sender_id());
        Array peers = get_tree()->get_nodes_in_group("Personne");
        for (int i = 0; i < peers.size(); i++) {
            String peerStr = peers[i].operator String();
            int peerID = static_cast<int>(peerStr.split(":")[0].to_int());
            if (peerID != senderID) {
                if(nodes->has_node(static_cast<NodePath>(String::num_int64(peerID)))){
                    Spatial* player = Object::cast_to<Spatial>(nodes->get_node(static_cast<NodePath>(String::num_int64(peerID))));
                    Vector3 location = player ? player->get_global_translation() : Vector3();
                    int index = player ? static_cast<int>(player->call("GetIndex")) : 2;
                    rpc_id(senderID, "InstanceOtherPlayer", peerID, location, index);
                }
            }
        }
    }else{
        rpc_id(1, "RequestOtherPlayer");
    }
}
Vector3 Server::ChooseSpawnLocation(){
    Ref<RandomNumberGenerator> rng = RandomNumberGenerator::_new();
    rng->randomize();
    int point = rng->randi() % spawnPoint.size();
    Vector3 loc = spawnPoint[point];
    takenPoint.append(loc);
    spawnPoint.remove(point);
    if (spawnPoint.size() <= 0) {
        spawnPoint = spawnPointClone.duplicate();
        takenPoint.clear();
    }
    return loc;
}
void Server::UpdatePlayer(int id, int newState, Vector3 rotationDeg, Vector3 translationDeg){
    if(nodes->has_node(static_cast<NodePath>(String::num_int64(id))) && (id != get_tree()->get_network_unique_id())){
        Node* player = Object::cast_to<Node>(nodes->get_node(static_cast<NodePath>(String::num_int64(id))));
        player->call("ChangeState", newState, rotationDeg, translationDeg);
    }
}
void Server::RequestUpdate(int senderID, int newState, Vector3 rotationDeg, Vector3 translationDeg){
    if(isServer){
        rpc("UpdatePlayer", senderID, newState, rotationDeg, translationDeg);
        UpdatePlayer(senderID, newState, rotationDeg, translationDeg);
    }else{
        rpc_id(1, "RequestUpdate", senderID, newState, rotationDeg, translationDeg);
    }
}
void Server::DeleteObj(String name){
    if(nodes->has_node(static_cast<NodePath>(name)))
		nodes->get_node(static_cast<NodePath>(name))->queue_free();
}
void Server::RequestDelete(String name){
    if(isServer){
        DeleteObj(name);
        rpc("DeleteObj", name);
    }else{
        rpc_id(1, "RequestDelete", name);
    }
}
void Server::StartServer() {
    if (Peer.is_valid() || isConnected) {
        return;
    }

    Peer.instance();
    Error err = Peer->create_server(SERVER_PORT, MAX_PLAYER);
    if (err != Error::OK) {
        Peer = Ref<NetworkedMultiplayerENet>(nullptr);
        return;
    }

    get_tree()->set_network_peer(Peer);
    isConnected = true;
    get_tree()->connect("network_peer_connected", this, "OnPlayerConnected");
    get_tree()->connect("network_peer_disconnected", this, "OnPlayerDisconnected");
    
    UDP.instance();
    UDP->set_dest_address(MULTICAST_GROUP, UDP_PORT);

    broadcaster->connect("timeout", this, "OnBroadcastTimeout");
    broadcaster->start();
    
    RequestPlayer(static_cast<int>(get_tree()->get_network_unique_id()), indexPlayer);
}

void Server::JoinServer() {
    if (Peer.is_valid() || isConnected) {
        return;
    }
    Peer.instance();
    Error error = Peer->create_client(serverIP, SERVER_PORT);
    get_tree()->set_network_peer(Peer);
    if(error != Error::OK){
        Peer = Ref<NetworkedMultiplayerENet>(nullptr);
        return;
    }
    SceneTree* tree = Object::cast_to<SceneTree>(get_tree());
    tree->connect("connected_to_server", this, "OnConnected");
    tree->connect("connection_failed", this, "OnConnectionFaild");
    tree->connect("server_disconnected", this, "OnDisconnected");
    isConnected = true;
    broadcaster->queue_free();
}

void Server::Die() {
    if (!(Peer.is_valid() || isConnected)) {
        return;
    }
    if(nodes->is_connected("Die", this, "ReLoc"))
        nodes->disconnect("Die", this, "ReLoc");
    if(nodes->is_connected("ChangeState", this, "OnChangeState"))
        nodes->disconnect("ChangeState", this, "OnChangeState");
    get_tree()->call_group("Personne", "queue_free");
    get_tree()->call_group("Coin", "queue_free");
    get_tree()->set_network_peer(nullptr);
    queue_free();
}

void Server::OnStateServer(int newState){
    if(state == newState)
        return;
    switch(newState){
        case HOSTER:
            if(!isConnected){
                StartServer();
            }
        break;
        case JOINER:
            if(!isConnected){
                JoinServer();
            }
        break;
        case DIE:
                Die();
    }
    state = newState;
}
void Server::OnBroadcastTimeout(){
    String message = MOT_MAGIC + ":";
    PoolByteArray bytes = PoolByteArray();
    for (int i = 0;i < message.length(); i++) {
        bytes.append((uint8_t)message[i]);
    }
    for(int i = 0; i < m4; i++){
        Error err = UDP->put_packet(bytes);
        if(err != Error::OK)
            break;
    }
}
void Server::OnChangeState(int newState, Vector3 rotationDeg, Vector3 translationDeg)
{
    const int id = static_cast<int>(get_tree()->get_network_unique_id());
    RequestUpdate(id, newState, rotationDeg, translationDeg);
}

void Server::OnPlayerConnected(int id) {
    if(get_tree()->get_network_connected_peers().size() >= MAX_PLAYER){
        broadcaster->stop();
        UDP->close();
    }
}
void Server::OnPlayerDisconnected(int id) {
    RequestDelete(String::num_int64(id));
}

void Server::OnConnected(){
    isConnected = true;
    RequestPlayer(static_cast<int>(get_tree()->get_network_unique_id()), indexPlayer);
}
void Server::OnDisconnected(){
    RespawnPlayer();
    isConnected = false;
}
void Server::OnConectionFaild(){
    OnStateServer(DIE);
}
void Server::OnServerIP(String ip, int index){
    serverIP = ip;
    indexPlayer = index;
    OnStateServer(JOINER);
}
void Server::OnHoster(int index){
    isServer = true;
    indexPlayer = index;
    OnStateServer(HOSTER);
}
