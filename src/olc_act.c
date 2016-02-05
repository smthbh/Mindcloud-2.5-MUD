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
 *  File: olc_act.c                                                        *
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
#include <limits.h>	/* OLC 1.1b */
#include "merc.h"
#include "interp.h"
#include "magic.h"
#include "olc.h"
#include "recycle.h"

#define HEDIT( fun )        bool fun( CHAR_DATA *ch, char *argument )

struct olc_help_type
{
    char *command;
    const void *structure;
    char *desc;
};

HELP_DATA *new_help(void)
{   
        HELP_DATA *pHelp;

        pHelp = alloc_perm( sizeof(*pHelp));
        pHelp->level = 0;
                return pHelp;
}

bool show_version( CHAR_DATA *ch, char *argument )
{
    send_to_char( VERSION, ch );
    send_to_char( "\n\r", ch );
    send_to_char( AUTHOR, ch );
    send_to_char( "\n\r", ch );
    send_to_char( DATE, ch );
    send_to_char( "\n\r", ch );
    send_to_char( CREDITS, ch );
    send_to_char( "\n\r", ch );

    return FALSE;
}    

/*
 * This table contains help commands and a brief description of each.
 * ------------------------------------------------------------------
 */
const struct olc_help_type help_table[] = {
    {	"bits",		area_bits,	 	 "Area flags."			},
    {	"room",		room_flags,	 	 "Room attributes."			},
    {	"sector",	sector_flags,	 "Sector types, terrain."	},
    {	"exit",		exit_flags,	 	 "Exit types."				},
    {	"type",		type_flags,	 	 "Types of objects."		},
    {	"extra",	extra_flags,	 "Object attributes."		},
    {	"wear",		wear_flags,	 	 "Where to wear object."	},
    {	"spec",		spec_table,	 	 "Available special programs." 	},
  	{   "quest",    quest_table,     "Available quest programs."	},
  	{   "shop",     shop_table,      "Available shop program."	},
    {	"sex",		sex_flags,	 	 "Sexes."					},
    {	"act",		act_flags,	 	 "Mobile attributes."		},
    {	"affect",	affect_flags,	 "Mobile affects."			},
    {	"wear-loc",	wear_loc_flags,	 "Where mobile wears object."	},
    {	"spells",	skill_table,	 "Names of current spells." },
    {	"weapon",	weapon_flags,	 "Type of weapon." 			},
    {	"container",container_flags, "Container status."		},
    {	"liquid",	liquid_flags,	 "Types of liquids."		},
    {   "position", position_flags,  "Mobile positions."        },
    {	"mprog",	mprog_flags,	 "MobProgram flags."		},
    {	"oprog",	oprog_flags,	 "ObjProgram flags."		 },
    {	"rprog",	rprog_flags,	 "RoomProgram flags."		 },
    {	"",		0,		 ""				}
};

/*****************************************************************************
 Name:		show_flag_cmds
 Purpose:	Displays settable flags and stats.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/
void show_flag_cmds( CHAR_DATA *ch, const struct flag_type *flag_table )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH ];
    int  flag;
    int  col;
 
    buf1[0] = '\0';
    col = 0;
    for (flag = 0; *flag_table[flag].name; flag++)
    {
	if ( flag_table[flag].settable )
	{
	    xprintf( buf, "%-19.18s", flag_table[flag].name );
	    strcat( buf1, buf );
	    if ( ++col % 4 == 0 )
		strcat( buf1, "\n\r" );
	}
    }
 
    if ( col % 4 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}

/*****************************************************************************
 Name:		show_skill_cmds
 Purpose:	Displays all skill functions.
 		Does remove those damn immortal commands from the list.
 		Could be improved by:
 		(1) Adding a check for a particular class.
 		(2) Adding a check for a level range.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/
void show_skill_cmds( CHAR_DATA *ch, int tar )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH*2 ];
    int  sn;
    int  col;
 
    buf1[0] = '\0';
    col = 0;
    for (sn = 0; sn < MAX_SKILL; sn++)
    {
	if ( !skill_table[sn].name )
	    break;

	if ( !str_cmp( skill_table[sn].name, "reserved" )
	  || skill_table[sn].spell_fun == spell_null )
	    continue;

	if ( tar == -1 || skill_table[sn].target == tar )
	{
	    xprintf( buf, "%-19.18s", skill_table[sn].name );
	    strcat( buf1, buf );
	    if ( ++col % 4 == 0 )
		strcat( buf1, "\n\r" );
	}
    }
 
    if ( col % 4 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}

/*****************************************************************************
 Name:		show_spec_cmds
 Purpose:	Displays settable special functions.
 Called by:	show_help(olc_act.c).
 ****************************************************************************/
void show_spec_cmds( CHAR_DATA *ch )
{
    char buf  [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH ];
    int  spec;
    int  col;
 
    buf1[0] = '\0';
    col = 0;
    send_to_char( "Preceed special functions with 'spec_'\n\r\n\r", ch );
    for (spec = 0; *spec_table[spec].spec_fun; spec++)
    {
	xprintf( buf, "%-19.18s", &spec_table[spec].spec_name[5] );
	strcat( buf1, buf );
	if ( ++col % 4 == 0 )
	    strcat( buf1, "\n\r" );
    }
 
    if ( col % 4 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return;
}

void show_quest_cmds(CHAR_DATA * ch)
{
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int quest, col;

  buf1[0] = '\0';
  col = 0;
  send_to_char("All special quest programs.\n\r\n\r", ch);
  for (quest = 0; *quest_table[quest].quest_fun; quest++)
  {
    xprintf(buf2, "%-26s", quest_table[quest].quest_name);
    strcat(buf1, buf2);
    if (++col % 3 == 0)
      strcat(buf1, "\n\r");
  }
  if (col % 3 != 0)
    strcat(buf1, "\n\r");
  send_to_char(buf1, ch);
  return;
}

void show_shop_cmds(CHAR_DATA * ch)
{
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int shop, col;

  buf1[0] = '\0';
  col = 0;
  send_to_char("All special shop programs.\n\r\n\r", ch);
  for (shop = 0; *shop_table[shop].shop_fun; shop++)
  {
    xprintf(buf2, "%-26s", shop_table[shop].shop_name);
    strcat(buf1, buf2);
    if (++col % 3 == 0)
      strcat(buf1, "\n\r");
  }
  if (col % 3 != 0)
    strcat(buf1, "\n\r");
  send_to_char(buf1, ch);
  return;
}

/*****************************************************************************
 Name:		show_help
 Purpose:	Displays help for many tables used in OLC.
 Called by:	olc interpreters.
 ****************************************************************************/
bool show_help( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char spell[MAX_INPUT_LENGTH];
    int cnt;

    argument = one_argument( argument, arg );
    one_argument( argument, spell );

    /*
     * Display syntax.
     */
    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax:  ? [command]\n\r\n\r", ch );
	send_to_char( "[command]  [description]\n\r", ch );
	for (cnt = 0; help_table[cnt].command[0] != '\0'; cnt++)
	{
      xprintf(buf, "%-10.10s -%s\n\r", capitalize(help_table[cnt].command), help_table[cnt].desc);
	    send_to_char( buf, ch );
	}
	return FALSE;
    }

    /*
     * Find the command, show changeable data.
     * ---------------------------------------
     */
    for (cnt = 0; *help_table[cnt].command; cnt++)
    {
    if (arg[0] == help_table[cnt].command[0] && !str_prefix(arg, help_table[cnt].command))
	{
	    if ( help_table[cnt].structure == spec_table )
	    {
		show_spec_cmds( ch );
		return FALSE;
	    }
      else if (help_table[cnt].structure == quest_table)
      {
        show_quest_cmds(ch);
        return FALSE;
      }
      else if (help_table[cnt].structure == shop_table)
      {
        show_shop_cmds(ch);
        return FALSE;
      }
      else if (help_table[cnt].structure == skill_table)
	    {

		if ( spell[0] == '\0' )
		{
		    send_to_char( "Syntax:  ? spells "
		        "[ignore/attack/defend/self/object/all]\n\r", ch );
		    return FALSE;
		}

		if ( !str_prefix( spell, "all" ) )
		    show_skill_cmds( ch, -1 );
		else if ( !str_prefix( spell, "ignore" ) )
		    show_skill_cmds( ch, TAR_IGNORE );
		else if ( !str_prefix( spell, "attack" ) )
		    show_skill_cmds( ch, TAR_CHAR_OFFENSIVE );
		else if ( !str_prefix( spell, "defend" ) )
		    show_skill_cmds( ch, TAR_CHAR_DEFENSIVE );
		else if ( !str_prefix( spell, "self" ) )
		    show_skill_cmds( ch, TAR_CHAR_SELF );
		else if ( !str_prefix( spell, "object" ) )
		    show_skill_cmds( ch, TAR_OBJ_INV );
		else
		    send_to_char( "Syntax:  ? spell "
		        "[ignore/attack/defend/self/object/all]\n\r", ch );
		    
		return FALSE;
	    }
	    else
	    {
		show_flag_cmds( ch, help_table[cnt].structure );
		return FALSE;
	    }
	}
    }

    show_help( ch, "" );
    return FALSE;
}

bool redit_rlist(CHAR_DATA * ch, char *argument)
{
  ROOM_INDEX_DATA *pRoom;
  AREA_DATA *pArea;
  char buf[MAX_STRING_LENGTH];
  char buf1[MAX_STRING_LENGTH * 2];
  bool found = FALSE;
  int vnum;

  pArea = ch->in_room->area;
  buf1[0] = '\0';
  send_to_char("  Vnum    Name                                     Sector       Flags\n\r", ch);
  for (vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++)
  {
    if ((pRoom = get_room_index(vnum)))
    {
      found = TRUE;
      xprintf(buf, "[%6d] %-40s [%-10s]", vnum, pRoom->name, flag_string(sector_flags, pRoom->sector_type));
      strcat(buf1, buf);
      xprintf(buf, " [%s]\n\r", flag_string(room_flags, pRoom->room_flags));
      strcat(buf1, buf);
    }
  }

  if (!found)
  {
    send_to_char("No rooms in this area.\n\r", ch);
    return FALSE;
  }

  send_to_char(buf1, ch);
  return FALSE;
}

bool redit_spell(CHAR_DATA * ch, char *argument)
{
  ROOM_INDEX_DATA *pRoom;
  ROOMTEXT_DATA *rt = NULL;
  char command[MAX_INPUT_LENGTH];
  char keyword[MAX_INPUT_LENGTH];
  bool found = FALSE;

  EDIT_ROOM(ch, pRoom);

  argument = one_argument(argument, command);
  argument = one_argument(argument, keyword);

  if (command[0] == '\0' || keyword[0] == '\0')
  {
    send_to_char("Syntax : spell <keyword> <flag> <value>\n\r", ch);
    send_to_char("Syntax : spell <keyword> [create|delete]\n\r", ch);
    send_to_char("\n\rValid flags are : type.\n\r", ch);
    return FALSE;
  }
  if (str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (!found)
    {
      send_to_char("No such spell, you'll need to create it first.\n\r", ch);
      return FALSE;
    }
  }
  if (!str_cmp(keyword, "delete"))
  {
    found = FALSE;
    free_string(rt->input);
    free_string(rt->output);
    free_string(rt->choutput);
    free_string(rt->name);

    if (rt == pRoom->roomtext)
    {
      pRoom->roomtext = rt->next;
    }
    else
    {
      ROOMTEXT_DATA *rt_next;

      for (rt_next = pRoom->roomtext; rt_next; rt_next = rt_next->next)
      {
        if (rt_next->next == rt)
        {
          rt_next->next = rt->next;
          found = TRUE;
          break;
        }
      }
      if (!found)
        bug("Roomtext Data not found.", 0);
    }

    send_to_char("Spell key deleted.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (found)
    {
      send_to_char("There is already a roomprogram with that keyword.\n\r", ch);
      return FALSE;
    }

    /* Allocate some permanent memory for the roomtext */
    rt = alloc_perm(sizeof(*rt));

    /* Assign all the correct values */
    rt->input = str_dup(command);
    rt->output = str_dup("");
    rt->choutput = str_dup("");
    rt->name = str_dup("all");
    rt->type = RT_SPELL;
    rt->power = 40;             /* shield spell */
    rt->mob = 0;                /* Not needed   */

    /* Link the roomtext to the room */
    rt->next = pRoom->roomtext;
    pRoom->roomtext = rt;

    send_to_char("Spell key added.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "type"))
  {
    rt->power = skill_lookup(argument);
    if (rt->power < 0)
    {
      rt->power = 40;
      send_to_char("Failed.\n\r", ch);
      return TRUE;
    }
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else
  {
    send_to_char("No such flag to modify.\n\r", ch);
    return FALSE;
  }
  return TRUE;
}

bool redit_action(CHAR_DATA * ch, char *argument)
{
  ROOM_INDEX_DATA *pRoom;
  ROOMTEXT_DATA *rt = NULL;
  char command[MAX_INPUT_LENGTH];
  char keyword[MAX_INPUT_LENGTH];
  bool found = FALSE;

  EDIT_ROOM(ch, pRoom);

  argument = one_argument(argument, command);
  argument = one_argument(argument, keyword);

  if (command[0] == '\0' || keyword[0] == '\0')
  {
    send_to_char("Syntax : action <keyword> <flag> <value>\n\r", ch);
    send_to_char("Syntax : action <keyword> [create|delete]\n\r", ch);
    send_to_char("\n\rValid flags are : action and mob.\n\r", ch);
    return FALSE;
  }
  if (str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (!found)
    {
      send_to_char("No such action, you'll need to create it first.\n\r", ch);
      return FALSE;
    }
  }
  if (!str_cmp(keyword, "delete"))
  {
    found = FALSE;
    free_string(rt->input);
    free_string(rt->output);
    free_string(rt->choutput);
    free_string(rt->name);

    if (rt == pRoom->roomtext)
    {
      pRoom->roomtext = rt->next;
    }
    else
    {
      ROOMTEXT_DATA *rt_next;

      for (rt_next = pRoom->roomtext; rt_next; rt_next = rt_next->next)
      {
        if (rt_next->next == rt)
        {
          rt_next->next = rt->next;
          found = TRUE;
          break;
        }
      }
      if (!found)
        bug("Roomtext Data not found.", 0);
    }

    send_to_char("Action key deleted.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (found)
    {
      send_to_char("There is already a roomprogram with that keyword.\n\r", ch);
      return FALSE;
    }

    /* Allocate some permanent memory for the roomtext */
    rt = alloc_perm(sizeof(*rt));

    /* Assign all the correct values */
    rt->input = str_dup(command);
    rt->output = str_dup("say hello");
    rt->choutput = str_dup("");
    rt->name = str_dup("all");
    rt->type = RT_ACTION;
    rt->power = 0;              /* Not set yet */
    rt->mob = 0;                /* Not set yet */

    /* Link the roomtext to the room */
    rt->next = pRoom->roomtext;
    pRoom->roomtext = rt;

    send_to_char("Action key added.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "action"))
  {
    free_string(rt->output);
    rt->output = str_dup(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "mob"))
  {
    rt->mob = atoi(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else
  {
    send_to_char("No such flag to modify.\n\r", ch);
    return FALSE;
  }
  return TRUE;
}

bool redit_texttrig(CHAR_DATA * ch, char *argument)
{
  ROOM_INDEX_DATA *pRoom;
  ROOMTEXT_DATA *rt = NULL;
  char command[MAX_INPUT_LENGTH];
  char keyword[MAX_INPUT_LENGTH];
  bool found = FALSE;

  EDIT_ROOM(ch, pRoom);

  argument = one_argument(argument, command);
  argument = one_argument(argument, keyword);

  if (command[0] == '\0' || keyword[0] == '\0')
  {
    send_to_char("Syntax : texttrig <keyword> output +<text>\n\r", ch);
    send_to_char("Syntax : texttrig <keyword> [create|delete|clear]\n\r", ch);
    return FALSE;
  }
  if (str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (!found)
    {
      send_to_char("No such texttrigger, you'll need to create it first.\n\r", ch);
      return FALSE;
    }
  }
  if (!str_cmp(keyword, "delete"))
  {
    found = FALSE;
    free_string(rt->input);
    free_string(rt->output);
    free_string(rt->choutput);
    free_string(rt->name);

    if (rt == pRoom->roomtext)
    {
      pRoom->roomtext = rt->next;
    }
    else
    {
      ROOMTEXT_DATA *rt_next;

      for (rt_next = pRoom->roomtext; rt_next; rt_next = rt_next->next)
      {
        if (rt_next->next == rt)
        {
          rt_next->next = rt->next;
          found = TRUE;
          break;
        }
      }
      if (!found)
        bug("Roomtext Data not found.", 0);
    }

    send_to_char("Texttrigger key deleted.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "create"))
  {
    /* Allocate some permanent memory for the roomtext */
    rt = alloc_perm(sizeof(*rt));

    /* Assign all the correct values */
    rt->input = str_dup(command);
    rt->output = str_dup("\n");
    rt->choutput = str_dup("");
    rt->name = str_dup("all");
    rt->type = RT_TEXT;
    rt->power = 0;              /* Not needed */
    rt->mob = 0;                /* Not needed */

    /* Link the roomtext to the room */
    rt->next = pRoom->roomtext;
    pRoom->roomtext = rt;
    send_to_char("Texttrigger key added.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "clear"))
  {
    free_string(rt->output);
    rt->output = str_dup("\n");
    send_to_char("Cleared.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "output"))
  {
    char buf[MAX_STRING_LENGTH];

    xprintf(buf, "%s%s\n", rt->output, argument);
    free_string(rt->output);
    rt->output = str_dup(buf);
    send_to_char("String attached.\n\r", ch);
    return TRUE;
  }
  else
  {
    send_to_char("No such flag to edit.\n\r", ch);
    return FALSE;
  }
  return TRUE;
}

bool redit_rprog(CHAR_DATA * ch, char *argument)
{
  ROOM_INDEX_DATA *pRoom;
  ROOMTEXT_DATA *rt = NULL;
  char command[MAX_INPUT_LENGTH];
  char keyword[MAX_INPUT_LENGTH];
  bool found = FALSE;

  EDIT_ROOM(ch, pRoom);

  argument = one_argument(argument, command);
  argument = one_argument(argument, keyword);

  if (command[0] == '\0' || keyword[0] == '\0')
  {
    send_to_char("Syntax : rprog <keyword> <flag> <value>\n\r", ch);
    send_to_char("Syntax : rprog <keyword> [create|delete]\n\r", ch);
    send_to_char("\n\rflags : output, choutput, name, type, power, mob\n\r", ch);
    return FALSE;
  }

  if (str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (!found)
    {
      send_to_char("No such roomprogram, you'll need to create it first.\n\r", ch);
      return FALSE;
    }
  }
  if (!str_cmp(keyword, "delete"))
  {
    found = FALSE;
    free_string(rt->input);
    free_string(rt->output);
    free_string(rt->choutput);
    free_string(rt->name);

    if (rt == pRoom->roomtext)   
    {
      pRoom->roomtext = rt->next;
    }
    else
    {
      ROOMTEXT_DATA *rt_next;

      for (rt_next = pRoom->roomtext; rt_next; rt_next = rt_next->next)
      {
        if (rt_next->next == rt)
        {
          rt_next->next = rt->next;
          found = TRUE;
          break;
        }
      }
      if (!found)
        bug("Roomtext Data not found.", 0);
    }

    send_to_char("Roomprogram deleted.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "create"))
  {  
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (found)
    {
      send_to_char("There is already a roomprogram with that keyword.\n\r", ch);
      return FALSE;
    }

    /* Allocate some permanent memory for the roomtext */
    rt = alloc_perm(sizeof(*rt));

    /* Assign all the correct values */
    rt->input = str_dup(command);
    rt->output = str_dup("");
    rt->choutput = str_dup("");
    rt->name = str_dup("all");
    rt->type = 0;
    rt->power = 0;
    rt->mob = 0;
         
    /* Link the roomtext to the room */
    rt->next = pRoom->roomtext;
    pRoom->roomtext = rt;
         
    send_to_char("Roomprogram added.\n\r", ch);
    return TRUE; 
  }
     
  if (!str_cmp(keyword, "output"))
  {
    free_string(rt->output);
    rt->output = str_dup(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "choutput"))
  {
    free_string(rt->choutput);
    rt->choutput = str_dup(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }  
  else if (!str_cmp(keyword, "mob"))
  {  
    if (!is_number(argument))
    {
      send_to_char("The mob flag is a number.\n\r", ch);
      return FALSE;
    }
    rt->mob = atoi(argument);  
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "power"))
  {  
    if (!is_number(argument))
    {
      send_to_char("The power flag is a number.\n\r", ch);
      return FALSE;
    }
    rt->power = atoi(argument);  
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "name"))
  {  
    free_string(rt->name);
    rt->name = str_dup(argument);  
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "type"))
  {  
    if (!is_number(argument))
    {
      send_to_char("The type flag is a number.\n\r", ch);
      return FALSE;
    }
    rt->type = atoi(argument);  
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else
  {
    send_to_char("No such flag to modify.\n\r", ch);
    return FALSE;
  }
  return TRUE;
}

bool redit_portal(CHAR_DATA * ch, char *argument)
{
  ROOM_INDEX_DATA *pRoom;
  ROOMTEXT_DATA *rt = NULL;
  char command[MAX_INPUT_LENGTH];
  char keyword[MAX_INPUT_LENGTH];
  bool found = FALSE;

  EDIT_ROOM(ch, pRoom);

  argument = one_argument(argument, command);
  argument = one_argument(argument, keyword);

  if (command[0] == '\0' || keyword[0] == '\0')
  {
    send_to_char("Syntax : portal <keyword> <flag> <value>\n\r", ch);
    send_to_char("Syntax : portal <keyword> [create|delete]\n\r", ch);
    send_to_char("\n\rflags : output, choutput, target\n\r", ch);
    return FALSE;
  }

  if (str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (!found)
    {
      send_to_char("No such portal, you'll need to create it first.\n\r", ch);
      return FALSE;
    }
  }
  if (!str_cmp(keyword, "delete"))
  {
    found = FALSE;
    free_string(rt->input);
    free_string(rt->output);
    free_string(rt->choutput);
    free_string(rt->name);

    if (rt == pRoom->roomtext)
    {
      pRoom->roomtext = rt->next;
    }
    else
    {
      ROOMTEXT_DATA *rt_next;

      for (rt_next = pRoom->roomtext; rt_next; rt_next = rt_next->next)
      {
        if (rt_next->next == rt)
        {
          rt_next->next = rt->next;
          found = TRUE;
          break;
        }
      }
      if (!found)
        bug("Roomtext Data not found.", 0);
    }

    send_to_char("Portal key deleted.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (found)
    {
      send_to_char("There is already a roomprogram with that keyword.\n\r", ch);
      return FALSE;
    }

    /* Allocate some permanent memory for the roomtext */
    rt = alloc_perm(sizeof(*rt));

    /* Assign all the correct values */
    rt->input = str_dup(command);
    rt->output = str_dup("A shimmering portal appears in front of $n.");
    rt->choutput = str_dup("A shimmering portal appears in front of you.");
    rt->name = str_dup("all");
    rt->type = RT_PORTAL;
    rt->power = 0;              /* Not set yet */
    rt->mob = 0;                /* doesn't need a mob present to work */

    /* Link the roomtext to the room */
    rt->next = pRoom->roomtext;
    pRoom->roomtext = rt;

    send_to_char("Portal key added.\n\r", ch);
    return TRUE;
  }

  if (!str_cmp(keyword, "output"))
  {
    free_string(rt->output);
    rt->output = str_dup(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "choutput"))
  {
    free_string(rt->choutput);
    rt->choutput = str_dup(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "target"))
  {
    if (!is_number(argument))
    {
      send_to_char("The target flag is a number.\n\r", ch);
      return FALSE;
    }
    rt->power = atoi(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else
  {
    send_to_char("No such flag to modify.\n\r", ch);
    return FALSE;
  }
  return TRUE;
}

bool redit_teleport(CHAR_DATA * ch, char *argument)
{
  ROOM_INDEX_DATA *pRoom;
  ROOMTEXT_DATA *rt = NULL;
  char command[MAX_INPUT_LENGTH];
  char keyword[MAX_INPUT_LENGTH];
  bool found = FALSE;

  EDIT_ROOM(ch, pRoom);

  argument = one_argument(argument, command);
  argument = one_argument(argument, keyword);

  if (command[0] == '\0' || keyword[0] == '\0')
  {
    send_to_char("Syntax : teleport <keyword> <flag> <value>\n\r", ch);
    send_to_char("Syntax : teleport <keyword> [create|delete]\n\r", ch);
    send_to_char("\n\rflags : output, choutput, target\n\r", ch);
    return FALSE;
  }

  if (str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (!found)
    {
      send_to_char("No such teleport, you'll need to create it first.\n\r", ch);
      return FALSE;
    }
  }
  if (!str_cmp(keyword, "delete"))
  {
    found = FALSE;
    free_string(rt->input);
    free_string(rt->output);
    free_string(rt->choutput);
    free_string(rt->name);

    if (rt == pRoom->roomtext)
    {
      pRoom->roomtext = rt->next;
    }
    else
    {
      ROOMTEXT_DATA *rt_next;

      for (rt_next = pRoom->roomtext; rt_next; rt_next = rt_next->next)
      {
        if (rt_next->next == rt)
        {
          rt_next->next = rt->next;
          found = TRUE;
          break;
        }
      }
      if (!found)
        bug("Roomtext Data not found.", 0);
    }

    send_to_char("Teleport key deleted.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "create"))
  {
    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (!str_cmp(command, rt->input))
      {
        found = TRUE;
        break;
      }
    }
    if (found)
    {
      send_to_char("There is already a teleport with that keyword.\n\r", ch);
      return FALSE;
    }

    /* Allocate some permanent memory for the roomtext */
    rt = alloc_perm(sizeof(*rt));

    /* Assign all the correct values */
    rt->input = str_dup(command);
    rt->output = str_dup("$n vanishes.");
    rt->choutput = str_dup("You are transported somewhere else.");
    rt->name = str_dup("all");
    rt->type = RT_TELEPORT;
    rt->power = 0;              /* Not set yet */
    rt->mob = 0;                /* doesn't need a mob present to work */

    /* Link the roomtext to the room */
    rt->next = pRoom->roomtext;
    pRoom->roomtext = rt;

    send_to_char("Teleport key added.\n\r", ch);
    return TRUE;
  }

  if (!str_cmp(keyword, "output"))
  {
    free_string(rt->output);
    rt->output = str_dup(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "choutput"))
  {
    free_string(rt->choutput);
    rt->choutput = str_dup(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else if (!str_cmp(keyword, "target"))
  {
    if (!is_number(argument))
    {
      send_to_char("The target flag is a number.\n\r", ch);
      return FALSE;
    }
    rt->power = atoi(argument);
    send_to_char("Ok.\n\r", ch);
    return TRUE;
  }
  else
  {
    send_to_char("No such flag to modify.\n\r", ch);
    return FALSE;
  }
  return TRUE;
}

bool redit_mlist( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA	*pMobIndex;
    AREA_DATA		*pArea;
    char		buf  [ MAX_STRING_LENGTH   ];
    char		buf1 [ MAX_STRING_LENGTH*2 ];
    char		arg  [ MAX_INPUT_LENGTH    ];
    bool fAll, found;
    int vnum;
    int  col = 0;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax:  mlist <all/name>\n\r", ch );
	return FALSE;
    }

    pArea = ch->in_room->area;
    buf1[0] = '\0';
    fAll    = !str_cmp( arg, "all" );
    found   = FALSE;

    for ( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) )
	{
	    if ( fAll || is_name( arg, pMobIndex->player_name ) )
	    {
		found = TRUE;
        xprintf(buf, "[%5d] %-17.16s", pMobIndex->vnum, capitalize(pMobIndex->short_descr));
		strcat( buf1, buf );
		if ( ++col % 3 == 0 )
		    strcat( buf1, "\n\r" );
	    }
	}
    }

    if ( !found )
    {
	send_to_char( "Mobile(s) not found in this area.\n\r", ch);
	return FALSE;
    }

    if ( col % 3 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return FALSE;
}

bool redit_olist( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA	*pObjIndex;
    AREA_DATA		*pArea;
    char		buf  [ MAX_STRING_LENGTH   ];
    char		buf1 [ MAX_STRING_LENGTH*2 ];
    char		arg  [ MAX_INPUT_LENGTH    ];
    bool fAll, found;
    int vnum;
    int  col = 0;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax:  olist <all/name/item_type>\n\r", ch );
	return FALSE;
    }

    pArea = ch->in_room->area;
    buf1[0] = '\0';
    fAll    = !str_cmp( arg, "all" );
    found   = FALSE;

    for ( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) )
	{
	    if ( fAll || is_name( arg, pObjIndex->name )
	    || flag_value( type_flags, arg ) == pObjIndex->item_type )
	    {
		found = TRUE;
		xprintf( buf, "[%5d] %-17.16s",
		    pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
		strcat( buf1, buf );
		if ( ++col % 3 == 0 )
		    strcat( buf1, "\n\r" );
	    }
	}
    }

    if ( !found )
    {
	send_to_char( "Object(s) not found in this area.\n\r", ch);
	return FALSE;
    }

    if ( col % 3 != 0 )
	strcat( buf1, "\n\r" );

    send_to_char( buf1, ch );
    return FALSE;
}

bool redit_mshow( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    int value;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  mshow <vnum>\n\r", ch );
	return FALSE;
    }

    if ( is_number( argument ) )
    {
	value = atoi( argument );
	if ( !( pMob = get_mob_index( value ) ))
	{
	    send_to_char( "REdit:  That mobile does not exist.\n\r", ch );
	    return FALSE;
	}

	ch->desc->pEdit = (void *)pMob;
    }
 
    medit_show( ch, argument );
    ch->desc->pEdit = (void *)ch->in_room;
    return FALSE; 
}

bool redit_oshow( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    int value;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  oshow <vnum>\n\r", ch );
	return FALSE;
    }

    if ( is_number( argument ) )
    {
	value = atoi( argument );
	if ( !( pObj = get_obj_index( value ) ))
	{
	    send_to_char( "REdit:  That object does not exist.\n\r", ch );
	    return FALSE;
	}

	ch->desc->pEdit = (void *)pObj;
    }
 
    oedit_show( ch, argument );
    ch->desc->pEdit = (void *)ch->in_room;
    return FALSE; 
}

/*****************************************************************************
 Name:		check_range( lower vnum, upper vnum )
 Purpose:	Ensures the range spans only one area.
 Called by:	aedit_vnum(olc_act.c).
 ****************************************************************************/
bool check_range( int lower, int upper )
{
    AREA_DATA *pArea;
    int cnt = 0;

    for ( pArea = area_first; pArea; pArea = pArea->next )
    {
	/*
	 * lower < area < upper
	 */
	if ( ( lower <= pArea->lvnum && upper >= pArea->lvnum )
	||   ( upper >= pArea->uvnum && lower <= pArea->uvnum ) )
	    cnt++;

	if ( cnt > 1 )
	    return FALSE;
    }
    return TRUE;
}

AREA_DATA * get_vnum_area( int vnum )
{
    AREA_DATA *pArea;

    for ( pArea = area_first; pArea; pArea = pArea->next )
    {
        if ( vnum >= pArea->lvnum
          && vnum <= pArea->uvnum )
            return pArea;
    }

    return 0;
}

/*
 * Area Editor Functions.
 */
bool aedit_show( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char buf  [MAX_STRING_LENGTH];

    EDIT_AREA(ch, pArea);

    xprintf( buf, "#wName:     #0[#W%5d#0] #B%s#n\n\r", pArea->vnum, pArea->name );
    send_to_char( buf, ch );

    xprintf( buf, "#wFile:     #W%s#n\n\r", pArea->filename );
    send_to_char( buf, ch );

    xprintf( buf, "#wVnums:    #0[#W%d#R-#W%d#0]#n\n\r", pArea->lvnum, pArea->uvnum );
    send_to_char( buf, ch );

  xprintf(buf, "#wCvnum:    #0[#W%d#0]#n\n\r", pArea->cvnum);
    send_to_char( buf, ch );

    xprintf( buf, "#wPlayers:  #0[#W%d#0]#n\n\r", pArea->nplayer );
    send_to_char( buf, ch );

  xprintf(buf, "#wAge:      #0[#W%d#0]#n\n\r", pArea->age);
    send_to_char( buf, ch );

    xprintf( buf, "#wSecurity: #0[#W%d#0]#n\n\r", pArea->security );
    send_to_char( buf, ch );

    xprintf( buf, "#wBuilders: #0[#W%s#0]#n\n\r", pArea->builders );
    send_to_char( buf, ch );

  xprintf(buf, "#wMusic   : #0[#W%s#0]#n\n\r", pArea->music);
  send_to_char(buf, ch);

    xprintf( buf, "#wFlags:    #0[#W%s#0]#n\n\r", flag_string( area_flags, pArea->area_flags ) );
    send_to_char( buf, ch );

  xprintf(buf, "#wBits:     #0[#W%s#0]#n\n\r", flag_string(area_bits, pArea->areabits));
  send_to_char(buf, ch);

    return FALSE;
}

bool aedit_reset( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;

    EDIT_AREA(ch, pArea);

    reset_area( pArea );
    send_to_char( "Area reset.\n\r", ch );

    return FALSE;
}

bool aedit_create( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;

    if ( top_area >= INT_MAX )	/* OLC 1.1b */
    {
	send_to_char( "We're out of vnums for new areas.\n\r", ch );
	return FALSE;
    }

    pArea               =   new_area();
    area_last->next     =   pArea;
    area_last		=   pArea;	/* Thanks, Walker. */
    ch->desc->pEdit     =   (void *)pArea;

    SET_BIT( pArea->area_flags, AREA_ADDED );
    send_to_char( "Area Created.\n\r", ch );
    return TRUE;	/* OLC 1.1b */
}

bool aedit_name( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;

    EDIT_AREA(ch, pArea);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:   name [$name]\n\r", ch );
	return FALSE;
    }

    free_string( pArea->name );
    pArea->name = str_dup( argument );

    send_to_char( "Name set.\n\r", ch );
    return TRUE;
}

bool aedit_music(CHAR_DATA * ch, char *argument)
{
  AREA_DATA *pArea;
  char file[MAX_STRING_LENGTH];
  int i, length;

  EDIT_AREA(ch, pArea);

  one_argument(argument, file); /* Forces Lowercase */

  if (argument[0] == '\0')
  {
    send_to_char("Syntax:  filename [$file]\n\r", ch);
    return FALSE;
  }

  /*
   * Simple Syntax Check.
   */
  length = strlen(argument);
  if (length > 8)
  {
    send_to_char("No more than eight characters allowed.\n\r", ch);
    return FALSE;
  }

  /*
   * Allow only letters and numbers.
   */
  for (i = 0; i < length; i++)
  {
    if (!isalnum(file[i]))
    {
      send_to_char("Only letters and numbers are valid.\n\r", ch);
      return FALSE;
    }
  }
  free_string(pArea->music);
  strcat(file, ".mid");
  pArea->music = str_dup(file);
  send_to_char("Music set.\n\r", ch);
  return TRUE;
}

bool aedit_file( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char file[MAX_STRING_LENGTH];
    int i, length;

    EDIT_AREA(ch, pArea);

    one_argument( argument, file );	/* Forces Lowercase */

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  filename [$file]\n\r", ch );
	return FALSE;
    }

    /*
     * Simple Syntax Check.
     */
    length = strlen( argument );
    if ( length > 8 )
    {
	send_to_char( "No more than eight characters allowed.\n\r", ch );
	return FALSE;
    }

    /*
     * Allow only letters and numbers.
     */
    for ( i = 0; i < length; i++ )
    {
	if ( !isalnum( file[i] ) )
	{
	    send_to_char( "Only letters and numbers are valid.\n\r", ch );
	    return FALSE;
	}
    }    

    free_string( pArea->filename );
    strcat( file, ".are" );
    pArea->filename = str_dup( file );

    send_to_char( "Filename set.\n\r", ch );
    return TRUE;
}

bool aedit_age( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char age[MAX_STRING_LENGTH];

    EDIT_AREA(ch, pArea);

    one_argument( argument, age );

    if ( !is_number( age ) || age[0] == '\0' )
    {
	send_to_char( "Syntax:  age [#age]\n\r", ch );
	return FALSE;
    }

    pArea->age = atoi( age );

    send_to_char( "Age set.\n\r", ch );
    return TRUE;
}

bool aedit_security( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char sec[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int  value;

    EDIT_AREA(ch, pArea);

    one_argument( argument, sec );

    if ( !is_number( sec ) || sec[0] == '\0' )
    {
	send_to_char( "Syntax:  security [##level]\n\r", ch );
	return FALSE;
    }

    value = atoi( sec );

    if ( value > ch->pcdata->security || value < 0 )
    {
	if ( ch->pcdata->security != 0 )
	{
	    xprintf( buf, "Security is 0-%d.\n\r", ch->pcdata->security );
	    send_to_char( buf, ch );
	}
	else
	    send_to_char( "Security is 0 only.\n\r", ch );
	return FALSE;
    }

    pArea->security = value;

    send_to_char( "Security set.\n\r", ch );
    return TRUE;
}

bool aedit_builder( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char name[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    EDIT_AREA(ch, pArea);

    one_argument( argument, name );

    if ( name[0] == '\0' )
    {
	send_to_char( "Syntax:  builder [$name]  -toggles builder\n\r", ch );
	send_to_char( "Syntax:  builder All      -allows everyone\n\r", ch );
	return FALSE;
    }

    name[0] = UPPER( name[0] );

    if ( strstr( pArea->builders, name ) != '\0' )
    {
	pArea->builders = string_replace( pArea->builders, name, "\0" );
	pArea->builders = string_unpad( pArea->builders );

	if ( pArea->builders[0] == '\0' )
	{
	    free_string( pArea->builders );
	    pArea->builders = str_dup( "None" );
	}
	send_to_char( "Builder removed.\n\r", ch );
	return TRUE;
    }
    else
    {
	buf[0] = '\0';
	if ( strstr( pArea->builders, "None" ) != '\0' )
	{
	    pArea->builders = string_replace( pArea->builders, "None", "\0" );
	    pArea->builders = string_unpad( pArea->builders );
	}

	if (pArea->builders[0] != '\0' )
	{
	    strcat( buf, pArea->builders );
	    strcat( buf, " " );
	}
	strcat( buf, name );
	free_string( pArea->builders );
	pArea->builders = string_proper( str_dup( buf ) );

	send_to_char( "Builder added.\n\r", ch );
	return TRUE;
    }

    return FALSE;
}

bool aedit_cvnum(CHAR_DATA *ch, char *argument)
{
  AREA_DATA *pArea;
  char arg[MAX_STRING_LENGTH];
  int i;
 
  EDIT_AREA(ch, pArea);

  one_argument(argument, arg);

  if (!is_number(arg))
  {
    send_to_char("Syntax:  cvnum <number>\n\r", ch);
    return FALSE;
  }
  if ((i = atoi(arg)) > pArea->uvnum || i < pArea->lvnum)
  {
    send_to_char("That is not in this area.\n\r", ch);
    return FALSE;
  }
  pArea->cvnum = i;
  send_to_char("Center vnum set.\n\r", ch);
  return TRUE;
}

bool aedit_vnum( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    AREA_DATA *pArea2;
    char lower[MAX_STRING_LENGTH];
    char upper[MAX_STRING_LENGTH];
    int  ilower;
    int  iupper;
    char buf[MSL];

    EDIT_AREA(ch, pArea);

    argument = one_argument( argument, lower );
    one_argument( argument, upper );

    if ( !is_number( lower ) || lower[0] == '\0'
    || !is_number( upper ) || upper[0] == '\0' )
    {
	send_to_char( "Syntax:  vnum [##lower] [##upper]\n\r", ch );
	return FALSE;
    }

    if ( ( ilower = atoi( lower ) ) > ( iupper = atoi( upper ) ) )
    {
	send_to_char( "AEdit:  Upper must be larger then lower.\n\r", ch );
	return FALSE;
    }
    
    /* OLC 1.1b */
    if ( ilower <= 0 || ilower >= INT_MAX || iupper <= 0 || iupper >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	xprintf( output, "AEdit: vnum must be between 0 and %d.\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    if ( !check_range( ilower, iupper ) )
    {
	send_to_char( "AEdit:  Range must include only this area.\n\r", ch );
	return FALSE;
    }

    if ( get_vnum_area( ilower )
    && get_vnum_area( ilower ) != pArea )
    {
	//send_to_char( "AEdit:  Lower vnum already assigned.\n\r", ch );
	pArea2 = get_vnum_area( ilower );
    xprintf( buf, "AEdit:  Lower vnum %d already assigned in area %s.\n\r",ilower, pArea2->name);
	send_to_char( buf, ch );
	return FALSE;
    }

    pArea->lvnum = ilower;
    send_to_char( "Lower vnum set.\n\r", ch );

    if ( get_vnum_area( iupper )
    && get_vnum_area( iupper ) != pArea )
    {
	send_to_char( "AEdit:  Upper vnum already assigned.\n\r", ch );
	return TRUE;	/* The lower value has been set. */
    }

    pArea->uvnum = iupper;
    send_to_char( "Upper vnum set.\n\r", ch );

    return TRUE;
}

bool aedit_lvnum( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char lower[MAX_STRING_LENGTH];
    int  ilower;
    int  iupper;

    EDIT_AREA(ch, pArea);

    one_argument( argument, lower );

    if ( !is_number( lower ) || lower[0] == '\0' )
    {
    send_to_char("Syntax:  lvnum [##lower]\n\r", ch);
	return FALSE;
    }

    if ( ( ilower = atoi( lower ) ) > ( iupper = pArea->uvnum ) )
    {
	send_to_char( "AEdit:  Value must be less than the uvnum.\n\r", ch );
	return FALSE;
    }
    
    /* OLC 1.1b */
    if ( ilower <= 0 || ilower >= INT_MAX || iupper <= 0 || iupper >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	xprintf( output, "AEdit: vnum must be between 0 and %d.\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    if ( !check_range( ilower, iupper ) )
    {
	send_to_char( "AEdit:  Range must include only this area.\n\r", ch );
	return FALSE;
    }

    if ( get_vnum_area( ilower )
    && get_vnum_area( ilower ) != pArea )
    {
	send_to_char( "AEdit:  Lower vnum already assigned.\n\r", ch );
	return FALSE;
    }

    pArea->lvnum = ilower;
    send_to_char( "Lower vnum set.\n\r", ch );
    return TRUE;
}

bool aedit_uvnum( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char upper[MAX_STRING_LENGTH];
    int  ilower;
    int  iupper;

    EDIT_AREA(ch, pArea);

    one_argument( argument, upper );

    if ( !is_number( upper ) || upper[0] == '\0' )
    {
	send_to_char( "Syntax:  uvnum [##upper]\n\r", ch );
	return FALSE;
    }

    if ( ( ilower = pArea->lvnum ) > ( iupper = atoi( upper ) ) )
    {
	send_to_char( "AEdit:  Upper must be larger then lower.\n\r", ch );
	return FALSE;
    }
    
    /* OLC 1.1b */
    if ( ilower <= 0 || ilower >= INT_MAX || iupper <= 0 || iupper >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	xprintf( output, "AEdit: vnum must be between 0 and %d.\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    if ( !check_range( ilower, iupper ) )
    {
	send_to_char( "AEdit:  Range must include only this area.\n\r", ch );
	return FALSE;
    }

    if ( get_vnum_area( iupper )
    && get_vnum_area( iupper ) != pArea )
    {
	send_to_char( "AEdit:  Upper vnum already assigned.\n\r", ch );
	return FALSE;
    }

    pArea->uvnum = iupper;
    send_to_char( "Upper vnum set.\n\r", ch );

    return TRUE;
}

/*
 * Room Editor Functions.
 */
bool redit_show( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA	*pRoom;
    char		buf  [MAX_STRING_LENGTH];
    char		buf1 [2*MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
    OBJ_DATA		*obj;
    CHAR_DATA		*rch;
    int			door;
    bool		fcnt;
    PROG_LIST		*list;
    
    EDIT_ROOM(ch, pRoom);

    buf1[0] = '\0';
    
    xprintf( buf, "#wDescription:\n\r#0%s", pRoom->description );
    strcat( buf1, buf );

    xprintf( buf, "#wName:       #0[#W%s#0]\n\r#wArea:       #0[#W%5d#0] %s\n\r",
	    pRoom->name, pRoom->area->vnum, pRoom->area->name );
    strcat( buf1, buf );

    xprintf( buf, "#wVnum:       #0[#W%5d#0]\n\r#wSector:     #0[#W%s#0]\n\r",
	    pRoom->vnum, flag_string( sector_flags, pRoom->sector_type ) );
    strcat( buf1, buf );

    xprintf( buf, "#wRoom flags: #0[#W%s#0]\n\r",
	    flag_string( room_flags, pRoom->room_flags ) );
    strcat( buf1, buf );

    if ( pRoom->extra_descr )
    {
	EXTRA_DESCR_DATA *ed;

    strcat(buf1, "#wDesc Kwds:#W\n\r");
	for ( ed = pRoom->extra_descr; ed; ed = ed->next )
	{
      if (ed->type == ED_TYPE_NONE)
      {
        strcat(buf1, "[Normal : ");
	    strcat( buf1, ed->keyword );
        strcat(buf1, "]\n\r");
      }
      else if (ed->type == ED_TYPE_PULL || ed->type == ED_TYPE_PRESS || ed->type == ED_TYPE_PUSH)
      {
        switch (ed->type)
        {
          case ED_TYPE_PULL:
            strcat(buf1, "[Pull  : ");
            break;
          case ED_TYPE_PRESS:
            strcat(buf1, "[Press : ");
            break;
          case ED_TYPE_PUSH:
            strcat(buf1, "[Push  : ");
            break;
        }
        strcat(buf1, ed->keyword);
        switch (ed->action)
        {
          case ED_ACTION_TELEPORT:
            xprintf(buf, " (Teleport to %d)]\n\r  [", ed->vnum);
            break;
          case ED_ACTION_OBJECT:
            xprintf(buf, " (Load object %d)]\n\r  [", ed->vnum);
            break;
          case ED_ACTION_ELEVATOR:
            xprintf(buf, " (Elevate to %d)]\n\r  [", ed->vnum);
            break;
          case ED_ACTION_SPELL:
            xprintf(buf, " (Cast spell %s)]\n\r  [", skill_table[ed->vnum].name);
            break;
        }
        strcat(buf1, buf);
        strcat(buf1, ed->buffer1);
        strcat(buf1, "]\n\r  [");
        strcat(buf1, ed->buffer2);
        strcat(buf1, "]\n\r");
      }
    }
  }

  if (pRoom->roomtext)
  {
    ROOMTEXT_DATA *rt;

    for (rt = pRoom->roomtext; rt; rt = rt->next)
    {
      if (rt->type == RT_TELEPORT)
      {
        strcat(buf1, "[RT Teleport:   (");
        strcat(buf1, rt->input);
        strcat(buf1, ") (");
        xprintf(buf2, "%d", rt->power);
        strcat(buf1, buf2);
        strcat(buf1, ")]\n\r  [");
        strcat(buf1, rt->output);
        strcat(buf1, "]\n\r  [");
        strcat(buf1, rt->choutput);
        strcat(buf1, "]\n\r");
      }
      else if (rt->type == RT_PORTAL)
      {
        strcat(buf1, "[RT Portal:     (");
        strcat(buf1, rt->input);
        strcat(buf1, ") (");
        xprintf(buf2, "%d", rt->power);
        strcat(buf1, buf2);
        strcat(buf1, ")]\n\r  [");
        strcat(buf1, rt->output);
        strcat(buf1, "]\n\r  [");
        strcat(buf1, rt->choutput);
        strcat(buf1, "]\n\r");
      }
      else if (rt->type == RT_ACTION)
      {
        strcat(buf1, "[RT Action:      (");
        strcat(buf1, rt->input);
        strcat(buf1, ") (");
        xprintf(buf2, "%d", rt->mob);
        strcat(buf1, buf2);
        strcat(buf1, ")]\n\r  [");
        strcat(buf1, rt->output);
        strcat(buf1, "]\n\r");
      }
      else if (rt->type == RT_SPELL)
      {
        strcat(buf1, "[RT Spell:       (");
        strcat(buf1, rt->input);
        strcat(buf1, ") (");
        xprintf(buf2, "%s", skill_table[rt->power].name);
        strcat(buf1, buf2);
        strcat(buf1, ")]\n\r");
	}
      else if (rt->type == RT_TEXT)
      {
        strcat(buf1, "[RT Text:        (");
        strcat(buf1, rt->input);
        strcat(buf1, ")]\n\r[\n\r");
        strcat(buf1, rt->output);
	strcat( buf1, "]\n\r" );
    }
      else
      {
        xprintf(buf2, "[RT (keyword: %s) (type: %d) (name: %s)]\n\r",
          rt->input, rt->type, rt->name);
        strcat(buf1, buf2);
        xprintf(buf2, "[(power: %d) (mob: %d)]\n\r",
          rt->power, rt->mob);         
        strcat(buf1, buf2);
        xprintf(buf2, "[Out  : %s]\n\r[ChOut: %s]\n\r",
          rt->output, rt->choutput);
        strcat(buf1, buf2);
      }
    }
  }

    strcat( buf1, "#wCharacters: #0[#W" );
    fcnt = FALSE;
    for ( rch = pRoom->people; rch; rch = rch->next_in_room )
    {
	one_argument( rch->name, buf );
	strcat( buf1, buf );
	strcat( buf1, " " );
	fcnt = TRUE;
    }

    if ( fcnt )
    {
	int end;

	end = strlen(buf1) - 1;
	buf1[end] = ']';
	strcat( buf1, "#n\n\r" );
    }
    else
	strcat( buf1, "none#0]#n\n\r" );

    strcat( buf1, "#wObjects:    #0[#w" );
    fcnt = FALSE;
    for ( obj = pRoom->contents; obj; obj = obj->next_content )
    {
	one_argument( obj->name, buf );
	strcat( buf1, buf );
	strcat( buf1, " " );
	fcnt = TRUE;
    }

    if ( fcnt )
    {
	int end;

	end = strlen(buf1) - 1;
	buf1[end] = ']';
	strcat( buf1, "#n\n\r" );
    }
    else
	strcat( buf1, "none#0]#n\n\r" );

    for ( door = 0; door < MAX_DIR; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = pRoom->exit[door] ) )
	{
	    char word[MAX_INPUT_LENGTH];
	    char reset_state[MAX_STRING_LENGTH];
	    char *state;
	    int i, length;

	    xprintf( buf, "#R-#Y%-5s #wto #0[#W%5d#0] #wKey: #0[#R%5d#0]#n",
		capitalize(dir_name[door]),
		pexit->to_room ? pexit->to_room->vnum : 0,
		pexit->key );
	    strcat( buf1, buf );

	    /*
	     * Format up the exit info.
	     * Capitalize all flags that are not part of the reset info.
	     */
	    strcpy( reset_state, flag_string( exit_flags, pexit->rs_flags ) );
	    state = flag_string( exit_flags, pexit->exit_info );
	    strcat( buf1, " #wExit flags: #0[#W" );
	    for (; ;)
	    {
		state = one_argument( state, word );

		if ( word[0] == '\0' )
		{
		    int end;

		    end = strlen(buf1) - 1;
		    buf1[end] = ']';
		    strcat( buf1, "\n\r" );
		    break;
		}

		if ( str_infix( word, reset_state ) )
		{
		    length = strlen(word);
		    for (i = 0; i < length; i++)
			word[i] = toupper(word[i]);
		}
		strcat( buf1, word );
		strcat( buf1, " " );
	    }

	    if ( pexit->keyword && pexit->keyword[0] != '\0' )
	    {
		xprintf( buf, "#wKwds: #0[#W%s#0]#n\n\r", pexit->keyword );
		strcat( buf1, buf );
	    }
	    if ( pexit->description && pexit->description[0] != '\0' )
	    {
		xprintf( buf, "#B%s#n", pexit->description );
		strcat( buf1, buf );
	    }
	}
    }

    send_to_char( buf1, ch );
    if ( pRoom->rprogs )
    {
	int cnt;

	xprintf(buf, "\n\rROOMPrograms for [%5d]:\n\r", pRoom->vnum);
	send_to_char( buf, ch );

	for (cnt=0, list=pRoom->rprogs; list; list=list->next)
	{
		if (cnt ==0)
		{
			send_to_char ( " Number Vnum Trigger Phrase\n\r", ch );
			send_to_char ( " ------ ---- ------- ------\n\r", ch );
		}

		xprintf(buf, "[%5d] %4d %7s %s\n\r", cnt,
			list->vnum,prog_type_to_name(list->trig_type),
			list->trig_phrase);
		send_to_char( buf, ch );
		cnt++;
	}
    }
    return FALSE;
}

/* OLC 1.1b */
/*****************************************************************************
 Name:		change_exit
 Purpose:	Command interpreter for changing exits.
 Called by:	redit_<dir>.  This is a local function.
 ****************************************************************************/
bool change_exit( CHAR_DATA *ch, char *argument, int door )
{
    ROOM_INDEX_DATA *pRoom;
    char command[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char total_arg[MAX_STRING_LENGTH];
    int  rev;
    int  value = 0;

    EDIT_ROOM(ch, pRoom);

    /* Often used data. */
    rev = rev_dir[door];
    
    if ( argument[0] == '\0' )
    {
	do_help( ch, "EXIT" );
	return FALSE;
    }

    /*
     * Now parse the arguments.
     */
    strcpy( total_arg, argument );
    argument = one_argument( argument, command );
    one_argument( argument, arg );

    if ( !str_cmp( command, "delete" ) )
    {	
	if ( !pRoom->exit[door] )
	{
	    send_to_char( "REdit:  Exit does not exist.\n\r", ch );
	    return FALSE;
	}

	/*
	 * Remove To Room Exit.
	 */
    if (pRoom->exit[door]->to_room && pRoom->exit[door]->to_room->exit[rev])
	{
	    free_exit( pRoom->exit[door]->to_room->exit[rev] );
	    pRoom->exit[door]->to_room->exit[rev] = NULL;
	}

	/*
	 * Remove this exit.
	 */
	free_exit( pRoom->exit[door] );
	pRoom->exit[door] = NULL;

	send_to_char( "Exit unlinked.\n\r", ch );
	return TRUE;
    }

    /*
     * Create a two-way exit.
     */
    if ( !str_cmp( command, "link" ) )
    {
	EXIT_DATA	*pExit;
	ROOM_INDEX_DATA	*pLinkRoom;

	if ( arg[0] == '\0' || !is_number( arg ) )
	{
	    send_to_char( "Syntax:  [direction] link [vnum]\n\r", ch );
	    return FALSE;
	}

	if ( !( pLinkRoom = get_room_index( atoi(arg) ) ) )
	{
	    send_to_char( "REdit:  Non-existant room.\n\r", ch );
	    return FALSE;
	}

	if ( !IS_BUILDER( ch, pLinkRoom->area ) )
	{
	    send_to_char( "REdit:  Cannot link to that area.\n\r", ch );
	    return FALSE;
	}

	if ( pLinkRoom->exit[rev] )
	{
	    send_to_char( "REdit:  Remote side's exit exists.\n\r", ch );
	    return FALSE;
	}

	if ( !pRoom->exit[door] )		/* No exit.		*/
	    pRoom->exit[door] = new_exit();

	pRoom->exit[door]->to_room = pLinkRoom;	/* Assign data.		*/
	pRoom->exit[door]->vnum = value;

	pExit			= new_exit();	/* No remote exit.	*/

	pExit->to_room		= ch->in_room;	/* Assign data.		*/
	pExit->vnum		= ch->in_room->vnum;

	pLinkRoom->exit[rev]	= pExit;	/* Link exit to room.	*/
	do_asave(ch,"changed");

	send_to_char( "Two-way link established.\n\r", ch );
	return TRUE;
    }

    /*
     * Create room and make two-way exit.
     */
    if ( !str_cmp( command, "dig" ) )
    {
	char buf[MAX_INPUT_LENGTH];
	
	if ( arg[0] == '\0' || !is_number( arg ) )
	{
	    send_to_char( "Syntax: [direction] dig <vnum>\n\r", ch );
	    return FALSE;
	}
	
	redit_create( ch, arg );		/* Create the room.	*/
	xprintf( buf, "link %s", arg );
	change_exit( ch, buf, door);		/* Create the exits.	*/
	return TRUE;
	do_asave(ch,"changed");
    }

    /*
     * Create one-way exit.
     */
    if ( !str_cmp( command, "room" ) )
    {
	ROOM_INDEX_DATA *pLinkRoom;

	if ( arg[0] == '\0' || !is_number( arg ) )
	{
	    send_to_char( "Syntax:  [direction] room [vnum]\n\r", ch );
	    return FALSE;
	}

	if ( !( pLinkRoom = get_room_index( atoi( arg ) ) ) )
	{
	    send_to_char( "REdit:  Non-existant room.\n\r", ch );
	    return FALSE;
	}

	if ( !pRoom->exit[door] )
	    pRoom->exit[door] = new_exit();

	pRoom->exit[door]->to_room = pLinkRoom;
	pRoom->exit[door]->vnum = value;

	do_asave(ch,"changed");

	send_to_char( "One-way link established.\n\r", ch );
	return TRUE;
    }

    if ( !str_cmp( command, "remove" ) )
    {
	if ( arg[0] == '\0' )
	{
	    send_to_char( "Syntax:  [direction] remove [key/name/desc]\n\r", ch );
	    return FALSE;
	}

	if ( !pRoom->exit[door] )
	{
	    send_to_char( "REdit:  Exit does not exist.\n\r", ch );
	    return FALSE;
	}

	if ( !str_cmp( argument, "key" ) )
	{
	    pRoom->exit[door]->key = 0;
            send_to_char( "Exit key removed.\n\r", ch );                        
            return TRUE;
	}

	if ( !str_cmp( argument, "name" ) )
	{
	    free_string( pRoom->exit[door]->keyword );
	    pRoom->exit[door]->keyword = &str_empty[0];
            send_to_char( "Exit name removed.\n\r", ch );                        
            return TRUE;
	}

	if ( argument[0] == 'd' && !str_prefix( argument, "description" ) )
	{
	    free_string( pRoom->exit[door]->description );
	    pRoom->exit[door]->description = &str_empty[0];
            send_to_char( "Exit description removed.\n\r", ch );                        
            return TRUE;
	}

	send_to_char( "Syntax:  [direction] remove [key/name/desc]\n\r", ch );
	return FALSE;
    }

    if ( !str_cmp( command, "key" ) )
    {
	OBJ_INDEX_DATA *pObjIndex;

	if ( arg[0] == '\0' || !is_number( arg ) )
	{
	    send_to_char( "Syntax:  [direction] key [vnum]\n\r", ch );
	    return FALSE;
	}

	if ( !( pObjIndex = get_obj_index( atoi( arg ) ) ) )
	{
	    send_to_char( "REdit:  Item does not exist.\n\r", ch );
	    return FALSE;
	}

	if ( pObjIndex->item_type != ITEM_KEY )
	{
	    send_to_char( "REdit:  Item is not a key.\n\r", ch );
	    return FALSE;
	}

	if ( !pRoom->exit[door] )
	    pRoom->exit[door] = new_exit();

	pRoom->exit[door]->key = pObjIndex->vnum;

	send_to_char( "Exit key set.\n\r", ch );
	return TRUE;
    }

    if ( !str_cmp( command, "name" ) )
    {
	if ( arg[0] == '\0' )
	{
	    send_to_char( "Syntax:  [direction] name [string]\n\r", ch );
	    return FALSE;
	}

	if ( !pRoom->exit[door] )
	    pRoom->exit[door] = new_exit();

	free_string( pRoom->exit[door]->keyword );
	pRoom->exit[door]->keyword = str_dup( argument );

	send_to_char( "Exit name set.\n\r", ch );
	return TRUE;
    }

    if ( command[0] == 'd' && !str_prefix( command, "description" ) )
    {
	if ( arg[0] == '\0' )
	{
	    if ( !pRoom->exit[door] )
	        pRoom->exit[door] = new_exit();

	    string_append( ch, &pRoom->exit[door]->description );
	    return TRUE;
	}

	send_to_char( "Syntax:  [direction] desc\n\r", ch );
	return FALSE;
    }

    /*
     * Set the exit flags, needs full argument.
     * ----------------------------------------
     */
    if ( ( value = flag_value( exit_flags, total_arg ) ) != NO_FLAG )
    {
	ROOM_INDEX_DATA *pToRoom;

	/*
	 * Create an exit if none exists.
	 */
	if ( !pRoom->exit[door] )
	    pRoom->exit[door] = new_exit();

	/*
	 * Set door bits for this room.
	 */
	TOGGLE_BIT(pRoom->exit[door]->rs_flags, value);
	pRoom->exit[door]->exit_info = pRoom->exit[door]->rs_flags;

	/*
	 * Set door bits of connected room.
	 * Skip one-way exits and non-existant rooms.
	 */
	if ( ( pToRoom = pRoom->exit[door]->to_room ) && pToRoom->exit[rev] )
	{
	    TOGGLE_BIT(pToRoom->exit[rev]->rs_flags, value);
	    pToRoom->exit[rev]->exit_info =  pToRoom->exit[rev]->rs_flags;
	}

	send_to_char( "Exit flag toggled.\n\r", ch );
	return TRUE;
    }

    return FALSE;
}

bool redit_north( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_NORTH ) )
	return TRUE;

    return FALSE;
}

bool redit_south( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_SOUTH ) )
	return TRUE;

    return FALSE;
}

bool redit_east( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_EAST ) )
	return TRUE;

    return FALSE;
}

bool redit_west( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_WEST ) )
	return TRUE;

    return FALSE;
}

bool redit_up( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_UP ) )
	return TRUE;

    return FALSE;
}

bool redit_down( CHAR_DATA *ch, char *argument )
{
    if ( change_exit( ch, argument, DIR_DOWN ) )
	return TRUE;

    return FALSE;
}

/* OLC 1.1b */
bool redit_move( CHAR_DATA *ch, char *argument )
{
    interpret( ch, argument );
    return FALSE;
}

bool redit_ed( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;
    EXTRA_DESCR_DATA *ed;
    char command[MAX_INPUT_LENGTH];
    char keyword[MAX_INPUT_LENGTH];

    EDIT_ROOM(ch, pRoom);

    argument = one_argument( argument, command );
  argument = one_argument(argument, keyword);

    if ( command[0] == '\0' || keyword[0] == '\0' )
    {
	send_to_char( "Syntax:  ed add [keyword]\n\r", ch );
	send_to_char( "         ed edit [keyword]\n\r", ch );
	send_to_char( "         ed delete [keyword]\n\r", ch );
	send_to_char( "         ed format [keyword]\n\r", ch );
    send_to_char("\n\r         ed type [keyword] [type]\n\r", ch);
    send_to_char("         ed vnum [keyword] [vnum]\n\r", ch);
    send_to_char("         ed action [keyword] [action]\n\r", ch);
    send_to_char("         ed buf1 [keyword] [buf1]\n\r", ch);
    send_to_char("         ed buf2 [keyword] [buf2]\n\r", ch);
	return FALSE;
    }

    if ( !str_cmp( command, "add" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "Syntax:  ed add [keyword]\n\r", ch );
	    return FALSE;
	}

	ed			=   new_extra_descr();
	ed->keyword		=   str_dup( keyword );
	ed->description		=   str_dup( "" );
    ed->buffer1 = str_dup("");
    ed->buffer2 = str_dup("");
    ed->type = ED_TYPE_NONE;
    ed->action = ED_ACTION_NONE;
    ed->vnum = 0;
	ed->next		=   pRoom->extra_descr;
	pRoom->extra_descr	=   ed;

	string_append( ch, &ed->description );

	return TRUE;
    }

    if ( !str_cmp( command, "edit" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "Syntax:  ed edit [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pRoom->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	}

	if ( !ed )
	{
	    send_to_char( "REdit:  Extra description keyword not found.\n\r", ch );
	    return FALSE;
	}

	string_append( ch, &ed->description );

	return TRUE;
    }

    if ( !str_cmp( command, "delete" ) )
    {
	EXTRA_DESCR_DATA *ped = NULL;

	if ( keyword[0] == '\0' )
	{
	    send_to_char( "Syntax:  ed delete [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pRoom->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	    ped = ed;
	}

	if ( !ed )
	{
	    send_to_char( "REdit:  Extra description keyword not found.\n\r", ch );
	    return FALSE;
	}

	if ( !ped )
	    pRoom->extra_descr = ed->next;
	else
	    ped->next = ed->next;

	free_extra_descr( ed );

	send_to_char( "Extra description deleted.\n\r", ch );
	return TRUE;
    }

    if ( !str_cmp( command, "format" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "Syntax:  ed format [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pRoom->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	}

	if ( !ed )
	{
	    send_to_char( "REdit:  Extra description keyword not found.\n\r", ch );
	    return FALSE;
	}

	/* OLC 1.1b */
	if ( strlen(ed->description) >= (MAX_STRING_LENGTH - 4) )
	{
	    send_to_char( "String too long to be formatted.\n\r", ch );
	    return FALSE;
	}

	ed->description = format_string( ed->description );

	send_to_char( "Extra description formatted.\n\r", ch );
	return TRUE;
    }

  if (!str_cmp(command, "vnum"))
  {
    char xarg[MAX_INPUT_LENGTH];

    one_argument(argument, xarg);

    if (keyword[0] == '\0')
    {
      send_to_char("Syntax: ed vnum [keyword] [vnum]\n\r", ch);
      return FALSE;
    }
    for (ed = pRoom->extra_descr; ed; ed = ed->next)
    {
      if (is_name(keyword, ed->keyword))
        break;
    }
    if (!ed)
    {
      send_to_char("No such extra descr.\n\r", ch);
    return FALSE;
}
    ed->vnum = atoi(xarg);
    send_to_char("Vnum set.\n\r", ch);
    return TRUE;
  }
  if (!str_cmp(command, "type"))
  {
    char xarg[MAX_INPUT_LENGTH];

    one_argument(argument, xarg);

    if (keyword[0] == '\0')
    {
      send_to_char("Syntax: ed type [keyword] [type]\n\r", ch);
      send_to_char("Valid types : none, pull, press, touch and push.\n\r", ch);
      return FALSE;
    }
    for (ed = pRoom->extra_descr; ed; ed = ed->next)
    {
      if (is_name(keyword, ed->keyword))
        break;
    }
    if (!ed)
    {
      send_to_char("No such extra descr.\n\r", ch);
      return FALSE;
    }
    if (!str_cmp(xarg, "pull"))
      ed->type = ED_TYPE_PULL;
    else if (!str_cmp(xarg, "press"))
      ed->type = ED_TYPE_PRESS;
    else if (!str_cmp(xarg, "push"))
      ed->type = ED_TYPE_PUSH;
    else if (!str_cmp(xarg, "none"))
    {
      free_string(ed->buffer1);
      ed->buffer1 = str_dup("");
      free_string(ed->buffer2);
      ed->buffer2 = str_dup("");
      ed->type = ED_TYPE_NONE;
    }
    else
    {
      send_to_char("No such type.\n\r", ch);
      return FALSE;
    }
    send_to_char("Type set.\n\r", ch);
    return TRUE;
  }
  if (!str_cmp(command, "action"))
  {
    char xarg[MAX_INPUT_LENGTH];

    one_argument(argument, xarg);

    if (keyword[0] == '\0')
    {
      send_to_char("Syntax: ed action [keyword] [action]\n\r", ch);
      send_to_char("Valid types : none, teleport, spell, object and elevator.\n\r", ch);
      return FALSE;
    }
    for (ed = pRoom->extra_descr; ed; ed = ed->next)
    {
      if (is_name(keyword, ed->keyword))
        break;
    }
    if (!ed)
    {
      send_to_char("No such extra descr.\n\r", ch);
      return FALSE;
    }
    if (!str_cmp(xarg, "teleport"))
      ed->action = ED_ACTION_TELEPORT;
    else if (!str_cmp(xarg, "object"))
      ed->action = ED_ACTION_OBJECT;
    else if (!str_cmp(xarg, "spell"))
      ed->action = ED_ACTION_SPELL;
    else if (!str_cmp(xarg, "elevator"))
      ed->action = ED_ACTION_ELEVATOR;
    else if (!str_cmp(xarg, "none"))
    {
      free_string(ed->buffer1);
      ed->buffer1 = str_dup("");
      free_string(ed->buffer2);
      ed->buffer2 = str_dup("");
      ed->action = ED_ACTION_NONE;
    }
    else
    {
      send_to_char("No such action.\n\r", ch);
      return FALSE;
    }
    send_to_char("Action set.\n\r", ch);
    return TRUE;
  }
  if (!str_cmp(command, "buf1"))
  {
    if (keyword[0] == '\0' || argument[0] == '\0')
    {
      send_to_char("Syntax: ed vnum [keyword] [vnum]\n\r", ch);
      return FALSE;
    }
    for (ed = pRoom->extra_descr; ed; ed = ed->next)
    {
      if (is_name(keyword, ed->keyword))
        break;
    }
    if (!ed)
    {
      send_to_char("No such extra descr.\n\r", ch);
      return FALSE;
    }
    free_string(ed->buffer1);
    ed->buffer1 = str_dup(argument);
    send_to_char("Buffer 1 set.\n\r", ch);
    return TRUE;
  }
  if (!str_cmp(command, "buf2"))
  {
    if (keyword[0] == '\0' || argument[0] == '\0')
    {
      send_to_char("Syntax: ed vnum [keyword] [vnum]\n\r", ch);
      return FALSE;
    }
    for (ed = pRoom->extra_descr; ed; ed = ed->next)
    {
      if (is_name(keyword, ed->keyword))
        break;
    }
    if (!ed)
    {
      send_to_char("No such extra descr.\n\r", ch);
      return FALSE;
    }
    free_string(ed->buffer2);
    ed->buffer2 = str_dup(argument);
    send_to_char("Buffer 2 set.\n\r", ch);
    return TRUE;
  }

  redit_ed(ch, "");
  return FALSE;
}

bool redit_create( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    ROOM_INDEX_DATA *pRoom;
    int value;
    int iHash;
    int door;

    EDIT_ROOM(ch, pRoom);

    value = atoi( argument );

    /* OLC 1.1b */
    if ( argument[0] == '\0' || value <= 0 || value >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	xprintf( output, "Syntax:  create [0 < vnum < %d]\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    if ( argument[0] == '\0' || value <= 0 )
    {
	send_to_char( "Syntax:  create [vnum > 0]\n\r", ch );
	return FALSE;
    }

    pArea = get_vnum_area( value );
    if ( !pArea )
    {
	send_to_char( "REdit:  That vnum is not assigned an area.\n\r", ch );
	return FALSE;
    }

    if ( !IS_BUILDER( ch, pArea ) )
    {
	send_to_char( "REdit:  Vnum in an area you cannot build in.\n\r", ch );
	return FALSE;
    }

    if ( get_room_index( value ) )
    {
	send_to_char( "REdit:  Room vnum already exists.\n\r", ch );
	return FALSE;
    }

    pRoom			= new_room_index();
    pRoom->area			= pArea;
    pRoom->vnum			= value;

    if ( value > top_vnum_room )
        top_vnum_room = value;

    iHash			= value % MAX_KEY_HASH;
    pRoom->next			= room_index_hash[iHash];
    room_index_hash[iHash]	= pRoom;
    ch->desc->pEdit		= (void *)pRoom;
    for ( door = 0; door <= 4; door++ )
    {
        pRoom->track[door]     = str_dup( "" );
        pRoom->track_dir[door] = 0;
    }
    for ( door = 0; door <= 5; door++ )
        pRoom->exit[door] = NULL;

    send_to_char( "Room created.\n\r", ch );
    return TRUE;
}

bool redit_name( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;

    EDIT_ROOM(ch, pRoom);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  name [name]\n\r", ch );
	return FALSE;
    }

    free_string( pRoom->name );
    pRoom->name = str_dup( argument );

    send_to_char( "Name set.\n\r", ch );
    return TRUE;
}

bool redit_desc( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;

    EDIT_ROOM(ch, pRoom);

    if ( argument[0] == '\0' )
    {
	string_append( ch, &pRoom->description );
	return TRUE;
    }

    send_to_char( "Syntax:  desc\n\r", ch );
    return FALSE;
}

bool redit_format( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;

    EDIT_ROOM(ch, pRoom);

    /* OLC 1.1b */
    if ( strlen(pRoom->description) >= (MAX_STRING_LENGTH - 4) )
    {
	send_to_char( "String too long to be formatted.\n\r", ch );
	return FALSE;
    }

    pRoom->description = format_string( pRoom->description );

    send_to_char( "String formatted.\n\r", ch );
    return TRUE;
}

bool redit_mreset( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA	*pRoom;
    MOB_INDEX_DATA	*pMobIndex;
    CHAR_DATA		*newmob;
    char		arg [ MAX_INPUT_LENGTH ];

    RESET_DATA		*pReset;
    char		output [ MAX_STRING_LENGTH ];

    EDIT_ROOM(ch, pRoom);

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number( arg ) )
    {
	send_to_char ( "Syntax:  mreset <vnum> <max ##>\n\r", ch );
	return FALSE;
    }

    if ( !( pMobIndex = get_mob_index( atoi( arg ) ) ) )
    {
	send_to_char( "REdit: No mobile has that vnum.\n\r", ch );
	return FALSE;
    }

    if ( pMobIndex->area != pRoom->area )
    {
	send_to_char( "REdit: No such mobile in this area.\n\r", ch );
	return FALSE;
    }

    /*
     * Create the mobile reset.
     */
    pReset = new_reset_data();
    pReset->command	= 'M';
    pReset->arg1	= pMobIndex->vnum;
    pReset->arg2	= is_number( argument ) ? atoi( argument ) : MAX_MOB;
    pReset->arg3	= pRoom->vnum;
    add_reset( pRoom, pReset, 0/* Last slot*/ );

    /*
     * Create the mobile.
     */
    newmob = create_mobile( pMobIndex );
    char_to_room( newmob, pRoom );

    xprintf( output, "%s (%d) has been loaded and added to resets.\n\r"
	"There will be a maximum of %d loaded to this room.\n\r",
	capitalize( pMobIndex->short_descr ),
	pMobIndex->vnum,
	pReset->arg2 );
    send_to_char( output, ch );
    act( "$n has created $N!", ch, NULL, newmob, TO_ROOM );
    return TRUE;
}

struct wear_type
{
    int	wear_loc;
    int	wear_bit;
};

const struct wear_type wear_table[] = {
    {	WEAR_NONE,	ITEM_TAKE		},
    {	WEAR_LIGHT,	ITEM_LIGHT		},
    {	WEAR_FINGER_L,	ITEM_WEAR_FINGER	},
    {	WEAR_FINGER_R,	ITEM_WEAR_FINGER	},
    {	WEAR_NECK_1,	ITEM_WEAR_NECK		},
    {	WEAR_NECK_2,	ITEM_WEAR_NECK		},
    {	WEAR_BODY,	ITEM_WEAR_BODY		},
    {	WEAR_HEAD,	ITEM_WEAR_HEAD		},
    {	WEAR_LEGS,	ITEM_WEAR_LEGS		},
    {	WEAR_FEET,	ITEM_WEAR_FEET		},
    {	WEAR_HANDS,	ITEM_WEAR_HANDS		},
    {	WEAR_ARMS,	ITEM_WEAR_ARMS		},
    {	WEAR_SHIELD,	ITEM_WEAR_SHIELD	},
    {	WEAR_ABOUT,	ITEM_WEAR_ABOUT		},
    {	WEAR_WAIST,	ITEM_WEAR_WAIST		},
    {	WEAR_WRIST_L,	ITEM_WEAR_WRIST		},
    {	WEAR_WRIST_R,	ITEM_WEAR_WRIST		},
    {	WEAR_WIELD,	ITEM_WIELD		},
    {	WEAR_HOLD,	ITEM_HOLD		},
    {	NO_FLAG,	NO_FLAG			}
};

/*****************************************************************************
 Name:		wear_loc
 Purpose:	Returns the location of the bit that matches the count.
 		1 = first match, 2 = second match etc.
 Called by:	oedit_reset(olc_act.c).
 ****************************************************************************/
int wear_loc(int bits, int count)
{
    int flag;
 
    for (flag = 0; wear_table[flag].wear_bit != NO_FLAG; flag++)
    {
        if ( IS_SET(bits, wear_table[flag].wear_bit) && --count < 1)
            return wear_table[flag].wear_loc;
    }
 
    return NO_FLAG;
}

/*****************************************************************************
 Name:		wear_bit
 Purpose:	Converts a wear_loc into a bit.
 Called by:	redit_oreset(olc_act.c).
 ****************************************************************************/
int wear_bit(int loc)
{
    int flag;
 
    for (flag = 0; wear_table[flag].wear_loc != NO_FLAG; flag++)
    {
        if ( loc == wear_table[flag].wear_loc )
            return wear_table[flag].wear_bit;
    }
 
    return 0;
}

bool redit_oreset( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA	*pRoom;
    OBJ_INDEX_DATA	*pObjIndex;
    OBJ_DATA		*newobj;
    OBJ_DATA		*to_obj;
    CHAR_DATA		*to_mob;
    char		arg1 [ MAX_INPUT_LENGTH ];
    char		arg2 [ MAX_INPUT_LENGTH ];
    int			olevel = 0;

    RESET_DATA		*pReset;
    char		output [ MAX_STRING_LENGTH ];

    EDIT_ROOM(ch, pRoom);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
	send_to_char ( "Syntax:  oreset <vnum> <args>\n\r", ch );
	send_to_char ( "        -no_args               = into room\n\r", ch );
	send_to_char ( "        -<obj_name>            = into obj\n\r", ch );
	send_to_char ( "        -<mob_name> <wear_loc> = into mob\n\r", ch );
	return FALSE;
    }

    if ( !( pObjIndex = get_obj_index( atoi( arg1 ) ) ) )
    {
	send_to_char( "REdit: No object has that vnum.\n\r", ch );
	return FALSE;
    }

    if ( pObjIndex->area != pRoom->area )
    {
	send_to_char( "REdit: No such object in this area.\n\r", ch );
	return FALSE;
    }

    /*
     * Load into room.
     */
    if ( arg2[0] == '\0' )
    {
	pReset		= new_reset_data();
	pReset->command	= 'O';
	pReset->arg1	= pObjIndex->vnum;
	pReset->arg2	= 0;
	pReset->arg3	= pRoom->vnum;
	add_reset( pRoom, pReset, 0/* Last slot*/ );

	newobj = create_object( pObjIndex, number_fuzzy( olevel ) );
	obj_to_room( newobj, pRoom );

	xprintf( output, "%s (%d) has been loaded and added to resets.\n\r",
	    capitalize( pObjIndex->short_descr ),
	    pObjIndex->vnum );
	send_to_char( output, ch );
    }
    else
    /*
     * Load into object's inventory.
     */
    if ( argument[0] == '\0'
    && ( ( to_obj = get_obj_list( ch, arg2, pRoom->contents ) ) != NULL ) )
    {
	pReset		= new_reset_data();
	pReset->command	= 'P';
	pReset->arg1	= pObjIndex->vnum;
	pReset->arg2	= 0;
	pReset->arg3	= to_obj->pIndexData->vnum;
	add_reset( pRoom, pReset, 0/* Last slot*/ );

	newobj = create_object( pObjIndex, number_fuzzy( olevel ) );
	newobj->cost = 0;
	obj_to_obj( newobj, to_obj );

	xprintf( output, "%s (%d) has been loaded into "
	    "%s (%d) and added to resets.\n\r",
	    capitalize( newobj->short_descr ),
	    newobj->pIndexData->vnum,
	    to_obj->short_descr,
	    to_obj->pIndexData->vnum );
	send_to_char( output, ch );
    }
    else
    /*
     * Load into mobile's inventory.
     */
    if ( ( to_mob = get_char_room( ch, NULL, arg2 ) ) != NULL )
    {
	int	wear_loca;

	/*
	 * Make sure the location on mobile is valid.
	 */
	if ( (wear_loca = flag_value( wear_loc_flags, argument )) == NO_FLAG )
	{
	    send_to_char( "REdit: Invalid wear_loc.  '? wear-loc'\n\r", ch );
	    return FALSE;
	}

	/*
	 * Disallow loading a sword(WEAR_WIELD) into WEAR_HEAD.
	 */
	if ( !IS_SET( pObjIndex->wear_flags, wear_bit(wear_loca) ) )
	{
	    xprintf( output,
	        "%s (%d) has wear flags: [%s]\n\r",
	        capitalize( pObjIndex->short_descr ),
	        pObjIndex->vnum,
		flag_string( wear_flags, pObjIndex->wear_flags ) );
	    send_to_char( output, ch );
	    return FALSE;
	}

	/*
	 * Can't load into same position.
	 */
	if ( get_eq_char( to_mob, wear_loca ) )
	{
	    send_to_char( "REdit:  Object already equipped.\n\r", ch );
	    return FALSE;
	}

	pReset		= new_reset_data();
	pReset->arg1	= pObjIndex->vnum;
	pReset->arg2	= wear_loca;
	if ( pReset->arg2 == WEAR_NONE )
	    pReset->command = 'G';
	else
	    pReset->command = 'E';
	pReset->arg3	= wear_loca;

	add_reset( pRoom, pReset, 0/* Last slot*/ );

	olevel  = URANGE( 0, to_mob->level - 2, LEVEL_HERO );

	    newobj = create_object( pObjIndex, number_fuzzy( olevel ) );

	obj_to_char( newobj, to_mob );
	if ( pReset->command == 'E' )
	    equip_char( to_mob, newobj, pReset->arg3 );

	xprintf( output, "%s (%d) has been loaded "
	    "%s of %s (%d) and added to resets.\n\r",
	    capitalize( pObjIndex->short_descr ),
	    pObjIndex->vnum,
	    flag_string( wear_loc_strings, pReset->arg3 ),
	    to_mob->short_descr,
	    to_mob->pIndexData->vnum );
	send_to_char( output, ch );
    }
    else	/* Display Syntax */
    {
	send_to_char( "REdit:  That mobile isn't here.\n\r", ch );
	return FALSE;
    }

    act( "$n has created $p!", ch, newobj, NULL, TO_ROOM );
    return TRUE;
}

/*
 * Object Editor Functions.
 */
void show_obj_values( CHAR_DATA *ch, OBJ_INDEX_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];

    switch( obj->item_type )
    {
	default:	/* No values. */
	    break;
            
	case ITEM_LIGHT:
            if ( obj->value[2] == -1 )
		xprintf( buf, "[v2] Light:  Infinite[-1]\n\r" );
            else
		xprintf( buf, "[v2] Light:  [%d]\n\r", obj->value[2] );
	    send_to_char( buf, ch );
	    break;

        case ITEM_GRENADE:
            xprintf(buf, "[v2] Explosive Power :   %d\n\r", obj->value[2]);
            send_to_char(buf, ch);
            break;

	case ITEM_WAND:
	case ITEM_STAFF:
            xprintf( buf,
		"[v0] Level:          [%d]\n\r"
		"[v1] Charges Total:  [%d]\n\r"
		"[v2] Charges Left:   [%d]\n\r"
		"[v3] Spell:          %s\n\r",
		obj->value[0],
		obj->value[1],
		obj->value[2],
		obj->value[3] != -1 ? skill_table[obj->value[3]].name
		                    : "none" );
	    send_to_char( buf, ch );
	    break;

        case ITEM_FAITHTOKEN:
            xprintf(buf, "[v0] Points:  [%d]\n\r", obj->value[0]);
            send_to_char(buf, ch);
            break;

	case ITEM_SCROLL:
	case ITEM_POTION:
	case ITEM_PILL:
            xprintf( buf,
		"[v0] Level:  [%d]\n\r"
		"[v1] Spell:  %s\n\r"
		"[v2] Spell:  %s\n\r"
		"[v3] Spell:  %s\n\r",
		obj->value[0],
		obj->value[1] != -1 ? skill_table[obj->value[1]].name
		                    : "none",
		obj->value[2] != -1 ? skill_table[obj->value[2]].name
                                    : "none",
		obj->value[3] != -1 ? skill_table[obj->value[3]].name
		                    : "none" );
	    send_to_char( buf, ch );
	    break;

        case ITEM_PORTAL:
      xprintf(buf, "[v0] Target Room:   %d\n\r", obj->value[0]);
      send_to_char(buf, ch);
      break;

	case ITEM_WEAPON:
            xprintf( buf,
 		"[v0] ItemAffects:    %d\n\r"
        	"[v1] Damage minimum: [%d]\n\r"
        	"[v2] Damage maximum: [%d]\n\r"
		"[v3] Type:           %s\n\r",
		obj->value[0],
		obj->value[1],
		obj->value[2],
		flag_string( weapon_flags, obj->value[3] ) );
	    send_to_char( buf, ch );
	    break;
	case ITEM_ARMOR:
	    xprintf(buf, 
		"[v0] Armor Class:  %d\n\r"
		"[v3] ItemAffects:  %d\n\r",
		obj->value[0],
		obj->value[3]);
		send_to_char( buf, ch );
		break;
	case ITEM_MISSILE:
        xprintf(buf, 
        "[v0] Ammo Vnum:    %d\n\r"
        "[v1] Current Ammo: %d\n\r"
		"[v2] Max Ammo:     %d\n\r"
	    "[v3] Ammo Type:    %d\n\r",
		 obj->value[0],obj->value[1], 
		 obj->value[2], obj->value[3]);
         send_to_char(buf, ch);
         break;
    case ITEM_AMMO:
        xprintf(buf, 
        "[v0] Chance to Hit: %d\n\r"
        "[v1] Min Damage:    %d\n\r"
        "[v2] Max Damage:    %d\n\r"
        "[v3] Ammo Type:     %d\n\r",
        obj->value[0], obj->value[1],
		obj->value[2], obj->value[3]);
            send_to_char(buf, ch);
            break;
	case ITEM_PIECE:
	    xprintf(buf, 
		"[v0] Prev piece vnum:  %d\n\r"
		"[v1] Next piece vnum:  %d\n\r"
		"[v2] Final Item vnum:  %d\n\r",
		obj->value[0],
		obj->value[1],
		obj->value[2]);
		send_to_char( buf, ch );
		break;
	case ITEM_CONTAINER:
	    xprintf( buf,
		"[v0] Weight: [%d kg]\n\r"
		"[v1] Flags:  [%s]\n\r"
		"[v2] Key:    %s [%d]\n\r",
		obj->value[0],
		flag_string( container_flags, obj->value[1] ),
                get_obj_index(obj->value[2])
                    ? get_obj_index(obj->value[2])->short_descr
                    : "none", obj->value[2]);
	    send_to_char( buf, ch );
	    break;

	case ITEM_DRINK_CON:
	    xprintf( buf,
	        "[v0] Liquid Total: [%d]\n\r"
	        "[v1] Liquid Left:  [%d]\n\r"
	        "[v2] Liquid:       %s\n\r"
	        "[v3] Poisoned:     %s\n\r",
	        obj->value[0],
	        obj->value[1],
	        flag_string( liquid_flags, obj->value[2] ),
	        obj->value[3] != 0 ? "Yes" : "No" );
	    send_to_char( buf, ch );
	    break;
     
    case ITEM_SLOT_MACHINE:
        xprintf( buf, "[v0] Gold Cost:       [%d]\n\r"
                      "[v1] Jackpot Value:   [%d]\n\r"
                      "[v2] Number of Bars:  [%d]\n\r"
                      "[v3] Partial Jackpot: [%d]\n\r", obj->value[0], obj->value[1], obj->value[2], obj->value[3]);
             send_to_char(buf, ch);
             break;
    
	case ITEM_FOOD:
	    xprintf( buf,
		"[v0] Food hours: [%d]\n\r"
		"[v3] Poisoned:   %s\n\r",
		obj->value[0],
		obj->value[3] != 0 ? "Yes" : "No" );
	    send_to_char( buf, ch );
	    break;

	case ITEM_MONEY:
            xprintf( buf, "[v0] Bones:   [%d]\n\r", obj->value[0] );
	    send_to_char( buf, ch );
	    break;
    }

    return;
}

bool set_obj_values( CHAR_DATA *ch, OBJ_INDEX_DATA *pObj, int value_num, char *argument)
{
    switch( pObj->item_type )
    {
        default:
            break;
            
        case ITEM_LIGHT:
	    switch ( value_num )
	    {
	        default:
		    do_help( ch, "ITEM_LIGHT" );
	            return FALSE;
	        case 2:
	            send_to_char( "HOURS OF LIGHT SET.\n\r\n\r", ch );
	            pObj->value[2] = atoi( argument );
	            break;
	    }
            break;


        case ITEM_GRENADE:
           switch(value_num)
           {
             default: return FALSE;
             case 2: pObj->value[2] = atoi(argument);
                     send_to_char("Ok.\n\r", ch);
                     break;
           }
           break;

        case ITEM_WAND:
        case ITEM_STAFF:
	    switch ( value_num )
	    {
	        default:
		    do_help( ch, "ITEM_STAFF_WAND" );
	            return FALSE;
	        case 0:
	            send_to_char( "SPELL LEVEL SET.\n\r\n\r", ch );
	            pObj->value[0] = atoi( argument );
	            break;
	        case 1:
	            send_to_char( "TOTAL NUMBER OF CHARGES SET.\n\r\n\r", ch );
	            pObj->value[1] = atoi( argument );
	            break;
	        case 2:
	            send_to_char( "CURRENT NUMBER OF CHARGES SET.\n\r\n\r", ch );
	            pObj->value[2] = atoi( argument );
	            break;
	        case 3:
	            send_to_char( "SPELL TYPE SET.\n\r", ch );
	            pObj->value[3] = skill_lookup( argument );
	            break;
	    }
            break;

        case ITEM_FAITHTOKEN:
            switch ( value_num )
            {
              default:
                return FALSE;
              case 0:
                send_to_char("V0 SET.\n\r\n\r", ch );
                pObj->value[0] = atoi(argument);
                break;
            }
            break;
	case ITEM_SLOT_MACHINE:
       switch (value_num)
         {
         case 0:
           send_to_char( "BONE COST SET.\n\r\n\r", ch);
           pObj->value[0] = atoi( argument );
           break;
         case 1:
           send_to_char( "JACKPOT VALUE.\n\r\n\r", ch);
           pObj->value[1] = atoi( argument );
           break;
         case 2:
           send_to_char( "NUMBER OF BARS SET.\n\r\n\r", ch);
           pObj->value[2] = atoi( argument );
           break;
         case 3:
           send_to_char( "PARTIAL JACKPOT SET.\n\r\n\r", ch);
           pObj->value[3] = atoi( argument );
           break;
         }
       break;
		
        case ITEM_SCROLL:
        case ITEM_POTION:
        case ITEM_PILL:
	    switch ( value_num )
	    {
	        default:
		    do_help( ch, "ITEM_SCROLL_POTION_PILL" );
	            return FALSE;
	        case 0:
	            send_to_char( "SPELL LEVEL SET.\n\r\n\r", ch );
	            pObj->value[0] = atoi( argument );
	            break;
	        case 1:
	            send_to_char( "SPELL TYPE 1 SET.\n\r\n\r", ch );
	            pObj->value[1] = skill_lookup( argument );
	            break;
	        case 2:
	            send_to_char( "SPELL TYPE 2 SET.\n\r\n\r", ch );
	            pObj->value[2] = skill_lookup( argument );
	            break;
	        case 3:
	            send_to_char( "SPELL TYPE 3 SET.\n\r\n\r", ch );
	            pObj->value[3] = skill_lookup( argument );
	            break;
	    }
            break;
	case ITEM_ARMOR:
	    switch ( value_num )
	    {
		default:
			return FALSE;
		case 0:
			send_to_char("V0 SET.\n\r\n\r", ch );
			pObj->value[0] = atoi(argument);
			break;
		case 3:
			send_to_char("V3 SET.\n\r\n\r", ch );
			pObj->value[3] = atoi(argument);
			break;
	    }
	break;
	case ITEM_AMMO:
	switch ( value_num )
	{
		default:
			return FALSE;
		case 0:
		    send_to_char("Chance to hit set.\n\r\n\r", ch );
		    pObj->value[0] = atoi(argument);
		    break;
		case 1:
		    send_to_char("Minimum Damage set.\n\r\n\r", ch );
		    pObj->value[1] = atoi(argument);
		    break;
		case 2:
		    send_to_char("Maximum Damage set.\n\r\n\r", ch );
		    pObj->value[2] = atoi(argument);
		    break;
		case 3:
		    send_to_char("Ammo Type Set.\n\r\n\r", ch );
		    pObj->value[3] = atoi(argument);
		    break;
	}
	break;
	case ITEM_MISSILE:
	switch ( value_num )
	{
		default:
			return FALSE;
		case 0:
		    send_to_char("Ammo vnum set.\n\r\n\r", ch );
		    pObj->value[0] = atoi(argument);
		    break;
		case 1:
		    send_to_char("Starting Ammo Set.\n\r\n\r", ch );
		    pObj->value[1] = atoi(argument);
		    break;
		case 2:
		    send_to_char("Max Ammo Set.\n\r\n\r", ch );
		    pObj->value[2] = atoi(argument);
		    break;
		case 3:
		    send_to_char("Ammo Type Set.\n\r\n\r", ch );
		    pObj->value[3] = atoi(argument);
		    break;
	}
	break;
	case ITEM_PIECE:
	    switch ( value_num )
	    {
		default:
			return FALSE;
		case 0:
			send_to_char("Prev Vnum Set.\n\r\n\r", ch );
			pObj->value[0] = atoi(argument);
			break;
		case 1:
			send_to_char("Next Vnum Set.\n\r\n\r", ch );
			pObj->value[1] = atoi(argument);
			break;
		case 2:
			send_to_char("Finished Item Vnum Set.\n\r\n\r", ch );
			pObj->value[2] = atoi(argument);
			break;
	    }
	break;
        case ITEM_WEAPON:
	    switch ( value_num )
	    {
	        default:
		    do_help( ch, "ITEM_WEAPON" );
	            return FALSE;
		case 0:
			send_to_char(" V0 SET.\n\r\n\r", ch);
			pObj->value[0] = atoi(argument);
			break;
	        case 1:
	            send_to_char( "MINIMUM DAMAGE SET.\n\r\n\r", ch );
	            pObj->value[1] = atoi( argument );
	            break;
	        case 2:
	            send_to_char( "MAXIMUM DAMAGE SET.\n\r\n\r", ch );
	            pObj->value[2] = atoi( argument );
	            break;
	        case 3:
	            send_to_char( "WEAPON TYPE SET.\n\r\n\r", ch );
	            pObj->value[3] = flag_value( weapon_flags, argument );
	            break;
	    }
            break;

        case ITEM_CONTAINER:
	    switch ( value_num )
	    {
		int value;
		
		default:
		    do_help( ch, "ITEM_CONTAINER" );
	            return FALSE;
		case 0:
	            send_to_char( "WEIGHT CAPACITY SET.\n\r\n\r", ch );
	            pObj->value[0] = atoi( argument );
	            break;
		case 1:
	            if ( ( value = flag_value( container_flags, argument ) )
	              != NO_FLAG )
	        	TOGGLE_BIT(pObj->value[1], value);
		    else
		    {
			do_help ( ch, "ITEM_CONTAINER" );
			return FALSE;
		    }
	            send_to_char( "CONTAINER TYPE SET.\n\r\n\r", ch );
	            break;
		case 2:
		    if ( atoi(argument) != 0 )
		    {
			if ( !get_obj_index( atoi( argument ) ) )
			{
			    send_to_char( "THERE IS NO SUCH ITEM.\n\r\n\r", ch );
			    return FALSE;
			}

			if ( get_obj_index( atoi( argument ) )->item_type != ITEM_KEY )
			{
			    send_to_char( "THAT ITEM IS NOT A KEY.\n\r\n\r", ch );
			    return FALSE;
			}
		    }
		    send_to_char( "CONTAINER KEY SET.\n\r\n\r", ch );
		    pObj->value[2] = atoi( argument );
		    break;
	    }
	    break;

	case ITEM_DRINK_CON:
	    switch ( value_num )
	    {
	        default:
		    do_help( ch, "ITEM_DRINK" );
	            return FALSE;
	        case 0:
	            send_to_char( "MAXIMUM AMOUT OF LIQUID HOURS SET.\n\r\n\r", ch );
	            pObj->value[0] = atoi( argument );
	            break;
	        case 1:
	            send_to_char( "CURRENT AMOUNT OF LIQUID HOURS SET.\n\r\n\r", ch );
	            pObj->value[1] = atoi( argument );
	            break;
	        case 2:
	            send_to_char( "LIQUID TYPE SET.\n\r\n\r", ch );
	            pObj->value[2] = flag_value( liquid_flags, argument );
	            break;
	        case 3:
	            send_to_char( "POISON VALUE TOGGLED.\n\r\n\r", ch );
	            pObj->value[3] = ( pObj->value[3] == 0 ) ? 1 : 0;
	            break;
	    }
            break;

	case ITEM_FOOD:
	    switch ( value_num )
	    {
	        default:
		    do_help( ch, "ITEM_FOOD" );
	            return FALSE;
	        case 0:
	            send_to_char( "HOURS OF FOOD SET.\n\r\n\r", ch );
	            pObj->value[0] = atoi( argument );
	            break;
	        case 3:
	            send_to_char( "POISON VALUE TOGGLED.\n\r\n\r", ch );
	            pObj->value[3] = ( pObj->value[3] == 0 ) ? 1 : 0;
	            break;
	    }
            break;

    case ITEM_PORTAL:
      switch (value_num)
      {
        default:
          break;
        case 0:
          pObj->value[0] = atoi(argument);
          send_to_char("Ok.\n\r", ch);
          break;
      }
      break;

	case ITEM_MONEY:
	    switch ( value_num )
	    {
	        default:
		    do_help( ch, "ITEM_MONEY" );
	            return FALSE;
	        case 0:
	            send_to_char( "BONES AMOUNT SET.\n\r\n\r", ch );
	            pObj->value[0] = atoi( argument );
	            break;
	    }
            break;
    }

    show_obj_values( ch, pObj );

    return TRUE;
}

bool oedit_show( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int cnt;
    PROG_LIST		*list;

    EDIT_OBJ(ch, pObj);

    xprintf( buf, "#wName:        #0[#W%s#0]#n\n\r#wArea:        #0[#W%5d#0] #B%s#n\n\r",
	pObj->name,
	!pObj->area ? -1        : pObj->area->vnum,
	!pObj->area ? "No Area" : pObj->area->name );
    send_to_char( buf, ch );


    xprintf( buf, "#wVnum:        #0[#W%5d#0]#n\n\r#wType:        #0[#W%s#0]#n\n\r",
	pObj->vnum,
	flag_string( type_flags, pObj->item_type ) );
    send_to_char( buf, ch );

    xprintf( buf, "#wWear flags:  #0[#W%s#0]#n\n\r",
	flag_string( wear_flags, pObj->wear_flags ) );
    send_to_char( buf, ch );

    xprintf( buf, "#wExtra flags: #0[#W%s#0]#n\n\r",
	flag_string( extra_flags, pObj->extra_flags ) );
    send_to_char( buf, ch );
    
    xprintf( buf, "#wQuest flags: #0[#W%s#0]#n\n\r",
	flag_string( quest_flags, pObj->quest ) );
    send_to_char( buf, ch );
    
    xprintf( buf, "#wWeight:      #0[#W%d#0]#n\n\r#wCost:        #0[#W%d#0]#n\n\r",
	pObj->weight, pObj->cost );
    send_to_char( buf, ch );
    
    xprintf( buf, "#wCondition:   #0[#W%d#0]#n\n\r#wToughness:   #0[#W%d#0]#n\n\r",
	pObj->condition, pObj->toughness );
    send_to_char( buf, ch );
    
    xprintf( buf, "#wResistance:  #0[#W%d#0]#n\n\r",
	pObj->resistance );
    send_to_char( buf, ch );

    if ( pObj->extra_descr )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "#wEx desc kwd: ", ch );

	for ( ed = pObj->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( "#0[#W", ch );
	    send_to_char( ed->keyword, ch );
	    send_to_char( "#0]", ch );
	}

	send_to_char( "#n\n\r", ch );
    }

    xprintf( buf, "#wShort desc:  #n%s\n\r#wLong desc:\n\r     #n%s\n\r",
	pObj->short_descr, pObj->description );
    send_to_char( buf, ch );

    for ( cnt = 0, paf = pObj->affected; paf; paf = paf->next )
    {
	if ( cnt == 0 )
	{
	    send_to_char( "#wNumber Modifier Affects\n\r", ch );
	    send_to_char( "#B------ -------- -------\n\r", ch );
	}
	xprintf( buf, "#0[#Y%4d#0] #w%-8d #B%s#n\n\r", cnt,
	    paf->modifier,
	    flag_string( apply_flags, paf->location ) );
	send_to_char( buf, ch );
	cnt++;
    }

    show_obj_values( ch, pObj );
    if ( pObj->oprogs )
    {
	int ncnt;

	xprintf(buf, "\n\rOBJPrograms for [%5d]:\n\r", pObj->vnum);
	send_to_char( buf, ch );

	for (ncnt=0, list=pObj->oprogs; list; list=list->next)
	{
		if (ncnt ==0)
		{
			send_to_char ( " Number Vnum Trigger Phrase\n\r", ch );
			send_to_char ( " ------ ---- ------- ------\n\r", ch );
		}

		xprintf(buf, "[%5d] %4d %7s %s\n\r", ncnt,
			list->vnum,prog_type_to_name(list->trig_type),
			list->trig_phrase);
		send_to_char( buf, ch );
		ncnt++;
	}
    }
    return FALSE;
}

/*
 * Need to issue warning if flag isn't valid.
 */
bool oedit_addaffect( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    AFFECT_DATA *pAf;
    char loc[MAX_STRING_LENGTH];
    char mod[MAX_STRING_LENGTH];

    EDIT_OBJ(ch, pObj);

    argument = one_argument( argument, loc );
    one_argument( argument, mod );

    if ( loc[0] == '\0' || mod[0] == '\0' || atoi(mod) == 0 )
    {
	send_to_char( "Syntax:  addaffect [location] [#mod]\n\r", ch );
	return FALSE;
    }

    pAf             =   new_affect();
    pAf->location   =   flag_value( apply_flags, loc );
    pAf->modifier   =   atoi( mod );
    pAf->type       =   -1;
    pAf->duration   =   -1;
    pAf->bitvector  =   0;
    pAf->next       =   pObj->affected;
    pObj->affected  =   pAf;

    send_to_char( "Affect added.\n\r", ch);
    return TRUE;
}

bool oedit_delete( CHAR_DATA *ch, char *argument )
{
     OBJ_INDEX_DATA *pObj;
     OBJ_INDEX_DATA *iObj;
     OBJ_INDEX_DATA *sObj;
     RESET_DATA *pReset = NULL;
     RESET_DATA *prev = NULL;
     ROOM_INDEX_DATA *pRoom = NULL;
     char arg[MIL];
     char buf[MSL];
     int oindex, count, iHash, i;

     if ( argument[0] == '\0' )
     {
          send_to_char( "Syntax:  oedit delete [vnum]\n\r", ch );
          return FALSE;
     }

     one_argument( argument, arg );

     if( is_number( arg ) )
     {
          oindex = atoi( arg );
          pObj = get_obj_index( oindex );
     }
     else
     {
          send_to_char( "That is not a number.\n\r", ch );
          return FALSE;
     }
     SET_BIT( pObj->area->area_flags, AREA_CHANGED );
     /* Remove it from the object list */

     iHash = oindex % MAX_KEY_HASH;

     sObj = obj_index_hash[iHash];

     if( sObj->next == NULL ) /* only entry */
          obj_index_hash[iHash] = NULL;
     else if( sObj == pObj ) /* first entry */
          obj_index_hash[iHash] = pObj->next;
     else /* everything else */
     {
          for( iObj = sObj; iObj != NULL; iObj = iObj->next )
          {
               if( iObj ==  pObj )
               {
                    sObj->next = pObj->next;
                    break;
               }
               sObj = iObj;
          }
     }

     if( top_vnum_obj == oindex )
          for( i = 1; i < oindex; i++ )
               if( get_obj_index( i ) )
                    top_vnum_obj = i;

     top_obj_index--;

     /* Now crush all resets */
     count = 0;
     for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
     {
          for( pRoom = room_index_hash[iHash]; pRoom; pRoom = pRoom->next )
          {
               prev = pRoom->reset_first;
               for( pReset = pRoom->reset_first; pReset; pReset = pReset->next )
               {
                    switch( pReset->command )
                    {
                         case 'O':
                         case 'E':
                         case 'P':
                         case 'G':
                              if( ( pReset->arg1 == oindex ) ||
                                  ( (  pReset->command == 'P' ) && (pReset->arg3 == oindex ) ) )
                              {          
                                   if( pRoom->reset_first == pReset )
                                   {
                                        pRoom->reset_first = pReset->next;
                                        if( !pRoom->reset_first )
                                             pRoom->reset_last = NULL;
                                   }
                                   else if( pRoom->reset_last == pReset )
                                   {
                                        pRoom->reset_last = prev;
                                        prev->next = NULL;
                                   }
                                   else
                                   {
                                        prev->next = prev->next->next;
                                   }

                                   count++;
                                   SET_BIT( pRoom->area->area_flags,AREA_CHANGED );
                              }
                    }
                    prev = pReset;
               }
          }
     }
     xprintf( buf, "Removed object vnum #C%d#n and #Y%d#n resets.\n\r", oindex,count );
     send_to_char( buf, ch );
     return TRUE;
}

/*
 * My thanks to Hans Hvidsten Birkeland and Noam Krendel(Walker)
 * for really teaching me how to manipulate pointers.
 */
bool oedit_delaffect( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    AFFECT_DATA *pAf;
    AFFECT_DATA *pAf_next;
    char affect[MAX_STRING_LENGTH];
    int  value;
    int  cnt = 0;

    EDIT_OBJ(ch, pObj);

    one_argument( argument, affect );

    if ( !is_number( affect ) || affect[0] == '\0' )
    {
	send_to_char( "Syntax:  delaffect [#affect]\n\r", ch );
	return FALSE;
    }

    value = atoi( affect );

    if ( value < 0 )
    {
	send_to_char( "Only non-negative affect-numbers allowed.\n\r", ch );
	return FALSE;
    }

    if ( !( pAf = pObj->affected ) )
    {
	send_to_char( "OEdit:  Non-existant affect.\n\r", ch );
	return FALSE;
    }

    if( value == 0 )	/* First case: Remove first affect */
    {
	pAf = pObj->affected;
	pObj->affected = pAf->next;
	free_affect( pAf );
    }
    else		/* Affect to remove is not the first */
    {
	while ( ( pAf_next = pAf->next ) && ( ++cnt < value ) )
	     pAf = pAf_next;

	if( pAf_next )		/* See if it's the next affect */
	{
	    pAf->next = pAf_next->next;
	    free_affect( pAf_next );
	}
	else                                 /* Doesn't exist */
	{
	     send_to_char( "No such affect.\n\r", ch );
	     return FALSE;
	}
    }

    send_to_char( "Affect removed.\n\r", ch);
    return TRUE;
}

bool oedit_name( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  name [string]\n\r", ch );
	return FALSE;
    }

    free_string( pObj->name );
    pObj->name = str_dup( argument );

    send_to_char( "Name set.\n\r", ch);
    return TRUE;
}

bool oedit_short( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  short [string]\n\r", ch );
	return FALSE;
    }

    free_string( pObj->short_descr );
    pObj->short_descr = str_dup( argument );
    pObj->short_descr[0] = LOWER( pObj->short_descr[0] );

    send_to_char( "Short description set.\n\r", ch);
    return TRUE;
}

bool oedit_long( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  long [string]\n\r", ch );
	return FALSE;
    }
        
    free_string( pObj->description );
    pObj->description = str_dup( argument );
    pObj->description[0] = UPPER( pObj->description[0] );

    send_to_char( "Long description set.\n\r", ch);
    return TRUE;
}

bool set_value( CHAR_DATA *ch, OBJ_INDEX_DATA *pObj, char *argument, int value )
{
    if ( argument[0] == '\0' )
    {
	set_obj_values( ch, pObj, -1, '\0' );
	return FALSE;
    }

    if ( set_obj_values( ch, pObj, value, argument ) )
	return TRUE;

    return FALSE;
}

/*****************************************************************************
 Name:		oedit_values
 Purpose:	Finds the object and sets its value.
 Called by:	The four valueX functions below.
 ****************************************************************************/
bool oedit_values( CHAR_DATA *ch, char *argument, int value )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( set_value( ch, pObj, argument, value ) )
        return TRUE;

    return FALSE;
}

bool oedit_value0( CHAR_DATA *ch, char *argument )
{
    if ( oedit_values( ch, argument, 0 ) )
        return TRUE;

    return FALSE;
}

bool oedit_value1( CHAR_DATA *ch, char *argument )
{
    if ( oedit_values( ch, argument, 1 ) )
        return TRUE;

    return FALSE;
}

bool oedit_value2( CHAR_DATA *ch, char *argument )
{
    if ( oedit_values( ch, argument, 2 ) )
        return TRUE;

    return FALSE;
}

bool oedit_value3( CHAR_DATA *ch, char *argument )
{
    if ( oedit_values( ch, argument, 3 ) )
        return TRUE;

    return FALSE;
}

bool oedit_weight( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "Syntax:  weight [number]\n\r", ch );
	return FALSE;
    }

  pObj->weight = UMAX(1, atoi(argument));

    send_to_char( "Weight set.\n\r", ch);
    return TRUE;
}

bool oedit_condition( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    int value;
    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "Syntax:  condition [1 - 100] (100 being best).\n\r", ch );
	return FALSE;
    }
    
    value = atoi( argument );

    if(value > 100 || value < 1)
    {
    	send_to_char( "Syntax:  condition [1 - 100] (100 being best).\n\r", ch );
	return FALSE;
    }

    pObj->condition = value;
    
    send_to_char( "Condition set.\n\r", ch);
    return TRUE;
}

bool oedit_resistance( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    int value;
    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "Syntax:  resistance [1 - 25] (1 being best) use 1 for relics.\n\r", ch );
	return FALSE;
    }
    
    value = atoi( argument );

    if(value > 25 || value < 1)
    {
    	send_to_char( "Syntax:  resistance [1 - 25] (1 being best) use 1 for relics.\n\r", ch );
	return FALSE;
    }

    pObj->resistance = value;
    
    send_to_char( "Resistance set.\n\r", ch);
    return TRUE;
}

bool oedit_toughness( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    int value;
    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "Syntax:  toughness [5 - 100] (100 being best)\n\r", ch );
	return FALSE;
    }
    
    value = atoi( argument );

    if(value > 100 || value < 5)
    {
    	send_to_char( "Syntax:  toughness [5 - 100] (100 being best)\n\r", ch );
	return FALSE;
    }

    pObj->toughness = value;
    
    send_to_char( "Toughness set.\n\r", ch);
    return TRUE;
}

bool oedit_cost( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;

    EDIT_OBJ(ch, pObj);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "Syntax:  cost [number]\n\r", ch );
	return FALSE;
    }

    pObj->cost = atoi( argument );

    send_to_char( "Cost set.\n\r", ch);
    return TRUE;
}

bool oedit_create( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    AREA_DATA *pArea;
    int  value;
    int  iHash;

    value = atoi( argument );

    /* OLC 1.1b */
    if ( argument[0] == '\0' || value <= 0 || value >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	xprintf( output, "Syntax:  create [0 < vnum < %d]\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    pArea = get_vnum_area( value );
    if ( !pArea )
    {
	send_to_char( "OEdit:  That vnum is not assigned an area.\n\r", ch );
	return FALSE;
    }

    if ( !IS_BUILDER( ch, pArea ) && ch->level < 12 )
    {
	send_to_char( "OEdit:  Vnum in an area you cannot build in.\n\r", ch );
	return FALSE;
    }

    if ( get_obj_index( value ) )
    {
	send_to_char( "OEdit:  Object vnum already exists.\n\r", ch );
	return FALSE;
    }
        
    pObj			= new_obj_index();
    pObj->vnum			= value;
    pObj->area			= pArea;
        
    if ( value > top_vnum_obj )
	top_vnum_obj = value;

    iHash			= value % MAX_KEY_HASH;
    pObj->next			= obj_index_hash[iHash];
    obj_index_hash[iHash]	= pObj;
    ch->desc->pEdit		= (void *)pObj;

    send_to_char( "Object Created.\n\r", ch );
    return TRUE;
}

bool oedit_ed( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    EXTRA_DESCR_DATA *ed;
    char command[MAX_INPUT_LENGTH];
    char keyword[MAX_INPUT_LENGTH];

    EDIT_OBJ(ch, pObj);

    argument = one_argument( argument, command );
    one_argument( argument, keyword );

    if ( command[0] == '\0' )
    {
	send_to_char( "Syntax:  ed add [keyword]\n\r", ch );
	send_to_char( "         ed delete [keyword]\n\r", ch );
	send_to_char( "         ed edit [keyword]\n\r", ch );
	send_to_char( "         ed format [keyword]\n\r", ch );
	return FALSE;
    }

    if ( !str_cmp( command, "add" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "Syntax:  ed add [keyword]\n\r", ch );
	    return FALSE;
	}

	ed                  =   new_extra_descr();
	ed->keyword         =   str_dup( keyword );
    ed->buffer1 = str_dup("");
    ed->buffer2 = str_dup("");
    ed->type = ED_TYPE_NONE;
    ed->action = ED_ACTION_NONE;
    ed->vnum = 0;
	ed->next            =   pObj->extra_descr;
	pObj->extra_descr   =   ed;

	string_append( ch, &ed->description );

	return TRUE;
    }

    if ( !str_cmp( command, "edit" ) )
    {
	if ( keyword[0] == '\0' )
	{
	    send_to_char( "Syntax:  ed edit [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pObj->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	}

	if ( !ed )
	{
	    send_to_char( "OEdit:  Extra description keyword not found.\n\r", ch );
	    return FALSE;
	}

	string_append( ch, &ed->description );

	return TRUE;
    }

    if ( !str_cmp( command, "delete" ) )
    {
	EXTRA_DESCR_DATA *ped = NULL;

	if ( keyword[0] == '\0' )
	{
	    send_to_char( "Syntax:  ed delete [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pObj->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	    ped = ed;
	}

	if ( !ed )
	{
	    send_to_char( "OEdit:  Extra description keyword not found.\n\r", ch );
	    return FALSE;
	}

	if ( !ped )
	    pObj->extra_descr = ed->next;
	else
	    ped->next = ed->next;

	free_extra_descr( ed );

	send_to_char( "Extra description deleted.\n\r", ch );
	return TRUE;
    }

    if ( !str_cmp( command, "format" ) )
    {
	EXTRA_DESCR_DATA *ped = NULL;

	if ( keyword[0] == '\0' )
	{
	    send_to_char( "Syntax:  ed format [keyword]\n\r", ch );
	    return FALSE;
	}

	for ( ed = pObj->extra_descr; ed; ed = ed->next )
	{
	    if ( is_name( keyword, ed->keyword ) )
		break;
	    ped = ed;
	}

	if ( !ed )
	{
                send_to_char( "OEdit:  Extra description keyword not found.\n\r", ch );
                return FALSE;
	}

	/* OLC 1.1b */
	if ( strlen(ed->description) >= (MAX_STRING_LENGTH - 4) )
	{
	    send_to_char( "String too long to be formatted.\n\r", ch );
	    return FALSE;
	}

	ed->description = format_string( ed->description );

	send_to_char( "Extra description formatted.\n\r", ch );
	return TRUE;
    }

    oedit_ed( ch, "" );
    return FALSE;
}

/*
 * Mobile Editor Functions.
 */
bool medit_show( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    char buf[MAX_STRING_LENGTH];
    PROG_LIST *list;

    EDIT_MOB(ch, pMob);

    xprintf( buf, "#wName:        #0[#W%s#0]\n\r#wArea:        #0[%5d#0] #B%s#n\n\r",
	pMob->player_name,
	!pMob->area ? -1        : pMob->area->vnum,
	!pMob->area ? "No Area" : pMob->area->name );
    send_to_char( buf, ch );

    xprintf( buf, "#wAct:         #0[#W%s#0]\n\r",
	flag_string( act_flags, pMob->act ) );
    send_to_char( buf, ch );

    xprintf( buf, "#wVnum:        #0[#R%5d#0]\n\r#wSex:         #0[#W%s#0]#n\n\r",
	pMob->vnum,
	pMob->sex == SEX_MALE    ? "male"   :
	pMob->sex == SEX_FEMALE  ? "female" : "neutral" );
    send_to_char( buf, ch );

    xprintf( buf,
	"#wLevel:       #0[#W%2d#0]\n\r#wAlign:       #0[#W%4d#0]\n\r",
	pMob->level,       pMob->alignment );
    send_to_char( buf, ch );
    
    //xprintf( buf, "#wBones:       #0[#W%d#0]\n\r",
    //pMob->bones );
    //send_to_char( buf, ch );

    xprintf( buf, "#wAffected by: #0[#W%s#0]\n\r",
	flag_string( affect_flags, pMob->affected_by ) );
    send_to_char( buf, ch );

    xprintf( buf, "#wDefault pos: #0[#W%s#0]#n\n\r",
	position_table[pMob->default_pos].name );
    send_to_char( buf, ch );

    xprintf( buf, "#wv0: Toughness         #0[#W%4d#0]\n\r",pMob->toughness);
    stc( buf, ch);
    xprintf( buf, "#wv1: Damage Modifier   #0[#W%4d#0]\n\r",pMob->dam_modifier);
    stc(buf,ch);
    xprintf( buf, "#wv2: Extra Attacks     #0[#W%4d#0]\n\r",pMob->extra_attack);
    stc(buf,ch); 

  switch (pMob->natural_attack)
  {
    default:
      xprintf(buf, "#wAttack     : #0[#RHit#0]\n\r");
      break;
    case 1:
      xprintf(buf, "#wAttack     : #0[#RSlice#0]\n\r");
      break;
    case 2:
      xprintf(buf, "#wAttack     : #0[#RStab#0]\n\r");
      break;
    case 3:
      xprintf(buf, "#wAttack     : #0[#RSlash#0]\n\r");
      break;
    case 4:
      xprintf(buf, "#wAttack     : #0[#RWhip#0]\n\r");
      break;
    case 5:
      xprintf(buf, "#wAttack     : #0[#RClaw#0]\n\r");
      break;
    case 6:
      xprintf(buf, "#wAttack     : #0[#RBlast#0]\n\r");
      break;
    case 7:
      xprintf(buf, "#wAttack     : #0[#RPound#0]\n\r");
      break;
    case 8:
      xprintf(buf, "#wAttack     : #0[#RCrush#0]\n\r");
      break;
    case 9:
      xprintf(buf, "#wAttack     : #0[#RGrep#0]\n\r");
      break;
    case 10:
      xprintf(buf, "#wAttack     : #0[#RBite#0]\n\r");
      break;
    case 11:
      xprintf(buf, "#wAttack     : #0[#RPierce#0]\n\r");
      break;
    case 12:
      xprintf(buf, "#wAttack     : #0[#RSuck#0]\n\r");
      break;
  }
  send_to_char(buf, ch);

    if ( pMob->spec_fun )
    {
	xprintf( buf, "#wSpec fun:    #0[#W%s#0]\n\r",  spec_string( pMob->spec_fun ) );
	send_to_char( buf, ch );
    }
    if ( pMob->mprogs )
    {
	int cnt;

	xprintf(buf, "\n\r#wMOBPrograms for #0[#B%5d#0]#R:#n\n\r", pMob->vnum);
	send_to_char( buf, ch );

	for (cnt=0, list=pMob->mprogs; list; list=list->next)
	{
		if (cnt ==0)
		{
			send_to_char ( " #wNumber Vnum   Trigger Phrase\n\r", ch );
			send_to_char ( " #0------ -----  ------- ------#n\n\r", ch );
		}

		xprintf(buf, "#0[#W%5d#0] #R%5d  #B%7s #G%s#n\n\r", cnt,
			list->vnum,prog_type_to_name(list->trig_type),
			list->trig_phrase);
		send_to_char( buf, ch );
		cnt++;
	}
    }
    send_to_char ( "\n\r", ch );
    if (pMob->quest_fun)
  {
    xprintf(buf, "#wQuest fun:    #0[#W%s#0]\n\r", quest_string(pMob->quest_fun));
    send_to_char(buf, ch);
  }
if (pMob->shop_fun)
  {
    xprintf(buf, "#wShop fun:    #0[#W%s#0]\n\r", shop_string(pMob->shop_fun));
    send_to_char(buf, ch);
  }
    xprintf( buf, "#wShort descr: #n%s\n\r#wLong descr:\n\r#n%s",
	pMob->short_descr,
	pMob->long_descr );
    send_to_char( buf, ch );

    xprintf( buf, "#wDescription:\n\r#n%s", pMob->description );
    send_to_char( buf, ch );

    return FALSE;
}

bool medit_create( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    AREA_DATA *pArea;
    int  value;
    int  iHash;

    value = atoi( argument );

    /* OLC 1.1b */
    if ( argument[0] == '\0' || value <= 0 || value >= INT_MAX )
    {
	char output[MAX_STRING_LENGTH];

	xprintf( output, "Syntax:  create [0 < vnum < %d]\n\r", INT_MAX );
	send_to_char( output, ch );
	return FALSE;
    }

    pArea = get_vnum_area( value );

    if ( !pArea )
    {
	send_to_char( "MEdit:  That vnum is not assigned an area.\n\r", ch );
	return FALSE;
    }

    if ( !IS_BUILDER( ch, pArea ) )
    {
	send_to_char( "MEdit:  Vnum in an area you cannot build in.\n\r", ch );
	return FALSE;
    }

    if ( get_mob_index( value ) )
    {
	send_to_char( "MEdit:  Mobile vnum already exists.\n\r", ch );
	return FALSE;
    }

    pMob			= new_mob_index();
    pMob->vnum			= value;
    pMob->area			= pArea;
        
    if ( value > top_vnum_mob )
	top_vnum_mob = value;        

    pMob->act			= ACT_IS_NPC;
    iHash			= value % MAX_KEY_HASH;
    pMob->next			= mob_index_hash[iHash];
    mob_index_hash[iHash]	= pMob;
    ch->desc->pEdit		= (void *)pMob;

    send_to_char( "Mobile Created.\n\r", ch );
    return TRUE;
}

bool medit_position( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    char arg[MAX_INPUT_LENGTH];
    int value;

    argument = one_argument( argument, arg );

    switch ( arg[0] )
    {
    default:
	break;

    case 'D':
    case 'd':
	if ( str_prefix( arg, "default" ) )
	    break;

	if ( ( value = flag_value( position_flags, argument ) ) == NO_FLAG )
	    break;

	EDIT_MOB( ch, pMob );

	pMob->default_pos = value;
	send_to_char( "Default position set.\n\r", ch );
	return TRUE;
    }

    send_to_char( "Syntax:  position default [position]\n\r"
		  "Type '? position' for a list of positions.\n\r", ch );
    return FALSE;
}

bool medit_quest(CHAR_DATA * ch, char *argument)
{
  MOB_INDEX_DATA *pMob;

  EDIT_MOB(ch, pMob);

  if (argument[0] == '\0')
  {
    send_to_char("Syntax:  quest [quest function]\n\r", ch);
    send_to_char("        'quest none' to remove\n\r", ch);
    return FALSE;
  }
  if (!str_cmp(argument, "none"))
  {
    pMob->quest_fun = NULL;
    send_to_char("Quest removed.\n\r", ch);
    return TRUE;
  }
  if (quest_lookup(argument))
  {
    pMob->quest_fun = quest_lookup(argument);
    send_to_char("Quest set.\n\r", ch);
    return TRUE;
  }
  send_to_char("MEdit: No such quest function.\n\r", ch);
  return FALSE;
}

bool medit_shop(CHAR_DATA * ch, char *argument)
{
  MOB_INDEX_DATA *pMob;

  EDIT_MOB(ch, pMob);

  if (argument[0] == '\0')
  {
    send_to_char("Syntax:  shop [shop function]\n\r", ch);
    send_to_char("        'shop none' to remove\n\r", ch);
    return FALSE;
  }
  if (!str_cmp(argument, "none"))
  {
    pMob->shop_fun = NULL;
    send_to_char("Shop removed.\n\r", ch);
    return TRUE;
  }
  if (shop_lookup(argument))
  {
    pMob->shop_fun = shop_lookup(argument);
    send_to_char("Shop set.\n\r", ch);
    return TRUE;
  }
  send_to_char("MEdit: No such shop function.\n\r", ch);
  return FALSE;
}

bool medit_spec( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  spec [special function]\n\r", ch );
	return FALSE;
    }

    if ( !str_cmp( argument, "none" ) )
    {
        pMob->spec_fun = NULL;

        send_to_char( "Spec removed.\n\r", ch);
        return TRUE;
    }

    if ( spec_lookup( argument ) )
    {
	pMob->spec_fun = spec_lookup( argument );
	send_to_char( "Spec set.\n\r", ch);
	return TRUE;
    }

    send_to_char( "MEdit: No such special function.\n\r", ch );
    return FALSE;
}

bool medit_addmprog( CHAR_DATA *ch, char *argument )
{
  int value;
  MOB_INDEX_DATA *pMob;
  PROG_LIST *list;
  PROG_CODE *code;
  char trigger[MAX_STRING_LENGTH];
  char phrase[MAX_STRING_LENGTH];
  char num[MAX_STRING_LENGTH];

  EDIT_MOB(ch, pMob);
  argument=one_argument(argument, num);
  argument=one_argument(argument, trigger);
  argument=one_argument(argument, phrase);

  if (!is_number(num) || trigger[0] =='\0' || phrase[0] =='\0' )
  {
        send_to_char("Syntax:   addmprog [vnum] [trigger] [phrase]\n\r",ch);
        return FALSE;
  }

  if ( (value = flag_value (mprog_flags, trigger) ) == NO_FLAG)
  {
        send_to_char("Valid flags are:\n\r",ch);
        show_help( ch, "mprog");
        return FALSE;
  }

  if ( ( code =get_prog_index (atoi(num), PRG_MPROG ) ) == NULL)
  {
        send_to_char("No such MOBProgram.\n\r",ch);
        return FALSE;
  }

  list                  = new_mprog();
  list->vnum            = atoi(num);
  list->trig_type       = value;
  list->trig_phrase     = str_dup(phrase);
  list->code            = code->code;
  SET_BIT(pMob->mprog_flags,value);
  list->next            = pMob->mprogs;
  pMob->mprogs          = list;

  send_to_char( "Mprog Added.\n\r",ch);
  return TRUE;
}

bool medit_delete( CHAR_DATA *ch, char *argument )
{
     MOB_INDEX_DATA *pMob;
     MOB_INDEX_DATA *sMob;
     MOB_INDEX_DATA *iMob;
     RESET_DATA *pReset = NULL;
     RESET_DATA *prev= NULL;
     ROOM_INDEX_DATA *pRoom = NULL;
     char arg[MIL];
     char buf[MSL];
     int mindex, count, iHash, i;
     int dobj[100];
     bool foundmob = FALSE;
     bool exist = FALSE;
     bool foundobj = FALSE;

     if ( argument[0] == '\0' )
     {
          send_to_char( "Syntax:  medit delete [vnum]\n\r", ch );
          return FALSE;
     }

     one_argument( argument, arg );

     if( is_number( arg ) )
     {
          mindex = atoi( arg );
          pMob = get_mob_index( mindex );
     }
     else
     {
          send_to_char( "That is not a number.\n\r", ch );
          return FALSE;
     }

     if( !pMob )
     {
          send_to_char( "No such mobile.\n\r", ch );
          return FALSE;
     }

     SET_BIT( pMob->area->area_flags, AREA_CHANGED );

     /* Remove it from the object list */

     iHash = mindex % MAX_KEY_HASH;

     sMob = mob_index_hash[iHash];

     if( sMob->next == NULL ) /* only entry */
          mob_index_hash[iHash] = NULL;
     else if( sMob == pMob ) /* first entry */
          mob_index_hash[iHash] = pMob->next;
     else /* everything else */
     {
          for( iMob = sMob; iMob != NULL; iMob = iMob->next )
          {
               if( iMob ==  pMob )
               {
                    sMob->next = pMob->next;
                    break;
               }
          }
     }

     if( top_vnum_mob == mindex )
          for( i = 1; i < mindex; i++ )
               if( get_obj_index( i ) )
                    top_vnum_obj = i;

     top_mob_index--;

     /* Now crush all resets */
     count = 0;
     for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
     {
          for( pRoom = room_index_hash[iHash]; pRoom; pRoom = pRoom->next )
          {
               dobj[0] = -1;
               prev = pRoom->reset_first;
               for( pReset = pRoom->reset_first; pReset; pReset = pReset->next )
               {
                    switch( pReset->command )
                    {
                         case 'M':
                              if( pReset->arg1 == mindex )
                              {
                                   foundmob = TRUE;

                                   if( pRoom->reset_first == pReset )
                                   {
                                        pRoom->reset_first = pReset->next;
                                        if( !pRoom->reset_first )
                                             pRoom->reset_last = NULL;
                                   }
                                   else if( pRoom->reset_last == pReset )
                                   {
                                        pRoom->reset_last = prev;
                                        prev->next = NULL;
                                   }
                                   else
                                        prev->next = prev->next->next;

                                   count++;
                                   SET_BIT( pRoom->area->area_flags,AREA_CHANGED );
                              }
                              else
                                   foundmob = FALSE;

                              break;
                         case 'E':
                         case 'G':
                              if( foundmob )
                              {
                                   exist = FALSE;

                                   for( i = 0; dobj[i] != -1; i++ )
                                   {
                                        if( dobj[i] == pReset->arg1 )
                                        {
                                             exist = TRUE;
                                             break;
                                        }
                                   }

                                   if( !exist )
                                   {
                                        dobj[i] = pReset->arg1;
                                        dobj[i + 1] = -1;
                                   }

                                   if( pRoom->reset_first == pReset )
                                   {
                                        pRoom->reset_first = pReset->next;
                                        if( !pRoom->reset_first )
                                             pRoom->reset_last = NULL;
                                   }
                                   else if( pRoom->reset_last == pReset )
                                   {
                                        pRoom->reset_last = prev;
                                        prev->next = NULL;
                                   }
                                   else
                                        prev->next = prev->next->next;

                                   count++;
                                   SET_BIT( pRoom->area->area_flags,AREA_CHANGED );

                              }

                              break;
                         case 'P':
                              foundobj = FALSE;

                              for( i = 0; dobj[i] != -1; i++ )
                                   if( dobj[i] == pReset->arg3 )
                                        foundobj = TRUE;

                              if( foundobj )
                              {
                                   printf( "Removing: command: %c  vnum: %d\n",pReset->command, pReset->arg1 );

                                   if( pRoom->reset_first == pReset )
                                   {
                                        pRoom->reset_first = pReset->next;
                                        if( !pRoom->reset_first )
                                             pRoom->reset_last = NULL;
                                   }
                                   else if( pRoom->reset_last == pReset )
                                   {
                                        pRoom->reset_last = prev;
                                        prev->next = NULL;
                                   }
                                   else
                                        prev->next = prev->next->next;

                                   count++;
                                   SET_BIT( pRoom->area->area_flags,AREA_CHANGED );

                              }
                    }
                    prev = pReset;
               }
          }
     }

     //if( ch->mount == NULL )
      //    medit_delete_kludge = ch;

     xprintf( buf, "Removed mobile vnum ^C%d^x and ^C%d^x resets.\n\r", mindex,count );
     send_to_char( buf, ch );
     return TRUE;
}

bool medit_delmprog( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;
    PROG_LIST *list;
    PROG_LIST *list_next;
    char mprog[MAX_STRING_LENGTH];
    int value;
    int cnt = 0;

    EDIT_MOB(ch, pMob);

    one_argument( argument, mprog );
    if (!is_number( mprog ) || mprog[0] == '\0' )
    {
       send_to_char("Syntax:  delmprog [#mprog]\n\r",ch);
       return FALSE;
    }

    value = atoi ( mprog );

    if ( value < 0 )
    {
        send_to_char("Only non-negative mprog-numbers allowed.\n\r",ch);
        return FALSE;
    }

    if ( !(list= pMob->mprogs) )
    {
        send_to_char("MEdit:  Non existant mprog.\n\r",ch);
        return FALSE;
    }

    if ( value == 0 )
    {
	REMOVE_BIT(pMob->mprog_flags, pMob->mprogs->trig_type);
        list = pMob->mprogs;
        pMob->mprogs = list->next;
        free_mprog( list );
    }
    else
    {
        while ( (list_next = list->next) && (++cnt < value ) )
                list = list_next;

        if ( list_next )
        {
		REMOVE_BIT(pMob->mprog_flags, list_next->trig_type);
                list->next = list_next->next;
                free_mprog(list_next);
        }
        else
        {
                send_to_char("No such mprog.\n\r",ch);
                return FALSE;
        }
    }

    send_to_char("Mprog removed.\n\r", ch);
    return TRUE;
}

bool oedit_addoprog( CHAR_DATA *ch, char *argument )
{
  int value;
  OBJ_INDEX_DATA *pObj;
  PROG_LIST *list;
  PROG_CODE *code;
  char trigger[MAX_STRING_LENGTH];
  char phrase[MAX_STRING_LENGTH];
  char num[MAX_STRING_LENGTH];

  EDIT_OBJ(ch, pObj);
  argument=one_argument(argument, num);
  argument=one_argument(argument, trigger);
  argument=one_argument(argument, phrase);

  if (!is_number(num) || trigger[0] =='\0' || phrase[0] =='\0' )
  {
        send_to_char("Syntax:   addoprog [vnum] [trigger] [phrase]\n\r",ch);
        return FALSE;
  }

  if ( (value = flag_value (oprog_flags, trigger) ) == NO_FLAG)
  {
        send_to_char("Valid flags are:\n\r",ch);
        show_help( ch, "oprog");
        return FALSE;
  }

  if ( ( code =get_prog_index (atoi(num), PRG_OPROG ) ) == NULL)
  {
        send_to_char("No such OBJProgram.\n\r",ch);
        return FALSE;
  }

  list                  = new_oprog();
  list->vnum            = atoi(num);
  list->trig_type       = value;
  list->trig_phrase     = str_dup(phrase);
  list->code            = code->code;
  SET_BIT(pObj->oprog_flags,value);
  list->next            = pObj->oprogs;
  pObj->oprogs          = list;

  send_to_char( "Oprog Added.\n\r",ch);
  return TRUE;
}

bool oedit_deloprog( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj;
    PROG_LIST *list;
    PROG_LIST *list_next;
    char oprog[MAX_STRING_LENGTH];
    int value;
    int cnt = 0;

    EDIT_OBJ(ch, pObj);

    one_argument( argument, oprog );
    if (!is_number( oprog ) || oprog[0] == '\0' )
    {
       send_to_char("Syntax:  deloprog [#oprog]\n\r",ch);
       return FALSE;
    }

    value = atoi ( oprog );

    if ( value < 0 )
    {
        send_to_char("Only non-negative oprog-numbers allowed.\n\r",ch);
        return FALSE;
    }

    if ( !(list= pObj->oprogs) )
    {
        send_to_char("OEdit:  Non existant oprog.\n\r",ch);
        return FALSE;
    }

    if ( value == 0 )
    {
	REMOVE_BIT(pObj->oprog_flags, pObj->oprogs->trig_type);
        list = pObj->oprogs;
        pObj->oprogs = list->next;
        free_oprog( list );
    }
    else
    {
        while ( (list_next = list->next) && (++cnt < value ) )
                list = list_next;

        if ( list_next )
        {
		REMOVE_BIT(pObj->oprog_flags, list_next->trig_type);
                list->next = list_next->next;
                free_oprog(list_next);
        }
        else
        {
                send_to_char("No such oprog.\n\r",ch);
                return FALSE;
        }
    }

    send_to_char("Oprog removed.\n\r", ch);
    return TRUE;
}

bool redit_addrprog( CHAR_DATA *ch, char *argument )
{
  int value;
  ROOM_INDEX_DATA *pRoom;
  PROG_LIST *list;
  PROG_CODE *code;
  char trigger[MAX_STRING_LENGTH];
  char phrase[MAX_STRING_LENGTH];
  char num[MAX_STRING_LENGTH];

  EDIT_ROOM(ch, pRoom);
  argument=one_argument(argument, num);
  argument=one_argument(argument, trigger);
  argument=one_argument(argument, phrase);

  if (!is_number(num) || trigger[0] =='\0' || phrase[0] =='\0' )
  {
        send_to_char("Syntax:   addrprog [vnum] [trigger] [phrase]\n\r",ch);
        return FALSE;
  }

  if ( (value = flag_value (rprog_flags, trigger) ) == NO_FLAG)
  {
        send_to_char("Valid flags are:\n\r",ch);
        show_help( ch, "rprog");
        return FALSE;
  }

  if ( ( code =get_prog_index (atoi(num), PRG_RPROG ) ) == NULL)
  {
        send_to_char("No such ROOMProgram.\n\r",ch);
        return FALSE;
  }

  list                  = new_rprog();
  list->vnum            = atoi(num);
  list->trig_type       = value;
  list->trig_phrase     = str_dup(phrase);
  list->code            = code->code;
  SET_BIT(pRoom->rprog_flags,value);
  list->next            = pRoom->rprogs;
  pRoom->rprogs          = list;

  send_to_char( "Rprog Added.\n\r",ch);
  return TRUE;
}

bool redit_delrprog( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;
    PROG_LIST *list;
    PROG_LIST *list_next;
    char rprog[MAX_STRING_LENGTH];
    int value;
    int cnt = 0;

    EDIT_ROOM(ch, pRoom);

    one_argument( argument, rprog );
    if (!is_number( rprog ) || rprog[0] == '\0' )
    {
       send_to_char("Syntax:  delrprog [#rprog]\n\r",ch);
       return FALSE;
    }

    value = atoi ( rprog );

    if ( value < 0 )
    {
        send_to_char("Only non-negative rprog-numbers allowed.\n\r",ch);
        return FALSE;
    }

    if ( !(list= pRoom->rprogs) )
    {
        send_to_char("REdit:  Non existant rprog.\n\r",ch);
        return FALSE;
    }

    if ( value == 0 )
    {
	REMOVE_BIT(pRoom->rprog_flags, pRoom->rprogs->trig_type);
        list = pRoom->rprogs;
        pRoom->rprogs = list->next;
        free_rprog( list );
    }
    else
    {
        while ( (list_next = list->next) && (++cnt < value ) )
                list = list_next;

        if ( list_next )
        {
		REMOVE_BIT(pRoom->rprog_flags, list_next->trig_type);
                list->next = list_next->next;
                free_rprog(list_next);
        }
        else
        {
                send_to_char("No such rprog.\n\r",ch);
                return FALSE;
        }
    }

    send_to_char("Rprog removed.\n\r", ch);
    return TRUE;
}


bool medit_align( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  alignment [number]\n\r", ch );
	return FALSE;
    }

    pMob->alignment = atoi( argument );

    send_to_char( "Alignment set.\n\r", ch);
    return TRUE;
}

bool medit_weapon(CHAR_DATA * ch, char *argument)
{
  MOB_INDEX_DATA *pMob;

  EDIT_MOB(ch, pMob);

  if (argument[0] == '\0')
  {
    send_to_char("Syntax:  weapon [type]\n\r", ch);
    return FALSE;
  }
  if (!str_cmp(argument, "slice"))
    pMob->natural_attack = 1;
  else if (!str_cmp(argument, "stab"))
    pMob->natural_attack = 2;
  else if (!str_cmp(argument, "slash"))
    pMob->natural_attack = 3;
  else if (!str_cmp(argument, "whip"))
    pMob->natural_attack = 4;
  else if (!str_cmp(argument, "claw"))
    pMob->natural_attack = 5;
  else if (!str_cmp(argument, "blast"))
    pMob->natural_attack = 6;
  else if (!str_cmp(argument, "pound"))
    pMob->natural_attack = 7;
  else if (!str_cmp(argument, "crush"))
    pMob->natural_attack = 8;
  else if (!str_cmp(argument, "grep"))
    pMob->natural_attack = 9;
  else if (!str_cmp(argument, "bite"))
    pMob->natural_attack = 10;
  else if (!str_cmp(argument, "pierce"))
    pMob->natural_attack = 11;
  else if (!str_cmp(argument, "suck"))
    pMob->natural_attack = 12;
  else
  {
    send_to_char("No such attack styl.\n\r", ch);
    return FALSE;
  }
  send_to_char("Attack set.\n\r", ch);
  return TRUE;
}

bool medit_value0( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  v0 [number]\n\r", ch );
	return FALSE;
    }

    if( atoi(argument) < -100 || atoi(argument) > 100)
    {
	stc( "Range is from -100 to 100.\n\r",ch);
	return FALSE;
    }

  pMob->toughness = atoi(argument);

    send_to_char( "v0 set.\n\r", ch);
    return TRUE;
}

bool medit_value1( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  v1 [number]\n\r", ch );
	return FALSE;
    }

    if( atoi(argument) < 1 || atoi(argument) > 3)
    {
	stc( "Range is from 1 to 3.\n\r",ch);
	return FALSE;
    }

  pMob->dam_modifier = atoi(argument);

    send_to_char( "v1 set.\n\r", ch);
    return TRUE;
}

bool medit_value2( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax: v2 [number]\n\r", ch );
	return FALSE;
    }

    if( atoi(argument) < 1 || atoi(argument) > 20)
    {
	stc( "Range is from 1 to 20.\n\r",ch);
	return FALSE;
    }

  pMob->extra_attack = atoi(argument);

    send_to_char( "v2 set.\n\r", ch);
    return TRUE;
}


bool medit_level( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "Syntax:  level [number]\n\r", ch );
	return FALSE;
    }
  
    pMob->level = atoi( argument );
    
    if(pMob->level > 4000)
    {
    	stc("4000 is max level\n\r", ch);
        pMob->level = 4000;
    }

    send_to_char( "Level set.\n\r", ch);
    return TRUE;
}
/*
bool medit_bones( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "Syntax:  bones [number]\n\r", ch );
	return FALSE;
    }

    pMob->bones = atoi( argument );
    
    if(pMob->bones > 5000)
    {
    	stc("5000 is max bones\n\r", ch);
        pMob->bones = 5000;
    }

    send_to_char( "Bones set.\n\r", ch);
    return TRUE;
}
*/
bool medit_desc( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	string_append( ch, &pMob->description );
	return TRUE;
    }

    send_to_char( "Syntax:  desc    - line edit\n\r", ch );
    return FALSE;
}

bool medit_long( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  long [string]\n\r", ch );
	return FALSE;
    }

    free_string( pMob->long_descr );
    strcat( argument, "\n\r" );
    pMob->long_descr = str_dup( argument );
    pMob->long_descr[0] = UPPER( pMob->long_descr[0]  );

    send_to_char( "Long description set.\n\r", ch);
    return TRUE;
}

bool medit_short( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  short [string]\n\r", ch );
	return FALSE;
    }

    free_string( pMob->short_descr );
    pMob->short_descr = str_dup( argument );

    send_to_char( "Short description set.\n\r", ch);
    return TRUE;
}

bool medit_name( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob;

    EDIT_MOB(ch, pMob);

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  name [string]\n\r", ch );
	return FALSE;
    }

    free_string( pMob->player_name );
    pMob->player_name = str_dup( argument );

    send_to_char( "Name set.\n\r", ch);
    return TRUE;
}

/*help editor by blade */
HEDIT( hedit_level )
{
    HELP_DATA *pHelp;

    EDIT_HELP(ch, pHelp);

    if ( argument[0] == '\0' || !is_number( argument ) )
    {
	send_to_char( "Syntax:  level [number]\n\r", ch );
	return FALSE;
    }

    pHelp->level = atoi( argument );

    send_to_char( "Level set.\n\r", ch);
    return TRUE;
}
/* reset keyword */
HEDIT( hedit_keyword )
{
    HELP_DATA *pHelp;
	HELP_DATA *tHelp;

    EDIT_HELP(ch, pHelp);

  if (!is_number(argument))
  {
    for (tHelp = first_help; tHelp != NULL; tHelp = tHelp->next)
    {
	if (is_name(argument, tHelp->keyword))
      {
        send_to_char("That keyword already exits.\n\r", ch);
        return FALSE;
      }
    }
pHelp->keyword = str_dup(argument);
	return TRUE;
    }
	send_to_char("Syntax: keyword [word(s)]\n\r", ch);
    return FALSE;
}

/* mod current help text */
HEDIT( hedit_text )
{
	HELP_DATA *pHelp;
	
	EDIT_HELP(ch, pHelp);

	if ( argument[0] == '\0')
    {
    string_append( ch, &pHelp->text );	
	return TRUE;
    }
  else
  {
	send_to_char("Syntax: text [no arguments!]\n\r", ch);
	return FALSE;
}
}

/* kill a help */
HEDIT( hedit_delete )
{		
//	extern int top_help;
	HELP_DATA *target;
  HELP_DATA *previous = NULL;
	
if (argument[0] == '\0')
	{
    send_to_char("Syntax: Hedit delete 'keyword'\n\r", ch);
		return FALSE;
	}

  for (target = first_help; target != NULL; target = target->next)
{
 if (is_name(argument, target->keyword))
 {
      if (target == first_help)
	 {
		 previous = target->next;
        first_help = previous;
		 top_help--;
        send_to_char("Help removed.\n\r", ch);
		 return TRUE;
	 }
      else
	 {
  previous->next = target->next; 
  top_help--;
  send_to_char("#rHelp removed.#n\n\r", ch);
  return TRUE;
	 }
 }
 previous = target; /* set previous to last target */
}
  send_to_char("No Help with that keyword found to delete!\n\r", ch);
return FALSE;
}

/* mod an existing help - throw into text editor */
HEDIT( hedit_change )
{
HELP_DATA *tHelp;

if (argument[0] == '\0')
	{
	send_to_char(" Syntax: Hedit change 'keyword'\n\r", ch);
		return FALSE;
	}
  for (tHelp = first_help; tHelp != NULL; tHelp = tHelp->next)
{
	if (is_name(argument, tHelp->keyword))
    {
      send_to_char(" Help found, Entering String editor\n\r", ch);
	ch->desc->pEdit		= (void *)tHelp;
 send_to_char( tHelp->keyword, ch);
 send_to_char("\n\r", ch);

string_append( ch, &tHelp->text );
return TRUE;
	}
}
  send_to_char("No Help by that keyword available,\n\r  Try 'hedit index' for a listing of current helps.\n\r Remember, keywords must match exactly!\n\r", ch);
	return FALSE;
}

/* list all helps by keyword */
HEDIT( hedit_index)
{
	HELP_DATA *pHelp;
	int level;
	char output[MAX_STRING_LENGTH];
        char buf[MAX_STRING_LENGTH];
        int bob = 0;
	buf[0] = '\0';

  xprintf(output, "Help By Index.\n\r");
  for (pHelp = first_help; pHelp; pHelp = pHelp->next)
    {
        level = (pHelp->level < 0) ? -1 * pHelp->level - 1 : pHelp->level;
 
        if (level > get_trust( ch ) )
            continue;
        xprintf(buf,"%-17.16s  ", pHelp->keyword);
        strcat(output, buf);
                if (++bob % 3 == 0)  /* column check */
                strcat(output, "\n\r");
        }
        send_to_char(output,ch);
        send_to_char("\n\r", ch);
        return TRUE;
}

/* create a new help */
HEDIT( hedit_create )
{
//	extern int top_help;
	HELP_DATA *pHelp;
	HELP_DATA *tHelp;	

if (argument[0] == '\0')
	{
    send_to_char("Syntax: Hedit create 'keyword'\n\r", ch);
		return FALSE;
	}
  for (tHelp = first_help; tHelp != NULL; tHelp = tHelp->next)
{
	if (is_name(argument, tHelp->keyword))
    {
      send_to_char("That Help already exits.\n\r", ch);
      return FALSE;
    }
}
/* make the help in memory and initialize */
pHelp = new_help();
pHelp->level = 0;
pHelp->keyword = str_dup(argument);
  pHelp->text = str_dup("");

  pHelp->next = first_help;
  first_help = pHelp;
	top_help++;
    
 ch->desc->pEdit		= (void *)pHelp;
 send_to_char( pHelp->keyword, ch);
  send_to_char("\n\r", ch);

string_append( ch, &pHelp->text );
	return TRUE;
}

