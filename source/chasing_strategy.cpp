#include "game.h"
#include "ghost.h"

QPair<int, int> strategy1(Ghost* ghost) {
    Pacman* pacman = ghost->game->pacman;
    int dist_x = pacman->get_x() - ghost->get_x();
    int dist_y = pacman->get_y() - ghost->get_y();
    return QPair<int, int>(dist_x, dist_y);
}

QPair<int, int> strategy2(Ghost* ghost) {
    Pacman* pacman = ghost->game->pacman;
    int dist_x = pacman->get_x() - ghost->get_x();
    int dist_y = pacman->get_y() - ghost->get_y();
    switch(pacman->getDir()) {
    case GameEntity::Up:
        dist_y -= 10;
        break;
    case GameEntity::Down:
        dist_y += 10;
        break;
    case GameEntity::Left:
        dist_x -= 10;
        break;
    case GameEntity::Right:
        dist_x += 10;
        break;
    default:
        break;
    }
    return QPair<int, int>(dist_x, dist_y);
}

QPair<int, int> strategy3(Ghost* ghost) {
    Pacman* pacman = ghost->game->pacman;
    Ghost* redGhost = ghost->game->ghost[Ghost::Blinky];
    int dist_x = (pacman->get_x() - redGhost->get_x()) - ghost->get_x();
    int dist_y = (pacman->get_y() - redGhost->get_y()) - ghost->get_y();
    return QPair<int, int>(dist_x, dist_y);
}

QPair<int, int> strategy4(Ghost* ghost) {
    Pacman* pacman = ghost->game->pacman;
    int dist_x = pacman->get_x() - ghost->get_x();
    int dist_y = pacman->get_y() - ghost->get_y();
    if ((dist_x ^ 2) + (dist_y ^ 2) < 100) {
        dist_x = 2 - ghost->get_x();
        dist_y = 2 - ghost->get_y();
    }
    return QPair<int, int>(dist_x, dist_y);
}
