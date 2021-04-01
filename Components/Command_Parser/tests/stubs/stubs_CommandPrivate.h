/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */

#ifndef STUBS_COMMANDRIVATE_H_
#define STUBS_COMMANDPRIVATE_H_

Std_Err step_forward_GCode(SystemCommand* cmd);
Std_Err remove_forward_GCode(SystemCommand* cmd);
Std_Err step_U00(SystemCommand* cmd);
Std_Err step_U01(SystemCommand* cmd);
Std_Err step_U02(SystemCommand* cmd);

#endif /* STUBS_COMMANDPRIVATE_H_ */