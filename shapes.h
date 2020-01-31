/************************************************************************************************************************
 *  [FILE NAME]   :      <shapes.h>                                                                                     *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 26, 2020>                                                                                 *
 *  [Description} :      <Header files contains the shapes formation>                                                   *
 ************************************************************************************************************************/

#ifndef SHAPES_H_
#define SHAPES_H_

/*-----------------------------------------INCLUDES---------------------------------------*/
#include "std_types.h"
#include "game.h"

/*------------------------------------------DEFINES---------------------------------------*/

#define NUM_SHAPES 9
#define SHAPE_WIDTH 8
#define FIRST_SHAPE 0
#define SECOND_SHAPE 1
#define THIRD_SHAPE 2
#define FOURTH_SHAPE 3
#define FIFTH_SHAPE 4
#define SIXTH_SHAPE 5
#define SEVENTH_SHAPE 6
#define EIGHTH_SHAPE 7
#define NINGTH_SHAPE 8

#define STARTED_SHAPE 1
#define LEFT_MARGIN_ALL_SHAPES 16
#define RIGHT_MARGIN_FIRST_SECOND_SHAPE 49
#define RIGHT_MARGIN_THIRD_SHAPE 52
#define RIGHT_MARGIN_FOURTH_SHAPE 54
#define RIGHT_MARGIN_SIXTH_SEVENTH_SHAPE 50
#define RIGHT_MARGINE_REST_SHAPES 48
#define SHAPE_OUTPUT_ADDRESS 30

#define ONE 1
#define TWO 2
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define NINE 9

#define STARTED_ROW 1
#define STARTED_COLUMN 34
#define STARTED_UPPER_PAGE 0
#define STARTED_LOWER_PAGE 1

/*---------------------------------------FUNCTIONS PROTOTYPES-----------------------------*/
void checkAfterMoving(uint8 *columnIndicator_Ptr);
void DrawShape(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *columnIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr, uint8 *nextShapeFlag_Ptr,
		uint8 *lowerState_Ptr, uint8 *upperState_Ptr);
void EraseShape(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *columnIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr, uint8 *lowerState_Ptr,
		uint8 *upperState_Ptr);
void GenerateLocation(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *columnIndicator_Ptr);
void RotateShape(uint8 *columnIndicator_Ptr);
uint8 NextShapeView(uint8 *shape_Ptr);

/*------------------------------------------EXTERNAL VARIABLES----------------------------*/
extern const uint8 shapes[NUM_SHAPES][SHAPE_WIDTH];

#endif /* SHAPES_H_ */
