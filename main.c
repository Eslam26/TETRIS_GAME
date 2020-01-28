/************************************************************************************************************************
 *  [FILE NAME]   :      <main.c>                                                                                       *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 25, 2020>                                                                                 *
 *  [Description} :      <Source file for the GAME>                                                                     *
 ************************************************************************************************************************/

/*-------------------------------------------------INCLUDES-------------------------------------------------------------*/

#include "micro_config.h"
#include "glcd.h"
#include "std_types.h"
#include "common_macros.h"
#include "External_interrupts.h"
#include "shapes.h"

/*-------------------------------------------------DEFINITIONS-----------------------------------------------------------*/
#define COLUMN_SETTING_ADDRESS 0x40
#define PAGE_SETTING_ADDRESS 0xB8
#define LEFT_BORDER 4
#define ZERO 0
#define BORDER_WIDTH 50
#define FIRST_ROW 0x01
#define LAST_ROW 0x80
#define LAST_PAGE 7
#define ALL_ROWS 0xFF
#define PAGE_0 0
#define PAGE_1 1
#define PAGE_2 2
#define PAGE_3 3
#define PAGE_7 7
#define ONE 1
#define TWO 2
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define SHAPE_WIDTH 8
#define STARTED_SHAPE 1
#define STARTED_ROW 1
#define STARTED_UPPER_PAGE 0
#define STARTED_LOWER_PAGE 1
#define OFF 0
#define ON 1
#define NOT_COLLAPSE 0
#define COLLAPSE 1
#define SHAPE_OUTPUT_ADDRESS 30

/*-------------------------------------------------FUNCTIONS DEFINITIONS---------------------------------------------------*/
void GameIntiate(uint8 gamePlay, uint8 * shape_Ptr);
void DrawShape(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *columnIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr, uint8 *nextShapeFlag_Ptr);
void EraseShape(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *columnIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr);
void GenerateLocation(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *columnIndicator_Ptr);
void RowCompleteCheck(uint8 *rowIndicator_Ptr, uint8 *pageIndicator_Ptr,
		uint8 *columnIndicator_Ptr);
void MoveRightCallBackFunction(void);
void MoveLeftCallBackFunction(void);
void TurnShapeCallBackFunction(void);
uint8 checkUpper(uint8 state, uint8 rowIndicator);
uint8 checkLower(uint8 state, uint8 rowIndicator);
void RowCompleteTranform(uint8 rowIndicator_Ptr, uint8 pageIndicator_Ptr,
		uint8 columnIndicator_Ptr);
void nextShapeView(uint8 * shape_Ptr);


/*----------------------------------------------------GLOBAL VARIABLES----------------------------------------------------*/
uint8 nextFlag = ZERO;
uint8 nextPage = ZERO;
uint8 moveLeftFlag = ZERO;
uint8 moveRightFlag = ZERO;
uint8 loseFlag = ZERO;
uint8 shape = STARTED_SHAPE; /* started shape */

int main() {
	/* Initialize the varibles */

	uint8 rowIndicator = STARTED_ROW; /* started row  */
	uint8 columnIndicator = LEFT_BORDER + ONE; /* started column */
	uint8 upperPageIndicator = STARTED_UPPER_PAGE; /* started upper page */
	uint8 lowerPageIndicator = STARTED_LOWER_PAGE; /* started lower page */
	uint8 nextShapeFlag = OFF; /* sets flag to change the shape to zero */
	uint8 prevRow = rowIndicator; /* stores previous row number */
	uint8 prevColumn = columnIndicator; /* stores previous column number */
	uint8 prevUpperPage = upperPageIndicator; /* stores previous upper page number */
	uint8 prevLowerPage = lowerPageIndicator; /* stores previous lower page number */

	uint8 gamePlay = ONE;

	E_Interrupts_configType interruptsConfig = { FALLING_EDGE_INT0,
				FALLING_EDGE_INT1, FALLING_EDGE_INT2 }; /* setting the Interrupt Intialized Structure */


	ExternalInterrupts_INT0_setCallBack(MoveLeftCallBackFunction); /* setting INT0 callback function */
	ExternalInterrupts_INT1_setCallBack(MoveRightCallBackFunction);/* setting INT1 callback function */
	ExternalInterrupts_INT2_setCallBack(TurnShapeCallBackFunction);
	ExternalInterrupts_init(&interruptsConfig); /* intialize the External Interrupts Driver */

	GLCD_Init();        /* intialize GLCD Driver */
	GLCD_clearScreen(); /* clear the GLCD Screen */
	GLCD_displayString(PAGE_3, "         Welcome               Puzzle Game");
	_delay_ms(400);
	GLCD_clearScreen(); /* clear the GLCD Screen */


	GameIntiate(0, &shape); /* initiate the game */
	sei();

	while (1) {
		while(gamePlay)
		{
			DrawShape(&shape, &rowIndicator, &columnIndicator, &lowerPageIndicator,
					&upperPageIndicator, &nextShapeFlag);
			if (nextShapeFlag) {
				nextShapeFlag = ZERO;
				DrawShape(&shape, &prevRow, &prevColumn, &prevLowerPage, &prevUpperPage,
						&nextShapeFlag);
				shape++;
				nextShapeView(&shape);
				if((shape)==9)
				{
					(shape) = ZERO;
				}
				rowIndicator = STARTED_ROW;
				upperPageIndicator = STARTED_UPPER_PAGE;
				lowerPageIndicator = STARTED_LOWER_PAGE;

			} else {
				if ((rowIndicator == 6) && (upperPageIndicator == 7)
						&& (lowerPageIndicator == 8)) {
					GenerateLocation(&shape, &rowIndicator, &upperPageIndicator,
								&lowerPageIndicator, &columnIndicator);
				} else {
					EraseShape(&shape, &rowIndicator, &columnIndicator,
							&lowerPageIndicator, &upperPageIndicator);
					prevRow = rowIndicator;
					prevColumn = columnIndicator;
					prevUpperPage = upperPageIndicator;
					prevLowerPage = lowerPageIndicator;
					GenerateLocation(&shape, &rowIndicator, &upperPageIndicator,
								&lowerPageIndicator, &columnIndicator);
				}



			}

			if(loseFlag)
			{

				gamePlay = ZERO;
				loseFlag=ZERO;
			}

			if (moveLeftFlag == ONE) {
				moveLeftFlag = ZERO;
				columnIndicator--;
				if (columnIndicator == LEFT_BORDER) {
					columnIndicator++;
				}

			}
			if (moveRightFlag == ONE) {
				moveRightFlag = ZERO;
				columnIndicator++;
				if (columnIndicator + SHAPE_WIDTH > BORDER_WIDTH + LEFT_BORDER) {
					columnIndicator--;
				}
			}
		}
		cli();
		GLCD_CTRL_PORT |= (ONE << CS1); /* Select Left half of display */
		GLCD_CTRL_PORT &= ~(ONE << CS2);
		GLCD_sendCommand(COLUMN_SETTING_ADDRESS); /* Set Y address (column=0) */
		GLCD_sendCommand(PAGE_SETTING_ADDRESS); /* Set x address (page=0) */
		GLCD_clearScreen();
		GLCD_displayString(PAGE_2, "Sorry, you have just lost  the game");
		_delay_ms(1000);
		GLCD_clearScreen();
		GLCD_displayString(PAGE_2, "      Thank You :')");
		_delay_ms(1000);
		while(!gamePlay)
		{

		}

	}
	return 0;
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GameIntiate
 [Description]  :  This function is responsible for initiating the game and displaying the main borders
 of the game.
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void GameIntiate(uint8 gamePlay,uint8 * shape_Ptr) {
	uint8 page = ZERO;
	uint8 loopCounter = ZERO;

	cli();
	/* Setting up records place */
	GLCD_CTRL_PORT |= (ONE << CS2); /* Select Right half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS1);
	GLCD_displayString(PAGE_0,"              ->Shape:");
	GLCD_CTRL_PORT |= (ONE << CS2); /* Select Right half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS1);
	GLCD_sendCommand(COLUMN_SETTING_ADDRESS + SHAPE_OUTPUT_ADDRESS ); /* Set column address) */
	GLCD_sendCommand(PAGE_SETTING_ADDRESS + PAGE_1); /* Set page address */
	for(loopCounter=ZERO;loopCounter<SHAPE_WIDTH;loopCounter++)
	{
		GLCD_sendData(shapes[(*shape_Ptr)+1][loopCounter]);
	}

	/* generating border of the game */
	GLCD_CTRL_PORT |= (ONE << CS1); /* Select Left half of display */
	GLCD_CTRL_PORT &= ~(1 << CS2);
	GLCD_sendCommand(COLUMN_SETTING_ADDRESS + LEFT_BORDER); /* Set Y address (fifth column) */
	GLCD_sendCommand(PAGE_SETTING_ADDRESS); /* Set x address (page=0) */
	/* displaying the border of the game */
	GLCD_sendData(ALL_ROWS);
	for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE; loopCounter++) {
		GLCD_sendData(FIRST_ROW);
	}
	GLCD_sendData(ALL_ROWS);
	for (page = ONE; page < PAGE_7; page++) {
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + page); /* Set page address */
		GLCD_sendCommand(COLUMN_SETTING_ADDRESS + LEFT_BORDER); /* Set column address */
		GLCD_sendData(ALL_ROWS);
		GLCD_sendCommand(
		COLUMN_SETTING_ADDRESS + LEFT_BORDER + BORDER_WIDTH); /* Set column address */
		GLCD_sendData(ALL_ROWS);
	}
	GLCD_sendCommand(PAGE_SETTING_ADDRESS + page);
	GLCD_sendCommand(COLUMN_SETTING_ADDRESS + LEFT_BORDER); /* Set column address */
	GLCD_sendData(ALL_ROWS);
	for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE; loopCounter++) {
		GLCD_sendData(LAST_ROW);
	}
	GLCD_sendData(ALL_ROWS);

	GLCD_sendCommand(COLUMN_SETTING_ADDRESS); /* Set Y address (column=0) */
	GLCD_sendCommand(PAGE_SETTING_ADDRESS); /* Set x address (page=0) */
	sei();
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  DrawShape
 [Description]  :  This function is responsible for drawing the shape at the top of the game zone
 [Args]         :  uint8 *shape_Ptr
 	 	 	 	 	 	 this argument shall contains the address of the array of the shapes formation
 	 	 	 	   uint8 * rowIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the rowIndicator variable
 	 	 	 	   uint8 * upperPageIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the upperPageIndicator variable
 	 	 	 	   uint8 * lowerPageIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the lowerPageIndicator variable
 	 	 	 	   uint8 * columnIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the columnIndicator variable
 	 	 	 	   uint8 * nextShapeFlag_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the nextShapeFlag variable
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void DrawShape(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *columnIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr, uint8 *nextShapeFlag_Ptr) {
	uint8 loopCounter = ZERO;
	uint8 upperstate[SHAPE_WIDTH] = { ZERO };
	uint8 lowerstate[SHAPE_WIDTH] = { ZERO };

	cli();
	GLCD_CTRL_PORT |= (ONE << CS1); /* Select Left half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS2);

	if (*upperPageIndicator_Ptr <= LAST_PAGE) {
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + *upperPageIndicator_Ptr);

		for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
			GLCD_sendCommand(
			COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr + loopCounter);
			upperstate[loopCounter] = GLCD_readData();
			upperstate[loopCounter] = GLCD_readData();
			if (checkUpper(upperstate[loopCounter], *rowIndicator_Ptr)
					& (((shapes[*shape_Ptr][loopCounter]) << (*rowIndicator_Ptr)))) {
				*nextShapeFlag_Ptr = ONE;
				break;
			}
		}
	}
	if (*nextShapeFlag_Ptr) {
	} else {
		if (*lowerPageIndicator_Ptr <= LAST_PAGE) {
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + *lowerPageIndicator_Ptr);

			for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
				GLCD_sendCommand(
				COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr + loopCounter);
				lowerstate[loopCounter] = GLCD_readData();
				lowerstate[loopCounter] = GLCD_readData();
				if (checkLower(lowerstate[loopCounter], *rowIndicator_Ptr)
						& (((shapes[*shape_Ptr][loopCounter])
								>> ((8 - *rowIndicator_Ptr))))) {
					*nextShapeFlag_Ptr = ONE;
					break;
				}
			}
		}
	}
	if (*nextShapeFlag_Ptr) {
		if(*upperPageIndicator_Ptr == STARTED_UPPER_PAGE)
		{
			loseFlag = ONE;
		}

	} else {
		if ((*upperPageIndicator_Ptr <= LAST_PAGE)) {
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + *upperPageIndicator_Ptr);
			GLCD_sendCommand(
			COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr);

			for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
				GLCD_sendData(
						(upperstate[loopCounter]
								& ~((shapes[*shape_Ptr][loopCounter])
										<< (*rowIndicator_Ptr)))
								| (((shapes[*shape_Ptr][loopCounter])
										<< (*rowIndicator_Ptr))));
			}
		}
		if ((*lowerPageIndicator_Ptr <= LAST_PAGE)) {
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + *lowerPageIndicator_Ptr);
			GLCD_sendCommand(
			COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr);

			for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
				GLCD_sendData(
						(lowerstate[loopCounter]
								& ~((shapes[*shape_Ptr][loopCounter])
										>> ((8 - *rowIndicator_Ptr))))
								| (((shapes[*shape_Ptr][loopCounter])
										>> ((8 - *rowIndicator_Ptr)))));
			}
		}
	}
	sei();
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  EraseShape
 [Description]  :  This function is responsible for drawing the shape at the top of the game zone
 [Args]         :  uint8 *shape_Ptr
 	 	 	 	 	 	 this argument shall contains the address of the array of the shapes formation
 	 	 	 	   uint8 * rowIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the rowIndicator variable
 	 	 	 	   uint8 * upperPageIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the upperPageIndicator variable
 	 	 	 	   uint8 * lowerPageIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the lowerPageIndicator variable
 	 	 	 	   uint8 * columnIndicator_Ptr
 	 	 	 	   	   	 this argument shall conatins the address of the columnIndicator variable
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/


void EraseShape(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *columnIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr) {

	uint8 loopCounter = ZERO;
	uint8 upperstate[SHAPE_WIDTH] = { ZERO };
	uint8 lowerstate[SHAPE_WIDTH] = { ZERO };
	cli();
	GLCD_CTRL_PORT |= (ONE << CS1); /* Select Left half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS2);
	if (*upperPageIndicator_Ptr <= LAST_PAGE) {
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + *upperPageIndicator_Ptr);

		for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
			GLCD_sendCommand(
			COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr + loopCounter);
			upperstate[loopCounter] = GLCD_readData();
			upperstate[loopCounter] = GLCD_readData();

		}

	}

	if (*lowerPageIndicator_Ptr <= LAST_PAGE) {
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + *lowerPageIndicator_Ptr);

		for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
			GLCD_sendCommand(
			COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr + loopCounter);
			lowerstate[loopCounter] = GLCD_readData();
			lowerstate[loopCounter] = GLCD_readData();

		}

	}
	if ((*upperPageIndicator_Ptr <= LAST_PAGE)) {
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + *upperPageIndicator_Ptr);
		GLCD_sendCommand(
		COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr);

		for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
			GLCD_sendData(
					(upperstate[loopCounter]
							& ~((shapes[*shape_Ptr][loopCounter])
									<< (*rowIndicator_Ptr))));
		}

	}
	if ((*lowerPageIndicator_Ptr <= LAST_PAGE)) {
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + *lowerPageIndicator_Ptr);
		GLCD_sendCommand(
		COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr);

		for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
			GLCD_sendData(
					(lowerstate[loopCounter]
							& ~((shapes[*shape_Ptr][loopCounter])
									>> ((8 - *rowIndicator_Ptr)))));
		}

	}
	sei();
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GenerateLocation
 [Description]  :  This function is responsible for drawing the shape at the top of the game zone
 [Args]         :  uint8 *shape_Ptr
 	 	 	 	 	 	 this argument shall contains the address of the array of the shapes formation
 	 	 	 	   uint8 * rowIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the rowIndicator variable
 	 	 	 	   uint8 * upperPageIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the upperPageIndicator variable
 	 	 	 	   uint8 * lowerPageIndicator_Ptr
 	 	 	 	   	   	 this argument shall contains the address of the lowerPageIndicator variable
 	 	 	 	   uint8 * columnIndicator_Ptr
 	 	 	 	   	   	 this argument shall conatins the address of the columnIndicator variable
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void GenerateLocation(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *columnIndicator_Ptr) {
	cli();
	(*rowIndicator_Ptr) = (*rowIndicator_Ptr) + 1;
	if ((*rowIndicator_Ptr == 1) && (*upperPageIndicator_Ptr == STARTED_UPPER_PAGE)
			&& (*lowerPageIndicator_Ptr == STARTED_LOWER_PAGE))
	{
	  loseFlag = ONE;
	}
	if ((*rowIndicator_Ptr == 6) && (*upperPageIndicator_Ptr == 7)
			&& (*lowerPageIndicator_Ptr == 8)) {
		*upperPageIndicator_Ptr = 0;
		*lowerPageIndicator_Ptr = 1;
		*rowIndicator_Ptr = 1;
		(*shape_Ptr)++;
		nextShapeView(&shape);
		if((*shape_Ptr)==9)
		{
			(*shape_Ptr) = ZERO;
		}

	}

	if ((*rowIndicator_Ptr) == 8) {
		(*rowIndicator_Ptr) = 0;
		(*upperPageIndicator_Ptr)++;
		(*lowerPageIndicator_Ptr)++;

	}

	sei();
}

void MoveLeftCallBackFunction(void) {
	moveLeftFlag = ONE;
}

void MoveRightCallBackFunction(void) {
	moveRightFlag = ONE;
}

void TurnShapeCallBackFunction(void)
{
	switch(shape)
	{
	case 0:
		shape = 1;
		break;
	case 1:
		shape = 0;
		break;
	case 2:
		break;
	case 3:
		shape = 4;
		break;
	case 4:
		shape = 3;
		break;
	case 5:
		shape = 8;
		break;
	case 6:
		shape = 7;
		break;
	case 7:
		shape = 5;
		break;
	case 8:
		shape =6;
		break;
	}
}

uint8 checkUpper(uint8 state, uint8 rowIndicator) {

	return state & (0xFF << rowIndicator);
}

uint8 checkLower(uint8 state, uint8 rowIndicator) {
	return state & (0xFF >> (8 - rowIndicator));
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  RowCompleteTranform
 [Description]  :  This function is responsible for drawing the shape at the top of the game zone
 [Args]         :  uint8 rowIndicator
 	 	 	 	   	   	 this argument shall contains the address of the rowIndicator variable
 	 	 	 	   uint8 pageIndicator
 	 	 	 	   	   	 this argument shall contains the address of the current page
 	 	 	 	   uint8 columnIndicator
 	 	 	 	   	   	 this argument shall contains the address of the columnIndicator variable
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void RowCompleteTranform(uint8 rowIndicator, uint8 pageIndicator,
		uint8 columnIndicator) {

	uint8 loopCounter = ZERO;
	uint8 state[BORDER_WIDTH] = { ZERO };
	uint8 newColumn = ZERO;
	cli();

	GLCD_sendCommand(PAGE_SETTING_ADDRESS + pageIndicator);
	GLCD_sendCommand(
	COLUMN_SETTING_ADDRESS + LEFT_BORDER + ONE);
	for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE; loopCounter++) {

		state[loopCounter] = GLCD_readData();
		state[loopCounter] = GLCD_readData();
	}
	GLCD_sendCommand(
	COLUMN_SETTING_ADDRESS + columnIndicator);
	for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE; loopCounter++) {

		newColumn = ((state[loopCounter] & (0xFF >> (rowIndicator - 1))) << 1);
		state[loopCounter] = (state[loopCounter] & (0xFF << (rowIndicator)));
		GLCD_sendData(newColumn | state[loopCounter]);

	}

}

void nextShapeView(uint8 * shape_Ptr)
{
	uint8 loopCounter = ZERO;
	uint8 shape = ZERO;
	GLCD_CTRL_PORT |= (ONE << CS2); /* Select Right half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS1);
	GLCD_sendCommand(COLUMN_SETTING_ADDRESS + SHAPE_OUTPUT_ADDRESS ); /* Set column address) */
	GLCD_sendCommand(PAGE_SETTING_ADDRESS + PAGE_1); /* Set page address */
	if((*shape_Ptr)+1 == 9)
	{
		shape=ZERO;
	}
	else
	{
		shape = (*shape_Ptr) + 1;
	}
	for(loopCounter=ZERO;loopCounter<SHAPE_WIDTH;loopCounter++)
	{
		GLCD_sendData(shapes[(shape)][loopCounter]);
	}
}
