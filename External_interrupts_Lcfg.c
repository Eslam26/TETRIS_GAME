/************************************************************************************************************************
 *  [FILE NAME]   :      <External_interrupts_Lcfg.c>                                                                   *
 *  [AUTHOR]      :      <Eslam EL-Naggar>                                                                              *
 *  [DATE CREATED]:      <JAN 26, 2020>                                                                                 *
 *  [Description} :      <Source file for link configuration for External Interrupts driver>                            *
 ************************************************************************************************************************/

#include "External_interrupts.h"
extern E_Interrupts_configType interruptsConfig;
	E_Interrupts_configType interruptsConfig = { FALLING_EDGE_INT0,
			FALLING_EDGE_INT1, OFF_INT2 }; /* setting the Interrupt Intialized Structure */
