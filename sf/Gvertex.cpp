#include "Gvertex.h"


Gvertex::Gvertex(int _x, int _y, sf::CircleShape _dot)
{
    x = _x;
    y = _y;
    dot = _dot;
    isActive = true;
}

sf::CircleShape Gvertex::GetShape()
{
    return dot;
}
