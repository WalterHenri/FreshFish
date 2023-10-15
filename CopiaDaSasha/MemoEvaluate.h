#include "board.h"
#include "Piece.h"

#define SIZE 100000


/*  * The code of a position should be easy to calculate and unique
    * for every position so the solution here is to sum the value of
    * of the piece and multiplie by the rank and the file,
    * the file and the rank shouldnt have the same 1-8 positions
    * because its an square and its 4am help me pls
    * if the key starts with 0 values to - 10000 would be easy to store in the hash map
    * but i have any idea how to do that
*/


// Define the initial hash key
unsigned long long int hashKey = 0ULL;

// Hash table for pieces (piece-square table)
unsigned long long int pieceKeys[12][64];

// Hash table for en passant square
unsigned long long int enPassantKeys[64];

// Hash table for castling rights
unsigned long long int castleKeys[16];

// Initialize the hash tables
void initHashKeys() {
    int piece, square, castle;
    for (piece = 0; piece < 12; ++piece) {
        for (square = 0; square < 64; ++square) {
            pieceKeys[piece][square] = rand();
        }
    }
    for (square = 0; square < 64; ++square) {
        enPassantKeys[square] = rand();
    }
    for (castle = 0; castle < 16; ++castle) {
        castleKeys[castle] = rand();
    }
}

// Compute hash key from FEN string
unsigned long long int generateHashKey(char fen[]) {
    unsigned long long int key = 0ULL;
    int index = 0;
    int square = 0;
    int len = strlen(fen);

    // Parse the FEN string and update the hash key
    while (fen[index] != ' ') {
        if (fen[index] >= '1' && fen[index] <= '8') {
            square += (fen[index] - '0');
        }
        else if (fen[index] >= 'a' && fen[index] <= 'z') {
            int piece = fen[index] - 'a';
            square += piece;
            key ^= pieceKeys[piece][square];
        }
        else if (fen[index] >= 'A' && fen[index] <= 'Z') {
            int piece = fen[index] - 'A' + 6; // Convert uppercase to corresponding piece index
            square += piece;
            key ^= pieceKeys[piece][square];
        }
        else if (fen[index] == '/') {
            // Skip to the next rank
            square = (square + 8) % 64;
        }
        ++index;
    }

    // Process castling rights
    ++index; // Skip space
    while (fen[index] != ' ') {
        key ^= castleKeys[fen[index] - 'K' + 2];
        ++index;
    }

    // Process en passant square
    ++index; // Skip space
    if (fen[index] != '-') {
        int file = fen[index] - 'a';
        int rank = fen[index + 1] - '1';
        int epSquare = rank * 8 + file;
        key ^= enPassantKeys[epSquare];
    }

    return key;
}




unsigned long long int boardToKey(ChessBoard* board) {

    char fenString[101];
    BoardGetAsFEN(board, fenString);

    return generateHashKey(fenString);
    
}

struct MemoEvaluation {
    int grade;
    saxa_move bestMove;
    unsigned long long int key;
};

struct MemoEvaluation* hashArray[SIZE];


struct MemoEvaluation* dummyItem;
struct MemoEvaluation* item;

int hashCode(unsigned long long int key) {
    return key % SIZE;
}

struct MemoEvaluation* search(unsigned long long int key) {
    //get the hash 
    int hashIndex = hashCode(key);

    //move in array until an empty 
    while (hashArray[hashIndex] != NULL) {

        if (hashArray[hashIndex]->key == key)
            return hashArray[hashIndex];

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void insert(unsigned long long int key, int data, saxa_move bestMove) {

    struct MemoEvaluation* item = (struct MemoEvaluation*)malloc(sizeof(struct MemoEvaluation));
    item->grade = data;
    item->bestMove = bestMove;
    item->key = key;

    //get the hash 
    int hashIndex = hashCode(key);

    //move in array until an empty or deleted cell
    while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    hashArray[hashIndex] = item;
}

struct MemoEvaluation* deleteItem(struct MemoEvaluation* item) {
    unsigned long long int key = item->key;

    //get the hash 
    int hashIndex = hashCode(key);

    //move in array until an empty
    while (hashArray[hashIndex] != NULL) {

        if (hashArray[hashIndex]->key == key) {
            struct MemoEvaluation* temp = hashArray[hashIndex];

            //assign a dummy item at deleted position
            hashArray[hashIndex] = dummyItem;
            return temp;
        }

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}