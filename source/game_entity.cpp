#include "game_entity.h"

GameEntity::GameEntity(EntityType t, QPixmap pixmap) : QGraphicsPixmapItem(pixmap)
{
    type = t;
}

GameEntity::EntityType GameEntity::getType()
{
    return type;
}

int GameEntity::get_x()
{
    return _x;
}

int GameEntity::get_y()
{
    return _y;
}

int GameEntity::getScore()
{
    return score;
}

void GameEntity::setScore(int s)
{
    score = s;
}

GameEntity::Direction GameEntity::getDir()
{
    return dir;
}

void GameEntity::setDir(Direction d)
{
    dir = d;
}

GameEntity::Direction GameEntity::getNextDir()
{
    return nextDir;
}

void GameEntity::setNextDir(Direction d)
{
    nextDir = d;
}

GameEntity::~GameEntity()
{
}
