#include "checkers.h"

void checkers::print_board()
{
    cout << "  ";
    for(int i = 1; i < 9; ++i)
    {
        cout << "   " << i;
    }

    cout << endl << "  //===============================\\\\\n";
    for(int i = 0; i < 8; ++i)
    {
        cout << (char)(i + (int)'a') << " |";
        for(int j = 0; j < 8; ++j)
        {
            if((i+j) % 2 == 1)
                cout << "||" << m_board[i][j] << "|";
            else
                cout << "| " << m_board[i][j] << " ";
        }
        if(i != 7)
            cout << "||\n" << "  ||===+===+===+===+===+===+===+===||\n";
        else
            cout << "||\n";
    }
    cout << "  \\\\===============================//" << endl;
}

void checkers::reset_board()
{
    cout << "Player 1, choose your piece (lowercase letters only): " << endl;
    cin >> m_player_char[0]; 


    cout << "Player 2, choose your piece (lowercase letters only): " << endl;
    cin >> m_player_char[1];

    for(int y = 0; y < 8; ++y)
    {
        for(int x = 0; x < 8; ++x)
        {
            if((y+x) % 2 == 1)
            {
                if(y < 3)
                    m_board[y][x] = m_player_char[0];
                else if(y > 4)
                    m_board[y][x] = m_player_char[1];
                else
                    m_board[y][x] = '|';
            }
        }
    }
    m_p1_avail_moves.clear();
    m_p2_avail_moves.clear();
}

void checkers::set_board()
{
    m_player_char[0] = '1';
    m_player_char[1] = '2';
    char p1 = m_player_char[0];
    char p2 = m_player_char[1];
    m_board[0] = {' ','|',' ', p1,' ', p1,' ', p1,' ', p1}; 
    m_board[1] = { p1,' ', p1,' ', p1,' ', p1,' ', p1,' '};
    m_board[2] = {' ', p1,' ', p1,' ', p1,' ', p1,' ', p1}; 
    m_board[3] = {'|',' ','|',' ','|',' ','|',' ','|',' '};
    m_board[4] = {' ','|',' ', p1,' ','|',' ','|',' ','|'};
    m_board[5] = { p2,' ', p2,' ', p2,' ', p2,' ', p2,' '};
    m_board[6] = {' ', p2,' ', p2,' ', p2,' ', p2,' ', p2};
    m_board[7] = { p2,' ', p2,' ', p2,' ', p2,' ', p2,' '};
}

void checkers::update_avail_moves(int player_num)
{
    vector<moves> *avail_moves;
    char player_char;
    char opp_char;

    if(player_num == 1)
    {
        avail_moves = &m_p1_avail_moves;
        player_char = m_player_char[0];
        opp_char = m_player_char[1];
    }
    else
    {
        avail_moves = &m_p2_avail_moves;
        player_char = m_player_char[1];
        opp_char = m_player_char[0];
    }

    avail_moves->clear();
    int player_offset = -1 * ((player_num * 2) - 3);    // +1 (down) if player1, -1 (up) if player2

    //go through each square, if current player, check for avail moves
    for(int y = 0; y < 8; ++y)
    {
        for(int x = 0; x < 8; ++x)
        {
            if(m_board[y][x] == player_char)
            {
                moves temp_move;
                coord potential_loc;
                potential_loc.y = y + player_offset;
                potential_loc.x = x + 1;                //check forward-right move first
                if(is_on_board(potential_loc) && is_freespace(potential_loc))       //free space, valid move
                {
                    temp_move.start.x = x;
                    temp_move.start.y = y;
                    temp_move.end.x = potential_loc.x;
                    temp_move.end.y = potential_loc.y;
                    avail_moves->push_back(temp_move);
                }
                else if(is_on_board(potential_loc) && 
                        m_board[potential_loc.y][potential_loc.x] == player_char) {}    //occupied by own piece, not valid move
                else if(is_on_board(potential_loc) && 
                        m_board[potential_loc.y][potential_loc.x] == opp_char)          //occupied by opponent, check if jumpable
                {
                    potential_loc.x += 1;
                    potential_loc.y += player_offset;
                    if(is_freespace(potential_loc))     //is jumpable
                    {
                    stack<coord> jumps;
                    set<coord> visited;

                        // jumps.push({x,y});
                        // visited.insert({x,y});

                        // while(!jumps.empty())
                        // {
                        //     potential_loc.x = //test
                        // }
                        /* do a depth first graph traversal using a stack 
                        *  starting node put on stack, mark as visited
                        *  go to next unvisited node, put on stack, rinse repeat 
                        *  if no next unvisited node, add as potential move and pop from stack */
                        temp_move.start.x = x;
                        temp_move.start.y = y;
                        temp_move.end.x = potential_loc.x;
                        temp_move.end.y = potential_loc.y;
                        temp_move.jumped_pieces.push_back({(short)(potential_loc.x - 1), (short)(potential_loc.y - player_offset)});
                        avail_moves->push_back(temp_move);
                    }
                }
                potential_loc.x = x - 1;
                potential_loc.y = y + player_offset;
                temp_move.jumped_pieces.clear();

                if(is_on_board(potential_loc) && is_freespace(potential_loc))       //free space, move valid
                {
                    temp_move.start.x = x;
                    temp_move.start.y = y;
                    temp_move.end.x = potential_loc.x;
                    temp_move.end.y = potential_loc.y;
                    avail_moves->push_back(temp_move);
                }
                else if(is_on_board(potential_loc) &&
                        m_board[potential_loc.y][potential_loc.x] == player_char) {}    //occupied by own piece, not valid move
                else if(is_on_board(potential_loc) &&
                        m_board[potential_loc.y][potential_loc.x] == opp_char)          //occupied by opponent, check if jumpable
                {

                    potential_loc.x -= 1;
                    potential_loc.y += player_offset;
                    if(is_freespace(potential_loc))
                    {
                        temp_move.start.x = x;
                        temp_move.start.y = y;
                        temp_move.end.x = potential_loc.x;
                        temp_move.end.y = potential_loc.y;
                        temp_move.jumped_pieces.push_back({(short)(potential_loc.x + 1), (short)(potential_loc.y - player_offset)});
                        avail_moves->push_back(temp_move);
                    }
                }
            }
        }
    }
}

void checkers::print_avail_moves(int player_num)
{
    cout << "\nAvailable Moves\n---------------\n";
    vector<moves> *avail_moves;

    if(player_num == 1)
        avail_moves = &m_p1_avail_moves;
    else
        avail_moves = &m_p2_avail_moves;

    for(vector<moves>::iterator iter = avail_moves->begin(); iter != avail_moves->end(); ++iter)
    {
        cout << iter->start.x + 1 << "," << (char)(iter->start.y + (int)'a');
        cout << " → ";
        cout << iter->end.x + 1 << "," << (char)(iter->end.y + (int)'a');
        if(iter->jumped_pieces.size() > 0)
        {
            cout << "\t jump: ";
            for(int i = 0; i < iter->jumped_pieces.size(); ++i)
            {
                cout << "(" << iter->jumped_pieces[i].x + 1 << ", " << (char)(iter->jumped_pieces[i].y + (int)'a') << ")" ;
            }
        } 
        cout << endl;

    }
}

bool checkers::is_on_board(coord loc)
{
    if(loc.x > 7 || loc.x < 0)
        return false;
    if(loc.y > 7 || loc.y < 0)
        return false;

    return true;
}

bool checkers::is_freespace(coord loc)
{
    if(m_board[loc.y][loc.x] == '|')
        return true;
    else
        return false;
}

bool checkers::is_jumpable(coord start_piece, coord over_piece)
{
    coord end;
    end.x = start_piece.x + 2 * (over_piece.x - start_piece.x);
    end.y = start_piece.y + 2 * (over_piece.y - start_piece.y);
    if(is_on_board(end))
    {
        if(is_freespace(end))
            return true;
    }
    return false;
}