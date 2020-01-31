/************************************************************************************************************************
 *  [FILE NAME]   :      <shapes_PBcfg.c>                                                                               *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 26, 2020>                                                                                 *
 *  [Description} :      <source file contains the array of shapes formation>                                           *
 ************************************************************************************************************************/

/*-----------------------------------------INCLUDES---------------------------------------*/
#include "std_types.h"
#include "shapes.h"

/*------------------------------------------SHAPES_FORMATION_ARRAY------------------------*/
const uint8 shapes[NUM_SHAPES][SHAPE_WIDTH] = {
/*
 *        x x x x
 *        x x x x
 *        x x x x
 *        x x x x
 * x x x x
 * x x x x
 * x x x x
 * x x x x
 */
{ 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F },
/*
 * x x x x
 * x x x x
 * x x x x
 * x x x x
 *        x x x x
 *        x x x x
 *        x x x x
 *        x x x x
 */
{ 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0 },
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
{ 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00 },

/*
 * x x
 * x x
 * x x
 * x x
 * x x
 * x x
 * x x
 * x x
 */
{ 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
/*
 *
 *
 *
 *
 *
 *
 * x x x x x x x x
 * x x x x x x x x
 */
{ 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0 },
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
{ 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0x00, 0x00 },
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
{ 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0x00, 0x00 },
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
{ 0x3F, 0x3F, 0x03, 0x03, 0x03, 0x03, 0x3F, 0x3F },
/*
 *
 * x x         x x
 * x x         x x
 * x x         x x
 * x x         x x
 * x x x x x x x x
 * x x x x x x x x
 */
{ 0xFC, 0xFC, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC } };

