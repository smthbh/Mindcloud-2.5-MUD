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
#include "interp.h"

void check_samuraiattack     args ((CHAR_DATA *ch, CHAR_DATA *victim));

void do_katana( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SAMURAI))
  {         
    send_to_char("Huh?\n\r",ch);
    return;
  }  
  if ( 250 > ch->practice)
  {
    send_to_char("It costs 250 points of primal to create a Katana.\n\r",ch);
    return;
  }     
  ch->practice   -= 250;
  obj = create_object(get_obj_index(33176) ,50 );
  SET_BIT(obj->quest, QUEST_RELIC);
  obj->condition = 100;
  obj->toughness = 100;
  obj->resistance = 1;
  obj->value[1] = 65;
  obj->value[2] = 115;
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj->ownerid = ch->pcdata->playerid;
  obj_to_char(obj, ch);
  act("A katana flashes into existance.",ch,NULL,NULL,TO_CHAR);
  act("A katana suddenly apear in $n's hands.",ch,NULL,NULL,TO_ROOM);
  return;
}

void do_samseye( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SAMURAI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  
  if (IS_SET(ch->act, PLR_HOLYLIGHT))
  {
    REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
    send_to_char("Your minds eye closes and you see the world as mortals do.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->act, PLR_HOLYLIGHT);
    send_to_char( "You meditate and open your minds eye to the world.\n\r", ch );
  }
  return;
}

void do_bladespin( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;
  if ( !IS_CLASS(ch, CLASS_SAMURAI) )
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if (ch->wpn[3] < 1000 || ch->wpn[0]<1000 || ch->wpn[1] <1000)
  {
    stc("You are not a true samurai yet.\n\r",ch);
    return;
  }          
  if ( IS_SET(ch->newbits, NEW_BLADESPIN) )
  {
    stc("You relax your combat techniques.\n\r",ch);
    act("$n relaxes.",ch,NULL,NULL,TO_ROOM);
    REMOVE_BIT(ch->newbits, NEW_BLADESPIN);
    return;
  }
  if (!IS_SET(ch->newbits, NEW_BLADESPIN) )
  {
    stc("Your concentrate on your combat techniques.\n\r",ch);
    act("$n's muscles flexes, looking for prey that one, better beware!",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->newbits, NEW_BLADESPIN);
    return;
  }
  return;
}

void do_hologramtransfer( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_SAMURAI) )
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Walk the ancestrial path to whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not of this world.\n\r", ch );
	return;
    }
    
   if( ch->move < 1000) 
    {
     stc( "The path escapes your senses as you loose control over your inner energies.\n\r",ch);
     return;
    }

 if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  { 
    stc( "You can't find it's room.\n\r",ch);
    return; 
  } 
    if (IS_SET(victim->act, ACT_NOTRAVEL))
  {
  	send_to_char("No Can Do.\n\r", ch);
  	return;
  }
  if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You cannot sense any paths leading from this room.\n\r",ch);
    return;
  }

    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
	  return;
    }
    if ( room_is_private(victim->in_room ) )
    {
        send_to_char( "That room is private right now.\n\r", ch );
        return;
    }
    if ( !IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    {	
	send_to_char( "They don't want you near them.\n\r", ch );
        return;
    }
    if (victim->in_room == ch->in_room)
    {
	send_to_char("But you're already there!\n\r",ch);
        return;
    }

    act("You follow the path to $N.", ch, NULL, victim, TO_CHAR);
    act("$n steps into the air, leaving behind no trace whatsoever.", ch, NULL, victim, TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    ch->move -= 1000;
    act("$n steps out of the air in front of $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n steps out of the air in front of you.", ch, NULL, victim, TO_VICT);
    do_look(ch,"auto");
    return;
}

void do_focus(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SAMURAI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SAMURAI_FOCUS] < 10)
  {
    send_to_char("You cannot focus your weaponcontrol any more.\n\r",ch);
    return;
  }
  ch->pcdata->powers[SAMURAI_FOCUS] -= number_range(1, ch->pcdata->powers[SAMURAI_FOCUS]);
  send_to_char("You focus your control, letting the battle slow to a halt.\n\r",ch);
  WAIT_STATE(ch, 8);
  return;
}

void do_slide(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SAMURAI))
  { 
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_SLIDE))
  {
    send_to_char("You need to learn that combo first.\n\r", ch);
    return;
  }
  if (ch->pcdata->powers[SAMURAI_FOCUS] > 40)
  {
    send_to_char("You are to exhausted.\n\r",ch);
    return;
  }
  if ((victim = ch->fighting) == NULL)
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  ch->pcdata->powers[SAMURAI_FOCUS] += 1;
  act("$n slides into a better fighting position, hitting $N while $e passes.",ch,NULL,victim,TO_NOTVICT);
  act("You slide into a better fighting position, hitting $N while you pass $E.",ch,NULL,victim,TO_CHAR);
  act("$n moves past you, lightning fast and strikes you before you can react.",ch,NULL,victim,TO_VICT);
  one_hit(ch, victim, gsn_lightningslash, 1);
  check_samuraiattack(ch, victim);
  WAIT_STATE(ch, 12);
  return;
}

void do_sidestep(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SAMURAI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_SIDESTEP))
  {
    send_to_char("You need to learn that combo first.\n\r", ch);
    return;
  }
  if (ch->pcdata->powers[SAMURAI_FOCUS] > 40)
  {
    send_to_char("You are to exhausted.\n\r",ch);
    return;
  }
  if ((victim = ch->fighting) == NULL)
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  ch->pcdata->powers[SAMURAI_FOCUS] += 2;
  act("$n sidesteps $N's attack and scores a hit.",ch,NULL,victim,TO_NOTVICT);
  act("You sidestep $N's attack, and scores a counterattack before $E can react.",ch,NULL,victim,TO_CHAR);
  act("$n sidesteps your feeble attempt to get near $m and strikes back at you.",ch,NULL,victim,TO_VICT);
  one_hit(ch, victim, gsn_lightningslash, 1);
  check_samuraiattack(ch, victim);
  WAIT_STATE(ch, 12);
  return;
}

void do_block(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SAMURAI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_BLOCK))
  {
    send_to_char("You need to learn that combo first.\n\r", ch);
    return;
  }
  if (ch->pcdata->powers[SAMURAI_FOCUS] > 40)
  {
    send_to_char("You are to exhausted.\n\r",ch);
    return;
  }
  if ((victim = ch->fighting) == NULL)
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  ch->pcdata->powers[SAMURAI_FOCUS] += 4;
  act("$n blocks $N's simpleminded attack and strikes back with a perfect attack.",ch,NULL,victim,TO_NOTVICT);
  act("You block $N's simple attack and return with one of your own design.",ch,NULL,victim,TO_CHAR);
  act("$n blocks your attack, and strikes back before you get a chance to react.",ch,NULL,victim,TO_VICT);
  one_hit(ch, victim, gsn_lightningslash, 1);
  check_samuraiattack(ch, victim);
  WAIT_STATE(ch, 12);
  return;
}

void do_countermove(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SAMURAI))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_COUNTERMOVE))
  {
    send_to_char("You need to learn that combo first.\n\r", ch);
    return;
  }
  if (ch->pcdata->powers[SAMURAI_FOCUS] > 40)
  {
    send_to_char("You are to exhausted.\n\r",ch);
    return;
  }
  if ((victim = ch->fighting) == NULL)
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  ch->pcdata->powers[SAMURAI_FOCUS] += 8;
  act("$n strikes out at $N before $E can even get a weapon out to defend $Mself.",ch,NULL,victim,TO_NOTVICT);
  act("You cut $N with a lightning fast attack.",ch,NULL,victim,TO_CHAR);
  act("$n attacks with a flurry of lightning fast attacks, one of them scores a hit.",ch,NULL,victim,TO_VICT);
  one_hit(ch, victim, gsn_lightningslash, 1);
  check_samuraiattack(ch, victim);
  WAIT_STATE(ch, 12);
  return;
}

void check_samuraiattack(CHAR_DATA *ch, CHAR_DATA *victim)
{
  if (!IS_CLASS(ch, CLASS_SAMURAI)) return;
  if (!victim || victim->hit < 1000) return;

  switch(ch->pcdata->powers[SAMURAI_FOCUS])
  {
    default : break;
    case 10 :
      if (ch->pcdata->powers[SAMURAI_LAST] == 10)
      {
        send_to_char("You cannot use the same combo twice.\n\r",ch);
        break;
      }
      ch->pcdata->powers[SAMURAI_LAST] = 10;
      do_say(ch, "What a worthless opponent you are. Here, let me show you how a real fighter fights.");
      one_hit(ch, victim, gsn_lightningslash, 1);
      one_hit(ch, victim, gsn_lightningslash, 1);
      one_hit(ch, victim, gsn_lightningslash, 1);
      break; 
    case 15 :
      if (ch->pcdata->powers[SAMURAI_LAST] == 15)
      {
        send_to_char("You cannot use the same combo twice.\n\r",ch);
        break;
      }  
      ch->pcdata->powers[SAMURAI_LAST] = 15;
      if (number_range(1,3) == 2)
      {
        send_to_char("You fail your attempt to disarm.\n\r",ch);
        break;
      }
      act("#GYou strike out at $N's weapon.#n",ch,NULL,victim,TO_CHAR);
      act("#G$n's attack strikes your weaponarm.#n",ch,NULL,victim,TO_VICT);
      disarm(ch, victim);
      break;
    case 20 :
      if (ch->pcdata->powers[SAMURAI_LAST] == 20)
      {
        send_to_char("You cannot use the same combo twice.\n\r",ch);
        break;
      }
      ch->pcdata->powers[SAMURAI_LAST] = 20;
      act("#GYour hit $N on a central nerve, paralysing $M.#n",ch,NULL,victim,TO_CHAR);
      act("#G$n's attack paralyses you, you cannot move.#n",ch,NULL,victim,TO_VICT);
      if (IS_CLASS(victim, CLASS_FAE))
      {
        send_to_char("You shrug of the attack.\n\r", victim);
        send_to_char("They don't seem that affected.\n\r", ch);
      }
      else WAIT_STATE(victim, 24);
      break;
    case 25 :
      if (ch->pcdata->powers[SAMURAI_LAST] == 25)
      {
        send_to_char("You cannot use the same combo twice.\n\r",ch);
        break;
      }
      ch->pcdata->powers[SAMURAI_LAST] = 25;
      if (number_range(1,3) == 2)
      {
        send_to_char("You fail your attempt to hurl.\n\r",ch);
        break;
      }
      act("#GYou grab $N and toss $M over your shoulder.#n",ch,NULL,victim,TO_CHAR);
      act("#G$n grabs you, and tosses you over $s shoulder.#n",ch,NULL,victim,TO_VICT);
      special_hurl(ch, victim);
      break;
    case 30 :
      if (ch->pcdata->powers[SAMURAI_LAST] == 30)
      {
        send_to_char("You cannot use the same combo twice.\n\r",ch);
        break;
      }
      ch->pcdata->powers[SAMURAI_LAST] = 30;
      ch->hit += number_range(2000, 4000);
      if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
      act("#GYou feel adrenalin pump through your body, awakening your senses.#n",ch,NULL,victim,TO_CHAR);
      act("#G$n flashes a wicked smile.#n",ch,NULL,victim,TO_VICT);
      break;
    case 35 :
      if (ch->pcdata->powers[SAMURAI_LAST] == 35)
      {
        send_to_char("You cannot use the same combo twice.\n\r",ch);
        break;
      }
      ch->pcdata->powers[SAMURAI_LAST] = 35;
      do_say(ch, "I am a master of arms, you cannot even begin to understand what I am able to do!");
      one_hit(ch, victim, gsn_backfist, 1);
      one_hit(ch, victim, gsn_thrustkick, 1);
      one_hit(ch, victim, gsn_monksweep, 1);
      one_hit(ch, victim, gsn_jumpkick, 1);
      one_hit(ch, victim, gsn_lightningslash, 1);
      break;
  }
  return;
}

void do_martial(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int cost = 150000000; // 150 million for each combo.

  one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SAMURAI))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("What martial art do you wish to learn : slide, sidestep, block or countermove ?\n\r", ch);
    return;
  }
  if (!str_cmp(arg, "slide"))
  {
    if (IS_SET(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_SLIDE))
    {
      send_to_char("You have already learned that technique.\n\r", ch);
      return;
    }
    if (ch->exp < cost)
    {
      xprintf(buf, "You need %d more exp to learn this technique.\n\r", cost - ch->exp);
      send_to_char(buf,ch);
      return;
    }
    ch->exp -= cost;
    SET_BIT(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_SLIDE);
    send_to_char("Ok.\n\r", ch);
    return;
  }
  else if (!str_cmp(arg, "sidestep"))
  {
    if (IS_SET(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_SIDESTEP))
    {
      send_to_char("You have already learned that technique.\n\r", ch);
      return;
    }
    if (ch->exp < cost)
    {
      xprintf(buf, "You need %d more exp to learn this technique.\n\r", cost - ch->exp);
      send_to_char(buf,ch);
      return;
    }
    ch->exp -= cost;
    SET_BIT(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_SIDESTEP);
    send_to_char("Ok.\n\r", ch);
    return;
  }
  else if (!str_cmp(arg, "block"))
  {
    if (IS_SET(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_BLOCK))
    {
      send_to_char("You have already learned that technique.\n\r", ch);
      return;
    }
    if (ch->exp < cost)
    {
      xprintf(buf, "You need %d more exp to learn this technique.\n\r", cost - ch->exp);
      send_to_char(buf,ch);
      return;
    }
    ch->exp -= cost;
    SET_BIT(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_BLOCK);
    send_to_char("Ok.\n\r", ch);
    return;
  }
  else if (!str_cmp(arg, "countermove"))
  {
    if (IS_SET(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_COUNTERMOVE))
    {
      send_to_char("You have already learned that technique.\n\r", ch);
      return;
    }
    if (ch->exp < cost)
    {
      xprintf(buf, "You need %d more exp to learn this technique.\n\r", cost - ch->exp);
      send_to_char(buf,ch);
      return;
    }
    ch->exp -= cost;
    SET_BIT(ch->pcdata->powers[SAMURAI_MARTIAL], SAM_COUNTERMOVE);
    send_to_char("Ok.\n\r", ch);
    return;
  }
  else do_martial(ch, "");
  return;
}

void do_samlearn( CHAR_DATA *ch, char *argument )
{
    char      arg1 [MAX_INPUT_LENGTH];
    int       aura = 0;
    int       cost = 0;

    smash_tilde(argument);
    argument = one_argument(argument,arg1);

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_SAMURAI))
	{
		send_to_char( "Huh?\n\r", ch);
		return;
	}
	
	if ( arg1[0] == '\0' )
    {
		send_to_char( "Syntax: Samlearn <power>\n\r", ch );
		send_to_char("#r ___________________________________________\n\r",ch);
		send_to_char("#r/    #cName#r    |  #cCost#r  |    #cName#r    |  #cCost#r  \\\n\r",ch);
		send_to_char("#r|------------+--------+------------+--------|   \n\r",ch);
		send_to_char("#r|  #gLitSword  #r| #y250    #r|    #gSword   #r| #y200#r    |   \n\r",ch);
		send_to_char("#r|  #gIceSword  #r| #y350    #r|  #gFireSword #r| #y300#r    |   \n\r",ch);
		send_to_char("#r| #gSoulSword  #r| #y500    #r| #gDrainSword #r| #y400#r    |   \n\r",ch);
		send_to_char("#r\\___________________________________________/\n\r",ch);
		return;
    }
        
    if (!str_cmp(arg1,"soulsword")) 
		{	aura = SAM_SOULSWORD;	cost = 500;	}
    else if (!str_cmp(arg1,"drainsword")) 
		{	aura = SAM_DRAINSWORD;	cost = 400;	}
    else if (!str_cmp(arg1,"icesword")) 
		{	aura = SAM_ICESWORD;	cost = 350;	}
    else if (!str_cmp(arg1,"firesword")) 
		{	aura = SAM_FIRESWORD;	cost = 300;	}
    else if (!str_cmp(arg1,"litsword")) 
		{	aura = SAM_LITSWORD;	cost = 250;	}
    else if (!str_cmp(arg1,"sword")) 
		{	aura = SAM_SWORD;	cost = 200;	}
    else
    {
		send_to_char( "Syntax: Samlearn <power>\n\r", ch );
		send_to_char("#r ___________________________________________\n\r",ch);
		send_to_char("#r/    #cName#r    |  #cCost#r  |    #cName#r    |  #cCost#r  \\\n\r",ch);
		send_to_char("#r|------------+--------+------------+--------|   \n\r",ch);
		send_to_char("#r|  #gLitSword  #r| #y250    #r|    #gSword   #r| #y200#r    |   \n\r",ch);
		send_to_char("#r|  #gIceSword  #r| #y350    #r|  #gFireSword #r| #y300#r    |   \n\r",ch);
		send_to_char("#r| #gSoulSword  #r| #y500    #r| #gDrainSword #r| #y400#r    |   \n\r",ch);
		send_to_char("#r\\___________________________________________/\n\r",ch);
		return;
    }
	
	if ((aura == SAM_LITSWORD) && (!IS_SAMPOWER(ch, SAM_SWORD)))
	{
		send_to_char("Without learning Sword this is useless.\n\r",ch);
		return;
	}
	if ((aura == SAM_FIRESWORD) && (!IS_SAMPOWER(ch, SAM_LITSWORD)))
	{
		send_to_char("You may not learn FireSword until you learn LitSword.\n\r",ch);
		return;
	}
	if ((aura == SAM_ICESWORD) && (!IS_SAMPOWER(ch, SAM_FIRESWORD)))
	{
		send_to_char("You may not learn IceSword until you learn FirSword.\n\r",ch);
		return;
	}
	if ((aura == SAM_DRAINSWORD) && (!IS_SAMPOWER(ch, SAM_ICESWORD)))
	{
		send_to_char("You may not learn DrainSword until you learn IceSword.\n\r",ch);
		return;
	}
	if ((aura == SAM_SOULSWORD) && (!IS_SAMPOWER(ch, SAM_DRAINSWORD)))
	{
		send_to_char("You may not learn SoulSword until you've mastered all sword techniques.\n\r",ch);
		return;
	}
		
	if (IS_SAMPOWER(ch, aura))
    {
		send_to_char("You already have that power.\n\r",ch);
		return;
    }
    if (ch->practice < cost)
    {
		send_to_char("You have insufficient primal to learn that power.\n\r",ch);
		return;
    }
    SET_BIT(ch->pcdata->powers[SAMURAI_MARTIAL], aura);
    ch->practice -= cost;
    send_to_char( "Ok.\n\r", ch);
    save_char_obj(ch);
    return;
}

void lightning_sword (CHAR_DATA *ch)
{
	//Lightning Stuns Victim and is fairly likely to hit, Avg Expense, Avg Damage

	CHAR_DATA *victim;
	OBJ_DATA *katana;
	char buf[MAX_STRING_LENGTH];
	int dmg;
	
	if (IS_NPC(ch))	return;
	
	katana = get_eq_char(ch, WEAR_WIELD);
	
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You find no need for this now.\n\r",ch);
		return;
	}
	
	if (ch->mana < 2500)
	{
		send_to_char("You have not the 2500 mana to use this.\n\r",ch);
		return;
	}
	
	if ((katana == NULL) || (katana->pIndexData->vnum != 33176))
	{
		send_to_char("Without a proper katana the energy will destroy your weapon.\n\r",ch);
		return;
	}
	ch->mana -= 2500;
	send_to_char("You channel mystical energy into your katana.\n\r",ch);
	do_say(ch,"#b#cL#CI#cG#CH#cT#CN#cI#CN#cG #nSWORD!");
	send_to_char("A humongous bolt of lightning blasts from your katana.\n\r",ch);
	act("A humongous bolt of lightning blasts from $n's katana!", ch, NULL, NULL, TO_ROOM);

	if (number_range(0,100) < 16)
	{
		act("Your lightning blast flies past $N and is absorbed by the ground.", ch, NULL, victim, TO_CHAR);
		act("The lightning blast slams into the ground and is absorbed.", ch, NULL, NULL, TO_ROOM);
		return;
	}

	dmg = number_range(ch->wpn[1], ch->wpn[1] * 3);
	
	xprintf(buf,"Your lightning blast slams into $N's and crackles around $S body sending them sprawling. #R[#C#b %d #n#R]#n", dmg);
		act(buf, ch, NULL, victim, TO_CHAR);
	xprintf(buf,"$n's lightning blast slams into you and crackles around your body sending you sprawling. #R[#C#b %d #n#R]#n", dmg);
		act(buf, ch, NULL, victim, TO_VICT);
		act("$n's lightning blast slams into $N's and crackles around $S body sending $N sprawling.", ch, NULL, victim, TO_NOTVICT);
	hurt_person(ch, victim, dmg);	
	WAIT_STATE(ch, (PULSE_VIOLENCE * 3));
	if (victim->position > POS_STUNNED)
		victim->position = POS_STUNNED;
	return;
}

void fire_sword (CHAR_DATA *ch)
{
	//Flame Cone Sword sets victim aflame, and is VERY likely to hit. More Expense, Low Dmg
	CHAR_DATA *victim;
	OBJ_DATA *katana;
	char buf[MAX_STRING_LENGTH];
	int dmg;
	
	if (IS_NPC(ch))	return;

	katana = get_eq_char(ch, WEAR_WIELD);
	
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You find no need for this now.\n\r",ch);
		return;
	}
	
	if (ch->mana < 3500)
	{
		send_to_char("You have not the 3500 mystical power to use this.\n\r",ch);
		return;
	}
	
	if ((katana == NULL) || (katana->pIndexData->vnum != 33176))
	{
		send_to_char("Without a proper katana the energy will destroy your weapon.\n\r",ch);
		return;
	}
	ch->mana -= 3500;
	send_to_char("You channel mystical energy into your katana.\n\r",ch);
	do_say(ch,"#rF#RIR#rE #nSWORD!");
	send_to_char("A tremendous cone of flame shoots from your katana.\n\r",ch);
	act("A tremendous cone of flame shoots from $n's katana!", ch, NULL, NULL, TO_ROOM);

	if (number_range(0,100) < 5)
	{
		act("Your cone of flame flashes past $N and scorches the ground.", ch, NULL, victim, TO_CHAR);
		act("The cone of flame flashes past you and scorches the ground.", ch, NULL, victim, TO_VICT);
		act("The cone of flame flashes past $N and scorches the ground.", ch, NULL, victim, TO_NOTVICT);
		return;
	}

	dmg = number_range(ch->wpn[1], ch->wpn[1] * 2);
	
	xprintf(buf,"Your cone of flame engulfs $N's in a sheet of flame. #R[#R#b %d #n#R]#n", dmg);
	act(buf, ch, NULL, victim, TO_CHAR);
	xprintf(buf,"$n's cone of flame engulfs you in a sheet of flame. #R[#R#b %d #n#R]#n", dmg);
	act(buf, ch, NULL, victim, TO_VICT);
	act("$n's engulfs $N's in a sheet of flame.", ch, NULL, victim, TO_NOTVICT);
	hurt_person(ch, victim, dmg);	
	WAIT_STATE(ch, (PULSE_VIOLENCE * 3));
	if (!IS_SET(victim->affected_by, AFF_FLAMING))
		SET_BIT(victim->affected_by, AFF_FLAMING);
	return;
}

void ice_sword (CHAR_DATA *ch)
{
	//Frost Sword can freeze victims solid, and is highly damaging, and is likely to hit. Incredibly Expensive
	CHAR_DATA *victim;
	OBJ_DATA *katana;
	char buf[MAX_STRING_LENGTH];
	char spc[MAX_STRING_LENGTH];
	char spcV[MAX_STRING_LENGTH];
	int dmg;
	bool froze = FALSE;
	
	if (IS_NPC(ch))	return;

	katana = get_eq_char(ch, WEAR_WIELD);
	
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You find no need for this now.\n\r",ch);
		return;
	}
	
	if (ch->mana < 5000)
	{
		send_to_char("You have not the 5000 mystical power to use this.\n\r",ch);
		return;
	}
	
	if ((katana == NULL) || (katana->pIndexData->vnum != 33176))
	{
		send_to_char("Without a proper katana the energy will destroy your weapon.\n\r",ch);
		return;
	}
	ch->mana -= 5000;
	send_to_char("You channel mystical energy into your katana.\n\r",ch);
	do_say(ch,"#lF#cR#LO#cS#lT #nSWORD!");
	send_to_char("A ray of frost lances out from your katana.\n\r",ch);
	act("A ray of frost lances out from $n's katana!", ch, NULL, NULL, TO_ROOM);

	if (number_range(0,100) < 10)
	{
		act("Your ray of frost shoots past $N and freezes the ground.", ch, NULL, victim, TO_CHAR);
		act("The ray of frost shoots past you and freezes the ground.", ch, NULL, victim, TO_VICT);
		act("The ray of frost shoots past $N and freezes the ground.", ch, NULL, victim, TO_NOTVICT);
		return;
	}
	
	if (number_range(0,100) > 90)
		froze = TRUE;

	if (!froze)		
	{
		xprintf(spc,"but $N recovers.");
		xprintf(spcV,"but you recover.");
	}
	else
	{
		xprintf(spc,"and $N freezes solid.");
		xprintf(spcV,"and you freeze solid.");
	}
		
	dmg = number_range(ch->wpn[1], ch->wpn[1] * 4);
	
	xprintf(buf,"Your ray of frost envelopes $N's in a block of ice, %s #R[#L#b %d #n#R]#n", spc, dmg);
	act(buf, ch, NULL, victim, TO_CHAR);
	xprintf(buf,"$n's ray of frost envelopes you in a block of ice, %s #R[#L#b %d #n#R]#n", spcV,dmg);
	act(buf, ch, NULL, victim, TO_VICT);
	xprintf(buf,"$n's ray of frost envelopes $N's in a block of ice, %s", spc);
	act(buf, ch, NULL, victim, TO_NOTVICT);
	hurt_person(ch, victim, dmg);	
	WAIT_STATE(ch, (PULSE_VIOLENCE * 3));
	if (froze)
	{
		if (!IS_AFF2(victim, AFF2_FROZEN))
			SET_AFF2(victim, AFF2_FROZEN);
	}
	return;
}

void drain_sword (CHAR_DATA *ch)
{
	//Drain Sword sucks health from a victim, and is moderately likely to hit. Avg Expense, Low Dmg
	CHAR_DATA *victim;
	OBJ_DATA *katana;
	char buf[MAX_STRING_LENGTH];
	int dmg;
	int heal;
	
	if (IS_NPC(ch))	return;

	katana = get_eq_char(ch, WEAR_WIELD);
	
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You find no need for this now.\n\r",ch);
		return;
	}
	
	if (ch->mana < 4000)
	{
		send_to_char("You have not the 4000 mystical power to use this.\n\r",ch);
		return;
	}
	
	if ((katana == NULL) || (katana->pIndexData->vnum != 33176))
	{
		send_to_char("Without a proper katana the energy will destroy your weapon.\n\r",ch);
		return;
	}
	ch->mana -= 4000;
	send_to_char("You channel mystical energy into your katana.\n\r",ch);
	do_say(ch,"#nD#0R#yA#0I#nN #nSWORD!");
	send_to_char("A wave of energy launches itself from your katana.\n\r",ch);
	act("A wave of energy launches itself from $n's katana!", ch, NULL, NULL, TO_ROOM);

	if (number_range(0,100) < 20)
	{
		act("Your wave of energy goes through $N, continues on a bit then fades.", ch, NULL, victim, TO_CHAR);
		act("The wave of energy goes through you, continues on a bit then fades.", ch, NULL, victim, TO_VICT);
		act("The wave of energy goes through $N, continues on a bit then fades.", ch, NULL, victim, TO_NOTVICT);
		return;
	}

	dmg = number_range(ch->wpn[1], ch->wpn[1] * 2);
	heal = number_range(dmg / 3, dmg / 2);
	
	xprintf(buf,"Your wave of energy enters $N's body and drains %d health painfully from $N. #R[#R#b %d #n#R]#n", heal, dmg);
	act(buf, ch, NULL, victim, TO_CHAR);
	xprintf(buf,"$n's wave of energy enters your body and drains you painfully. #R[#R#b %d #n#R]#n", dmg);
	act(buf, ch, NULL, victim, TO_VICT);
	act("$n's wave of energy enters $N's body causing $N to look weak.", ch, NULL, victim, TO_NOTVICT);
	hurt_person(ch, victim, dmg);
	ch->hit += heal;
	if (ch->hit > ch->max_hit)	ch->hit = ch->max_hit;	
	WAIT_STATE(ch, (PULSE_VIOLENCE * 3));
	return;
}

void soul_sword (CHAR_DATA *ch)
{
	//Soul Sword can rarely kill victims outright, and does 1 dmg, and is VERY unlikely to hit. UNGODLY Expensive
	CHAR_DATA *victim;
	OBJ_DATA *katana;
	char buf[MAX_STRING_LENGTH];
	int thebonus = 0;
	
	if (IS_NPC(ch))		return;

	katana = get_eq_char(ch, WEAR_WIELD);
	
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You find no need for this now.\n\r",ch);
		return;
	}
	
	if (IS_NPC(victim))		thebonus = 45;
		else				thebonus = 7;
	if (ch->mana < 30000)
	{
		send_to_char("You have not the 30000 mana power to use this.\n\r",ch);
		return;
	}
	
	if ((katana == NULL) || (katana->pIndexData->vnum != 33176))
	{
		send_to_char("Without a proper katana the energy will destroy your weapon.\n\r",ch);
		return;
	}
	ch->mana -= 30000;
	send_to_char("You channel mystical energy into your katana.\n\r",ch);
	do_say(ch,"#0SOUL #nSWORD!");
	
	act("A bolt of light shoots out of your katana and into $N",ch, NULL, victim, TO_CHAR);
	act("A bolt of light shoots out of $n's katana and into you!", ch, NULL, victim, TO_VICT);
	act("A bolt of light shoots out of $n's katana and into $N!", ch, NULL, victim, TO_NOTVICT);

	if (number_range(0,250) > (4 + thebonus))  // Was .5% now it's 5%/20%
	{
		act("Your attack was ineffective.", ch, NULL, victim, TO_CHAR);
		act("$n looks confused as nothing happens.", ch, NULL, NULL, TO_ROOM);
		multi_hit( ch, victim, TYPE_UNDEFINED );
		WAIT_STATE(ch, (PULSE_VIOLENCE * 5));
		return;
	}
	
	xprintf(buf,"$N screams as $E is ripped apart by the dark energy. #R[#y#b MORTAL #n#R]#n");
	act(buf, ch, NULL, victim, TO_CHAR);
	xprintf(buf,"$n looks on as you start screaming while dark energy rips you apart from the inside out. #R[#y#b MORTAL #n#R]#n");
	act(buf, ch, NULL, victim, TO_VICT);
	send_to_char("You crumple to the ground.\n\r", victim);
	act("$n looks on as $N starts screaming.  Weakened $N crumples to the ground.", ch, NULL, victim, TO_NOTVICT);
	WAIT_STATE(ch, (PULSE_VIOLENCE * 5));
	victim->hit = -10;
	hurt_person(ch, victim, 1);
	return;
}

void do_sword(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_STRING_LENGTH];
	bool lit = FALSE;
	bool fire = FALSE;
	bool ice = FALSE;
	bool drain = FALSE;
	bool soul = FALSE;
	
	argument = one_argument(argument, arg);
	
	if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_SAMURAI))
	{
		do_rand_typo(ch,"");
		return;
	}
	
	  if (arg[0] == '\0')
  {
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
	send_to_char("                           -= Samurai Sword Skills =-\n\r",ch);
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
	if (!IS_SAMPOWER(ch,SAM_SWORD))
		send_to_char("None",ch);
	if (IS_SAMPOWER(ch,SAM_SWORD))
		send_to_char("     Sword: ",ch);
	if (IS_SAMPOWER(ch,SAM_LITSWORD))
		send_to_char("#YLightning#n",ch);
	if (IS_SAMPOWER(ch,SAM_FIRESWORD))
		send_to_char(", #RFire#n",ch);
	if (IS_SAMPOWER(ch,SAM_ICESWORD))
		send_to_char(", #CIce#n",ch);
	if (IS_SAMPOWER(ch,SAM_DRAINSWORD))
		send_to_char(", #0Drain#n",ch);
	if (IS_SAMPOWER(ch,SAM_SOULSWORD))
		send_to_char(", #WSoul#n.",ch);
	send_to_char("\n\r",ch);
	if (IS_SAMPOWER(ch,SAM_SWORD))
	send_to_char("     Type sword <skill> to use that skill.\n\r",ch);
	send_to_char("\n\r",ch);
    return;
  }
	
	if (!IS_SAMPOWER(ch,SAM_SWORD))
	{
		send_to_char("You have not yet learned how to do sword techniques.\n\r",ch);
		return;
	}
	
	if (!str_cmp(arg, "lightning"))		lit = TRUE;
	if (!str_cmp(arg, "fire"))			fire = TRUE;
	if (!str_cmp(arg, "ice"))			ice = TRUE;
	if (!str_cmp(arg, "drain"))			drain = TRUE;
	if (!str_cmp(arg, "soul"))			soul = TRUE;
	
	if ((!lit) && (!fire) && (!ice) && (!drain) && (!soul))
	{
		send_to_char("That is not a technique, try lightning, fire, ice, drain or soul.\n\r",ch);
		return;
	}

	if (lit)
	{
		if (!IS_SAMPOWER(ch,SAM_LITSWORD))
		{
			send_to_char("You have not yet learned this technique.\n\r",ch);
			return;
		}
		else
		{
			lightning_sword(ch);
			return;
		}
	}
	
	if (fire)
	{
		if (!IS_SAMPOWER(ch,SAM_FIRESWORD))
		{
			send_to_char("You have not yet learned this technique.\n\r",ch);
			return;
		}
		else
		{
			fire_sword(ch);
			return;
		}
	}
	
	if (ice)
	{
		if (!IS_SAMPOWER(ch,SAM_ICESWORD))
		{
			send_to_char("You have not yet learned this technique.\n\r",ch);
			return;
		}
		else
		{
			ice_sword(ch);
			return;
		}
	}
	
	if (drain)
	{
		if (!IS_SAMPOWER(ch,SAM_DRAINSWORD))
		{
			send_to_char("You have not yet learned this technique.\n\r",ch);
			return;
		}
		else
		{
			drain_sword(ch);
			return;
		}
	}
	
	if (soul)
	{
		if (!IS_SAMPOWER(ch,SAM_SOULSWORD))
		{
			send_to_char("You have not yet learned this technique.\n\r",ch);
			return;
		}
		else
		{
			soul_sword(ch);
			return;
		}
	}
}
