/************************************************************************************************************************
 *  [FILE NAME]   :      <shapes.h>                                                                                       *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 26, 2020>                                                                                 *
 *  [Description} :      <Header files contains the shapes formation>                                                   *
 ************************************************************************************************************************/

#ifndef SHAPES_H_
#define SHAPES_H_
#include "std_types.h"

const uint8 shapes[11][8] = {
		/*
		 *       x x x x x
		 *       x x x x x
		 *       x x x x x
		 *       x x x x x
		 * x x x x x
		 * x x x x x
		 * x x x x x
		 * x x x x x
		 */
		{
				0xF0,0xF0,0xF0,0xFF,0xFF,0x0F,0x0F,0x0F
		},
		/*
		 * x x x x x
		 * x x x x x
		 * x x x x x
		 * x x x x x
		 *       x x x x x
		 *       x x x x x
		 *       x x x x x
		 *       x x x x x
		 */
		{
				0x0F,0x0F,0x0F,0xFF,0xFF,0xF0,0xF0,0xF0
		},
		/*
		 *
		 *
		 *
		 *
		 * x x x x
		 * x x x x
		 * x x x x
		 * x x x x
		 */
		{
				0x0F,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x00
		},

		/*
		 * x
		 * x
		 * x
		 * x
		 * x
		 * x
		 * x
		 * x
		 */
		{
				0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
		/*
		 *
		 *
		 *
		 *
		 *
		 *
		 *
		 * x x x x x x x x
		 */
		{
				0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
		},
		/*
		 * x x x x x x
		 * x x x x x x
		 * x x
		 * x x
		 * x x
		 * x x
		 * x x x x x x
		 * x x x x x x
		 */
		{
				0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0x00, 0x00
		},
		/*
		 * x x x x x x
		 * x x x x x x
		 *         x x
		 *         x x
		 *         x x
		 *         x x
		 * x x x x x x
		 * x x x x x x
		 */
		{
				0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0x00, 0x00
		},
		/*
		 * x x x x x x x x
		 * x x x x x x x x
		 * x x         x x
		 * x x         x x
		 * x x         x x
		 * x x         x x
		 *
		 *
		 */
        {
        		0x3F,0x3F,0x03,0x03,0x03,0x03,0x3F,0x3F
        },
		/*
		 *
		 * x x         x x
		 * x x         x x
		 * x x         x x
		 * x x         x x
		 * x x x x x x x x
		 * x x x x x x x x
	     */
		{
				0xFC, 0xFC, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC
		},

		{

        },

		{

		}};

#endif /* SHAPES_H_ */
