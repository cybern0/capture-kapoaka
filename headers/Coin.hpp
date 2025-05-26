#ifndef COIN_H
#define COIN_H

#include <Godot.hpp>
#include <Area.hpp>
#include <SceneTree.hpp>
#include <AudioStreamPlayer3D.hpp>

namespace godot
{
    class Coin : public Area
    {
        GODOT_CLASS(Coin, Area);
    private:
        float rotateSpeed;
        Node* nodes;
        AudioStreamPlayer3D* bip;
    public:
        static void _register_methods();
        Coin();
        ~Coin();
        void _init();
        void _ready();
        void _process(float);
        void OnBodyEntered(Node*);
        void OnBipFinished();
    };
}

#endif