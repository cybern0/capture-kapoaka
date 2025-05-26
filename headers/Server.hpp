#ifndef SERVER_H
#define SERVER_H

#include <Godot.hpp>
#include <Node.hpp>
#include <NetworkedMultiplayerENet.hpp>
#include <SceneTree.hpp>
#include <PacketPeerUDP.hpp>
#include <UDPServer.hpp>
#include <Timer.hpp>
#include <IP.hpp>
#include <Spatial.hpp>
#include <RandomNumberGenerator.hpp>

namespace godot {

class Server : public Node {
    GODOT_CLASS(Server, Node)

private:
    enum {
        HOSTER = 10,
        JOINER,
        DIE
    };
    Ref<NetworkedMultiplayerENet> Peer;
    Ref<PacketPeerUDP> UDP;
    Node* nodes;
    Node* global;
    const int UDP_PORT = 54321;
    const int SERVER_PORT = 4242;
    const int MAX_PLAYER = 7;
    const String MOT_MAGIC = "gl0bal";
    const String MULTICAST_GROUP =  "224.1.1.1";//239.0.0.1
    const int m4 = 10;
    String serverIP;
    bool isConnected;
    bool isServer;
    int state;
    int nbrSender;
    int winnerCoin;
    int winnerID;
    int indexPlayer;
    Timer* broadcaster;
    Array spawnPoint;
    Array spawnPointClone;
    Array takenPoint;
public:
    static void _register_methods();
    Server();
    ~Server();
    void _init();
    void _ready();
    void RespawnPlayer();
    void ExitTree();
    void IsNext();
    void ReLoc();
    void OnGetCoin(int);
    void OnEnd();

    void SetWinner(int);
    void RequestWinner(int, int);

    void InstanceCoin(String, Vector3);
    void RequestCoin(int, Vector3);
    void RequestFirstCoin();

    void NextLevel();
    void RequestLevel();

    void InstancePlayer(int, Vector3, int);
    void RequestPlayer(int, int);

    void InstanceOtherPlayer(int, Vector3, int);
    void RequestOtherPlayer();

    Vector3 ChooseSpawnLocation();

    void DeleteObj(String);
    void RequestDelete(String);

    void UpdatePlayer(int,int, Vector3, Vector3);
    void RequestUpdate(int,int, Vector3, Vector3);

    void StartServer();
    void JoinServer();
    void Die();

    void OnChangeState(int, Vector3, Vector3);

    void OnBroadcastTimeout();
    void OnStateServer(int);

    void OnConnected();
    void OnDisconnected();
    void OnConectionFaild();

    void OnServerIP(String, int);
    void OnHoster(int);

    void OnPlayerConnected(int);
    void OnPlayerDisconnected(int);
};

}

#endif