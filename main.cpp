#include "checkers.h"

int main()
{
    checkers game;

//    game.reset_board();
//    game.print_board();
    game.set_board();
    game.print_board();
    game.update_avail_moves(2);
    game.print_avail_moves(2);

    return 0;
}