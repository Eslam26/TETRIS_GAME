/*
 * shapes.c
 *
 *  Created on: Jan 31, 2020
 *      Author: eslamelnaggar
 */

/*------------------------------------------------------INCLUDES-------------------------------------------------------*/
#include "shapes.h"


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
 uint8 * lowerState_Ptr
 this argument shall contains the address of the array of lower page state of columns
 uint8 * upperState_Ptr
 this argument shall contains the address of the array of upper page state of columns
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void DrawShape(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *columnIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr, uint8 *nextShapeFlag_Ptr,
		uint8 *lowerState_Ptr, uint8 *upperState_Ptr) {

	uint8 loopCounter = ZERO;

	cli();
	/* Select Right half of display */
	GLCD_CTRL_PORT |= (ONE << CS1);
	GLCD_CTRL_PORT &= ~(ONE << CS2);

	/* check on the page number to be sure that is a valid page number */
	if (*upperPageIndicator_Ptr <= LAST_PAGE) {
		/* set the page number */
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + *upperPageIndicator_Ptr);

		/* looping along the shape width */
		for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
			/* set the column address */
			GLCD_sendCommand(
			COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr + loopCounter);
			/* read the state of the column */
			upperState_Ptr[loopCounter] = GLCD_readData();
			/* one dummy read is required to get the state*/
			upperState_Ptr[loopCounter] = GLCD_readData();
			/* check if the upper part of the shape collides with another shape or not */
			if (checkUpper(upperState_Ptr[loopCounter], *rowIndicator_Ptr)
					& (((shapes[*shape_Ptr][loopCounter]) << (*rowIndicator_Ptr)))) {

				/* set the nextShapeFlag to ONE ( indication of the collision ) */
				*nextShapeFlag_Ptr = ONE;
				/* break the loop */
				break;
			}

		}
	}
	/* check if the shape collides or not */
	if (*nextShapeFlag_Ptr == ONE) {

	} else {
		/* if it doesn't collide, check the lower part of the shape collides or not */
		/* check on the page number to be sure that is a valid page number */
		if (*lowerPageIndicator_Ptr <= LAST_PAGE) {
			/* set the page address */
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + *lowerPageIndicator_Ptr);

			for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
				/* set the column address */
				GLCD_sendCommand(
				COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr + loopCounter);
				/* read the state of the column */
				lowerState_Ptr[loopCounter] = GLCD_readData();
				/* one dummy read is required to get the state*/
				lowerState_Ptr[loopCounter] = GLCD_readData();
				/* check if the upper part of the shape collides with another shape or not */
				if (checkLower(lowerState_Ptr[loopCounter], *rowIndicator_Ptr)
						& (((shapes[*shape_Ptr][loopCounter])
								>> ((NUM_BITS_IN_BYTE - *rowIndicator_Ptr))))) {
					/* set the nextShapeFlag to ONE ( indication of the collision ) */
					*nextShapeFlag_Ptr = ONE;
					/* break the loop */
					break;
				}
			}
		}
	}
	/* check if the shape collides or not
	 * if yes, skip display the current shape
	 * if not, displaying the current shape
	 */
	if (*nextShapeFlag_Ptr == ONE) {

		/* if current page is the top page, this means that the player losses the game */
		if (*upperPageIndicator_Ptr == STARTED_UPPER_PAGE) {
			/* set the loseFlag to ONE ( indication of losing the game ) */
			loseFlag = ONE;
		}

	} else {

		/* check on the page number to be sure that is a valid page number */
		if ((*upperPageIndicator_Ptr <= LAST_PAGE)) {
			/* set the page address */
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + *upperPageIndicator_Ptr);
			/* set the column address */
			GLCD_sendCommand(
			COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr);

			for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
				/*display each column of the shape */
				GLCD_sendData(
						(upperState_Ptr[loopCounter]
								& ~((shapes[*shape_Ptr][loopCounter])
										<< (*rowIndicator_Ptr)))
								| (((shapes[*shape_Ptr][loopCounter])
										<< (*rowIndicator_Ptr))));
			}
		}
		/* check on the page number to be sure that is a valid page number */
		if ((*lowerPageIndicator_Ptr <= LAST_PAGE)) {
			/* set the page address */
			GLCD_sendCommand(PAGE_SETTING_ADDRESS + *lowerPageIndicator_Ptr);
			/* set the column address */
			GLCD_sendCommand(
			COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr);

			for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
				/*display each column of the shape */
				GLCD_sendData(
						(lowerState_Ptr[loopCounter]
								& ~((shapes[*shape_Ptr][loopCounter])
										>> ((NUM_BITS_IN_BYTE
												- *rowIndicator_Ptr))))
								| (((shapes[*shape_Ptr][loopCounter])
										>> ((NUM_BITS_IN_BYTE
												- *rowIndicator_Ptr)))));
			}
		}
		_delay_ms(DELAY_AFTER_DRAWING_TIME);
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
 this argument shall contains the address of the columnIndicator variable
 uint8 * lowerState_Ptr
 this argument shall contains the address of the array of lower page state of columns
 uint8 * upperState_Ptr
 this argument shall contains the address of the array of upper page state of columns
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void EraseShape(uint8 *shape_Ptr, uint8 *rowIndicator_Ptr,
		uint8 *columnIndicator_Ptr, uint8 *lowerPageIndicator_Ptr,
		uint8 *upperPageIndicator_Ptr, uint8 *lowerState_Ptr,
		uint8 *upperState_Ptr) {

	uint8 loopCounter = ZERO;
	cli();
	GLCD_CTRL_PORT |= (ONE << CS1); /* Select Right half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS2);

	/* check on the page number to be sure that is a valid page number */
	if ((*upperPageIndicator_Ptr <= LAST_PAGE)) {
		/* set the page address */
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + *upperPageIndicator_Ptr);
		/* set the column address */
		GLCD_sendCommand(
		COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr);

		for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
			/* erasing the upper part of the shape */
			GLCD_sendData(
					(upperState_Ptr[loopCounter]
							& ~((shapes[*shape_Ptr][loopCounter])
									<< (*rowIndicator_Ptr))));
		}

	}
	/* check on the page number to be sure that is a valid page number */
	if ((*lowerPageIndicator_Ptr <= LAST_PAGE)) {
		/* set the page address */
		GLCD_sendCommand(PAGE_SETTING_ADDRESS + *lowerPageIndicator_Ptr);
		/* set the column address */
		GLCD_sendCommand(
		COLUMN_SETTING_ADDRESS + *columnIndicator_Ptr);

		for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
			/* erasing the lower part of the shape */
			GLCD_sendData(
					(lowerState_Ptr[loopCounter]
							& ~((shapes[*shape_Ptr][loopCounter])
									>> ((NUM_BITS_IN_BYTE - *rowIndicator_Ptr)))));
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

	/* row numbers will be 1,3,5,7 */
	(*rowIndicator_Ptr) = (*rowIndicator_Ptr) + 2;

	/* check if the row number is out of range
	 * if yes, we have to increment the pages number
	 */
	if ((*rowIndicator_Ptr) == NINE) {

		(*rowIndicator_Ptr) = ONE;
		(*upperPageIndicator_Ptr)++;
		(*lowerPageIndicator_Ptr)++;

	}

	/* check if the row number is last row and the last upper page
	 * and the lower page is out of the number
	 *  this means, the current shape reaches to the bottom of the border
	 *  so, we have to start again with new shape
	 */
	if ((*rowIndicator_Ptr == SEVEN) && (*upperPageIndicator_Ptr == SEVEN)
			&& (*lowerPageIndicator_Ptr == EIGHTH)) {
		*upperPageIndicator_Ptr = STARTED_UPPER_PAGE;
		*lowerPageIndicator_Ptr = STARTED_LOWER_PAGE;
		*rowIndicator_Ptr = STARTED_ROW;
		(*shape_Ptr)++;
		(*columnIndicator_Ptr) = STARTED_COLUMN;
		NextShapeView(&shape);
		if ((*shape_Ptr) == NUM_SHAPES) {
			(*shape_Ptr) = ZERO;
		}

	}


	sei();
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  RotateShape
 [Description]  :  This function is responsible for setting the number of the new shape after rotating
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/
void RotateShape(uint8 *columnIndicator_Ptr) {
	switch (shape) {
	case FIRST_SHAPE:
		shape = SECOND_SHAPE;
		break;
	case SECOND_SHAPE:
		shape = FIRST_SHAPE;
		break;
	case THIRD_SHAPE:

		break;
	case FOURTH_SHAPE:
		shape = FIFTH_SHAPE;
		break;
	case FIFTH_SHAPE:
		shape = FOURTH_SHAPE;
		break;
	case SIXTH_SHAPE:
		shape = EIGHTH_SHAPE;
		break;
	case SEVENTH_SHAPE:
		shape = NINGTH_SHAPE;
		break;
	case EIGHTH_SHAPE:
		shape = SEVENTH_SHAPE;
		break;
	case NINGTH_SHAPE:
		shape = SIXTH_SHAPE;
		break;
	}
	checkAfterMoving(columnIndicator_Ptr);

}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  NextShapeView
 [Description]  :  This function is responsible for drawing the next shape
 [Args]         :  uint8 * shape_Ptr
 this argument shall contains the address of the next shape
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void NextShapeView(uint8 *shape_Ptr) {
	uint8 loopCounter = ZERO;
	uint8 shape = ZERO;
	GLCD_CTRL_PORT |= (ONE << CS2); /* Select Left half of display */
	GLCD_CTRL_PORT &= ~(ONE << CS1);
	GLCD_sendCommand(COLUMN_SETTING_ADDRESS + SHAPE_OUTPUT_ADDRESS); /* Set column address) */
	GLCD_sendCommand(PAGE_SETTING_ADDRESS + PAGE_2); /* Set page address */

	if ((*shape_Ptr) + ONE == NUM_SHAPES) {
		shape = FIRST_SHAPE;
	} else {
		shape = (*shape_Ptr) + ONE;
	}
	/* displaying the current shape */
	for (loopCounter = ZERO; loopCounter < SHAPE_WIDTH; loopCounter++) {
		GLCD_sendData(shapes[(shape)][loopCounter]);
	}
}

/*----------------------------------------------------------------------------------------------------
 [Function Name]:  checkAfterMoving
 [Description]  :  This function is responsible for checking and adjusting
 the correct number of columns of each shape
 [Args]         :  uint8 *columnIndicator_Ptr
 this argument shall contains the address of the columnIndicator variable
 [Returns]      :  This function returns void
 ----------------------------------------------------------------------------------------------------*/

void checkAfterMoving(uint8 *columnIndicator_Ptr) {

	switch (shape) {
	case THIRD_SHAPE:
		if (*columnIndicator_Ptr > RIGHT_MARGIN_THIRD_SHAPE) {
			*columnIndicator_Ptr = RIGHT_MARGIN_THIRD_SHAPE;
		}
		if (*columnIndicator_Ptr < LEFT_MARGIN_ALL_SHAPES) {
			*columnIndicator_Ptr = LEFT_MARGIN_ALL_SHAPES;
		}

		break;
	case FOURTH_SHAPE:
		if (*columnIndicator_Ptr > RIGHT_MARGIN_FOURTH_SHAPE) {
			*columnIndicator_Ptr = RIGHT_MARGIN_FOURTH_SHAPE;
		}
		if (*columnIndicator_Ptr < LEFT_MARGIN_ALL_SHAPES) {
			*columnIndicator_Ptr = LEFT_MARGIN_ALL_SHAPES;
		}
		break;
	case FIRST_SHAPE:
	case SECOND_SHAPE:
		if (*columnIndicator_Ptr > RIGHT_MARGIN_FIRST_SECOND_SHAPE) {
			*columnIndicator_Ptr = RIGHT_MARGIN_FIRST_SECOND_SHAPE;
		}
		if (*columnIndicator_Ptr < LEFT_MARGIN_ALL_SHAPES) {
			*columnIndicator_Ptr = LEFT_MARGIN_ALL_SHAPES;
		}
		break;

	case SIXTH_SHAPE:
	case SEVENTH_SHAPE:
		if (*columnIndicator_Ptr > RIGHT_MARGIN_SIXTH_SEVENTH_SHAPE) {
			*columnIndicator_Ptr = RIGHT_MARGIN_SIXTH_SEVENTH_SHAPE;
		}
		if (*columnIndicator_Ptr < LEFT_MARGIN_ALL_SHAPES) {
			*columnIndicator_Ptr = LEFT_MARGIN_ALL_SHAPES;
		}
		break;
	case FIFTH_SHAPE:
	case EIGHTH_SHAPE:
	case NINGTH_SHAPE:
		if (*columnIndicator_Ptr > RIGHT_MARGINE_REST_SHAPES) {
			*columnIndicator_Ptr = RIGHT_MARGINE_REST_SHAPES;
		}
		if (*columnIndicator_Ptr < LEFT_MARGIN_ALL_SHAPES) {
			*columnIndicator_Ptr = LEFT_MARGIN_ALL_SHAPES;
		}
		break;
	}

}
