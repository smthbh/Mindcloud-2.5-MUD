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
 *  File: mem.c                                                            *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
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


#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


/*
 * Globals
 */
extern		int			top_mprog_index;
extern		int			top_oprog_index;
extern		int			top_rprog_index;
void    free_oprog              args ( ( PROG_LIST *op ) );
void    free_rprog              args ( ( PROG_LIST *rp ) );

AREA_DATA		*	area_free;
EXTRA_DESCR_DATA	*	extra_descr_free;
EXIT_DATA		*	exit_free;
ROOM_INDEX_DATA		*	room_index_free;
OBJ_INDEX_DATA		*	obj_index_free;
MOB_INDEX_DATA		*	mob_index_free;
RESET_DATA		*	reset_free;
HELP_DATA		*	help_free;
void	free_mprog              args ( ( PROG_LIST *mp ) );

/*****************************************************************************
 Name:		new_reset_data
 Purpose:	Creates and clears a reset structure.
 ****************************************************************************/
RESET_DATA *new_reset_data( void )
{
    RESET_DATA *pReset;

    if ( !reset_free )
    {
	pReset		= alloc_perm( sizeof(*pReset) );
	top_reset++;
    }
    else
    {
	pReset		= reset_free;
	reset_free	= reset_free->next;
    }

    pReset->next	= NULL;
    pReset->command	= 'X';
    pReset->arg1	= 0;
    pReset->arg2	= 0;
    pReset->arg3	= 0;

    return pReset;
}



/*****************************************************************************
 Name:		free_reset_data
 Purpose:	Clears and deletes a reset structure.
 ****************************************************************************/
void free_reset_data( RESET_DATA *pReset )
{
    pReset->next            = reset_free;
    reset_free              = pReset;
    return;
}



/*****************************************************************************
 Name:		new_area
 Purpose:	Creates and clears a new area structure.
 ****************************************************************************/
AREA_DATA *new_area( void )
{
    AREA_DATA *pArea;
    char buf[MAX_INPUT_LENGTH];

    if ( !area_free )
    {
	pArea		= alloc_perm( sizeof(*pArea) );
	top_area++;
    }
    else
    {
	pArea		= area_free;
	area_free	= area_free->next;
    }

    pArea->next		= NULL;
    pArea->name		= str_dup( "New area" );
    pArea->music        = str_dup( "default.mid" );
    pArea->area_flags	= AREA_ADDED;
    pArea->security	= 1;
    pArea->builders	= str_dup( "None" );
    pArea->lvnum	= 0;
    pArea->uvnum	= 0;
    pArea->age		= 0;
    pArea->nplayer	= 0;
    pArea->vnum		= top_area-1;		/* OLC 1.1b */
    xprintf( buf, "area%d.are", pArea->vnum );
    pArea->filename	= str_dup( buf );

    return pArea;
}



/*****************************************************************************
 Name:		free_area
 Purpose:	Clears and deletes an area structure.
 ****************************************************************************/
void free_area( AREA_DATA *pArea )
{
    free_string( pArea->name );
    free_string( pArea->filename );
    free_string( pArea->builders );

    pArea->next		=   area_free->next;
    area_free		=   pArea;
    return;
}



EXIT_DATA *new_exit( void )
{
    EXIT_DATA *pExit;

    if ( !exit_free )
    {
        pExit           =   alloc_perm( sizeof(*pExit) );
        top_exit++;
    }
    else
    {
        pExit           =   exit_free;
        exit_free       =   exit_free->next;
    }

    pExit->to_room      =   NULL;
    pExit->next         =   NULL;
    pExit->vnum         =   0;
    pExit->exit_info    =   0;
    pExit->key          =   0;
    pExit->keyword      =   &str_empty[0];;
    pExit->description  =   &str_empty[0];;
    pExit->rs_flags     =   0;

    return pExit;
}



void free_exit( EXIT_DATA *pExit )
{
    free_string( pExit->keyword );
    free_string( pExit->description );

    pExit->next         =   exit_free;
    exit_free           =   pExit;
    return;
}



EXTRA_DESCR_DATA *new_extra_descr( void )
{
    EXTRA_DESCR_DATA *pExtra;

    if ( !extra_descr_free )
    {
        pExtra              =   alloc_perm( sizeof(*pExtra) );
        top_ed++;
    }
    else
    {
        pExtra              =   extra_descr_free;
        extra_descr_free    =   extra_descr_free->next;
    }

    pExtra->keyword         =   NULL;
    pExtra->description     =   NULL;
    pExtra->next            =   NULL;

    return pExtra;
}



void free_extra_descr( EXTRA_DESCR_DATA *pExtra )
{
    free_string( pExtra->keyword );
    free_string( pExtra->description );
    free_string( pExtra->buffer1);
    free_string( pExtra->buffer2);
    pExtra->type = ED_TYPE_NONE;

    pExtra->next        =   extra_descr_free;
    extra_descr_free    =   pExtra;
    return;
}



ROOM_INDEX_DATA *new_room_index( void )
{
    ROOM_INDEX_DATA *pRoom;
    int door;

    if ( !room_index_free )
    {
        pRoom           =   alloc_perm( sizeof(*pRoom) );
        top_room++;
    }
    else
    {
        pRoom           =   room_index_free;
        room_index_free =   room_index_free->next;
    }

    pRoom->next             =   NULL;
    pRoom->people           =   NULL;
    pRoom->contents         =   NULL;
    pRoom->extra_descr      =   NULL;
    pRoom->area             =   NULL;

    for ( door=0; door < MAX_DIR; door++ )
        pRoom->exit[door]   =   NULL;

    pRoom->name             =   &str_empty[0];
    pRoom->description      =   &str_empty[0];
    pRoom->vnum             =   0;
    pRoom->room_flags       =   0;
    pRoom->light            =   0;
    pRoom->sector_type      =   0;

    return pRoom;
}



void free_room_index( ROOM_INDEX_DATA *pRoom )
{
    int door;
    EXTRA_DESCR_DATA *pExtra;
    RESET_DATA *pReset;

    free_string( pRoom->name );
    free_string( pRoom->description );
    
    free_rprog( pRoom->rprogs );

    for ( door = 0; door < MAX_DIR; door++ )
    {
        if ( pRoom->exit[door] )
            free_exit( pRoom->exit[door] );
    }

    for ( pExtra = pRoom->extra_descr; pExtra; pExtra = pExtra->next )
    {
        free_extra_descr( pExtra );
    }

    for ( pReset = pRoom->reset_first; pReset; pReset = pReset->next )
    {
        free_reset_data( pReset );
    }

    pRoom->next     =   room_index_free;
    room_index_free =   pRoom;
    return;
}



AFFECT_DATA *new_affect( void )
{
    AFFECT_DATA *pAf;

    if ( !affect_free )
    {
        pAf             =   alloc_perm( sizeof(*pAf) );
        top_affect++;
    }
    else
    {
        pAf             =   affect_free;
        affect_free     =   affect_free->next;
    }

    pAf->next       =   NULL;
    pAf->location   =   0;
    pAf->modifier   =   0;
    pAf->type       =   0;
    pAf->duration   =   0;
    pAf->bitvector  =   0;

    return pAf;
}



void free_affect( AFFECT_DATA* pAf )
{
    pAf->next           = affect_free;
    affect_free         = pAf;
    return;
}



OBJ_INDEX_DATA *new_obj_index( void )
{
    OBJ_INDEX_DATA *pObj;
    int value;

    if ( !obj_index_free )
    {
        pObj           =   alloc_perm( sizeof(*pObj) );
        top_obj_index++;
    }
    else
    {
        pObj            =   obj_index_free;
        obj_index_free  =   obj_index_free->next;
    }

    pObj->next          =   NULL;
    pObj->extra_descr   =   NULL;
    pObj->affected      =   NULL;
    pObj->area          =   NULL;
    pObj->name          =   str_dup( "no name" );
    pObj->short_descr   =   str_dup( "(no short description)" );
    pObj->description   =   str_dup( "(no description)" );
    pObj->vnum          =   0;
    pObj->item_type     =   ITEM_TRASH;
    pObj->extra_flags   =   0;
    pObj->wear_flags    =   0;
    pObj->count         =   0;
    pObj->weight        =   1;
    pObj->cost          =   0;
    for ( value=0; value<4; value++ )
        pObj->value[value]  =   0;

    return pObj;
}

MOB_INDEX_DATA *new_mob_index( void )
{
    MOB_INDEX_DATA *pMob;

    if ( !mob_index_free )
    {
        pMob           =   alloc_perm( sizeof(*pMob) );
        top_mob_index++;
    }
    else
    {
        pMob            =   mob_index_free;
        mob_index_free  =   mob_index_free->next;
    }

    pMob->next          =   NULL;
    pMob->spec_fun      =   NULL;
    pMob->quest_fun     =   NULL;
    pMob->shop_fun      =   NULL;
    pMob->area          =   NULL;
    pMob->player_name   =   str_dup( "no name" );
    pMob->short_descr   =   str_dup( "(no short description)" );
    pMob->long_descr    =   str_dup( "(no long description)\n\r" );
    pMob->description   =   &str_empty[0];
    pMob->vnum          =   0;
    pMob->count         =   0;
    pMob->killed        =   0;
    pMob->sex           =   0;
    pMob->level         =   0;
    pMob->act           =   ACT_IS_NPC;
    pMob->affected_by   =   0;
    pMob->alignment     =   0;
    pMob->toughness	=   0;
    pMob->extra_attack	=   0;
    pMob->dam_modifier	=   0;
    pMob->default_pos   =   POS_STANDING; /*  -- Hugin */

    return pMob;
}



void free_mob_index( MOB_INDEX_DATA *pMob )
{
    free_string( pMob->player_name );
    free_string( pMob->short_descr );
    free_string( pMob->long_descr );
    free_string( pMob->description );
    free_mprog( pMob->mprogs );

    pMob->next              = mob_index_free;
    mob_index_free          = pMob;
    return;
}

PROG_CODE              *       mpcode_free;
PROG_CODE	       *       opcode_free;
PROG_CODE	       *       rpcode_free;

PROG_CODE *new_mpcode(void)
{
     PROG_CODE *NewCode;

     if (!mpcode_free)
     {
         NewCode = alloc_perm(sizeof(*NewCode) );
         top_mprog_index++;
     }
     else
     {
         NewCode     = mpcode_free;
         mpcode_free = mpcode_free->next;
     }

     NewCode->vnum    = 0;
     NewCode->code    = str_dup("");
     NewCode->next    = NULL;

     return NewCode;
}

void free_mpcode(PROG_CODE *pMcode)
{
    free_string(pMcode->code);
    pMcode->next = mpcode_free;
    mpcode_free  = pMcode;
    return;
}

PROG_CODE *new_opcode(void)
{
     PROG_CODE *NewCode;

     if (!rpcode_free)
     {
         NewCode = alloc_perm(sizeof(*NewCode) );
         top_oprog_index++;
     }
     else
     {
         NewCode     = opcode_free;
         opcode_free = opcode_free->next;
     }

     NewCode->vnum    = 0;
     NewCode->code    = str_dup("");
     NewCode->next    = NULL;

     return NewCode;
}

PROG_CODE *new_rpcode(void)
{
     PROG_CODE *NewCode;

     if (!rpcode_free)
     {
         NewCode = alloc_perm(sizeof(*NewCode) );
         top_rprog_index++;
     }
     else
     {
         NewCode     = rpcode_free;
         rpcode_free = rpcode_free->next;
     }

     NewCode->vnum    = 0;
     NewCode->code    = str_dup("");
     NewCode->next    = NULL;

     return NewCode;
}

void free_opcode(PROG_CODE *pOcode)
{
    free_string(pOcode->code);
    pOcode->next = opcode_free;
    opcode_free  = pOcode;
    return;
}

void free_rpcode(PROG_CODE *pRcode)
{
    free_string(pRcode->code);
    pRcode->next = rpcode_free;
    rpcode_free  = pRcode;
    return;
}

