#include <string>

#include "ChessGame.h"

string getAI_move(string position, int depth){
    ChessGame chess(position);
    chess.init_all();
    chess.search_position(depth);
    chess.make_move(chess.m_best_move, all_moves);
    
    return chess.get_move(chess.m_best_move);
}