#include "Board.h"

using namespace std;

const BITBOARD not_a_file = 18374403900871474942ULL;
const BITBOARD not_h_file = 9187201950435737471ULL;
const BITBOARD not_gh_file = 4557430888798830399ULL;
const BITBOARD not_ab_file = 18229723555195321596ULL;

const char *square_to_coordinates[] = {
                   "a8","b8","c8","d8","e8","f8","g8","h8",
                   "a7","b7","c7","d7","e7","f7","g7","h7",
                   "a6","b6","c6","d6","e6","f6","g6","h6",
                   "a5","b5","c5","d5","e5","f5","g5","h5",
                   "a4","b4","c4","d4","e4","f4","g4","h4",
                   "a3","b3","c3","d3","e3","f3","g3","h3",
                   "a2","b2","c2","d2","e2","f2","g2","h2",
                   "a1","b1","c1","d1","e1","f1","g1","h1", "-"};

Board::Board(string fen){
    // initializing boards and pieces
    BITBOARD temp;
    temp.all();
    for(int i=0; i<12; i++){
        bitboards.push_back(temp);
        if(i<3) occupancies.push_back(temp);
    }

    // set starting FEN and parse
    FEN = fen;
    parseFen(FEN);
}

int Board::getBit(BITBOARD board, int index){
    string str = board.to_string();
    return int(str[64 - index - 1] - '0');
}

// Temp debugging bitboard printer
void Board::printBoard(int board){
    std::cout << "\n\n";
    for(int rank=0; rank<8; rank++){
        for (int file=0; file<8; file++){
            std::cout << " " << bitboards[board][(rank * 8) + file] << " ";
        }
        std::cout << "\n";
    }
}

// bitboard visualization
void Board::printBitboard(BITBOARD bitboard){
    for(int rank=0; rank<8; rank++){
        cout << "\n  " << 8- rank << "  ";
        for(int file=0; file<8; file++){
            cout << bitboard[(rank * 8) + file] << " ";
        }
    }
    cout << "\n\n     a b c d e f g h";
    cout << "\n  -------------------";
    cout << "\n  Value: " << bitboard.to_ullong() << endl;
}

void Board::parseFen(string fen){
    // reset game variables
    for(int i=P; i<=k; i++){
        bitboards[i].reset();
    }
    side_to_move = -1;
    enpassant_square = no_sq;
    castling_rights = 0;

    int index = 0;
    for(int rank=0; rank<8; rank++){
        for(int file=0; file<8; file++){
            int square = (rank*8) + file;
            // if it is a piece, set the bit on the corresponding bitboard
            if((fen[index] >= 'a' && fen[index] <= 'z') || (fen[index] >= 'A' && fen[index] <= 'Z')){
                int piece = char_to_piece.at(fen[index]);
                bitboards[piece].set(square);
                index++;
                continue;
            }

            // if it is a number, adjust file index
            if(fen[index] >= '0' && fen[index] <= '9'){
                int offset = fen[index] - '0';
                file += offset - 1;
                index++;
                continue;
            }
            // if it's next rank, adjust indicies accordingly
            if(fen[index] == '/'){
                index++;
                file--;
                continue;
            }
        }
    }

    // isolate game state from position
    int variables = fen.find_first_of(" ") + 1;
    string variable_settings = fen.substr(variables,fen.size()-variables);

    // set game state variables
    side_to_move = (variable_settings[0] == 'w') ? white : black;

    index = 2;
    while(variable_settings[index] != ' '){
        switch (variable_settings[index])
        {
        case 'K': castling_rights |= wk; break;
        case 'Q': castling_rights |= wq; break;
        case 'k': castling_rights |= bk; break;
        case 'q': castling_rights |= bq; break;
        default:break;
        }
        index++;
    }

    index++;
    if(variable_settings[index] != '-'){
        int file = variable_settings[index] -'a';
        int rank = 8 - (variable_settings[++index] - '0');

        enpassant_square = (rank*8) + file;
    }else{
        enpassant_square = no_sq;
    }

    occupancies[white].reset();
    occupancies[black].reset();
    occupancies[both].reset();

    // init occupancy boards
    for(int piece=P; piece<=K; piece++){
        occupancies[white] |= bitboards[piece];
        occupancies[black] |= bitboards[piece + p];
    }
    occupancies[both] |= (occupancies[white] | occupancies[black]);
}

// updating FEN string using the boards
string Board::updateFEN(){
    // declare output string
    string output;

    // declare empty checker and counter
    bool empty = true;
    int empty_squares = 0;
    int piece_occupier = -1;

    // loop over ranks
    for(int rank=0; rank<8; rank++){
        // add empty spaces and / after every rank
        if(rank > 0){
            if(empty_squares > 0){
                char num = empty_squares + '0';
                output += num;
                empty_squares = 0;
            }
            output += '/';
        }

        // loop over files
        for(int file=0; file<8; file++){
            // reset empty bool for every square and set square index
            empty = true;
            int square = (rank * 8) + file;

            // loop over piece bitboards to find if a square is occupied by a piece
            for(int piece=P; piece<=k; piece++){
                if(getBit(bitboards[piece], square)){
                    piece_occupier = piece;
                    empty = false;
                    break;
                }
            }

            // check if square is empty
            if(!empty){
                // check if there were any empty spaces before this
                if(empty_squares > 0){
                    // if so, add the number of empty spaces
                    char num = empty_squares + '0';
                    output += num;
                    empty_squares = 0;
                }
                output += piece_to_char.at(piece_occupier);
            }else{
                empty_squares++;
            }
        }
        if(rank==7){
            if(empty_squares){
                char num = empty_squares + '0';
                output += num;
            }
        }
    }

    // add side to move
    output += ' ';
    if(side_to_move == white) output += 'w';
    else output += 'b';    

    // castling rights
    output += ' ';
    if(castling_rights){
        if(castling_rights & wk) output += 'K';
        else output += '-';
        if(castling_rights & wq) output += 'Q';
        else output += '-';
        if(castling_rights & bk) output += 'k';
        else output += '-';
        if(castling_rights & bq) output += 'q';
        else output += '-';
    }else output += '-';

    // enpassant square
    output += ' ';
    if(enpassant_square) output += square_to_coordinates[enpassant_square];
    else output += '-';

    // temp move clock holders
    output += " 0 1";

    FEN = output;
    return output;
}