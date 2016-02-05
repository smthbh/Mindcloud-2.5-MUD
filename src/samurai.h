/***************************************************************************
 *                                 _/                            _/        *
 *      _/_/_/  _/_/      _/_/_/  _/    _/_/    _/    _/    _/_/_/         *
 *     _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/          *
 *    _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/           *
 *   _/    _/    _/    _/_/_/  _/    _/_/      _/_/_/    _/_/_/            *
 ***************************************************************************
 * Mindcloud Copyright 2001-2003 by Jeff Boschee (Zarius),                 *
 * Additional credits are in the help file CODECREDITS                     *
 * All Rights Reserved.                                                    *
 ***************************************************************************/

/* Samurai bits */

#define SAMURAI_FOCUS    1
#define SAMURAI_LAST     2
#define SAMURAI_MARTIAL  3

#define SAM_SLIDE        1
#define SAM_SIDESTEP     2
#define SAM_BLOCK        4
#define SAM_COUNTERMOVE  8

#define SAM_DRAINSWORD  16
#define SAM_LITSWORD    32
#define SAM_SWORD       64
#define SAM_SOULSWORD  	128
#define SAM_ICESWORD 	256
#define SAM_FIRESWORD   512

#define IS_SAMPOWER(ch, sn)	(IS_SET((ch)->pcdata->powers[SAMURAI_MARTIAL], (sn)))

