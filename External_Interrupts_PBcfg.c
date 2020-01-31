/*******************************************************************************************************
 *  [FILE NAME]   :      <External_Interrupts_PBcfg.c>                                                       *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                             *
 *  [DATE CREATED]:      <Dec 4, 2019>                                                                 *
 *  [Description} :      <source file for external interrupts Postbuild configuratio                   *
 ******************************************************************************************************/
/*-------------------------------------------INCLUDES---------------------------------*/

#include "External_interrupts.h"


/*-----------------------------------interruptsConfig_Structure-----------------------*/
	E_Interrupts_configType interruptsConfig = { FALLING_EDGE_INT0,
			FALLING_EDGE_INT1, FALLING_EDGE_INT2 }; /* setting the Interrupt Intialized Structure */
