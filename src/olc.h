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
 *  File: olc.h                                                            *
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
/*
 * This is a header file for all the OLC files.  Feel free to copy it into
 * merc.h if you wish.  Many of these routines may be handy elsewhere in
 * the code.  -Jason Dinkel
 */

/*
 * The version info.  Please use this info when reporting bugs.
 * It is displayed in the game by typing 'version' while editing.
 * Do not remove these from the code - by request of Jason Dinkel
 */
#define VERSION	"ILAB Online Creation [Beta 1.1]"
#define AUTHOR	"     By Jason(jdinkel@mines.colorado.edu) and Zarius"
#define DATE	"     (May. 15, 1995)"
#define CREDITS "     Original by Surreality(cxw197@psu.edu) and Locke(locke@lm.com)"

/*
 * New typedefs.
 */
typedef	bool OLC_FUN		args( ( CHAR_DATA *ch, char *argument ) );

#define DECLARE_OLC_FUN( fun )	OLC_FUN    fun

/*
 * Connected states for editor.
 */
#define ED_AREA 	1
#define ED_ROOM 	2
#define ED_OBJECT 	3
#define ED_MOBILE 	4
#define ED_HELP 	5
#define ED_MPCODE	6
#define ED_OPCODE	7
#define ED_RPCODE	8

/*
 * Interpreter Prototypes
 */
void    aedit           args( ( CHAR_DATA *ch, char *argument ) );
void    redit           args( ( CHAR_DATA *ch, char *argument ) );
void    medit           args( ( CHAR_DATA *ch, char *argument ) );
void    oedit           args( ( CHAR_DATA *ch, char *argument ) );
void    hedit           args( ( CHAR_DATA *ch, char *argument ) );
void	mpedit		    args( ( CHAR_DATA *ch, char *argument ) );
void    opedit          args( ( CHAR_DATA *ch, char *argument ) );
void    rpedit          args( ( CHAR_DATA *ch, char *argument ) );

/*
 * OLC Constants
 */
#define MAX_MOB	1		/* Default maximum number for resetting mobs */

/*
 * Structure for an OLC editor command.
 */
struct olc_cmd_type
{
    char * const	name;
    OLC_FUN *		olc_fun;
};

/*
 * Structure for an OLC editor startup command.
 */
struct	editor_cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
};

/*
 * Utils.
 */
AREA_DATA *get_vnum_area	args ( ( int vnum ) );
AREA_DATA *get_area_data	args ( ( int vnum ) );
int flag_value args((const struct flag_type * flag_table, char *argument));
void add_reset args((ROOM_INDEX_DATA * room, RESET_DATA * pReset, int rIndex));
char *flag_string args((const struct flag_type * flag_table, int bits));

/*
 * Interpreter Table Prototypes
 */
extern const struct olc_cmd_type	hedit_table[];
extern const struct olc_cmd_type	aedit_table[];
extern const struct olc_cmd_type	redit_table[];
extern const struct olc_cmd_type	oedit_table[];
extern const struct olc_cmd_type	medit_table[];
extern const struct olc_cmd_type	mpedit_table[];
extern const struct olc_cmd_type	opedit_table[];
extern const struct olc_cmd_type	rpedit_table[];


/*
 * General Functions
 */
bool show_commands		args ( ( CHAR_DATA *ch, char *argument ) );
bool show_help			args ( ( CHAR_DATA *ch, char *argument ) );
bool edit_done			args ( ( CHAR_DATA *ch ) );
bool show_version		args ( ( CHAR_DATA *ch, char *argument ) );

/* help editor */
DECLARE_OLC_FUN( hedit_create );
DECLARE_OLC_FUN( hedit_level  );
DECLARE_OLC_FUN( hedit_keyword   );
DECLARE_OLC_FUN( hedit_text   );
DECLARE_OLC_FUN( hedit_index );
DECLARE_OLC_FUN( hedit_change );
DECLARE_OLC_FUN( hedit_delete );

#define EDIT_HELP(Ch, Help) ( Help = (HELP_DATA *)Ch->desc->pEdit )
HELP_DATA   *new_help_data      args ( ( void ) );

/*
 * Area Editor Prototypes
 */
DECLARE_OLC_FUN( aedit_show		);
DECLARE_OLC_FUN( aedit_cvnum	);
DECLARE_OLC_FUN( aedit_create		);
DECLARE_OLC_FUN( aedit_name		);
DECLARE_OLC_FUN( aedit_file		);
DECLARE_OLC_FUN( aedit_music	);
DECLARE_OLC_FUN( aedit_age		);
DECLARE_OLC_FUN( aedit_reset		);
DECLARE_OLC_FUN( aedit_security		);
DECLARE_OLC_FUN( aedit_builder		);
DECLARE_OLC_FUN( aedit_vnum		);
DECLARE_OLC_FUN( aedit_lvnum		);
DECLARE_OLC_FUN( aedit_uvnum		);

/*
 * Room Editor Prototypes
 */
DECLARE_OLC_FUN(redit_spell);
DECLARE_OLC_FUN( redit_show		);
DECLARE_OLC_FUN( redit_create		);
DECLARE_OLC_FUN( redit_name		);
DECLARE_OLC_FUN( redit_desc		);
DECLARE_OLC_FUN( redit_ed		);
DECLARE_OLC_FUN( redit_format		);
DECLARE_OLC_FUN( redit_north		);
DECLARE_OLC_FUN( redit_south		);
DECLARE_OLC_FUN( redit_east		);
DECLARE_OLC_FUN( redit_west		);
DECLARE_OLC_FUN( redit_up		);
DECLARE_OLC_FUN( redit_down		);
DECLARE_OLC_FUN( redit_move		);
DECLARE_OLC_FUN( redit_mreset		);
DECLARE_OLC_FUN( redit_oreset		);
DECLARE_OLC_FUN( redit_mlist		);
DECLARE_OLC_FUN(redit_teleport);
DECLARE_OLC_FUN(redit_portal);
DECLARE_OLC_FUN(redit_rprog);
DECLARE_OLC_FUN(redit_texttrig);
DECLARE_OLC_FUN(redit_action);
DECLARE_OLC_FUN(redit_rlist);
DECLARE_OLC_FUN( redit_olist		);
DECLARE_OLC_FUN( redit_mshow		);
DECLARE_OLC_FUN( redit_oshow		);
DECLARE_OLC_FUN( redit_addrprog		);
DECLARE_OLC_FUN( redit_delrprog		);

/*
 * Object Editor Prototypes
 */
DECLARE_OLC_FUN( oedit_show		);
DECLARE_OLC_FUN( oedit_delete		);
DECLARE_OLC_FUN( oedit_create		);
DECLARE_OLC_FUN( oedit_name		);
DECLARE_OLC_FUN( oedit_short		);
DECLARE_OLC_FUN( oedit_long		);
DECLARE_OLC_FUN( oedit_addaffect	);
DECLARE_OLC_FUN( oedit_delaffect	);
DECLARE_OLC_FUN( oedit_value0		);
DECLARE_OLC_FUN( oedit_value1		);
DECLARE_OLC_FUN( oedit_value2		);
DECLARE_OLC_FUN( oedit_value3		);
DECLARE_OLC_FUN( oedit_weight		);
DECLARE_OLC_FUN( oedit_cost		);
DECLARE_OLC_FUN( oedit_ed		);
DECLARE_OLC_FUN( oedit_condition	);
DECLARE_OLC_FUN( oedit_resistance	);
DECLARE_OLC_FUN( oedit_toughness	);
DECLARE_OLC_FUN( oedit_addoprog		);
DECLARE_OLC_FUN( oedit_deloprog		);
/*
 * Mobile Editor Prototypes
 */
DECLARE_OLC_FUN( medit_show		);
DECLARE_OLC_FUN( medit_create		);
DECLARE_OLC_FUN( medit_name		);
DECLARE_OLC_FUN( medit_short		);
DECLARE_OLC_FUN( medit_long		);
DECLARE_OLC_FUN( medit_desc		);
DECLARE_OLC_FUN( medit_level		);
DECLARE_OLC_FUN( medit_bones        );
DECLARE_OLC_FUN( medit_align		);
DECLARE_OLC_FUN( medit_spec		);
DECLARE_OLC_FUN(medit_quest             );
DECLARE_OLC_FUN(medit_shop              );
DECLARE_OLC_FUN(medit_weapon);
DECLARE_OLC_FUN( medit_value0		);
DECLARE_OLC_FUN( medit_value1		);
DECLARE_OLC_FUN( medit_value2		);
DECLARE_OLC_FUN( medit_addmprog		);  /* ROM */
DECLARE_OLC_FUN( medit_delmprog		);  /* ROM */
DECLARE_OLC_FUN( medit_position		);  /* ROM */
DECLARE_OLC_FUN( medit_delete		);  /* ROM */

/* Mobprog editor */
DECLARE_OLC_FUN( mpedit_create		);
DECLARE_OLC_FUN( mpedit_code		);
DECLARE_OLC_FUN( mpedit_show		);
DECLARE_OLC_FUN( mpedit_list		);

/*Objprog editor*/
DECLARE_OLC_FUN( opedit_create		);
DECLARE_OLC_FUN( opedit_code		);
DECLARE_OLC_FUN( opedit_show		);
DECLARE_OLC_FUN( opedit_list		);

/* Roomprog editor */
DECLARE_OLC_FUN( rpedit_create		);
DECLARE_OLC_FUN( rpedit_code		);
DECLARE_OLC_FUN( rpedit_show		);
DECLARE_OLC_FUN( rpedit_list		);

/*
 * Macros
 */
#define IS_BUILDER(ch, Area)	(( strstr( Area->builders, ch->name )  \
                                || ch->level == MAX_LEVEL - 1          \
                                || ch->level == MAX_LEVEL              \
				|| strstr( Area->builders, "All" ) )   \
				&& !IS_EXTRA( ch, EXTRA_OSWITCH )      \
				&& !IS_HEAD(ch, LOST_HEAD))

#define TOGGLE_BIT(var, bit)    ((var) ^= (bit))

/* Return pointers to what is being edited. */
#define EDIT_MOB(Ch, Mob)	( Mob = (MOB_INDEX_DATA *)Ch->desc->pEdit )
#define EDIT_OBJ(Ch, Obj)	( Obj = (OBJ_INDEX_DATA *)Ch->desc->pEdit )
#define EDIT_ROOM(Ch, Room)	( Room = Ch->in_room )
#define EDIT_AREA(Ch, Area)	( Area = (AREA_DATA *)Ch->desc->pEdit )
#define EDIT_MPCODE(Ch, Code)   ( Code = (PROG_CODE*)Ch->desc->pEdit )
#define EDIT_OPCODE(Ch, Code)   ( Code = (PROG_CODE*)Ch->desc->pEdit )
#define EDIT_RPCODE(Ch, Code)   ( Code = (PROG_CODE*)Ch->desc->pEdit )

/*
 * Prototypes
 */
/* mem.c - memory prototypes. */
#define ED	EXTRA_DESCR_DATA
RESET_DATA	*new_reset_data		args ( ( void ) );
void		free_reset_data		args ( ( RESET_DATA *pReset ) );
AREA_DATA	*new_area		args ( ( void ) );
void		free_area		args ( ( AREA_DATA *pArea ) );
EXIT_DATA	*new_exit		args ( ( void ) );
void		free_exit		args ( ( EXIT_DATA *pExit ) );
ED 		*new_extra_descr	args ( ( void ) );
void		free_extra_descr	args ( ( ED *pExtra ) );
ROOM_INDEX_DATA *new_room_index		args ( ( void ) );
void		free_room_index		args ( ( ROOM_INDEX_DATA *pRoom ) );
AFFECT_DATA	*new_affect		args ( ( void ) );
void		free_affect		args ( ( AFFECT_DATA* pAf ) );
OBJ_INDEX_DATA	*new_obj_index		args ( ( void ) );
void		free_obj_index		args ( ( OBJ_INDEX_DATA *pObj ) );
MOB_INDEX_DATA	*new_mob_index		args ( ( void ) );
void		free_mob_index		args ( ( MOB_INDEX_DATA *pMob ) );
#undef	ED

char *          prog_type_to_name       args ( ( int type ) );
PROG_LIST      *new_mprog              args ( ( void ) );
void            free_mprog              args ( ( PROG_LIST *mp ) );
PROG_CODE	*new_mpcode		args ( (void) );
void		free_mpcode		args ( ( PROG_CODE *pMcode));
PROG_LIST	*new_oprog		args ( ( void ) );
void		free_oprog		args ( ( PROG_LIST *op ) );
PROG_LIST	*new_rprog		args ( ( void ) );
void		free_rprog		args ( ( PROG_LIST *rp ) );
PROG_CODE	*new_opcode		args ( ( void ) );
void		free_opcode		args ( ( PROG_CODE *pOcode ) );
PROG_CODE	*new_rpcode		args ( ( void ) );
void		free_rpcode		args ( ( PROG_CODE *pRcode ) );
