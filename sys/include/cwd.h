/*
 * cwd.h
 *
 *  Created on: 2023. dec. 22.
 *      Author: Balint
 */

#ifndef __CWD_H__
#define __CWD_H__

/**
 * @brief  Initialize the CWD
 *         Creates the _cwd_mutex, clears _cwd
 *
 * @param  None
 *
 * @retval None
 */
void cwd_init(void);

/**
 * @brief  De-initialize the CWD
 *         Deletes the _cwd_mutex
 *
 * @param  None
 *
 * @retval None
 */
void cwd_deinit(void);

#endif /* __CWD_H__ */
