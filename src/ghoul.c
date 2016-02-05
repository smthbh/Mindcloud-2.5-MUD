/*			  <(()Seeds of Hate:  Swords & Sorcery Edition())>					 |
|					    _\|::>< Ghoul Class ><::|/_								 |
|						  |Class Tier Number: I|								 |
|							  |Coder: Trent|									 |
|  *Ghoul is an original class made for Seeds of Hate, Swords & Sorcery Edition! |
|	Please don't copy without permission and due credit!						 |
|	Thanks.																	     |
|		 -Trent, God of PaiN!   (ASDF!)											 |
X--------------------------------------------------------------------------------X
*/
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
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

#define GPTS(ch)	(ch->pcdata->stats[GHOUL_PTS])

void do_realms( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_GHOUL))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	xprintf(buf,"#cRealms of Power#n: #GS#chadow#n #g(#G%d#g)#n, #GZ#combie#n #g(#G%d#g), #GU#cndead #GM#cight#n #g(#G%d#g).#n\n\r",
	ch->pcdata->powers[GPOWER_SHADOW], ch->pcdata->powers[GPOWER_ZOMBIE],
	ch->pcdata->powers[GPOWER_UNDEADMIGHT]);
	send_to_char(buf,ch);
	return;
    }
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"shadow"))
	{
	    send_to_char("#cShadows - #GThe realm of darkness and night, which a ghoul thrives in.\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_SHADOW] < 1) 
			send_to_char("#cYou have no knowledge of the Shadow Realm.\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_SHADOW] > 0) 
			send_to_char("#cShadowsight - #GThe ability to see into the nether realm#n.\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_SHADOW] > 1) 
			send_to_char("#cTruesight - #GThe power to see all.#n\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_SHADOW] > 2) 
			send_to_char("#cShadowplane - #GThe ability to merge into the Shadow Realm.#n\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_SHADOW] > 3) 
			stc("#cPerception  - #GThe ability to make your attacks more efficient.  Adds hit/dam.#n\n\r",ch);
		if (ch->pcdata->powers[GPOWER_SHADOW] > 4) 
			send_to_char("#cGore - #GA vicious attack from the Shadow Plane#n\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"zombie"))
	{
	    send_to_char("#cZombie - #GThe Innate gifts of the Undead.#n\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_ZOMBIE] < 1) 
			send_to_char("#cYou have no knowledge of the frightening powers the Undead possess.#n\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_ZOMBIE] > 0) 
			send_to_char("#cClaws - #GThe god of Death grants you special, razor sharp claws.#n\n\r         #gSyntax: Gcreate Claws#n\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_ZOMBIE] > 1) 
			send_to_char("#cDisease - #GYour claws infect others with the essence of disease.#n\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_ZOMBIE] > 2) 
			send_to_char("#cRavage - #GA ferenzical clawing attack.#n\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_ZOMBIE] > 3) 
			send_to_char("#cStench of Decay - #GRemoves a characters Protection.#n\n\r",ch);
		if (ch->pcdata->powers[GPOWER_ZOMBIE] > 4)
			send_to_char("#cStench of Death - #GRemoves a characters Sanctuary#n.\n\r", ch);
	    return;
	}
	else if (!str_cmp(arg1,"undeadmight"))
	{
	    send_to_char("#cUndead Might - #GYour Strength from Beyond the Grave!.#n\n\r",ch);
	    if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] < 1) 
		send_to_char("#cUndead Might: [     ]#n\n\r",ch);
	    else if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] > 4) 
			send_to_char("#cUndead Might: [#G*****#c]#n\n\r",ch);
	    else if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] > 3) 
			send_to_char("#cUndead Might: [#G**** #c]#n\n\r",ch);
	    else if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] > 2) 
			send_to_char("#cUndead Might: [#G***  #c]#n\n\r",ch);
	    else if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] > 1) 
			send_to_char("#cUndead Might: [#G**   #c]#n\n\r",ch);
		else if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] > 0)
			send_to_char("#cUndead Might: [#G*    #c]#n\n\r", ch);
	    return;
	}
	xprintf(buf,"#cRealms of Power#n: #GS#chadow#n #g(#G%d#g)#n, #GZ#combie#n #g(#G%d#g), #GU#cndead #GM#cight#n #g(#G%d#g).#n\n\r",
	ch->pcdata->powers[GPOWER_SHADOW], ch->pcdata->powers[GPOWER_ZOMBIE],
	ch->pcdata->powers[GPOWER_UNDEADMIGHT]);
	send_to_char(buf,ch);
	return;
    }
    if (!str_cmp(arg2,"manifest"))
    {
	int improve;
	int cost;
	int max;

	     if (!str_cmp(arg1,"shadow"   )) {improve = GPOWER_SHADOW;   max=5;}
	else if (!str_cmp(arg1,"zombie"   )) {improve = GPOWER_ZOMBIE;   max=5;}
	else if (!str_cmp(arg1,"undeadmight"   )) {improve = GPOWER_UNDEADMIGHT;   max=5;}
	else
	{
	    send_to_char("#GR#cealms#n: #GS#chadow#n, #GZ#combie#n, #GU#cndead#GM#cight#n.\n\r",ch);
	    return;
	}							
	cost = (ch->pcdata->powers[improve]+1) * 1000;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    xprintf(buf,"#cYou have already gained all the powers of the #0%s #crealms.#n\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->pcdata->stats[GHOUL_PTS] )
	{
	    xprintf(buf,"#cYou need #G%d #cGhoul Points to manifest in the #0%s #crealm.#n\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->pcdata->stats[GHOUL_PTS] -= cost;
	xprintf(buf,"#cYou manifest deeper into the #0%s #crealm!.#n\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("#cTo learn powers, type #Grealm <realm> manifest.#n\n\r",ch);
	return;
}

void do_ghouleq( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_GHOUL))
  {
      send_to_char("What?\n\r",ch);
      return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("#GPlease specify which piece of ghoulish armor you wish to make: #wRing Bands Choker Vest "
                 "Helmet Boots Gloves Sleeves Pants Cloak Rope Mask.\n\r",ch);
    return;
  }

    if ( ch->practice < 200 )
    {
        send_to_char("It costs 200 points of primal to create ghoul equipment.\n\r",ch);
        return;
    }

    if      (!str_cmp(arg,"ring"	)) vnum = 6204;    
    else if (!str_cmp(arg,"bands"	)) vnum = 6205;
    else if (!str_cmp(arg,"choker"	)) vnum = 6206;
    else if (!str_cmp(arg,"vest"        )) vnum = 6207;
    else if (!str_cmp(arg,"helmet"      )) vnum = 6208;
    else if (!str_cmp(arg,"boots"       )) vnum = 6209;
    else if (!str_cmp(arg,"gloves"      )) vnum = 6210;
    else if (!str_cmp(arg,"sleeves"     )) vnum = 6211;
    else if (!str_cmp(arg,"pants"       )) vnum = 6212;
    else if (!str_cmp(arg,"cloak"       )) vnum = 6213;
    else if (!str_cmp(arg,"rope"        )) vnum = 6214;
    else if (!str_cmp(arg,"mask"        )) vnum = 6215;
    else
    {
    do_ghouleq(ch,"");
    return;
    }
    
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
    send_to_char("Missing object, please inform a God.\n\r",ch);
    return;
    }

    ch->practice -= 200;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj->ownerid = ch->pcdata->playerid;
    obj_to_char(obj, ch);
    act("You form some living flesh into $p.",ch,obj,NULL,TO_CHAR);
    act("$n forms some living flesh into $p.",ch,obj,NULL,TO_ROOM);
    return;
}


void do_gcreate( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;
    
  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_GHOUL))
  {
    send_to_char("What?\n\r",ch);
    return;
  }
  	if (ch->pcdata->powers[GPOWER_ZOMBIE] < 1) 
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}
  
  if (arg[0] == '\0')
  {
    send_to_char("syntax: gcreate claws\n\r",ch);
    return;
  }
  if ( ch->practice < 500 )
  {
    send_to_char("It costs 500 primal to create a pair of Ghoul Claws.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"claws")) vnum = 33015;
  else
  {
    do_gcreate(ch,"");
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
 
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj->ownerid = ch->pcdata->playerid;
  obj_to_char(obj, ch);

  ch->practice -= 500;
  act("A pair of $p appears in your hands.",ch,obj,NULL,TO_CHAR);
  act("A pair of $p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
  return;
  }

void do_gore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

        if (IS_NPC(ch)) return;
    
    if (!IS_CLASS(ch, CLASS_GHOUL))
    {
            send_to_char("Huh?\n\r",ch);
            return;
    }    
	
	if (ch->pcdata->powers[GPOWER_SHADOW] < 5) 
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

	if ( arg[0] == '\0' )
    {
	send_to_char( "Gore whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "#cWhatever#n\n\r", ch );
	return;
    }
    
	if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL
    ||   obj->value[3] != 5 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL
    ||   obj->value[3] != 5 ) )
    {
	send_to_char( "#cYou cannot gore without Claws!!#n\n\r", ch );
	return;
    }

    if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
	{
	send_to_char( "#cYou must be in Shadowplane to do this!#n\n\r", ch );
	return;
	}

	if (ch->move < 10000)
	{
	send_to_char( "#cYou need 10000 move to use this!#n\n\r", ch );
	return;
	}

    if (IS_NPC(victim))
	{
	    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    send_to_char("#cYou materialize in front of your opponent and #Rgore him with your sharp claws!#n\n\r",ch);
	act("#G$n #cmaterializes out of thin air, screaming wildly, and #Rgores $s opponent with $s razor sharp claws!!#n",ch,NULL,NULL,TO_ROOM);
	ch->move -= 10000;
    WAIT_STATE( ch, 3 * PULSE_VIOLENCE );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return;
	}

    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    send_to_char("#cYou materialize in front of your opponent and #Rgore him with your sharp claws!#n\n\r",ch);
	act("#G$n #cmaterializes out of thin air, screaming wildly, and #Rgores you with $s razor sharp claws!!#n",ch,NULL,NULL,TO_VICT);
	ch->move -= 10000;
    WAIT_STATE( ch, 3 * PULSE_VIOLENCE );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	if (number_percent() > 85)
	multi_hit( ch, victim, TYPE_UNDEFINED );
	if (number_percent() > 95)
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return;
	}

void do_ravage( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

        if (IS_NPC(ch)) return;
    
    if (!IS_CLASS(ch, CLASS_GHOUL))
    {
            send_to_char("Huh?\n\r",ch);
            return;
    }    
	
	if (ch->pcdata->powers[GPOWER_ZOMBIE] < 3) 
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

	if ( arg[0] == '\0' )
    {
	send_to_char( "Ravage Whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	if ( (victim = ch->fighting) == NULL)
	{
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    }

    if ( victim == ch )
    {
	send_to_char( "#cWhatever#n\n\r", ch );
	return;
    }
    
	if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL
    ||   obj->value[3] != 5 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL
    ||   obj->value[3] != 5 ) )
    {
	send_to_char( "#cYou cannot ravage without Claws!!#n\n\r", ch );
	return;
    }

	if (ch->move < 2000)
	{
	send_to_char( "#cYou need 2000 move to use this!#n\n\r", ch );
	return;
	}

    if (IS_NPC(victim))
	{
    send_to_char("#cYou #rRA#RVA#rGE#c your opponent with your sharp claws!#n\n\r",ch);
	act("#G$n #rRA#RVAG#rES #c$s opponent with $s razor sharp claws!!#n",ch,NULL,NULL,TO_ROOM);
	ch->move -= 2000;
    ch->hitroll += 2000;
	WAIT_STATE( ch, 24 );	
	multi_hit( ch, victim, TYPE_UNDEFINED );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	ch->hitroll -= 2000;	
	return;
	}

    send_to_char("#cYou #rRA#RVA#rGE#c your opponent with your sharp claws!#n\n\r",ch);
	act("#G$n #rRA#RVAG#rES #cyou with $s razor sharp claws!!#n",ch,NULL,NULL,TO_VICT);
	ch->move -= 2000;
    ch->hitroll += 2000;
	WAIT_STATE( ch, 24 );	
	multi_hit( ch, victim, TYPE_UNDEFINED );
	multi_hit( ch, victim, TYPE_UNDEFINED );	
	if (number_percent() > 90)
	multi_hit( ch, victim, TYPE_UNDEFINED );
	ch->hitroll -= 2000;	
	return;
	}

void do_sdecay( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

        if (IS_NPC(ch)) return;
    
    if (!IS_CLASS(ch, CLASS_GHOUL))
    {
            send_to_char("Huh?\n\r",ch);
            return;
    }    
	
	if (ch->pcdata->powers[GPOWER_ZOMBIE] < 4) 
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

	if ( arg[0] == '\0' )
    {
	send_to_char( "Use Stench of Decay upon whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "#cWhatever#n\n\r", ch );
	return;
    }

	  if ( victim->position <= POS_FIGHTING )
  {
    send_to_char( "#cNot while fighting!#n\n\r", ch );
    return;
  }

	if ( !IS_AFFECTED(victim, AFF_PROTECT) )
  {
	send_to_char( "#cThey don't have protection on!#n\n\r", ch );
	return;
  }

	if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
	{
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);    
	}

    REMOVE_BIT(victim->affected_by, AFF_PROTECT);
	send_to_char("#cYour decaying body emits a most foul scent!#n\n\r",ch);
	act("#G$n's #cdecaying body emits a most foul scent, making you gag!#n\n\r",ch,NULL,NULL,TO_VICT);
	WAIT_STATE( victim, 16 );
	WAIT_STATE( ch, 8 );
	return;
	}

void do_sdeath( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

        if (IS_NPC(ch)) return;
    
    if (!IS_CLASS(ch, CLASS_GHOUL))
    {
            send_to_char("Huh?\n\r",ch);
            return;
    }    
	
	if (ch->pcdata->powers[GPOWER_ZOMBIE] < 5) 
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

	if ( arg[0] == '\0' )
    {
	send_to_char( "Use Stench of Death upon whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "#cWhatever#n\n\r", ch );
	return;
    }

	  if ( victim->position <= POS_FIGHTING )
  {
    send_to_char( "#cNot while fighting!#n\n\r", ch );
    return;
  }

	if ( !IS_AFFECTED(victim, AFF_SANCTUARY) )
  {
	send_to_char( "#cThey don't have sanctuary on!#n\n\r", ch );
	return;
  }

	if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
	{
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);    
	}
	REMOVE_BIT(victim->affected_by, AFF_SANCTUARY);
	send_to_char("#cYour decaying body emits an #gUNGODLY #cscent!#n\n\r",ch);
	act("#G$n's #cdecaying body emits an #gUNGODLY #cscent, making you want to retch!#n\n\r",ch,NULL,NULL,TO_VICT);
	WAIT_STATE( ch, 12 );
	WAIT_STATE( victim, 16 );
	return;
	}

void do_swampwalk( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_GHOUL) )
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Use the swamps to find the path to whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not of this world.\n\r", ch );
	return;
    }
    
   if( ch->move < 1500) 
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
    act("$n sinks into the swamps.", ch, NULL, victim, TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    ch->move -= 1500;
    act("$n rises from the swamps in front of $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n rises from the swamps in front of you.", ch, NULL, victim, TO_VICT);
    do_say(ch,"BRAAAAAAAAINS...");    
    do_look(ch,"auto");
    return;
}
