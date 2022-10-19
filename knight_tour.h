/******************************************************   
 * Author : Avia Avikasis                             *
 * Reviewer: Haggai Newman                            *
 * 15/08/2022                                         *
 * Description : imlementation of knight tour problem *
 *                                                    *
 ******************************************************/
#ifndef __ILRD_OL127_128_KNIGHT_TOUR_H__
#define __ILRD_OL127_128_KNIGHT_TOUR_H__

#include <stddef.h> /*size_t*/
#include "utils.h"  /* utils functions */
/*
DESCRIPTION : result of knight tour problem,
			  for 8 * 8 board
			  
PARAMETERS : start position from 0 to 63(size_t),
			 unsigned char array in size 64, where
			 the function will return the path of result,
			 flag that mark if the result execute with
			 the wansdorff optimization or not

RETURN : 	 in success the return value will be 
		     SUCCESS and the path of the night will be
		     write in the o_result_tour array.
		     if the time will be over 5 minutes,
		     the function return FAIL 
			 
TIME COMPLEXITY : O(8 ^ (n ^ 2)) in the worst case,
				  where n is the num of lines 
				  in the matrix

SPACE COMPLEXITY : O(n ^ 2)

*/
status_ty KnightTour(const size_t start_position,
					unsigned char *o_result_tour,
					 bool_ty should_use_heuristic);

void PrintTransLut(void);

#endif /* __ILRD_OL127_128_KNIGHT_TOUR_H__ */


