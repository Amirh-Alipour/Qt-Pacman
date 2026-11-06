#include <QRandomGenerator>
#include "game.h"
#include "chasing_strategy.cpp"
#define W (GameEntity::Width)

Ghost::Ghost(int clr) : GameEntity(GameEntity::Ghost, QPixmap())
{
    character = (Character)clr;
    animIndex = 0;
    isReleased = false;
    dir = Direction(QRandomGenerator::global()->generate() % 4);
    status = Normal;

    switch (character) {
    case Blinky:
        animation[Right].push_back(QPixmap(":/game_objects/ghosts/redright1.png"));
        animation[Right].push_back(QPixmap(":/game_objects/ghosts/redright2.png"));
        animation[Up].push_back(QPixmap(":/game_objects/ghosts/redup1.png"));
        animation[Up].push_back(QPixmap(":/game_objects/ghosts/redup2.png"));
        animation[Left].push_back(QPixmap(":/game_objects/ghosts/redleft1.png"));
        animation[Left].push_back(QPixmap(":/game_objects/ghosts/redleft2.png"));
        animation[Down].push_back(QPixmap(":/game_objects/ghosts/reddown1.png"));
        animation[Down].push_back(QPixmap(":/game_objects/ghosts/reddown2.png"));
        break;
    case Pinky:
        animation[Right].push_back(QPixmap(":/game_objects/ghosts/pinkright1.png"));
        animation[Right].push_back(QPixmap(":/game_objects/ghosts/pinkright2.png"));
        animation[Up].push_back(QPixmap(":/game_objects/ghosts/pinkup1.png"));
        animation[Up].push_back(QPixmap(":/game_objects/ghosts/pinkup2.png"));
        animation[Left].push_back(QPixmap(":/game_objects/ghosts/pinkleft1.png"));
        animation[Left].push_back(QPixmap(":/game_objects/ghosts/pinkleft2.png"));
        animation[Down].push_back(QPixmap(":/game_objects/ghosts/pinkdown1.png"));
        animation[Down].push_back(QPixmap(":/game_objects/ghosts/pinkdown2.png"));
        break;
    case Inky:
        animation[Right].push_back(QPixmap(":/game_objects/ghosts/greenright1.png"));
        animation[Right].push_back(QPixmap(":/game_objects/ghosts/greenright2.png"));
        animation[Up].push_back(QPixmap(":/game_objects/ghosts/greenup1.png"));
        animation[Up].push_back(QPixmap(":/game_objects/ghosts/greenup2.png"));
        animation[Left].push_back(QPixmap(":/game_objects/ghosts/greenleft1.png"));
        animation[Left].push_back(QPixmap(":/game_objects/ghosts/greenleft2.png"));
        animation[Down].push_back(QPixmap(":/game_objects/ghosts/greendown1.png"));
        animation[Down].push_back(QPixmap(":/game_objects/ghosts/greendown2.png"));
        break;
    case Clyde:
        animation[Right].push_back(QPixmap(":/game_objects/ghosts/orangeright1.png"));
        animation[Right].push_back(QPixmap(":/game_objects/ghosts/orangeright2.png"));
        animation[Up].push_back(QPixmap(":/game_objects/ghosts/orangeup1.png"));
        animation[Up].push_back(QPixmap(":/game_objects/ghosts/orangeup2.png"));
        animation[Left].push_back(QPixmap(":/game_objects/ghosts/orangeleft1.png"));
        animation[Left].push_back(QPixmap(":/game_objects/ghosts/orangeleft2.png"));
        animation[Down].push_back(QPixmap(":/game_objects/ghosts/orangedown1.png"));
        animation[Down].push_back(QPixmap(":/game_objects/ghosts/orangedown2.png"));
        break;
    }

    panicAnimation.push_back(QPixmap(":/game_objects/ghosts/pannic1.png"));
    panicAnimation.push_back(QPixmap(":/game_objects/ghosts/pannic2.png"));
    runningAnimation.push_back(QPixmap(":/game_objects/ghosts/run.png"));
}


Ghost::Character Ghost::getCharacter()
{
    return character;
}


void Ghost::moveUp()
{
    QVector<QPixmap>* ptr;
    switch (status) {
    case Normal:
        ptr = &animation[Up];
        break;
    case Panic:
        ptr = &panicAnimation;
        break;
    case Running:
        ptr = &runningAnimation;
        break;
    }

    animIndex++;
    if (animIndex >= ptr->size()) {
        animIndex = 0;
    }
    setPixmap((*ptr)[animIndex]);
    setY(static_cast<int>(y()) - 1);
}

void Ghost::moveDown()
{
    QVector<QPixmap>* ptr;
    switch (status) {
    case Normal:
        ptr = &animation[Down];
        break;
    case Panic:
        ptr = &panicAnimation;
        break;
    case Running:
        ptr = &runningAnimation;
        break;
    }

    animIndex++;
    if (animIndex >= ptr->size()) {
        animIndex = 0;
    }
    setPixmap((*ptr)[animIndex]);
    setY(static_cast<int>(y()) + 1);
}

void Ghost::moveLeft()
{
    QVector<QPixmap>* ptr;
    switch (status) {
    case Normal:
        ptr = &animation[Left];
        break;
    case Panic:
        ptr = &panicAnimation;
        break;
    case Running:
        ptr = &runningAnimation;
        break;
    }

    animIndex++;
    if (animIndex >= ptr->size()) {
        animIndex = 0;
    }
    setPixmap((*ptr)[animIndex]);
    setX(static_cast<int>(x()) - 1);
}

void Ghost::moveRight()
{
    QVector<QPixmap>* ptr;
    switch (status) {
    case Normal:
        ptr = &animation[Right];
        break;
    case Panic:
        ptr = &panicAnimation;
        break;
    case Running:
        ptr = &runningAnimation;
        break;
    }

    animIndex++;
    if (animIndex >= ptr->size()) {
        animIndex = 0;
    }
    setPixmap((*ptr)[animIndex]);
    setX(static_cast<int>(x()) + 1);
}


bool Ghost::overlapable(int i, int j)
{
    if (i < 0 || j < 0) {
        return false;
    }

    if (i >= game->mapHeight || j >= game->mapWidth) {
        return false;
    }

    switch (game->map[i][j]->getType()) {
    case Wall:
        return false;
    case Gate:
        if (isReleased == false && releaseTime == 0) {
            return true;
        }
        else if (status == Running) {
            return true;
        }
        else {
            return false;
        }
    default:
        return true;
    }
}

void Ghost::setDirectionRandomly()
{
    QVector<Direction> dirVector;
    if (overlapable(_y, _x + 1)) {
        dirVector.push_back(Right);
    }
    if (overlapable(_y, _x - 1)) {
        dirVector.push_back(Left);
    }
    if (overlapable(_y + 1, _x)) {
        dirVector.push_back(Down);
    }
    if (overlapable(_y - 1, _x)) {
        dirVector.push_back(Up);
    }
    dir = dirVector.at(QRandomGenerator::global()->generate() % dirVector.size());
}


void Ghost::goOutOfCage()
{
    int xDistToGate = game->gate->_x - _x;
    int yDistToGate = game->gate->_y - _y;
    if (xDistToGate > 0) {
        setDir(GameEntity::Right);
    }
    else if (xDistToGate < 0) {
        setDir(GameEntity::Left);
    }
    else {
        if (yDistToGate > 0) {
            setDir(GameEntity::Down);
        }
        else if (yDistToGate < 0) {
            setDir(GameEntity::Up);
        }
    }
}


void Ghost::chasePacman()
{
    // false if the adjacent block is a wall
    bool trueDir[5] = {false, false, false, false, false};
    QVector<Direction> dirVector;
    if (overlapable(_y, _x + 1)) {
        trueDir[Right] = true;
        dirVector.push_back(Right);
    }
    if (overlapable(_y, _x - 1)) {
        trueDir[Left] = true;
        dirVector.push_back(Left);
    }
    if (overlapable(_y + 1, _x)) {
        trueDir[Down] = true;
        dirVector.push_back(Down);
    }
    if (overlapable(_y - 1, _x)) {
        trueDir[Up] = true;
        dirVector.push_back(Up);
    }

    Direction backwardDir;
    switch (dir) {
    case Up:
        backwardDir = Down;
        break;
    case Down:
        backwardDir = Up;
        break;
    case Left:
        backwardDir = Right;
        break;
    case Right:
        backwardDir = Left;
        break;
    case Stop:
        backwardDir = Stop;
        break;
    }

    // Change direction only when ghost is not at an intersection in order to avoid oscillation
    if (dirVector.size() > 2) {
        QPair<int, int> vector = chaseStrategy(this);
        int dist_x = vector.first;
        int dist_y = vector.second;
        if (dist_y > 0) {
            if (trueDir[Down] && dir != Up) {
                dir = Down;
            }
            else {
                if (dist_x >= 0) {
                    if (trueDir[Right]) {
                        dir = Right;
                    }
                    else if (trueDir[Left]) {
                        dir = Left;
                    }
                }
                else {
                    if (trueDir[Left]) {
                        dir = Left;
                    }
                    else if (trueDir[Right]) {
                        dir = Right;
                    }
                }
            }
        }
        else if (dist_y < 0) {
            if (trueDir[Up] && dir != Down) {
                dir = Up;
            }
            else {
                if (dist_x >= 0) {
                    if (trueDir[Right]) {
                        dir = Right;
                    }
                    else if (trueDir[Left]) {
                        dir = Left;
                    }
                }
                else {
                    if (trueDir[Left]) {
                        dir = Left;
                    }
                    else if (trueDir[Right]) {
                        dir = Right;
                    }
                }
            }
        }
        else if (dist_x > 0) {
            if (trueDir[Right] && dir != Left) {
                dir = Right;
            }
            else {
                if (trueDir[Up]) {
                    dir = Up;
                }
                else if(trueDir[Down]) {
                    dir = Down;
                }
            }
        }
        else if (dist_x < 0) {
            if (trueDir[Left] && dir != Right) {
                dir = Left;
            }
            else {
                if (trueDir[Up]) {
                    dir = Up;
                }
                else if(trueDir[Down]) {
                    dir = Down;
                }
            }
        }
    }
    else if (dirVector.size() == 2) {
        if (trueDir[dir] == false) {
            // ghost is on a corner
            if (dirVector[0] == backwardDir) {
                dir = dirVector[1];
            }
            else {
                dir = dirVector[0];
            }
        }
    }
}


void Ghost::dodgePacman()
{
    // false if the adjacent block is a wall
    bool trueDir[5] = {false, false, false, false, false};
    QVector<Direction> dirVector;
    if (overlapable(_y, _x + 1)) {
        trueDir[Right] = true;
        dirVector.push_back(Right);
    }
    if (overlapable(_y, _x - 1)) {
        trueDir[Left] = true;
        dirVector.push_back(Left);
    }
    if (overlapable(_y + 1, _x)) {
        trueDir[Down] = true;
        dirVector.push_back(Down);
    }
    if (overlapable(_y - 1, _x)) {
        trueDir[Up] = true;
        dirVector.push_back(Up);
    }

    Direction backwardDir;
    switch (dir) {
    case Up:
        backwardDir = Down;
        break;
    case Down:
        backwardDir = Up;
        break;
    case Left:
        backwardDir = Right;
        break;
    case Right:
        backwardDir = Left;
        break;
    case Stop:
        backwardDir = Stop;
        break;
    }

    // Change direction only when ghost is not at an intersection in order to avoid oscillation
    if (dirVector.size() > 2) {
        int dist_x = game->pacman->get_x() - _x;
        int dist_y = game->pacman->get_y() - _y;
        if (dist_y < 0) {
            if (trueDir[Down] && dir != Up) {
                dir = Down;
            }
            else {
                if (dist_x >= 0) {
                    if (trueDir[Left]) {
                        dir = Left;
                    }
                    else if (trueDir[Right]) {
                        dir = Right;
                    }
                }
                else {
                    if (trueDir[Right]) {
                        dir = Right;
                    }
                    else if (trueDir[Left]) {
                        dir = Left;
                    }
                }
            }
        }
        else if (dist_y > 0) {
            if (trueDir[Up] && dir != Down) {
                dir = Up;
            }
            else {
                if (dist_x >= 0) {
                    if (trueDir[Left]) {
                        dir = Left;
                    }
                    else if (trueDir[Right]) {
                        dir = Right;
                    }
                }
                else {
                    if (trueDir[Right]) {
                        dir = Right;
                    }
                    else if (trueDir[Left]) {
                        dir = Left;
                    }
                }
            }
        }
        else if (dist_x < 0) {
            if(trueDir[Right] && dir != Left) {
                dir = Right;
            }
            else {
                if (trueDir[Up]) {
                    dir = Up;
                }
                else if(trueDir[Down]) {
                    dir = Down;
                }
            }
        }
        else if (dist_x > 0) {
            if(trueDir[Left] && dir != Right) {
                dir = Left;
            }
            else {
                if (trueDir[Up]) {
                    dir = Up;
                }
                else if(trueDir[Down]) {
                    dir = Down;
                }
            }
        }
    }
    else if (dirVector.size() == 2) {
        if (trueDir[dir] == false) {
            // ghost is on a corner
            if (dirVector[0] == backwardDir) {
                dir = dirVector[1];
            }
            else {
                dir = dirVector[0];
            }
        }
    }
}

void Ghost::goToCage()
{
    bool trueDir[5] = {false, false, false, false, false};
    QVector<Direction> dirVector;
    if (overlapable(_y, _x + 1)) {
        trueDir[Right] = true;
        dirVector.push_back(Right);
    }
    if (overlapable(_y, _x - 1)) {
        trueDir[Left] = true;
        dirVector.push_back(Left);
    }
    if (overlapable(_y + 1, _x)) {
        trueDir[Down] = true;
        dirVector.push_back(Down);
    }
    if (overlapable(_y - 1, _x)) {
        trueDir[Up] = true;
        dirVector.push_back(Up);
    }

    Direction backwardDir;
    switch (dir) {
    case Up:
        backwardDir = Down;
        break;
    case Down:
        backwardDir = Up;
        break;
    case Left:
        backwardDir = Right;
        break;
    case Right:
        backwardDir = Left;
        break;
    case Stop:
        backwardDir = Stop;
        break;
    }

    // Change direction only when ghost is not at an intersection in order to avoid oscillation
    if (dirVector.size() > 2) {
        int dist_x = game->gate->get_x() - _x;
        int dist_y = (game->gate->get_y() + 1) - _y;
        if (dist_y > 0) {
            if (trueDir[Down] && dir != Up) {
                dir = Down;
            }
            else {
                if (dist_x >= 0) {
                    if (trueDir[Right]) {
                        dir = Right;
                    }
                    else if (trueDir[Left]) {
                        dir = Left;
                    }
                }
                else {
                    if (trueDir[Left]) {
                        dir = Left;
                    }
                    else if (trueDir[Right]) {
                        dir = Right;
                    }
                }
            }
        }
        else if (dist_y < 0) {
            if (trueDir[Up] && dir != Down) {
                dir = Up;
            }
            else {
                if (dist_x >= 0) {
                    if (trueDir[Right]) {
                        dir = Right;
                    }
                    else if (trueDir[Left]) {
                        dir = Left;
                    }
                }
                else {
                    if (trueDir[Left]) {
                        dir = Left;
                    }
                    else if (trueDir[Right]) {
                        dir = Right;
                    }
                }
            }
        }
        else if (dist_x > 0) {
            if(trueDir[Right] && dir != Left) {
                dir = Right;
            }
            else {
                if (trueDir[Up]) {
                    dir = Up;
                }
                else if (trueDir[Down]) {
                    dir = Down;
                }
            }
        }
        else if (dist_x < 0) {
            if(trueDir[Left] && dir != Right) {
                dir = Left;
            }
            else {
                if (trueDir[Up]) {
                    dir = Up;
                }
                else if (trueDir[Down]) {
                    dir = Down;
                }
            }
        }
    }
    else if (dirVector.size() == 2) {
        if (trueDir[dir] == false) {
            // ghost is on a corner
            if (dirVector[0] == backwardDir) {
                dir = dirVector[1];
            }
            else {
                dir = dirVector[0];
            }
        }
    }
}


void Ghost::move()
{
    if (status != Running && releaseTime > 0) {
        releaseTime--;
    }
    else if (status == Panic) {
        panicTime--;
        if (panicTime <= 0) {
            status = Normal;
            game->ghostTimer[character]->setInterval(NORMAL_INTERVAL);
        }
    }

    int ghost_x = static_cast<int>(x());
    int ghost_y = static_cast<int>(y());
    int __x = (ghost_x - game->geo_x) / W;           // block x coordinate in map
    int __y = (ghost_y - game->geo_y) / W;           // block y coordinate in map
    int x_remainder = (ghost_x - game->geo_x) % W;   // remainder x pixel to fit a block
    int y_remainder = (ghost_y - game->geo_y) % W;   // remainder y pixel to fit a block

    /* When ghost completely fits a block, decide whether to change direction */
    if (x_remainder == 0 && y_remainder == 0) {
        // update ghost's coordinate in map
        _x = __x;
        _y = __y;
        if (isReleased) {
            // Chase Pacman
            switch (status) {
            case Normal:
                chasePacman();
                break;
            case Panic:
                dodgePacman();
                break;
            default:
                break;
            }
        }
        else {
            if (releaseTime == 0) {
                // Time to go out of the cage
                goOutOfCage();
                if (game->map[_y][_x]->getType() == Gate) {
                    // If arriving at gate, turn into release
                    isReleased = true;
                }
            }
            else {
                if (status == Running) {
                    if (_x == game->gate->get_x() && _y == game->gate->get_y() + 1) {
                        status = Normal;
                        game->ghostTimer[character]->setInterval(NORMAL_INTERVAL);
                    }
                    else {
                        // Go back to the cage
                        goToCage();
                    }
                }
                else {
                    // Keep staying in the cage
                    setDirectionRandomly();
                }
            }
        }
    }

    switch (dir) {
    case Stop:
        break;
    case Up:
        moveUp();
        break;
    case Down:
        moveDown();
        break;
    case Left:
        moveLeft();
        break;
    case Right:
        moveRight();
        break;
    }

    /* Test if pacman hits panic ghosts */
    if (collidesWithItem(game->pacman, Qt::IntersectsItemShape)){
        if (status == Normal) {
            game->stat = Game::Lose;
        }
        else if (status == Panic) {
            setPixmap(QPixmap(":/game_objects/ghosts/run.png"));
            game->score += score;
            status = Running;
            releaseTime = 400;
            isReleased = false;
            game->ghostTimer[character]->setInterval(RUNNING_INTERVAL);
        }
    }
}
