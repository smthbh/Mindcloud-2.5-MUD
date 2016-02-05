/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Dystopia Mud improvements copyright (C) 2000, 2001 by Brian Graversen  *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
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
/* 
The Breed, Auspice and Tribe defines are for ch->gifts[ ], replacing Totems.
*/

/* Garou Macros */
#define IS_GAR1(ch, gift)	(IS_SET((ch)->garou1, (gift)))

/* Gnosis */
#define GCURRENT		0
#define GMAXIMUM		1

/* Garou 1 Flags */
#define WOLF_RAZORCLAWS			1
#define WOLF_COCOON			2

/* Stats */

#define UNI_GEN         0 /* vampgen */
#define UNI_AFF         1 /* vampaff */
#define UNI_CURRENT     2 /* vamppass */
#define UNI_RAGE        3 /* ch->wolf = how big rage they are in*/
#define UNI_FORM0       4/* wolfform[0] */
#define UNI_FORM1       5 /* wolfform[1] */
#define WOLF_POLYAFF    6

#define VPOWER_OBJ_VNUM      19

