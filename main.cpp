#include "../include/common.h"
#include "../include/Game.h"


int main() {
    Game *game = new Game(true);


    game->run();


    return 0;
}