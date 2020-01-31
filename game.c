/************************************************************************************************************************
 *  [FILE NAME]   :      <game.c>                                                                                       *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 25, 2020>                                                                                 *
 *  [Description} :      <Source file for the GAME>                                                                     *
 ************************************************************************************************************************/

/*-------------------------------------------------INCLUDES-------------------------------------------------------------*/

#include "game.h"

/*--------------------------------------------FUNCTIONS_DECLARATIONS-----------------------------------------------------*/
static void Setting_Interrupts(void);
static void GameInit(void);
static void GameIntiate(uint8 gamePlay, uint8 *shape_Ptr);
static void MoveRightCallBackFunction(void);
static void MoveLeftCallBackFunction(void);
static void RotateShapeCallBackFunction(void);

/*-----------------------------------------------GLOBAL VARIABLES---------------------------------------------------------*/
/* initialize the global variables */
uint8 moveLeftFlag = ZERO; /* this variable is set to one when user wants to move the shape to the left  */
uint8 moveRightFlag = ZERO; /* this variable is set to one when user wants to move the shape to the right */
uint8 loseFlag = ZERO; /* this variable is set to one when user loses the game                       */
uint8 shape = STARTED_SHAPE; /* this variable holds the current shape number                               */
uint8 rotateShapeFlag = ZERO; /* this variable is set to one when user wants to rotate the shape            */
uint8 nextShape = ZERO; /* this variable holds the value of the next shape */

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  main
 [Description]  :  This function is the main function of the program
 [Returns]      :  This function returns Zero
 ----------------------------------------------------------------------------------------------------*/
int main() {

	/* Initialize the main local varibles */
	uint8 rowIndicator = STARTED_ROW; /* started row  */
	uint8 columnIndicator = STARTED_COLUMN; /* started column */
	uint8 upperPageIndicator = STARTED_UPPER_PAGE; /* started upper page */
	uint8 lowerPageIndicator = STARTED_LOWER_PAGE; /* started lower page */
	uint8 nextShapeFlag = OFF; /* this variable is set to one when the shape collides with another shape */
	uint8 prevRow = rowIndicator; /* stores previous row number */
	uint8 prevColumn = columnIndicator; /* stores previous column number */
	uint8 prevUpperPage = upperPageIndicator; /* stores previous upper page number */
	uint8 prevLowerPage = lowerPageIndicator; /* stores previous lower page number */
	uint8 upperState[SHAPE_WIDTH] = { ZERO }; /* array for storing the state of upper columns */
	uint8 lowerState[SHAPE_WIDTH] = { ZERO }; /* array for storing the state of lower columns */
	uint8 gamePlay = ON; /* sets Game mode " ON "*/

	Setting_Interrupts(); /* intializes and sets the interrupts parameters */
	GameInit(); /* intializes the game interface */

	while (ONE) {
		while (gamePlay) { /* looping while gamePlay mode is ON  */

			/* displaying the current shape in specific location (specific row, specific column, specific page ) */
			DrawShape(&shape, &rowIndicator, &columnIndicator,
					&lowerPageIndicator, &upperPageIndicator, &nextShapeFlag,
					lowerState, upperState);

			/* checking on the shape collides with another shape or not */
			switch (nextShapeFlag) {
			case ONE:
				/* current shape collides with another shape */
				/* set the nextShapeFlag variable again to Zero */
				nextShapeFlag = ZERO;
				/* displaying the last position of the shape before collision */
				DrawShape(&shape, &prevRow, &prevColumn, &prevLowerPage,
						&prevUpperPage, &nextShapeFlag, lowerState, upperState);
				/* switch to the next shape */
				completeCheck(&rowIndicator, &upperPageIndicator);

				shape = nextShape;
				/* displaying the next shape on the screen */
				nextShape = NextShapeView(&shape);

				/* check if it is the last shape or not */
				if ((shape) == NUM_SHAPES) {
					(shape) = ZERO; /* let the current shape is the first shape */
				}

				/* setting the location of the current shape to start from the top */
				rowIndicator = STARTED_ROW;
				upperPageIndicator = STARTED_UPPER_PAGE;
				lowerPageIndicator = STARTED_LOWER_PAGE;
				columnIndicator = STARTED_COLUMN;
				break;

			case ZERO:
				/*  current shape does not collide
				 *  checks if the current location is the last location or not
				 *  if true skip the erasing shape function
				 *  if false erase the current shape
				 */
				if ((rowIndicator == 7)
						&& (upperPageIndicator == LOCATION_LAST_UPPER_PAGE)
						&& (lowerPageIndicator == LOCATION_LAST_LOWER_PAGE)) {

				} else {
					/* erasing the current shape */
					EraseShape(&shape, &rowIndicator, &columnIndicator,
							&lowerPageIndicator, &upperPageIndicator,
							lowerState, upperState);

					/* storing the location of the last shape erased */
					prevRow = rowIndicator;
					prevColumn = columnIndicator;
					prevUpperPage = upperPageIndicator;
					prevLowerPage = lowerPageIndicator;
					_delay_ms(DELAY_AFTER_ERASING_TIME);

				}

				/* generates the new location of the next shape */
				GenerateLocation(&shape, &rowIndicator, &upperPageIndicator,
						&lowerPageIndicator, &columnIndicator);
				break;

			}

			/* checking if the user loses or not  */
			switch (loseFlag) {
			/* in case of losing the game */
			case LOST:
				/* set the loseFlag againg to zero
				 * and set the gamePlay mode to "OFF"
				 */
				gamePlay = OFF;
				loseFlag = ZERO;
				break;
				/* in case of not losing the game */
			case NOT_LOST:
				/* checks if the user presses the moveLeft button or not */
				if (moveLeftFlag == ONE) {

					columnIndicator = columnIndicator - TWO;
					checkAfterMoving(&columnIndicator);

					moveLeftFlag = ZERO;
				}
				/* checks if the user presses the moveRight button or not */
				if (moveRightFlag == ONE) {

					columnIndicator = columnIndicator + TWO;
					checkAfterMoving(&columnIndicator);

					moveRightFlag = ZERO;
				}
				/* checks if the user presses the rotate button or not */
				if (rotateShapeFlag == ONE) {

					rotateShapeFlag = ZERO;
					/* rotates the shape */
					RotateShape(&columnIndicator);
				}

				break;
			}

		}
		cli();
		LosingFunction();
	}
	return ZERO;
}

/*----------------------------------------------------------FUNCTIONS_DEFINITIONS-------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GameInit
 [Description]  :  This function is responsible for intializing the game
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
static void GameInit(void) {

	GLCD_Init(); /* intialize GLCD Driver */
	GLCD_clearScreen(); /* clear the GLCD Screen */
	GLCD_displayString(PAGE_3, "         Welcome               Tetris Game");
	_delay_ms(400);
	GLCD_clearScreen(); /* clear the GLCD Screen */

	GameIntiate(0, &shape); /* initiate the game */
	sei();
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  Setting_Interrupts
 [Description]  :  This function is responsible for intializing the external interrupts
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
static void Setting_Interrupts(void) {

	ExternalInterrupts_INT0_setCallBack(MoveLeftCallBackFunction); /* setting INT0 callback function */
	ExternalInterrupts_INT1_setCallBack(MoveRightCallBackFunction);/* setting INT1 callback function */
	ExternalInterrupts_INT2_setCallBack(RotateShapeCallBackFunction);
	ExternalInterrupts_init(&interruptsConfig); /* intialize the External Interrupts Driver */
}
/*----------------------------------------------------------------------------------------------------
 [Function Name]:  GameIntiate
 [Description]  :  This function is responsible for initiating the game and displaying the main borders
 	 	 	 	 	 	 of the game.
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

static void GameIntiate(uint8 gamePlay, uint8 *shape_Ptr) {
	uint8 page = ZERO;
	uint8 loopCounter = ZERO;

	cli();
	/* Setting up records place */
	GLCD_CTRL_PORT |= (ONE << CS2); /* Select Left half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS1);
	GLCD_displayString(PAGE_1, "              ->Shape:");
	nextShape = NextShapeView(shape_Ptr); /* displaying the next shape */

	/* generating border of the game */
	GLCD_CTRL_PORT |= (ONE << CS1); /* Select Right half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS2);
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
 [Function Name]:  MoveLeftCallBackFunction
 [Description]  :  This function is call back function when moveLeft button is pressed
 [Returns]      :  This function returns the masked part of the state
 ----------------------------------------------------------------------------------------------------*/

static void MoveLeftCallBackFunction(void) {
	/* set the moveLeft flag to one */
	moveLeftFlag = ONE;
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  MoveRightCallBackFunction
 [Description]  :  This function is call back function when moveRight button is pressed
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
static void MoveRightCallBackFunction(void) {
	/* set the moveRight flag to one */
	moveRightFlag = ONE;
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  RotateShapeCallBackFunction
 [Description]  :  This function is responsible is called when INT2 is triggred
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
static void RotateShapeCallBackFunction(void) {
	/* set the roteteShape flag to one */
	rotateShapeFlag = ONE;
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  checkUpper
 [Description]  :  This function is responsible for returning the masking part of the state column
 [Returns]      :  This function returns the masked part of the state
 ----------------------------------------------------------------------------------------------------*/
uint8 checkUpper(uint8 state, uint8 rowIndicator) {
	return (uint8) (state & (HIGH_8BITS << rowIndicator));
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  checkLower
 [Description]  :  This function is responsible for returning the masking part of the state column
 [Returns]      :  This function returns the masked part of the state
 ----------------------------------------------------------------------------------------------------*/
uint8 checkLower(uint8 state, uint8 rowIndicator) {
	return (uint8) (state & (HIGH_8BITS >> (NUM_BITS_IN_BYTE - rowIndicator)));
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  completeCheck
 [Description]  :  This function is responsible for checking
 	 	 	 	 	 	 the completion of the row or page according the configuration of ROW_TRANSFER
 	 	 	 	 	 	 (ROW_TRANSFER) is defined as ON, function will check on Row Completition
 	 	 	 	 	 	 (ROW_TRANSFER) is defined as OFF, function will check on Page completition

 [Args]         :  uint8 * rowIndicator_Ptr
 	 	 	 	 	 	 this argument shall contains the address of the rowIndicator variable
 	 	 	 	   uint8 * upperPageIndicator_Ptr
 	 	 	 	 	 	 this argument shall contains the address of the upperPageIndicator variable

 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void completeCheck(uint8 *rowIndicator_Ptr, uint8 *upperPageIndicator_Ptr) {
#if ROW_TRANSFORM == OFF
	uint8 loopCounter = ZERO;
	uint8 state = ZERO;
	uint8 counter = ZERO;
	/* set the page address */
	GLCD_sendCommand(PAGE_SETTING_ADDRESS + *upperPageIndicator_Ptr);

	/* set the column address */
	GLCD_sendCommand(
	COLUMN_SETTING_ADDRESS + LEFT_BORDER + ONE);
	for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE;
			loopCounter++) {

		state = GLCD_readData();
		state = GLCD_readData();
		if(state == HIGH_8BITS)
		{
			counter++;
		}
	}
	if(counter == (BORDER_WIDTH - ONE))
	{
		RowCompleteTranform(*rowIndicator_Ptr, *upperPageIndicator_Ptr);
	}

#endif
#if ROW_TRANSFORM== ON
	uint8 loopCounter = ZERO;
	uint8 state = ZERO;
	uint8 rowCounter = ZERO;
	uint8 counter[NUM_BITS_IN_BYTE] = { ZERO };

	/* set the page address */
	GLCD_sendCommand(PAGE_SETTING_ADDRESS + *upperPageIndicator_Ptr);
	/* set the column address */
	GLCD_sendCommand(
	COLUMN_SETTING_ADDRESS + LEFT_BORDER + ONE);
	for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE; loopCounter++) {

		state = GLCD_readData();
		state = GLCD_readData();
		for (rowCounter = ZERO; rowCounter < NUM_BITS_IN_BYTE; rowCounter++) {
			if (BIT_IS_SET(state, rowCounter)) {
				counter[rowCounter]++;
			}
		}
	}
	for (rowCounter = NUM_BITS_IN_BYTE; rowCounter > ZERO; rowCounter--) {
		if (counter[rowCounter - ONE] == (BORDER_WIDTH - ONE)) {
			if (rowCounter - ONE == SEVEN && *upperPageIndicator_Ptr == SEVEN) {

			} else {
				RowCompleteTranform(rowCounter - ONE, *upperPageIndicator_Ptr);
			}

		}
	}

#endif

}
/*----------------------------------------------------------------------------------------------------
 [Function Name]:  RowCompleteTranform
 [Description]  :  This function is responsible for checking
 	 	 	 	 	 	 the completion of the row or page according the configuration of ROW_TRANSFER
 	 	 	 	 	 	 (ROW_TRANSFER) is defined as ON, function will check on Row Completition
 	 	 	 	 	 	 (ROW_TRANSFER) is defined as OFF, function will check on Page completitio
 [Args]         :  uint8 rowIndicator
 	 	 	 	 	 	 this argument shall contains the address of the rowIndicator variable
                   uint8 pageIndicator
 	 	 	 	 	 	 this argument shall contains the address of the current page
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void RowCompleteTranform(uint8 rowIndicator, uint8 pageIndicator) {
#if ROW_TRANSFORM == ON
	/* intialize local variables */
	uint8 loopCounter = ZERO; /* this variable is responsible for looping counter */

	/* this variable is responsible for storing the new state of the column */
	uint8 newColumn = ZERO;
	uint8 page = ZERO;
	uint8 upperRow = ZERO;
	uint8 upperPagePart[BORDER_WIDTH] = { ZERO };
	uint8 lowerPagePart[BORDER_WIDTH] = { ZERO };
	cli();

	page = pageIndicator;

	while (page >= ONE) {

		/* set the page address */
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + page);

		/* set the column address */
		GLCD_sendCommand(
		COLUMN_SETTING_ADDRESS + LEFT_BORDER + ONE);

		for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE;
				loopCounter++) {

			upperPagePart[loopCounter] = GLCD_readData();
			upperPagePart[loopCounter] = GLCD_readData();
		}
		/* set the column address */

		if (page + ONE == EIGHT) {
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + page);
		} else {
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + page + ONE);
		}

		GLCD_sendCommand(
		COLUMN_SETTING_ADDRESS + LEFT_BORDER + ONE);
		for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE;
				loopCounter++) {

			lowerPagePart[loopCounter] = GLCD_readData();
			lowerPagePart[loopCounter] = GLCD_readData();
		}
		if (page + ONE == EIGHT) {
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + page);
		} else {
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + page + ONE);
			GLCD_sendCommand(
			COLUMN_SETTING_ADDRESS + LEFT_BORDER + ONE);
			/* displaying the new column state by ORing the new part with the remaining the old part*/
			for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE;
					loopCounter++) {

				upperRow = (upperPagePart[loopCounter] & 0x80)
						>> LAST_ROW_IN_PAGE;
				newColumn = ((lowerPagePart[loopCounter]
						& (HIGH_8BITS >> (rowIndicator + ONE))) << ONE);
				lowerPagePart[loopCounter] = (lowerPagePart[loopCounter]
						& (HIGH_8BITS << (rowIndicator + ONE)));
				GLCD_sendData(
						(newColumn | lowerPagePart[loopCounter]) | upperRow);

			}
		}

		page--;
		rowIndicator = LAST_ROW_IN_PAGE;
	}

#elif ROW_TRANSFORM == OFF
	/* intialize local variables */
	uint8 loopCounter = ZERO; /* this variable is responsible for looping counter */

	/* this variable is responsible for storing the state of the column along the border width */
	uint8 state[BORDER_WIDTH] = { ZERO };

	uint8 page = pageIndicator;
	cli();

	while(page>=ZERO)
	{
		/* set the page address */
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + page);

		/* set the column address */
		GLCD_sendCommand(
		COLUMN_SETTING_ADDRESS + LEFT_BORDER + ONE);

		for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE; loopCounter++) {

			state[loopCounter] = GLCD_readData();
			state[loopCounter] = GLCD_readData();
			if(page == ZERO)
			{
				state[loopCounter] = state[loopCounter] & 0b11111110;
			}
		}
		/* set the column address */
		GLCD_sendCommand(
		COLUMN_SETTING_ADDRESS + LEFT_BORDER + ONE);
		if(page+ONE == EIGHT)
		{
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + page );
		}
		else
		{
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + page + ONE);
		}

		/* displaying the new column state by ORing the new part with the remaining the old part*/
		for (loopCounter = ONE; loopCounter <= BORDER_WIDTH - ONE; loopCounter++) {


			if(page == SEVEN)
			{
				GLCD_sendData((((state[loopCounter])>>ONE) | 0x80));
			}
			else
			{
				GLCD_sendData(state[loopCounter]);
			}

		}
		page--;
	}

#endif

}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  LosingFunction
 [Description]  :  This function is responsible for generating the exit message
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void LosingFunction(void) {

	GLCD_sendCommand(COLUMN_SETTING_ADDRESS); /* Set Y address (column=0) */
	GLCD_sendCommand(PAGE_SETTING_ADDRESS); /* Set x address (page=0) */
	GLCD_CTRL_PORT |= (ONE << CS1); /* Select Right half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS2);
	/* clear the graphical LCD screen */
	GLCD_clearScreen();
	GLCD_CTRL_PORT |= (ONE << CS2); /* Select Left half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS1);
	/* Display the " Sorry, you have lost the game " sentence */
	GLCD_displayString(PAGE_2, "Sorry, you have lost  the game :')");

	_delay_ms(ONE_SECOND);
	/* clear the graphical LCD screen */
	GLCD_clearScreen();
	/* Display the "Thank you" sentence */
	GLCD_displayString(PAGE_2, "      Thank You :')");
	_delay_ms(ONE_SECOND);
	/* reaches the end of the game */
	while (ONE) {

	}
}

