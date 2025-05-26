#ifndef GLOBAL_H
#define GLOBAL_H

#include <Godot.hpp>
#include <Spatial.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include <PackedScene.hpp>
#include <RandomNumberGenerator.hpp>

namespace godot
{
    class Global : public Node
    {
        GODOT_CLASS(Global, Node);
    private:
        PoolStringArray levels;
        String endScreen;
        String startScreen;
        int currentLevel;
    public:
        static void _register_methods();
        Global();
        ~Global();
        void _init();
        void _ready();
        void MainMenu();
        void NewGame();
        void GameOver();
        void NextLevel();
        Node* LoadAndInstance(String, Node*, Vector3);
    };
}

#endif