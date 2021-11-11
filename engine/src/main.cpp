#include "Headers/ChessGame.h"

using namespace std;

int main(){
    ChessGame chess;
    chess.init_all();
    chess.printFullCharBoard();

    bool inCheck = false, checkmate = false;

    while(!checkmate){
        inCheck = chess.is_square_attacked((!chess.board.side_to_move) ? chess.indexLeastSigBit(chess.board.bitboards[K]) : 
                                                                         chess.indexLeastSigBit(chess.board.bitboards[k]), 
                                                                         chess.board.side_to_move ^ 1);
        
        chess.search_position(3);

        if(inCheck){
            if(!chess.m_legal_moves_num) checkmate = true;
        }

        chess.make_move(chess.m_best_move, all_moves);

        chess.printFullCharBoard();
    }

    return 0;
}