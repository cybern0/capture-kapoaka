#ifndef LEVEL_H
#define LEVEL_H

#include <Godot.hpp>
#include <Spatial.hpp>
#include <SceneTree.hpp>
#include <RandomNumberGenerator.hpp>
#include <AudioStreamPlayer.hpp>

namespace godot
{
    class Level : public Spatial
    {
        GODOT_CLASS(Level, Spatial);
    private:
        float tmp;
        Node* nodes;
        int nbrCoin;
        Array spawnCoin;
        AudioStreamPlayer* audio;
    public:
        static void _register_methods();
        Level();
        ~Level();
        void _init();
        void _ready();
        Vector3 ChooseCoinLocation();
        void OnAudioFinished();
    };
}

#endif