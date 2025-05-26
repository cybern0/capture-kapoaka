#ifndef PLAYER_H
#define PLAYER_H

#include <Personne.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <GlobalConstants.hpp>
#include <Label.hpp>

namespace godot
{
    class Player: public Personne
    {
        GODOT_CLASS(Player, KinematicBody);
    private:
        Label* label;
        int nbrCoin;
    public:
        static void _register_methods();
        Player();
        ~Player();
        void _init();
        void _ready();
        void _physics_process(float);
        void GiveGold();
        void IsWinner(bool);
        void SetIndex(int);
        int GetIndex();
    };
}

#endif