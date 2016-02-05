
/******************************************************************************
 Snippet: Soundex parser.
 Author:  Richard Woolcock (aka KaVir).
 Date:    20th December 2000.
 ******************************************************************************
 This code is copyright (C) 2000 by Richard Woolcock.  It may be used and
 distributed freely, as long as you don't remove this copyright notice.
 ******************************************************************************/
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

#ifndef SOUNDEX_HEADER
#define SOUNDEX_HEADER

/******************************************************************************
 Global operation prototypes.
 ******************************************************************************/

char *GetSoundexKey ( const char *szTxt );
int   SoundexMatch  ( char *szFirst, char *szSecond );

#endif /* SOUNDEX_HEADER */
