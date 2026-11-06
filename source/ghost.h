#ifndef GHOST_H
#define GHOST_H

#include <QGraphicsItem>
#include <QVector>
#include <QPair>
#include "game_entity.h"

class Ghost : public GameEntity
{
public:
    enum Character {
        Blinky = 0,    // Red Ghost (The Leader / Chaser)
        Pinky = 1,     // Pink Ghost (Ambusher)
        Inky = 2,      // Green Ghost (Fickle one)
        Clyde = 3      // Orange Ghost (Dumb Enemy)
    };

    enum Status {
        Normal,
        Panic,
        Running
    };

    static const int GhostNum = 4;

    Game* game;    // the pacman game entity
    Ghost(int);
    Character getCharacter();
    void move();

    friend class Game;
    friend class Pacman;

private:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    bool overlapable(int, int);   // check if ghost can go to map[i][j]
    void setDirectionRandomly();
    void goOutOfCage();
    void chasePacman();
    void dodgePacman();
    void goToCage();
    QPair<int, int> (*chaseStrategy)(Ghost*);   // function pointer of chasing strategy

    Character character;
    Status status;

    /* animations */
    QVector<QPixmap> animation[4];
    QVector<QPixmap> panicAnimation;
    QVector<QPixmap> runningAnimation;

    int animIndex;
    int releaseTime;    // time to get out the cage
    bool isReleased;
    int panicTime;
};

/* Different chasing strategies of ghosts, which is represented as a QPair of vector from each ghost to the pacman */
QPair<int, int> strategy1(Ghost*);
QPair<int, int> strategy2(Ghost*);
QPair<int, int> strategy3(Ghost*);
QPair<int, int> strategy4(Ghost*);

#endif // GHOST_H
