#ifndef PACMAN_H
#define PACMAN_H

#include "game_entity.h"

class Pacman : public GameEntity
{
public:
    Game* game;     // the pacman game entity
    Pacman();
    void move();

    friend class Game;

private:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void eatBall(int, int);
    bool overlapable(int, int); // check if pacman can go to map[i][j]

    QVector<QPixmap> anim[4];   // animations
    int animIndex;
};

#endif // PACMAN_H
