#ifndef NODES_H
#define NODES_H

#include <Godot.hpp>
#include <Node.hpp>

namespace godot
{
    class Nodes: public Node
    {
        GODOT_CLASS(Nodes, Node);
    public:
        static void _register_methods();
        Nodes();
        ~Nodes();
        void _init();
        void _ready();
    };
}

#endif