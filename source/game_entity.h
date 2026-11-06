#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QTimer>
#include <QVector>
#include <QPair>


/* Predefine the game entity */
class Game;


/* Base class of pacman and ghosts */
class GameEntity : public QGraphicsPixmapItem
{
public:
    enum EntityType {
        Ball,
        Wall,
        Pacman,
        Ghost,
        Blank,
        Cage,
        Gate,
        PowerPellet
    };

    enum Direction {
        Up = 0,
        Down = 1,
        Left = 2,
        Right = 3,
        Stop = 4
    };

    /* game entity size by pixel */
    static const int Width = 20;

    GameEntity(EntityType, QPixmap);
    EntityType getType();
    int get_x();
    int get_y();
    int getScore();
    void setScore(int);
    Direction getDir();
    void setDir(Direction);
    Direction getNextDir();
    void setNextDir(Direction);
    ~GameEntity();

    friend class Game;
    friend class Ghost;

protected:
    int score;
    int _x, _y;         // coordinate in map
    Direction dir;      // current moving direction
    Direction nextDir;  // next direction by W, A, S, D keys or Up, Left, Down, Right keys
    EntityType type;
};

#endif // GAME_ENTITY_H
