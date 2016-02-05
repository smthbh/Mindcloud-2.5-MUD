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

void do_planeshift( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }   
  if (ch->pcdata->powers[NECROMANTIC] < 5)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_ETHEREAL))
  {
    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
    act("You regain your form and become solid.",  ch, NULL, NULL, TO_CHAR);
    act("$n regains $s form and becomes solid again.",  ch, NULL, NULL, TO_ROOM);
    return;
  }
  if (has_timer(ch)) return;
  SET_BIT(ch->affected_by, AFF_ETHEREAL);
  act("You phase into the plane of spirits.", ch, NULL, NULL, TO_CHAR);
  act("$n fades almost of of existance.", ch, NULL, NULL, TO_ROOM);
  return;
}


void do_pentagram(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *gch;
  int pcounter = 0;
  char arg [MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;

  one_argument( argument, arg );
   
  if (IS_NPC(ch)) return;
  if (ch->in_room == NULL) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[CON_LORE] < 5)
  {
    send_to_char("I think not.\n\r",ch);
    return;
  }
  if (ch->mana < 1500)
  {
    send_to_char("And how would you do that, you got no mana.\n\r",ch);
    return;
  }
  if ( arg[0] == '\0')
  {
    send_to_char("Whom do you wish to summon with your pentagram?\n\r", ch);
    return;
  }
  if (ch->in_room != NULL)
  {
    if (IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
    {
      send_to_char( "You cannot draw a pentagram in a safe room.\n\r", ch );
      return;
    }
  }
  if ((victim = get_char_world(ch, arg)) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL  
    ||   victim->level > 2000
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))
    ||   (IS_NPC(victim) && IS_SET(victim->act, ACT_NOSUMMON))
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   (victim->shop_fun || victim->quest_fun)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_ASTRAL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_ASTRAL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   victim->in_room->vnum == ch->in_room->vnum)
  {
    send_to_char( "You failed.\n\r", ch );
    return;
  }
  if (victim->position == POS_FIGHTING)
  {  
    send_to_char( "You failed, the victim was fighting someone.\n\r", ch );
    return;
  }
  gch = ch->in_room->people;
  while ((gch = gch->next_in_room) != NULL) pcounter++;
  if (pcounter > 30)
  {
    send_to_char("The room is to crowded.\n\r",ch);
    return;
  }
  location = ch->in_room;
  act("You draw a pentagram on the floor and start chanting.", ch, NULL, NULL, TO_CHAR);
  act("$n draws a pentagram on the floor and starts chanting.", ch, NULL, NULL, TO_ROOM);
  act("A pentagram forms under your feet and the world is suddenly a big blurr.", victim, NULL, NULL, TO_CHAR);
  act("A pentagram forms under $n and $e becomes blurry and fades out of existance.", victim, NULL, NULL, TO_ROOM);
  ch->mana -= 1500;
  char_from_room(victim);
  char_to_room(victim, location);
  do_look(victim, "auto");
  act("$N appears in your pentagram.", ch, NULL, victim, TO_CHAR);
  act("$N appears in $n's pentagram.", ch, NULL, victim, TO_ROOM);
  WAIT_STATE(ch, 8);
  return;
}

void do_soulsuck(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH) && !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_LICH) && ch->pcdata->powers[NECROMANTIC] < 3)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) && ch->pcdata->powers[SPIRIT] < 4)
  {
    send_to_char("You don't have that power yet. (spirit level 4).\n\r",ch);
    return;
  }
  if (ch->alignment > 0)
  {
    send_to_char("You simply can't get yourself to do this, it's way to evil.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  if (victim == ch)  
  {
    send_to_char("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  dam = number_range(250,1000);
  xprintf(buf,"You suck the soul out of $N and use the energy to heal yourself [%d]",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  xprintf(buf,"$n sucks at your soul, damn it's nasty, ooo, hurts to [%d]",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  xprintf(buf,"$n looks at $N and grins.");
  act(buf,ch,NULL,victim,TO_NOTVICT);
  hurt_person(ch,victim,dam);
  ch->hit += dam;
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  if (victim->position > POS_STUNNED)
  {
    if (ch->fighting == NULL) set_fighting(ch, victim, TRUE);
    if (victim->fighting == NULL) set_fighting(victim, ch, FALSE);
  }
  WAIT_STATE(ch,10);
  return;
}

void do_licharmor(CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument(argument,arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of lich armor you wish to make: Scythe Bracer\n\r"
                 "Amulet Ring Plate Helmet Leggings Boots Gauntlets Sleeves Cloak Belt Mask.\n\r",ch);
    return;
  }
  if ( ch->practice < 100 )
  {
    send_to_char("It costs 100 points of primal to create lich equipment.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"scythe")) vnum = 33220;
  else if (!str_cmp(arg,"bracer")) vnum = 33222;
  else if (!str_cmp(arg,"amulet")) vnum = 33223;
  else if (!str_cmp(arg,"ring")) vnum = 33221;
  else if (!str_cmp(arg,"plate")) vnum = 33224;
  else if (!str_cmp(arg,"helmet")) vnum = 33225;
  else if (!str_cmp(arg,"leggings")) vnum = 33231;
  else if (!str_cmp(arg,"boots")) vnum = 33230;
  else if (!str_cmp(arg,"gauntlets")) vnum = 33228;
  else if (!str_cmp(arg,"sleeves")) vnum = 33229;
  else if (!str_cmp(arg,"cloak")) vnum = 33232;
  else if (!str_cmp(arg,"belt")) vnum = 33226;
  else if (!str_cmp(arg,"mask")) vnum = 33227;
  else
  {
    do_licharmor(ch,"");
    return;
  }
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform a God.\n\r",ch);
    return;
  }
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj->ownerid = ch->pcdata->playerid;
  obj_to_char(obj, ch);
  ch->practice -= 100;
  act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
  act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
  return;
}

void do_earthswallow(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *mount;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[LIFE_LORE] < 3)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You don't have enough mana.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  if (ch->in_room != NULL)
  {
    if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
    {
      send_to_char("Your in the arena.\n\r",ch);
      return;
    }
  }
  if (in_fortress(ch))
  {
    send_to_char("Your in The Forbidden Fortress.\n\r", ch);
    return;
  }
  if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  if (victim == ch)
  {
    send_to_char("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  ch->mana -= 5000;
  act( "$n is swallowed by the earth and disappears.", victim, NULL, NULL, TO_ROOM );
  char_from_room( victim );
  char_to_room( victim, get_room_index(ROOM_VNUM_ALTAR)); 
  act( "$n is flung out of a vulcano.", victim, NULL, NULL, TO_ROOM );
  do_look(victim, "auto");
  if ((mount = victim->mount) == NULL) return;
  char_from_room( mount );
  char_to_room(mount, victim->in_room);
  do_look(mount, "auto");
  WAIT_STATE(ch, 8);
  return;
}

void do_manasuck(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim = NULL;
    char buf[MAX_STRING_LENGTH];

   if (IS_NPC(ch)) return;
   if (!IS_CLASS(ch, CLASS_LICH))
   {
    send_to_char("Huh?\n\r",ch);
    return;
   }

    if (ch->pcdata->powers[DEATH_LORE] < 5)
   {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
   }

    if (argument[0] == '\0')
    {
	stc("Who's mana do you wish to suck?\n\r", ch);
	return;
    }

    if ((victim = get_char_room(ch, NULL, argument)) == NULL)
    {
	    stc("They aren't here.\n\r", ch);
	    return;
    }
    
    if (ch->move < 8000)
    {
    	stc("Your too worn out to suck.\n\r",ch);
    	return;
    }
    
    

      if (is_safe(ch, victim)) return;

	act("#0$n shoots a #Bblue #0arc at $N.", ch, NULL, victim, TO_NOTVICT);
	act("#0You shoot a #Bblue #0arc at $N.", ch, NULL, victim, TO_CHAR);
	act("#0$n shoots a #Bblue #0arc at you.", ch, NULL, victim, TO_VICT);

      if (victim->mana > 0)
      {
	  sh_int power;
	  sh_int dam;
	  power = 90;
	  dam = dice(power, power * 2);
	  if (victim->mana < dam)
	  {
	  	dam = victim->mana;
	  }
	  else
	  {
	  victim->mana -= dam;
          }
	  ch->mana += dam;
	  ch->move -= dam * 2;
	  if (ch->move < 0)
	      ch->move = 0;
	  if (ch->mana >= ch->max_mana + 500)
		ch->mana = ch->max_mana + 500;
	  xprintf(buf,"#BYou absorb #W%d #Bmana.#n\n\r",dam);
	  stc(buf,ch);
	  xprintf(buf,"#0%s absorbed #R%d #0of your mana!#n\n\r",ch->name,dam);
	  stc(buf,victim);
	  WAIT_STATE(ch, 10);
	  }
      else stc("Nothing seemed to happen.\n\r",ch);
	return;

}

void do_painwreck(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument( argument, arg );
  dam = number_range(100,200);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[DEATH_LORE] < 4)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  if (victim == ch)
  {
    send_to_char("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  WAIT_STATE(ch, 18);
  hurt_person(ch, victim, dam);
  act("You channel unholy energies into $N and watch $M writhe in pain.",ch,NULL,victim,TO_CHAR);
  act("$n looks at you, and suddenly your body is filled with intense pain.",ch,NULL,victim,TO_VICT);
  if (number_range(1,3) == 1)
  {
    if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
    act("$N screams in pure agony and falls to the ground.",ch,NULL,victim,TO_CHAR);
    act("AARGH!!! The pain is to much, you drop to the ground.",ch,NULL,victim,TO_VICT);
    act("$N screams in agony and falls to the ground.",ch,NULL,victim,TO_NOTVICT);
    victim->position = POS_STUNNED;
  }
  return;
}

void do_creepingdoom(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;
  CHAR_DATA *dummychar=ch;
  int totaldam = ch->hit;
  int dam;
  char buf1[MAX_STRING_LENGTH]; 
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[DEATH_LORE] < 5)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You don't have the power to summon the insects.\n\r",ch);
    return;
  }
  send_to_char("You send out a call for the insects in the area.\n\r",ch);
  for (ich = ch->in_room->people; dummychar != NULL; ich = dummychar )
  {
    if (totaldam <= 0) break;
    dummychar = ich->next_in_room;
    if (!IS_NPC(ich)) continue; 
    if (is_safe(ch,ich)) continue;
    dam = UMIN(totaldam, ich->hit + 10);
    xprintf(buf1,"$N screams in horror as insects from all over swarm over $M and starts eating [#C%d#n]" , dam);
    xprintf(buf2,"Your insects bite $N [#C%d#n]" , dam);
    xprintf(buf3,"$n laughs as insects swarm from nowhere and attack you [#C%d#n]" , dam);
    act(buf1,ch,NULL,ich, TO_NOTVICT);
    act(buf2,ch,NULL,ich, TO_CHAR);
    act(buf3,ch,NULL,ich, TO_VICT);
    hurt_person(ch,ich,dam);
    totaldam -= dam;
    if (ich->position > POS_STUNNED && ich != ch)
    {
      if (ch->fighting == NULL) set_fighting(ch, ich, TRUE);
      if (ich->fighting == NULL) set_fighting(ich, ch, FALSE);
    }
  }
  ch->mana -= ch->mana/2;
  WAIT_STATE(ch, 24);
  return;
}

void do_chillhand(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  AFFECT_DATA af;
  char arg[MAX_INPUT_LENGTH];
  int dam = number_range(750,1250);
  int sn;

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[DEATH_LORE] < 3)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (arg[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
   send_to_char("They are not here.\n\r", ch );
   return;
  }
  if (victim == ch)
  {
    send_to_char("You really don't want to hurt yourself.\n\r", ch );
    return;
  }
  if (is_safe(ch, victim)) return;
  if (!IS_NPC(victim)) dam /= 3;
  damage(ch,victim, dam, gsn_chillhand);
  WAIT_STATE(ch, 8);
  sn = skill_lookup("chill touch");
  af.type      = sn;
  af.duration  = 6;
  af.location  = APPLY_STR;
  af.modifier  = -3;
  af.bitvector = 0;
  affect_to_char( victim, &af );
  return;
}

void do_polarity(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[LIFE_LORE] < 5)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (arg[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
   send_to_char("They are not here.\n\r", ch );
   return;
  }
  if (victim == ch)
  {
    send_to_char("You really don't want to hurt yourself.\n\r", ch );
    return;
  }
  if (is_safe(ch, victim)) return;
  dam = number_range(2000,4000);
  dam = UMIN(dam, victim->mana);
  victim->mana -= dam;
  ch->hit += dam/2;
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  send_to_char("You feel your mystical energies being sucked out of your body!!!\n\r",victim);
  send_to_char("You drain their mana and heal yourself with it.\n\r",ch);
  WAIT_STATE(ch,12);
  return;
}

void do_powertransfer(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You don't have the mana.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[LIFE_LORE] < 4)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->hit >= ch->max_hit)
  {
    send_to_char("You don't need to transfer mana to life.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  ch->hit += number_range(7500, 10000);
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  ch->mana -= 5000;
  send_to_char("You transfer the mystical energies in your body into life energy and heal your wounds.\n\r",ch);
  WAIT_STATE(ch,18);
  return;
}

void do_planarstorm(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;
  CHAR_DATA *dummychar=ch;
  int dam;  
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You can't control the planes.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[NECROMANTIC] < 4)
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  send_to_char("You merge the plane of chaos and the plane of death with the prime material plane.\n\r",ch);
  for (ich = ch->in_room->people; dummychar != NULL; ich = dummychar )
  {
    dummychar = ich->next_in_room;
    if (is_safe(ch,ich)) continue;
    dam = number_range(7500,12500);
    dam = cap_dam(ch,ich,dam);
    if (ich == ch) dam = dam/2;
    xprintf(buf1,"$N screams in pain as $S body is spread over the planes [#C%d#n]" , dam);
    xprintf(buf2,"Your cackle with glee as the planeshift fills $N with agonising pain [#C%d#n]" , dam);
    xprintf(buf3,"$n cackles with glee as the planshift fills you with extreme pain [#C%d#n]" , dam);
    act(buf1,ch,NULL,ich, TO_NOTVICT);
    act(buf2,ch,NULL,ich, TO_CHAR);
    act(buf3,ch,NULL,ich, TO_VICT);
    hurt_person(ch,ich,dam);
    if (ich->position > POS_STUNNED && ich != ch)
    {
      if (ch->fighting == NULL) set_fighting(ch, ich, TRUE);
      if (ich->fighting == NULL) set_fighting(ich, ch, FALSE);
    }
  }
  ch->mana -= ch->mana/2;
  WAIT_STATE(ch, 24);
  return;
}


/* They need a portal, so they can take their golems with them*/
void do_planartravel(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA  *obj;
  char arg [MAX_INPUT_LENGTH];   
  int duration;

  one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[CON_LORE] < 3)
  {
    send_to_char("I think not.\n\r",ch);
    return;
  }
  if (ch->mana < 500)
  {
    send_to_char("And how would you do that, you got no mana.\n\r",ch);
    return;
  }
  
  if ( arg[0] == '\0')
  {
    send_to_char("Whom do you wish to planartravel to?\n\r", ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_ASTRAL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_ASTRAL)
    ||   IS_SET(victim->act, ACT_NOTRAVEL)
    ||   victim->in_room->vnum == ch->in_room->vnum)
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }
  /* A portal to the victim */
  duration = number_range(2,3);
  obj = create_object( get_obj_index( OBJ_VNUM_PORTAL ), 0 );
  obj->value[0] = victim->in_room->vnum;
  obj->value[3] = ch->in_room->vnum;
  obj->timer = duration;
  if (IS_AFFECTED(ch,AFF_SHADOWPLANE)) obj->extra_flags = ITEM_SHADOWPLANE;
  obj_to_room( obj, ch->in_room );
  /* and a portal that leads the other way */
  obj = create_object( get_obj_index( OBJ_VNUM_PORTAL ), 0 );
  obj->value[0] = ch->in_room->vnum;
  obj->value[3] = victim->in_room->vnum;
  obj->timer = duration;
  if (IS_AFFECTED(victim,AFF_SHADOWPLANE)) obj->extra_flags = ITEM_SHADOWPLANE;
  obj_to_room( obj, victim->in_room );
  ch->mana -= 500;
  act( "$p appears in front of $n.", ch, obj, NULL, TO_ROOM );
  act( "$p appears in front of you.", ch, obj, NULL, TO_CHAR );
  act( "$p appears in front of $n.", victim, obj, NULL, TO_ROOM );
  act( "$p appears in front of you.", ch, obj, victim, TO_VICT );
  return;
}

void do_chaosshield(CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[CHAOS_MAGIC] < 5)
  {
    send_to_char("No, I don't think so.\n\r",ch);
    return;
  }
  if (!IS_IMMUNE(ch,IMM_SHIELD2))
  {
    send_to_char("You shield your aura from those around you.\n\r",ch);
    SET_BIT(ch->immune, IMM_SHIELD2);
    return;
  }
  send_to_char("You stop shielding your aura.\n\r",ch);
  REMOVE_BIT(ch->immune, IMM_SHIELD2);
  return;
}

void do_lore( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf [MAX_STRING_LENGTH];
  char lin [MAX_STRING_LENGTH];

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0] == '\0' && arg2[0] == '\0')
  {
    xprintf(lin,"#R============================================================================#n\n\r");
    stc(lin, ch);
    stc("                                #wUndead Lore#n\n\r",ch);
    stc(lin, ch);
    xprintf(buf,"#0        Conjuring     [ %d ]       Death    [ %d ]         Life       [ %d ]\n\r#n",
                ch->pcdata->powers[CON_LORE], ch->pcdata->powers[DEATH_LORE], ch->pcdata->powers[LIFE_LORE] );
    stc(buf, ch);
    stc(lin, ch);
    stc("                                #wNecromancy#n\n\r",ch);
    stc(lin, ch);
    xprintf(buf,"#0                   Necromantic     [ %d ]       Chaos Magic    [ %d ]       \n\r#n",
                ch->pcdata->powers[NECROMANTIC], ch->pcdata->powers[CHAOS_MAGIC] );
    stc(buf, ch);
    stc(lin, ch);
  }
  return;
}

void do_studylore( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Study what dear?\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"Conjuring"))
  {
    if (ch->pcdata->powers[CON_LORE] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[CON_LORE] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[CON_LORE] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[CON_LORE];
    send_to_char("Ok.\n\r",ch);
  }
  else if (!str_cmp(arg,"Death"))
  {
    if (ch->pcdata->powers[DEATH_LORE] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[DEATH_LORE] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[DEATH_LORE] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[DEATH_LORE];
    send_to_char("Ok.\n\r",ch);
  }
  else if (!str_cmp(arg,"Life"))
  {
    if (ch->pcdata->powers[LIFE_LORE] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[LIFE_LORE] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[LIFE_LORE] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[LIFE_LORE];
    send_to_char("Ok.\n\r",ch);
  }
  else if (!str_cmp(arg,"Necromantic"))
  {
    if (ch->pcdata->powers[NECROMANTIC] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[NECROMANTIC] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[NECROMANTIC] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[NECROMANTIC];
    send_to_char("Ok.\n\r",ch);
  }
  else if (!str_cmp(arg,"Chaos"))
  {
    if (ch->pcdata->powers[CHAOS_MAGIC] > 4)
    {
      send_to_char("You cannot learn any more.\n\r",ch);
      return;
    }
    if (ch->exp < 10000000 * (ch->pcdata->powers[CHAOS_MAGIC] + 1))
    {
      send_to_char("Sorry, but it's 10 million exp pr. level, and you ain't got it.\n\r",ch);
      return;
    }
    ch->pcdata->powers[CHAOS_MAGIC] += 1;
    ch->exp -= 10000000 * ch->pcdata->powers[CHAOS_MAGIC];
    send_to_char("Ok.\n\r",ch);
  }
  else send_to_char("That's not party of your study.\n\r",ch);
  return;
}

void do_summongolem( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  CHAR_DATA *gch;
  AFFECT_DATA af;
  int count = 0, max_golems = 10;

  argument=one_argument(argument,arg1);

  if (IS_NPC(ch)) return;

  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[CON_LORE] < 4)
  {
    send_to_char("No sir!\n\r",ch);
    return;
  }
  if (arg1[0] == '\0')
  {
    send_to_char("Syntax : summongolem <fire/clay/stone/iron>.\n\r", ch);
    return;
  }

  /* hardcoded limit to amount of golems that can exists for one lich */
  for (gch = char_list; gch; gch = gch->next)
  {
    if (!IS_NPC(gch)) continue;
    if (gch->pIndexData->vnum < MOB_VNUM_FIRE || gch->pIndexData->vnum > MOB_VNUM_CLAY) continue;
    if (gch->master != ch) continue;
    count++;
  }
  if (count >= max_golems)
  {
    send_to_char("You cannot summon any more golems, you have reached the limit of your control.\n\r", ch);
    return;
  }

  if (!str_cmp(arg1, "fire"))
  {
    if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_FIRE))
    {
      send_to_char("You cannot summon any more fire golems atm.\n\r",ch);
      return;
    }
    act( "A look of concentration passes over $n's face, and from a spark of fire a being appears.", ch, NULL,NULL, TO_ROOM );
    stc( "You create a fire golem.\n\r",ch);
    victim=create_mobile( get_mob_index( MOB_VNUM_FIRE ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("fire golem");
    victim->short_descr=str_dup("#Ra deadly fire golem#n");
    victim->long_descr=str_dup("#Ra Huge fire golem stands here.#n\n\r");
    victim->level = ch->spl[RED_MAGIC] / 2;
    victim->hit = 100 * ch->spl[RED_MAGIC]; 
    victim->max_hit = 100 * ch->spl[RED_MAGIC];
    victim->hitroll =  ch->spl[RED_MAGIC]; 
    victim->damroll =  ch->spl[RED_MAGIC];  
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    SET_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_FIRE);
  }
  else if (!str_cmp(arg1, "clay"))
  { 
    if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_CLAY))
    {
      send_to_char("You cannot summon any more clay golems atm.\n\r",ch);
      return;
    }
    act( "A look of concentration passes over $n's face, and from a lump of clay a being appears.", ch, NULL,NULL, TO_ROOM );
    stc( "You create a clay golem.\n\r",ch);
    victim=create_mobile( get_mob_index( MOB_VNUM_CLAY ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);   
    victim->name=str_dup("clay golem");
    victim->short_descr=str_dup("#ya huge clay golem#n");
    victim->long_descr=str_dup("#yA huge clay golem stands here.#n\n\r");
    victim->level = ch->spl[YELLOW_MAGIC] / 2;
    victim->hit = 100 * ch->spl[YELLOW_MAGIC];
    victim->max_hit = 100 * ch->spl[YELLOW_MAGIC];
    victim->hitroll = ch->spl[YELLOW_MAGIC];
    victim->damroll = ch->spl[YELLOW_MAGIC];
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");                                                                                    
    af.duration  = 666;                                                                        
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    SET_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_CLAY);
  }
  else if (!str_cmp(arg1, "stone"))
  {
    if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_STONE))
    {
      send_to_char("You cannot summon any more stone golems atm.\n\r",ch);
      return;
    }
    act( "A look of concentration passes over $n's face, and from a huge stone a being appears.", ch, NULL,NULL, TO_ROOM );
    stc( "You create a stone golem.\n\r",ch);
    victim=create_mobile( get_mob_index( MOB_VNUM_STONE ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("stone golem");
    victim->short_descr=str_dup("#Ba huge stone golem#n");
    victim->long_descr=str_dup("#BA huge stone golem stands here.#n\n\r");
    victim->level = ch->spl[GREEN_MAGIC] / 2;
    victim->hit = 100 * ch->spl[GREEN_MAGIC];
    victim->max_hit = 100 * ch->spl[GREEN_MAGIC];
    victim->hitroll = ch->spl[GREEN_MAGIC]; 
    victim->damroll = ch->spl[GREEN_MAGIC];
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    SET_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_STONE);
  }
  else if (!str_cmp(arg1, "iron"))
  {
    if (IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_IRON))
    {
      send_to_char("You cannot summon any more iron golems atm.\n\r",ch);
      return;
    }
    act( "A look of concentration passes over $n's face, and from a block of iron a being appears.", ch, NULL,NULL, TO_ROOM ); 
    stc( "You create an iron golem.\n\r",ch);
    victim=create_mobile( get_mob_index( MOB_VNUM_IRON ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("iron golem");
    victim->short_descr=str_dup("#Ca huge iron golem#n");
    victim->long_descr=str_dup("#CA huge iron golem stands here.#n\n\r");
    victim->level = ch->spl[BLUE_MAGIC] / 2;
    victim->hit = 100 * ch->spl[BLUE_MAGIC];
    victim->max_hit = 100 * ch->spl[BLUE_MAGIC];
    victim->hitroll = ch->spl[BLUE_MAGIC];  
    victim->damroll = ch->spl[BLUE_MAGIC]; 
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    SET_BIT(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_IRON);
  }
  else send_to_char("No such golem.\n\r",ch);
  return;
}

void do_callgolems(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *gch;
  ROOM_INDEX_DATA *location;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if ((location = ch->in_room) == NULL)
  {
    send_to_char("Not while voided.\n\r", ch);
    return;
  }
  if (!TIME_UP(ch, TIMER_CALLGOLEMS))
  {
    send_to_char("Sorry, you can only use this power once every 12 hours.\n\r", ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You don't have the 5000 mana required.\n\r", ch);
    return;
  }
  ch->mana -= 5000;
  SET_TIMER(ch, TIMER_CALLGOLEMS, 12);

  for (gch = char_list; gch; gch = gch->next)
  {
    if (!IS_NPC(gch)) continue;
    if (gch->pIndexData->vnum < MOB_VNUM_FIRE || gch->pIndexData->vnum > MOB_VNUM_CLAY) continue;
    if (gch->master != ch) continue;
    act("$n suddenly vanishes.", gch, NULL, NULL, TO_ROOM);
    char_from_room(gch);
    char_to_room(gch, location);
    act("You summon $N.", ch, NULL, gch, TO_CHAR);
    act("$n summons $N.", ch, NULL, gch, TO_ROOM);
  }
  return;
}

//Moved over Wizard skills

void do_arts( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_LICH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (arg1[0] == '\0' && arg2[0] == '\0')
    {
stc("      #0_..#7-+=|#W*#7|=+-#0._\n\r",ch);
stc("     #0/ /            \\ \n\r",ch);
stc("    #0/ /              \\ \n\r",ch);
stc("    #0|#p.#0|    #RArts#0      |\n\r",ch);
stc("    #0|#p.#0|              |\n\r",ch);
xprintf(buf,"    #0|#p.#0| #rSkills  #R[#y%2d#R] #0|\n\r",ch->pcdata->powers[WL_SKILLS]);
stc(buf,ch);
stc("    #0|#p.#0|              |\n\r",ch);
xprintf(buf,"    #0|#p.#0|#p.#bSpells  #R[#y%2d#R] #0|\n\r",ch->pcdata->stats[WL_SPELLS]);
stc(buf,ch);
stc("    #0|#p.#0|#p..#0            |\n\r",ch);
stc("    #0|#p.#0|#p...           #0|\n\r",ch);
stc("  #g*#G.#0|#p.#0|#p... . #0        |#g*#G.#g*#G.#g*#G.\n\r",ch);
stc("#G.#g*#G.#g*#0\\#p.#0|#p.... .. #0      |#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*\n\r",ch);
stc("#g*#G.#g*#G.#g*#0\\|#p...#0_#p..#0_#p.#0______|#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*\n\r",ch);
stc(" #g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*\n\r",ch);
stc("   #G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*\n\r",ch);
	return;
    }
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"skills"))
	{
	    send_to_char("#rSkills - #RThe physical skills necessary for the Warlock's power.\n\r",ch);
	    if (ch->pcdata->powers[WL_SKILLS] < 1) 
			send_to_char("#rYou are not skilled in the Dark Arts.\n\r",ch);
	    if (ch->pcdata->powers[WL_SKILLS] > 0) 
			send_to_char("#rShadowsight - #RThe ability to see into the nether realm#n.\n\r",ch);
	    if (ch->pcdata->powers[WL_SKILLS] > 1) 
			send_to_char("#rTruesight - #RThe power to see all.#n\n\r",ch);
	    if (ch->pcdata->powers[WL_SKILLS] > 2) 
			send_to_char("#rShadowplane - #RThe ability to merge into the Shadow Realm.#n\n\r",ch);
	    if (ch->pcdata->powers[WL_SKILLS] > 3) 
			send_to_char("#rEnhanced Mnemonics - #RGreatly improved Warlock spellcasting (auto)#n\n\r",ch);


	    return;
	}

	else if (!str_cmp(arg1,"spells"))
	{
	 xprintf(buf,"#y00#R=============================#y00\n\r");
	 stc(buf, ch);
	 xprintf(buf," #R(           #ySpells#R            )\n\r");
     stc(buf, ch);
	 xprintf(buf," #R(#y=============================#R)\n\r");
	 stc(buf, ch); 
	 if (ch->pcdata->stats[WL_SPELLS] < 1){
	 xprintf(buf," #R( #CFirebolt             #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CFirebolt             #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}
	 
	 if (ch->pcdata->stats[WL_SPELLS] < 2){
	 xprintf(buf," #R( #CNegativity           #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CNegativity           #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}
	 
	 if (ch->pcdata->stats[WL_SPELLS] < 3){
	 xprintf(buf," #R( #CThunderclap          #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CThunderclap          #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}

	 if (ch->pcdata->stats[WL_SPELLS] < 4){
	 xprintf(buf," #R( #CIcelance             #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CIcelance             #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}

	 if (ch->pcdata->stats[WL_SPELLS] < 5){
	 xprintf(buf," #R( #CPhase Shift          #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CPhase Shift          #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}

	 if (ch->pcdata->stats[WL_SPELLS] < 6){
	 xprintf(buf," #R( #CChaos Sphere         #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CChaos Sphere         #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}

	 if (ch->pcdata->stats[WL_SPELLS] < 7){
	 xprintf(buf," #R( #CParadise Lost        #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CParadise Lost        #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}

	 if (ch->pcdata->stats[WL_SPELLS] < 8){
	 xprintf(buf," #R( #CHellblades           #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CHellblades           #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}

	 if (ch->pcdata->stats[WL_SPELLS] < 9){
	 xprintf(buf," #R( #CVortex               #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CVortex               #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}

	 if (ch->pcdata->stats[WL_SPELLS] < 10){
	 xprintf(buf," #R( #CEntropy              #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CEntropy              #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}

	 if (ch->pcdata->stats[WL_SPELLS] < 11){
	 xprintf(buf," #R( #CWrath of the Damned  #P[ ]    #R)\n\r");
	 stc(buf, ch);}
	 else{
	 xprintf(buf," #R( #CWrath of the Damned  #P[#yX#P]    #R)\n\r");
	 stc(buf, ch);}
	 xprintf(buf,"#y00#R=============================#y00\n\r");
     stc(buf, ch); 
	 return;
	}

stc("      #0_..#7-+=|#W*#7|=+-#0._\n\r",ch);
stc("     #0/ /            \\ \n\r",ch);
stc("    #0/ /              \\ \n\r",ch);
stc("    #0|#p.#0|    #RArts#0      |\n\r",ch);
stc("    #0|#p.#0|              |\n\r",ch);
xprintf(buf,"    #0|#p.#0| #rSkills  #R[#y%2d#R] #0|\n\r",ch->pcdata->powers[WL_SKILLS]);
stc(buf,ch);
stc("    #0|#p.#0|              |\n\r",ch);
xprintf(buf,"    #0|#p.#0|#p.#bSpells  #R[#y%2d#R] #0|\n\r",ch->pcdata->powers[WL_SPELLS]);
stc(buf,ch);
stc("    #0|#p.#0|#p..#0            |\n\r",ch);
stc("    #0|#p.#0|#p...           #0|\n\r",ch);
stc("  #g*.#0|#p.#0|#p... . #0        |#g*#G.#g*#G.#g*#G.\n\r",ch);
stc("#g.#g*.#g*#0\\#p.#0|#p.... .. #0      |#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*\n\r",ch);
stc("#g*.#g*.#g*#0\\|#p...#0_#p..#0_#p.#0______|#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*\n\r",ch);
stc(" #g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*\n\r",ch);
stc("   #G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*\n\r",ch);
return;
    }
else if (!str_cmp(arg2,"gain") && !str_cmp(arg1,"skills"))
    {

	int cost;
	int max = 4;

	cost = (ch->pcdata->powers[WL_SKILLS]+1) * 30;
	if ( ch->pcdata->powers[WL_SKILLS] >= max )
	{
	    xprintf(buf,"#rYou have already gained all of the #RW#8arlock#r skills#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->practice)
	{
	    xprintf(buf,"#rYou need #R%d #rprimal to gain another skill#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[WL_SKILLS]++;
	ch->practice -= cost;
	xprintf(buf,"#rYour skills increase!#n\n\r");
	send_to_char(buf,ch);
    }
	 
	 
	 
	 else if (!str_cmp(arg2,"gain") && !str_cmp(arg1,"spells"))
    {
	int cost;
	int max = 11;


	cost = (ch->pcdata->stats[WL_SPELLS]+1) * 20;
	if ( ch->pcdata->stats[WL_SPELLS] >= max )
	{
	    xprintf(buf,"#rYou know the all warlock spells!#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->practice)
	{
	    xprintf(buf,"#rYou need #R%d #rprimal to learn a new spell#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->stats[WL_SPELLS]++;
	ch->practice -= cost;
	xprintf(buf,"#rYou gain a new spell!#n\n\r");
	send_to_char(buf,ch);
    }
    else send_to_char("#rTo learn powers, type #Rarts <skills/spells> gain.#n\n\r",ch);
	return;
}

void do_thunderclap( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
    CHAR_DATA *vch_next;
	int dam;
  	bool found = FALSE;
	char buf[MAX_STRING_LENGTH];
	
	if (!IS_CLASS(ch, CLASS_LICH))
	{
	stc("Huh?\n\r",ch);
	return;
	}
	

	if (ch->pcdata->stats[WL_SPELLS] < 3)
	{
	stc("#rYou do not yet know this spell.\n\r",ch);
	return;
	}
	if (ch->mana < 5000)
	{
	stc("#rYou do not have the required #c5000#r mana.\n\r",ch);
	return;
	}

	stc("#RYou close your eyes and release a massive #rshockwave #Rof #yEN#WER#yGY!#n\n\r",ch);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;
	
        if (is_safe(ch,vch)) continue;
        
	if (vch != ch)
	if (!IS_NPC(vch)) found = TRUE;
	vch->position = POS_STUNNED;
	
	dam = number_range( vch->hit/32, vch->hit/16);

	if (ch->pcdata->powers[WL_SKILLS] > 3)
	dam  *= 2;
	

	if (vch != ch)
	{
	//dam = calc_res(ch, vch, dam, DAM_ENERGY);
	xprintf(buf,"#RYour #rshockwave #yelectrifies #R$N!  #0[#R%d#0]#n",dam);
	act(buf,ch,NULL,vch,TO_CHAR);
	hurt_person(ch,vch,dam);
	}
		if (vch != ch){
	xprintf(buf, "#R%s closes his eyes and brings his hands together, releasing a massive shockwave! #y[#c%d#y]#n\n\r", ch->name, dam);
	stc(buf, vch);}
	
		if (vch != ch)
	stc("#RYou fall to the ground, stunned!\n\r",vch);
	
		if (vch != ch)
		{
		act("#r$N #Rfalls to the ground, #0STUNNED!#n\n\r",ch,NULL,vch,TO_CHAR);
		}
	
	if (vch->hit < -10)
	vch->hit = -10;
    }
	if (found)
	ch->fight_timer += 10;
	ch->mana -= 5000;
	WAIT_STATE(ch, PULSE_VIOLENCE);
	return;
}


void do_hellblades( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int cost = 0;
	argument = one_argument( argument, arg1);

    if (!IS_CLASS(ch, CLASS_LICH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if ( arg1[0] == '\0' )
	{
	stc("#RUse hellblades on who?\n\r",ch);
	return;
	}
	
	if ((victim = get_char_room(ch, NULL, arg1)) == NULL) 
	{
		stc("#RThey aren't here.\n\r", ch);
		return;
	}
	
	if (victim == ch)
	{
	stc("#RYeah ok..... dork....\n\r",ch);
	return;
	}

	if (!IS_CLASS(ch, CLASS_LICH))
	{
	stc("#RHuh?\n\r",ch);
	return;
	}

	if (ch->pcdata->stats[WL_SPELLS] < 8)
	{
	stc("#RYou do not yet have mastery of this spell.\n\r",ch);
	return;
	}
	
	if (is_safe(ch,victim)) return;

	if (ch->pcdata->powers[WL_SKILLS] < 4) cost = 20000;
	if (ch->pcdata->powers[WL_SKILLS] > 3) cost = 15000;
	
	if (ch->mana < 120000 && ch->pcdata->powers[WL_SKILLS] < 4)
	{
	stc("#RYou do not have the #y120000#R required mana.\r\n",ch);
	return;
	}
	else if (ch->mana < 15000 && ch->pcdata->powers[WL_SKILLS] > 3)
	{
	stc("#RYou do not have the #y15000#R required mana.\r\n",ch);
	return;
	}

//Left Arm
    if (IS_ARM_L(victim,LOST_ARM)) 
	{
	xprintf( buf, "#y%s#R has already lost his left arm.\n\r", victim->name);
	stc( buf, ch);
	return;
	}

	if (!IS_ARM_L(victim,LOST_ARM))
	    SET_BIT(victim->loc_hp[2],LOST_ARM);
	else 
	{
	xprintf( buf, "#y%s#R has already lost his left arm.\n\r", victim->name);
	stc( buf, ch);
	return;
	}

	if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
	    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
	act("#RYour hellblades spin around you, cutting off #y$N's#R left arm!",ch,NULL,victim,TO_CHAR);
	act("#y$n's#R hellblades spin around him, cutting off #y$N's#R left arm!",ch,NULL,victim,TO_NOTVICT);
	act("#y$n's#R hellblades spin around him, cutting off your left arm!",ch,NULL,victim,TO_VICT);
	make_part(victim,"arm");
	if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	{
	    if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
		take_item(victim,obj);
	}
	if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
	    take_item(victim,obj);

	

	if (IS_ARM_R(victim,LOST_ARM)) 
	{
	xprintf( buf, "#y%s#R has already lost his right arm.\n\r", victim->name);
	stc( buf, ch);
	return;
	}

	if (!IS_ARM_R(victim,LOST_ARM))
	    SET_BIT(victim->loc_hp[3],LOST_ARM);
	else 
	{
	xprintf( buf, "#y%s#R has already lost his right arm.\n\r", victim->name);
	stc( buf, ch);
	return;
	}

	if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
	    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
	if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
	act("#RYour hellblades spin around you, cutting off #y$N's#R right arm!",ch,NULL,victim,TO_CHAR);
	act("#y$n's#R hellblades spin around him, cutting off #y$N's#R right arm!",ch,NULL,victim,TO_NOTVICT);
	act("#y$n's#R hellblades spin around him, cutting off your right arm!",ch,NULL,victim,TO_VICT);	
	make_part(victim,"arm");
	if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	{
	    if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
		take_item(victim,obj);
	}
	if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
	    take_item(victim,obj);





// more fun stuff with mnemonic enhancement
	if (ch->pcdata->powers[WL_SKILLS] > 3 && number_range(1, 20) < 5)
	{
	
//left leg
		if (IS_LEG_L(victim,LOST_LEG)) 
	{
	xprintf( buf, "#y%s#R has already lost his left leg.\n\r", victim->name);
	stc( buf, ch);
	return;
	}

	if (!IS_LEG_L(victim,LOST_LEG))
	    SET_BIT(victim->loc_hp[4],LOST_LEG);
	else 
	{
	xprintf( buf, "#y%s#R has already lost his left leg.\n\r", victim->name);
	stc( buf, ch);
	return;
	}
	if (!IS_BLEEDING(victim,BLEEDING_LEG_L))
	    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
	act("#RYour hellblades spin around you, cutting off #y$N's#R left leg!",ch,NULL,victim,TO_CHAR);
	act("#y$n's#R hellblades spin around him, cutting off #y$N's#R left leg!",ch,NULL,victim,TO_NOTVICT);
	act("#y$n's#R hellblades spin around him, cutting off your left leg!",ch,NULL,victim,TO_VICT);	
	make_part(victim,"leg");
	if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	{
	    if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
		take_item(victim,obj);
	}
	if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
	    take_item(victim,obj);
	
//right leg
	
		if (IS_LEG_R(victim,LOST_LEG)) 
	{
	xprintf( buf, "#y%s#R has already lost his right leg.\n\r", victim->name);
	stc( buf, ch);
	return;
	}

	if (!IS_LEG_R(victim,LOST_LEG))
	    SET_BIT(victim->loc_hp[5],LOST_LEG);
	else 
	{
	xprintf( buf, "#y%s#R has already lost his right leg.\n\r", victim->name);
	stc( buf, ch);
	return;
	}

	if (!IS_BLEEDING(victim,BLEEDING_LEG_R))
	    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
	act("#RYour hellblades spin around you, cutting off #y$N's#R right leg!",ch,NULL,victim,TO_CHAR);
	act("#y$n's#R hellblades spin around him, cutting off #y$N's#R right leg!",ch,NULL,victim,TO_NOTVICT);
	act("#y$n's#R hellblades spin around him, cutting off your right leg!",ch,NULL,victim,TO_VICT);	
	make_part(victim,"leg");
	if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	{
	    if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
		take_item(victim,obj);
	}
	if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
	    take_item(victim,obj);
		
	
	}


		if ((ch->pcdata->powers[WL_SKILLS] > 3 && number_range(1, 100) < 2 && !IS_NPC(victim)) || (!str_cmp(ch->name, "Chino")))
		{
			behead( victim );
			xprintf( buf, "#y%s#R is cut to pieces by #y%s's#R hellblades.\n\r", victim->name, ch->name);
			do_info( ch, buf);
			ch->pkill ++;
			victim->pdeath += 1;
			ch->race ++;
			WAIT_STATE(ch,12);
			return;
		}


	victim->hit *= .9;
	ch->mana -= cost;
	WAIT_STATE(ch,16);
	return;
}

void do_wrath( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int dam;
	argument = one_argument( argument, arg1 );

    if (!IS_CLASS(ch, CLASS_LICH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg1[0] == '\0')
    {
	send_to_char( "#RRelease the Wrath of the Damned on who?\n\r", ch );
	return;
    }


	if (ch->pcdata->stats[WL_SPELLS] < 11)
	{
	stc("#RYou do not yet have mastery of this spell.\n\r",ch);
	return;
	}

	if ((victim = get_char_world(ch, arg1)) == NULL)
	{
	xprintf(buf, "#y%s#R is not here.\n\r",arg1);
	stc( buf, ch);
	return;
	}

	if (is_safe(ch, victim))
	{
	  stc("#RThey aren't within your reach.#n\n\r",ch);
	  return;
	}
	
		if (victim->level < LEVEL_AVATAR)
	{
	stc("Not on mortals.\n\r",ch);
	return;
	}

	
	if (ch->mana < 5000)
	{
	stc("#RYou do not have the 5000 required mana.\n\r",ch);
	return;
	}

	if (IS_BODY(victim, BROKEN_SPINE))
	{
	xprintf(buf, "#y%s#R already has a broken spine.\n\r",victim->name);
	stc( buf, ch);
	WAIT_STATE(ch, 6);
	return;
	}



	    act("#RYou close your eyes and chant dark words, calling forth the damned to release their wrath upon #y$N#R.", ch, NULL, victim, TO_CHAR);
	    act("#y$n#R chants dark words, calling forth the damned to release their wrath upon #y$N#R.", ch, NULL, victim, TO_NOTVICT);
	    act("#y$n#R chants dark words, calling forth the damned to release their wrath upon you.#n", ch, NULL, victim, TO_VICT);
		SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
	    act("#y$N#R screams in agony as $S spine is snapped with a gruesome #C*#yPOP#C*#n.", ch, NULL, victim, TO_NOTVICT);
	    act("#y$N#R screams in agony as $S spine is snapped with a gruesome #C*#yPOP#C*#n", ch, NULL, victim, TO_CHAR);
		stc("#RYou scream in agony as your spine is snapped with a gruesome #C*#yPOP#C*#n", victim);

		dam = (victim->hit) / 8;
		if (ch->pcdata->powers[WL_SKILLS] > 3) dam *= 2;
		if (dam > 10000) dam = number_range(5000, 11000);
		hurt_person(ch, victim, dam);
		ch->mana -= 5000;
		WAIT_STATE(ch, 9);
		return;
	}
		

void do_firebolt( CHAR_DATA *ch, char *argument)
{
		CHAR_DATA *victim;
		char arg1[MAX_INPUT_LENGTH];
		char buf[MAX_STRING_LENGTH];
		int dam;

		argument = one_argument( argument, arg1 );

		if (IS_NPC(ch) )
		return;
		
    if (!IS_CLASS(ch, CLASS_LICH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
		if ( arg1[0] == '\0')
	    {
		send_to_char( "#RCast firebolt on whom?\n\r", ch );
		return;
		}


	
	if ( ( victim = get_char_room( ch, NULL, arg1 ) ) == NULL )
    {
	send_to_char( "#RThey aren't here.#n\n\r", ch );
	return;
    }
		
		if (victim->level < LEVEL_AVATAR)
	{
	stc("Not on mortals.\n\r",ch);
	return;
	}
if (is_safe(ch,victim)) return;

		if (ch->pcdata->stats[WL_SPELLS] < 1)
		{
		stc("#RYou do not yet have mastery of this spell.\n\r",ch);
		return;
		}

		if (ch->mana < 5000)
		{
		stc("#RYou do not have the required 5000 mana.\n\r",ch);
		return;
		}

		dam = number_range(1500, 3000);
		dam += (ch->damcap[DAM_CAP] / 2);
		if (ch->pcdata->powers[WL_SKILLS] > 3) 
		{
		dam += number_range( 500, 1000 );
		}
		if (IS_NPC(victim)) dam *= 1.7;
                if (IS_IMMUNE(victim, IMM_HEAT)) dam = dam / 2;
		act("#y$n#R points $s hands towards #y$N#R, releasing a powerful bolt of flame.", ch, NULL, victim, TO_NOTVICT);
	    xprintf( buf, "#RYou point your hands toward $N, releasing a powerful blase of flame#n   #y[#C%d#y]#n\n\r", dam);
		act( buf, ch,NULL,victim,TO_CHAR);
		xprintf( buf, "#y$n#R points his hands towards you, releasing a powerful bolt of flame.#n   #y[#C%d#y]#n\n\r", dam);
		act( buf, ch,NULL,victim,TO_VICT);        if (!IS_SET(victim->affected_by, AFF_FLAMING))
        SET_BIT(victim->affected_by, AFF_FLAMING);  

		ch->mana -= 5000;
		hurt_person(ch, victim, dam);
		
		if (number_percent() < 50 && (ch->fighting != NULL) && (victim->hit > 0))
		{
		dam -= number_range(100,300);
		if (dam < 0) dam = 0;
		act("#y$n#R points $s hands towards #y$N#R, releasing a powerful bolt of flame.", ch, NULL, victim, TO_NOTVICT);
	    xprintf( buf, "#RYou point your hands toward $N, releasing a powerful blast of flame#n   #y[#C%d#y]#n\n\r", dam);
		act( buf, ch,NULL,victim,TO_CHAR);
		xprintf( buf, "#y$n#R points his hands towards you, releasing a powerful bolt of flame.#n   #y[#C%d#y]#n\n\r", dam);
		act( buf, ch,NULL,victim,TO_VICT);        if (!IS_SET(victim->affected_by, AFF_FLAMING))
        SET_BIT(victim->affected_by, AFF_FLAMING);  
		

		hurt_person(ch,victim,dam);
		}		
		
		if (number_percent() < 10 && (ch->fighting != NULL) && (victim->hit > 0))
		{
		dam -= number_range(350,750);
		if (dam < 0) dam = 0;		
		act("#y$n#R points $s hands towards #y$N#R, releasing a powerful bolt of flame.", ch, NULL, victim, TO_NOTVICT);
	    xprintf( buf, "#RYou point your hands toward $N, releasing a powerful blast of flame#n   #y[#C%d#y]#n\n\r", dam);
		act( buf, ch,NULL,victim,TO_CHAR);
		xprintf( buf, "#y$n#R points his hands towards you, releasing a powerful bolt of flame.#n   #y[#C%d#y]#n\n\r", dam);
		act( buf, ch,NULL,victim,TO_VICT);
        if (!IS_SET(victim->affected_by, AFF_FLAMING))
        SET_BIT(victim->affected_by, AFF_FLAMING);  

		hurt_person(ch,victim,dam);
		}			


		if (ch->pcdata->powers[WL_SKILLS] > 3) 
		{WAIT_STATE(ch, 8 );}
		else WAIT_STATE(ch, 12);
		return;
	}
		
		
		
void do_pshift( CHAR_DATA *ch, char *argument )
{

    ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	CHAR_DATA *victim;
	int door;   


	
	
	if (!IS_CLASS(ch, CLASS_LICH))
	{
	stc("Huh?\n\r",ch);
	return;
	}


	if (ch->pcdata->stats[WL_SPELLS] < 5)
	{
	stc("#RYou do not yet have mastery of this spell.#n\n\r",ch);
	return;
	}
    
	if ( ( victim = ch->fighting ) == NULL )
    {
	if ( ch->position == POS_FIGHTING )
	    ch->position = POS_STANDING;
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }
	
	if(ch->mana < 5000)
	{
	   send_to_char("#RYou d not have the 750 required mana.\n\r",ch);
	   return;
	}





	door = number_door( );
	if ( ( pexit = ch->in_room->exit[door] ) == NULL
	||   (to_room = pexit->to_room) == NULL)
	    {
		act( "#y$n#R mutters some words and disappears!", ch, NULL, NULL, TO_ROOM );	
	    send_to_char( "#RYou mutter some dark words and vanish from combat.\n\r", ch );
		char_from_room( ch );
		char_to_room( ch, get_room_index(ROOM_VNUM_ALTAR));
		stop_fighting( ch, TRUE );
		return;
		};


	act( "#y$n#R mutters some words and disappears!", ch, NULL, NULL, TO_ROOM );
    send_to_char( "#RYou mutter some dark words and vanish from combat.\n\r", ch );
	char_from_room(ch);
	char_to_room( ch, to_room );
    do_look(ch,"auto");
	stop_fighting( ch, TRUE );
	ch->mana -= 5000;
	return;
}


void do_plost( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

        if (IS_NPC(ch)) return;
    
    if (!IS_CLASS(ch, CLASS_LICH))
    {
            send_to_char("Huh?\n\r",ch);
            return;
    }    
	
	if (ch->pcdata->stats[WL_SPELLS] < 7) 
	{
	send_to_char("#RYou do not yet have mastery over this spell.\n\r",ch);
	return;
	}

	if (ch->mana < 20000)
	{
	stc("#RYou do not have the required 20000 mana.\n\r",ch);
	return;
	}
	
	if ( arg[0] == '\0' )
    {
	send_to_char( "#RCast paradise lost on who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "#RBeware the penguins.#n\n\r", ch );
	return;
    }

	if ( !IS_AFFECTED(victim, AFF_SANCTUARY) )
  {
	send_to_char( "#RThey don't have sanctuary on!#n\n\r", ch );
	return;
  }

    REMOVE_BIT(victim->affected_by, AFF_SANCTUARY);
	send_to_char("#RYour eyes sink into your head as you fill the room with despair.#n\n\r",ch);
	xprintf(buf, "#y%s#R's eyes sink into his head, and you are overcome with depression.#n\n\r", ch->name);
	stc( buf, victim);
	if (ch->pcdata->powers[WL_SKILLS] > 3) 
	{
    multi_hit( ch, victim, TYPE_UNDEFINED );
	}
	WAIT_STATE( ch, 24 );
	ch->mana -= 20000;
	return;
	}



void do_icelance( CHAR_DATA *ch, char *argument)
{
		CHAR_DATA *victim;
		char arg1[MAX_INPUT_LENGTH];
		char buf[MAX_STRING_LENGTH];
		int dam;

		argument = one_argument( argument, arg1 );

		if (IS_NPC(ch) )
		return;
		
    if (!IS_CLASS(ch, CLASS_LICH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
		if ( arg1[0] == '\0')
	    {
		send_to_char( "#RUse icelance on who?\n\r", ch );
		return;
		}

    if ( ( victim = get_char_room( ch, NULL, arg1 ) ) == NULL )
    {
	send_to_char( "#RThey aren't here.#n\n\r", ch );
	return;
    }
		
		if ((victim == ch))
		{
		stc("#RYeah ok...\n\r",ch);
		return;
		}

		if (ch->pcdata->stats[WL_SPELLS] < 4)
		{
		stc("#RYou do not yet have mastery of this spell.\n\r",ch);
		return;
		}
		if (is_safe(ch, victim))
		{
		stc("#RYou can't attack them, they are safe.\n\r",ch);
		return;
		}
		if (ch->mana < 5000)
		{
		stc("#RYou do not have the required 5000 mana.\n\r",ch);
		return;
		}
		
		dam = number_range(3000, 5500);
		dam += (ch->damcap[DAM_CAP] / 2);
		if (ch->pcdata->powers[WL_SKILLS] > 3) 
		{
		dam += number_range( 1000, 1500 );
		}

		if (IS_NPC(victim))
		{
		dam *= 2.5;
		}
                if(IS_IMMUNE(victim, IMM_COLD))
                 dam = dam / 2;
		act("#y$n#R points $s hands towards #y$N#R and sends forth a wicked spear of ice.", ch, NULL, victim, TO_NOTVICT);
	    xprintf( buf, "#RYou point your hands toward %s and sends forth a wicked spear of ice.#n   #y[#C%d#y]#n\n\r", victim->name, dam);
		stc( buf, ch);
		xprintf( buf, "#y%s#R points his hands towards you and sends forth a wicked spear of ice.#n   #y[#C%d#y]#n\n\r", ch->name, dam);
		stc( buf, victim);
		hurt_person(ch,victim,dam);
		ch->mana -= 5000;

		if (!IS_BLEEDING(victim, BLEEDING_THROAT))
		SET_BIT(victim->loc_hp[6], BLEEDING_THROAT);
		if (victim->hit < -10) victim->hit = -10;
		if (ch->pcdata->powers[WL_SKILLS] > 3) 
		{WAIT_STATE(ch, 8 );}
		else WAIT_STATE(ch, 16);
		if (ch->fighting == NULL) ch->fighting = victim;
		return;
	}

void do_negativity( CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	int dam;
	
	argument = one_argument( argument, arg );

	if (IS_NPC(ch) )
	return;

	if (!IS_CLASS(ch, CLASS_LICH))
	{
		stc("Huh?\n\r",ch);
		return;
	}
	
	if ( arg[0] == '\0')
    {
	send_to_char( "#RCast negativity on who?\n\r", ch );
	return;
	}

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "#RThey aren't here.#n\n\r", ch );
	return;
    }
    
       if (is_safe(ch,victim)) return;
		
	if ((victim == ch))
	{
	stc("#RYeah ok...\n\r",ch);
	return;
	}

	if (ch->pcdata->stats[WL_SPELLS] < 2)
	{
		stc("You do not yet have mastery of this spell.",ch);
		return;
	}

	

	dam = number_range(1500, 2500);
	if (ch->pcdata->powers[WL_SKILLS] > 3) dam *= 2;
	
	if (ch->mana < dam * 2)
	{
		stc("You do not have enough mana for this spell.",ch);
		return;
	}


	hurt_person(ch, victim, dam);
	ch->hit += dam / 3;
	ch->mana -= dam;
       
	xprintf(buf, "#rYou place your hands on #R%s,#r sucking the life out of them. #B[#R%d#B]#n\n\r", victim->name, dam);
	stc(buf, ch);
	xprintf(buf, "#R%s#r places his hands on you, sucking out your life. #B[#R%d#B]#n\n\r", ch->name, dam);
	stc(buf, victim);
	set_fighting(ch, victim, 1);
	WAIT_STATE(ch, 6);
	return;
}



void do_chaos( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	AFFECT_DATA af;
	int chaos, dam;	
	char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg );
    

	if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_LICH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
	if (ch->pcdata->stats[WL_SPELLS] < 6)
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
	
  if ( arg[0] == '\0')
  {
	send_to_char( "#RCast chaos orb on whom?\n\r", ch );
	return;
  }

  if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
  {
    send_to_char( "They are not here.\n\r", ch );
    return;
  }
  if (is_safe(ch, victim))
  {
	stc("You cannot attack them.\n\r",ch);
	return;
  }

  if (ch->mana < 5000)
  {
	  stc("You do not ave the required 5000 mana.\n\r",ch);
	  return;
  }


	chaos = number_range(1, 9);


act("#r$n#0 begins chanting dark words, summoning forth a sphere of #rc#lh#pa#go#cs#0 to hurl at #r$N!#n", ch, NULL, victim, TO_NOTVICT);
xprintf( buf, "#0You summon forth a perfect sphere of #rc#lh#pa#go#cs#0 and hurl it at #r$N!#n\n\r");
act(buf, ch, NULL, victim, TO_CHAR);
xprintf( buf, "#r$n#0 summons forth a perfect sphere of #rc#lh#pa#go#cs#0 and hurls it at you!#n\n\r");
act(buf, ch, NULL, victim, TO_VICT);
	

	if (chaos == 1 || chaos == 6)
	{
	stc("The sphere explodes in a shower of #Ggreen bubbles#n.\n\r",victim);
	stc("Your sphere explodes in a shower of #Ggreen bubbles#n.\n\r",ch);
	if ( IS_AFFECTED(victim, AFF_POISON)) return;
	af.type      = gsn_poison;
    af.duration  = chaos * 10;
    af.location  = APPLY_STR;
    af.modifier  = 0 - number_range(3,8);
    af.bitvector = AFF_POISON;
    affect_join( victim, &af );                                             
	WAIT_STATE(victim, number_range(5, 20));
	send_to_char( "#GYou are poisoned!#n\n\r", victim );  
	}
	else if (chaos == 2 || chaos == 7 )
	{
	stc("The sphere explodes, forming a #yyellow haze#n.\n\r",victim);
	stc("Your sphere explodes, forming a #yyellow haze#n.\n\r",ch);
    if ( IS_AFFECTED(victim, AFF_BLIND))
	{
	af.type      = gsn_blindness;
    af.location  = APPLY_HITROLL;
    af.modifier  = -750;
    af.duration  =  chaos * 10;
    af.bitvector = AFF_BLIND;
    affect_to_char( victim, &af );
	WAIT_STATE(victim, number_range(5, 15));
    send_to_char( "#yYou are blinded!#n\n\r", victim );                            
	}
	else
	{
	stc("They are already blinded.\n\r",ch);
	}
	}
	else if (chaos == 3 || chaos == 8)
	{
	stc("The sphere explodes in a shower of #Rred flames#n.\n\r",victim);
	stc("Your sphere explodes in a shower of #Rred flames#n.\n\r",ch);
	WAIT_STATE(victim, number_range(5, 20));
	SET_BIT(victim->affected_by, AFF_FLAMING);
	}
	else if (chaos == 4 || chaos == 9)
	{
	dam = number_range(7500, 15000);
	xprintf(buf,"The sphere explodes in a scattering of #Lblue sparks#n.  #R[#y%d#R]#n\n\r", dam);
	stc( buf, victim);
	xprintf(buf,"Your sphere explodes in a scattering of #Lblue sparks#n.  #R[#y%d#R]#n\n\r", dam);
	stc( buf, ch);
	WAIT_STATE(victim, number_range(5, 8));
	hurt_person(ch, victim, dam);
	}
	else if (chaos == 5 )
	{
	stc("The sphere dissipates before it hits you#n.\n\r",victim);
	stc("Your dissipates shortly after casting#n.\n\r",ch);
	}
	
	WAIT_STATE(ch, 4);	
	ch->mana -= 5000;
	if (victim->hit < -10) victim->hit = -10;
	return;
}
