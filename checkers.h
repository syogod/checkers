#ifndef CHECKERS_H
#define CHECKERS_H

#include <vector>
#include <iostream>
#include <stack>
#include <set>

using namespace std;

struct Coord
{
    short x;
    short y;
};

struct Moves
{
    Coord start;
    Coord end;
    vector<Coord> jumped_pieces;
};

struct Board
{
    vector<vector<char> > m_board;
    Board() : m_board(8,vector<char>(8,' ')) {}
    char get_piece(Coord loc) {return m_board[loc.y][loc.x];};
    void set_piece(Coord loc, char character) {m_board[loc.y][loc.x] = character;}
    void set_row(short row_num, vector<char> row) {m_board[row_num] = row;}
};

struct Token
{
    char base_player_char;
    bool is_king = false;
    short player_num;
    char get_char() 
    {
        if(is_king)
            return (int)base_player_char - 32;
        else
            return base_player_char;
    }
    Coord loc;
};

enum Direction {up = -1, down = 1};

class Checkers
{
    private:
        Board m_board;
        Token m_player_char[2];
        //char m_player1_char, m_player2_char;
        char m_cur_player;
        vector<Moves> m_p1_avail_moves;
        vector<Moves> m_p2_avail_moves;
    public:
        
        void reset_board();
        void set_board();
        void print_board();
        vector<Moves> valid_moves(Board m_board, Token piece, Direction direc);
        void update_avail_moves(int player_num);
        void print_avail_moves(int player_num);
        bool is_on_board(Coord loc);
        bool is_freespace(Coord loc);
        bool is_jumpable(Coord start_piece, Coord over_piece);
};

#endif