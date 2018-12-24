#ifndef CHECKERS_H
#define CHECKERS_H

#include <vector>
#include <iostream>
#include <stack>
#include <set>

using namespace std;

struct coord
{
    short x;
    short y;
};

struct moves
{
    coord start;
    coord end;
    vector<coord> jumped_pieces;
};

struct board
{
    vector<vector<char> > m_board;
    board() : m_board(8,vector<char>(8,' ')) {}
    char get_piece(coord loc) {return m_board[loc.y][loc.x];};
    void set_piece(coord loc, char character) {m_board[loc.y][loc.x] = character;}
    void set_row(short row_num, vector<char> row) {m_board[row_num] = row;}
};

enum direction {up = -1, down = 1};

class Checkers
{
    private:
        board m_board;
        char m_player_char[2];
        //char m_player1_char, m_player2_char;
        char m_cur_player;
        vector<moves> m_p1_avail_moves;
        vector<moves> m_p2_avail_moves;
    public:
        
        void reset_board();
        void set_board();
        void print_board();
        vector<moves> valid_moves(board m_board, coord piece, direction direc);
        void update_avail_moves(int player_num);
        void print_avail_moves(int player_num);
        bool is_on_board(coord loc);
        bool is_freespace(coord loc);
        bool is_jumpable(coord start_piece, coord over_piece);
};

#endif