#ifndef PERSONNE_H
#define PERSONNE_H

#include <Godot.hpp>
#include <SceneTree.hpp>
#include <KinematicBody.hpp>
#include <AnimationPlayer.hpp>

namespace godot
{
    class Personne : public KinematicBody
    {
        GODOT_CLASS(Personne, KinematicBody);
    protected:
        enum {
            MOVING_SPEED = 0,
            MOVING_FORWARD,
            MOVING_BACK,
            JUMPING,
            IDLE,
            WINNER,
            LOSER
        };
        Node* nodes;
        Node* global;
		float moveSpeed;
		float jumpForce;
        float gravity;
        Vector3 GravityVector;
        Vector3 vel;
        Vector3 translation;
        Vector3 rotation;
        int state;
        int myIndex;
        String animName;
		AnimationPlayer* moving;
		AnimationPlayer* jumping;
		AnimationPlayer* idle;
		AnimationPlayer* speed;
		AnimationPlayer* win;
		AnimationPlayer* lose;
        PoolStringArray charPlayer;
        Node* mesh;
    public:
        static void _register_methods();
        Personne();
        virtual ~Personne();
        void _init();
        void _ready();
        void _physics_process(float);
        void SetIndex(int);
        int GetIndex();
    };
}

#endif