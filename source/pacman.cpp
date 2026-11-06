#include "game.h"
#include "pacman.h"
#define W (GameEntity::Width)

Pacman::Pacman() : GameEntity(GameEntity::Pacman, QPixmap(":/game_objects/pacman/a3.png"))
{
    dir = Right;
    nextDir = Right;
    animIndex = 2;
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a1.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a2.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a3.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a4.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a5.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a6.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a5.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a4.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a3.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a2.png"));

    anim[Up].push_back(QPixmap(":/game_objects/pacman/a1.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b2.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b3.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b4.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b5.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b6.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b5.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b4.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b3.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b2.png"));

    anim[Left].push_back(QPixmap(":/game_objects/pacman/a1.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c2.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c3.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c4.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c5.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c6.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c5.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c4.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c3.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c2.png"));

    anim[Down].push_back(QPixmap(":/game_objects/pacman/a1.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d2.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d3.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d4.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d5.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d6.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d5.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d4.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d3.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d2.png"));
}

void Pacman::moveUp()
{
    animIndex++;
    if (animIndex == anim[Up].size()) {
        animIndex = 0;
    }
    setPixmap(anim[Up][animIndex]);
    setY(static_cast<int>(y()) - 1);
}

void Pacman::moveLeft()
{
    animIndex++;
    if (animIndex == anim[Left].size()) {
        animIndex = 0;
    }
    setPixmap(anim[Left][animIndex]);
    setX(static_cast<int>(x()) - 1);
}

void Pacman::moveDown()
{
    animIndex++;
    if (animIndex == anim[Down].size()) {
        animIndex = 0;
    }
    setPixmap(anim[Down][animIndex]);
    setY(static_cast<int>(y()) + 1);
}

void Pacman::moveRight()
{
    animIndex++;
    if (animIndex == anim[Right].size()) {
        animIndex = 0;
    }
    setPixmap(anim[Right][animIndex]);
    setX(static_cast<int>(x()) + 1);
}


void Pacman::eatBall(int __y, int __x)
{
    GameEntity* obj = game->map[__y][__x];
    switch (obj->getType()) {
    case Ball:
        game->score += obj->getScore();
        game->ballNum--;
        break;
    case PowerPellet:
        game->score += obj->getScore();
        game->ballNum--;
        for (int i = 0; i < game->powerpellet.size(); i++) {
            if (game->powerpellet.at(i) == obj) {
                game->powerpellet.remove(i);
                break;
            }
        }

        for (int i = 0; i < Ghost::GhostNum; i++) {
            if (game->ghost[i]->status != Ghost::Running) {
                game->ghost[i]->status = Ghost::Panic;
                game->ghost[i]->panicTime = PANNIC_TIME;
                game->ghostTimer[i]->setInterval(PANNIC_INTERVAL);
            }
        }
        break;
    default:
        return;
    }

    /* Pacman eat a ball and fill the previous block with blank */
    QPixmap blankpix;
    game->map[_y][_x] = new GameEntity(GameEntity::Blank, blankpix);
    game->map[__y][__x] = this;
    delete obj;
}


bool Pacman::overlapable(int i, int j)
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
        return false;
    default:
        return true;
    }
}


void Pacman::move()
{
    int pacman_x = static_cast<int>(x());
    int pacman_y = static_cast<int>(y());
    int __x = (pacman_x - game->geo_x) / W;            // x coordinate in map
    int __y = (pacman_y - game->geo_y) / W;            // y coordinate in map
    int x_remainder = (pacman_x - game->geo_x) % W;    // remainder x pixel to fit a block
    int y_remainder = (pacman_y - game->geo_y) % W;    // remainder y pixel to fit a block
    Direction nextDir = getNextDir();

    /* When pacman completely fits a block, decide whether to change direction */
    if (x_remainder == 0) {
        if (y_remainder == 0) {
            // When pacman completely fits a block, test if this block is a ball to eat
            // Win if all balls are eaten
            eatBall(__y, __x);

            // update pacman's coordinate in map
            _x = __x;
            _y = __y;

            if (game->ballNum == 0) {
                game->stat = Game::Win;
                return;
            }
        }

        switch (nextDir) {
        case Stop:
            dir = nextDir;
            break;
        case Up:        // test if it can turn upward
            if (overlapable(_y - 1, _x)) {
                dir = nextDir;
            }
            break;
        case Down:      // test if it can turn downward
            if (overlapable(_y + 1, _x)) {
                dir = nextDir;
            }
            break;
        case Left:      // test if it can turn left
            if (y_remainder == 0 && overlapable(_y, _x - 1)) {
                dir = nextDir;
            }
            break;
        case Right:      // test if it can turn right
            if (y_remainder == 0 && overlapable(_y, _x + 1)) {
                dir = nextDir;
            }
            break;
        }
    }
    else if (y_remainder == 0) {
        switch (nextDir) {
        case Stop:
            dir = nextDir;
            break;
        case Left:      // test if it can turn left
            if (overlapable(_y, _x - 1)) {
                dir = nextDir;
            }
            break;
        case Right:     // test if it can turn right
            if (overlapable(_y, _x + 1)) {
                dir = nextDir;
            }
            break;
        default:        // decide when x_remainder = 0
            break;
        }
    }

    /* Pacman keeps moving or stops when hitting wall */
    switch (dir) {
    case Stop:
        break;
    case Up:
        if (y_remainder == 0 && !overlapable(_y - 1, _x)) {
            dir = Stop;
        }
        else {
            moveUp();
        }
        break;
    case Down:
        if (y_remainder == 0 && !overlapable(_y + 1, _x)) {
            dir = Stop;
        }
        else {
            moveDown();
        }
        break;
    case Left:
        if (x_remainder == 0 && !overlapable(_y, _x - 1)) {
            dir = Stop;
        }
        else {
            moveLeft();
        }
        break;
    case Right:
        if (x_remainder == 0 && !overlapable(_y, _x + 1)) {
            dir = Stop;
        }
        else {
            moveRight();
        }
        break;
    }
}
