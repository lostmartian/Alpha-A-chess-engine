#ifndef DEFS_H
#define DEFS_H

#include "stdio.h"
#include "stdlib.h"

// Debugger flags

#define DEBUG
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if (!(n))  \
	printf("%s - Failed ", #n); \
	printf("On %s ", __DATE__); \
	printf("At %s ", __TIME__); \
	printf("In file %s ", __FILE__); \
	printf("At line %d \n", __LINE__); \
	exit(1);

#endif

// 64-bit integer representation will be used to represent pieces
// on board as there are 64 cells in a chess board.
// Example: If a Pawn is present on B2 then U64 will represent is as a set bit
// 00000000 01000000 00000000 00000000 00000000 00000000 00000000 00000000
typedef unsigned long long U64;

#define NAME "Vice 1.0" 
#define BRD_SQ_NUM 120

#define MAXGAMEMOVES 2058

// Representing the board pieces
// EMPTY represents empty space on board cell
// Starting letter repressents the side ie w-white and b-black
// Followed by P-Pawn, N-Knight, B-Bishop, R-Rook, Q-Queen, K-King
// Example: wK - white King, bN - black Knight

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };

// Columns - FILES denoted from A to H
// Rows - RANK numbered from 1 to 8

enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

// Representing colors

enum { WHITE, BLACK, BOTH };

// Representing board cell numbers

enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

// Castling
// Here numbers are represented as 1, 2, 4 and 8 so that these
// can be represented as 4 bit binary string where set bit means
// TRUE and unset bit means FALSE
// Example: 0010 - bK can castle King's side

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8};

enum { FALSE, TRUE };

// Stores information to UNDO a move
typedef struct {

	int move;
	int castlePerm;
	int enPas;
	int fiftyMove;
	U64 posKey;

} S_UNDO;

typedef struct {

	int pieces[BRD_SQ_NUM];

// For 3 colors specified earlier
	U64 pawns[3];

// Holds the position of King square for black and white
	int KingSq[2];

// Hold the data for which side to move
	int side;

// Hold data for en-pasant square if any
	int enPas;

// Hold data for the Fifty Moves rules
	int fiftyMove;

// Half moves into current search
	int ply;

// History play moves used as a index for S_UNDO history array
	int hisPly;

// Castle permission
	int castlePerm;

// This is a key we will use to check position
	U64 posKey; // basically a hash-key

// Number of pieces on the board
// 13 pieces corredponding to the enum of different pieces declared above
	int pceNum[13];

// Storing data for pieces currently onboard
// Size 3 for BOTH, WHITE and BLACK
	int bigPce[3]; //pieces except Knight
	int majPce[3]; // Rooks and Queens
	int minPce[3]; // Knights and Bishops

// Store the details of every move played in the game so indication the conditions
// in the S_UNDO struct before the move is made
	S_UNDO history[MAXGAMEMOVES];

// Piece List array keeps the position of pieces other than pawns
// The 13 x 10 represents 13 pieces with a posibility of each pawn promoted
	int pList[13][10];

} S_BOARD;

/*  MACROS */

// Returns the 120 based square when provided with file-f and rank-r
#define FR2SQ(f, r) ((21+f) + (r*10))
#define SQ64(sq120) Sq120ToSq64[sq120]
#define POP(b) PopBit(b)
#define CNT(b) CountBits(b); 
#define CLRBIT(bb, sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)])

/* GLOBALS */

// Return 120 cells representation to 64 cells representations
extern int Sq120ToSq64[BRD_SQ_NUM] ;

// Return 64 cells representation to 120 cells representation
extern int Sq64ToSq120[64];

// To set the bits of pawns
extern U64 SetMask[64];

// To clear the set bits of pawns
extern U64 ClearMask[64];

extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKeys[16];

/* FUNCTIONS */

// init.cpp
extern void AllInit();

// bitboards.cpp
extern void PrintBitBoard(U64 bb);
extern int PopBit(U64 *bb);
extern int CountBits(U64 b);

// hashkeys.cpp
extern  U64 GeneratePositionKey(const S_BOARD *pos);

#endif
