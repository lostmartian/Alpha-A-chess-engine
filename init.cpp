#include "defs.h"
#include "stdlib.h"

// 0000 000000000000000 000000000000000 000000000000000 111111111111111
// 0000 000000000000000 000000000000000 111111111111111 000000000000000
// 0000 000000000000000 111111111111111 000000000000000 000000000000000
// 0000 111111111111111 000000000000000 000000000000000 000000000000000
// 1111 000000000000000 000000000000000 000000000000000 000000000000000
// After adding we then get a random number of 64 bits  
#define RAND_64	((U64)rand() | \
					((U64) rand() << 15) | \
					((U64) rand() << 30) | \
					((U64) rand() << 45) | \
					(((U64)rand() & 0xf) << 60))

// 64 based representation
int Sq120ToSq64[BRD_SQ_NUM] ;
// 120 based representation
int Sq64ToSq120[64];

U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120];
U64 SideKey;
U64 CastleKeys[16];

void InitHashKeys() {
	int index = 0;
	int index2 = 0;

	for(index = 0; index < 13; index++)
		for(index2 = 0; index2 < 120; index2++)
			PieceKeys[index][index2] = RAND_64;

	SideKey = RAND_64;

	for(index = 0; index < 16; index++)
		CastleKeys[index] = RAND_64;
}

void InitBitMask() {
	int index = 0;

	for(index = 0; index < 64; index++) {
		SetMask[index] = 0ULL;
		ClearMask[index] = 0ULL;
	}

	for(index = 0; index < 64; index++) {
		SetMask[index] = (1ULL << index);
		ClearMask[index] = ~SetMask[index];
	}

}

void InitSq120To64 () {

	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;
	int sq64 = 0;

// Setting the values on board with impossible values
	for (index = 0; index <BRD_SQ_NUM; index++) {
		Sq120ToSq64[index] = 65;
	}
	for (index = 0; index <BRD_SQ_NUM; index++) {
		Sq64ToSq120[index] = 120;
	}

// Setting the square values to correct representation
	for(rank = RANK_1; rank<=RANK_8; rank++) {
		for(file = FILE_A; file<=FILE_H; file++) {
			sq = FR2SQ(file, rank);

			Sq64ToSq120[sq64] = sq;

/* Sq64ToSq120 Array
	21   22   23   24   25   26   27   28
   31   32   33   34   35   36   37   38
   41   42   43   44   45   46   47   48
   51   52   53   54   55   56   57   58
   61   62   63   64   65   66   67   68
   71   72   73   74   75   76   77   78
   81   82   83   84   85   86   87   88
   91   92   93   94   95   96   97   98
*/

			Sq120ToSq64[sq] = sq64;

/*  Sq120ToSq64 array 
	65   65   65   65   65   65   65   65   65   65
   65   65   65   65   65   65   65   65   65   65
   65    0    1    2    3    4    5    6    7   65
   65    8    9   10   11   12   13   14   15   65
   65   16   17   18   19   20   21   22   23   65
   65   24   25   26   27   28   29   30   31   65
   65   32   33   34   35   36   37   38   39   65
   65   40   41   42   43   44   45   46   47   65
   65   48   49   50   51   52   53   54   55   65
   65   56   57   58   59   60   61   62   63   65
   65   65   65   65   65   65   65   65   65   65
   65   65   65   65   65   65   65   65   65   65
*/

			sq64++;
		}
	}

}

void AllInit() {

	InitSq120To64(); 
	InitBitMask();
	InitHashKeys();

}
