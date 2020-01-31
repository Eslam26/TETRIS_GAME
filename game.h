/************************************************************************************************************************
 *  [FILE NAME]   :      <game.h>                                                                                       *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 25, 2020>                                                                                 *
 *  [Description} :      <Header file for the GAME>                                                                     *
 ************************************************************************************************************************/

#ifndef GAME_H_
#define GAME_H_

/*---------------------------------------------------INCLUDES------------------------------------------------------------*/
#include "micro_config.h"
#include "glcd.h"
#include "std_types.h"
#include "common_macros.h"
#include "External_interrupts.h"
#include "shapes.h"
/*-------------------------------------------------DEFINITIONS-----------------------------------------------------------*/
#define COLUMN_SETTING_ADDRESS 0x40
#define PAGE_SETTING_ADDRESS 0xB8
#define LEFT_BORDER 15
#define RIGHT_BORDER 56
#define ZERO 0
#define BORDER_WIDTH 41
#define FIRST_ROW 0x01
#define LAST_ROW 0x80
#define LAST_PAGE 7
#define ALL_ROWS 0xFF
#define PAGE_0 0
#define PAGE_1 1
#define PAGE_2 2
#define PAGE_3 3
#define PAGE_4 4
#define PAGE_5 5
#define PAGE_7 7
#define OFF 0
#define ON 1
#define NOT_COLLAPSE 0
#define COLLAPSE 1
#define LOST 1
#define NOT_LOST 0
#define DELAY_AFTER_DRAWING_TIME 70
#define DELAY_AFTER_ERASING_TIME 10
#define LOCATION_LAST_ROW 6
#define LOCATION_LAST_UPPER_PAGE 7
#define LOCATION_LAST_LOWER_PAGE 8
#define NUM_BITS_IN_BYTE 8
#define HIGH_8BITS 0xFF
#define ONE_SECOND 1000
#define LAST_ROW_IN_PAGE 7

#define ROW_TRANSFORM ON

/*-------------------------------------------------FUNCTIONS PROTOTYPES---------------------------------------------------*/

uint8 checkUpper(uint8 state, uint8 rowIndicator);
uint8 checkLower(uint8 state, uint8 rowIndicator);
void LosingFunction(void);
void RowCompleteTranform(uint8 rowIndicator, uint8 pageIndicator);
void completeCheck(uint8 *rowIndicator_Ptr, uint8 *lowerPageIndicator_Ptr);
/*-----------------------------------------------EXTERNAL VARIABLES------------------------------------------------------*/
extern uint8 loseFlag;
extern uint8 shape;
extern uint8 nextShape;

#endif /* GAME_H_ */
