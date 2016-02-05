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

/* local functions */
void drop_bloodrage     args (( CHAR_DATA *ch ));

void do_moonstrike(CHAR_DATA *ch, char *argument) 
{
  OBJ_DATA *obj;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int i;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_ATTACK], NATTACK_MOONSTRIKE))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  one_argument(argument, arg);
  if (arg[0] == '\0')
  {
    if ((victim = ch->fighting) == NULL)
    {
      send_to_char("But you are not fighting anyone.\n\r", ch);
      return;
    }
  }
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (is_safe(ch, victim)) return;
  drop_bloodrage(ch);
  one_hit(ch, victim, gsn_moonstrike, 1);
  if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_DIRTTHROW_1) &&
      IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_SOULREAPER_1))
  {
    act("You summon a #7**#0shadow#7**#n storm to shred the life from $N.", ch, NULL, victim, TO_CHAR);
    act("$n conjures forth a #7**#0shadow#7**#n vortex, and you are caught in it's shredding embrace.", ch, NULL, victim, TO_VICT);
    act("$n conjueres forth a #7**#0shadow#7**#n storm to shred $N.", ch, NULL, victim, TO_NOTVICT);
    for (i = 0; i < MAX_WEAR; i++)
    {
      if ((obj = get_eq_char(victim, i)) != NULL)
      {
        if (number_range(1, 7) == 2) take_item(victim, obj);
      }
    }
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    WAIT_STATE(ch, 24);
    return;
  }
  else if (ch->pcdata->powers[SHADOW_COMBO] > 0)
  {
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    send_to_char("You feel the shadows subside.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_MOONSTRIKE_1);
    send_to_char("The shadows form around you.\n\r", ch);
  }
  WAIT_STATE(ch, 6);
  return;
}

void do_shadowthrust(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_ATTACK], NATTACK_SHADOWTHRUST))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  one_argument(argument, arg);
  if (arg[0] == '\0')
  {
    if ((victim = ch->fighting) == NULL)
    {
      send_to_char("But you are not fighting anyone.\n\r", ch);
      return;
    }
  }
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (is_safe(ch, victim)) return;
  drop_bloodrage(ch);
  one_hit(ch, victim, gsn_shadowthrust, 1);
  if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_CALTROPS_1) &&
      IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_DIRTTHROW_1))
  {
    if (!IS_SET(ch->pcdata->powers[SHADOW_BITS], NPOWER_SHADOWFORM))
    {
      act("You become one with the shadows, transforming your body into shadow matter.", ch, NULL, NULL, TO_CHAR);
      act("$n becomes one with the shadows, transforming $s body into shadow matter.", ch, NULL, NULL, TO_ROOM);
      SET_BIT(ch->pcdata->powers[SHADOW_BITS], NPOWER_SHADOWFORM);
    }
    else
    {
      act("You reform your body, and leave the shadow existance.", ch, NULL, NULL, TO_CHAR);
      act("$n reforms $s body, returning from the shadows.", ch, NULL, NULL, TO_ROOM);
      REMOVE_BIT(ch->pcdata->powers[SHADOW_BITS], NPOWER_SHADOWFORM);
    }
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    WAIT_STATE(ch, 12);
    return;
  }
  else if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_DIRTTHROW_1) &&
      IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_KNIFESPIN_1) &&
      IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_KNIFESPIN_2))
  {
    do_say(ch, "By the power of the shadows, you must die!");
    one_hit(ch, victim, gsn_shadowthrust, 1);
    one_hit(ch, victim, gsn_shadowthrust, 1);
    one_hit(ch, victim, gsn_shadowthrust, 1);
    one_hit(ch, victim, gsn_shadowthrust, 1);
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    WAIT_STATE(ch, 12); 
    return;
  }
  else if (ch->pcdata->powers[SHADOW_COMBO] > 0)
  {
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    send_to_char("You feel the shadows subside.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_SHADOWTHRUST_1);
    send_to_char("The shadows form around you.\n\r", ch);
  }
  WAIT_STATE(ch, 6);
  return;
}

void do_dirtthrow(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
     
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_ATTACK], NATTACK_DIRTTHROW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  one_argument(argument, arg);
  if (arg[0] == '\0')
  {
    if ((victim = ch->fighting) == NULL)
    {
      send_to_char("But you are not fighting anyone.\n\r", ch);
      return;
    }
  }
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  { 
    send_to_char("They are not here.\n\r", ch); 
    return;
  }
  if (is_safe(ch, victim)) return;
  drop_bloodrage(ch);
  one_hit(ch, victim, gsn_dirtthrow, 1);
  if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_CALTROPS_1))
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_DIRTTHROW_1);
    send_to_char("The shadows flicker.\n\r", ch);
  }
  else if (ch->pcdata->powers[SHADOW_COMBO] > 0)
  {
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    send_to_char("You feel the shadows subside.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_DIRTTHROW_1);
    send_to_char("The shadows form around you.\n\r", ch);
  }
  WAIT_STATE(ch, 6);
  return;
}

void do_gutcutter(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
 
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_ATTACK], NATTACK_GUTCUTTER))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }      
  one_argument(argument, arg);
  if (arg[0] == '\0')
  {
    if ((victim = ch->fighting) == NULL)
    {
      send_to_char("But you are not fighting anyone.\n\r", ch);
      return;
    }
  }
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (is_safe(ch, victim)) return;
  drop_bloodrage(ch);
  one_hit(ch, victim, gsn_gutcutter, 1);
  if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_SHADOWTHRUST_1))
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_GUTCUTTER_1);
    send_to_char("The shadows form closer around you.\n\r", ch);
  }
  else if (ch->pcdata->powers[SHADOW_COMBO] > 0)
  {
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    send_to_char("You feel the shadows subside.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_DIRTTHROW_1);
    send_to_char("The shadows form around you.\n\r", ch);
  }
  WAIT_STATE(ch, 6);
  return;
}

void do_soulreaper(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_ATTACK], NATTACK_SOULREAPER))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  one_argument(argument, arg);
  if (arg[0] == '\0')
  {
    if ((victim = ch->fighting) == NULL)
    {
      send_to_char("But you are not fighting anyone.\n\r", ch);
      return;
    }
  }
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (is_safe(ch, victim)) return;
  drop_bloodrage(ch);
  one_hit(ch, victim, gsn_soulreaper, 1);
  if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_SHADOWTHRUST_1) &&
      IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_GUTCUTTER_1))
  {
    if (!IS_SET(ch->pcdata->powers[SHADOW_BITS], NPOWER_BLUR))
    {
      act("You shroud yourself in a blanket of shadows.", ch, NULL, NULL, TO_CHAR);
      act("$n shrouds $mself in a blanket of shadows.", ch, NULL, NULL, TO_ROOM);
      SET_BIT(ch->pcdata->powers[SHADOW_BITS], NPOWER_BLUR);
    }
    else
      send_to_char("You are already blurred in shadows.\n\r", ch);
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    WAIT_STATE(ch, 12);
    return;
  }
  else if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_DIRTTHROW_1))
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_SOULREAPER_1);
    send_to_char("The shadows form around you, ready to strike.\n\r", ch);
  }
  else if (ch->pcdata->powers[SHADOW_COMBO] > 0)
  {
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    send_to_char("You feel the shadows subside.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_SOULREAPER_1);
    send_to_char("The shadows form around you.\n\r", ch);
  }
  WAIT_STATE(ch, 6);
  return;
}

void do_knifespin(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
 
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_ATTACK], NATTACK_KNIFESPIN))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  one_argument(argument, arg);
  if (arg[0] == '\0')
  {
    if ((victim = ch->fighting) == NULL)
    { 
      send_to_char("But you are not fighting anyone.\n\r", ch);
      return;
    }
  }
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (is_safe(ch, victim)) return;
  drop_bloodrage(ch);
  one_hit(ch, victim, gsn_knifespin, 1);
  if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_CALTROPS_1))
  {
    if (!IS_SET(ch->pcdata->powers[SHADOW_BITS], NPOWER_KNIFESHIELD))
    {
      act("You throw the knife into the air and a barrier of knifes form around you.", ch, NULL, NULL, TO_CHAR);
      act("$n throws a knife into the air, and a barrier of knifes surround $m.", ch, NULL, NULL, TO_ROOM);
      SET_BIT(ch->pcdata->powers[SHADOW_BITS], NPOWER_KNIFESHIELD);
    }
    else
      send_to_char("You already have a set of flying knifes around you.\n\r", ch);
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    WAIT_STATE(ch, 6);
    return;
  }
  else if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_WAKASASHISLICE_1) &&
           IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_WAKASASHISLICE_2))
  {
    act("You throw back your head and howl in glee as you enter the bloodrage.", ch, NULL, NULL, TO_CHAR);
    act("$n throws back $s head and howl in glee as $e enters a bloodrage.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[SHADOW_BITS], NPOWER_BLOODRAGE);
    if (IS_SET(ch->pcdata->powers[SHADOW_BITS], NPOWER_SHADOWFORM))
    {
      REMOVE_BIT(ch->pcdata->powers[SHADOW_BITS], NPOWER_SHADOWFORM);
      send_to_char("You return from the shadows.\n\r", ch);
    }
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    WAIT_STATE(ch, 12);
    return;
  }
  else if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_DIRTTHROW_1) &&
           IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_KNIFESPIN_1))
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_KNIFESPIN_2);
    send_to_char("The shadows grow more substantial.\n\r", ch);
  }
  else if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_DIRTTHROW_1))
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_KNIFESPIN_1);
    send_to_char("The shadows grow more substantial.\n\r", ch);
  }
  else if (ch->pcdata->powers[SHADOW_COMBO] > 0)
  {
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    send_to_char("You feel the shadows subside.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_KNIFESPIN_1);
    send_to_char("The shadows form around you.\n\r", ch);
  }
  WAIT_STATE(ch, 6);
  return;
}

void do_wakasashislice(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
    
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_ATTACK], NATTACK_WAKASASHISLICE))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  one_argument(argument, arg);
  if (arg[0] == '\0')
  {
    if ((victim = ch->fighting) == NULL)
    {
      send_to_char("But you are not fighting anyone.\n\r", ch);
      return;
    }
  }
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (is_safe(ch, victim)) return;
  drop_bloodrage(ch);
  one_hit(ch, victim, gsn_wakasashislice, 1);
  if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_KNIFESPIN_1))
  {
    do_say(ch, "Wakasashi!!");
    one_hit(ch, victim, gsn_wakasashislice, 1);
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    WAIT_STATE(ch, 6); 
    return;
  }
  else if (IS_SET(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_WAKASASHISLICE_1))
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_WAKASASHISLICE_2);
    send_to_char("The shadows form around you, ready to strike.\n\r", ch);
  }
  else if (ch->pcdata->powers[SHADOW_COMBO] > 0)
  {
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    send_to_char("You feel the shadows subside.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_WAKASASHISLICE_1);
    send_to_char("The shadows form around you.\n\r", ch);
  }
  WAIT_STATE(ch, 6);
  return;
}

void do_caltrops(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_ATTACK], NATTACK_CALTROPS))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  one_argument(argument, arg);
  if (arg[0] == '\0')
  {
    if ((victim = ch->fighting) == NULL)
    {
      send_to_char("But you are not fighting anyone.\n\r", ch);
      return;
    }
  }
  else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (is_safe(ch, victim)) return;
  drop_bloodrage(ch);
  one_hit(ch, victim, gsn_caltrops, 1);
  if (ch->pcdata->powers[SHADOW_COMBO] > 0)
  {
    ch->pcdata->powers[SHADOW_COMBO] = 0;
    send_to_char("You feel the shadows subside.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->pcdata->powers[SHADOW_COMBO], NCOMBO_CALTROPS_1);
    send_to_char("The shadows form around you.\n\r", ch);
  }
  WAIT_STATE(ch, 6);
  return;
}

void do_shadowlearn(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int i, cost, power;
  bool exp = TRUE;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  one_argument(argument, arg);
  if (arg[0] == '\0')
  {
    send_to_char("                    #R{}#y***#R{} #0Shadow Powers #R{}#y***#R{}#n\n\r\n\r", ch);
    xprintf(buf, "              #yMartial Skill #0[#R");
    for (i = 0; i < 25; i++)
    {
      if (ch->pcdata->powers[SHADOW_MARTIAL] > i) strcat(buf, "*");
      else strcat(buf, " ");
    }
    strcat(buf, "#0]#n\n\r\n\r");
    send_to_char(buf, ch);
    send_to_char("   #RSilentwalk #0[#y10000#0]   #RVanish       #0[#y25000#0]   #RShadowwalk   #0[#y25000#0]#n\n\r", ch);
    send_to_char("   #RTruesight  #0[#y10000#0]   #RDemonic Pact #0[#y50000#0]#n\n\r", ch);
    send_to_char("   #RNight Pact #0[#y50000#0]   #RScry         #0[#y25000#0]   #RShield       #0[#y10000#0]#n\n\r", ch);
    send_to_char("   #RSoulseeker #0[#y25000#0]   #RAssassinate  #0[#y25000#0]#n\n\r\n\r", ch);
    send_to_char("                 #0Ninja Attacks (5 mill exp to learn)#n\n\r\n\r", ch);
    send_to_char("      #RMoonstrike    Shadowthrust    Dirtthrow        Gutcutter#n\n\r", ch);
    send_to_char("      #RSoulreaper    Knifespin       Wakasashislice   Caltrops#n\n\r", ch);
    return;
  }
  if (!str_cmp(arg, "martial"))
  {
    if (ch->pcdata->powers[SHADOW_MARTIAL] >= 25)
    {
      send_to_char("You are already a master of martial combat.\n\r", ch);
      return;
    }
    if (ch->practice < (ch->pcdata->powers[SHADOW_MARTIAL] + 1) * 10)
    {
      send_to_char("You need 10 primal for each level.\n\r", ch);
      return;
    }
    ch->pcdata->powers[SHADOW_MARTIAL]++;
    ch->practice -= ch->pcdata->powers[SHADOW_MARTIAL] * 10;
    send_to_char("Ok.\n\r", ch);
    return;
  }
  else if (!str_cmp(arg, "silentwalk"))
  {
    power = NSHADOWS_SILENTWALK;
    cost = 10000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "vanish"))
  {
    power = NSHADOWS_HIDE;
    cost = 25000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "shadowwalk"))
  {
    power = NSHADOWS_SHADOWWALK;
    cost = 25000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "truesight"))
  {
    power = NSHADOWS_SIGHT;
    cost = 10000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "demonic"))
  {
    power = NSHADOWS_TPACT;
    cost = 50000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "night"))
  {
    power = NSHADOWS_DPACT;
    cost = 50000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "scry"))
  {
    power = NSHADOWS_SCRY;
    cost = 25000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "shield"))
  {
    power = NSHADOWS_SHIELD;
    cost = 10000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "soulseeker"))
  {
    power = NSHADOWS_SOULSEEKERS;
    cost = 25000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "assassinate"))
  {
    power = NSHADOWS_ASSASSINATE;
    cost = 25000;
    exp = FALSE;
  }
  else if (!str_cmp(arg, "moonstrike"))
  {
    power = NATTACK_MOONSTRIKE;
    cost = 5000000;
  }
  else if (!str_cmp(arg, "shadowthrust"))
  {
    power = NATTACK_SHADOWTHRUST;
    cost = 5000000;
  }
  else if (!str_cmp(arg, "dirtthrow"))
  {
    power = NATTACK_DIRTTHROW;
    cost = 5000000;
  }
  else if (!str_cmp(arg, "gutcutter"))
  {
    power = NATTACK_GUTCUTTER;
    cost = 5000000;
  }
  else if (!str_cmp(arg, "soulreaper"))
  {
    power = NATTACK_SOULREAPER;
    cost = 5000000;
  }
  else if (!str_cmp(arg, "knifespin"))
  {
    power = NATTACK_KNIFESPIN;
    cost = 5000000;
  }
  else if (!str_cmp(arg, "wakasashislice"))
  {
    power = NATTACK_WAKASASHISLICE;
    cost = 5000000;
  }
  else if (!str_cmp(arg, "caltrops"))
  {
    power = NATTACK_CALTROPS;
    cost = 5000000;
  }
  else
  {
    do_shadowlearn(ch, "");
    return;
  }

  /* take the cost and give the skill */
  if (!exp)
  {
    if (IS_SET(ch->pcdata->powers[SHADOW_POWERS], power))
    {
      send_to_char("You already have that power.\n\r", ch);
      return;
    }
    if (ch->pcdata->powers[SHADOW_POWER] < cost)
    {
      send_to_char("You don't have enough shadow points to buy this power.\n\r", ch);
      return;
    }
    ch->pcdata->powers[SHADOW_POWER] -= cost;
    SET_BIT(ch->pcdata->powers[SHADOW_POWERS], power);
  }
  else
  {
    if (IS_SET(ch->pcdata->powers[SHADOW_ATTACK], power))
    {
      send_to_char("You already know that attack style.\n\r", ch);
      return;
    }
    if (ch->exp < cost)
    {
      xprintf(buf, "You don't have the %d exp.\n\r", cost);
      send_to_char(buf, ch);
      return;
    }
    ch->exp -= cost;
    SET_BIT(ch->pcdata->powers[SHADOW_ATTACK], power);
  }
  send_to_char("Ok.\n\r", ch);
}

void do_soulseek(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *victimroom;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_POWERS], NSHADOWS_SOULSEEKERS))
  {
    send_to_char("You don't have the soulseeker power yet.\n\r", ch);
    return;
  }
  if (strlen(ch->pcdata->soultarget) < 3)
  {
    send_to_char("But your not targetting anyone.\n\r", ch);
    return;
  }
  if (ch->pcdata->powers[SHADOW_SOULAMMO] < 1)
  {
    send_to_char("You have lost your lock on their soul.\n\r", ch);
    return;
  }
  if (ch->pcdata->powers[SHADOW_POWER] < 500)
  {
    send_to_char("You don't have enough class points.\n\r", ch);
    return;
  }
  if ((victim = get_char_world(ch, ch->pcdata->soultarget)) == NULL)
  {
    send_to_char("You cannot find them.\n\r", ch);
    return;
  }
  if ((victimroom = victim->in_room) == NULL)
  {
    send_to_char("They are hiding from you.\n\r", ch);
    return;
  }
  chroom = ch->in_room;
  char_from_room(ch);
  char_to_room(ch, victimroom);
  one_hit(ch, victim, gsn_soulseeker, 1);
  one_hit(ch, victim, gsn_soulseeker, 1);
  one_hit(ch, victim, gsn_soulseeker, 1);
  char_from_room(ch);
  char_to_room(ch, chroom);
  ch->pcdata->powers[SHADOW_POWER] -= 500;
  WAIT_STATE(ch, 12);
}

void do_soultarget(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHADOW))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[SHADOW_POWERS], NSHADOWS_SOULSEEKERS))
  {  
    send_to_char("You don't have the soulseeker power yet.\n\r", ch);
    return;
  }
  if ((victim = ch->fighting) == NULL)
  {
    send_to_char("You are not fighting anyone.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("NPC's have no soul to target.\n\r", ch);
    return;
  }
  free_string(ch->pcdata->soultarget);
  ch->pcdata->soultarget = str_dup(victim->pcdata->switchname);
  ch->pcdata->powers[SHADOW_SOULAMMO] = 5;
  send_to_char("You get a lock on their soul, they cannot escape your wrath now.\n\r", ch);
  send_to_char("You feel a dark presence in the very core of your being.\n\r", victim);
  WAIT_STATE(ch, 12);
}

void drop_bloodrage(CHAR_DATA *ch)
{
  if (IS_SET(ch->pcdata->powers[SHADOW_BITS], NPOWER_BLOODRAGE))
  {
    send_to_char("Your bloodrage subsides.\n\r", ch);
    REMOVE_BIT(ch->pcdata->powers[SHADOW_BITS], NPOWER_BLOODRAGE);
  }
}

/*
void do_shadowtalk(CHAR_DATA *ch, char *argument)
{
  int class = ch->class;

  if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && !IS_CLASS(ch, CLASS_SHADOW)))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  ch->class = CLASS_SHADOW;
  talk_channel(ch, argument, CHANNEL_CLASS, CC_SHADOW, "shadow sign");
  ch->class = class;
  return;
}
*/
