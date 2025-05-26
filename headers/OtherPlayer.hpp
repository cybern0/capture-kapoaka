#ifndef OTHER_PLAYER_H
#define OTHER_PLAYER_H

#include <Personne.hpp>
#include <Timer.hpp>
#include <Math.hpp>

namespace godot
{
    class OtherPlayer: public Personne
    {
        GODOT_CLASS(OtherPlayer, KinematicBody);
    private:
        Timer* isLocation;
    public:
        static void _register_methods();
        OtherPlayer();
        ~OtherPlayer();
        void _init();
        void _ready();
        void _physics_process(float);
        void ChangeState(int, Vector3, Vector3);
        void OnIsLocationTimeout();
        void SetIndex(int);
        int GetIndex();
    };
}

#endif