#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
class Gvertex
{
public:
    Gvertex(int _x, int _y, sf::CircleShape _dot);
    sf::CircleShape GetShape();
    int x;
    int y;
    bool isActive;
    sf::CircleShape dot;
};
