#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <Godot.hpp>
#include <CanvasLayer.hpp>
#include <Button.hpp>
#include <SceneTree.hpp>
#include <Input.hpp>

namespace godot {

class GameOver : public CanvasLayer {
    GODOT_CLASS(GameOver, CanvasLayer);
private:
    Button* exit;
    Button* restart;
    Node* nodes;
public:
    static void _register_methods();

    GameOver();
    ~GameOver();

    void _init();
    void _ready();
    void OnExitPressed();
    void OnRespawnPressed();
};

}

#endif
