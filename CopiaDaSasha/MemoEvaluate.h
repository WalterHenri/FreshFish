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

unsigned int boardToKey(ChessBoard* board) {
    unsigned int key = 0;

    for (int i = 0; i < 64; i++) {
        int piece = board->squares[i];
        key += piece * (PieceRank(piece) * 10 + PieceFile(piece));
    }
    return key;
}

struct MemoEvaluation {
    double grade;
    unsigned int key;
};

struct MemoEvaluation* hashArray[SIZE];
struct MemoEvaluation* dummyItem;
struct MemoEvaluation* item;

int hashCode(int key) {
    return key % SIZE;
}

struct MemoEvaluation* search(int key) {
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

void insert(int key, double data) {

    struct MemoEvaluation* item = (struct MemoEvaluation*)malloc(sizeof(struct MemoEvaluation));
    item->grade = data;
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
    int key = item->key;

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