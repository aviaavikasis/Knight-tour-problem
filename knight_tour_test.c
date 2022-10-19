#include <assert.h> /* assert */
#include <stdio.h>  /* printf */

#include "utils.h"
#include "knight_tour.h"
#include "bit_array.h"

#define RESULT_ARR_SIZE 64
#define INIT_VAL 100 

void TransitionsLutTest(void); 
void KnightTourTest(void);
void WansdorffKnightTourTest(void);


int main(void)
{
	TransitionsLutTest();
	KnightTourTest();
	WansdorffKnightTourTest();

	printf("\n->->->->->-> success!! <-<-<-<-<-<-\n\n");	

	return 0;
}


void TransitionsLutTest(void)
{
	PrintTransLut();
}

static void PrintResult(unsigned char *o_result_tour)
{
	int i = 0;
	
	printf("\n\n\n");
	
	for(i = 0 ; (i < RESULT_ARR_SIZE) && (INIT_VAL != o_result_tour[i]) ; ++i)
	{
		printf("%d ", o_result_tour[i]);
	}
	printf("\n\n%d\n", i);
}

void KnightTourTest(void)
{
	unsigned char o_result_tour[RESULT_ARR_SIZE] = {0};
	size_t start_position = 0;
	int i = 0;
	
	for(i = 0 ; i < RESULT_ARR_SIZE ; ++i)
	{
		o_result_tour[i] = INIT_VAL;
	}
	
	
	assert(SUCCESS == KnightTour(start_position, o_result_tour, FALSE));
	
	PrintResult(o_result_tour);
}


void WansdorffKnightTourTest(void)
{
	unsigned char o_result_tour[RESULT_ARR_SIZE] = {0};
	int i = 0;
	
	for(i = 0 ; i < RESULT_ARR_SIZE ; ++i)
	{
		o_result_tour[i] = INIT_VAL;
	}
	
	assert(SUCCESS == KnightTour(12, o_result_tour, TRUE));
	
	PrintResult(o_result_tour);

}
















