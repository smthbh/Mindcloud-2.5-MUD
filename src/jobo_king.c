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
/***********************************
 * Dystopian Kingdom Code, by Jobo *
 ***********************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

void imm_kset     args (( CHAR_DATA *ch, char *argument ));

void save_kingdoms()
{
  int i;
  FILE *fp;

  if ((fp = fopen("../txt/kingdoms.txt","w")) == NULL)
  {
    log_string(LOG_CRIT, "Error writing to kingdoms.txt");
    return;
  }
  for (i = 1; i <= MAX_KINGDOM; i++)
  {
    fprintf(fp, "%s~\n", kingdom_table[i].name);
    fprintf(fp, "%s~\n", kingdom_table[i].whoname);
    fprintf(fp, "%s~\n", kingdom_table[i].leader);
    fprintf(fp, "%s~\n", kingdom_table[i].general);
    fprintf(fp, "%d\n", kingdom_table[i].recall);
    fprintf(fp, "%d %d %d\n",
      kingdom_table[i].kills, kingdom_table[i].deaths, kingdom_table[i].qps);
    fprintf(fp, "%d %d %d %d\n",
      kingdom_table[i].req_hit, kingdom_table[i].req_move,
      kingdom_table[i].req_mana, kingdom_table[i].req_qps);
  }
  fclose(fp);
}

void load_kingdoms()
{
  int i;
  FILE *fp;

  kingdom_table[0].name = "";
  kingdom_table[0].whoname = "";
  kingdom_table[0].leader = "";
  kingdom_table[0].general = "";
  kingdom_table[0].kills = 0;
  kingdom_table[0].deaths = 0;
  kingdom_table[0].qps = 0;
  kingdom_table[0].req_hit = 0;
  kingdom_table[0].req_move = 0;
  kingdom_table[0].req_mana = 0;
  kingdom_table[0].req_qps = 0;

  if ((fp = fopen("../txt/kingdoms.txt", "r")) == NULL)
  {
    log_string(LOG_CRIT, "Fatal Error: kingdoms.txt not found!");
    exit(1);
  }
  for (i = 1; i <= MAX_KINGDOM; i++)
  {
    kingdom_table[i].name = fread_string(fp);
    kingdom_table[i].whoname = fread_string(fp);
    kingdom_table[i].leader = fread_string(fp);
    kingdom_table[i].general = fread_string(fp);
    kingdom_table[i].recall = fread_number(fp);
    kingdom_table[i].kills = fread_number(fp);
    kingdom_table[i].deaths = fread_number(fp);
    kingdom_table[i].qps = fread_number(fp);
    kingdom_table[i].req_hit = fread_number(fp);
    kingdom_table[i].req_move = fread_number(fp);
    kingdom_table[i].req_mana = fread_number(fp);
    kingdom_table[i].req_qps = fread_number(fp);
  }
  fclose(fp);
}

void do_kingdoms(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char pkratio[20];  // perhaps some colors :)
  int i;

  if (IS_NPC(ch)) return;

  xprintf(buf, "Name          Pkills  Pdeaths  Leader   REQS : hps    move    mana     bones\n\r");
  for (i = 1; i <= MAX_KINGDOM; i++)
  {
    /*
     * calculation ratio
     */
    if (kingdom_table[i].kills > 0)
    {
      if (100 * kingdom_table[i].kills/(kingdom_table[i].kills + kingdom_table[i].deaths) < 100)
        xprintf(pkratio, "0.%-2d",
          (100 * kingdom_table[i].kills/(kingdom_table[i].kills + kingdom_table[i].deaths)));
      else
        xprintf(pkratio, "1.00");
    }
    else xprintf(pkratio, "0.00");

    /*
     * buffering everything
     */
    xprintf(buf2, "%-14s  %3d      %3d    %s       %5d   %5d   %5d   %5d\n\r",
      kingdom_table[i].name, kingdom_table[i].kills, kingdom_table[i].deaths, kingdom_table[i].leader, 
      kingdom_table[i].req_hit, kingdom_table[i].req_move, kingdom_table[i].req_mana, kingdom_table[i].req_qps);
    strcat(buf, buf2);
  }
  send_to_char(buf, ch);
  return;
}

void do_kinduct(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (ch->pcdata->kingdom == 0)
  {
    send_to_char("What kingdoms did you say you where from ?\n\r", ch);
    return;
  }
  if (str_cmp(kingdom_table[ch->pcdata->kingdom].leader, ch->name) &&
      str_cmp(kingdom_table[ch->pcdata->kingdom].general, ch->name))
  {
    send_to_char("Talk to your kingdom leaders, they are the only persons allowed to induct new members.\n\r", ch);
    return;
  }
  if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("That's silly.\n\r", ch);
    return;
  }
  if (victim->pcdata->kingdom != 0)
  {
    send_to_char("They are already a member of another kingdom.\n\r", ch);
    return;
  }
  if (victim->max_hit < kingdom_table[ch->pcdata->kingdom].req_hit ||
      victim->max_move < kingdom_table[ch->pcdata->kingdom].req_move ||
      victim->bones < kingdom_table[ch->pcdata->kingdom].req_qps ||
      victim->max_mana < kingdom_table[ch->pcdata->kingdom].req_mana)
  {
    send_to_char("They don't have the reqs to get inducted.\n\r", ch);
    return;
  }
  if (!IS_SET(victim->pcdata->jflags, JFLAG_WANT_KINGDOM))
  {
    send_to_char("They don't want to be inducted.\n\r", ch);
    return;
  }
  victim->pcdata->kingdom = ch->pcdata->kingdom;
  victim->bones -= kingdom_table[ch->pcdata->kingdom].req_qps;
  kingdom_table[ch->pcdata->kingdom].qps += kingdom_table[ch->pcdata->kingdom].req_qps;
  send_to_char("You induct them into your kingdom.\n\r", ch);
  send_to_char("You have been inducted.\n\r", victim);
  return;
}

void do_wantkingdom(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;

  if (IS_SET(ch->pcdata->jflags, JFLAG_WANT_KINGDOM))
  {
    REMOVE_BIT(ch->pcdata->jflags, JFLAG_WANT_KINGDOM);
    send_to_char("You can no longer be inducted.\n\r", ch);
    return;
  }
  SET_BIT(ch->pcdata->jflags, JFLAG_WANT_KINGDOM);
  send_to_char("You can now be inducted.\n\r", ch);
  return;
}

void do_kdeposit(CHAR_DATA *ch, char *argument) {
	//char arg1[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int	 amt;
	
	if (IS_NPC(ch)) return;
	
	if (ch->pcdata->kingdom == 0) {
	send_to_char("Huh?\n\r", ch );
	return;}
	
	amt = atoi(argument);
		if (amt < 1)
		{
			send_to_char("Syntax: kdeposit <amt>\n\r",ch);
			return;
		}

	if ( amt > ch->bones)
		{
			send_to_char("My my my, you have philanthropy down to an artform?\n\rYou give away more then you own!\n\r",ch);
			return;
		}
	
		xprintf(buf,"You deposit %d bones into the kingdom treasury.\n\r",amt);
		send_to_char(buf,ch);
		xprintf(buf,"%s has deposited %d bones into the kingdom treasury.",ch->name,amt);
		do_info(ch,buf);
		make_note("Kingdom","Kingdom Clerk",kingdom_table[ch->pcdata->kingdom].name,"Treasury Grows",7,buf);
		
		ch->bones -= amt;
		kingdom_table[ch->pcdata->kingdom].qps += amt;
		save_char_obj(ch);
		save_kingdoms();
}

void do_kset(CHAR_DATA *ch, char *argument)
{
  char keyword[MAX_INPUT_LENGTH];
  char value[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;
  if (ch->level > 6)
  {
    imm_kset(ch, argument);
    return;
  }
  argument = one_argument(argument, keyword);
  one_argument(argument, value);
  if (ch->pcdata->kingdom == 0)
  {
    send_to_char("But you are not a member of a kingdom.\n\r", ch);
    return;
  }
  if (str_cmp(kingdom_table[ch->pcdata->kingdom].leader, ch->name))
  {
    send_to_char("Only the leader can change the kingdom settings.\n\r", ch);
    return;
  }
  if (keyword[0] == '\0' || value[0] == '\0')
  {
    send_to_char("What do you want to change ?\n\r\n\r", ch);
    send_to_char("req_mana, req_hit, req_move, req_qps or general.\n\r", ch);
    return;
  }
  if (!str_cmp(keyword, "req_mana"))
  {
    kingdom_table[ch->pcdata->kingdom].req_mana = atoi(value);
  }
  else if (!str_cmp(keyword, "req_qps"))
  {
    kingdom_table[ch->pcdata->kingdom].req_qps = atoi(value);
  }
  else if (!str_cmp(keyword, "req_hit"))
  {
    kingdom_table[ch->pcdata->kingdom].req_hit = atoi(value);
  }
  else if (!str_cmp(keyword, "req_move"))
  {
    kingdom_table[ch->pcdata->kingdom].req_move = atoi(value);
  }
  else if (!str_cmp(keyword, "general"))
  {
    free_string(kingdom_table[ch->pcdata->kingdom].general);
    kingdom_table[ch->pcdata->kingdom].general = str_dup(value);
  }
  else
  {
    do_kset(ch, "");
    return;
  }
  send_to_char("Done.\n\r", ch);
  save_kingdoms();
  return;
}

void imm_kset(CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char arg3[MAX_INPUT_LENGTH];
  int i;

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  one_argument(argument, arg3);

  if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0')
  {
    send_to_char("Syntax       : kset <kingdom> <field> <value>\n\r", ch);
    send_to_char("Valid fields : leader, name, whoname, recall, kills, deaths, treasury\n\r", ch);
    return;
  }
  if (!is_number(arg1))
  {
    send_to_char("Please pick a number as the kingdom.\n\r", ch);
    return;
  }
  i = atoi(arg1);
  if (i < 1 || i > MAX_KINGDOM)
  {
    send_to_char("Please pick a real kingdom.\n\r", ch);
    return;
  }
  if (!str_cmp(arg2, "leader"))
  {
    free_string(kingdom_table[i].leader);
    arg3[0] = UPPER(arg3[0]);
    kingdom_table[i].leader = str_dup(arg3);
  }
  else if (!str_cmp(arg2, "name"))
  {
    free_string(kingdom_table[i].name);
    arg3[0] = UPPER(arg3[0]);
    kingdom_table[i].name = str_dup(arg3);
  }
  else if (!str_cmp(arg2, "whoname"))
  {
    free_string(kingdom_table[i].whoname); 
    arg3[0] = UPPER(arg3[0]);
    kingdom_table[i].whoname = str_dup(arg3);
  }
  else if (!str_cmp(arg2, "kills"))
  {
    kingdom_table[i].kills = atoi(arg3);
  }
  else if (!str_cmp(arg2, "treasury"))
  {
    kingdom_table[i].qps = atoi(arg3);
  }
  else if (!str_cmp(arg2, "recall"))
  {
    kingdom_table[i].recall = atoi(arg3);
  }
  else if (!str_cmp(arg2, "deaths"))
  {
    kingdom_table[i].deaths = atoi(arg3);
  }
  else
  {
    imm_kset(ch, "");
    return;
  }
  send_to_char("Done.\n\r", ch);
  save_kingdoms();
  return;
}

void do_koutcast(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (ch->pcdata->kingdom == 0)
  {
    send_to_char("What kingdoms did you say you where from ?\n\r", ch);
    return;
  }
  if (str_cmp(kingdom_table[ch->pcdata->kingdom].leader, ch->name) &&
      str_cmp(kingdom_table[ch->pcdata->kingdom].general, ch->name))
  {
    send_to_char("You are not allowed to outcast members.\n\r", ch);
    return;
  }
  if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("Outcast whom?\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("That's a monster, not a player.\n\r", ch);
    return;
  }
  if (ch == victim)
  {
    send_to_char("You cannot outcast yourself.\n\r", ch);
    return;
  }
  if (victim->pcdata->kingdom != ch->pcdata->kingdom)
  {
    send_to_char("They are not a member of your kingdom.\n\r", ch);
    return;
  }
  if (!str_cmp(victim->name, kingdom_table[ch->pcdata->kingdom].leader))
  {
    send_to_char("That is not a good plan.\n\r", ch);
    return;
  }
  victim->pcdata->kingdom = 0;
  send_to_char("Done.\n\r", ch);
  send_to_char("You have been outcasted from your kingdom.\n\r", victim);
  return;
}

void do_kstats(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int i;

  if (IS_NPC(ch)) return;
  if ((i = ch->pcdata->kingdom) == 0)
  {
    send_to_char("You are not a member of any kingdom.\n\r", ch);
    return;
  }
  xprintf(buf, " [***]  The Kingdom stats of %s  [***]\n\r\n\r", kingdom_table[i].whoname);
  xprintf(buf2, " Current Leader    : %s\n\r", kingdom_table[i].leader);
  strcat(buf, buf2);
  xprintf(buf2, " Current General   : %s\n\r", kingdom_table[i].general);
  strcat(buf, buf2);
  xprintf(buf2, " Treasury          : %d bones\n\r", kingdom_table[i].qps);
  strcat(buf, buf2);
  send_to_char(buf, ch);
  return;
}

void do_kingset(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  int i;

  if (IS_NPC(ch) || ch->level < 7)
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  argument = one_argument(argument, arg1);
  one_argument(argument, arg2);
  if (arg1[0] == '\0' || arg2[0] == '\0')
  {
    send_to_char("Syntax : kingset <player> <kingdom number>\n\r", ch);
    return;
  }
  if ((victim = get_char_world(ch, arg1)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if ((i = atoi(arg2)) < 0 || i > MAX_KINGDOM)
  {
    send_to_char("Please pick a valid kingdom.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Please pick a player.\n\r", ch);
    return;
  }
  victim->pcdata->kingdom = i;
  send_to_char("Ok.\n\r", ch);
  send_to_char("Your kingdom has been changed.\n\r", victim);
  return;
}

void load_war(void)
{
    FILE *fp;
    int i;

	if ((fp = fopen(WAR_FILE, "r")) == NULL) {
	log_string(LOG_CRIT, "Error: war.dat not found!");
	exit(1);}

    for(i=0; i < MAX_WAR; i++ ){
	war_table[i].one = fread_number(fp);
	war_table[i].two = fread_number(fp);
    }
	fclose(fp);
    return;
}

void save_war(void)
{
    FILE *fp;
    int i;

	if ((fp = fopen(WAR_FILE, "w")) == NULL) {
	log_string(LOG_CRIT, "Error: kingdom.dat not found!");
	exit(1);}
                
        for (i = 0; i < MAX_WAR; i++) {
        fprintf(fp,"%d\n", war_table[i].one);
	fprintf(fp,"%d\n", war_table[i].two);
        }
        
        fclose(fp);
        return;
}

void do_warlist(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    int i;

    stc( "#B--==== #RThe WARLIST!!! #B====--#n\n\r\n\r",ch);

    for(i=0 ; i < MAX_WAR ; i++){
	if( war_table[i].one != 0
	&& war_table[i].one <= MAX_KINGDOM
	&& war_table[i].two <= MAX_KINGDOM)
	{
	xprintf(buf, "#R(#W%d#R) #n%s's Kingdom of the %s is at war with\n\r%s's Kingdom of the %s\n\r",
	i + 1, kingdom_table[war_table[i].one].leader,
        kingdom_table[war_table[i].one].name,
	kingdom_table[war_table[i].two].leader,
        kingdom_table[war_table[i].two].name );
	stc(buf,ch);
	}
    }
    return;
}

void do_decwar(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    int i;
    int target=0, slot = -1;

    one_argument(argument, arg);

	if (ch->pcdata->kingdom == 0) {
	send_to_char("You are not in a clan.\n\r", ch );
	return;}

	if (str_cmp(ch->pcdata->switchname,kingdom_table[ch->pcdata->kingdom].leader)) {
	send_to_char("You are not the leader of your clan.\n\r", ch );
	return;}

    if( arg[0] == '\0' )
    {
	stc( "You need to type the name of the leader of the kingdom to declare war on.\n\r",ch);
	return;
    }

    for( i = 1 ; i <= MAX_KINGDOM ; i++ ){
	if( !str_prefix(arg,kingdom_table[i].leader) ) target = i;
    }

    if( target == 0 )
    {
	stc( "No match found.\n\r",ch);
	return;
    }

    for( i=0 ; i <= MAX_WAR ; i++ ){
	if( (war_table[i].one == ch->pcdata->kingdom && war_table[i].two == target)
	|| (war_table[i].two == ch->pcdata->kingdom && war_table[i].one == target )){
	    stc( "You are already at war with them. Use warpeace to have a peace.\n\r",ch);
	    return;
        }
	if(war_table[i].one == 0 && war_table[i].two == 0 && slot < 0 ) slot = i;
    }

    if( slot < 0 ){
	stc( "No slots left.\n\r",ch);
	return;
    }

    war_table[slot].one = ch->pcdata->kingdom;
    war_table[slot].two = target;
    stc( "You are now at WAR!!!!\n\r",ch);
	save_war();
    do_info( ch, "THERE IS A NEW WAR!!!!! WARLIST TO SEE!!!!");
    return;
}

void do_warpeace( CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    int i;
    int target=0, slot = -1;

    one_argument(argument, arg);

	if (ch->pcdata->kingdom == 0) {
	send_to_char("You are not in a clan.\n\r", ch );
	return;}

	if (str_cmp(ch->pcdata->switchname,kingdom_table[ch->pcdata->kingdom].leader)) {
	send_to_char("You are not the leader of your clan.\n\r", ch );
	return;}

    if( arg[0] == '\0' )
    {
	stc( "You need to type the name of the leader of the kingdom to make peace with.\n\r",ch);
	return;
    }

    for( i = 1 ; i <= MAX_KINGDOM ; i++ ){
	if( !str_prefix(arg,kingdom_table[i].leader) ) target = i;
    }

    if( target == 0 )
    {
	stc( "No match found.\n\r",ch);
	return;
    }

    for( i=0 ; i <= MAX_WAR ; i++ ){
	if( (war_table[i].one == ch->pcdata->kingdom && war_table[i].two == target) ){
	slot = i;
        }
    }

    if( slot < 0 ){
	stc( "You are not at war with them, or they declared war on you. Get the other leader to make peace.\n\r",ch);
	return;
    }

    war_table[slot].one = 0;
    war_table[slot].two = 0;
    stc( "You are now at PEACE!!!!\n\r",ch);
    do_info(ch,"THERE IS A NEW PEACE BETWEEN KINGDOMS!!");
	save_war();

    return;
}
void do_krecall(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;
	CHAR_DATA *mount;
	ROOM_INDEX_DATA *location;

	if (IS_NPC(ch)) return;

	if (ch->pcdata->kingdom == 0) {
	send_to_char("Huh?\n\r", ch );
	return;}

	act("$n's body flickers with green energy.",ch,NULL,NULL,TO_ROOM);
	act("Your body flickers with green energy.",ch,NULL,NULL,TO_CHAR);

        if (ch->fight_timer>0)
	{
	send_to_char("Not with a fighttimer.\n\r",ch);
	return;
	}

	if ((location = get_room_index(kingdom_table[ch->pcdata->kingdom].recall)) == NULL) {
	send_to_char("You are completely lost.\n\r", ch );
	return;}

	if (ch->in_room == location) return;

	if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) || IS_AFFECTED(ch, AFF_CURSE)) {
	send_to_char("You are unable to recall.\n\r", ch );
	return;}

	if ((victim = ch->fighting) != NULL) {
	if (number_bits(1) == 0) {
		WAIT_STATE(ch,4);
		send_to_char("You failed!\n\r", ch );
		return;}
	send_to_char("You recall from combat!\n\r", ch );
	stop_fighting(ch,TRUE);
	}

	act("$n disappears.",ch,NULL,NULL,TO_ROOM);
	char_from_room(ch);
	char_to_room(ch,location);
	act("$n appears in the room.",ch,NULL,NULL,TO_ROOM);
	do_look(ch,"auto");
	if ((mount = ch->mount) == NULL) return;
	char_from_room(mount);
	char_to_room(mount,ch->in_room);
	
	return;
}
