#include <string>

#include "ChessGame.h"

string getAI_move(string position){
    std::cout << "Received position: " << position << endl;
    ChessGame chess(position);
    chess.init_all();
    chess.search_position(4);
    chess.make_move(chess.m_best_move, all_moves);
    std::cout << "game post move position: " << chess.board.FEN << endl;
    std::cout << "\n";
    
    return chess.board.FEN;
}