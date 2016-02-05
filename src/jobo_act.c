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
#include <ctype.h>
#include "merc.h"
#include "interp.h"
#include "md5.h"

ALIAS_DATA * alias_free;

void do_bountylist(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;

  xprintf(buf, " %s\n\r", get_dystopia_banner("  Bountylist  ", 60));
  send_to_char(buf, ch);
  stc("  #CName               Bounty     Status     Generation	Tier\n\r#n",ch);
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if ( d->character != NULL )
    {
       if (!d->connected == CON_PLAYING) continue;
       if (d->character->level > 6) continue;
       if (!can_see(ch, d->character)) continue;
       xprintf(buf, "  #G%-15s#n    %6d        %2d           %d	%d\n\r",d->character->name, d->character->pcdata->bounty,
         d->character->race,d->character->generation, d->character->tier);
       stc(buf,ch);
    }
  }
  xprintf(buf, " %s\n\r", get_dystopia_banner("", 60));
  send_to_char(buf, ch);
  return;
}

void do_top( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  int i;

  if (IS_NPC(ch)) return;

  stc("#r==--==#L**#r==--==#L**#r==--==#L**#r==--== #GTOP PKSCORE #r==--==#L**#r==--==#L**#r==--#r==#L**#r==--==#n\n\r\n\r",ch);
  for (i = 1; i <= MAX_TOP_PLAYERS / 2; i++)
  {
    xprintf(buf," %2d.  %-15s      %5d       %2d.  %-15s      %5d\n\r",
      i, top_board[i].name, top_board[i].pkscore,
      i + MAX_TOP_PLAYERS / 2, top_board[i + MAX_TOP_PLAYERS / 2].name, top_board[i + MAX_TOP_PLAYERS / 2].pkscore);
    send_to_char(buf,ch);
  }
  xprintf(buf,"\n\r                       #GYou #0:#G                %-4d\n\r",ch->race);
  send_to_char(buf,ch);
  stc("\n\r#r--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--#n\n\r",ch);
  return;
}

void do_trigger(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char ttime[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  int tick;

  argument = one_argument(argument, arg);
  one_argument(argument, ttime);

  if (arg[0] == '\0')
  {
    send_to_char("Trigger what ?\n\r", ch);
    return;
  }
  if (!is_number(ttime))
  {
    send_to_char("What should the timer be set at (a number please) ?\n\r", ch);
    return;
  }
  tick = atoi(ttime);
  if (tick < 1 || tick > 3)
  {
    send_to_char("Between 1 and 3 ticks please.\n\r", ch);
    return;
  }
  if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
  {
    send_to_char("You don't have that item.\n\r", ch);
    return;
  }
  if (obj->item_type != ITEM_GRENADE)
  {
    send_to_char("That is not a grenade or a bomb.\n\r", ch);
    return;
  }
  if (obj->timer > 0)
  {
    send_to_char("It's already ticking, better run.\n\r", ch);
    return;
  }
  obj->timer = tick;
  send_to_char("You set the timer.\n\r", ch);
  act("$n fiddles with $p.", ch, obj, NULL, TO_ROOM);
  WAIT_STATE(ch, 6);
  return;
}

void do_dcredits(CHAR_DATA *ch, char *argument)
{
  do_help(ch, "dcredits");
  return;
}

void do_runeeq( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of rune armor you wish to make: \n\rRing Collar Plate Helmet Leggings Boots Gloves Sleeves Cape Belt Bracer Mask.\n\r",ch);
    return;
  }
  if ( ch->bones < 15000 )
  {
    send_to_char("You don't have the 15K bones needed.\n\r",ch);
    return;
  }
  if      (!str_cmp(arg,"ring"        )) vnum = 814;
  else if (!str_cmp(arg,"collar"      )) vnum = 815;
  else if (!str_cmp(arg,"plate"       )) vnum = 817;
  else if (!str_cmp(arg,"helmet"      )) vnum = 818;
  else if (!str_cmp(arg,"leggings"    )) vnum = 819;
  else if (!str_cmp(arg,"boots"       )) vnum = 820;
  else if (!str_cmp(arg,"gloves"      )) vnum = 821;
  else if (!str_cmp(arg,"sleeves"     )) vnum = 822;
  else if (!str_cmp(arg,"cape"        )) vnum = 823;
  else if (!str_cmp(arg,"belt"        )) vnum = 824;
  else if (!str_cmp(arg,"bracer"      )) vnum = 816;
  else if (!str_cmp(arg,"mask"        )) vnum = 825;
  else
  {
    do_runeeq(ch,"");
    return;
  }
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform Jobo .\n\r",ch);
    return;
  }
  if(!IS_IMMORTAL(ch)) ch->bones -= 15000;
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj->ownerid = ch->pcdata->playerid;
  obj_to_char(obj, ch);
  ch->pcdata->rune_count++;
  act("You transform the 10K qp into $p.",ch,obj,NULL,TO_CHAR);
  act("$n forms $p out of 10K qp.",ch,obj,NULL,TO_ROOM);
  return;
}

void do_upkeep(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  send_to_char("#RUpkeep list#n\n\r\n\r",ch);
  if (IS_AFFECTED(ch, AFF_HIDE))          send_to_char("* You cannot be seen by other players.\n\r",ch);
  if (IS_SET(ch->act, PLR_WIZINVIS))      send_to_char("* You cannot be seen by other players.\n\r",ch);
  if (IS_SET(ch->newbits, NEW_DARKNESS))  send_to_char("* You are shrouded in a globe of darkness.\n\r",ch);
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE))   send_to_char("* You are standing in the shadowplane.\n\r",ch);
  if (IS_AFFECTED(ch, AFF_ETHEREAL))      send_to_char("* You are in ethereal form.\n\r",ch);
  if (IS_SET(ch->act,PLR_HOLYLIGHT))      send_to_char("* You have superior vision.\n\r",ch);
  if (IS_IMMUNE(ch,IMM_SHIELDED))         send_to_char("* You are shielded from scrying eyes.\n\r",ch);

  if (IS_CLASS(ch, CLASS_GIANT))
  {
    if (ch->pcdata->powers[GIANT_STANDFIRM] == 1) send_to_char("* You have your feet planted firmly in the ground.\n\r",ch);
    if (IS_SET(ch->newbits, NEW_CUBEFORM))        send_to_char("* You feel the strength of the dawn running through you.\n\r",ch);
  }

  if (IS_CLASS(ch, CLASS_DROW))
  {
    if (IS_SET(ch->newbits, NEW_DROWHATE))
      send_to_char("* You are filled with hatred towards other creatures.\n\r", ch);
  }

  if (IS_CLASS(ch, CLASS_LICH))
  {
    if (IS_IMMUNE(ch,IMM_SHIELD2)) send_to_char("* You are shielded by the powers of chaos.\n\r",ch);
    if (ch->tick_timer[TIMER_CALLGOLEMS] < 1)   send_to_char("* You can call on your golems.\n\r", ch);
    if (!IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_FIRE))
      send_to_char("* You can summon another #Rfire#n golem.\n\r", ch);
    if (!IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_CLAY))
      send_to_char("* You can summon another #oclay#n golem.\n\r", ch);
    if (!IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_STONE))
      send_to_char("* You can summon another #Lstone#n golem.\n\r", ch);
    if (!IS_SET(ch->pcdata->powers[GOLEMS_SUMMON], HAS_SUMMONED_IRON))
      send_to_char("* You can summon another #ciron#n golem.\n\r", ch);
  }

  if (IS_CLASS(ch, CLASS_NINJA))
  {
    if (IS_VAMPAFF(ch, VAM_CLAWS)) send_to_char("* You have iron wrists attached.\n\r",ch);
  }
  if (IS_CLASS(ch, CLASS_TANARRI))
  {
    if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) send_to_char("* You are in a mad fury.\n\r", ch);
  }
  if (IS_CLASS(ch, CLASS_DRONE))
  {
    if (ch->pcdata->powers[DRONE_EYE] == DRONE_EYE_OFF)
      send_to_char("* Your mindseye is off.\n\r", ch);
    else if (ch->pcdata->powers[DRONE_EYE] == DRONE_EYE_LEVEL)
      send_to_char("* Your mindseye reads the level of your victims.\n\r", ch);
    else if (ch->pcdata->powers[DRONE_EYE] == DRONE_EYE_READAURA)
      send_to_char("* Your mindseye reads the aura of your victims.\n\r", ch);
    else if (ch->pcdata->powers[DRONE_EYE] == DRONE_EYE_CUSTOM)
    {
      xprintf(buf, "* Your mindseye : %s\n\r", ch->poweraction);
      send_to_char(buf, ch);
    }
    else send_to_char("* Your mindseye is bugged.\n\r", ch);
    if (ch->pcdata->powers[DRONE_NIGHTMARE] == 1)
      send_to_char("* You have unleashed your nightmarish aura.\n\r", ch);
  }

  if (IS_CLASS(ch, CLASS_WEREWOLF))
  {
    if (IS_GAR1(ch, WOLF_COCOON)) send_to_char("* You are engulfed in a cocoon.\n\r",ch);
  }

  if (IS_CLASS(ch, CLASS_FAE))
  {
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) send_to_char("* You are a force of nature.\n\r",ch);
  }

  if (IS_CLASS(ch, CLASS_DEMON))
  {
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) send_to_char("* You are in demonic form.\n\r", ch);
    if (IS_VAMPAFF(ch, VAM_CLAWS)) send_to_char("* You have wicked claws extending from your fingers.\n\r", ch);
    if (IS_VAMPAFF(ch, VAM_FANGS)) send_to_char("* You have sharp fangs extending from your gums.\n\r", ch);
    if (IS_DEMAFF(ch,DEM_TAIL)) send_to_char("* You have a long tail extending from your back.\n\r", ch);
    if (IS_DEMAFF(ch,DEM_HOOVES)) send_to_char("* You have hooves instead of feet.\n\r", ch);
  }

  if (IS_CLASS(ch, CLASS_VAMPIRE))
  {
    if (IS_SET(ch->extra, EXTRA_FLASH)) send_to_char("* You move with lightning speed.\n\r", ch);
    if (IS_SET(ch->extra, EXTRA_BAAL)) send_to_char("* You carry the spirit of Baal.\n\r", ch);
  }
  return;
}

/* The costy healer */
void do_healme(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (ch->in_room != NULL && ch->in_room->vnum != ROOM_VNUM_ALTAR)
  {
    send_to_char("The healer is located at the altar in midgaard.\n\r",ch);
    return;
  }
  if (ch->fight_timer > 0)
  {
    send_to_char("Not with a fighttimer.\n\r",ch);
    return;
  }
  if (ch->bones < 1000)
  {
    send_to_char("The healer demands 1000 bones as payment for his service.\n\r",ch);
    return;
  }
  ch->hit  = ch->max_hit;
  ch->mana = ch->max_mana;
  ch->move = ch->max_move;
  ch->loc_hp[0] = 0;
  ch->loc_hp[1] = 0;
  ch->loc_hp[2] = 0;
  ch->loc_hp[3] = 0;
  ch->loc_hp[4] = 0;
  ch->loc_hp[5] = 0;
  ch->loc_hp[6] = 0;
  send_to_char("The healer heals your wounds and takes 1000 bones as payment.\n\r",ch);
  ch->bones -= 1000;
  WAIT_STATE(ch, 36);
  return;
}

/* MUDSTAT command */
void do_mudstat( CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];

  extern char str_boot_time[];

  if (IS_NPC(ch)) return;

  send_to_char("#0--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==--#n\n\r\n\r",ch);

  if (global_exp)
  {
    send_to_char("#R*** #0Double Experience is #GON!#n\n\r",ch);
    if(pulse_exp > 0)
    {
    xprintf(buf, "      #Y%d #0minutes remaining#n\n\r\n\r", pulse_exp);
    send_to_char(buf, ch);
    }
  }
  else
  {
    send_to_char("#R*** #0Double Experience is #ROFF#n\n\r",ch);
    xprintf(buf, "#0--- #wCost for Exp still: #Y%d#n\n\r\n\r", exp_cost);
    send_to_char(buf, ch);
  }
  if (global_qp)
    send_to_char("#R*** #0Double Bones is #GON!#n\n\r\n\r#n",ch);
  else
    send_to_char("#R*** #0Double Bones is #ROFF#n\n\r\n\r#n",ch);
  if (global_cp)
  {
    send_to_char("#R*** #0Double Class Points is #GON!#n\n\r\n\r#n",ch);
    if(pulse_cp > 0)
    {
    xprintf(buf, "      #Y%d #0minutes remaining#n\n\r\n\r", pulse_cp);
    send_to_char(buf, ch);
    }
  }
  else
  {
    send_to_char("#R*** #0Double Class Points is #ROFF#n\n\r#n",ch);
    xprintf(buf, "#0--- #wCost for Double CP still: #Y%d#n\n\r\n\r", cp_cost);
    send_to_char(buf, ch);
  }
  if (global_dt)
  {
    send_to_char("#R*** #0SUPER TRAINING is #GON!#n\n\r#n",ch);
    if(pulse_dt > 0)
    {
    xprintf(buf, "      #Y%d #0minutes remaining#n\n\r\n\r", pulse_dt);
    send_to_char(buf, ch);
    }
  }
  else
  {
    send_to_char("#R*** #0SUPER TRAINING is #ROFF#n\n\r#n",ch);
    xprintf(buf, "#0--- #wCost for Super Training still: #Y%d#n\n\r\n\r", dt_cost);
    send_to_char(buf, ch);
  }
  if (global_chaos)
    send_to_char("#s*#s*#s* #GWorld is in #sC#sh#sa#so#ss#n\n\r\n\r#n",ch);
  else
    send_to_char("\n\r#n",ch);
  if (ragnarok)
    send_to_char("#CRAGNAROK MODE!!!!!!!#n  Everyone can kill everyone (HELP RAGNAROK)\n\r\n\r",ch);
  else
  {
    xprintf(buf,"#RRagnarok is still #Y%d #Rbones from happening.#n\n\r\n\r",ragnarok_cost);
    send_to_char(buf,ch);
  }

  xprintf(buf, "#0Number of mobs left to kill before another artifact is loaded :#C %d\n\r#n", artimkills);
  send_to_char(buf,ch);
  xprintf(buf, "#0Number of players connected since last copyover/restart :#C %d\n\r#n", players_logged);
  send_to_char(buf,ch);
  xprintf(buf, "#0Number of players beheaded since last copyover/restart  :#C %d\n\r#n", players_decap);
  send_to_char(buf,ch);
  xprintf(buf, "#0Amount of generation stolen since last copyover/restart :#C %d\n\r\n\r#n", players_gstolen);
  send_to_char(buf,ch);
  xprintf( buf, "%s was last (re)started at : %s\rThe system time is currently     : %s\n\r",
    MUDNAME, str_boot_time, (char *) ctime( &current_time));
  send_to_char( buf, ch );
  send_to_char("#0--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==#B**#0==--==--#n\n\r\n\r",ch);
  return;
}

void do_pkvision(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
 
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) < 45)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  if (!IS_SET(ch->itemaffect, ITEMA_VISION))
  {
    SET_BIT(ch->itemaffect, ITEMA_VISION);
    SET_BIT(ch->newbits, NEW_VISION);  // so we know to remove it again.
    send_to_char("Your eyes flicker, and everything is clear.\n\r",ch);
  }
  else
    send_to_char("You already have superior sight.\n\r",ch);
  return;
}

void do_pkscry(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *victimroom;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) < 10)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
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
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->in_room)
  {
    if (has_area_affect(victim->in_room->area, AREA_AFF_BADMOON, 0))
    {
      send_to_char("Some powerful magic prevent your scrying attempt.\n\r", ch);
      return;
    }
  }
  if (victim->trust > 6)
  {
    send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
    send_to_char("Someone is scrying you.\n\r",victim);
    return;
  }
  chroom = ch->in_room;
  victimroom = victim->in_room;
  char_from_room(ch);
  char_to_room(ch,victimroom);
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
  { 
    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    do_look(ch,"scry");
    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
  }
  else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
  {
    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    do_look(ch,"scry");
    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
  } 
  else
    do_look(ch,"scry");
  char_from_room(ch);
  char_to_room(ch,chroom);
  if (ch->level < 7) ch->fight_timer += 3;
  if (victim->race > 14)
    send_to_char("You feel a slight tingle.\n\r",victim);
  return;
}

void do_pkobjscry(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *gch;
  CHAR_DATA *victim; 
  OBJ_DATA *obj;
  ROOM_INDEX_DATA *location;
  ROOM_INDEX_DATA *from;
  DESCRIPTOR_DATA *tmp;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  bool afk = FALSE;

  one_argument(argument, arg);
   
  if (IS_NPC(ch)) return;  
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) < 35)
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ((obj = get_obj_world(ch, arg)) == NULL)
  {
    send_to_char("Nothing like that in hell, earth, or heaven.\n\r", ch);
    return;
  }
  if (IS_OBJ_STAT(obj, ITEM_NOLOCATE))
  {
    send_to_char("A powerful force prevents the scrying of this object.\n\r", ch);
    return;
  }
  if ((gch = obj->carried_by) != NULL)
  {
    if (gch->in_room == NULL)
    {
      send_to_char("All you see is a chaotic mass of colors, nothing seems right.\n\r",ch);
      return;
    }
    if (has_area_affect(gch->in_room->area, AREA_AFF_BADMOON, 0))
    {
      send_to_char("Some powerful magic prevent your scrying attempt.\n\r", ch);
      return;
    }
    tmp = gch->desc;
    gch->desc = ch->desc;
    xprintf(buf,"A pair of eyes grows on %s's %s.\n\rThe eyes blink once, then disappear.\n\r", gch->name, obj->short_descr);
    for (victim = gch->in_room->people; victim != NULL; victim = victim->next_in_room)
    {
      if (victim == gch) continue;  // the victim cannot see this.
      send_to_char(buf,victim);
    }
    if (IS_SET(gch->extra, EXTRA_AFK))
    {
      afk = TRUE;
      REMOVE_BIT(gch->extra, EXTRA_AFK);
    }
    do_look(gch,"");
    gch->desc = tmp;
    if (afk) SET_BIT(gch->extra, EXTRA_AFK);
    return;
  }
  else if ((location = obj->in_room) != NULL)
  {
    if (has_area_affect(location->area, AREA_AFF_BADMOON, 0))
    {
      send_to_char("Some powerful magic prevent your scrying attempt.\n\r", ch);
      return;
    }
    xprintf(buf,"A pair of eyes grows on %s.\n\rThe eyes blink once, then disappear.\n\r", obj->short_descr);
    for (victim = location->people; victim != NULL; victim = victim->next_in_room)
    {
      if (victim == ch) continue; // the player is seeing through the item, and will not see the eyes even if he is in the room.
      send_to_char(buf,victim);
    }
    obj_from_room(obj);
    from = ch->in_room;  
    char_from_room(ch);
    char_to_room(ch, location);
    do_look(ch, "");
    char_from_room(ch);
    char_to_room(ch, from);
    obj_to_room(obj, location);
    return;
  }
  else  // inside something.
  {
    send_to_char("It's pitch black, wonder where this item is.\n\r",ch);
    return;
  }
}

void do_pkhunter(CHAR_DATA *ch, char *argument)
{
  MOB_INDEX_DATA *pMobIndex;
  CHAR_DATA *mob;
  MOB_TRIGGER *mProg;

  if (IS_NPC(ch)) return;
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) < 60)
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!TIME_UP(ch, TIMER_HUNTER))
  {
    send_to_char("You can only summon the hunter once every 18 hours.\n\r", ch);
    return;
  }
  if ((pMobIndex = get_mob_index(MOB_VNUM_HUNTER)) == NULL)
  {
    send_to_char("Bug, please report this to an immortal.\n\r", ch);
    return;
  }
  mob = create_mobile(pMobIndex);
  if (trigger_free == NULL)
  {
    mProg             = alloc_perm(sizeof(*mProg));
  }   
  else
  {
    mProg             = trigger_free;
    trigger_free      = trigger_free->next;
  }
  mProg->type         = MTRIG_HUNTER;
  mProg->vnum         = 0;
  mProg->keywords     = str_dup("&tracking*");
  mProg->roomOutput   = str_dup("Follow me master, I have found the trail.");
  mProg->chOutput     = str_dup("I cannot find the trail, master.");
  mProg->next         = mob->triggers;
  mob->triggers       = mProg;

  mob->decay_pulse    = number_range(6, 8);

  char_to_room(mob, ch->in_room);
  act("You call upon the dark hunter, and it appears in a puff of smoke.", ch, NULL, NULL, TO_CHAR);
  act("$n calls upon the dark hunter, and it appears in a puff of smoke.", ch, NULL, NULL, TO_ROOM);
  do_follow(ch, "hunter");
  SET_TIMER(ch, TIMER_HUNTER, 18);
  return;
}


void do_pkportal (CHAR_DATA *ch, char *argument)
{  
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *location;

  one_argument (argument, arg);
    
  if (IS_NPC(ch)) return;
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) < 50)
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Walk the silverpath to whom?\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->fight_timer == 0)
  {
    send_to_char("They are not engaged in pk.\n\r",ch);
    return;
  }
  if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  location = victim->in_room;
  act("You step onto the silverpath.", ch, NULL, NULL, TO_CHAR);
  act("$n steps into thin air.", ch, NULL, NULL, TO_ROOM);
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  act("You leave the silverpath.", ch, NULL, NULL, TO_CHAR);
  act("$n steps out of thin air.", ch, NULL, NULL, TO_ROOM);
  WAIT_STATE(ch, 6);
  return;  
}

void do_pkaura (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char      arg [MAX_INPUT_LENGTH];
  char      buf [MAX_INPUT_LENGTH];
   
  argument = one_argument( argument, arg );
   
  if (IS_NPC(ch)) return;
 if (UMIN(ch->race, (ch->pkill - ch->pdeath)) < 20)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
  if ( arg[0] == '\0' )
  {
    send_to_char("Read the aura of who?\n\r",ch);
    return;
  }
  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->in_room)
  {
    if (has_area_affect(victim->in_room->area, AREA_AFF_BADMOON, 0))
    {
      send_to_char("Some powerful magic prevent your aura scrying attempt.\n\r", ch);
      return;
    }
  }
  if (victim->trust > 6)
  {
    send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
    send_to_char("Someone is trying to read your aura.\n\r",victim);
    return;
  }
  xprintf(buf,"Hp:%d/%d, Mana:%d/%d,Move:%d/%d.\n\r",
    victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
  send_to_char(buf,ch);
  if (ch->trust < 7) send_to_char("You are being examined.\n\r",victim); 
  xprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
  send_to_char(buf,ch);
  xprintf(buf,"Generation:%d\n\r",victim->generation);
  send_to_char(buf,ch);
  if (ch->level < 7) ch->fight_timer += 3;
  return;
}

void do_pkheal(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) < 5)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
  if (ch->fight_timer > 0)
  {
    send_to_char("You cannot focus your mind.\n\r",ch);
    return;
  }
  ch->hit += UMIN(ch->race, 25);
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  send_to_char("You release your control and let harmony and peace flow over you.\n\r",ch);
  WAIT_STATE(ch,12);
  return;
}

void do_pkpowers(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) < 5)
    send_to_char("You suck, get lost.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 4)
    send_to_char("* Sanctum      - Concentrate on your inner peace.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 9)
    send_to_char("* Eaglesight   - Scry on players even through shield.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 14)
    send_to_char("* You have enhanced awereness.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 19)
    send_to_char("* Lifesense    - Global readaura.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 24)
    send_to_char("* You have enhanced fighting abilities.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 29)
    send_to_char("* Calltoarms   - Call for help during battle.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 34)
    send_to_char("* Objectscry   - See through the 'eyes' of an item.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 39)
    send_to_char("* Ironmind     - Prepare yourself mentally for battle.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 44)
    send_to_char("* Crystalsight - Gain superior sight for a short while.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 49)
    send_to_char("* Silverpath   - Low lag portal to players with a fighttimer.\n\r",ch);
  if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 59)
    send_to_char("* Darkhunter   - The ability to summon The Dark Hunter.\n\r",ch);  
return;
}

/* Mastery command to gain mastery items */

void do_mastery(CHAR_DATA *ch, char *argument)
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  int vnum;
  char buf[MAX_INPUT_LENGTH];
  
  if (IS_NPC(ch)) return;
  
  if (IS_SET(ch->newbits, NEW_MASTERY) && ch->level < 6)
  {
    send_to_char("You've already gotten your mastery. If you lost it, tough luck!\n\r",ch);
    return;
  }
  if (ch->wpn[0] < 200 || ch->wpn[1] < 200 || ch->wpn[2] < 200 || ch->wpn[3] < 200 || ch->wpn[4] < 200
   || ch->wpn[5] < 200 || ch->wpn[6] < 200 || ch->wpn[7] < 200 || ch->wpn[8] < 200
   || ch->wpn[9] < 200 || ch->wpn[10] < 200 || ch->wpn[11] < 200 || ch->wpn[12] < 200 )
  {
    send_to_char("Maybe you should grandmaster your weapons first.\n\r",ch);
    return;
  }
  if (ch->spl[0] < 200 || ch->spl[1] < 200 || ch->spl[2] < 200 || ch->spl[3] < 200 || ch->spl[4] < 200 )
  {
    send_to_char("Maybe you should be grand sorcerer in all spell colors first.\n\r",ch);
    return;
  }
  if (ch->stance[1] < 200 || ch->stance[2] < 200 || ch->stance[3] < 200 || ch->stance[4] < 200
   || ch->stance[5] < 200 || ch->stance[6] < 200 || ch->stance[7] < 200 || ch->stance[8] < 200
   || ch->stance[9] < 200 || ch->stance[10] < 200 )
  {
    send_to_char("Maybe you should grandmaster your stances first.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_MAGE)) vnum = 33014;
  else if (IS_CLASS(ch, CLASS_WEREWOLF)) vnum = 33112;
  else if (IS_CLASS(ch, CLASS_NINJA)) vnum = 33094;
  else if (IS_CLASS(ch, CLASS_MONK)) vnum = 33032;
  else if (IS_CLASS(ch, CLASS_DROW)) vnum = 33074;
  else if (IS_CLASS(ch, CLASS_DEMON)) vnum = 33134;
  else if (IS_CLASS(ch, CLASS_VAMPIRE)) vnum = 33054;
  else if (IS_CLASS(ch, CLASS_CYBORG)) vnum = 33153;
  else if (IS_CLASS(ch, CLASS_SAMURAI)) vnum = 33177;
  else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) vnum = 29989;
  else if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) vnum = 33174;
  else if (IS_CLASS(ch, CLASS_ANGEL)) vnum = 33193;
  else if (IS_CLASS(ch, CLASS_TANARRI)) vnum = 33213;
  else if (IS_CLASS(ch, CLASS_LICH)) vnum = 33233;
  else if (IS_CLASS(ch, CLASS_SKYBLADE)) vnum = 33326;
  else if (IS_CLASS(ch, CLASS_PRIEST)) vnum = 33415;
  else if (IS_CLASS(ch, CLASS_SHADOW)) vnum = 33369;
  else if (IS_CLASS(ch, CLASS_FAE)) vnum = 33261;
  else if (IS_CLASS(ch, CLASS_GIANT)) vnum = 33293;
  else if (IS_CLASS(ch, CLASS_DRONE)) vnum = 33313;
  else if (IS_CLASS(ch, CLASS_GHOUL)) vnum = 6216;
  else if (IS_CLASS(ch, CLASS_DRACONIAN)) vnum = 33341;
  else if (IS_CLASS(ch, CLASS_THIEF)) vnum = 33356;
  else if (IS_CLASS(ch, CLASS_JEDI)) vnum = 33418;
  else
  {
    send_to_char("Your class mastery is not done yet, please notify Zarius that you want him to make it.\n\r",ch);
    return;
  }
  if ((pObjIndex = get_obj_index(vnum)) == NULL)
  {
    send_to_char("Missing object, inform Jobo.\n\r",ch);
    return;
  }
  obj = create_object(pObjIndex, 50);
  obj_to_char(obj,ch);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj->ownerid = ch->pcdata->playerid;
  xprintf(buf,"%s has achieved mastery.",ch->name);
  do_info(ch,buf);
  SET_BIT(ch->newbits, NEW_MASTERY);
  return;
}

/* superstances are in :-) */

void do_setstance(CHAR_DATA *ch, char *argument)
{
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  char arg3 [MAX_INPUT_LENGTH];
  int currentstance;
  char buf[MAX_STRING_LENGTH];
  int cost = 0;
  int count = 0;
  int max_supreme = 0;
  int max_greater = 0;
  int max_lesser = 0; 
  int min_cost = 0;   
  int max_cost = 0;   
  int current_supreme = 0;
  int current_greater = 0;
  int current_lesser = 0; 
  
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  strcpy(arg3, argument);
  
  if (ch->stance[STANCE_TIGER] < 200 || ch->stance[STANCE_SWALLOW] < 200 || ch->stance[STANCE_MONKEY] < 200
  || ch->stance[STANCE_MANTIS] < 200 || ch->stance[STANCE_DRAGON] < 200)
  {
    send_to_char("You need to max your normal stances first.\n\r",ch);
    return;
  }
  if (ch->stance[19] == -1) {max_lesser = 3; currentstance = 19; min_cost=40; max_cost=240;}
  else if (ch->stance[20] == -1) {max_lesser = 4; max_greater = 1; currentstance = 20; min_cost = 80; max_cost = 280;}
  else if (ch->stance[21] == -1) {max_lesser = 4; max_greater = 2; currentstance = 21; min_cost = 120; max_cost = 300;}
  else if (ch->stance[22] == -1) {currentstance = 22; max_lesser = 4; max_greater = 4; max_supreme = 1; min_cost = 160; max_cost = 360;}
  else if (ch->stance[23] == -1) {currentstance = 23; max_lesser = 4; max_greater = 4; max_supreme = 2; min_cost = 200; max_cost = 400;}
  else if (!str_cmp(arg1,"clear") || !str_cmp(arg1,"show")) {currentstance = 23; max_lesser = 4; max_greater = 4; max_supreme = 2; min_cost = 200; max_cost = 380;}
  else
  {
    send_to_char("You already have all your superstances, if you want to clear ALL your superstances, you can\n\r",ch);
    send_to_char("type 'setstance clear all stances'. WARNING, this will remove ALL your superstances!\n\r",ch);
    return;
  }
  if (currentstance > 19 && ch->stance[currentstance-7] < 200)
  {
    send_to_char("Maybe you should max your current superstance first.\n\r",ch);
    return;
  }

  if (IS_SET(ch->stance[18], STANCEPOWER_DODGE)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_PARRY)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_SPEED)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_BYPASS)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3)) {current_supreme++;cost += 60;}
  if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_3)) {current_supreme++;cost += 60;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3)) {current_supreme++;cost += 60;}
  if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3)) {current_supreme++;cost += 60;}
   
  if (arg1[0] == '\0')
  {
    send_to_char(" #r*****************************************************************************************#n\n\r",ch);
    send_to_char(" #r*#C                          Superstance Editor v0.3 by Jobo                              #r*#n\n\r",ch);
    send_to_char(" #r*****************************************************************************************#n\n\r",ch);
    send_to_char(" #r*#n [DAMCAP] Increase your damcap.                                                        #r*#n\n\r",ch);
    send_to_char(" #r*#n [REV_DC] Lower your opponents damcap.                                                 #r*#n\n\r",ch);
    send_to_char(" #r*#n [DAMAGE] Increase your damage.                                                        #r*#n\n\r",ch);
    send_to_char(" #r*#n [RESIST] Resist more damage.                                                          #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n The above powers comes in lesser, greater and supreme versions.                       #r*#n\n\r",ch);
    send_to_char(" #r*#n The powers each cost 20/40/60 million exp, depending on level.                        #r*#n\n\r",ch);

    if (currentstance == 19)
      send_to_char(" #r*#C You may choose up to three lesser powers.                                             #r*#n\n\r",ch);
    else if (currentstance == 20)
      send_to_char(" #r*#C You may choose up to one greater power.                                               #r*#n\n\r",ch);
    else if (currentstance == 21)
      send_to_char(" #r*#C You may choose up to two greater powers.                                              #r*#n\n\r",ch);
    else if (currentstance == 22)
      send_to_char(" #r*#C You may choose up to one supreme power.                                               #r*#n\n\r",ch);
    else if (currentstance == 23)
      send_to_char(" #r*#C You may choose up to two supreme powers.                                              #r*#n\n\r",ch);
    if (currentstance > 19)
      send_to_char(" #r*#n There are no maximum on powers of lower level than that.                              #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n Currently you have chosen the following for your next superstance :                   #r*#n\n\r",ch);
    if (ch->stance[18] == 0)
      send_to_char(" #r*#n None.                                                                                 #r*#n\n\r",ch);
    else
    {
      if (IS_SET(ch->stance[18], STANCEPOWER_DODGE))
        send_to_char(" #r*#n Advanced dodge.                                                                       #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_PARRY))
        send_to_char(" #r*#n Advanced parry.                                                                       #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_SPEED))
        send_to_char(" #r*#n Superior Speed.                                                                       #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_BYPASS))
        send_to_char(" #r*#n Bypass Parry and Dodge.                                                               #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1))
        send_to_char(" #r*#n Increased damage (lesser)                                                             #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2))
        send_to_char(" #r*#n Increased damage (greater)                                                            #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
        send_to_char(" #r*#n Increased damage (supreme)                                                            #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1))
        send_to_char(" #r*#n Increased damage resistance (lesser)                                                  #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2))
        send_to_char(" #r*#n Increased damage resistance (greater)                                                 #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
        send_to_char(" #r*#n Increased damage resistance (supreme)                                                 #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1))
        send_to_char(" #r*#n Increased damcap (lesser)                                                             #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2))
        send_to_char(" #r*#n Increased damcap (greater)                                                            #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
        send_to_char(" #r*#n Increased damcap (supreme)                                                            #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1))
        send_to_char(" #r*#n Damcap reduction for opponent (lesser)                                                #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2))
        send_to_char(" #r*#n Damcap reduction for opponent (greater)                                               #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
        send_to_char(" #r*#n Damcap reduction for opponent (supreme)                                               #r*#n\n\r",ch);
    }
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n You may also select any of these powers for the cost of 20 mill exp for the first,    #r*#n\n\r",ch);
    send_to_char(" #r*#n 40 for the next, 60 for the third, and finally 80 for the last.                       #r*#n\n\r",ch);
    send_to_char(" #r*#n [SPEED] Increase your fighting speed.                                                 #r*#n\n\r",ch);
    send_to_char(" #r*#n [PARRY] Increase your parrying ability.                                               #r*#n\n\r",ch);
    send_to_char(" #r*#n [DODGE] Increase your dodging ability.                                                #r*#n\n\r",ch);
    send_to_char(" #r*#n [BYPASS] Bypass the two above powers.                                                 #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n Type \"setstance <power> <lesser/greater/supreme>\"                                     #r*#n\n\r",ch);
    send_to_char(" #r*#n [the latter only for the four top powers] to set or remove a certain power            #r*#n\n\r",ch);
    send_to_char(" #r*#n to your stance. then type \"setstance done\" when your done.                            #r*#n\n\r",ch);  
    send_to_char(" #r*#n You can also type \"setstance clear\" to clear your current settings.                   #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n You can also type \"setstance show <ss1/ss2/etc>\"                                      #r*#n\n\r",ch);
    send_to_char(" #r*#n to show the settings on one of your stances.                                          #r*#n\n\r",ch);
    send_to_char(" #r*---------------------------------------------------------------------------------------*#n\n\r",ch);
    xprintf(buf, " #r*#n Cost of current stance : %-3d million exp.                                             #r*#n\n\r", cost);
    send_to_char(buf,ch);
    xprintf(buf, " #r*#n Minimum/Maximum Cost of Current stance : %-3d/%-3d million exp.                         #r*#n\n\r", min_cost, max_cost);
    send_to_char(buf,ch);
    send_to_char(" #r*****************************************************************************************#n\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "show"))
  {
    if (!str_cmp(arg2,"ss1")) currentstance = 19;
    else if (!str_cmp(arg2,"ss2")) currentstance = 20;
    else if (!str_cmp(arg2,"ss3")) currentstance = 21;
    else if (!str_cmp(arg2,"pizza")) currentstance = 22;
    else if (!str_cmp(arg2,"zarius")) currentstance = 23;
    else
    {
      send_to_char("No stance by that name.\n\r",ch);
      return;
    }
    if (ch->stance[currentstance] == -1)
    {
      send_to_char("No, you don't have that stance yet, SILLY!\n\r",ch);
      return;
    }
    send_to_char("This stance has the following powers :\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DODGE)) send_to_char("Advanced Dodge.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_PARRY)) send_to_char("Advanced Parry.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_SPEED)) send_to_char("Superior Speed.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_BYPASS)) send_to_char("The Bypassing ability.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMAGE_1)) send_to_char("Lesser Damage.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMAGE_2)) send_to_char("Greater Damage.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMAGE_3)) send_to_char("Superior Damage.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_RESIST_1)) send_to_char("Lesser Resistance.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_RESIST_2)) send_to_char("Greater Resistance.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_RESIST_3)) send_to_char("Supreme Resistance.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMCAP_1)) send_to_char("Lesser Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMCAP_2)) send_to_char("Greater Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_DAMCAP_3)) send_to_char("Supreme Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_REV_DAMCAP_1)) send_to_char("Lesser reverse Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_REV_DAMCAP_2)) send_to_char("Greater reverse Damcap.\n\r",ch);
    if (IS_SET(ch->stance[currentstance], STANCEPOWER_REV_DAMCAP_3)) send_to_char("Supreme reverse Damcap.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "done"))
  {
    if (ch->exp < cost * 1000000)
    {
      send_to_char("You don't have enough exp to buy this stance.\n\r",ch);
      return;
    }
    if (cost < min_cost)
    {
      xprintf(buf, "You need to spend at least %d million exp on this stance.\n\r", min_cost);
      send_to_char(buf, ch);
      return;
    }
    if (cost > max_cost)
    {
      xprintf(buf, "You can only spend %d million exp on this stance.\n\r", max_cost);
      send_to_char(buf, ch);
      return;
    }
    ch->stance[currentstance] = ch->stance[18];
    ch->stance[18] = 0;
    ch->exp -= cost * 1000000;
    switch (currentstance)
    {
      default: break;
      case 19: SET_BIT(ch->pcdata->jflags, JFLAG_SS1); break;
      case 20: SET_BIT(ch->pcdata->jflags, JFLAG_SS2); break;
      case 21: SET_BIT(ch->pcdata->jflags, JFLAG_SS3); break;
      case 22: SET_BIT(ch->pcdata->jflags, JFLAG_SS4); break;
      case 23: SET_BIT(ch->pcdata->jflags, JFLAG_SS5); break;
    }
    send_to_char("ok.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "clear"))
  {
    if (arg2[0] == '\0')
    {
      ch->stance[18] = 0;
      send_to_char("Stance cleared.\n\r",ch);
      return;
    }
    if (!str_cmp(arg2,"all") && !str_cmp(arg3,"stances"))
    {
      if ((ch->in_room == NULL || ch->in_room->vnum != 3054))
      {
        send_to_char("To clear ALL your superstance, you must be at the Temple Altar of Midgaard.\n\r",ch);
        return;
      }
      ch->stance[12] = STANCE_NONE;
      ch->stance[0] = 0;
      ch->stance[13] = 0;
      ch->stance[14] = 0;
      ch->stance[15] = 0;
      ch->stance[16] = 0;
      ch->stance[17] = 0;
      ch->stance[18] = 0;
      ch->stance[19] = -1;
      ch->stance[20] = -1;
      ch->stance[21] = -1;
      ch->stance[22] = -1;
      ch->stance[23] = -1;
      send_to_char("Ok.\n\r",ch);
    }
  }
  else if (!str_cmp(arg1, "dodge"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_DODGE ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_DODGE);
    else SET_BIT(ch->stance[18], STANCEPOWER_DODGE);
    send_to_char("Ok.\n\r",ch);
    return;
  }  
  else if (!str_cmp(arg1, "parry"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_PARRY ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_PARRY);
    else SET_BIT(ch->stance[18], STANCEPOWER_PARRY);
    send_to_char("Ok.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "speed"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_SPEED ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_SPEED);
    else SET_BIT(ch->stance[18], STANCEPOWER_SPEED);
    send_to_char("Ok.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "bypass"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_BYPASS ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_BYPASS);
    else SET_BIT(ch->stance[18], STANCEPOWER_BYPASS);
    send_to_char("Ok.\n\r",ch);
    return;
  }
  else if (arg2[0] == '\0')
  {  
    send_to_char("What?!?\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "damage"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2) || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
        {
          send_to_char("You allready have the damage power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMAGE_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_DAMAGE_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
        {
          send_to_char("You allready have the damage power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMAGE_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_DAMAGE_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      }
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2))
        {
          send_to_char("You allready have the damage power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMAGE_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_DAMAGE_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {  
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else if (!str_cmp(arg1, "damcap"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1))
      {  
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2) || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
        {
          send_to_char("You allready have the damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMCAP_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_DAMCAP_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      { 
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
        {
          send_to_char("You allready have the damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMCAP_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_DAMCAP_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      } 
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2))
        {
          send_to_char("You allready have the damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMCAP_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_DAMCAP_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {   
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else if (!str_cmp(arg1, "resist"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_RESIST_1))
      {  
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2) || IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
        {
          send_to_char("You allready have the resist power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_RESIST_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_RESIST_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      { 
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_RESIST_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1) || IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
        {
          send_to_char("You allready have the resist power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_RESIST_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_RESIST_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      } 
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1) || IS_SET(ch->stance[18], STANCEPOWER_RESIST_2))
        {
          send_to_char("You allready have the resist power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_RESIST_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_RESIST_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {   
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else if (!str_cmp(arg1, "rev_dc"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1))
      {  
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2) || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
        {
          send_to_char("You allready have the reverse damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      { 
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
        {
          send_to_char("You allready have the reverse damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      } 
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2))
        {
          send_to_char("You allready have the reverse damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {   
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else do_setstance(ch,"");
}

void do_address(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;

  send_to_char("Zarius's homepage      : http://www.mindcloud.com",ch);
  return;
}

void do_ragnarok( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  int amount;

  argument = one_argument(argument, arg);
  if (IS_NPC(ch)) return;
  if (ragnarok_timer < 120 * PULSE_AREA)
  {
    send_to_char("You may not call for ragnarok yet.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  if (arg[0] == '\0')
  {
    send_to_char("How many bones do you want to bid ?\n\r",ch);
    return;
  }
  if (!is_number(arg))
  {
    send_to_char("A number please, how many bones do you want to bid towards ragnarok.\n\r",ch);
    return;
  }
  amount = atoi(arg);
  if (amount < 100)
  {
    send_to_char("You cannot tempt the gods with this puny amount.\n\r",ch);
    return;
  }
  if (amount > 1000)
  {
    send_to_char("You don't want to spend this many bones to end the world.\n\r",ch);
    return;
  }
  if (ch->bones < amount)
  {
    send_to_char("You don't have the bones.\n\r",ch);
    return;
  }
  ch->bones -= amount;
  ragnarok_cost -= amount;
  if (ragnarok_cost <= 0)
  {
    ragnarok_cost = 5000;
    ragnarok = TRUE;
    ragnarok_timer = 0;
    do_info(ch,"#0The world comes to an end, #YRAGNAROK#0 is over us all!!!!#n");
  }
  else do_info(ch,"The ragnarok moves closer, the gods shiver with fear");
  return;
}

void do_timer( CHAR_DATA *ch, char *argument)
{
  int arenatime, ragnaroktime;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  if (arenaopen) arenatime = 0;
  else arenatime = (PULSE_ARENA - 45 * PULSE_PER_SECOND - pulse_arena);
  if (ragnarok) ragnaroktime = 0;
  else ragnaroktime = (120 * PULSE_AREA - ragnarok_timer);
  if (ragnaroktime < 0 ) ragnaroktime = 0;

  xprintf(buf, "Next arena will happen in       :  %3d minutes.\n\r", arenatime/PULSE_AREA);
    send_to_char(buf,ch);
  xprintf(buf, "Bidding on ragnarok starts in   :  %3d minutes.\n\r", ragnaroktime/PULSE_AREA);
    send_to_char(buf,ch);
    if (global_exp)
  {
  xprintf(buf, "Double Exp will end in          :  %3d minutes.\n\r", pulse_exp);
    send_to_char(buf,ch);
  }

  if (global_dt)
  {
  xprintf(buf, "Double Training will end in     :  %3d minutes.\n\r", pulse_dt );
    send_to_char(buf,ch);
  }
  
    if (global_cp)
  {
  xprintf(buf, "Double Class Points will end in :  %3d minutes.\n\r", pulse_cp );
    send_to_char(buf,ch);
  }

  return;
}

void do_exp( CHAR_DATA *ch, char *argument )
{  
  char buf[MAX_STRING_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  unsigned int cost, to, from;

  argument = one_argument(argument,arg1);
  argument = one_argument(argument,arg2);

  if (IS_NPC(ch)) return;
  if (arg1[0] == '\0' || arg2[0] == '\0')
  {
    send_to_char("Usage is :\n\r",ch);
    send_to_char("exp <from> <to>\n\r",ch);
    send_to_char("ie. exp 4000 6000 will tell you how many exp it will cost to train from 4K to 6K\n\r",ch);
    send_to_char("can be used for primal as well, just time the result by 500.\n\r", ch);
    return;
  }
  if (!is_number(arg1) || !is_number(arg2))
  {
    send_to_char("Please use numbers.\n\r",ch);
    return;
  }
  from = atoi(arg1);
  to = atoi(arg2);
  if (from >= to)
  {
    send_to_char("Begin with a smaller number.\n\r",ch);
    return;
  }
  if (to > 120000)
  {
    send_to_char("Thats beyond the statcap.\n\r",ch);
    return;
  }
  if ((to > 50000 && to - from > 20000) || (to > 100000 && to - from > 10000) || (to - from > 50000))
  {
    send_to_char("Please use a smaller interval to avoid integer overflows.\n\r",ch);
    return;
  }
  cost = (from + to) * (to - from + 1) / 2 - from;
  if (cost > 500000000)
    xprintf(buf,"The total cost in exp will be %d million\n\r", cost/1000000);
  else xprintf(buf,"The total cost in exp will be %d\n\r", cost);
  send_to_char(buf,ch);
  return;
}

void do_favor( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];

  one_argument(argument,arg);

  if (IS_NPC(ch)) return;
  if (IS_SET(ch->act, PLR_RIGHTHAND) || IS_SET(ch->act, PLR_LEFTHAND) || IS_SET(ch->act, PLR_AMBI))
  {
    send_to_char("Already set.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Favor left, right or both hands ?\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"left"))
  {
    SET_BIT(ch->act, PLR_LEFTHAND);
    send_to_char("You favor your left arm in combat.\n\r",ch);
  }
  else if (!str_cmp(arg,"right"))
  {
    SET_BIT(ch->act, PLR_RIGHTHAND);
    send_to_char("You favor your right arm in combat.\n\r",ch);
  }
  else if (!str_cmp(arg,"both"))
  {
    SET_BIT(ch->act, PLR_AMBI);
    send_to_char("You fight well with both arms.\n\r",ch);
  }
  else do_favor(ch,"");
  return;
}

void do_alias(CHAR_DATA *ch, char *argument)
{
  ALIAS_DATA *ali;
  char arg1[MAX_INPUT_LENGTH];
  char testarg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;

  argument = one_argument(argument, arg1);
  one_argument(argument, testarg);

  if ((ch->in_room == NULL || ch->in_room->vnum != 3054))
  {
    send_to_char("You must be at the Temple Altar of Midgaard to create a new alias.\n\r",ch);
    return;
  }
  if (ch->pcdata->alias_count >= MAX_ALIAS)
  {
    send_to_char("Sorry, you have reached the limit for aliases, you need to remove some before you can make more.\n\r",ch);
    return;
  }
  if (arg1[0] == '\0' || argument[0] == '\0')
  {
    send_to_char("Syntax : alias 'short' 'long'\n\r",ch);
    send_to_char("\n\rThe 'short' is the alias, and the 'long' is the string that the alias replaces.\n\r",ch);
    send_to_char("ie. alias tt teleport orc.\n\r",ch);
    return;
  }
  if (strlen2(argument) > 400)
  {
    send_to_char("Behave... that's way to big.\n\r",ch);
    return;
  }
  smash_tilde(arg1);
  smash_tilde(testarg);
  smash_tilde(argument);
  if (!str_cmp(arg1, testarg))
  {
    send_to_char("You cannot alias the alias.\n\r",ch);
    return;
  }
  if (!str_cmp(arg1, "showalias") || !str_cmp(arg1, "alias") || !str_cmp(arg1, "removealias"))
  {
    send_to_char("You really shouldn't alias the alias commands.\n\r", ch);
    return;
  }
  for (ali = ch->pcdata->alias; ali; ali = ali->next)
  {
    if (!str_cmp(arg1, ali->short_n))
    {
      send_to_char("You already have such an alias.\n\r",ch);
      return;
    }
    if (!str_cmp(arg1, ali->long_n) || !str_cmp(argument, ali->short_n) || !str_cmp(argument, ali->long_n))
    {
      send_to_char("No looping aliases please.\n\r", ch);
      return;
    }
  }
  if (alias_free == NULL )
  {
    ali = alloc_perm(sizeof(*ali));
  }
  else
  {
    ali = alias_free;
    alias_free = alias_free->next;
  }
  ali->short_n = str_dup(arg1);
  ali->long_n = str_dup(argument);
  ali->next = ch->pcdata->alias;
  ch->pcdata->alias = ali;
  ch->pcdata->alias_count++;
  send_to_char("Ok.\n\r",ch);
  return;
}

void do_showalias(CHAR_DATA *ch, char *argument)
{
  ALIAS_DATA *ali;
  bool found = FALSE;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  for (ali = ch->pcdata->alias; ali; ali = ali->next)
  {
    found = TRUE;
    xprintf(buf, "%s '%s'\n\r", ali->short_n, ali->long_n);
    send_to_char(buf,ch);
  }
  if (!found) send_to_char("You have no aliases.\n\r",ch);
  return;
}

void do_removealias(CHAR_DATA *ch, char *argument)
{
  ALIAS_DATA *ali;
  char arg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;

  one_argument(argument, arg);

  if (arg[0] == '\0')
  {
    send_to_char("Remove which alias?\n\r",ch);
    return;
  }
  for (ali = ch->pcdata->alias; ali; ali = ali->next)
  {
    if (!str_cmp(arg, ali->short_n))
    {
      alias_remove(ch, ali);
      send_to_char("Alias removed.\n\r",ch);
      ch->pcdata->alias_count--;
      return;
    }
  }
  send_to_char("No such alias.\n\r",ch);
  return;
}


void do_birth(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  DESCRIPTOR_DATA *dummy;

  if (IS_NPC(ch)) return;

  argument = one_argument(argument, arg);

  if (ch->sex != SEX_FEMALE)
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if (!IS_EXTRA(ch, EXTRA_PREGNANT))
  {
    send_to_char("But you are not even pregnant!\n\r",ch);
    return;
  }
  if (!IS_EXTRA(ch, EXTRA_LABOUR))
  {
    send_to_char("You're not ready to give birth yet.\n\r",ch);
    return;
  }
  if ( arg[0] == '\0' )
  {
    if (ch->pcdata->genes[4] == SEX_MALE)
      send_to_char( "What do you wish to name your little boy?\n\r", ch );
    else if (ch->pcdata->genes[4] == SEX_FEMALE)
      send_to_char( "What do you wish to name your little girl?\n\r", ch );
    else
      send_to_char( "What do you wish to name your child?\n\r", ch );
    return;
  }
  if (!check_parse_name(arg))
  {
    send_to_char("Thats an illegal name.\n\r", ch);
    return;
  }
  if (char_exists(FALSE, arg))
  {
    send_to_char("That player already exists.\n\r", ch);
    return;
  }
  if (descriptor_free == NULL )
  {
    dummy = alloc_perm(sizeof(*dummy));
  }
  else
  {
    dummy = descriptor_free;
    descriptor_free = descriptor_free->next;
  }
  arg[0] = UPPER(arg[0]);
  load_char_short(dummy, arg);
  victim = dummy->character;

  victim->pcdata->perm_str = 18;
  victim->pcdata->perm_int = 18;
  victim->pcdata->perm_wis = 18;
  victim->pcdata->perm_dex = 18;
  victim->pcdata->perm_con = 18;
  victim->stance[19] = -1;
  victim->stance[20] = -1;  
  victim->stance[21] = -1;
  victim->stance[22] = -1;
  victim->stance[23] = -1;
  victim->class = ch->class;
  victim->max_hit = 5000;
  victim->hit = 5000;
  victim->max_mana = 5000;
  victim->mana = 5000;
  victim->max_move = 5000;
  victim->move = 5000;
  victim->sex = ch->pcdata->genes[4];
  victim->level = 2;
  victim->generation = 6;
  
  victim->in_room = ch->in_room;
  set_learnable_disciplines(victim);
  save_char_obj(victim);
  free_char(victim);
  if (ch->pcdata->genes[4] == SEX_MALE)
    xprintf(buf, "Son of %s and %s.", ch->name, ch->pcdata->cparents);
  else
    xprintf(buf, "Daughter of %s and %s.", ch->name, ch->pcdata->cparents);
  victim->description = str_dup( buf );
    
  /*
   * Being nice and putting the descriptor back into the free list.
   */
  dummy->next     = descriptor_free;
  descriptor_free = dummy;

  ch->pcdata->genes[9] += 1;
  REMOVE_BIT(ch->extra, EXTRA_PREGNANT);
  REMOVE_BIT(ch->extra, EXTRA_LABOUR);  
  save_char_obj(ch);
  if (ch->pcdata->genes[4] == SEX_MALE)
    xprintf(buf, "%s gives birth to %s's son, named %s!", ch->name, ch->pcdata->cparents, arg);
  else
    xprintf(buf, "%s gives birth to %s's daughter, named %s!", ch->name, ch->pcdata->cparents, arg);
  do_info(ch, buf);
  send_to_char("Pop!\n\r", ch);
  interpret(ch, "bleed");
  return;
}

void do_areaaffects(CHAR_DATA *ch, char *argument)
{
  AREA_AFFECT *paf;
  bool found = FALSE;
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  send_to_char("This area has been enchanted with :\n\r\n\r", ch);
  for (paf = ch->in_room->area->affects; paf; paf = paf->next)
  {
    found = TRUE;
    if (paf->type == AREA_AFF_GZONE)            xprintf(buf, "  God's Zone");
    else if (paf->type == AREA_AFF_THORNS)      xprintf(buf, "  Thorns and Brambles");
    else if (paf->type == AREA_AFF_PESTILENCE)  xprintf(buf, "  Pestilence");
    else if (paf->type == AREA_AFF_BADMOON)     xprintf(buf, "  Bad Moon");
    else if (paf->type == AREA_AFF_GAIA)        xprintf(buf, "  Gaia's Blessing");
    else if (paf->type == AREA_AFF_CHAOS)       xprintf(buf, "  Chaotic Landscape");
    else xprintf(buf, "  Unknown");

    xprintf(buf2, "%s by %s for a duration of %d hours\n\r",
      buf, religion_table[paf->religion].truename, paf->duration);
    send_to_char(buf2, ch);
  }
  if (!found) send_to_char("  nothing.\n\r", ch);
  return;
}


void do_selfdelete(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  if (argument[0] == '\0')
  {
    send_to_char("Syntax : selfdelete <your password>\n\r", ch);
    send_to_char("WARNING : THIS WILL REMOVE YOU PERMANENTLY!!\n\r", ch);
    return;
  }

  if (has_timer(ch)) return;

  /*
   * check for password.
   */
  if (strcmp(mc_crypt(argument), ch->pcdata->pwd))
  {
    send_to_char("Illegal password.\n\r", ch);
    WAIT_STATE(ch, 12);
    return;
  }

  /*
   * logged.
   */
  xprintf(buf, "%s has selfdeleted, what a fool.", ch->name);
  log_string(LOG_GAME, buf);

  /*
   * set up the deletion
   */
  xprintf(buf, "%s%c/%s", PLAYER_DIR, tolower(ch->pcdata->switchname[0]),capitalize(ch->pcdata->switchname));

  /*
   * make sure the player is offline.
   */
  ch->fight_timer = 0;   
  char_from_room(ch);
  char_to_room(ch, get_room_index(ROOM_VNUM_LIMBO));
  if (ch->level > 6)
  {
		update_wizlist(ch, 1);
	    }
  do_quit(ch, "");

  unlink(buf);

  return;
}

void do_leader( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  
  if (IS_NPC(ch)) return;
  
  xprintf(buf, " %s\n\r\n\r", get_dystopia_banner("  Leader Board  ", 72));
  send_to_char(buf, ch);
  
  send_to_char("   #yMost Player Kills  #C--->   ", ch);
  xprintf(buf, "#G%-13s", leader_board.pk_name);
  send_to_char(buf, ch);
  xprintf(buf, " #ywith #G%d #yKills#n\n\r", leader_board.pk_number);
  send_to_char(buf, ch);
  
  send_to_char("   #yMost Hours Played  #C--->   ", ch);
  xprintf(buf, "#G%-13s", leader_board.time_name);
  send_to_char(buf, ch);
  xprintf(buf, " #ywith #G%d #yHours#n\n\r", leader_board.time_number);
  send_to_char(buf, ch);
  
  send_to_char("   #yMost Mobs Killed   #C--->   ", ch);
  xprintf(buf, "#G%-13s", leader_board.mobkills_name);
  send_to_char(buf, ch);
  xprintf(buf, " #ywith #G%d #yKills#n\n\r", leader_board.mobkills_number);
  send_to_char(buf, ch);
    
  send_to_char("   #yHighest PK score   #C--->   ", ch);
  xprintf(buf, "#G%-13s", leader_board.pkscore_name);
  send_to_char(buf, ch);
  xprintf(buf, " #ywith #G%d #yPoints#n\n\r", leader_board.pkscore_number);
  send_to_char(buf, ch);
    
  send_to_char("   #yMost Arena Wins    #C--->   ", ch);
  xprintf(buf, "#G%-13s", leader_board.arena_name);
  send_to_char(buf, ch);
  xprintf(buf, " #ywith #G%d #yWins#n\n\r", leader_board.arena_number);
  send_to_char(buf, ch);

  xprintf(buf, "\n\r %s\n\r", get_dystopia_banner("", 68));
  send_to_char(buf, ch);
    
  return;
}

void do_policy(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  argument = one_argument(argument, arg);
  if (IS_SET(ch->pcdata->jflags, JFLAG_POLICY))
  {
    send_to_char("You have already accepted the policy.\n\r", ch);
    return;
  }
  if (!str_cmp(arg, "accept"))
  {
    if (strcmp(mc_crypt(argument), ch->pcdata->pwd))
    {
      send_to_char("Illegal password.\n\r", ch);  
      WAIT_STATE(ch, 12);
      return;
    }
    send_to_char("You have accepted the player policy at Mindcloud Mud.\n\r", ch);
    SET_BIT(ch->pcdata->jflags, JFLAG_POLICY);
    do_autosave(ch, "");
    xprintf(buf, "%s has accepted the player policy.", ch->name);
    log_string(LOG_GAME, buf);
    return;
  }
  else if (!str_cmp(arg, "decline"))
  {
    if (strcmp(mc_crypt(argument), ch->pcdata->pwd))
    {
      send_to_char("Illegal password.\n\r", ch);
      WAIT_STATE(ch, 12);
      return;
    }
    xprintf(buf, "%s has declined the player policy.", ch->name);
    log_string(LOG_GAME, buf);
    xprintf(buf, "%s%c/%s", PLAYER_DIR, tolower(ch->pcdata->switchname[0]),capitalize(ch->pcdata->switchname));
    ch->fight_timer = 0;
    char_from_room(ch);
    char_to_room(ch, get_room_index(ROOM_VNUM_LIMBO));
    do_quit(ch, "");
    unlink(buf);
  }
  else
  {
    send_to_char("Syntax : policy [accept|decline] [password]\n\r", ch);
    return;
  }
  return;
}




void extra_action(CHAR_DATA *ch, char *argument, int type)
{
  EXTRA_DESCR_DATA *ed;
  ROOM_INDEX_DATA *location;
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  CHAR_DATA *gch;
  CHAR_DATA *gch_next;
  char arg[MAX_INPUT_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int sn;

  one_argument(argument, arg);
 
  for (ed = ch->in_room->extra_descr; ed; ed = ed->next)
  {
    if (is_full_name(arg, ed->keyword))
    {
      if (ed->type == type)
      {
        switch(type)
        {
          case ED_TYPE_PULL:
            xprintf(buf1, "You pull %s.\n\r", ed->keyword);
            xprintf(buf2, "$n pulls %s.", ed->keyword);
            break;
          case ED_TYPE_PRESS:
            xprintf(buf1, "You press %s.\n\r", ed->keyword);
            xprintf(buf2, "$n presses %s.", ed->keyword);
            break;
          case ED_TYPE_PUSH:
            xprintf(buf1, "You push %s.\n\r", ed->keyword);
            xprintf(buf2, "$n pushes %s.", ed->keyword);
            break;
          case ED_TYPE_TOUCH:
            xprintf(buf1, "You touch %s.\n\r", ed->keyword);
            xprintf(buf2, "$n touches %s.", ed->keyword);
            break;
        }
        send_to_char(buf1, ch);
        act(buf2, ch, NULL, NULL, TO_ROOM);
      }
      else
      {
        switch(type)
        {
          case ED_TYPE_PULL:
            send_to_char("That cannot be pulled.\n\r", ch);
            break;
          case ED_TYPE_PUSH:
            send_to_char("That cannot be pushed.\n\r", ch);
            break;
          case ED_TYPE_PRESS:
            send_to_char("That cannot be pressed.\n\r", ch);
            break;
          case ED_TYPE_TOUCH:
            send_to_char("You seem unable to touch it.\n\r", ch);
            break;
        }
        return;
      }
      switch(ed->action)  
      {
        default:
          send_to_char("Nothing happens.\n\r", ch);
          return;
        case ED_ACTION_TELEPORT:
          if ((location = get_room_index(ed->vnum)) == NULL)
          {
            send_to_char("Nothing happens.\n\r", ch);
            return;
          }
          act(ed->buffer1, ch, NULL, NULL, TO_CHAR);
          act(ed->buffer2, ch, NULL, NULL, TO_ROOM);
          char_from_room(ch);
          char_to_room(ch, location);
          do_look(ch, "auto");
          return;
        case ED_ACTION_OBJECT:
          if ((pObjIndex = get_obj_index(ed->vnum)) == NULL)
          {
            send_to_char("Bug : Please report this.\n\r", ch);
            return;
          }
          obj = create_object( pObjIndex, 50);
          if (CAN_WEAR(obj, ITEM_TAKE)) obj_to_char(obj, ch);
          else obj_to_room(obj, ch->in_room);
          act(ed->buffer1, ch, NULL, NULL, TO_CHAR);
          act(ed->buffer2, ch, NULL, NULL, TO_ROOM);
          return;  
        case ED_ACTION_SPELL:
          if ((sn = skill_lookup(ed->buffer1)) < 0)
          {
            send_to_char("Bug : Please report this.\n\r", ch);
            return;
          }
          (*skill_table[sn].spell_fun) (sn, number_range(40,50), ch, ch);
          return;
        case ED_ACTION_ELEVATOR:
          if ((location = get_room_index(ed->vnum)) == NULL)
          {
            send_to_char("Bug : Please report this.\n\r", ch);
            return;
          }
          if (location->people)
          {
            act(ed->buffer2, ch, NULL, NULL, TO_ALL);
            return;
          }
          for (gch = ch->in_room->people; gch; gch = gch_next)
          {
            gch_next = gch->next_in_room;
            char_from_room(gch);
            char_to_room(gch, location);
            act(ed->buffer1, gch, NULL, NULL, TO_CHAR);
          }
          return;
      }
    }
  }
  send_to_char("You are unable to find that.\n\r", ch);
}
/*
void do_pull(CHAR_DATA *ch, char *argument)
{
  extra_action(ch, argument, ED_TYPE_PULL);
}

void do_press(CHAR_DATA *ch, char *argument)
{
  extra_action(ch, argument, ED_TYPE_PRESS);
}

void do_push(CHAR_DATA *ch, char *argument)
{
  extra_action(ch, argument, ED_TYPE_PUSH);
}

void do_touch(CHAR_DATA *ch, char *argument)
{
  extra_action(ch, argument, ED_TYPE_TOUCH);
}
*/
void do_ignore(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  DESCRIPTOR_DATA *d;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int i;
  
  if (IS_NPC(ch)) return;
  one_argument(argument, arg);
  
  if (arg[0] == '\0')
  {
    xprintf(buf, "You are currently ignoring :\n\r");
    for (d = descriptor_list; d; d = d->next)
    {
      if (d->connected != CON_PLAYING) continue;
      if ((victim = d->character) == NULL) continue;
      for (i = 0; i < MAX_IGNORE; i++)
      {
        if (ch->pcdata->ignore[i] == victim->pcdata->playerid)
        {
          strcat(buf, "* ");  
          strcat(buf, victim->name);
          strcat(buf, "\n\r");
        }
      }
    }
    send_to_char(buf, ch);
    return;
  }
  if (!str_cmp(arg, "clear"))
  {
    for (i = 0; i < MAX_IGNORE; i++)
    {
      ch->pcdata->ignore[i] = -1;
    }  
    send_to_char("All ignores cleared.\n\r", ch);
    return;
  }
  for (victim = char_list; victim; victim = victim->next)
  {
    if (IS_NPC(victim)) continue;
    if (!str_prefix(victim->name, arg))
    {
      for (i = 0; i < MAX_IGNORE; i++)
      {
        if (ch->pcdata->ignore[i] == victim->pcdata->playerid)
        {
          ch->pcdata->ignore[i] = -1;
          xprintf(buf, "You stop ignoring %s.\n\r", victim->name);
          send_to_char(buf, ch);
          return;
        }
      }
      for (i = 0; i < MAX_IGNORE; i++)
      {
        if (ch->pcdata->ignore[i] == -1)
        {
          ch->pcdata->ignore[i] = victim->pcdata->playerid;
          xprintf(buf, "You now ignore %s.\n\r", victim->name);
          send_to_char(buf, ch);
          return;
        }
      }
      send_to_char("You cannot ignore any more people, try 'ignore clear'.\n\r", ch);
      return;
    }
  }
  send_to_char("Ignore whom?\n\r", ch);
  return;
}

void do_vt100(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (IS_SET(ch->pcdata->tempflag, TEMP_VT100))
  {
    REMOVE_BIT(ch->pcdata->tempflag, TEMP_VT100);
    send_to_char(VT_RESET_TERMINAL, ch);
    return;
  }
  if (!IS_SET(ch->in_room->room_flags, ROOM_SAFE))
  {
    send_to_char("You cannot enable vt100 outside safe rooms.\n\r", ch);
    return;
  }
  send_to_char("Requesting VT100 information from your client.\n\r", ch);
  send_to_char("\e[999;999H", ch);
  send_to_char("\e[6n", ch);
  return;
}

void do_classportal(CHAR_DATA *ch, char *argument)
{
  ROOM_INDEX_DATA *location;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  char buf4[MAX_STRING_LENGTH];
  int cost = 0;
  bool move = FALSE;

  one_argument(argument, arg);
  if (IS_NPC(ch)) return;
  switch(ch->class)
  {
    default:
      send_to_char("Huh?\n\r", ch);
      return;
    case CLASS_DEMON:
      if (!IS_DEMPOWER(ch, DEM_TRAVEL))
      {
        send_to_char("You haven't been granted the gift of travel.\n\r",ch);
        return;
      }
      xprintf(buf1, "You sink into the ground, and make your way towards $N.");
      xprintf(buf2, "$n sinks into the ground.");
      xprintf(buf3, "You rise up from the ground, opposing $N.");
      xprintf(buf4, "$n rises up out of the ground.");
      move = TRUE;
      cost = 500;
      break;
    case CLASS_MAGE:
      if (ch->pcdata->powers[PINVOKE] < 1)
      {
        send_to_char("You don't have that invoke yet.\n\r",ch);
        return;
      }
      xprintf(buf1, "You utter a single powerword, teleporting you towards $N.");
      xprintf(buf2, "$n utters a strange sounding word and disappers.");
      xprintf(buf3, "You materialize infront of $N.");
      xprintf(buf4, "$n suddenly appears from out of nowhere.");
      move = FALSE;
      cost = 500;
      break;
    case CLASS_WEREWOLF:
      if (ch->power[DISC_WERE_LUNA] < 6)
      {
        send_to_char("You need to obtain level 6 Luna to use moongate.\n\r",ch);
        return;
      }
      move = FALSE; 
      cost = 500;
      break;
    //case CLASS_BEASTMASTER:
      // Need something here
      //xprintf(buf1, "You run through the forests, searching for $N.");
      //xprintf(buf2, "$n disappears into the forest.");
      //xprintf(buf3, "You leap out of the forest before $N.");
      //xprintf(buf4, "$n leaps out of from behind a tree.");
      //move = TRUE; 
      //cost = 500;
      //break;
    case CLASS_VAMPIRE:
      if (ch->power[DISC_VAMP_AUSP] < 4)
      {
        send_to_char("You must obtain level 4 Auspex to use Astral Walk.\n\r",ch);
        return;
      }
      xprintf(buf1, "You enter the astral plane, searching for $N.");
      xprintf(buf2, "$n enters the astral plane.");
      xprintf(buf3, "You leap out of the astral plane before $N.");
      xprintf(buf4, "$n leaps out of a rift in the astral plane.");
      move = TRUE; 
      cost = 500;
      break;
    case CLASS_SAMURAI:
      xprintf(buf1, "You follow the ancestral path to $N.");
      xprintf(buf2, "$n steps into the air, leaving behind no trace whatsoever.");
      xprintf(buf3, "You step out of the air infront of $N.");
      xprintf(buf4, "$n steps out of the air in front of you.");
      move = TRUE; 
      cost = 1000;
      break;
    case CLASS_DROW:
      if (!IS_SET(ch->pcdata->powers[1], DPOWER_SHADOWWALK))
      {
        send_to_char("You have not been granted that power yet.\n\r", ch);
        return;
      }
      xprintf(buf1, "You walk into the shadows, hunting $N.");
      xprintf(buf2, "$n walks into the shadows.");
      xprintf(buf3, "You walk out of the shadows in front of $N.");
      xprintf(buf4, "$n walks out of the shadows.");
      move = TRUE; 
      cost = 500;
      break;
    case CLASS_MONK:
      if (ch->pcdata->powers[PMONK] < 10)
      {
        send_to_char("You must obtain level 10 in mantras to use Celestial Path.\n\r",ch);
        return;
      }
      xprintf(buf1, "You concentrate and open a Celestial path to $N.");
      xprintf(buf2, "$n opens a celestial gateway and steps into it.");
      xprintf(buf3, "You leave a celestial path and stand before $N.");
      xprintf(buf4, "$n appears before you through a Celestial portal!");
      move = FALSE; 
      cost = 500;
      break;
    case CLASS_NINJA:
      if(!IS_SET(ch->pcdata->powers[SHADOW_POWERS], NSHADOWS_SHADOWWALK))
      {
        send_to_char("You have not learned that kill yet.\n\r", ch );
        return;
      }
      xprintf(buf1, "You walk into the shadows, hunting $N.");
      xprintf(buf2, "$n walks into the shadows.");
      xprintf(buf3, "You walk out of the shadows in front of $N.");
      xprintf(buf4, "$n walks out of the shadows.");
      move = FALSE;
      cost = 500;
      break;
    case CLASS_LICH:
      if (ch->pcdata->powers[CON_LORE] < 3)
      {
        send_to_char("You need conjuring level 3 to do this.\n\r",ch);
        return;
      }
      move = FALSE;
      cost = 1000;
      break;
    case CLASS_SHAPESHIFTER:
      if (ch->pcdata->powers[SHAPE_POWERS] < 2)
      {
        send_to_char("You need level 2 shiftpowers.\n\r",ch);
        return;
      }
      xprintf(buf1, "You reform into thin mist and seep into the floor, tracking $N.");
      xprintf(buf2, "$n reforms into thin mist and seeps into the floor.");
      xprintf(buf3, "You seep up from the floor and reform in front of $N.");
      xprintf(buf4, "A fine mist seeps up from the floor and reforms into $n.");
      move = TRUE;
      cost = 1000;
      break;
    case CLASS_TANARRI:
      if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_CHAOSGATE))
      {
        send_to_char("You don't have that power yet.\n\r",ch);
        return;
      }
      xprintf(buf1, "You open a gate made from pure chaos, trying to reach $N.");
      xprintf(buf2, "$n opens a gate of pure chaos and steps into it.");
      xprintf(buf3, "You step out of the gate in front of $N.");
      xprintf(buf4, "A gate appears from out of nowhere and $n steps out of it.");
      move = TRUE;
      cost = 1000;
      break;
    case CLASS_ANGEL:
      if (ch->pcdata->powers[ANGEL_JUSTICE] < 1)
      {
        send_to_char("You haven't learned to fly yet.\n\r",ch);
        return;
      }
      if (!IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_WINGS))
      {
        send_to_char("Where are your wings?\n\r",ch);
        return;
      }
      xprintf(buf1, "You fly towards $N.");
      xprintf(buf2, "$n flies into the sky.");
      xprintf(buf3, "You swoop down upon $N.");
      xprintf(buf4, "$n swoops down from the sky.");
      move = TRUE;
      cost = 1000;  
      break;
    case CLASS_UNDEAD_KNIGHT:
      xprintf(buf1, "You ride your skeleton steed towards $N.");
      xprintf(buf2, "$n leaves the room, riding a black horse.");
      xprintf(buf3, "You arrive in front of $N.");
      xprintf(buf4, "$n rides toward you on $s skeleton steed!");
      move = TRUE;
      cost = 1000;
      break;
    case CLASS_DRONE:
      if (ch->pcdata->powers[DRONE_MINDCONTROL] < 3)
      {
        send_to_char("You haven't learned to dreamwalk yet.\n\r",ch);
        return;
      }
      xprintf(buf1, "You fade into the dreamworld, searching for $N's dreams.");
      xprintf(buf2, "$n closes $s eyes and fades away.");
      xprintf(buf3, "You enter $N's dreams and step out into the real world next to $M.");
      xprintf(buf4, "$n suddenly fades into existance.");
      move = FALSE;
      cost = 1000;
      break;
    case CLASS_FAE:
      if (ch->power[DISC_FAE_NATURE] < 6)
      {
        send_to_char("You need level 6 discipline in nature.\n\r",ch);
        return;
      }
      xprintf(buf1, "You enter the nearest tree, searching for the roots of $N.");
      xprintf(buf2, "$n steps right into a tree, and is gone.");
      xprintf(buf3, "You step out from another tree right in front of $N.");
      xprintf(buf4, "$n steps out of a tree right next to you.");
      move = FALSE; 
      cost = 1000;
      break;
    case CLASS_GIANT:
      if (ch->pcdata->rank < FOOT_20)
      {
        send_to_char("You're not high enough to do that, shorty.\n\r",ch);
        return;
      }
      xprintf(buf1, "You take a giant step towards $N.");
      xprintf(buf2, "$n takes a giant step, and is gone.");
      xprintf(buf3, "You step down in front of $N.");
      xprintf(buf4, "$n drops down in front of you, seemingly from one giant step.");
      move = TRUE; 
      cost = 500;
      break;
  }
  if (cost == 0) cost = 10000; // debug.

  if (is_webbed(ch)) return;
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch );
    return;
  }
  if (victim->in_room == ch->in_room)
  {
    send_to_char("You are already there.\n\r", ch);
    return;
  }
  if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    send_to_char("They are in the astral sphere.\n\r", ch);
    return;
  }
  if (IS_IMMUNE(victim, IMM_TRAVEL) && !IS_NPC(victim))
  {
    if (IS_CLASS(ch, CLASS_DRONE) && victim->position == POS_SLEEPING) ;
    else
    {
      send_to_char("I don't think they want you to do that.\n\r", ch);
      return;
    }
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    send_to_char("Your room is not connected to the astral plane.\n\r", ch);
    return;
  }
  if (move && ch->move < cost)
  {
    send_to_char("You need more move.\n\r", ch);
    return;
  }
  else if (!move && ch->mana < cost)
  {
    send_to_char("You need more mana.\n\r", ch);
    return;
  }
  if (move) ch->move -= cost;
  else ch->mana -= cost;
  location = victim->in_room;

  if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_LICH))
  {
    act(buf1, ch, NULL, victim, TO_CHAR);
    act(buf2, ch, NULL, NULL, TO_ROOM);
    char_from_room(ch);
    char_to_room(ch, location);
    do_look(ch, "auto");
    act(buf3, ch, NULL, victim, TO_CHAR);
    act(buf4, ch, NULL, NULL, TO_ROOM);
  }
  else
  {
    OBJ_DATA *obj;
    int duration;

    /* A portal to the victim */
    duration = number_range(2,3);
    if (IS_CLASS(ch, CLASS_LICH))
      obj = create_object(get_obj_index(OBJ_VNUM_PORTAL), 0);
    else
      obj = create_object(get_obj_index(OBJ_VNUM_GATE2), 0);
    obj->value[0] = victim->in_room->vnum;
    obj->value[3] = ch->in_room->vnum;
    obj->timer = duration;
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE)) obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room(obj, ch->in_room);

    /* and a portal that leads the other way */
    if (IS_CLASS(ch, CLASS_LICH))
      obj = create_object(get_obj_index(OBJ_VNUM_PORTAL), 0);
    else
      obj = create_object(get_obj_index(OBJ_VNUM_GATE2), 0);
    obj->value[0] = ch->in_room->vnum;
    obj->value[3] = victim->in_room->vnum;
    obj->timer = duration;
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE)) obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room(obj, victim->in_room);

    if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
      act("A look of concentration passes over $n's face.", ch, NULL,NULL, TO_ROOM);
      send_to_char("A look of concentration passes over your face.\n\r", ch);
      act("$p appears in front of $n in a beam of moonlight.", ch, obj, NULL, TO_ROOM);
      act("$p appears in front of you in a beam of moonlight.", ch, obj, NULL, TO_CHAR);
      act("$p appears in front of $n in a beam of moonlight.", victim, obj, NULL, TO_ROOM);
      act("$p appears in front of you in a beam of moonlight.", ch, obj, victim, TO_VICT);
    }
    else
    {
      act("$p appears in front of $n.", ch, obj, NULL, TO_ROOM);
      act("$p appears in front of you.", ch, obj, NULL, TO_CHAR);
      act("$p appears in front of $n.", victim, obj, NULL, TO_ROOM);
      act("$p appears in front of you.", ch, obj, victim, TO_VICT);
    }
  }
  return;
}

void do_newscore(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  char tempbuf[MAX_STRING_LENGTH];
  char linebuf1[MAX_STRING_LENGTH];
  char linebuf2[MAX_STRING_LENGTH];
  char linebuf3[MAX_STRING_LENGTH];
  char linebuf4[MAX_STRING_LENGTH];
  char linebuf5[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  xprintf(linebuf1, " %s\n\r", get_dystopia_banner("Mindcloud 2 Score", 72));
  xprintf(linebuf2, " %s\n\r", get_dystopia_banner("Stats", 72));
  xprintf(linebuf3, " %s\n\r", get_dystopia_banner("Kill Info", 72));
  xprintf(linebuf4, " %s\n\r", get_dystopia_banner("Class/Misc Info", 72));
  xprintf(linebuf5, " %s\n\r", get_dystopia_banner("Immortal Info", 72));


  send_to_char(linebuf1, ch);
  send_to_char("\n\r", ch);

  xprintf(buf, "  Your name is #C%s #nand you have been playing for #C%d #nhours\n\r",
    ch->name, (ch->played + (int) (current_time - ch->logon))/3600);
  send_to_char(buf, ch);
  birth_date(ch, TRUE);
    if (!IS_NPC(ch) && IS_EXTRA(ch, EXTRA_PREGNANT)) birth_date(ch, FALSE);
  send_to_char("\n\r", ch);

  xprintf(tempbuf, "  STR: #G%3d#n    Hitpoints : #C%d#n/#C%d#n",
    get_curr_str(ch), ch->hit, ch->max_hit);
  xprintf(buf, "%-55s #uLast two kills#n\n\r", tempbuf);
  send_to_char(buf, ch);

  xprintf(tempbuf, "  DEX: #G%3d#n    Movement  : #C%d#n/#C%d#n",
    get_curr_dex(ch), ch->move, ch->max_move);
  xprintf(buf, "%-55s #C%s#n\n\r", tempbuf, ch->pcdata->last_decap[0]);
  send_to_char(buf, ch);

  xprintf(tempbuf, "  CON: #G%3d#n    Magics    : #C%d#n/#C%d#n",
    get_curr_con(ch), ch->mana, ch->max_mana);
  xprintf(buf, "%-55s #C%s#n\n\r", tempbuf, ch->pcdata->last_decap[1]);
  send_to_char(buf, ch);

  xprintf(tempbuf, "  WIS: #G%3d#n    Status    : #Y%d#n",
    get_curr_wis(ch), ch->race);
  xprintf(buf, "%-51s #uRetaliation#n\n\r", tempbuf);
  send_to_char(buf, ch);

  xprintf(tempbuf, "  INT: #G%3d#n    Bones/Bank: #Y%d#n/#G%d#n",
    get_curr_int(ch), ch->bones,
    (!IS_NPC(ch) ? ch->pcdata->bank : 0));
  xprintf(buf, "%-55s #C%s#n\n\r", tempbuf, ch->pcdata->retaliation);
  send_to_char(buf, ch);
  

  
  xprintf(buf,"  You are currently on Tier #R%d#n\n\r",ch->tier);
    stc(buf,ch);
	
    xprintf(buf,"  You have #Y%d #nTier Kills for this tier#n\n\r",ch->tks);
    stc(buf,ch);

  send_to_char(linebuf2, ch);
  send_to_char("\n\r", ch);

  xprintf(buf, "  Damroll : #C%d#n  Hitroll : #C%d#n  Armor : #C%d#n  Damcap : #C%d#n\n\r",
    char_damroll(ch), char_hitroll(ch), char_ac(ch), ch->damcap[0]);
  send_to_char(buf, ch);

  xprintf(buf, "  You are carrying #C%d#n/#C%d#n items with weight #C%d#n/#C%d#n lbs\n\r",
    ch->carry_number, can_carry_n(ch), ch->carry_weight, can_carry_w(ch));
  send_to_char(buf, ch);
  if (!IS_NPC(ch))
  {
    ch_printf(ch, "  You are a %s#n member of the bank.", 
    ch->pcdata->membership == 0 ? "#Dbasic" :
    ch->pcdata->membership == 1 ? "#ybronze" :
    ch->pcdata->membership == 2 ? "#Wsilver" :
    ch->pcdata->membership == 3 ? "#Ygold" : "#Wp#Cl#Wa#Ct#Wi#Cn#Wu#Cm#n" );
  }
  
  xprintf(buf, "\n\r  Primal : #C%d#n   Experience : #C%d#n   Faithpoints : #C%d#n/#C%d#n\n\r",
    ch->practice, ch->exp, ch->pcdata->current_faith, ch->pcdata->faith);
  send_to_char(buf, ch);

  xprintf(buf, "  Alignment : #C%d#n - ",
    ch->alignment);
  send_to_char(buf, ch);

  /* class stuff */
  if (IS_CLASS(ch, CLASS_DEMON))
  {
    xprintf(buf, "  Class Points : #C%d#n   Souls : #C%d#n   Rage : #C%d#n\n\r",
      ch->pcdata->stats[DEMON_CURRENT], ch->pcdata->souls, ch->rage);
    send_to_char(buf, ch);
  }
  else if (IS_CLASS(ch, CLASS_DROW))
  {
    xprintf(buf, "  Class Points : #C%d#n   Magic Resistance : #C%d#n\n\r",
      ch->pcdata->stats[DEMON_CURRENT], ch->pcdata->stats[DROW_MAGIC]);
    send_to_char(buf, ch);
  }
  else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    xprintf(buf, "  Shift Counter : #C%d#n   Phase Counter : #C%d#n\n\r",
      ch->pcdata->powers[SHAPE_COUNTER], ch->pcdata->powers[PHASE_COUNTER]);
    send_to_char(buf, ch);
  }
  else if (IS_CLASS(ch, CLASS_ANGEL))
  {
    xprintf(buf, "  Justice : #C%d#n   Love : #C%d#n   Harmony : #C%d#n   Peace : #C%d#n\n\r",
      ch->pcdata->powers[ANGEL_JUSTICE], ch->pcdata->powers[ANGEL_LOVE],
      ch->pcdata->powers[ANGEL_HARMONY], ch->pcdata->powers[ANGEL_PEACE]);
    send_to_char(buf, ch);
  }
  else if (IS_CLASS(ch, CLASS_VAMPIRE))
  {
    xprintf(buf, "  Beast  : #C%d#n   Rage : #C%d#n   Blood : #C%d#n\n\r",
      ch->beast, ch->rage, ch->pcdata->condition[COND_THIRST]);
    send_to_char(buf, ch);
  }
  else if (IS_CLASS(ch, CLASS_WEREWOLF))
  {
    xprintf(buf, "  Rage : #C%d#n   Gnosis : #C%d#n/#C%d#n\n\r",
      ch->rage, ch->gnosis[GCURRENT], ch->gnosis[GMAXIMUM]);
    send_to_char(buf, ch);
  }
  else if (IS_CLASS(ch, CLASS_MONK))
  {
    xprintf(buf, "  Block Counter : #C%d#n   Chi : #C%d#n/#C%d#n\n\r",
      ch->monkblock, ch->chi[CURRENT], ch->chi[MAXIMUM]);
    send_to_char(buf, ch);
  }
  else if (IS_CLASS(ch, CLASS_GIANT) || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch, CLASS_TANARRI))
  {
    xprintf(buf, "  Class Points : #C%d#n\n\r", ch->pcdata->stats[8]);
    send_to_char(buf, ch);
  }
  
  send_to_char( "  You are ", ch );
         if ( ch->alignment >  900 ) send_to_char( "#Cangelic#n", ch );
    else if ( ch->alignment >  700 ) send_to_char( "#Csai#Yn#Ctly#n", ch );
    else if ( ch->alignment >  350 ) send_to_char( "#Yg#Coo#Yd#n",    ch );
    else if ( ch->alignment >  100 ) send_to_char( "#Ykind#n",    ch );
    else if ( ch->alignment > -100 ) send_to_char( "#Bneutral#n", ch );
    else if ( ch->alignment > -350 ) send_to_char( "#0mean#n",    ch );
    else if ( ch->alignment > -700 ) send_to_char( "#0e#Rvi#0l#n",    ch );
    else if ( ch->alignment > -900 ) send_to_char( "#Rdem#0o#Rnic#n", ch );
    else                             send_to_char( "#Rsatanic#n", ch );
    send_to_char( "\n\r", ch );

  send_to_char(linebuf3, ch);
  send_to_char("\n\r", ch);

  xprintf(tempbuf, "  Players  : #C%6d#n %s and #C%3d#n %s",
    ch->pkill, (ch->pkill == 1) ? "kill" : "kills",
    ch->pdeath, (ch->pdeath == 1) ? "death" : "deaths");
  xprintf(buf, "%-50s Ratio #C%3d#n%s\n\r",
    tempbuf, calc_ratio(ch->pkill, ch->pdeath), "%");
  send_to_char(buf, ch);

  xprintf(tempbuf, "  Arena    : #C%6d#n %s and #C%3d#n %s",
    ch->pcdata->awins, (ch->pcdata->awins == 1) ? "kill" : "kills",
    ch->pcdata->alosses, (ch->pcdata->alosses == 1) ? "death" : "deaths");
  xprintf(buf, "%-50s Ratio #C%3d#n%s\n\r",
    tempbuf, calc_ratio(ch->pcdata->awins, ch->pcdata->alosses), "%");
  send_to_char(buf, ch);

  xprintf(buf, "  Monsters : #C%6d#n %s and #C%3d#n %s\n\r",
    ch->mkill, (ch->mkill == 1) ? "kill" : "kills",
    ch->mdeath, (ch->mdeath == 1) ? "death" : "deaths");
  send_to_char(buf, ch);

  send_to_char(linebuf4, ch);
  send_to_char("\n\r", ch);
  
  if (!IS_NPC(ch) && IS_CLASS( ch, CLASS_DEMON))
    {
  xprintf(buf, "  You have #C%d#n out of #C%d #npoints of demonic power stored.\n\r",
	ch->pcdata->stats[DEMON_CURRENT],  ch->pcdata->stats[DEMON_TOTAL] );
	send_to_char( buf, ch );
    }

    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_CYBORG)))
    {
  xprintf(buf, "  You have #C%d #nCyborg points stored\n\r",ch->pcdata->stats[CYBORG_POWER] );
        send_to_char( buf, ch );
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW))
  {
  xprintf(buf, "  You have #C%d #nShadow Points stored\n\r", ch->pcdata->powers[SHADOW_POWER]);
    send_to_char(buf, ch);
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PRIEST))
  {
  xprintf(buf, "  You have #W%d #nFaith Points stored\n\r", ch->pcdata->stats[FAITH_PTS]);
    send_to_char(buf, ch);
  }
    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_TANARRI)))
    {
  xprintf(buf, "  You have #C%d #nTanar'ri points stored\n\r", ch->pcdata->stats[TPOINTS] );
	send_to_char( buf, ch);
    }
    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_GHOUL)) )
    {
	xprintf( buf,
	"  You have #C%d #GGhoul #nPoints stored\n\r",
	ch->pcdata->stats[GHOUL_PTS] );
	send_to_char( buf, ch );
    }
    if (IS_CLASS(ch, CLASS_GIANT))
  {
    xprintf(buf, "  Giant Points : #C%d#n\n\r", ch->pcdata->stats[8]);
    send_to_char(buf, ch);
  }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON))
    {
	xprintf( buf,
	"  You have obtained #C%d #nsouls, which you keep in a bloody jar\n\r",
	ch->pcdata->souls);
	stc(buf, ch);
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW)) {
	xprintf( buf, "  You have #C%d#n drow power points stored\n\r",
	ch->pcdata->stats[DROW_POWER] );
	send_to_char( buf, ch );
	xprintf( buf, "  You have #C%d #npoints of magic resistance\n\r",
	ch->pcdata->stats[DROW_MAGIC] );
	send_to_char( buf, ch ); 
	if (weather_info.sunlight == SUN_DARK) 
	send_to_char("  You feel strong in the night\n\r", ch );}
    
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      xprintf(buf,"  Your shapeshifter counter is : #C%d#n\n\r",ch->pcdata->powers[SHAPE_COUNTER]);
      stc(buf,ch);
      xprintf(buf,"  Your phase counter is        : #C%d#n\n\r",ch->pcdata->powers[PHASE_COUNTER]);
      stc(buf,ch);
    }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
    {
      xprintf(buf,"  Angelic Justice   : #C%d#n\n\r",ch->pcdata->powers[ANGEL_JUSTICE]);
      stc(buf,ch);
      xprintf(buf,"  Angelic Love      : #C%d#n\n\r",ch->pcdata->powers[ANGEL_LOVE]);
      stc(buf,ch);
      xprintf(buf,"  Angelic Harmony   : #C%d#n\n\r",ch->pcdata->powers[ANGEL_HARMONY]);
      stc(buf,ch);
      xprintf(buf,"  Angelic Peace     : #C%d#n\n\r",ch->pcdata->powers[ANGEL_PEACE]);
      stc(buf,ch);
   }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
    {
      xprintf(buf,"  Your current beast is : #C%d#n\n\r",ch->beast);
      stc(buf,ch);

    }
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) )
    {
      xprintf(buf,"  Your block counter is currently: #C%d#n\n\r",ch->monkblock);
      stc(buf,ch);
      xprintf(buf,"  Your current level of chi:       #C%d#n\n\r",ch->chi[CURRENT]);
      stc(buf,ch);
      xprintf(buf,"  Your maximum level of chi:       #C%d#n\n\r",ch->chi[MAXIMUM]);
      stc(buf,ch);
    }
    
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI) )
    {
      xprintf(buf,"  Your current focus points:       #C%d#n\n\r",ch->focus[CURRENT]);
      stc(buf,ch);
      xprintf(buf,"  Your maximum focus points:       #C%d#n\n\r",ch->focus[MAXIMUM]);
      stc(buf,ch);
    }
    
    if( !IS_NPC(ch) && (IS_CLASS(ch, CLASS_WEREWOLF)) && ch->gnosis[GMAXIMUM] > 0 )
    {
	xprintf( buf, "  Current Gnosis:            #C%d#n\n\r", ch->gnosis[GCURRENT] );
	stc( buf, ch);
	xprintf( buf, "  Maximum Gnosis:            #C%d#n\n\r", ch->gnosis[GMAXIMUM] );
	stc( buf, ch);
    }
    
    if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 100 )
	send_to_char( "  You are feeling extremely horny\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 50 )
	send_to_char( "  You are feeling pretty randy#R]\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 1 )
	send_to_char( "  You are feeling rather kinky#R]\n\r",  ch );

    if ( !IS_NPC(ch) && ch->pcdata->stage[1] > 0 && ch->position == POS_STANDING )
    {
	send_to_char( "  You are having sexual intercourse\n\r",  ch );
	if (!IS_NPC(ch) && (ch->pcdata->stage[2] + 25) >= ch->pcdata->stage[1])
	    send_to_char( "  You are on the verge of having an orgasm\n\r",  ch );
    }
    else switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char( "  You are #RD#0E#RA#0D#n!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "  You are #Rmortally #nwounded\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "  You are incapacitated\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "  You are stunned\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "  You are sleeping\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "  You are resting\n\r",		ch );
	break;
    case POS_MEDITATING:
	send_to_char( "  You are meditating\n\r",	ch );
	break;
    case POS_SITTING:
	send_to_char( "  You are sitting\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "  You are standing\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "  You are fighting\n\r",		ch );
	break;
    }

    if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) )
    {
	xprintf( buf, "  Blood: #C%d#n\n\r", ch->pcdata->condition[COND_THIRST] );
	send_to_char( buf, ch );

    }
    else if (ch->level >= 0)
	send_to_char( "\n\r", ch );

    if (IS_AFFECTED(ch,AFF_HIDE)) send_to_char( "  You are keeping yourself hidden from those around you\n\r", ch );

    if ( !IS_NPC(ch) )
    {
	if ( IS_CLASS(ch,CLASS_WEREWOLF) && ch->siltol > 0)
	{
	    xprintf(buf,"  You have attained #C%d #npoints of silver tolerance\n\r",ch->siltol);
	    send_to_char( buf, ch );
	}
	if ( IS_CLASS(ch,CLASS_VAMPIRE) && ch->rage > 0)
	{
	    xprintf(buf,"  The beast is in control of your actions:  Affects Hitroll and Damroll by #C+%d\n\r",ch->rage);
	    send_to_char( buf, ch );
	}
	if ( (IS_CLASS(ch, CLASS_WEREWOLF)) && IS_SET(ch->special,SPC_WOLFMAN) && ch->rage > 0)
	{
	    xprintf(buf,"  You are raging:  Affects Hitroll and Damroll by #C+%d\n\r",ch->rage);
	    send_to_char( buf, ch );
	}
	if (IS_CLASS(ch, CLASS_NINJA) && ch->rage > 0) {
	xprintf(buf,"  Your michi gives you an extra #C%d#n hitroll and damroll\n\r", ch->rage);
	send_to_char(buf,ch);
	}
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 6
	&& ch->pcdata->powers[HARA_KIRI] > 0) {
	xprintf(buf,"  You receive the power of HaraKiri for #C%d#n more ticks\n\r",ch->pcdata->powers[HARA_KIRI]);
	send_to_char(buf,ch);
	}

        if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_VAMPIRE) && ch->rage > 0 && !IS_CLASS(ch, CLASS_NINJA))
	{
	   xprintf(buf,"  You are in a mad frenzy, adding #C%d#n Hitroll and Damroll\n\r",ch->rage);
	   stc(buf, ch);
	}

	else if ( IS_CLASS( ch, CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
	{
	    xprintf(buf,"  You are wearing demonic armour:  Affects Hitroll and Damroll by #C+%d\n\r",
		((ch->pcdata->stats[DEMON_POWER]) *ch->pcdata->stats[DEMON_POWER]));
	    send_to_char( buf, ch );
	}
        else if ( IS_CLASS( ch, CLASS_DROW) && ch->pcdata->stats[DEMON_POWER] > 0)
        {
            xprintf(buf,"  You are wearing drow armour:  Affects Hitroll and Damroll by #C+%d\n\r",
                ((ch->pcdata->stats[DEMON_POWER]) *ch->pcdata->stats[DEMON_POWER]));  
            send_to_char( buf, ch );
        }

    }	
	if (ch->fight_timer > 0) {
	xprintf(buf,"  You have #C%d#n rounds left on your fight timer#n\n\r", ch->fight_timer);
	send_to_char(buf,ch);
  }
      if ( IS_IMMORTAL ( ch ) )
    {
  	 send_to_char(linebuf5, ch);
  	 send_to_char("\n\r", ch);
    	 xprintf(buf, "  #nInvis: #C%3d#n    Incog Level: #C%d#n    Ghost Level    #C%3d#n\n\r",
         ch->invis_level, ch->incog_level, ch->ghost_level);
         stc(buf, ch);
    }
  return;
}


