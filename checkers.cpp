#include "checkers.h"

void Checkers::print_board()
{
    cout << "  ";
    for(int i = 1; i < 9; ++i)
    {
        cout << "   " << i;
    }

    cout << endl << "  //===============================\\\\\n";

    Coord temp_coord;
    for(int y = 0; y < 8; ++y)
    {
        temp_coord.y = y;
        cout << (char)(y + (int)'a') << " |";
        for(int x = 0; x < 8; ++x)
        {
            temp_coord.x = x;
            if((y+x) % 2 == 1)
                cout << "||" <<  m_board.get_piece(temp_coord) << "|";
            else
                cout << "| " << m_board.get_piece(temp_coord) << " ";
        }
        if(y != 7)
            cout << "||\n" << "  ||===+===+===+===+===+===+===+===||\n";
        else
            cout << "||\n";
    }
    cout << "  \\\\===============================//" << endl;
}

void Checkers::reset_board()
{
    cout << "Player 1, choose your piece (lowercase letters only): " << endl;
    cin >> m_player_char[0].base_player_char;
    m_player_char[0].player_num = 1;

    cout << "Player 2, choose your piece (lowercase letters only): " << endl;
    cin >> m_player_char[1].base_player_char;
    m_player_char[1].player_num = 2;

    Coord temp_coord;
    for(int y = 0; y < 8; ++y)
    {
        temp_coord.y = y;
        for(int x = 0; x < 8; ++x)
        {
            temp_coord.x = x;
            if((y+x) % 2 == 1)
            {
                if(y < 3)
                    m_board.set_piece(temp_coord, m_player_char[0].base_player_char);
                else if(y > 4)
                    m_board.set_piece(temp_coord, m_player_char[1].base_player_char);
                else
                    m_board.set_piece(temp_coord, '|');
            }
        }
    }
    m_p1_avail_moves.clear();
    m_p2_avail_moves.clear();
}

void Checkers::set_board()
{
    m_player_char[0].base_player_char = 'a';
    m_player_char[1].base_player_char = 'b';
    char p1 = m_player_char[0].base_player_char;
    char p2 = m_player_char[1].base_player_char;
    char kp1 = (int)m_player_char[0].base_player_char - 32;
    char kp2 = (int)m_player_char[1].base_player_char - 32;
    m_board.set_row(0, {' ','|',' ', p1,' ', p1,' ', p1,' ', p1}); 
    m_board.set_row(1, { p1,' ', p1,' ', p1,' ', p1,' ', p1,' '});
    m_board.set_row(2, {' ', p1,' ', p1,' ', p1,' ', p1,' ', p1}); 
    m_board.set_row(3, {'|',' ','|',' ','|',' ','|',' ','|',' '});
    m_board.set_row(4, {' ','|',' ', p1,' ','|',' ','|',' ','|'});
    m_board.set_row(5, { p2,' ',kp2,' ', p2,' ', p2,' ', p2,' '});
    m_board.set_row(6, {' ', p2,' ', p1,' ', p2,' ', p2,' ', p2});
    m_board.set_row(7, { p2,' ', p2,' ', '|',' ', p2,' ', p2,' '});
}


// gets all valid moves for given piece
// if jump is available, use recursion to check for double-jumps
vector<Moves> Checkers::valid_moves(Board m_board, Token piece, Direction direc, bool is_double_jump)
{
    Moves temp_move;
    vector<Moves> avail_moves;

    char opp_char, opp_char_king;
    if(piece.player_num = 1)
    {
        opp_char = m_player_char[0].base_player_char;
        opp_char_king = (int)m_player_char[0].base_player_char - 32;
    }
    else
    {
        opp_char = m_player_char[1].base_player_char;
        opp_char_king = (int)m_player_char[1].base_player_char - 32;
    }

    Coord potential_loc;
    potential_loc.y = piece.loc.y + direc;
    potential_loc.x = piece.loc.x - 1;                                  //check forward-left move first
    if(is_on_board(potential_loc) && is_freespace(potential_loc) && !is_double_jump)       //free space, valid move
    {
        temp_move.start = piece.loc;
        temp_move.end = potential_loc;
        avail_moves.push_back(temp_move);
    }
    else if(is_on_board(potential_loc) && 
            (m_board.get_piece(potential_loc) == piece.base_player_char ||
             m_board.get_piece(potential_loc) == piece.base_player_char - 32)) {}         //occupied by own piece, not valid move
    else if(is_on_board(potential_loc) && 
            (m_board.get_piece(potential_loc) == opp_char ||
             m_board.get_piece(potential_loc) == opp_char_king))               //occupied by opponent, check if jumpable
    {
        potential_loc.x += 1;
        potential_loc.y += direc;
        if(is_freespace(potential_loc))                                 //is jumpable
        {
            Board temp_board = m_board;
            Token temp_token = piece;
            temp_token.loc = temp_move.end;
            temp_board = make_move(temp_board, temp_move);
            temp_move.start = piece.loc;
            temp_move.end = potential_loc;
            temp_move.jumped_pieces.push_back({(short)(potential_loc.x - 1), (short)(potential_loc.y - direc)});
            avail_moves.push_back(temp_move);
            vector<Moves> double_jump_moves = valid_moves(temp_board, temp_token, direc, true);
            if(!double_jump_moves.empty())
                avail_moves.insert(avail_moves.end(), double_jump_moves.begin(), double_jump_moves.end());
        }
    }

    potential_loc.x = piece.loc.x + 1;                                      //check forward-right next
    potential_loc.y = piece.loc.y + direc;
    temp_move.jumped_pieces.clear();

    if(is_on_board(potential_loc) && is_freespace(potential_loc) && !is_double_jump)       //free space, valid move
    {
        temp_move.start = piece.loc;
        temp_move.end = potential_loc;
        avail_moves.push_back(temp_move);
    }
    else if(is_on_board(potential_loc) && 
            (m_board.get_piece(potential_loc) == piece.base_player_char ||
             m_board.get_piece(potential_loc) == piece.base_player_char - 32)) {}         //occupied by own piece, not valid move
    else if(is_on_board(potential_loc) && 
            (m_board.get_piece(potential_loc) == opp_char ||
             m_board.get_piece(potential_loc) == opp_char_king))               //occupied by opponent, check if jumpable
    {
        potential_loc.x += 1;
        potential_loc.y += direc;
        if(is_freespace(potential_loc))                                 //is jumpable
        {
            Board temp_board = m_board;
            Token temp_token = piece;
            temp_token.loc = temp_move.end;
            temp_board = make_move(temp_board, temp_move);
            temp_move.start = piece.loc;
            temp_move.end = potential_loc;
            temp_move.jumped_pieces.push_back({(short)(potential_loc.x - 1), (short)(potential_loc.y - direc)});
            avail_moves.push_back(temp_move);
            vector<Moves> double_jump_moves = valid_moves(temp_board, temp_token, direc, true);
            if(!double_jump_moves.empty())
                avail_moves.insert(avail_moves.end(), double_jump_moves.begin(), double_jump_moves.end());
        }
    }


    if(piece.is_king)                  //check if king
    {
        if(direc == up)
            direc = down;
        else
            direc = up;
        potential_loc.y = piece.loc.y + direc;
        potential_loc.x = piece.loc.x - 1;                                  //check forward-left move first
        if(is_on_board(potential_loc) && is_freespace(potential_loc) && !is_double_jump)       //free space, valid move
        {
            temp_move.start = piece.loc;
            temp_move.end = potential_loc;
            avail_moves.push_back(temp_move);
        }
        else if(is_on_board(potential_loc) && 
                (m_board.get_piece(potential_loc) == piece.base_player_char ||
                m_board.get_piece(potential_loc) == piece.base_player_char - 32)) {}         //occupied by own piece, not valid move
        else if(is_on_board(potential_loc) && 
                (m_board.get_piece(potential_loc) == opp_char ||
                m_board.get_piece(potential_loc) == opp_char_king))               //occupied by opponent, check if jumpable
        {
            potential_loc.x += 1;
            potential_loc.y += direc;
            if(is_freespace(potential_loc))                                 //is jumpable
            {
                Board temp_board = m_board;
                Token temp_token = piece;
                temp_token.loc = temp_move.end;
                temp_board = make_move(temp_board, temp_move);
                temp_move.start = piece.loc;
                temp_move.end = potential_loc;
                temp_move.jumped_pieces.push_back({(short)(potential_loc.x - 1), (short)(potential_loc.y - direc)});
                avail_moves.push_back(temp_move);
                vector<Moves> double_jump_moves = valid_moves(temp_board, temp_token, direc, true);
                if(!double_jump_moves.empty())
                    avail_moves.insert(avail_moves.end(), double_jump_moves.begin(), double_jump_moves.end());
            }
        }

        potential_loc.x = piece.loc.x + 1;                                      //check forward-right next
        potential_loc.y = piece.loc.y + direc;
        temp_move.jumped_pieces.clear();

        if(is_on_board(potential_loc) && is_freespace(potential_loc) && !is_double_jump)       //free space, valid move
        {
            temp_move.start = piece.loc;
            temp_move.end = potential_loc;
            avail_moves.push_back(temp_move);
        }
        else if(is_on_board(potential_loc) && 
                (m_board.get_piece(potential_loc) == piece.base_player_char ||
                m_board.get_piece(potential_loc) == piece.base_player_char - 32)) {}         //occupied by own piece, not valid move
        else if(is_on_board(potential_loc) && 
                (m_board.get_piece(potential_loc) == opp_char ||
                m_board.get_piece(potential_loc) == opp_char_king))               //occupied by opponent, check if jumpable
        {
            potential_loc.x += 1;
            potential_loc.y += direc;
            if(is_freespace(potential_loc))                                 //is jumpable
            {
                Board temp_board = m_board;
                Token temp_token = piece;
                temp_token.loc = temp_move.end;
                temp_board = make_move(temp_board, temp_move);
                temp_move.start = piece.loc;
                temp_move.end = potential_loc;
                temp_move.jumped_pieces.push_back({(short)(potential_loc.x - 1), (short)(potential_loc.y - direc)});
                avail_moves.push_back(temp_move);
                vector<Moves> double_jump_moves = valid_moves(temp_board, temp_token, direc, true);
                if(!double_jump_moves.empty())
                    avail_moves.insert(avail_moves.end(), double_jump_moves.begin(), double_jump_moves.end());
            }
        }

    }

    return avail_moves;
}

void Checkers::update_avail_moves(int player_num)
{
    vector<Moves> *avail_moves;
    Token player_token;

    if(player_num == 1)
    {
        avail_moves = &m_p1_avail_moves;
        player_token = m_player_char[0];
    }
    else
    {
        avail_moves = &m_p2_avail_moves;
        player_token = m_player_char[1];
    }

    avail_moves->clear();
    Direction player_offset;

    player_offset = (player_num == 1) ? down : up;


    //go through each square, if current player, check for avail moves
    Coord temp_loc; 
    for(int y = 0; y < 8; ++y)
    {
        temp_loc.y = y;
        for(int x = 0; x < 8; ++x)
        {
            temp_loc.x = x;
            if(m_board.get_piece(temp_loc) == player_token.base_player_char || 
               m_board.get_piece(temp_loc) == (int)player_token.base_player_char - 32)
            {
                if(m_board.get_piece(temp_loc) == (int)player_token.base_player_char - 32)
                    player_token.is_king = true;
                else
                    player_token.is_king = false;
                player_token.loc = temp_loc;
                vector<Moves> temp_moves = valid_moves(m_board,player_token,player_offset, false);
                avail_moves->insert(avail_moves->end(),temp_moves.begin(), temp_moves.end());
            }
        }
    }
}

Board Checkers::make_move(Board board, Moves move)
{
    board.set_piece(move.end,board.get_piece(move.start));
    board.set_piece(move.start, '|');
    for(vector<Coord>::iterator iter = move.jumped_pieces.begin(); iter != move.jumped_pieces.end(); iter++)
        board.set_piece(*iter, '|');
    return board;
}

void Checkers::print_avail_moves(int player_num)
{
    cout << "\nAvailable Moves\n---------------\n";
    vector<Moves> *avail_moves;

    if(player_num == 1)
        avail_moves = &m_p1_avail_moves;
    else
        avail_moves = &m_p2_avail_moves;

    for(vector<Moves>::iterator iter = avail_moves->begin(); iter != avail_moves->end(); ++iter)
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

bool Checkers::is_on_board(Coord loc)
{
    if(loc.x > 7 || loc.x < 0)
        return false;
    if(loc.y > 7 || loc.y < 0)
        return false;

    return true;
}

bool Checkers::is_freespace(Coord loc)
{
    if(m_board.get_piece(loc) == '|')
        return true;
    else
        return false;
}

bool Checkers::is_jumpable(Coord start_piece, Coord over_piece)
{
    Coord end;
    end.x = start_piece.x + 2 * (over_piece.x - start_piece.x);
    end.y = start_piece.y + 2 * (over_piece.y - start_piece.y);
    if(is_on_board(end))
    {
        if(is_freespace(end))
            return true;
    }
    return false;
}

// :(