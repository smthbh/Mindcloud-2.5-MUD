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
#include <time.h>
#include "merc.h"


void do_logstat(CHAR_DATA * ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  int ratio, mspusers, a, b, c;

  if (IS_NPC(ch)) return;

  xprintf(buf, "Total Players Created                 : %d players\n\r", top_playerid);
  send_to_char(buf, ch);
  a = mudinfo[MUDINFO_UPDATED] / 120;
  b = a / 24;
  c = a - b * 24;
  xprintf(buf, "Mudinfo : Days running                : %d days and %d hours\n\r", b, c);
  send_to_char(buf, ch);
  xprintf(buf, "Players : Average Online              : %d\n\r", (mudinfo[MUDINFO_MCCP_USERS] + mudinfo[MUDINFO_OTHER_USERS]) / mudinfo[MUDINFO_UPDATED]);
  send_to_char(buf, ch);
  xprintf(buf, "Players : Peak Online                 : %d\n\r", mudinfo[MUDINFO_PEAK_USERS]);
  send_to_char(buf, ch);
  if (mudinfo[MUDINFO_MCCP_USERS] + mudinfo[MUDINFO_OTHER_USERS] == 0) ratio = 0;
  else ratio = (100 * mudinfo[MUDINFO_MCCP_USERS] /
    (mudinfo[MUDINFO_MCCP_USERS] + mudinfo[MUDINFO_OTHER_USERS]));
  xprintf(buf, "Players : Mccp Users                  : %d %%\n\r", ratio);
  send_to_char(buf, ch);
  if (mudinfo[MUDINFO_MCCP_USERS] + mudinfo[MUDINFO_OTHER_USERS] == 0) mspusers = 0;
  else mspusers = (100 * mudinfo[MUDINFO_MSP_USERS] /
    (mudinfo[MUDINFO_MCCP_USERS] + mudinfo[MUDINFO_OTHER_USERS]));
  xprintf(buf, "Players : MSP Users                   : %d %%\n\r", mspusers);
  send_to_char(buf, ch);

  /*
   * a : Amount of kbit data send totally.
   * b : Amount of kbit data send each 10 seconds
   * c : The fractional part
   */
  a = mudinfo[MUDINFO_MBYTE] * 1024 + mudinfo[MUDINFO_BYTE] / 1024;
  b = a / (mudinfo[MUDINFO_UPDATED] * 3);
  c = b / 10;
  c = c * 10;
  c = b - c;
  xprintf(buf, "Datatransfer : Average Rate           : %d.%d kb/sec\n\r", b / 10, c);
  send_to_char(buf, ch);
  b = mudinfo[MUDINFO_DATA_PEAK] / (3 * 1024);
  c = b / 10;
  c = c * 10;
  c = b - c;
  xprintf(buf, "Datatransfer : Peak Rate              : %d.%d kb/sec\n\r", b / 10, c);
  send_to_char(buf, ch);
  xprintf(buf, "Datatransfer : This Week              : %d MB\n\r", mudinfo[MUDINFO_MBYTE]);
  send_to_char(buf, ch);
  xprintf(buf, "Datatransfer : This Boot              : %d MB\n\r", mudinfo[MUDINFO_MBYTE_S]);
  send_to_char(buf, ch);
  return;
}

void do_noset(CHAR_DATA * ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  one_argument(argument, arg);

  if (IS_NPC(ch)) return;

  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("No such player.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r", ch);
    return;
  }
  if (IS_SET(victim->pcdata->jflags, JFLAG_NOSET))
  {
    REMOVE_BIT(victim->pcdata->jflags, JFLAG_NOSET);
    send_to_char("Noset removed.\n\r", ch);
    send_to_char("You can set your messages again.\n\r", victim);
    return;
  }

  free_string(victim->pcdata->loginmessage);
  victim->pcdata->loginmessage = str_dup("");
  free_string(victim->pcdata->logoutmessage);
  victim->pcdata->logoutmessage = str_dup("");
  free_string(victim->pcdata->decapmessage);
  victim->pcdata->decapmessage = str_dup("");
  free_string(victim->pcdata->tiemessage);
  victim->pcdata->tiemessage = str_dup("");
  free_string(victim->pcdata->avatarmessage);
  victim->pcdata->avatarmessage = str_dup("");
  REMOVE_BIT(victim->pcdata->jflags, JFLAG_SETTIE);
  REMOVE_BIT(victim->pcdata->jflags, JFLAG_SETAVATAR);
  REMOVE_BIT(victim->pcdata->jflags, JFLAG_SETLOGIN);
  REMOVE_BIT(victim->pcdata->jflags, JFLAG_SETLOGOUT);
  REMOVE_BIT(victim->pcdata->jflags, JFLAG_SETDECAP);
  SET_BIT(victim->pcdata->jflags, JFLAG_NOSET);
  send_to_char("Ok.\n\r", ch);
  send_to_char("All your custom messages have been removed, and you can no longer change your messages.\n\r", victim);
  return;
}


/* Don't play around with this, to much can cause problems */
       
void do_specfind(CHAR_DATA *ch, char *argument)
{
  int value,i;
  ROOM_INDEX_DATA *pRoomIndex;
  char buf[MAX_STRING_LENGTH];

  if (!str_cmp(ch->name, "jobo"))
  {
    send_to_char("This command can crash the mud if used with the wrong argument, don't use it!\n\r", ch);
    return;
  }

  value = atoi(argument);
  for (i=1; i < 200000; i++)
  {
    pRoomIndex = get_room_index( i );
    if ( pRoomIndex != NULL )
    {
      if (IS_SET(pRoomIndex->room_flags, value))
      {
        xprintf(buf,"Room (%d) has the flag set\n\r",i);
        send_to_char(buf,ch);
      }
    }  
  }
  return;
}

void do_itemwearfind(CHAR_DATA *ch, char *argument)
{
  int value,i;
  OBJ_INDEX_DATA *iindex;
  char buf[MAX_STRING_LENGTH];

  if (!str_cmp(ch->name, "jobo"))
  {
    send_to_char("This command can crash the mud if used with the wrong argument, don't use it!\n\r", ch);
    return;
  }

  value = atoi(argument);
  for (i=1; i < 200000; i++)
  {
    iindex = get_obj_index( i );
    if (iindex != NULL)
    {
      if (IS_SET(iindex->wear_flags, value))
      {
        xprintf(buf,"Object (%d) has the flag set\n\r",i);
        send_to_char(buf,ch);
      }
    }
  }
  return;
}


void do_multicheck(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  DESCRIPTOR_DATA *d2;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;
  
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING) continue;
    for ( d2 = d->next; d2 != NULL; d2 = d2->next )
    {
      if (d2->lookup_status != STATUS_DONE) continue;
      if (!str_cmp(d->host, d2->host))
      {
        if (d2->connected != CON_PLAYING) continue;
        if (d2->character == NULL || d->character == NULL) continue;
        found = TRUE;
        xprintf(buf, "%s and %s are multiplaying bitches\n\r",d2->character->name, d->character->name);
        send_to_char(buf,ch);
      }
    }  
  }    
  if (!found) send_to_char("Noone is multiplaying atm.\n\r",ch);
  return;
}

void do_slay2(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  
  one_argument(argument, arg);
  
  if (IS_NPC(ch)) return;
  if (ch->level < 12) return;
  if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("Garotte whom?\n\r", ch );
    return;
  }
  if (victim == ch)
  {
    send_to_char("That would be a bad idea.\n\r", ch );
    return;
  }
  if (is_safe(ch, victim)) return;
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  one_hit(ch,victim, gsn_supreme,1);
  return;
}

void do_openthearena( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (arena_open) return;
  arena_open = TRUE;
  return;
}

void do_showsilence( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA *d;
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;
  
  if (IS_NPC(ch)) return; 
  
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->connected != CON_PLAYING) continue;
    if ( d->character != NULL ) gch = d->character;
    else continue;
    if (gch->level > 6) continue;
    if (IS_SET(gch->act, PLR_SILENCE))
    {
      found = TRUE;
      xprintf(buf,"%-15s is silenced\n\r",gch->name);
      send_to_char(buf,ch);
    }
  }
  if (!found) send_to_char("Noone is silenced.\n\r",ch);
  return;
}

void do_showsounds(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;

  if (IS_NPC(ch)) return;

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->connected != CON_PLAYING) continue;
    if ( d->character != NULL ) gch = d->character;
    else continue;
    if (gch->level > 6) continue;
    if (IS_SET(gch->act, PLR_SOUND))
    {
      found = TRUE;
      xprintf(buf,"%-15s uses sounds\n\r",gch->name);
      send_to_char(buf,ch);
    }
  }
  if (!found) send_to_char("Noone uses sounds.\n\r",ch);
  return;
}

void do_asperson(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  DESCRIPTOR_DATA *tmp;
  char arg[MAX_INPUT_LENGTH];
  bool afk = FALSE;

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch );
    return;
  }
  if (victim == ch)
  {
    send_to_char("That would be a bad idea.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  tmp = victim->desc;
  victim->desc = ch->desc;
  if (IS_SET(victim->extra, EXTRA_AFK))
  {
    afk = TRUE;
    REMOVE_BIT(victim->extra, EXTRA_AFK);
  }
  interpret(victim, argument);
  victim->desc = tmp;
  if (afk) SET_BIT(victim->extra, EXTRA_AFK);
  return;
}

void do_faith(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];

  for (d = descriptor_list ;d ; d = d->next)
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING) continue;
    if ((gch = d->character) != NULL)
    {
      if (gch->pcdata->faith < 1) continue;
      xprintf(buf,"%-14s - %3d/%-3d faithpoints\n\r", gch->name, gch->pcdata->current_faith, gch->pcdata->faith);
      send_to_char(buf,ch);
    }
  }
  return;
}

void do_bully(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
 
  argument = one_argument(argument, arg);
  
  if (IS_NPC(ch)) return;
  if (ch->level < MAX_LEVEL) return;
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch );
    return;
  }
  if (victim == ch)
  {
    send_to_char("That would be a bad idea.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (IS_SET(victim->pcdata->jflags, JFLAG_BULLY))
  {
    REMOVE_BIT(victim->pcdata->jflags, JFLAG_BULLY);
    send_to_char("You are to nice.\n\r",ch);
  }
  else
  {
    SET_BIT(victim->pcdata->jflags, JFLAG_BULLY);
    send_to_char("Take that bully.\n\r",ch);
  }
  return;
}

void do_offline(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *dummy;
  DESCRIPTOR_DATA *temp;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (ch->level < MAX_LEVEL)
  {
    send_to_char("This action is restricted.\n\r", ch);
    return;
  }
  if (strlen(arg) < 3)
  {
    send_to_char("What player do you wish to load?\n\r", ch);
    return;
  }
  if (argument[0] == '\0')
  {
    send_to_char("What do you want the player to do?\n\r", ch);
    return;
  }
  arg[0] = UPPER(arg[0]);

  if (descriptor_free == NULL )
  {
    dummy = alloc_perm(sizeof(*dummy));
  }
  else
  {
    dummy = descriptor_free;
    descriptor_free = descriptor_free->next;
  }
  if (load_char_obj(dummy, arg))
  {
    victim = dummy->character;
    temp = victim->desc;
    victim->desc = ch->desc;
    interpret(victim, argument);
    victim->desc = temp;
  }
  else
  {
    send_to_char("Player doesn't exist.\n\r", ch);
    dummy->next     = descriptor_free;
    descriptor_free = dummy;
    return;
  }

  /*
   * Clean up.
   */
  save_char_obj(victim);
  free_char(victim);
  dummy->next     = descriptor_free;
  descriptor_free = dummy;

  return;
}

void do_newban(CHAR_DATA * ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  BAN_DATA *pban;

  if (IS_NPC(ch))
    return;

  argument = one_argument(argument, arg);

  if (arg[0] == '\0')
  {
    strcpy(buf, "Newbie Banned sites:\n\r");
    for (pban = newbieban_list; pban != NULL; pban = pban->next)
    {
      strcat(buf, pban->name);
      strcat(buf, "    (");
      strcat(buf, pban->reason);
      strcat(buf, ")\n\r");
    }
    send_to_char(buf, ch);
    return;
  }
  for (pban = newbieban_list; pban != NULL; pban = pban->next)
  {
    if (!str_cmp(arg, pban->name))
    {
      send_to_char("That site is already banned!\n\r", ch);
      return;
    }
  }
  if (newbieban_free == NULL)
  {
    pban = alloc_perm(sizeof(*pban));
  }
  else
  {
    pban = newbieban_free;
    newbieban_free = newbieban_free->next;
  }
  pban->name = str_dup(arg);
  if (argument[0] == '\0')
    pban->reason = str_dup("no reason given");
  else
    pban->reason = str_dup(argument);
  pban->next = newbieban_list;
  newbieban_list = pban;
  send_to_char("Ok.\n\r", ch);
  save_newbiebans();
  return;
}

void do_newallow(CHAR_DATA * ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  BAN_DATA *prev;
  BAN_DATA *curr;

  one_argument(argument, arg);

  if (arg[0] == '\0')
  {
    send_to_char("Remove which site from the ban list?\n\r", ch);
    return;
  }
  prev = NULL;
  for (curr = newbieban_list; curr != NULL; prev = curr, curr = curr->next)
  {
    if (!str_cmp(arg, curr->name))
    {
      if (prev == NULL)
        newbieban_list = newbieban_list->next;
      else
        prev->next = curr->next;
      free_string(curr->name);
      free_string(curr->reason);
      curr->next = newbieban_free;
      newbieban_free = curr;
      send_to_char("Ok.\n\r", ch);
      save_newbiebans();
      return;
    }
  }
  send_to_char("Site is not banned.\n\r", ch);
  return;
}

void do_logstatclear(CHAR_DATA * ch, char *argument)
{
  int i;

  if (IS_NPC(ch))
    return;

  for (i = 0; i < MUDINFO_MAX; i++)
  {
    mudinfo[i] = 0;
  }

  update_mudinfo();
  send_to_char("Cleared.\n\r", ch);
  return;
}

void do_topclear( CHAR_DATA *ch, char *argument )
{
  int i;
  
  if (IS_NPC(ch)) return;
  if (ch->level < MAX_LEVEL) return;
  for (i = 1; i <= MAX_TOP_PLAYERS; i++)
  {
    top_board[i].pkscore = 0;
    free_string(top_board[i].name);
    top_board[i].name = str_dup("Noone");
  }
  save_topboard();
  send_to_char("Top cleared.\n\r",ch);
  return;
}

void do_leaderclear(CHAR_DATA * ch, char *argument)
{
  if (IS_NPC(ch))
    return;
  if (ch->level < 7)
    return;
  free_string(leader_board.pkscore_name);
  leader_board.pkscore_name = str_dup("Noone");
  leader_board.pkscore_number = 0;
  free_string(leader_board.pk_name);
  leader_board.pk_name = str_dup("Noone");
  leader_board.pk_number = 0;
  free_string(leader_board.mobkills_name);
  leader_board.mobkills_name = str_dup("Noone");
  leader_board.mobkills_number = 0;
  free_string(leader_board.quest_name);
  leader_board.quest_name = str_dup("Noone");
  leader_board.quest_number = 0;
  free_string(leader_board.time_name);
  leader_board.time_name = str_dup("Noone");
  leader_board.time_number = 0;
  free_string(leader_board.arena_name);
  leader_board.arena_name = str_dup("Noone");
  leader_board.arena_number = 0;
  save_leaderboard();
  send_to_char("Leader board cleared.\n\r", ch);
  return;
}

void do_ccenter(CHAR_DATA * ch, char *argument)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  argument = one_argument(argument, arg1);
  one_argument(argument, arg2);

  if(atoi(arg2) < 0)
  {
  	send_to_char( "Oh come on, what are you trying to do\n\r", ch );
	return;
  }

  if (arg1[0] == '\0')
  {
    xprintf(buf, "  #0[#G***#0]   #Y%s Control Center   #0[#G***#0]#n\n\r\n\r", MUDNAME);
    send_to_char(buf, ch);
    xprintf(buf, "  #R(#0lexp#R) #YMinimum Experience        #G%7d#n\n\r", ccenter[CCENTER_MIN_EXP]);
    send_to_char(buf, ch);
    xprintf(buf, "  #R(#0uexp#R) #YMaximum Experience        #G%7d#n\n\r", ccenter[CCENTER_MAX_EXP]);
    send_to_char(buf, ch);
    xprintf(buf, "  #R(#0elvl#R) #YExperience Level              #G%3d#n\n\r", ccenter[CCENTER_EXP_LEVEL]);
    send_to_char(buf, ch);
    xprintf(buf, "  #R(#0qlvl#R) #YQuestpoint Level              #G%3d#n\n\r", ccenter[CCENTER_QPS_LEVEL]);
    send_to_char(buf, ch);
    xprintf(buf, "  #R(#0blvl#R) #YBones Multiplier              #G%3d#n\n\r", ccenter[CCENTER_BONES_LEVEL]);
    send_to_char(buf, ch);
    xprintf(buf, "  #R(#0artm#R) #YLoad artifact on #G%3d#n #Ymob kills\n\r", ccenter[CCENTER_ARTI_MKILLS]);
    send_to_char(buf, ch);
    send_to_char("\n\r\n\r'ccenter reset' to restore default values.\n\r", ch);
    return;
  }
  if (!str_cmp(arg1, "lexp"))
  {
    ccenter[CCENTER_MIN_EXP] = atoi(arg2);
    send_to_char("Ok. Value Changed.\n\r", ch);
    save_ccenter();
    do_ccenter(ch, "");
    return;
  }
  else if (!str_cmp(arg1, "uexp"))
  {
    ccenter[CCENTER_MAX_EXP] = atoi(arg2);
    send_to_char("Ok. Value Changed.\n\r", ch);
    save_ccenter();
    do_ccenter(ch, "");
    return;
  }
  else if (!str_cmp(arg1, "elvl"))
  {
    ccenter[CCENTER_EXP_LEVEL] = atoi(arg2);
    send_to_char("Ok. Value Changed.\n\r", ch);
    save_ccenter();
    do_ccenter(ch, "");
    return;
  }
  else if (!str_cmp(arg1, "qlvl"))
  {
    ccenter[CCENTER_QPS_LEVEL] = atoi(arg2);   
    send_to_char("Ok. Value Changed.\n\r", ch);
    save_ccenter();
    do_ccenter(ch, "");
    return;
  }
  else if (!str_cmp(arg1, "blvl"))
  {
    ccenter[CCENTER_BONES_LEVEL] = atoi(arg2);
    send_to_char("Ok. Value Changed.\n\r", ch);
    save_ccenter();
    do_ccenter(ch, "");
    return;
  }
  else if (!str_cmp(arg1, "artm"))
  {
    ccenter[CCENTER_ARTI_MKILLS] = atoi(arg2);
    send_to_char("Ok. Value Changed.\n\r", ch);
    save_ccenter();
    do_ccenter(ch, "");
    artimkills = ccenter[CCENTER_ARTI_MKILLS]; 
    return;
  }

  else if (!str_cmp(arg1, "reset"))
  {
    ccenter[CCENTER_MIN_EXP] = 4000;
    ccenter[CCENTER_MAX_EXP] = 3000000;
    ccenter[CCENTER_EXP_LEVEL] = 100;
    ccenter[CCENTER_QPS_LEVEL] = 100;
    ccenter[CCENTER_BONES_LEVEL] = 1;
    ccenter[CCENTER_ARTI_MKILLS] = 3000;
    send_to_char("Ok. Values Reset.\n\r", ch);
    save_ccenter();
    do_ccenter(ch, "");
    return;
  }
  else
  {
    send_to_char("Nope.\n\r", ch);
    return;
  }
}





void do_pathfind(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  char *path;

  one_argument(argument, arg);
  if ((victim = get_char_world(ch, arg)) == NULL) return;

  if ((path = pathfind(ch, victim)) != NULL)
    xprintf(buf, "Path: %s\n\r", path);
  else
    xprintf(buf, "Path: Unknown.\n\r");
  send_to_char(buf, ch);

  return;
}
void do_tourney(CHAR_DATA *ch, char *argument)
{
  if (tourney)
  {
    tourney = FALSE;
    send_to_char("Off.\n\r", ch);
    return;
  }
  tourney = TRUE;
  send_to_char("On.\n\r", ch);
  return;
}

void do_aggrocheck(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];

  send_to_char("#0Name          From    Towards   Counter  Bully   BC#n\n\r", ch);
  for (gch = char_list; gch; gch = gch->next)
  {
    if (IS_NPC(gch)) continue;
    xprintf(buf, "%-12s  %4d       %4d       %3d    %3d  %3d\n\r",
      gch->name, gch->pcdata->aggress_from, gch->pcdata->aggress_towards,
      gch->pcdata->agg_counter, gch->pcdata->bully_points,
      gch->pcdata->bully_counter);
    send_to_char(buf, ch);
  }
}

void do_cutthis(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];

  xprintf(buf, "%s", cut_after_locate(argument));
  send_to_char(buf, ch);
}

void do_trusted(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;
  if (str_cmp(ch->name, "Jobo"))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }

  one_argument(argument, arg);

  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r", ch);
    return;
  }
  if (IS_SET(victim->pcdata->jflags, JFLAG_POLICE))
  {
    send_to_char("Removing trust.\n\r", ch);
    send_to_char("Your trust has been removed.\n\r", victim);
    REMOVE_BIT(victim->pcdata->jflags, JFLAG_POLICE);
    return;
  }
  SET_BIT(victim->pcdata->jflags, JFLAG_POLICE);
  send_to_char("Adding trust.\n\r", ch);
  send_to_char("You are now trusted.\n\r", victim);
  return;
}

void do_psilence(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  if (!IS_SET(ch->pcdata->jflags, JFLAG_POLICE))
  {
    send_to_char("Huh??\n\r", ch);
    return;
  }
  one_argument(argument, arg);
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Silence whom?\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("You cannot silence mobiles.\n\r", ch);
    return;
  }
  if ((get_age(victim) - 17) > 1)
  {
    send_to_char("Sorry, you can only silence newbies.\n\r", ch);
    return;
  }
  SET_BIT(victim->act, PLR_SILENCE);
  send_to_char("They are now silenced, this action was logged.\n\r", ch);
  xprintf(buf, "%s was silenced by our local police %s.", victim->name, ch->name);
  log_string(LOG_GAME, buf);
  return;
}

void do_plog(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  int count;

  if (IS_NPC(ch)) return;
  if (!IS_SET(ch->pcdata->jflags, JFLAG_POLICE))
  {
    send_to_char("Huh??\n\r", ch);
    return;
  }
  argument = one_argument(argument, arg);
  one_argument(argument, arg2);
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Log whom?\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("You cannot log mobiles.\n\r", ch);
    return;
  }
  if ((count = atoi(arg2)) < 10 || count >  100)
  {
    send_to_char("Please state how many actions should be logged (between 10 and 100).\n\r", ch);
    return;
  }
  victim->pcdata->log_count = count;
  SET_BIT(victim->act, PLR_LOG);
  send_to_char("Logging.\n\r", ch);
  return;
}


