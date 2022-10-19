/******************************************************   
 * Author : Avia Avikasis                             *
 * Reviewer: Haggai Newman                            *
 * 15/08/2022                                         *
 * Description : imlementation of knight tour problem *
 *                                                    *
 ******************************************************/
#include <assert.h> /* assert */
#include <stdio.h>  /* printf */
#include <time.h>   /* time */

#include "knight_tour.h"
#include "bit_array.h" /* bit array functions */

#define ROW_SIZE 8
#define COL_SIZE 8
#define ROWS_NUM 8
#define COLS_NUM 8
#define ZERO 0
#define POSITIONS_NUM 64
#define ALL_BITS_ON 64
#define TRANS_OPTIONS 8
#define INVALID_POS 65
#define MAX_TIME 300

typedef struct knight_luts
{
	size_t trans_lut[POSITIONS_NUM][TRANS_OPTIONS];
	unsigned char board[ROWS_NUM][COLS_NUM];
	bit_array_ty bitarr;

}knight_luts;


/* init static functions */
static void InitLuts(knight_luts *luts);
static void InitBoard(knight_luts *luts);
static bool_ty IndexInOrders(int index);
static void InitPosition(int i, int j, knight_luts *luts);
static void PreInitTransLut(knight_luts *luts);
static void InitTransLut(knight_luts *luts);

/* help static functions */
static bool_ty TimeIsValid(time_t start_time);
static bool_ty ThereAreNoAvailPositions(size_t position, knight_luts *luts);
static size_t CountAvailTrans(size_t position, knight_luts *luts);
static int FindMinDegreePosition(knight_luts *luts, const size_t position);
static bool_ty ItsAvail(knight_luts *luts, size_t position);
static status_ty WonsdorffKnightTour(knight_luts *luts, 
									const size_t position,
						 			unsigned char *o_result_tour,
					 				size_t result_index,
					  				time_t start_time);


/*---------------- init functions---------------*/

static void InitLuts(knight_luts *luts)
{
	assert(NULL != luts);
	
	InitBoard(luts);
	InitTransLut(luts);
	luts->bitarr = 0;
}

static void InitBoard(knight_luts *luts)
{
	int counter = ZERO;
	int i = ZERO;
	int j = ZERO;
	
	for(i = ZERO ; i < ROWS_NUM ; ++i)
	{
		for(j = ZERO ; j < COLS_NUM ; ++j)
		{
			luts->board[i][j] = counter;
			++counter;
		}
	}
}

static bool_ty IndexInOrders(int index)
{
	return (index <= ROWS_NUM - 1 && index >= ZERO);
}


static void InitPosition(int i, int j, knight_luts *luts)
{
	int k = TRANS_OPTIONS - 1;
	int cell = ZERO;

    int row_skip[ROWS_NUM] = { 2 , 1 , -1 , -2 , -2 , -1 , 1 , 2 };
    int col_skip[COLS_NUM] = { 1 , 2 , 2 , 1 , -1 , -2 , -2 , -1 };
	
	for(k = TRANS_OPTIONS - 1 ; k >= ZERO ; --k)
	{
		if(IndexInOrders(i + row_skip[k]) && IndexInOrders(j + col_skip[k]))
		{
			luts->trans_lut[luts->board[i][j]][cell] = 
			luts->board[i + row_skip[k]][j + col_skip[k]];
			++cell;
		}
	}
}

static void PreInitTransLut(knight_luts *luts)
{
	int i = ZERO;
	int j = ZERO;
	
	for(i = ZERO; i < POSITIONS_NUM ; ++i)
	{
		for(j = ZERO ; j < COLS_NUM ; ++j)
		{
			luts->trans_lut[i][j] = INVALID_POS;
		}
	}
} 


static void InitTransLut(knight_luts *luts)
{
	int i = ZERO;
	int j = ZERO;
	
	PreInitTransLut(luts);

	for(i = ZERO ; i < ROWS_NUM ; ++i)
	{
		for(j = ZERO ; j < COLS_NUM ; ++j)
		{
			InitPosition(i, j, luts);
		}
	}
}

/*----------------------------------------*/


static status_ty RecursiveKnightTour(knight_luts *luts, 
								const size_t position,
						 		unsigned char *o_result_tour,
					 				size_t result_index,
					  					time_t start_time)
{
	size_t next_position = 0;
	size_t next_pos_indx = 0;
	status_ty status = FAIL;

	if(!TimeIsValid(start_time) || (ThereAreNoAvailPositions(position, luts)
	 	&& (ALL_BITS_ON - 1 != BitArrayCountOn(luts->bitarr))))
	{
		return FAIL;
	}
	
	luts->bitarr = BitArraySetOn(luts->bitarr, position);
	o_result_tour[result_index] = position;
	
	if(ALL_BITS_ON == BitArrayCountOn(luts->bitarr))
	{
		return SUCCESS;
	}
	
	/* try to progress via one of available transitions */
	while((SUCCESS != status) && TimeIsValid(start_time) &&
		 (next_pos_indx < TRANS_OPTIONS))
	{
		next_position = luts->trans_lut[position][next_pos_indx];

		if((INVALID_POS != next_position) && ItsAvail(luts, next_position))
		{
			status = RecursiveKnightTour(luts, next_position, o_result_tour,
												 result_index + 1, start_time);
		}
		
		++next_pos_indx;
	}

	if(SUCCESS == status)
	{
		return SUCCESS;
	}
	
	else
	{
		luts->bitarr = BitArraySetOff(luts->bitarr, position);

		return FAIL;
	}

}


static status_ty WonsdorffKnightTour(knight_luts *luts, 
								const size_t position,
						 		unsigned char *o_result_tour,
					 				size_t result_index,
					  					time_t start_time)
{
	size_t next_position = 0;
	status_ty status = FAIL;
	
	if(!TimeIsValid(start_time) || (ThereAreNoAvailPositions(position, luts)
	 	&& (ALL_BITS_ON - 1 != BitArrayCountOn(luts->bitarr))))
	{
		return FAIL;
	}
	
	luts->bitarr = BitArraySetOn(luts->bitarr, position);
	o_result_tour[result_index] = position;
	
	if(ALL_BITS_ON == BitArrayCountOn(luts->bitarr))
	{
		return SUCCESS;
	}
	
	next_position = FindMinDegreePosition(luts, position);
	if(INVALID_POS == next_position)
	{
		return FAIL;
	}
		
	status = WonsdorffKnightTour(luts, next_position, o_result_tour,
										 result_index + 1, start_time);
		
	if(SUCCESS == status)
	{
		return SUCCESS;
	}
	
	else
	{
		luts->bitarr = BitArraySetOff(luts->bitarr, position);

		return FAIL;
	}

}

/*++++++++++++++++++ knight tour function ++++++++++++++++++*/

status_ty KnightTour(const size_t start_position,
					 unsigned char *o_result_tour,
					 bool_ty should_use_heuristic)
{	

	knight_luts luts = {0};
	
	assert(NULL != o_result_tour);
	
	InitLuts(&luts);	
	
	return (FALSE == should_use_heuristic) ? 
		RecursiveKnightTour(&luts, start_position, o_result_tour, 0, time(0)) :
		WonsdorffKnightTour(&luts, start_position, o_result_tour, 0, time(0));
	
}


/*------------- help static functions -------------- */

static bool_ty TimeIsValid(time_t start_time)
{
	return (MAX_TIME > (time(0) - start_time));
}

static bool_ty ThereAreNoAvailPositions(size_t position, knight_luts *luts)
{
	size_t i = 0;
	
	/* move forward while positions aren't avialable*/
	for( ; (INVALID_POS != luts->trans_lut[position][i]) &&
	(!ItsAvail(luts, luts->trans_lut[position][i])) ; ++i)
	{
		/* empty loop */
	}
	
	return (INVALID_POS == luts->trans_lut[position][i]) ? TRUE : FALSE;
}



static size_t CountAvailTrans(size_t position, knight_luts *luts)
{
	int i = 0;
	int counter = 0;
	
	for(i = 0 ; INVALID_POS != luts->trans_lut[position][i] ; ++i)
	{
		counter = (!ItsAvail(luts, luts->trans_lut[position][i])) ? counter : 
																	counter + 1;
	}
	
	return counter;
}


void PrintTransLut(void)
{
	int i = ZERO;
	int j = ZERO;

	knight_luts luts = {0};
		
	InitBoard(&luts);
	InitTransLut(&luts);
	luts.bitarr = 0;
	
	for(i = ZERO ; i < POSITIONS_NUM ; ++i)
	{
		printf("\nposition %d : ", i);
		
		for(j = ZERO ; j < TRANS_OPTIONS ; ++j)
		{
			
			if(INVALID_POS == luts.trans_lut[i][j])
			{
				break;
			}
			
			printf("%ld ", luts.trans_lut[i][j]);
		}
	}	
	printf("\n\n\n");
	
	for(i = ZERO ; i < TRANS_OPTIONS ; ++i)
	{
		for(j = ZERO ; j < TRANS_OPTIONS ; ++j)
		{
			printf("%d ", luts.board[i][j]);
		}
		printf("\n");
	}
}


static int FindMinDegreePosition(knight_luts *luts, const size_t position)
{

	int i = 0;
	int min_position = 0;
	
	for(i = 0 ; i < TRANS_OPTIONS && 
	(!ItsAvail(luts, luts->trans_lut[position][i])) ; ++i)
	{
		/* empty loop */
	}
	
	min_position = (ItsAvail(luts, luts->trans_lut[position][i])) ? 
					luts->trans_lut[position][i] : INVALID_POS;
		
	for( ; i < TRANS_OPTIONS && (INVALID_POS != luts->trans_lut[position][i]) ; ++i)
	{
		/* the current position have avaliable transitions less then min_position*/
		if(ItsAvail(luts, luts->trans_lut[position][i]) && 
		   (CountAvailTrans(min_position, luts) >
		   CountAvailTrans(luts->trans_lut[position][i], luts)))
		{
			min_position = luts->trans_lut[position][i];
		}
	}

	return min_position;
}


static bool_ty ItsAvail(knight_luts *luts, size_t position)
{
	assert(NULL != luts);
	
	return (0 == BitArrayGetValue(luts->bitarr , position));
}












































