#ifndef PIECE_HPP
#define PIECE_HPP

/* PIECE_DATA:
 *
 * Every piece has a type and color associated with it, this data is formed as:
 *     PIECE_TYPE + PIECE_COLOR
 *
 *  and to fetch piece type is:
 *      PIECE_DATA % 8
 *
 *  this work because the color is always multiple of 8 then when get the
 *  remainder of the division by 8 give the PIECE_TYPE that's not divisible by
 *  8.
 *
 *  To get the color it's even more simple:
 *      PIECE_DATA - PIECE_TYPE
 *
 *  remember, what is PIECE_DATA so the hard work is to get the PIECE_TYPE to
 *  then get the PIECE_COLOR, but how get the type was explained above.
 */

#define PieceGetType(piece)         ((piece) % 8)
#define PieceHasType(piece, type)   (PieceGetType(piece) == (type))

#define PieceGetColor(piece)        ((piece) - PieceGetType(piece))
#define PieceHasColor(piece, color) (PieceGetColor(piece) == (color))

 /* Helper functions to convert from rank and file system to square index system
  * and the inverse path, square index to rank and file.
  */
#define PieceSquare(rank, file)     ((rank) * 8 + (file))
#define PieceRank(index)            ((index) / 8)
#define PieceFile(index)            ((index) % 8)

typedef enum {
    PIECE_NONE = 0,
    PIECE_KING = 1,
    PIECE_QUEEN = 2,
    PIECE_BISHOP = 3,
    PIECE_KNIGHT = 4,
    PIECE_ROOK = 5,
    PIECE_PAWN = 6,

    PIECE_BLACK = 8,
    PIECE_WHITE = 16,
} Piece;

#endif /* PIECE_HPP */

