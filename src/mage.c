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

void do_reveal(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?!?.\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You don't have the mystical energies to do this.\n\r",ch);
    return;
  } 
  act( "$n mumles a few words, and you are suddenly blinded by a flash.", ch, NULL, NULL, TO_ROOM );   
  send_to_char( "You reveal everything hidden in the room.\n\r", ch );
  for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
  {
    if (ich==ch || ich->trust > 6) continue;
    affect_strip ( ich, gsn_invis );
    affect_strip ( ich, gsn_mass_invis );
    affect_strip ( ich, gsn_sneak );
    if (IS_AFFECTED(ich, AFF_HIDE))      REMOVE_BIT(ich->affected_by, AFF_HIDE);
    if (IS_SET(ich->affected_by, AFF_INVISIBLE)) REMOVE_BIT(ich->affected_by, AFF_INVISIBLE);
    if (IS_SET(ich->affected_by, AFF_SNEAK))     REMOVE_BIT(ich->affected_by, AFF_SNEAK);
    if (IS_SET(ich->act, PLR_WIZINVIS))          REMOVE_BIT(ich->act, PLR_WIZINVIS);
    if (IS_SET(ich->affected_by, AFF_SHIFT))     REMOVE_BIT(ich->affected_by, AFF_SHIFT);
    if (IS_SET(ich->extra, EXTRA_EARTHMELD))     REMOVE_BIT(ich->extra, EXTRA_EARTHMELD);
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE)) REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    if (IS_AFFECTED(ch, AFF_PEACE)) REMOVE_BIT(ch->affected_by, AFF_PEACE);
    if (IS_AFFECTED(ch,AFF_ETHEREAL)) REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

    if (IS_CLASS(ich, CLASS_DROW) && IS_SET(ich->newbits, NEW_DARKNESS))
    {
      REMOVE_BIT(ich->newbits, NEW_DARKNESS);
      REMOVE_BIT(ich->in_room->room_flags, ROOM_TOTAL_DARKNESS);
    }
    send_to_char("You are suddenly very visible.\n\r",ich);
  }
  ch->mana -= 5000;
  return;
}

void do_magearmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
 	if(!IS_IMMORTAL(ch))
	{
    if( !IS_CLASS(ch, CLASS_MAGE) )
    {
      send_to_char("What?\n\r",ch);
      return;
    }
	}
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of mage armor you wish to make: Dagger Staff\n\r"
                 "Ring Collar Robe Cap Leggings Boots Gloves Sleeves Cape Belt Bracer Mask.\n\r",ch);
    return;
  }
  if ( ch->practice < 100 )
  {
    send_to_char("It costs 100 points of primal to create mage equipment.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"mask")) vnum = 33013;
  else if (!str_cmp(arg,"belt")) vnum = 33011;
  else if (!str_cmp(arg,"cape")) vnum = 33010;
  else if (!str_cmp(arg,"ring")) vnum = 33002;
  else if (!str_cmp(arg,"collar")) vnum = 33003;
  else if (!str_cmp(arg,"robe")) vnum = 33004;
  else if (!str_cmp(arg,"cap")) vnum = 33005;
  else if (!str_cmp(arg,"leggings")) vnum = 33006;
  else if (!str_cmp(arg,"boots")) vnum = 33007;
  else if (!str_cmp(arg,"gloves")) vnum = 33008;
  else if (!str_cmp(arg,"sleeves")) vnum = 33009;
  else if (!str_cmp(arg,"bracer")) vnum = 33012;
  else if (!str_cmp(arg,"staff")) vnum = 33000;
  else if (!str_cmp(arg,"dagger")) vnum = 33001;
  else
  {
    do_magearmor(ch,"");
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

void do_chaosmagic (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  int sn, crandom, level;

  argument=one_argument(argument,arg);
  level = ch->spl[RED_MAGIC]/4;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_LICH) && ch->pcdata->powers[CHAOS_MAGIC] < 1)
  {
    send_to_char("Not yet, not yet.\n\r",ch);
    return;
  }
  if (ch->mana < 1500)
  {
    send_to_char("You need more mana.\n\r",ch);
    return;
  }
  if (ch->in_room != NULL)
  {
    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
    {
      send_to_char( "You cannot invoke the forces of chaos in a safe room.\n\r", ch );
      return;
    }
  }
  if (arg[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch );
    return;
  }
  crandom = number_range(1,12);
  if (crandom == 1) sn = skill_lookup("spirit kiss");
  else if (crandom == 2) sn = skill_lookup("desanct");
  else if (crandom == 3) sn = skill_lookup("imp heal");
  else if (crandom == 4) sn = skill_lookup("imp fireball");
  else if (crandom == 5) sn = skill_lookup("imp faerie fire");
  else if (crandom == 6) sn = skill_lookup("imp teleport");
  else if (crandom == 7) sn = skill_lookup("change sex");
  else if (crandom == 8) sn = skill_lookup("shield");
  else if (crandom == 9) sn = skill_lookup("readaura");
  else if (crandom == 10) sn = skill_lookup("earthquake");
  else if (crandom == 11) sn = skill_lookup("gate");
  else if (crandom == 12) sn = skill_lookup("dispel magic");
  else sn = 0;
  if (sn  > 0) (*skill_table[sn].spell_fun) (sn,level,ch,victim);
  ch->mana -= 1500;
  WAIT_STATE(ch,6);
  return;
}

/* Syntax : chant <type> <color> <target>
 * ex. chant bless red self 
 */
void do_chant (CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  AFFECT_DATA af;
  int i = 0, dam = 0, count = 0, sn = 0;
  int red_magic = ch->spl[RED_MAGIC];
  int blue_magic = ch->spl[BLUE_MAGIC];
  int green_magic = ch->spl[GREEN_MAGIC];
  int purple_magic = ch->spl[PURPLE_MAGIC];
  int yellow_magic = ch->spl[YELLOW_MAGIC];
  int magic_power = ch->spl[RED_MAGIC] + ch->spl[BLUE_MAGIC] + ch->spl[GREEN_MAGIC] + ch->spl[PURPLE_MAGIC] + ch->spl[YELLOW_MAGIC];
    
  argument=one_argument(argument,arg1);
  strcpy(arg2,argument);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0] == '\0')
  {  
    send_to_char("Syntax : chant <bless/curse/damage/heal> <target>.\n\r", ch);
    return;
  }  
  if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) 
  {
    magic_power +=100;
    red_magic +=20;
    yellow_magic +=20;
    purple_magic +=20;
    green_magic +=20;
    blue_magic +=20;
  }
  if (!str_cmp(arg1, "heal"))
  {
    if (arg2[0] == '\0') victim = ch;
    else if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana < 4500)
    {
      send_to_char("You need 4500 mana.\n\r",ch);
      return;
    }    
    act("$n channels lifeforce from the five elements into $N.",ch,NULL,victim,TO_NOTVICT);
    act("You channel lifeforce from the five elements into $N.",ch,NULL,victim,TO_CHAR);
    act("$n channels lifeforce from the five elements into you.",ch,NULL,victim,TO_VICT);
    WAIT_STATE(ch,8);
    ch->mana -= 4500;
    victim->hit = UMIN(victim->hit + (magic_power * 1.5), victim->max_hit);
    return;
  }
  else if (!str_cmp(arg1, "damage"))
  {
    if (arg2[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
    else if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana <5000)
    {
      send_to_char("You need 5000 mana.\n\r",ch);
      return;
    }
    if (victim == ch)
    {
      send_to_char("You really don't want to hurt yourself.\n\r", ch );
      return;
    }
    if (is_safe(ch, victim)) return;
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    if (IS_AFFECTED(ch, AFF_PEACE)) REMOVE_BIT(ch->affected_by, AFF_PEACE);
    if (victim->position > POS_STUNNED)
    {
      if (ch->fighting == NULL) set_fighting(ch, victim, TRUE);
      if (victim->fighting == NULL) set_fighting(victim, ch, FALSE);
    }
    WAIT_STATE(ch,8);
    ch->mana -= 5000;
    dam = number_range(red_magic*4, red_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    xprintf(buf1,"$n calls for the forces of #Rhell#n to destroy $N [#C%d#n]" , dam);
    xprintf(buf2,"You call for the forces of #Rhell#n to destroy $N [#C%d#n]" , dam);
    xprintf(buf3,"$n calls for the forces of #Rhell#n to destroy you [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(blue_magic*4, blue_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    xprintf(buf1,"$n summons a supernatural #Bstorm#n to strike $N with lightning [#C%d#n]" , dam);
    xprintf(buf2,"You summon a supernatural #Bstorm#n to strike $N with lightning [#C%d#n]" , dam);
    xprintf(buf3,"$n summons a supernatural #Bstorm#n to strike you with lightning [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(green_magic*4, green_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    xprintf(buf1,"$n calls for #Gthorns#n to grow from the earth and pierce $N [#C%d#n]" , dam);
    xprintf(buf2,"You call for #Gthorns#n to grow from the earth and pierce $N [#C%d#n]" , dam);
    xprintf(buf3,"$n calls for #Gthorns#n to grow from the earth and pierce You [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(purple_magic*4, purple_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    xprintf(buf1,"$n calls on the powers of the #punderdark#n to shatter $N's bones [#C%d#n]" , dam);
    xprintf(buf2,"You call on the powers of the #punderdark#n to shatter $N's bones [#C%d#n]" , dam);
    xprintf(buf3,"$n calls on the powers of the #punderdark#n to shatter your bones [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(yellow_magic*4, yellow_magic*5);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
//      if (IS_CLASS(ch, CLASS_MAGE) && dam > 600) dam = number_range(580,620);
      if (dam > 1000) dam = number_range(950,1050);
    }
    xprintf(buf1,"$n spews forth a shower of #Yacid#n striking $N [#C%d#n]" , dam);
    xprintf(buf2,"you spew forth a shower of #Yacid#n striking $N [#C%d#n]" , dam);
    xprintf(buf3,"$n spews forth a shower of #Yacid#n striking you [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    return;
  }
  else if (!str_cmp(arg1, "bless"))
  {
    if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana <5500)
    {
      send_to_char("You need 5500 mana.\n\r",ch);
      return;
    }
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    count = 0;
    for (i = 0; i<5 ; i++)
    {
      if (i==0) {sn = skill_lookup("purple sorcery");dam=purple_magic;}
      if (i==1) {sn = skill_lookup("yellow sorcery");dam=yellow_magic;}
      if (i==2) {sn = skill_lookup("green sorcery");dam=green_magic;}
      if (i==3) {sn = skill_lookup("red sorcery");dam=red_magic;}
      if (i==4) {sn = skill_lookup("blue sorcery");dam=blue_magic;}
      if (is_affected(victim, sn))
        continue;
      
      count++;
      
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HIT;
      af.modifier  = dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_AC;
      af.modifier  = -dam/2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MOVE;
      af.modifier  = dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MANA;
      af.modifier  = dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HITROLL;
      af.modifier  = dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_DAMROLL;
      af.modifier  = dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
    }
    ch->mana -= count * 1100;
    WAIT_STATE(ch,6);
    xprintf(buf1, "You bless $N with the power of the elements.");
    xprintf(buf2, "The power of the five elements fills your body.");
    act(buf1, ch, NULL, victim, TO_CHAR);
    act(buf2, ch, NULL, victim, TO_VICT);
    return;
  }
  else if (!str_cmp(arg1, "curse"))
  {
    if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana <4500)
    {
      send_to_char("You need 4500 mana.\n\r",ch);
      return;
    }
    if (victim->level < 3)
    {
      send_to_char("Not on mortal, thier bodies cant handles the strain....poor little buggers.\n\r",ch);
      return;
    }
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    count = 0;
    for (i = 0;i<5;i++)
    {
      if (i==0) {sn = skill_lookup("purple sorcery");dam=purple_magic;}
      if (i==1) {sn = skill_lookup("yellow sorcery");dam=yellow_magic;}
      if (i==2) {sn = skill_lookup("green sorcery");dam=green_magic;}
      if (i==3) {sn = skill_lookup("red sorcery");dam=red_magic;}
      if (i==4) {sn = skill_lookup("blue sorcery");dam=blue_magic;}
      if (is_affected(victim, sn))
        continue;
      
      count++;

      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HIT;
      af.modifier  = -dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_AC;
      af.modifier  = dam/2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MOVE;
      af.modifier  = -dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MANA;
      af.modifier  = -dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HITROLL;
      af.modifier  = -dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_DAMROLL;
      af.modifier  = -dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
    }
    ch->mana -= count * 900;
    WAIT_STATE(ch,6);
    xprintf(buf1, "You curse $N with the power of the elements.");
    xprintf(buf2, "The power of the five elements wrecks your body.");
    act(buf1, ch, NULL, victim, TO_CHAR);
    act(buf2, ch, NULL, victim, TO_VICT);
    return;      
  }
  send_to_char("Syntax : chant <bless/curse/damage/heal> <target>.\n\r", ch);
  return;
}

/* Syntax : invoke <list/learn/spell> <none/power/power>
 * ex. invoke learn stoneskin
 * ex. invoke spell stoneskin
 */
void do_invoke (CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int shieldcost=25;
  int steelshieldcost=5;
  int beastcost=10;
  int illusioncost=5;
  int defshieldcost=5;

  argument=one_argument(argument,arg1);
  strcpy(arg2,argument);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0]== '\0')
  {
    if(ch->pcdata->powers[PINVOKE] > 0)
      send_to_char("* You have the power to teleport yourself anywhere.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 1)
      send_to_char("* You know how to invoke mageshields.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 2)
      send_to_char("* You have the power to scry on far away places.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 3)
      send_to_char("* You can discharge your shields in a shower of power.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 4)
      send_to_char("* A deflector shield defends you in combat.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 5)
      send_to_char("* You have the power to turn your skin into solid steel.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 7)
      send_to_char("* You have mastered the power of illusions.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 8)
      send_to_char("* You can invoke the power of the beast.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 6 && ch->pcdata->powers[PINVOKE] < 10)
      send_to_char("* You have a deeper understanding of magic.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 9)
      send_to_char("* You have mastered the art of magic.\n\r",ch);
    xprintf(buf,"invokes Learned (%d).\n\r", ch->pcdata->powers[PINVOKE]);
    send_to_char(buf,ch);
    return;  
  }
  if (!str_cmp(arg1,"learn"))
  {
    int cost = (ch->pcdata->powers[PINVOKE]+1) * 20;
    if ( ch->pcdata->powers[PINVOKE] >= 10 )
    {
      send_to_char("You have already gained all the known invokes.\n\r",ch);
      return;
    }
    else if (cost > ch->practice)
    {
      send_to_char("you don't have enough primal.\n\r",ch);
      return;
    }
    else
    {
      ch->pcdata->powers[PINVOKE] +=1;
      ch->practice -= cost;
      send_to_char("You gain a deeper understanding of the mystical powers.\n\r",ch);
    }
    return;
  }
  else if (!str_cmp(arg1,"steelshield"))
  {
    if (ch->pcdata->powers[PINVOKE] < 6)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_STEELSHIELD) && ch->practice >= steelshieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
      ch->practice -= steelshieldcost;
      send_to_char("Your skin turns to solid steel.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_STEELSHIELD))
    {
      send_to_char("Your skin cannot become any harder.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to activate steelskin.\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg1,"beast"))
  {
    if (ch->pcdata->powers[PINVOKE] < 9)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_BEAST) && ch->practice >= beastcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_BEAST);
      ch->practice -= beastcost;
      send_to_char("Your let your inner beast take control over your actions.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_BEAST))
    {
      send_to_char("You feel the beast inside of you roar, but nothing else happens.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 10 primal to let the beast take over.\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg1,"illusions"))
  {
    if (ch->pcdata->powers[PINVOKE] < 8)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_ILLUSIONS) && ch->practice >= illusioncost)
    {
      SET_BIT(ch->itemaffect, ITEMA_ILLUSIONS);
      ch->practice -= illusioncost;
      send_to_char("You conjure several images of yourself, making it impossibly to tell which is really you.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_ILLUSIONS))
    {
      send_to_char("You cannot summon any more illusions.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to invoke the illusions of yourself.\n\r",ch);
      return;
    }
    return;
  }
  else if (!str_cmp(arg1,"deflector"))
  {
    if (ch->pcdata->powers[PINVOKE] < 5)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_DEFLECTOR) && ch->practice >= defshieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_DEFLECTOR);
      ch->practice -= defshieldcost;
      send_to_char("A shield appears in the air, preventing people from hitting you.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_DEFLECTOR))
    {
      send_to_char("You cannot summon any more shields.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to summon a shield.\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg1,"mageshield") || !str_cmp(arg1,"shield"))
  {
    if (ch->pcdata->powers[PINVOKE] < 2)
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_SET(ch->newbits, NEW_MULTIARMS))
      SET_BIT(ch->newbits, NEW_MULTIARMS);
    if (!IS_ITEMAFF(ch, ITEMA_MAGESHIELD) && ch->practice>=shieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_MAGESHIELD);
      ch->practice -= shieldcost;
      send_to_char("A layer of mystic shields envelopes your body.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
    {
      send_to_char("You are allready shielded.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("The cost is 25 primal.\n\r",ch);
      return;
    }
  }
  else if (!str_cmp(arg1,"all"))
  {
    int totalcost = 0;
    if (ch->pcdata->powers[PINVOKE] < 9)
    {
      send_to_char("You don't have all invokes, so theres no point in invoking them all.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_MAGESHIELD)) totalcost += shieldcost;
    if (!IS_ITEMAFF(ch, ITEMA_DEFLECTOR)) totalcost += defshieldcost;
    if (!IS_ITEMAFF(ch, ITEMA_ILLUSIONS)) totalcost += illusioncost;
    if (!IS_ITEMAFF(ch, ITEMA_BEAST)) totalcost += beastcost;
    if (!IS_ITEMAFF(ch, ITEMA_STEELSHIELD)) totalcost += steelshieldcost;
    if (totalcost == 0)
    {
      send_to_char("You already have all invokes on.\n\r",ch);
      return;
    }
    if (ch->practice < totalcost)
    {
      send_to_char("You don't have the primal.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_MAGESHIELD)) SET_BIT(ch->itemaffect, ITEMA_MAGESHIELD);
    if (!IS_ITEMAFF(ch, ITEMA_DEFLECTOR)) SET_BIT(ch->itemaffect, ITEMA_DEFLECTOR);
    if (!IS_ITEMAFF(ch, ITEMA_ILLUSIONS)) SET_BIT(ch->itemaffect, ITEMA_ILLUSIONS);
    if (!IS_ITEMAFF(ch, ITEMA_BEAST)) SET_BIT(ch->itemaffect, ITEMA_BEAST);
    if (!IS_ITEMAFF(ch, ITEMA_STEELSHIELD)) SET_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
    send_to_char("You invoke all your powers.\n\r",ch);
    ch->practice -= totalcost;
    return;
  }
  send_to_char("invoke <learn/mageshield/deflector/steelshield/illusions/beast/all>.\n\r",ch);
  return;
}

/* syntax : magics
 * shows all class powers in effect
 */
void do_magics (CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
    send_to_char("* Your skin is covered in a layer of mystical energies.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_BEAST))
    send_to_char("* Your inner beast is in control of your actions.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_STEELSHIELD))
    send_to_char("* Your skin is solid steel.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_DEFLECTOR))
    send_to_char("* A shield hovers in the air, defending you.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_ILLUSIONS))
    send_to_char("* Several images of you stand by your side.\n\r",ch);
  return;
}

/* syntax : discharge
 * (drops all elementalshields in one BIG blast)
 */
void do_discharge (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  int magic_power = ch->spl[RED_MAGIC] + ch->spl[BLUE_MAGIC] + ch->spl[GREEN_MAGIC] + ch->spl[PURPLE_MAGIC] + ch->spl[YELLOW_MAGIC];
  int dam;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PINVOKE] < 4) /* 4 is just a test */
  {
    send_to_char("You don't have that invoke yet.\n\r",ch);
    return;
  }
  if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) 
    magic_power +=100;
  if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
  {
    REMOVE_BIT(ch->itemaffect, ITEMA_MAGESHIELD);
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
      vch_next = vch->next_in_room;
      if (vch != ch)
      {
        if (is_safe(ch,vch)) continue;
        if ((mount = ch->mount) != NULL)
          if (mount == vch) continue;
        dam = number_range(magic_power*7/2, magic_power*9/2) + ch->damroll;
        if (IS_AFFECTED(vch, AFF_SANCTUARY)) dam *= 0.5;
        xprintf(buf1,"$n's mystical shields explodes in a shower of flames striking $N [#C%d#n]",dam);
        xprintf(buf2,"Your mystical shields explodes in a shower of flames striking $N [#C%d#n]",dam);
        xprintf(buf3,"$n's mystical shields explodes in a shower of flames striking you [#C%d#n]",dam);
        act(buf1,ch,NULL,vch, TO_NOTVICT);
        act(buf2,ch,NULL,vch, TO_CHAR);
        act(buf3,ch,NULL,vch, TO_VICT);
        hurt_person(ch,vch,dam);
        if (vch->position > POS_STUNNED)
        {
          if (vch->fighting == NULL) set_fighting(vch, ch, TRUE);
          if (ch->fighting == NULL) set_fighting(ch, vch, FALSE);
        }
      }
    }
  }
  return;
}


/* syntax : teleport <target>
 * ex : teleport satan
 */
void do_teleport (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;
  CHAR_DATA *victim;

  one_argument (argument, arg);
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r", ch );
    return;
	}
  if (ch->pcdata->powers[PINVOKE] < 1) /* 1 is just a test */
  {
    send_to_char("You don't have that invoke yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL) 
  {
    send_to_char("Teleport to whom?\n\r", ch );
    return;
  }
  if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  { 
    stc( "You can't find it's room.\n\r",ch);
    return; 
  }
  if (IS_IMMUNE(victim, IMM_TRAVEL) && !IS_NPC(victim))
  {
    send_to_char("I don't think they want you to do that.\n\r",ch);
    return;
  } 
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
      if (IS_SET(victim->act, ACT_NOTRAVEL))
  {
  	send_to_char("No Can Do.\n\r", ch);
  	return;
  }
	location = victim->in_room;
  if (ch->mana < 250) 
  {
    send_to_char("You don't have the mana to cast teleport.\n\r", ch );
    return;
  }
	act("You utter a single powerword.", ch, NULL, NULL, TO_CHAR);
	act("$n utters a strange sounding word and disappers.", ch, NULL, NULL, TO_ROOM);
	ch->mana -= 250;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");
	act("You materialize.", ch, NULL, NULL, TO_CHAR);
	act("$n suddenly appear from out of nowhere.", ch, NULL, NULL, TO_ROOM);
  return;
}

/* syntax : objectgate <object>
 * ex. objectgate 2.bells
 */
void do_objectgate (CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  char arg1[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg1);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_LICH))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if (IS_CLASS(ch, CLASS_LICH) && ch->pcdata->powers[CON_LORE] < 1)
  {
    send_to_char("But you don't know how to do that silly.\n\r",ch);
    return;
  }
  if (arg1[0] == '\0')
  {
    send_to_char("What object do you want to attempt to gate?\n\r",ch);
    return;
  }
  if ((obj = get_obj_world(ch, arg1)) == NULL)
  {
    send_to_char("No such object.\n\r",ch);
    return;
  }
  if (IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->quest, QUEST_RELIC))
  {
    send_to_char("You cannot grab artifacts or relics.\n\r",ch);
    return;
  }
  if (IS_OBJ_STAT(obj, ITEM_NOLOCATE))
  {
    send_to_char("You wish.\n\r", ch);
    return;
  }
  if (obj->carried_by != NULL)
  {
    send_to_char("Someone is holding the item.\n\r",ch);
    return;
  }
  if (obj->in_room == NULL)
  {
    send_to_char("It seems to be inside something.\n\r",ch);
    return;
  }
  if (!IS_SET(obj->wear_flags, ITEM_TAKE))
  {
    send_to_char("This item is too big.\n\r",ch);
    return;
  }
  if( IS_SET(obj->in_room->room_flags, ROOM_ASTRAL))
  {
    send_to_char( "You can't find it's room.\n\r",ch);
    return;
  }
  if(IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    send_to_char( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  obj_from_room( obj );
  obj_to_char( obj, ch );
  send_to_char("The item appears in your hand.\n\r",ch);
  return;
}

/* 
 * for affect spells
 */

void do_bewitch (CHAR_DATA *ch, char *argument)
{
  /*ged*/
  WAIT_STATE(ch, 2);
  return;
}


void do_mageskill( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MAGE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	stc(" #o_________________________________#n\n\r",ch);
	stc("#o/  _\\                             \\#n\n\r",ch);
	stc("#o\\ (_/_____________________________/#n\n\r",ch);
	stc(" #o\\           #0.:#pS#Pkills#0:.        #o\\#n\n\r",ch);
	xprintf(buf,"  #o\\   #P*#cS#Cpritual #cK#Carma       #0[#P%d#0]#o \\#n\n\r",ch->pcdata->powers[MAGE_SK]);
	stc(buf,ch);
	xprintf(buf,"   #o\\   #P*#cS#Corcery #cS#Cpell #cU#Cse    #0[#P%d#0]#o \\#n\n\r",ch->pcdata->powers[MAGE_SSU]);
	stc(buf,ch);
	xprintf(buf,"  #o__\\   #P*#cM#Cagical #cS#Cpell #cU#Cse    #0[#P%d#0] #o\\#n\n\r",ch->pcdata->powers[MAGE_MSU]);
	stc(buf,ch);
	xprintf(buf," #o/ )_\\   #P*#cN#Caturalism #cS#Cpell #cU#Cse #0[#P%d#0] #o\\#n\n\r",ch->pcdata->powers[MAGE_NSU]);
	stc(buf,ch);
	stc(" #o\\___/_____________________________/#n\n\r",ch);
	return;
	}

else if (!str_cmp(arg2,"learn") && !str_cmp(arg1,"sk"))
    {
	int cost;
	int max = 5;

	cost = (ch->pcdata->powers[MAGE_SK]+1) * 2000000;
	if ( ch->pcdata->powers[MAGE_SK] >= max )
	{
	    xprintf(buf,"#0You've learned as much #cS#Cpiritual #cK#Carma #0as possible at this point in time!#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->exp)
	{
	    xprintf(buf,"#0You need #P%d #0exp to improve this skill any further!#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[MAGE_SK] += 1;
	ch->exp -= cost;
	xprintf(buf,"#0You further your studies in #cS#Cpiritual #cK#Carma!#n\n\r");
	send_to_char(buf,ch);
    }

else if (!str_cmp(arg2,"learn") && !str_cmp(arg1,"ssu"))
    {
	int cost;
	int max = 5;

	cost = (ch->pcdata->powers[MAGE_SSU]+1) * 2000000;
	if ( ch->pcdata->powers[MAGE_SSU] >= max )
	{
	    xprintf(buf,"#0You've learned as much #rS#Rorcery #rS#Rpell #rU#Rse #0as possible at this point in time!#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->exp)
	{
	    xprintf(buf,"#0You need #P%d #0exp to improve this skill any further!#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[MAGE_SSU] += 1;
	ch->exp -= cost;
	xprintf(buf,"#0You further your studies in #rS#Rorcery #rS#Rpell #rU#Rse!#n\n\r");
	send_to_char(buf,ch);
    }

else if (!str_cmp(arg2,"learn") && !str_cmp(arg1,"msu"))
    {
	int cost;
	int max = 5;

	cost = (ch->pcdata->powers[MAGE_MSU]+1) * 2000000;
	if ( ch->pcdata->powers[MAGE_MSU] >= max )
	{
	    xprintf(buf,"#0You've learned as much #lM#Lagical #lS#Lpell #lU#Lse #0as possible at this point in time!#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->exp)
	{
	    xprintf(buf,"#0You need #P%d #0exp to improve this skill any further!#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[MAGE_MSU] += 1;
	ch->exp -= cost;
	xprintf(buf,"#0You further your studies in #lM#Lagical #lS#Lpell #lU#Lse!#n\n\r");
	send_to_char(buf,ch);
    }

else if (!str_cmp(arg2,"learn") && !str_cmp(arg1,"nsu"))
    {
	int cost;
	int max = 5;

	cost = (ch->pcdata->powers[MAGE_NSU]+1) * 2000000;
	if ( ch->pcdata->powers[MAGE_NSU] >= max )
	{
	    xprintf(buf,"#0You've learned as much #gN#Gaturalism #gS#Gpell #gU#Gse #0as possible at this point in time!#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->exp)
	{
	    xprintf(buf,"#0You need #P%d #0exp to improve this skill any further!#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[MAGE_NSU] += 1;
	ch->exp -= cost;
	xprintf(buf,"#0You further your studies in #gN#Gaturalism #gS#Gpell #gU#Gse!#n\n\r");
	send_to_char(buf,ch);
    }
    else send_to_char("#PTo learn skills, type #0Mageskill #c<#Csk#c/#Cssu#c/#Cnsu#c/#Cmsu#c> #0Learn.#n\n\r",ch);
	return;
}

void do_magespells( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MAGE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	stc("                #W.-~~~~~~~~~-._       _.-~~~~~~~~~-.#n\n\r",ch);
	stc("            #l__#W.'  #C((#0Spells#C))  #W~.   .~   #C((#yIndex#C))  #7`.#l__#n\n\r",ch);
	xprintf(buf,"          #l.'#7// #rS#Rorcery    #P[#0%d#P]   #7\\#0.#7/   #lM#Lagical     #P[#0%d#P]#7\\\\#l`.#n\n\r",ch->pcdata->powers[MAGE_SORCERY],ch->pcdata->powers[MAGE_MAGIC]);
	stc(buf,ch);
	xprintf(buf,"        #l.'#7//  #gN#Gaturalism #P[#0%d#P]     #0|     #oE#ynchantment #P[#0%d#P]#7 \\\\#l`.#n\n\r",ch->pcdata->powers[MAGE_NATURALISM],ch->pcdata->powers[MAGE_ENCHANTMENT]);
	stc(buf,ch);
	stc("      #l.'#7// #W.-~\"\"\"\"\"\"\"~~~~-._     #0|#W     _,-~~~~\"\"\"\"\"\"\"~-. #7\\\\#l`.#n\n\r",ch);
	stc("    #L.'#7//#W.-\"                 `-.  #0|#7  #W.-'                 \"-.#7\\\\#L`.#n\n\r",ch);
	stc("  #L.'#7//______.============-..   \\ #0|#7 /   ..-============.______\\\\#L`.#n\n\r",ch);
	stc("#L.'______________________________#7\\#0|#7/#L______________________________`.#n\n\r",ch);
	stc("Syntax: magespells (sorcery, enchantment, naturalism, magical) [scribe]#n\n\r",ch);
	return;
	}

//Fuckin Output
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"sorcery"))
	{
	stc("      ___________________   ___________________\n\r",ch);
	stc("  #R.-#7/|       #0~~#P**#0~~      #7\\ /      #0~~#P**#0~~       #7|\\#R-.#n\n\r",ch);
	stc("  #R|#7||#W|   #rS#Rorcery #rS#Rpells   #7:     #CM#cana  #CC#cost     #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	if (ch->pcdata->powers[MAGE_SORCERY] > 0)
	{
	stc("  #R|#7||#W|   #rNerve Blank      #7:        #y2000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_SORCERY] > 1)
	{
	stc("  #R|#7||#W|   #rAcid Arrow       #7:        #y3000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_SORCERY] > 2)
	{
	stc("  #R|#7||#W|   #rEther Blast      #7:        #y3000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_SORCERY] > 3)
	{
	if (IS_SET(ch->sohbits, SOH_SPECFIRE))
	{
	stc("  #R|#7||#W|   #rDemon Fire       #7:        #y5000        #W|#7||#R|#n\n\r",ch);}
else if (IS_SET(ch->sohbits, SOH_SPECICE))
	{
	stc("  #R|#7||#W|   #rIcy Chill        #7:        #y5000        #W|#7||#R|#n\n\r",ch);}
else if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING))
{
	stc("  #R|#7||#W|   #rElectroBolt      #7:        #y5000        #W|#7||#R|#n\n\r",ch);}
else
{
	stc("  #R|#7||#W|   #rElemental Spell  #7:        #y5000        #W|#7||#R|#n\n\r",ch);}
}
else 
{	
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_SORCERY] > 4)
	{
	stc("  #R|#7||#W|   #rEngulf           #7:        #y7000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|       #0~~#C**#0~~       #7:       #0~~#C**#0~~       #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7|/====================#W\\#7:#W/#7====================#W\\#7|#R|#n\n\r",ch);
	stc("  #R`---------------------~#r___#R~--------------------''#n\n\r",ch);
        stc("Syntax: magespells (sorcery, enchantment, naturalism, magical) [scribe]#n\n\r",ch);
	return;
	}
	}


    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"magical"))
	{
	stc("      ___________________   ___________________\n\r",ch);
	stc("  #R.-#7/#W|       #0~~#P**#0~~      #W\\ /      #0~~#P**#0~~       #W|#7\\#R-.#n\n\r",ch);
	stc("  #R|#7||#W|   #lMa#Lgic#7a#Wl S#7p#Lel#lls   #7:     #CM#cana  #CC#cost     #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	if (ch->pcdata->powers[MAGE_MAGIC] > 0)
	{
	stc("  #R|#7||#W|   #LThird Eye        #7:        #y1000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_MAGIC] > 1)
	{
	stc("  #R|#7||#W|   #LScry             #7:        #y1500        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_MAGIC] > 2)
	{
	stc("  #R|#7||#W|   #LTeleport         #7:        #y2000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_MAGIC] > 3)
	{
	stc("  #R|#7||#W|   #LSlow             #7:        #y3000        #W|#7||#R|#n\n\r",ch);}
	else 
	{	
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_MAGIC] > 4)
	{
	stc("  #R|#7||#W|   #LHaste            #7:        #y5000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|       #0~~#C**#0~~       #7:       #0~~#C**#0~~       #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7|/====================#W\\#7:#W/#7====================#W\\#7|#R|#W\n\r",ch);
	stc("  #R`---------------------~#r___#R~--------------------''#n\n\r",ch);
	stc("Syntax: magespells (sorcery, enchantment, naturalism, magical) [scribe]#n\n\r",ch);
	return;
	}
	}

    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"naturalism"))
	{
	stc("      ___________________   ___________________\n\r",ch);
	stc("  #R.-#7/#W|       #0~~#P**#0~~      #W\\ /      #0~~#P**#0~~       #W|#7\\#R-.#n\n\r",ch);
	stc("  #R|#7||#W|  #GN#gaturalism #GS#gpells #7:     #CM#cana  #CC#cost     #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	if (ch->pcdata->powers[MAGE_NATURALISM] > 0)
	{
	stc("  #R|#7||#W|   #GEagle Eye        #7:        #y2000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_NATURALISM] > 1)
	{
	stc("  #R|#7||#W|   #GOgre Strength    #7:        #y3000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_NATURALISM] > 2)
	{
	stc("  #R|#7||#W|   #GLight Shards     #7:        #y5000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_NATURALISM] > 3)
	{
	stc("  #R|#7||#W|   #GSoul Mend        #7:        #y2000        #W|#7||#R|#n\n\r",ch);}
	else 
	{	
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_NATURALISM] > 4)
	{
	stc("  #R|#7||#W|   #GRegenerate       #7:        #y6000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|       #0~~#C**#0~~       #7:       #0~~#C**#0~~       #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7|/====================#W\\#7:#W/#7====================#W\\#7|#R|#W\n\r",ch);
	stc("  #R`---------------------~#r___#R~--------------------''#n\n\r",ch);
	stc("Syntax: magespells (sorcery, enchantment, naturalism, magical) [scribe]#n\n\r",ch);
	return;
	}
	}

    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"enchantment"))
	{
	stc("      ___________________   ___________________\n\r",ch);
	stc("  #R.-#7/#W|       #0~~#P**#0~~      #W\\ /      #0~~#P**#0~~       #W|#7\\#R-.#n\n\r",ch);
	stc("  #R|#7||#W| #oE#ynchantment #oS#ypells #7:     #CM#cana  #CC#cost     #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	if (ch->pcdata->powers[MAGE_ENCHANTMENT] > 0)
	{
	stc("  #R|#7||#W|   #yElemental Rune   #7:        #y2000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_ENCHANTMENT] > 1)
	{
	stc("  #R|#7||#W|   #yChaos Rune       #7:        #y3000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_ENCHANTMENT] > 2)
	{
	stc("  #R|#7||#W|   #yClown Rune       #7:        #y3000        #W|#7||#R|#W\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_ENCHANTMENT] > 3)
	{
	if (IS_SET(ch->sohbits, SOH_SPECFIRE))
	{
	stc("  #R|#7||#W|   #yFire Enchant     #7:       #y10000        #W|#7||#R|#n\n\r",ch);}
else if (IS_SET(ch->sohbits, SOH_SPECICE))
	{
	stc("  #R|#7||#W|   #yFrost Enchant    #7:       #y10000        #W|#7||#R|#n\n\r",ch);}
else if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING))
{
	stc("  #R|#7||#W|   #yLightning Enchant#7:       #y10000        #W|#7||#R|#n\n\r",ch);}
else
{
	stc("  #R|#7||#W|   #yElemental Enchant#7:       #y10000        #W|#7||#R|#n\n\r",ch);}
}
	else 
	{	
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	if (ch->pcdata->powers[MAGE_ENCHANTMENT] > 4)
	{
	stc("  #R|#7||#W|   #yMagic Plating    #7:       #y15000        #W|#7||#R|#n\n\r",ch);}
	else
	{
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);}
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|       #0~~#C**#0~~       #7:       #0~~#C**#0~~       #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7||#W|                    #7:                    #W|#7||#R|#n\n\r",ch);
	stc("  #R|#7|/====================#W\\#7:#W/#7====================#W\\#7|#R|#n\n\r",ch);
	stc("  #R`---------------------~#r___#R~--------------------''#n\n\r",ch);
	stc("Syntax: magespells (sorcery, enchantment, naturalism, magical) [scribe]#n\n\r",ch);
	return;
	}
	}


else if (!str_cmp(arg2,"scribe") && !str_cmp(arg1,"sorcery"))
    {
	int cost;
	int max = 5;

	cost = (ch->pcdata->powers[MAGE_SORCERY]+1) * 1000000;
	if ( ch->pcdata->powers[MAGE_SORCERY] >= max )
	{
	    xprintf(buf,"#0You've learned as many #rS#Rorcery #rS#Rpells #0as possible at this point in time!#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->exp)
	{
	    xprintf(buf,"#0You need #P%d #0exp to improve this skill any further!#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[MAGE_SORCERY] += 1;
	ch->exp -= cost;
	xprintf(buf,"#0You learn a new #rS#Rorcery #0Spell!#n\n\r");
	send_to_char(buf,ch);
    }

else if (!str_cmp(arg2,"scribe") && !str_cmp(arg1,"enchantment"))
    {
	int cost;
	int max = 5;

	cost = (ch->pcdata->powers[MAGE_ENCHANTMENT]+1) * 1000000;
	if ( ch->pcdata->powers[MAGE_ENCHANTMENT] >= max )
	{
	    xprintf(buf,"#0You've learned as many #oE#ynchantment #oS#ypells #0as possible at this point in time!#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->exp)
	{
	    xprintf(buf,"#0You need #P%d #0exp to improve this skill any further!#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[MAGE_ENCHANTMENT] += 1;
	ch->exp -= cost;
	xprintf(buf,"#0You learn a new #oE#ynchant #oS#ypell!#n\n\r");
	send_to_char(buf,ch);
    }

else if (!str_cmp(arg2,"scribe") && !str_cmp(arg1,"naturalism"))
    {
	int cost;
	int max = 5;

	cost = (ch->pcdata->powers[MAGE_NATURALISM]+1) * 1000000;
	if ( ch->pcdata->powers[MAGE_NATURALISM] >= max )
	{
	    xprintf(buf,"#0You've learned as many #gN#Gaturalism #gS#Gpells #0as possible at this point in time!#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->exp)
	{
	    xprintf(buf,"#0You need #P%d #0exp to improve this skill any further!#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[MAGE_NATURALISM] += 1;
	ch->exp -= cost;
	xprintf(buf,"#0You learn a new #gN#Gaturalism #0Spell!#n\n\r");
	send_to_char(buf,ch);
    }

else if (!str_cmp(arg2,"scribe") && !str_cmp(arg1,"magic"))
    {
	int cost;
	int max = 5;

	cost = (ch->pcdata->powers[MAGE_MAGIC]+1) * 1000000;
	if ( ch->pcdata->powers[MAGE_MAGIC] >= max )
	{
	    xprintf(buf,"#0You've learned as many #lM#Lagical #lS#Lpells #0as possible at this point in time!#n\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->exp)
	{
	    xprintf(buf,"#0You need #P%d #0exp to improve this skill any further!#n\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[MAGE_MAGIC] += 1;
	ch->exp -= cost;
	xprintf(buf,"#0You learn a new #lM#Lagical #0Spell!#n\n\r");
	send_to_char(buf,ch);
    }
    else send_to_char("#PTo learn spells, type #0spells #c<#Csorcery#c/#Cenchantment#c/#Cnaturalism#c/#Cmagic#c> #0Scribe.#n\n\r",ch);
	return;
}

void do_specialize( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MAGE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING) || IS_SET(ch->sohbits, SOH_SPECFIRE) || IS_SET(ch->sohbits, SOH_SPECICE))
	{
	stc("#PYou've already specialized your magic!#n\n\r",ch);
	return;
	}

    if (arg1[0] == '\0')
	{
	stc("#0Syntax: #PSpecialize #C<#ylightning#0/#Cice#0/#Rfire#C>#n\n\r",ch);
	return;
	}

	if (!str_cmp(arg1,"lightning"))
	{
	stc("#0You specialize in #oLi#ygh#Wtn#yin#yg M#Wa#ygi#oc!#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPECLIGHTNING);
	xprintf(buf,"#P%s #0has specialized in #oLi#ygh#Wtn#yin#yg M#Wa#ygi#oc!#n",ch->pcdata->switchname);
	do_info(ch,buf);
	return;
	}

	else if (!str_cmp(arg1,"ice"))
	{
	stc("#0You specialize in #LI#Cc#7e M#Wa#Cgi#Lc!#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPECICE);
	xprintf(buf,"#P%s #0has specialized in #LI#Cc#7e M#Wa#Cgi#Lc!#n",ch->pcdata->switchname);
	do_info(ch,buf);
	return;
	}

	else if (!str_cmp(arg1,"fire"))
	{
	stc("#0You specialize in #rF#Ri#yr#7e #WM#7a#yg#Ri#rc!#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPECFIRE);
	xprintf(buf,"#P%s #0has specialized in #rF#Ri#yr#7e #WM#7a#yg#Ri#rc!#n",ch->pcdata->switchname);
	do_info(ch,buf);
	return;
	}

	else
	stc("#0Syntax: #PSpecialize #C<#ylightning#0/#Cice#0/#Rfire#C>#n\n\r",ch);
	return;
	}


void do_sorchant( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj = NULL;
	char arg1[MAX_INPUT_LENGTH];
	int cost;
	int failure; /*Chance of your spell to fizzle*/
    int chance; /*% to check for success/failure*/
	bool sorcstaff = FALSE;

	chance = number_range(1, 100);
	argument = one_argument( argument, arg1 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MAGE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (IS_SET(ch->sohbits, SOH_SPELLREADY))
	{
	stc("#PYou've already got a spell readied!  Type 'mindblank' to clear it!#n\n\r",ch);
	return;
	}

	if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
	{
	stc("You cannot speak! The room has been silenced!\n\r",ch);
	return;
	}


    if (arg1[0] == '\0')
    {
	stc("#0Chant which Spell?#n\n\r",ch);
	return;
	}
	
	if (((obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL) && (obj->pIndexData->vnum == 33000))
	sorcstaff = TRUE;
	else sorcstaff = FALSE;

	/*We'll start first with Magical Spells*/
	if (!str_cmp(arg1,"thirdeye"))
	{
		int lag;
		cost = 1000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 5 - ch->pcdata->powers[MAGE_MSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
			

		if (ch->pcdata->powers[MAGE_MAGIC] < 1)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		ch->mana -= cost;
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"newt eye",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLTRUESIGHT);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	if (sorcstaff == FALSE)
	{
	extract_obj( obj );	
	stc("#0Your spell component #Rflashes #0and #Rburns #0out of reality!#n\n\r",ch);	
	}
	WAIT_STATE(ch, lag);
	return;
	}
	
	else if (!str_cmp(arg1,"scry"))
	{
		int lag;
		cost = 1500;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 5 - ch->pcdata->powers[MAGE_MSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
			

		if (ch->pcdata->powers[MAGE_MAGIC] < 2)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		ch->mana -= cost;
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"eye beholder",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSCRY);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	if (sorcstaff == FALSE)
	stc("#0Your spell component #gg#Gl#7o#Gw#gs #ybrightly#0!#n\n\r",ch);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"teleport"))
	{
		int lag;
		cost = 2000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 5 - ch->pcdata->powers[MAGE_MSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}
		
		if (ch->position == POS_FIGHTING)
		{
		stc("Not in a fight!\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_MAGIC] < 3)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLTELEPORT);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"slow"))
	{
		int lag;
		cost = 3000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 12 - ch->pcdata->powers[MAGE_MSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
			

		if (ch->pcdata->powers[MAGE_MAGIC] < 4)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"spirit plasma",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSLOW);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
    ch->mana -= cost;
	stc("#0Your spell component #gg#Gl#7o#Gw#gs #ybrightly#0!#n\n\r",ch);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"haste"))
	{
		int lag;
		cost = 5000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 17 - ch->pcdata->powers[MAGE_MSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
			

		if (ch->pcdata->powers[MAGE_MAGIC] < 5)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"monkey paw",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLHASTE);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
    ch->mana -= cost;
	stc("#0Your spell component #gg#Gl#7o#Gw#gs #ybrightly#0!#n\n\r",ch);
	WAIT_STATE(ch, lag);
	return;
	}

	/*Sorcery Spells*/
	else if (!str_cmp(arg1,"nerveblank"))
	{
		int lag;
		cost = 2000;
		failure = 110 - ch->pcdata->powers[MAGE_SSU] * 20;
		lag = 5 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		act("#PY#0ou begin chanting the spidery language of magic...#n",ch,NULL,NULL,TO_CHAR);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
                ch->mana -= cost;
		if (ch->pcdata->powers[MAGE_SORCERY] < 1)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"white powder",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSORC1);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	if (sorcstaff == FALSE)
	{
	extract_obj( obj );	
	stc("#0Your spell component #Rflashes #0and #Rburns #0out of reality!#n\n\r",ch);
	}
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"acidarrow"))
	{
		int lag;
		cost = 3000;
		failure = 110 - ch->pcdata->powers[MAGE_SSU] * 20;
		lag = 11 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_SORCERY] < 2)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"weapon arrowhead",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSORC2);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	if (sorcstaff == FALSE)
	stc("#0Your spell component #gg#Gl#7o#Gw#gs #ybrightly#0!#n\n\r",ch);    
	WAIT_STATE(ch, lag);
	return;
	}	

	else if (!str_cmp(arg1,"etherblast"))
	{
		int lag;
		cost = 3000;
		failure = 110 - ch->pcdata->powers[MAGE_SSU] * 20;
		lag = 13 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_SORCERY] < 3)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"wyrm tooth",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSORC3);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
    ch->mana -= cost;
	stc("#0Your spell component #gg#Gl#7o#Gw#gs #ybrightly#0!#n\n\r",ch);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"icychill"))
	{
		int lag;
		cost = 5000;
		failure = 110 - ch->pcdata->powers[MAGE_SSU] * 20;
		lag = 11 - ch->pcdata->powers[MAGE_SSU];
		
		if (!IS_SET(ch->sohbits, SOH_SPECICE))
		{
		stc("#CYou are not specialized in that elemental field!#n\n\r",ch);
		return;
		}

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_SORCERY] < 4)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSORC4);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"demonfire"))
	{
		int lag;
		cost = 5000;
		failure = 110 - ch->pcdata->powers[MAGE_SSU] * 20;
		lag = 11 - ch->pcdata->powers[MAGE_SSU];
		
		if (!IS_SET(ch->sohbits, SOH_SPECFIRE))
		{
		stc("#CYou are not specialized in that elemental field!#n\n\r",ch);
		return;
		}

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_SORCERY] < 4)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSORC4);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"electrobolt"))
	{
		int lag;
		cost = 5000;
		failure = 110 - ch->pcdata->powers[MAGE_SSU] * 20;
		lag = 11 - ch->pcdata->powers[MAGE_SSU];
		
		if (!IS_SET(ch->sohbits, SOH_SPECLIGHTNING))
		{
		stc("#CYou are not specialized in that elemental field!#n\n\r",ch);
		return;
		}

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_SORCERY] < 4)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSORC4);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"engulf"))
	{
		int lag;
		cost = 7000;
		failure = 110 - ch->pcdata->powers[MAGE_SSU] * 20;
		lag = 13 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_SORCERY] < 5)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"spider web",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSORC5);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	if (sorcstaff == FALSE)
	{
	extract_obj( obj );	
	stc("#0Your spell component #Rflashes #0and #Rburns #0out of reality!#n\n\r",ch);
	}
	WAIT_STATE(ch, lag);
	return;
	}

	/*Naturalism Spells*/
	else if (!str_cmp(arg1,"eagleeye"))
	{
		int lag;
		cost = 2000;
		failure = 110 - ch->pcdata->powers[MAGE_NSU] * 20;
		lag = 5 - ch->pcdata->powers[MAGE_NSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_NATURALISM] < 1)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"eagle eye",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLACCURACY);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	if (sorcstaff == FALSE)
	{
	extract_obj( obj );	
	stc("#0Your spell component #Rflashes #0and #Rburns #0out of reality!#n\n\r",ch);
	}
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"ogrestrength"))
	{
		int lag;
		cost = 3000;
		failure = 110 - ch->pcdata->powers[MAGE_NSU] * 20;
		lag = 5 - ch->pcdata->powers[MAGE_NSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_NATURALISM] < 2)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"Mandrake Root",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLMIGHT);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	if (sorcstaff == FALSE)
	{
	extract_obj( obj );	
	stc("#0Your spell component #Rflashes #0and #Rburns #0out of reality!#n\n\r",ch);
	}
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"lightshards"))
	{
		int lag;
		cost = 5000;
		failure = 110 - ch->pcdata->powers[MAGE_NSU] * 20;
		lag = 12 - ch->pcdata->powers[MAGE_NSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_NATURALISM] < 3)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"crystal",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLSHARDS);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	if (sorcstaff == FALSE)
	{
	extract_obj( obj );	
	stc("#0Your spell component #Rflashes #0and #Rburns #0out of reality!#n\n\r",ch);
	}
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"soulmend"))
	{
		int lag;
		cost = 2000;
		failure = 110 - ch->pcdata->powers[MAGE_NSU] * 20;
		lag = 15 - ch->pcdata->powers[MAGE_NSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_NATURALISM] < 4)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLHEAL);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"regen"))
	{
		int lag;
		cost = 6000;
		failure = 110 - ch->pcdata->powers[MAGE_NSU] * 20;
		lag = 17 - ch->pcdata->powers[MAGE_NSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_NATURALISM] < 5)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( (obj = get_obj_wear(ch,"lizard tail",TRUE)) == NULL )         
		{
        send_to_char( "You lack the spell component!\n\r", ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLREGEN);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	if (sorcstaff == FALSE)
	{
	extract_obj( obj );	
	stc("#0Your spell component #Rflashes #0and #Rburns #0out of reality!#n\n\r",ch);
	}
	WAIT_STATE(ch, lag);
	return;
	}

/*Enchantment Spells.  These don't require components to cast, but you need the runes to enchant stuff.*/
	else if (!str_cmp(arg1,"elemental"))
	{
		int lag;
		cost = 2000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 5 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_ENCHANTMENT] < 1)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLENCHANT1);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"chaos"))
	{
		int lag;
		cost = 3000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 10 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_ENCHANTMENT] < 2)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLENCHANT2);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"metta"))
	{
		int lag;
		cost = 3000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 5 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_ENCHANTMENT] < 3)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLENCHANT3);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"lightningenchant"))
	{
		int lag;
		cost = 10000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 20 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		if (!IS_SET(ch->sohbits, SOH_SPECLIGHTNING))
		{
		stc("#pYou are not specialized in that elemental field!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_ENCHANTMENT] < 4)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLENCHANT4);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"iceenchant"))
	{
		int lag;
		cost = 10000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 20 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		if (!IS_SET(ch->sohbits, SOH_SPECICE))
		{
		stc("#pYou are not specialized in that elemental field!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_ENCHANTMENT] < 4)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLENCHANT4);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"fireenchant"))
	{
		int lag;
		cost = 10000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 20 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		if (!IS_SET(ch->sohbits, SOH_SPECFIRE))
		{
		stc("#pYou are not specialized in that elemental field!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_ENCHANTMENT] < 4)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLENCHANT4);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else if (!str_cmp(arg1,"magicplating"))
	{
		int lag;
		cost = 15000;
		failure = 110 - ch->pcdata->powers[MAGE_MSU] * 20;
		lag = 20 - ch->pcdata->powers[MAGE_SSU];

		if (ch->mana < cost)
		{
		stc("#pN#Pot enough mana!#n\n\r",ch);
		return;
		}

		stc("#PY#0ou begin chanting the spidery language of magic...#n\n\r",ch);
		act("#C$n #0begins chanting the spidery language of magic...#n",ch, NULL, NULL, TO_ROOM);	
		ch->mana -= cost;	

		if (ch->pcdata->powers[MAGE_ENCHANTMENT] < 5)
		{
		stc("#pYou attempt to cast the spell, but alas you haven't the knowledge.#n\n\r",ch);
		stc("#CYour spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
        }

		if ( failure >= chance ) 
		{
		stc("#CYou mispronounce a syllable and your spell fizzles and leaves your mind!#n\n\r",ch);
		WAIT_STATE(ch, lag);		
		return;
		}
	
	stc("#pY#Pour spell comes to mind.#n\n\r",ch);
	SET_BIT(ch->sohbits, SOH_SPELLENCHANT5);
	SET_BIT(ch->sohbits, SOH_SPELLREADY);
	WAIT_STATE(ch, lag);
	return;
	}

	else
	{
	stc("#pY#Pou have no knowledge of that spell!#n\n\r",ch);
	return;
	}
	}

void do_mindblank( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MAGE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (!IS_SET(ch->sohbits, SOH_SPELLREADY))
	{
	stc("#CYou don't have a spell readied!#n\n\r",ch);
	return;
	}

	stc("#PYou force your mind to blank, removing your spell from memory#n\n\r",ch);

	if (IS_SET(ch->sohbits, SOH_SPELLREADY))
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	if (IS_SET(ch->sohbits, SOH_SPELLTRUESIGHT))
	REMOVE_BIT(ch->sohbits, SOH_SPELLTRUESIGHT);
	if (IS_SET(ch->sohbits, SOH_SPELLSCRY))
	REMOVE_BIT(ch->sohbits, SOH_SPELLSCRY);
	if (IS_SET(ch->sohbits, SOH_SPELLTELEPORT))
	REMOVE_BIT(ch->sohbits, SOH_SPELLTELEPORT);
	if (IS_SET(ch->sohbits, SOH_SPELLSLOW))
	REMOVE_BIT(ch->sohbits, SOH_SPELLSLOW);
	if (IS_SET(ch->sohbits, SOH_SPELLHASTE))
	REMOVE_BIT(ch->sohbits, SOH_SPELLHASTE);
	if (IS_SET(ch->sohbits, SOH_SPELLACCURACY))
	REMOVE_BIT(ch->sohbits, SOH_SPELLACCURACY);
	if (IS_SET(ch->sohbits, SOH_SPELLMIGHT))
	REMOVE_BIT(ch->sohbits, SOH_SPELLMIGHT);
	if (IS_SET(ch->sohbits, SOH_SPELLSHARDS))
	REMOVE_BIT(ch->sohbits, SOH_SPELLSHARDS);
	if (IS_SET(ch->sohbits, SOH_SPELLHEAL))
	REMOVE_BIT(ch->sohbits, SOH_SPELLHEAL);
	if (IS_SET(ch->sohbits, SOH_SPELLREGEN))
	REMOVE_BIT(ch->sohbits, SOH_SPELLREGEN);
	if (IS_SET(ch->sohbits, SOH_SPELLSORC1))
	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC1);
	if (IS_SET(ch->sohbits, SOH_SPELLSORC2))
	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC2);
	if (IS_SET(ch->sohbits, SOH_SPELLSORC3))
	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC3);
	if (IS_SET(ch->sohbits, SOH_SPELLSORC4))
	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC4);
	if (IS_SET(ch->sohbits, SOH_SPELLSORC5))
	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC5);
	if (IS_SET(ch->sohbits, SOH_SPELLENCHANT1))
	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT1);
	if (IS_SET(ch->sohbits, SOH_SPELLENCHANT2))
	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT2);
	if (IS_SET(ch->sohbits, SOH_SPELLENCHANT3))
	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT3);
	if (IS_SET(ch->sohbits, SOH_SPELLENCHANT4))
	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT4);
	if (IS_SET(ch->sohbits, SOH_SPELLENCHANT5))
	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT5);
	return;
}

/*All this is invoked below*/
void do_magesight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

	if (!IS_CLASS(ch,CLASS_MAGE))
	{
	stc("Huh?\n\r",ch);
	return;
	}
	
	if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "#cYou close your third eye.#n\n\r", ch );
	act("#c$n #0closes their third eye.#n\n\r",ch, NULL, NULL, TO_ROOM);	
	return;
	}
	
	else
	{
	stc("#cYou open your third eye, and it #Cglows #ybrightly#C!#n\n\r",ch);
	act("#c$n #0opens their third eye, and it #Cglows #ybrightly#C!#n\n\r",ch, NULL, NULL, TO_ROOM);	
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	return;
	}
}

void do_magescry( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];
    int rand_room;
    bool chaosvalue = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (victim->trust > 6)
    {
      send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
      send_to_char("Someone is scrying you.\n\r",victim);
      return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim,IMM_SHIELDED))
    {
       send_to_char( "You cannot find them.\n\r",ch);
       return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_SHIELD2)) chaosvalue = TRUE;



    chroom = ch->in_room;
    victimroom = victim->in_room;
    if (chaosvalue)
      for ( ; ; ) 
      {
        rand_room = number_range( 0, 65535 );
        victimroom = get_room_index( rand_room );
        if (victimroom != NULL) break;   
      }

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
	do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    if (!IS_NPC(victim)) ch->fight_timer += 2;
    
}

void do_magegoto( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
    
  if ( IS_NPC(ch) )
    return;

  argument = one_argument (argument, arg);

  if (!IS_CLASS(ch, CLASS_MAGE) )
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    send_to_char( "Teleport to whom?\n\r", ch );
    return;
  }

  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "You cannot find them.\n\r", ch );
    return;
  }

  if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  { 
    stc( "You can't find it's room.\n\r",ch);
    return; 
  } 
  
  if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your body can't go there.\n\r",ch);
    return;
  }

  if (ch == victim)
  {
    send_to_char("But you're already at yourself!\n\r",ch);
    return;
  }
      if (IS_SET(victim->act, ACT_NOTRAVEL))
  {
  	send_to_char("No Can Do.\n\r", ch);
  	return;
  }
  if ( room_is_private(victim->in_room ) )
  {
    send_to_char( "That room is private right now.\n\r", ch );
    return;
  }
  if ( !IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
  { 
    send_to_char( "They are hiding from you.\n\r", ch );
    return;
  }
  if (victim->in_room == ch->in_room)
  {
    send_to_char("But you're already there!\n\r",ch);
    return;
  }

  act("#cYou #Cblink #cout of reality, and #Cte#Glepo#Crt #cto #P$N!#n", ch, NULL, victim, TO_CHAR);
  act("#c$n #Cblink #cout of reality in a #yflash #cof #Cenergy!#n", ch, NULL, victim, TO_ROOM);
  char_from_room(ch);
  char_to_room(ch,victim->in_room);
  act("#c$n #Cteleports into the room next to #P$N #Cin a #yflash #Cof #yenergy!#n", ch, NULL, victim, TO_NOTVICT);
  act("#c$n #Cteleports into the room in a #yflash #Cof #yenergy!#n", ch, NULL, victim, TO_VICT);
  do_look(ch,"auto");
  WAIT_STATE(ch, 12);
  return;
}

void do_mageslow(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim = NULL;
	argument = one_argument (argument, arg);
	if (IS_NPC(victim)) {stc("You can only use that on the smart creatures.\n\r",ch);return;}
	if (arg[0] == '\0') {stc("Mageslow has no target check this.",ch);return;}
	
		if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	
	if (is_safe(ch, victim))
	{
	stc("You can't use that on them.\n\r",ch);
	return;
	}
	if (IS_SET(victim->sohbits, SOH_SLOW) && !IS_NPC(victim))
	{
	stc("They are already slowed!#n\n\r",ch);
	return;
	}


	act("#C$N #gg#Glow#gs #Ca #od#yull #oy#yellow #Cas their movements begin to #yslow!#n",ch,NULL,victim,TO_CHAR);
	act("#CYou begin to glow a #odull yellow #Cas your movements start to #yslow!#n",ch,NULL,victim,TO_VICT);
	act("#C$N #gg#Gl#yo#Gw#gs #Ca #od#yull #oy#yellow #Cas their movements begin to #yslow!#n",ch,NULL,victim,TO_NOTVICT);
	SET_TIMER(victim, TIMER_SLOW, (ch->pcdata->powers[MAGE_MSU] * 5) );
/*        for (i = 0; i < 10; i ++)
        {
	if (ch->pcdata->powers[MAGE_MSU] > i)
	ADD_TIMER(victim,TIMER_SLOW,2);
 	}
*/
	SET_BIT(victim->sohbits, SOH_SLOW);
	return;
}
	

void do_magehaste(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	argument = one_argument (argument, arg);
	if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	

	if (IS_NPC(victim)) {stc("You can only use that on the smart creatures.\n\r",ch);return;}
	if (arg[0] == '\0') {stc("Mageslow has no target check this.",ch);return;}
	if (IS_SET(victim->sohbits, SOH_HASTE) && !IS_NPC(victim))
	{
	stc("They are already hasted!#n\n\r",ch);
	return;
	}
	act("#l$N #Lbegins moving with blinding speed!#n",ch,NULL,victim,TO_CHAR);
	act("#LYou begin to move with blinding speed!#n",ch,NULL,victim,TO_VICT);
	act("#l$N #Lbegins moving with blinding speed!#n",ch,NULL,victim,TO_NOTVICT);
	SET_TIMER(victim, TIMER_HASTE, (ch->pcdata->powers[MAGE_MSU] * 6) );
/*        for (i = 0; i < 10; i ++)
        {
	if (ch->pcdata->powers[MAGE_MSU] > i)
	ADD_TIMER(victim,TIMER_HASTE,2);
 	}
*/
	SET_BIT(victim->sohbits, SOH_HASTE);
	return;
}

void do_mageaccuracy(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim = NULL;
	argument = one_argument (argument, arg);
	
	if (IS_NPC(victim)) {stc("You can only use that on the smart creatures.\n\r",ch);return;}
	if (arg[0] == '\0') {stc("Mageslow has no target check this.",ch);return;}
	if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	
	
	if (IS_SET(victim->sohbits, SOH_ACCURACY) && !IS_NPC(victim))
	{
	stc("Their accuracy has already been magically enhanced!#n\n\r",ch);
	return;
	}

	act("#g$N #Gbegins to #yglow #Ga light #gg#Gr#ye#Ge#gn #Gcolor.#n",ch,NULL,victim,TO_CHAR);
	act("#GYou begin to #yglow #Ga light #gg#Gr#ye#Ge#gn!#n",ch,NULL,victim,TO_VICT);
	act("#g$N #Gbegins to #yglow #Ga light #gg#Gr#ye#Ge#gn #Gcolor.#n",ch,NULL,victim,TO_NOTVICT);	
	SET_TIMER(victim, TIMER_ACCURACY, (ch->pcdata->powers[MAGE_MSU] * 6) );
/*
        for (i = 0; i < 10; i ++)
        {
	if (ch->pcdata->powers[MAGE_NSU] > i)
	ADD_TIMER(victim,TIMER_ACCURACY,2);
 	}
*/
	SET_BIT(victim->sohbits, SOH_ACCURACY);
	victim->hitroll += 200;
	return;
}

void do_magemight(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim = NULL;
	argument = one_argument (argument, arg);
	
	if (IS_NPC(victim)) {stc("You can only use that on the smart creatures.\n\r",ch);return;}
	if (arg[0] == '\0') {stc("Magemight has no target check this.",ch);return;}
	if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	
	
	if (IS_SET(victim->sohbits, SOH_MIGHT) && !IS_NPC(victim))
	{
	stc("#RTheir might has already been magically enhanced!#n\n\r",ch);
	return;
	}

	act("#0$N #Rbegins to #yglow #0a #ybright #rr#Re#rd c#Ro#rl#Ro#rr#R!#n",ch,NULL,victim,TO_CHAR);
	act("#0You begin to #yglow #0a #ybright #rr#Re#rd c#Ro#rl#Ro#rr#R!#n",ch,NULL,victim,TO_VICT);
	act("#0$N #Rbegins to #yglow #0a #ybright #rr#Re#rd c#Ro#rl#Ro#rr#R!#n",ch,NULL,victim,TO_NOTVICT);	
	SET_TIMER(victim, TIMER_MIGHT, (ch->pcdata->powers[MAGE_MSU] * 6)	);
/*
        for (i = 0; i < 10; i ++)
        {
	if (ch->pcdata->powers[MAGE_NSU] > i)
	ADD_TIMER(victim,TIMER_MIGHT,2);
 	}
*/
	SET_BIT(victim->sohbits, SOH_MIGHT);
	victim->damroll += 200;
	return;
}

void do_mageshards(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim = NULL;
	argument = one_argument (argument, arg);
	
	if (IS_NPC(victim)) {stc("You can only use that on the smart creatures.\n\r",ch);return;}
	if (arg[0] == '\0') {stc("Mageshards has no target check this.",ch);return;}
		if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	
	
	if (IS_SET(victim->sohbits, SOH_SHARDS) && !IS_NPC(victim))
	{
	stc("#CThey are already magically protected!#n\n\r",ch);
	return;
	}

	act("#0$N #Cis surrounded by a #7cr#Cys#7ta#Cli#7ne #cforce shield!#n",ch,NULL,victim,TO_CHAR);
	act("#CYou are surrounded by a #7cr#Cys#7ta#Cli#7ne #cforce shield!#n",ch,NULL,victim,TO_VICT);
	act("#0$N #Cis surrounded by a #7cr#Cys#7ta#Cli#7ne #cforce shield!#n",ch,NULL,victim,TO_NOTVICT);
	SET_TIMER(victim, TIMER_SHARDS, (ch->pcdata->powers[MAGE_MSU] * 6)	);
/*
        for (i = 0; i < 10; i ++)
        {
	if (ch->pcdata->powers[MAGE_NSU] > i)
	ADD_TIMER(victim,TIMER_SHARDS,1);
 	}
*/
	SET_BIT(victim->sohbits, SOH_SHARDS);
	victim->armor -= 200;
	return;
}

void do_mageheal(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int lowamt;
	int highamt;
	int amt;

	argument = one_argument (argument, arg);
	
	lowamt = ch->pcdata->powers[MAGE_NSU] * 300;
	highamt = ch->pcdata->powers[MAGE_NSU] * 600;
	amt = number_range(highamt,lowamt);
		if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	

	if (IS_NPC(victim)) {stc("You can only use that on the smart creatures.\n\r",ch);return;}
	if (arg[0] == '\0') {stc("Mageheal has no target check this.",ch);return;}


	act("#cA #Cglowing #Wwhite #7light #Csinks into #c$N!#n",ch,NULL,victim,TO_CHAR);
	act("#cA #Cglowing #Wwhite #7light #Csinks into you!#n",ch,NULL,victim,TO_VICT);
	act("#cA #Cglowing #Wwhite #7light #Csinks into #c$N!#n",ch,NULL,victim,TO_NOTVICT);	
	victim->hit += amt;
	if (victim->hit > victim->max_hit)
	{
	victim->hit = victim->max_hit;
	}
	return;
}

void do_mageregen(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	argument = one_argument (argument, arg);
		if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	
	
	if (IS_NPC(victim)) {stc("You can only use that on the smart creatures.\n\r",ch);return;}
	if (arg[0] == '\0') {stc("Mageregen has no target check this.",ch);return;}
	if (IS_SET(victim->sohbits, SOH_REGEN) && !IS_NPC(victim))
	{
	stc("#CThey are already magically regenerating!#n\n\r",ch);
	return;
	}
        
	act("#C$N #cbegins to #yglow #Gbright #gg#Gree#gn, #Cand they start to #gr#Gegenerate!#n",ch,NULL,victim,TO_CHAR);
	act("#CYou #cbegin to #yglow #Gbright #gg#Gree#gn, #Cand start to #gr#Gegenerate!#n",ch,NULL,victim,TO_VICT);
	act("#C$N #cbegins to #yglow #Gbright #gg#Gree#gn, #Cand they start to #gr#Gegenerate!#n",ch,NULL,victim,TO_NOTVICT);	
	SET_TIMER(victim, TIMER_REGEN, (ch->pcdata->powers[MAGE_MSU] * 3) );
/*
        for (i = 0; i < 10; i ++)
        {
	if (ch->pcdata->powers[MAGE_NSU] > i)
	ADD_TIMER(victim,TIMER_REGEN,2);
 	}
*/
	SET_BIT(victim->sohbits, SOH_REGEN);
	victim->hit += 100;
	return;
}



void do_nerveblank( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *vch;
    CHAR_DATA *vch_next;
	int dam;
	int lowdam;
	int highdam;
	char buf[MAX_STRING_LENGTH];

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;
	lowdam = ch->pcdata->powers[MAGE_SSU] * 200;
	highdam = ch->pcdata->powers[MAGE_SSU] * 400;
	
	dam = number_range(lowdam, highdam);
	if (!IS_NPC(vch) && is_safe(ch, vch)) continue;
	if (vch != ch)
	hurt_person(ch, vch, dam);

		if (vch != ch){
	xprintf(buf, "The room sparks and flares with #7white light #Was you feel #rp#Rai#rn #Wshoot through your nerves! #0[#P%d#0]#n\n\r",dam);
	stc(buf, vch);
	xprintf(buf, "#0You cause the room to #Rfl#ya#Rre #0with #Rintense #7w#White #7l#Wight! #0[#P%d#0]#n\n\r", dam);
	stc(buf, ch);	
	}
	
		if (vch != ch)
	stc("#RYou fall to the ground, stunned!\n\r",vch);
	
		if ((vch != ch) && (IS_NPC(vch))){
	xprintf(buf, "#r%s #Rfalls to the ground, stunned!\n\r", vch->short_descr);
	stc(buf, ch);}
	else if (vch != ch)
	{
	xprintf(buf, "#r%s #Rfalls to the ground, stunned!\n\r", vch->name);
	stc(buf, ch);
	}

	if (vch != ch)
	vch->position = POS_STUNNED;

    }

	ch->fight_timer += 5;
	if (number_range(1,10) > 4) WAIT_STATE(ch, 12);
	return;
}

void do_acidarrow( CHAR_DATA *ch, char *argument)
{
		CHAR_DATA *victim;
		ROOM_INDEX_DATA *location;
		char arg[MAX_INPUT_LENGTH];
		char buf[MAX_STRING_LENGTH];
		int dam;
		int highdam;
		int lowdam;

		argument = one_argument( argument, arg );

		lowdam = ch->pcdata->powers[MAGE_SSU] * 300;
		highdam = ch->pcdata->powers[MAGE_SSU] * 600;

		dam = number_range(lowdam, highdam);
		dam += (ch->damcap[DAM_CAP] / 4);

			if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	
	
	if ( (location = ch->in_room) == NULL)
	{
	stc("The spell failed.\n\r",ch);
        return;	
	}
        if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
        {
	  stc("The spell fizzled out.\n\r",ch);
	  return;
	}

        if ( ( victim = get_char_world( ch, arg ) ) == NULL )
        {send_to_char("The spell failed.\n\r",ch);return;}

	if (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) && (ch->in_room != victim->in_room))
	{
	stc("You may not bomb from a no_teleport room!\n\r",ch);
	return;
	}
	
	if (!IS_NPC(victim) && (ch->in_room->area != victim->in_room->area))
	{
	stc("You must be in the same area as your victim, if they are a PC!\n\r",ch);
	return;
	}

	if (is_safe(ch, victim))
	{
	  stc("A powerful force causes your spell to explode before your eyes.\n\r",ch);
	  hurt_person(ch,ch,number_range(1,1000));
	  return;	  
	}
   
		if (victim->position == POS_SLEEPING || victim->position == POS_RESTING)
		dam *= 2.5;
		
		if (IS_NPC(victim))
		{
		dam *= 2.5;
		xprintf(buf,"#0You send forth #0an arrow of #gb#Gub#gbl#Gin#gg #ga#Gc#yi#Gd #0to strike #R$N!  #0[#P%d#0]#n",dam);
        act(buf,ch,NULL,victim,TO_CHAR);
		}

		else if (!IS_NPC(victim))
		{
                if(IS_IMMUNE(victim, IMM_ACID)) dam = dam / 3;
		xprintf(buf,"#0You send forth #0an arrow of #gb#Gub#gbl#Gin#gg #ga#Gc#yi#Gd #0to strike #R$N!  #0[#P%d#0]#n",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		}

		xprintf(buf,"#C$n #0sends forth an arrow of #gb#Gub#gbl#Gin#gg a#Gc#yi#Gd #0to strike #P$N!#n");
		act(buf,ch,NULL,victim,TO_NOTVICT);		
		xprintf(buf,"#C$n's #ga#Gc#yi#Gd #ga#Gr#gr#Go#gw #0strikes you, #Rcorroding #0flesh!  #0[#P%d#0]#n",dam);
		act(buf,ch,NULL,victim,TO_VICT);

		if (!IS_NPC(victim))
		{
		if (!IS_NPC(victim) && ch->fight_timer <= 18) ch->fight_timer += 6;
		if (!IS_NPC(victim) && victim->fight_timer <= 3) victim->fight_timer += 2;
		victim->fight_timer += number_range(2,4);
		}
		hurt_person(ch, victim, dam);

		if (number_percent() > 65 && victim->hit > 0)
		{
		if (IS_NPC(victim))
		{
		xprintf(buf,"#0You send forth #0an arrow of #gb#Gub#gbl#Gin#gg #ga#Gc#yi#Gd #0to strike #R$N!  #0[#P%d#0]#n",dam);
        act(buf,ch,NULL,victim,TO_CHAR);
		}

		else if (!IS_NPC(victim))
		{
		xprintf(buf,"#0You send forth #0an arrow of #gb#Gub#gbl#Gin#gg #ga#Gc#yi#Gd #0to strike #R$N!  #0[#P%d#0]#n",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		}

		xprintf(buf,"#C$n #0sends forth an arrow of #gb#Gub#gbl#Gin#gg a#Gc#yi#Gd #0to strike #P$N!#n");
		act(buf,ch,NULL,victim,TO_NOTVICT);		
		xprintf(buf,"#C$n's #ga#Gc#yi#Gd #ga#Gr#gr#Go#gw #0strikes you, #Rcorroding #0flesh!  #0[#P%d#0]#n",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		if (!IS_NPC(victim))
		{
		if (!IS_NPC(victim) && victim->fight_timer <= 18) ch->fight_timer += 6;
		if (!IS_NPC(victim) && victim->fight_timer <= 3) victim->fight_timer += 2;
		}
		hurt_person(ch, victim, dam);
		}

		return;
	}

void do_etherblast( CHAR_DATA *ch, char *argument)
{
		CHAR_DATA *victim;
		char arg[MAX_INPUT_LENGTH];
		char buf[MAX_STRING_LENGTH];
		int dam;
		int highdam;
		int lowdam;
		int highenergy;
		int lowenergy;
		int energy;

		argument = one_argument( argument, arg );
		if (IS_NPC(ch)) return;

		lowdam = (ch->pcdata->powers[MAGE_SSU]+1) * 300;
		highdam = (ch->pcdata->powers[MAGE_SSU]+1) * 500;

		dam = number_range(lowdam, highdam);

		lowenergy = dam - 500;
		highenergy = dam + 2000;
		
		dam += ch->damcap[DAM_CAP]-500;
		energy = number_range(lowenergy, highenergy);
		if (dam < 0) dam = 0;
		if (energy < 0) energy = 1;
	

			if ((victim = get_char_world(ch, arg)) == NULL)
	{
	  send_to_char("The spell failed.\n\r",ch);
	return;
	}	
	    if (is_safe( ch, victim )) return;
		
        if ( ( victim = get_char_world( ch, arg ) ) == NULL )
        {send_to_char("The spell failed.\n\r",ch);return;}

		if (IS_NPC(victim))
		{
		xprintf(buf,"#0You shoot forth a #pb#Polt of #pe#Pther #0at #C$N!  #0[#P%d#0]#n",dam);
        act(buf,ch,NULL,victim,TO_CHAR);
		}

		else if (!IS_NPC(victim))
		{
		xprintf(buf,"#0You shoot forth a #pb#Polt of #pe#Pther #0at #C$N!  #0[#P%d#0]#n",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		}

		xprintf(buf,"#C$n #0sends forth a #pb#Polt of #pe#Pther #0to strike #P$N!#n");
		act(buf,ch,NULL,victim,TO_NOTVICT);		
		xprintf(buf,"#C$n's #pb#Polt of #pe#Pther #0strikes you, #Rsearing flesh!  #0[#P%d#0]#n",dam);
		act(buf,ch,NULL,victim,TO_VICT);

		hurt_person(ch, victim, dam);
		xprintf(buf,"#0You channel some of the #pe#Pther #0back into your body!  #0[#C%d#0]#n\n\r",energy);
		stc(buf,ch);
		ch->mana += energy;
		if (ch->mana > ch->max_mana) ch->mana = ch->max_mana;
		if (!IS_NPC(victim))
		{
		if (ch->fight_timer < 15)
		ch->fight_timer += 3;
		if (victim->fight_timer < 1)
		victim->fight_timer += 2;
		}
		return;
	}

void do_magesorc4( CHAR_DATA *ch, char *argument)
{
		CHAR_DATA *victim;
		char arg[MAX_INPUT_LENGTH];
		char buf[MAX_STRING_LENGTH];
		int dam;
		int highdam = 0;
		int lowdam = 0;

		argument = one_argument( argument, arg );

	if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "#0They aren't here.#n\n\r", ch );
	return;
    }
		
		if ((victim == ch))
		{
		stc("#RYeah ok...\n\r",ch);
		return;
		}
		if (is_safe(ch, victim))
		{
		stc("#RThey are #Wsafe #Rfrom your magic!#n\n\r",ch);
		return;
		}
		if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING))
		{
		lowdam = ch->pcdata->powers[MAGE_SSU] * 700;
		highdam = ch->pcdata->powers[MAGE_SSU] * 1000;
		}

		if (IS_SET(ch->sohbits, SOH_SPECFIRE))
		{
		lowdam = ch->pcdata->powers[MAGE_SSU] * 400;
		highdam = ch->pcdata->powers[MAGE_SSU] * 1200;
		}

		if (IS_SET(ch->sohbits, SOH_SPECICE))
		{
		lowdam = ch->pcdata->powers[MAGE_SSU] * 500;
		highdam = ch->pcdata->powers[MAGE_SSU] * 1100;
		}

		dam = number_range(lowdam, highdam);
		dam += ch->damcap[DAM_CAP];
		dam /= 2;
		
		//if (victim->position == POS_SLEEPING || victim->position == POS_RESTING)
		//dam *= 2.5;

		//if (IS_NPC(victim)) dam *= 3;
		//if (IS_SET(ch->sohbits, SOH_SPECICE) && IS_CLASS(victim, CLASS_DEMON)) dam *= 1.5;
		//if (IS_SET(ch->sohbits, SOH_SPECICE) && IS_CLASS(victim, CLASS_GHOUL)) dam *= .6;
		//if (IS_SET(ch->sohbits, SOH_SPECFIRE) && IS_CLASS(victim, CLASS_GHOUL)) dam *= 1.5;		
		//if (IS_SET(ch->sohbits, SOH_SPECFIRE) && IS_CLASS(victim, CLASS_DEMON)) dam *= .5;		
		//if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING) && IS_CLASS(victim, CLASS_SKYBLADE)) dam *= .8;
		//if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING) && IS_CLASS(victim, CLASS_THIEF)) dam *= 1.3;
		if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING))
		{
                        if(IS_IMMUNE(victim, IMM_LIGHTNING)) dam = dam / 2;
			xprintf(buf,"#0Your #oE#yle#7ct#yro#oB#yol#ot #Rstrikes #P$N, #0sending a shower of #ysp#7a#Wr#yks #0everywhere!  #0[#P%d#0]#n",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"#C$n's #oE#yle#7ct#yro#oB#yol#ot #Rstrikes #0you, sending a shower of #ysp#7a#Wr#yks #0everywhere!  #0[#P%d#0]#n",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			act("#C$n's #oE#yle#7ct#yro#oB#yol#ot #Rstrikes #P$N, #0sending a shower of #ysp#7a#Wr#yks #0everywhere!#n",ch,NULL,victim,TO_NOTVICT);
		}

		else if (IS_SET(ch->sohbits, SOH_SPECFIRE))
		{
                        if(IS_IMMUNE(victim, IMM_HEAT)) dam = dam / 2;
			xprintf(buf,"#0Your #rD#Re#ym#7o#Rn#rF#Ri#yr#Re #rs#Rco#7rc#Rhe#rs #y$N, #0consuming them in #Rf#yl#Ra#ym#Re#ys#R!  #0[#P%d#0]#n",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"#y$n's #rD#Re#ym#7o#Rn#rF#Ri#yr#Re #rs#Rco#7rc#Rhe#rs you, #0consuming you in #Rf#yl#Ra#ym#Re#ys#R!  #0[#P%d#0]#n",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"#y$n's #rD#Re#ym#7o#Rn#rF#Ri#yr#Re #rs#Rco#7rc#Rhe#rs #y$N, #0consuming them in #Rf#yl#Ra#ym#Re#ys#R!");
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}

		else if (IS_SET(ch->sohbits, SOH_SPECICE))
		{
                        if(IS_IMMUNE(victim, IMM_COLD)) dam = dam / 2;
			xprintf(buf,"#0Your #cI#Cc#Wy #cC#Ch#Wi#Cl#cl #Lblasts #P$N, #0pelting them with #7snow #0and #Cice!  #0[#P%d#0]#n",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"#c$n's #cI#Cc#Wy #cC#Ch#Wi#Cl#cl #Lblasts #0you with ungodly force, #0pelting you with #7snow #0and #Cice!  #0[#P%d#0]#n",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"#c$n's #cI#Cc#Wy #cC#Ch#Wi#Cl#cl #Lblasts #0$N with ungodly force, #0pelting them with #7snow #0and #Cice!");
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}

		hurt_person(ch, victim, dam);		
		if (!IS_NPC(victim)) ch->fight_timer += number_range(0,2);
		if (!IS_NPC(victim)) victim->fight_timer += number_range(0,2);
                WAIT_STATE(ch, 9 );
		if (IS_SET(ch->sohbits, SOH_SPECFIRE) && !IS_AFFECTED(victim, AFF_FLAMING) && victim->hit > 0)		
		{
		act("#RYou ignite in #rf#Rl#yam#Re#rs!#n",ch,NULL,victim,TO_VICT);
		act("#R$N #rignites in #rf#Rl#yam#Re#rs!#n",ch,NULL,victim,TO_CHAR);
		act("#R$N #rignites in #rf#Rl#yam#Re#rs!#n",ch,NULL,victim,TO_NOTVICT);
		SET_BIT(victim->affected_by, AFF_FLAMING);
		}
		return;
	}



void do_mageweb( CHAR_DATA *ch, char *argument)
{
		CHAR_DATA *victim;
		char arg[MAX_INPUT_LENGTH];

		argument = one_argument( argument, arg );

		
        if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	stc("They arent here.\n\r",ch);
	return;
	}	
	
		
        if ( ( victim = get_char_world( ch, arg ) ) == NULL )
        {send_to_char("The spell failed.\n\r",ch);return;}
		
		if ((victim == ch))
		{
		stc("#RYeah ok...\n\r",ch);
		return;
		}
  	if (is_safe(ch, victim))
	{
	stc("#oThey are safe from you!#n\n\r",ch);
	return;
	}
		if (IS_AFFECTED(victim, AFF_WEBBED))		
		{
		stc("#0They are already #gw#Gebbed!#n\n\r",ch);
		return;
		}

		else if (!IS_AFFECTED(victim, AFF_WEBBED))		
		{
		act("#gW#Geeds #0and #gv#Gines #0spring from the #gearth, e#Gngulfing #0you!",ch,NULL,victim,TO_VICT);
		act("#gW#Geeds #0and #gv#Gines #0spring from the #gearth, e#Gngulfing #c$N!#n",ch,NULL,victim,TO_CHAR);
		act("#gW#Geeds #0and #gv#Gines #0spring from the #gearth, e#Gngulfing #c$N!#n",ch,NULL,victim,TO_NOTVICT);
		SET_BIT(victim->affected_by, AFF_WEBBED);
		WAIT_STATE(victim, number_range(1,12));
		WAIT_STATE(ch, 12);
		}
return;
}

void do_wpnenchant(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  OBJ_DATA *obj2;

  char arg[MAX_INPUT_LENGTH];

  one_argument(argument, arg);

  if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
  {
    send_to_char("You dont have that item.\n\r",ch);
    return;
  }

  if (obj->item_type != ITEM_WEAPON)
  {
    send_to_char("You can only put this enchant on weapons!\n\r", ch);
    return;
  }

  if (IS_SET(obj->extra_flags, ITEM_ELECENCHANT) || IS_SET(obj->extra_flags, ITEM_ICEENCHANT) || IS_SET(obj->extra_flags, ITEM_FIREENCHANT))
  {
    send_to_char("#rThis item has already been #0Enchanted!#n.\n\r", ch);
    return;
  }

if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING))
{
	if ( (obj2 = get_obj_wear(ch,"lightning rune",TRUE)) == NULL )         
	{
	stc("#cYou don't have the proper rune!#n\n\r",ch);
	return;
	}
	
	SET_BIT(obj->extra_flags, ITEM_ELECENCHANT);
	stc("#yYour weapon suddenly begins to spark!#n\n\r", ch);
	act("#C$n's #0weapon suddenly begins to #yspark!#n",ch,NULL,NULL,TO_ROOM);
	extract_obj( obj2 );
}

else if (IS_SET(ch->sohbits, SOH_SPECFIRE))
{
	if ( (obj2 = get_obj_wear(ch,"fire Rune",TRUE)) == NULL )         
	{
	stc("#cYou don't have the proper rune!#n\n\r",ch);
	return;
	}
	
	SET_BIT(obj->extra_flags, ITEM_FIREENCHANT);
	stc("#RYour weapon suddenly begins to #rf#Rl#ya#Rm#re!#n\n\r", ch);
	act("#C$n's #0weapon suddenly begins to #rf#Rl#ya#Rm#re!#n",ch,NULL,NULL,TO_ROOM);
	extract_obj( obj2 );
}

else if (IS_SET(ch->sohbits, SOH_SPECICE))
{
	if ( (obj2 = get_obj_wear(ch,"ice rune",TRUE)) == NULL )         
	{
	stc("#cYou don't have the proper rune!#n\n\r",ch);
	return;
	}
	
	SET_BIT(obj->extra_flags, ITEM_ICEENCHANT);
	stc("#CYour weapon is suddenly covered in #cf#Cr#Wo#Cs#ct!#n\n\r", ch);
	act("#C$n's #0weapon is suddenly covered in #cf#Cr#Wo#Cs#ct!#n",ch,NULL,NULL,TO_ROOM);
	extract_obj( obj2 );
}

    WAIT_STATE(ch, 15);
	obj->timer = (ch->pcdata->powers[MAGE_SSU] * 3) + dice(1,3);
	return;
}

/*Spell invoking*/
void do_target( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim = NULL;
 	char arg[MAX_INPUT_LENGTH];
	argument = one_argument(argument,arg);
	
	if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MAGE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (!IS_SET(ch->sohbits, SOH_SPELLREADY))
	{
	stc("#CYou don't have a spell readied!#n\n\r",ch);
	return;
	}
	


	if (IS_SET(ch->sohbits, SOH_SPELLTRUESIGHT))
	{
	REMOVE_BIT(ch->sohbits, SOH_SPELLTRUESIGHT);	
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);	
	do_magesight(ch,"");
	return;
	}

	/*Scry*/
	else if (IS_SET(ch->sohbits,SOH_SPELLSCRY))
	{
/*       char arg [MAX_INPUT_LENGTH];
	
	argument = one_argument( argument, arg );	*/

    if ( arg[0] == '\0' )
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

	REMOVE_BIT(ch->sohbits, SOH_SPELLSCRY);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_magescry(ch,arg);
	return;
	}
	/*Slow*/
	else if (IS_SET(ch->sohbits,SOH_SPELLSLOW))
	{
/*    char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	
	argument = one_argument( argument, arg );	*/

    if ( arg[0] == '\0' )
    {
	send_to_char( "#0Invoke #pS#Plow #0on who?#n\n\r", ch );
	return;
    }
    
    if ( ( victim = get_char_room(ch, NULL, arg) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
    if (IS_NPC(victim))
    {
    stc("Use that on someone with a brain.\n\r",ch);
    return;
    }	
	if (victim == ch)
	{
	stc("Not on yourself!\n\r",ch);
	return;
	}

	REMOVE_BIT(ch->sohbits, SOH_SPELLSLOW);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_mageslow(ch,arg);
	return;
	}
	
	/*Teleport*/
	else if (IS_SET(ch->sohbits,SOH_SPELLTELEPORT))
	{
/*    char arg [MAX_INPUT_LENGTH];
	
	argument = one_argument( argument, arg );	*/

    if ( arg[0] == '\0' )
    {
	send_to_char( "Teleport to whom?\n\r", ch );
	return;
    }

	REMOVE_BIT(ch->sohbits, SOH_SPELLTELEPORT);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_magegoto(ch,arg);
	return;
	}
/*Haste*/
	else if (IS_SET(ch->sohbits,SOH_SPELLHASTE))
	{
/*    char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	
	argument = one_argument( argument, arg );*/

    if ( arg[0] == '\0' )
    {
	send_to_char( "#0Invoke #lH#Laste #0on who?#n\n\r", ch );
	return;
    }
    	if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	

    if (IS_NPC(victim))
    {
    stc("Use that on someone with a brain.\n\r",ch);
    return;
    }	

    if ( ( victim = get_char_room(ch, NULL, arg) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
	

	REMOVE_BIT(ch->sohbits, SOH_SPELLHASTE);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_magehaste(ch,arg);
	return;
	}
/*Naturalism Starts Here*/
/*Eagle Eye*/

	else if (IS_SET(ch->sohbits,SOH_SPELLACCURACY))
	{
/*    char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	
	argument = one_argument( argument, arg );	*/

    if ( arg[0] == '\0' )
    {
	send_to_char( "#0Invoke #gE#Gagle #gE#Gye #0on who?#n\n\r", ch );
	return;
    }
    	if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	
    
	
	if (IS_NPC(victim))
    {
    stc("Use that on someone with a brain.\n\r",ch);
    return;
    }	
    if ( ( victim = get_char_room(ch, NULL, arg) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
	

	REMOVE_BIT(ch->sohbits, SOH_SPELLACCURACY);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_mageaccuracy(ch,arg);
	return;
	}
/*Ogre Might*/
	else if (IS_SET(ch->sohbits,SOH_SPELLMIGHT))
	{
/*    char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	
	argument = one_argument( argument, arg );	*/

    if ( arg[0] == '\0' )
    {
	send_to_char( "#0Invoke #rO#Rgre #rM#Right #0on who?#n\n\r", ch );
	return;
    }
    	if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	

	if (IS_NPC(victim))
    {
    stc("Use that on someone with a brain.\n\r",ch);
    return;
    }	
    if ( ( victim = get_char_room(ch, NULL, arg) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
	

	REMOVE_BIT(ch->sohbits, SOH_SPELLMIGHT);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_magemight(ch,arg);
	return;
	}

/*Light Shards*/
	else if (IS_SET(ch->sohbits,SOH_SPELLSHARDS))
	{
/*    char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	
	argument = one_argument( argument, arg );	*/

    if ( arg[0] == '\0' )
    {
	send_to_char( "#0Invoke #cL#Ci#7g#Cht #cS#Ch#7a#Crds #0on who?#n\n\r", ch );
	return;
    }
    	if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	

	if (IS_NPC(victim))
    {
    stc("Use that on someone with a brain.\n\r",ch);
    return;
    }	
    if ( ( victim = get_char_room(ch, NULL, arg) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
	

	REMOVE_BIT(ch->sohbits, SOH_SPELLSHARDS);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_mageshards(ch,arg);
	return;
	}

/*Soul Mend*/
	else if (IS_SET(ch->sohbits,SOH_SPELLHEAL))
	{
/*    char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	
	argument = one_argument( argument, arg );	*/

    if ( arg[0] == '\0' )
    {
	send_to_char( "#0Invoke #lS#Lo#7u#Ll #lM#Le#7n#Ld #0on who?#n\n\r", ch );
	return;
    }
    	if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	

	if (IS_NPC(victim))
    {
    stc("Use that on someone with a brain.\n\r",ch);
    return;
    }	
    if ( ( victim = get_char_room(ch, NULL, arg) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
	

	REMOVE_BIT(ch->sohbits, SOH_SPELLHEAL);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_mageheal(ch,arg);
	return;
	}

/*Regen*/
	else if (IS_SET(ch->sohbits,SOH_SPELLREGEN))
	{
/*    char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	
	argument = one_argument( argument, arg );	*/

    if ( arg[0] == '\0' )
    {
	send_to_char( "#0Imbue #gR#Gegenerate #0into who?#n\n\r", ch );
	return;
    }
    	if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	

	if (IS_NPC(victim))
    {
    stc("Use that on someone with a brain.\n\r",ch);
    return;
    }	
    if ( ( victim = get_char_room(ch, NULL, arg) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
	

	REMOVE_BIT(ch->sohbits, SOH_SPELLREGEN);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_mageregen(ch,arg);
	return;
	}
   
	/*Sorcery Starts Here*/
	
	else if (IS_SET(ch->sohbits,SOH_SPELLSORC1))
	{
	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC1);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_nerveblank(ch,"");
	return;
	}

	else if (IS_SET(ch->sohbits,SOH_SPELLSORC2))
	{
    if ( arg[0] == '\0' )
    {
	send_to_char( "#0Launch an #gA#Gcid #gA#Grrow #0at whom?#n\n\r", ch );
	return;
    }
    
	if ((victim == ch))
	{
	stc("#RYeah ok...#n\n\r",ch);
	return;
	}
	
		if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	


	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC2);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_acidarrow(ch,arg);
	return;
	}


	else if (IS_SET(ch->sohbits,SOH_SPELLSORC3))
	{
    if ( arg[0] == '\0' )
    {
	send_to_char( "#pE#Pther #pB#Plast #0who?#n\n\r", ch );
	return;
    }
    
	if ((victim == ch))
	{
	stc("#RYeah ok...#n\n\r",ch);
	return;
	}
	
		if ((victim = get_char_world(ch, arg)) == NULL)
	{
	stc("They arent here.\n\r",ch);
	return;
	}	


	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC3);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_etherblast(ch,arg);
	return;
	}

	else if (IS_SET(ch->sohbits,SOH_SPELLSORC4))
	{

    if ( arg[0] == '\0' )
    {
		if (IS_SET(ch->sohbits, SOH_SPECFIRE))
		send_to_char( "#0Invoke #rD#Remon #rF#Rire #0upon whom?#n\n\r", ch );
		if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING))		
		stc("#0Invoke #oE#yle#7ct#yro#oB#yo#7l#yt#W #0upon whom?#n\n\r",ch);
		if (IS_SET(ch->sohbits, SOH_SPECICE))
		stc("#0Invoke #cI#Cc#7y #cC#Ch#7i#Cl#7l #0upon whom?#n\n\r",ch);
	return;
    }
    

    if ( ( victim = get_char_room(ch, NULL, arg) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
	

	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC4);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_magesorc4(ch,arg);
	return;
	}

	else if (IS_SET(ch->sohbits,SOH_SPELLSORC5))
	{

    if ( arg[0] == '\0' )
    {
	stc("#0Invoke #gE#Gngulf #0on whom?\n\r",ch);
	return;
    }

    

    if ( ( victim = get_char_world(ch, arg) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
	

	REMOVE_BIT(ch->sohbits, SOH_SPELLSORC5);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_mageweb(ch,arg);
	return;
	}

//Magic Plating  THis one should prove interesting...
	else if (IS_SET(ch->sohbits,SOH_SPELLENCHANT5))
	{
	OBJ_DATA *obj;
	AFFECT_DATA paf;

	if ( arg[0] == '\0' )
    {
	stc("#0Enchant what??\n\r",ch);
	return;
    }


  if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
  {
    send_to_char("You dont have that item.\n\r",ch);
    return;
  }

  if (obj->item_type != ITEM_ARMOR)
  {
    send_to_char("You can only put this enchant on armor!\n\r", ch);
    return;
  }

  if (IS_SET(obj->quest, QUEST_PLATING))
  {
    send_to_char("#0This item has already been Magically Plated!#n.\n\r", ch);
    return;
  }


  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_AC;
  paf.modifier       = -50;
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);

  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_HIT;
  paf.modifier       = 500;
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);
		
	SET_BIT(obj->quest, QUEST_PLATING);

	act("#0The item is suddenly plated in a magical alloy!#n",ch,NULL,NULL,TO_CHAR);
	act("#0#P$n #0guestures at the object, surrounding it with a magical plating!#n",ch,NULL,NULL,TO_ROOM);

	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT5);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);

    WAIT_STATE(ch, 15);
	return;
	}


//Chaos Rune
	else if (IS_SET(ch->sohbits,SOH_SPELLENCHANT2))
	{
	OBJ_DATA *obj;
	OBJ_DATA *obj2;

	if ( arg[0] == '\0' )
    {
	stc("#0Enchant what??\n\r",ch);
	return;
    }


  if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
  {
    send_to_char("You dont have that item.\n\r",ch);
    return;
  }

  if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
  {
    send_to_char("You can only put this enchant on weapons or armor!\n\r", ch);
    return;
  }

		if ( obj->value[3] > 0 )
	    {
		send_to_char("That item already has a spell power.\n\r",ch);
		return;
	    }

		if ( (obj2 = get_obj_wear(ch,"clown rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		
		obj->value[3] = 15000;
		extract_obj( obj2 );	
	act("#0You engrave the #Prune #0into the item, surrounding it in a shield of #RC#0h#RA#0o#RS#0!#n",ch,NULL,NULL,TO_CHAR);
	act("#0#P$n #0engraves a #Prune #0into his item, surrounding it in a shield of #RC#0h#RA#0o#RS#0!#n",ch,NULL,NULL,TO_ROOM);

	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT2);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	return;
	}


//Elemental Rune
	else if (IS_SET(ch->sohbits,SOH_SPELLENCHANT1))
	{
	OBJ_DATA *obj;
    OBJ_DATA *obj2;
	char spellarg [MAX_STRING_LENGTH];

    argument = one_argument( argument, spellarg );

	if ( arg[0] == '\0' )
    {
	stc("#0Enchant what??\n\r",ch);
	return;
    }

	if (spellarg[0] == '\0')
	{
	stc("#yShockshield, #gA#Gcid#gS#Ghield, #rF#Rireshiel#rd, #cI#Cce#cS#Chield#n\n\r",ch);
	return;	
	}

  if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
  {
    send_to_char("You dont have that item.\n\r",ch);
    return;
  }

  if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
  {
    send_to_char("You can only put this enchant on weapons or armor!\n\r", ch);
    return;
  }

	if ( obj->value[3] > 0 )
	    {
		send_to_char("That item already has a spell power.\n\r",ch);
		return;
	    }

	if (!str_cmp( spellarg, "acidshield"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"acid rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		obj->value[3] = 13000;
		extract_obj( obj2 );	
	}

	else if (!str_cmp( spellarg, "fireshield"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"fire Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		obj->value[3] = 11000;
		extract_obj( obj2 );	
	act("#0You engrave the #Prune #0into the item, surrounding it in a #Rshield of flames!#n",ch,NULL,NULL,TO_CHAR);
	act("#0#P$n #0engraves a #Prune #0into his item, surrounding it in a #Rshield of flames!#n",ch,NULL,NULL,TO_ROOM);	
	}

	else if (!str_cmp( spellarg, "shockshield"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"lightning rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		obj->value[3] = 10000;
		extract_obj( obj2 );	
	act("#0You engrave the #Prune #0into the item, surrounding it in a #yshield of lightning!#n",ch,NULL,NULL,TO_CHAR);
	act("#0#P$n #0engraves a #Prune #0into his item, surrounding it in a #yshield of lightning!#n",ch,NULL,NULL,TO_ROOM);	
	}

	else if (!str_cmp( spellarg, "iceshield"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"ice rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		obj->value[3] = 12000;
		extract_obj( obj2 );	
	act("#0You engrave the #Prune #0into the item, surrounding it in a #Cshield of frost!#n",ch,NULL,NULL,TO_CHAR);
	act("#0#P$n #0engraves a #Prune #0into his item, surrounding it in a #Cshield of frost!#n",ch,NULL,NULL,TO_ROOM);
	}

	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT1);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	return;
	}

//Metta Rune Enchants
	else if (IS_SET(ch->sohbits,SOH_SPELLENCHANT3))
	{
	OBJ_DATA *obj;
    OBJ_DATA *obj2;
	char spellarg [MAX_STRING_LENGTH];

    argument = one_argument( argument, spellarg );

	if ( arg[0] == '\0' )
    {
	stc("#0Enchant what??\n\r",ch);
	return;
    }

	if (spellarg[0] == '\0')
	{
    send_to_char("#0Spell affects: #CSeeInvis, #LFly, #RInfravision, #0Invis, #cPassdoor, #rProtection_vs_#0evil, #RProtection_vs_#Ggood, #7Sanct, #0Sneak\n\r",ch);
	return;	
	}

  if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
  {
    send_to_char("You dont have that item.\n\r",ch);
    return;
  }

  if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
  {
    send_to_char("You can only put this enchant on weapons or armor!\n\r", ch);
    return;
  }

	if ((obj->item_type == ITEM_WEAPON && obj->value[0] > 0) || (obj->item_type == ITEM_ARMOR && obj->value[3] > 0 ))
	    {
		send_to_char("That item already has a spell power.\n\r",ch);
		return;
	    }

	if (!str_cmp( spellarg, "seeinvis"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"Zarius Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		if (obj->item_type == ITEM_WEAPON)
		{
		obj->value[0] = 2000;
		}

		else if (obj->item_type == ITEM_ARMOR)
		{
		obj->value[3] = 2;
		}
		
		extract_obj( obj2 );	
	stc("#PYou carve the Metta Rune into the object.#n\n\r",ch);
	}

	if (!str_cmp( spellarg, "fly"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"Zarius Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		if (obj->item_type == ITEM_WEAPON)
		{
		obj->value[0] = 3000;
		}

		else if (obj->item_type == ITEM_ARMOR)
		{
		obj->value[3] = 3;
		}

		extract_obj( obj2 );	
	stc("#PYou carve the Metta Rune into the object.#n\n\r",ch);
	}

	if (!str_cmp( spellarg, "infravision"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"Zarius Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		if (obj->item_type == ITEM_WEAPON)
		{
		obj->value[0] = 1000;
		}

		else if (obj->item_type == ITEM_ARMOR)
		{
		obj->value[3] = 1;
		}
		extract_obj( obj2 );	
	stc("#PYou carve the Metta Rune into the object.#n\n\r",ch);
	}

	if (!str_cmp( spellarg, "invis"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"Zarius Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		if (obj->item_type == ITEM_WEAPON)
		{
		obj->value[0] = 5000;
		}

		else if (obj->item_type == ITEM_ARMOR)
		{
		obj->value[3] = 5;
		}
		extract_obj( obj2 );	
	stc("#PYou carve the Metta Rune into the object.#n\n\r",ch);
	}

	if (!str_cmp( spellarg, "passdoor"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"Zarius Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		if (obj->item_type == ITEM_WEAPON)
		{
		obj->value[0] = 6000;
		}

		else if (obj->item_type == ITEM_ARMOR)
		{
		obj->value[3] = 6;
		}
		extract_obj( obj2 );	
	stc("#PYou carve the Metta Rune into the object.#n\n\r",ch);
	}

	if (!str_cmp( spellarg, "protection_vs_evil"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"Zarius Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		if (obj->item_type == ITEM_WEAPON)
		{
		obj->value[0] = 7000;
		}

		else if (obj->item_type == ITEM_ARMOR)
		{
		obj->value[3] = 7;
		}
		extract_obj( obj2 );	
	stc("#PYou carve the Metta Rune into the object.#n\n\r",ch);
	}

	if (!str_cmp( spellarg, "protection_vs_good"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"Zarius Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		if (obj->item_type == ITEM_WEAPON)
		{
		obj->value[0] = 139000;
		}

		else if (obj->item_type == ITEM_ARMOR)
		{
		obj->value[3] = 139;
		}
		extract_obj( obj2 );	
	stc("#PYou carve the Metta Rune into the object.#n\n\r",ch);
	}

	if (!str_cmp( spellarg, "sanct"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"Zarius Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		if (obj->item_type == ITEM_WEAPON)
		{
		obj->value[0] = 8000;
		}

		else if (obj->item_type == ITEM_ARMOR)
		{
		obj->value[3] = 8;
		}
		extract_obj( obj2 );	
	stc("#PYou carve the Metta Rune into the object.#n\n\r",ch);
	}

	if (!str_cmp( spellarg, "sneak"        ))
	{
		if ( (obj2 = get_obj_wear(ch,"Zarius Rune",TRUE)) == NULL )         
		{
		stc("#cYou don't have the proper rune!#n\n\r",ch);
		return;
		}
		if (obj->item_type == ITEM_WEAPON)
		{
		obj->value[0] = 9000;
		}

		else if (obj->item_type == ITEM_ARMOR)
		{
		obj->value[3] = 9;
		}
		extract_obj( obj2 );	
	stc("#PYou carve the Metta Rune into the object.#n\n\r",ch);
	}

	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT3);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	return;
	}


	else if (IS_SET(ch->sohbits,SOH_SPELLENCHANT4))
	{
	OBJ_DATA *obj;
    OBJ_DATA *obj2;

	if ( arg[0] == '\0' )
    {
	stc("#0Enchant what??\n\r",ch);
	return;
    }

  if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
  {
    send_to_char("You dont have that item.\n\r",ch);
    return;
  }
	
  if (obj->item_type != ITEM_WEAPON)
  {
    send_to_char("You can only put this enchant on weapons!\n\r", ch);
    return;
  }

  if (IS_SET(obj->extra_flags, ITEM_ELECENCHANT) || IS_SET(obj->extra_flags, ITEM_ICEENCHANT) || IS_SET(obj->extra_flags, ITEM_FIREENCHANT))
  {
    send_to_char("#rThis item has already been #0Enchanted!#n.\n\r", ch);
    return;
  }

	if (IS_SET(ch->sohbits, SOH_SPECICE))		
	{
		if ( (obj2 = get_obj_wear(ch,"ice rune",TRUE)) == NULL )         
		{
		stc("#cYou haven't the proper rune!#n\n\r",ch);
		return;
		}
	}

	else if (IS_SET(ch->sohbits, SOH_SPECFIRE))		
	{
		if ( (obj2 = get_obj_wear(ch,"fire Rune",TRUE)) == NULL )         
		{
		stc("#cYou haven't the proper rune!#n\n\r",ch);
		return;
		}
	}

	else if (IS_SET(ch->sohbits, SOH_SPECLIGHTNING))		
	{
		if ( (obj2 = get_obj_wear(ch,"lightning rune",TRUE)) == NULL )         
		{
		stc("#cYou haven't the proper rune!#n\n\r",ch);
		return;
		}
	}

	REMOVE_BIT(ch->sohbits, SOH_SPELLENCHANT4);
	REMOVE_BIT(ch->sohbits, SOH_SPELLREADY);
	do_wpnenchant(ch,arg);
	return;
	}


	else
    stc("You have no spell to cast!\n\r",ch);
   return;
}
