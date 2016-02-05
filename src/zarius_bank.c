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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

void do_balance ( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    xprintf( buf, "#wYou have #Y%d#w #0bones #win between your #pcheeks#n.\n\r", ch->bones );
    send_to_char( buf, ch );
    xprintf( buf, "#wYou have #Y%d#w #0bones #win the bank.#n\n\r", ch->pcdata->bank );
    send_to_char( buf, ch );
    return;
}
void do_membership( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int cost = 0;
  argument = one_argument(argument, arg);

  if (IS_NPC(ch))
    return;

  if (str_cmp(arg, "upgrade"))
  {
    if (ch->pcdata->membership <= 0)
    {
      send_to_char("You have a basic membership.\n\r",ch);
      cost = 100000;
    }
    else if (ch->pcdata->membership == 1)
    {
      send_to_char("You have a bronze membership.\n\r",ch);
      cost = 250000;
    }
    else if (ch->pcdata->membership == 2)
    {
      send_to_char("You have a silver membership.\n\r",ch);
      cost = 500000;
    }
    else if (ch->pcdata->membership == 3)
    {
      send_to_char("You have a gold membership.\n\r",ch);
      cost = 2500000;
    }
    else if (ch->pcdata->membership == 4)
    {
      send_to_char("You have a platinum membership.\n\r",ch);
      cost = 0;
    }
    else
    {
      send_to_char("Your membership is bugged, report to an imm.\n\r",ch);
      return;
    }
    if (cost > 0)
    {
      xprintf(buf, "It will cost you %d bones to upgrade your membership.\n\r", cost);
      send_to_char(buf,ch);
    }
    return;
  }
  else
  {
    if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))  
    {
	xprintf( buf, "You can't only do that at a bank.\n\r" );
	send_to_char( buf, ch );
	return;
    }
    if (ch->pcdata->membership == 4)
    {
      send_to_char("You're already platinum baby.\n\r",ch);
      return;
    }
    if (ch->pcdata->membership == 0)
      cost = 100000;
    else if (ch->pcdata->membership == 1)
      cost = 250000;
    else if (ch->pcdata->membership == 2)
      cost = 500000;
    else if (ch->pcdata->membership == 3)
      cost = 2500000;
    else cost = 2000000000;
    if ( cost > ch->bones + ch->pcdata->bank )
    {
      send_to_char("You don't have enough bones to upgrade your membership.\n\r",ch);
      return;
    }

    ch->pcdata->membership += 1;
    send_to_char("Your membership has been upgraded!\n\r",ch);
    cost -= ch->bones;

    if (cost == 0)
	{
	  ch->bones = 0;
      return;
    }
    if (cost < 0)
    {
      ch->bones = (cost * -1);
      return;
    }    
    ch->pcdata->bank -= cost;
    return;
  }
  return;
}
void do_deposit ( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int amnt, allowed;

    if (IS_NPC(ch))
	{
	return;
	}

    if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))  
    {
	xprintf( buf, "But you are not in a bank.\n\r" );
	send_to_char( buf, ch );
	return;
    }

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	xprintf( buf, "How many bones do you wish to deposit?\n\r" );
	send_to_char( buf, ch );
	return;
    }
    
    if (!str_cmp(arg,"all"))
    {
	amnt = ch->bones;
    }
    else
    {
        amnt = atoi( arg );
    }
    
    if ( amnt < 1 )
    {
        send_to_char("Put a REAL amount in\n\r", ch);
        return;
    }    

    if ( amnt > ch->bones )
    {
	xprintf( buf, "#R%s#w, you do not have #Y%d#w bones.\n\r", ch->name, amnt );
	send_to_char( buf, ch );
	return;
    }
    
    switch(ch->pcdata->membership)
    {
      case 1: allowed = 250000;
      break;
      case 2: allowed = 500000;
      break;
      case 3: allowed = 1000000;
      break;
      case 4: allowed = 5000000;
      break;
      default: allowed = 150000;
      break;
    }
    
    if(ch->pcdata->bank >= allowed)
    {
        xprintf(buf, "Your bank account is full at %d, place them back between your cheeks.\n\r", allowed );
        send_to_char(buf, ch);
        return;	
    }
    
    if((ch->pcdata->bank + amnt) > allowed)
    {
    	amnt = allowed - ch->pcdata->bank;
        xprintf(buf, "You can only hold %d bones in your account, so you only deposit %d.\n\r", allowed, amnt );
        send_to_char(buf, ch);
    	//return;
    }
    ch->pcdata->bank += amnt;
    ch->bones -= amnt;
    xprintf( buf, "#R%s#w, your account now contains: #Y%d#w bones, after depositing #Y%d#w bones.\n\r", ch->name, ch->pcdata->bank, amnt );
    send_to_char(buf, ch );
    return;
}

void do_withdraw ( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int amnt, allowed;

    if (IS_NPC(ch))
	{
	return;
	}
    if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))  
    {
	xprintf( buf, "But you are not in a bank.\n\r" );
	send_to_char( buf, ch );
	return;
  }
  
    argument = one_argument(argument, arg);

    if ( arg[0] == '\0' )
    {
	 xprintf( buf, "How many bones do you wish to withdraw?\n\r" );
	 send_to_char( buf, ch );
	 return;
    }
    
    if (!str_cmp(arg,"all"))
    {
	amnt = ch->pcdata->bank;
    }
    else
    {
        amnt = atoi( arg );
    }
    
     switch(ch->pcdata->membership)
    {
      case 1: allowed = 250000;
      break;
      case 2: allowed = 500000;
      break;
      case 3: allowed = 1000000;
      break;
      case 4: allowed = 5000000;
      break;
      default: allowed = 150000;
      break;
    }
    
    if ( amnt < 1 || amnt > allowed )
    {
        send_to_char("Nice try, Put a REAL amount in\n\r", ch);
        return;
    }   
    
    if ( amnt >= (ch->pcdata->bank + 1) )
    {
	xprintf( buf, "#R%s#w, you do not have #Y%d#w bones in the bank.\n\r", ch->name, amnt );
	send_to_char(buf, ch );
	return;
    }

    ch->bones += amnt;
    ch->pcdata->bank -= amnt;
    xprintf( buf, "#R%s#w, your account now contains: #Y%d#w bones, after withdrawing #Y%d#w bones.\n\r", ch->name, ch->pcdata->bank, amnt );
    send_to_char( buf, ch );
    return;
}


