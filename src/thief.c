/*  (O)xx}========> Thief Class  <========{xx(O)
                      By Trent
				    Seeds of Hate
			  Swords & Sorcery Edition
		====================================
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


void do_thiefskills(CHAR_DATA *ch,char *argument)
{
 
        if (IS_NPC(ch)) return;
 
        if (!IS_CLASS(ch, CLASS_THIEF))  {
        send_to_char("#cHuh?\n\r", ch );
        return;}
        
        send_to_char("#rT#Rh#0ief Skil#Rl#rs#n\n\n\r", ch );
		
		if (IS_SET(ch->pcdata->powers[1],THIEF_CONCEAL))
        send_to_char("#0Hide in Shadows (Conceal)#n\n\r", ch );
		
		if (IS_SET(ch->pcdata->powers[1],THIEF_GLEAM))
		send_to_char("#0Gleam of Nighteyes#n\n\r", ch );

		if (IS_SET(ch->pcdata->powers[1],THIEF_SWIFTNESS))
		send_to_char("#0Swiftness (auto)#n\n\r", ch );

		if (IS_SET(ch->pcdata->powers[1],THIEF_CRITICALSTRIKE))
		send_to_char("#0Critical Strike (auto)#n\n\r", ch );

		if (IS_SET(ch->pcdata->powers[1],THIEF_BACKSTAB))
		send_to_char("#0Backstab (bstab)#n\n\r", ch );

		if (IS_SET(ch->pcdata->powers[1],THIEF_POISONBLADE))
		send_to_char("#0Poison Blade#n\n\r", ch );

		if (IS_SET(ch->pcdata->powers[1],THIEF_TDAGGER))
		send_to_char("#0Throwing Dagger (tdagger)#n\n\r", ch );

		if (IS_SET(ch->pcdata->powers[1],THIEF_TOUGHSKIN))
		send_to_char("#0Toughskin (auto)#n\n\r", ch );

		if (IS_SET(ch->pcdata->powers[1],THIEF_MIGHT))
		send_to_char("#0Might of the Panther(auto)#n\n\r", ch );

		if (IS_SET(ch->pcdata->powers[1],THIEF_DTRICK))
		send_to_char("#0Dirty Trick (dtrick)#n\n\r", ch );
        
		send_to_char("\n\r", ch);
        return;
}


void do_rolldice(CHAR_DATA *ch,char *argument)
{
	int	roll;

        if (IS_NPC(ch)) return;
 
        if (!IS_CLASS(ch, CLASS_THIEF))  {
        send_to_char("#cHuh?\n\r", ch );
        return;}
		
		if (ch->practice < 100)
		{
		send_to_char("#cIt costs 100 primal to roll the magikal dice!\n\r", ch );
		return;}

		ch->practice -= 100;
		send_to_char("#CYou roll the dice of fate...#n\n\r", ch );

	roll = number_range(1,10);
    
	if ( (roll == 1) )
	{
	send_to_char("#CThe die spins on its side, then comes up 1!\n\r", ch );
		if (IS_SET(ch->pcdata->powers[1],THIEF_GLEAM))    
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }
	
    SET_BIT(ch->pcdata->powers[1], THIEF_GLEAM);
	send_to_char("#cYou gain #0Gleam of Night Eyes#c!#n\n\r", ch );
	return;
	}

	else if ( (roll == 2) )
	{
	send_to_char("#CThe die spins on its side, then comes up 2!\n\r", ch );
    
		if (IS_SET(ch->pcdata->powers[1],THIEF_CONCEAL))  
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }

	SET_BIT(ch->pcdata->powers[1], THIEF_CONCEAL);
	send_to_char("#cYou gain #0Hide in Shadows#c!#n\n\r", ch );
	return;
	}

	else if ( (roll == 3) )
	{
	send_to_char("#CThe die spins on its side, then comes up 3!\n\r", ch );
    
		if (IS_SET(ch->pcdata->powers[1],THIEF_SWIFTNESS))  
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }
	
    SET_BIT(ch->pcdata->powers[1], THIEF_SWIFTNESS);
	send_to_char("#cYou gain #0Swiftness#c!#n\n\r", ch );
	return;
	}

	else if ( (roll == 4) )
	{
	send_to_char("#CThe die spins on its side, then comes up 4!\n\r", ch );
    
		if (IS_SET(ch->pcdata->powers[1],THIEF_CRITICALSTRIKE))  
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }
	
    SET_BIT(ch->pcdata->powers[1], THIEF_CRITICALSTRIKE);
	send_to_char("#cYou gain the #0Critical Strike#c skill!#n\n\r", ch );
	return;
	}

	else if ( (roll == 5) )
	{
	send_to_char("#CThe die spins on its side, then comes up 5!\n\r", ch );
    
		if (IS_SET(ch->pcdata->powers[1],THIEF_BACKSTAB))  
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }
	
    SET_BIT(ch->pcdata->powers[1], THIEF_BACKSTAB);
	send_to_char("#cYou gain the #0Backstab#c skill!#n\n\r", ch );
	return;
	}
	
	else if ( (roll == 6) )
	{
	send_to_char("#CThe die spins on its side, then comes up 6!\n\r", ch );
    
		if (IS_SET(ch->pcdata->powers[1],THIEF_POISONBLADE))  
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }
	
    SET_BIT(ch->pcdata->powers[1], THIEF_POISONBLADE);
	send_to_char("#cYou gain the #0PoisonBlade#c skill!#n\n\r", ch );
	return;
	}

	else if ( (roll == 7) )
	{
	send_to_char("#CThe die spins on its side, then comes up 7!\n\r", ch );
    
		if (IS_SET(ch->pcdata->powers[1],THIEF_TDAGGER))  
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }
	
    SET_BIT(ch->pcdata->powers[1], THIEF_TDAGGER);
	send_to_char("#cYou learn the art of #0Throwing Daggers#c!#n\n\r", ch );
	return;
	}

	else if ( (roll == 8) )
	{
	send_to_char("#CThe die spins on its side, then comes up 8!\n\r", ch );
    
		if (IS_SET(ch->pcdata->powers[1],THIEF_TOUGHSKIN))  
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }
	
    SET_BIT(ch->pcdata->powers[1], THIEF_TOUGHSKIN);
	send_to_char("#cYou train to withstand more pain and gain the #0Toughskin#c discipline!#n\n\r", ch );
	return;
	}

	else if ( (roll == 9) )
	{
	send_to_char("#CThe die spins on its side, then comes up 9!\n\r", ch );
    
		if (IS_SET(ch->pcdata->powers[1],THIEF_MIGHT))  
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }
	
    SET_BIT(ch->pcdata->powers[1], THIEF_MIGHT);
	send_to_char("#cYou gain the #0Might of the Panther#c!#n\n\r", ch );
	return;
	}

	else if ( (roll == 10) )
	{
	send_to_char("#CThe die spins on its side, then comes up 10!\n\r", ch );
    
		if (IS_SET(ch->pcdata->powers[1],THIEF_DTRICK))  
    {
	send_to_char("#CYou already have that power! Roll again.#n\n\r",ch);
	ch->practice += 100;
	return;
    }
	
    SET_BIT(ch->pcdata->powers[1], THIEF_DTRICK);
	send_to_char("#cYou learn some #0Dirty Tricks#c!#n\n\r", ch );
	return;
	}

return;
}

void do_tconceal( CHAR_DATA *ch, char *argument )
{

	if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_THIEF))  
    {
    	send_to_char("#cHuh?\n\r", ch ); 
    	return;
    }

	if (!IS_SET(ch->pcdata->powers[1],THIEF_CONCEAL)) 
	{
	stc("#RYou must have the #0Hide in Shadows#R skill to do this!#n\n\r", ch );
	return; 
	}

    if (ch->fight_timer > 0)
    {
	stc("#RNot with a fight timer!#n\n\r", ch );
    return;
	}

	if (ch->move < 2500)
	{
	stc("#RYou do not have the necessary 2500 move!\n\r", ch );
	return;
	}

  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
	if ( IS_SET(ch->act, PLR_WIZINVIS) )
    {
	REMOVE_BIT(ch->act, PLR_WIZINVIS);
	act( "#R$n #0steps out of the shadows.#n", ch, NULL, NULL, TO_ROOM );
	send_to_char( "#0You step out of the shadows.#n\n\r", ch );
	return;
	}
    else
    {
	act( "#R$n #0dissapears into the shadows, dagger in hand.#n", ch, NULL, NULL, TO_ROOM );
	send_to_char( "#0You dissapear into the shadows, dagger in hand.#n\n\r", ch );
	SET_BIT(ch->act, PLR_WIZINVIS);
    }
	ch->move -= 2500;
    return;
}

void do_gleam(CHAR_DATA *ch, char *argument)
{
	if ( IS_NPC(ch) )
	return;

	if (!IS_CLASS(ch, CLASS_THIEF) || !IS_SET(ch->pcdata->powers[1],THIEF_GLEAM))
	{
	stc("Huh?\n\r",ch);
	return;
	}
    
	if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "#0Your eyes stop gleaming.#n\n\r", ch );
	}
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "#0Your eyes begin to gleam in the night.#n\n\r", ch );
    }
    return;
}

void do_bstab( CHAR_DATA *ch, char *argument)
	{
		CHAR_DATA *victim;
		OBJ_DATA *obj;
		char arg1[MAX_INPUT_LENGTH];
		char buf[MAX_STRING_LENGTH];
		int dam;
		int assassinationchance;
		int randomizer;
		argument = one_argument( argument, arg1 );

		if (IS_NPC(ch))
		return;

		if (!IS_CLASS(ch, CLASS_THIEF) || !IS_SET(ch->pcdata->powers[1],THIEF_BACKSTAB))
		{
		stc("Huh?\n\r",ch);
		return;
		}
		
		if ( arg1[0] == '\0')
	    {
		send_to_char( "#0Backstab whom?#n\n\r", ch );
		return;
		}

    if ( ( victim = get_char_room( ch, NULL, arg1 ) ) == NULL )
    {
	send_to_char( "#0They aren't here.#n\n\r", ch );
	return;
    }
		
		if ((victim == ch))
		{
		stc("#RLike to see you pull THAT one off...#n\n\r",ch);
		return;
		}

		if ((( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11)
		&& ((obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || obj->value[3] != 11))
		{
		send_to_char( "You need to wield a piercing weapon.\n\r", ch );
		return;
		}
		if ( victim->fighting != NULL )
		{
		send_to_char( "You can't backstab a fighting person.\n\r", ch );
		return;
		}

		if ( !IS_SET(ch->act, PLR_WIZINVIS) )
		{
		stc("#0You must be concealed in the shadows to preform this technique!#n\n\r",ch);
		return;
		}


		if (ch->move < 10000)
		{
		stc("#0You do not have the required #R10000 #rm#Rove#n.\n\r",ch);
		return;
		}
		randomizer = (10000 / (ch->max_hit / ch->hit));
		assassinationchance = number_range(1, 10);
		dam = number_range(victim->max_hit * .1, victim->max_hit * .25);
	        if (!IS_NPC(victim) && dam > 7000) dam = (number_fuzzy(number_range(1,5) * 5) + 7000);	
		//assassination for little mobs
		if (IS_NPC(victim) && victim->hit < (ch->max_move / 2) && assassinationchance >= 8)
		{
		REMOVE_BIT(ch->act, PLR_WIZINVIS);
		act("#R$n #0steps out of the shadows, and in one quick move, #Rslices #r$N's #Rhead #0from $S shoulders!#n\n\r", ch, NULL, victim, TO_NOTVICT);
		act("#0You leap from the shadows, and without breaking a sweat, #Rsever #r$N's #Rhead #0from $S shoulders!  #r<<#RAssassination!#r>>#n",ch, NULL, victim, TO_CHAR);
		ch->move -= randomizer;
		make_part( victim, "head" );
		ch->exp += 10000 * victim->level;
		raw_kill(victim, ch);
		xprintf(buf,"#r%s #0wipes the #Rblood #0from his #7dagger #0after making another skillfull #Rassa#rssina#Rtion#n.",ch->pcdata->switchname);
		do_info(ch,buf);
		return;
		}

		REMOVE_BIT(ch->act, PLR_WIZINVIS);
		act("#R$n#0 steps out from the shadows behind #r$N #0and drives a #7dagger #0into $s back!#n\n\r", ch, NULL, victim, TO_NOTVICT);
	    xprintf( buf, "#0You step out of the shadows behind #R$N #0and drive a #7dagger #0into $S back!#n   #r[#R%d#r]#n\n\r#n", dam);
		act(buf, ch, NULL, victim, TO_CHAR);
		xprintf( buf, "#0You scream in #Ragony #0as you feel a sharp #7dagger #0plunge into your back!   #r[#R%d#r]#n\n\r", dam);
		stc(buf, victim);
		ch->move -= 10000;
		hurt_person(ch,victim,dam);
		if (victim->hit < -10) victim->hit = -10;
		WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
		multi_hit( ch, victim, TYPE_UNDEFINED );
		return;
	}

void do_tdagger( CHAR_DATA *ch, char *argument)
	{
		CHAR_DATA *victim;
		OBJ_DATA *obj;
		char arg1[MAX_INPUT_LENGTH];
		char buf[MAX_STRING_LENGTH];
		int dam;
		int chance;
		int vischance;

		argument = one_argument( argument, arg1 );

		if (IS_NPC(ch))
		return;

		if (!IS_CLASS(ch, CLASS_THIEF) || !IS_SET(ch->pcdata->powers[1],THIEF_TDAGGER))
		{
		stc("Huh?\n\r",ch);
		return;
		}
		
		if ( arg1[0] == '\0')
	    {
		send_to_char( "#0Throw a dagger at whom?#n\n\r", ch );
		return;
		}

    if ( ( victim = get_char_room( ch, NULL, arg1 ) ) == NULL )
    {
	send_to_char( "#0They aren't here.#n\n\r", ch );
	return;
    }
		
		if ((victim == ch))
		{
		stc("#0You toss your dagger in the air playfully.#n\n\r",ch);
		return;
		}
		if (is_safe(ch, victim))
		{
		stc("No.\n\r",ch);
		return;
		}
		if ((( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11)
		&& ((obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || obj->value[3] != 11))
		{
		send_to_char( "#0You need to wield a piercing weapon.#n\n\r", ch );
		return;
		}

		if (ch->move < 1000)
		{
		stc("#0You do not have the required #R1000 #rm#Rove#n.\n\r",ch);
		return;
		}
		
		vischance = number_range(1, 100);
		chance = number_range(1, 100);

		if (chance >= 95) //Head Shot
		{
			if (( obj = get_eq_char( victim, ITEM_WEAR_HEAD)) == NULL)
			{
			dam = number_range(ch->damcap[DAM_CAP] * 2.5, ch->damcap[DAM_CAP] * 3);
			}
			else
			{
			dam = number_range(ch->damcap[DAM_CAP] * 1.9, ch->damcap[DAM_CAP] * 2.3);
			}
			act("#R$n#0 hurls a #7dagger #0at #r$N, #0hitting them right in the forehead!#n", ch, NULL, victim, TO_NOTVICT);
			xprintf( buf, "#0You hurl a #7dagger #0at #R$N, #0hitting $M right in #S forehead!#n   #r[#R%d#r]#n", dam);
			act(buf, ch, NULL, victim, TO_CHAR);
			xprintf( buf, "#0A #7dagger #0flies out of nowhere and hits you in the forehead!  #RSHIT that fucking hurt!#n  #r[#R%d#r]#n", dam);
			stc(buf, victim);
		}

		else if (chance >= 90) //Neck Hit
		{
			if (( obj = get_eq_char( victim, ITEM_WEAR_NECK)) == NULL)
			{
			dam = number_range(ch->damcap[DAM_CAP] * 2, ch->damcap[DAM_CAP] * 2.5);
			}
			else
			{
			dam = number_range(ch->damcap[DAM_CAP] * 1.5, ch->damcap[DAM_CAP] * 2.3);
			}
			act("#R$n#0 hurls a #7dagger #0at #r$N, #Rpenetrating #0$S neck!#n", ch, NULL, victim, TO_NOTVICT);
			xprintf( buf, "#0You hurl a #7dagger #0at #R$N, penetrating #0$S neck!#n   #r[#R%d#r]#n", dam);
			act(buf, ch, NULL, victim, TO_CHAR);
			xprintf( buf, "#0A #7dagger #0flies out of nowhere and #Rimpales #0your neck!  #RSHIT that fucking hurt!#n  #r[#R%d#r]#n\n\r", dam);
			stc(buf, victim);
			SET_BIT(victim->loc_hp[6], BLEEDING_THROAT);
		}
		
		else if (chance >= 30) //General Chest Hit
		{
			if (( obj = get_eq_char( victim, ITEM_WEAR_BODY)) == NULL)
			{
			dam = number_range(ch->damcap[DAM_CAP] * 1.8, ch->damcap[DAM_CAP] * 2);
			}
			else
			{
			dam = number_range(ch->damcap[DAM_CAP] - 200, ch->damcap[DAM_CAP] + 200);
			}
			act("#R$n#0 hurls a #7dagger #0at #r$N, #Rpiercing #0into #0$S chest!#n", ch, NULL, victim, TO_NOTVICT);
			xprintf( buf, "#0You hurl a #7dagger #0at #R$N, piercing #0into $S chest!#n   #r[#R%d#r]#n", dam);
			act(buf, ch, NULL, victim, TO_CHAR);
			xprintf( buf, "#0A #7dagger #0flies out of nowhere and #Rpierces #0deep into your chest!#n  #r[#R%d#r]#n\n\r#n", dam);
			stc(buf, victim);
		}
		
		else if (chance >= 2) //Leg Hit
		{
			if ((obj = get_eq_char( victim, ITEM_WEAR_LEGS)) == NULL)
			{
			dam = number_range(ch->damcap[DAM_CAP] * 0.9, ch->damcap[DAM_CAP] * 1.2);
			}
			else
			{
			dam = number_range(ch->damcap[DAM_CAP] - 200, ch->damcap[DAM_CAP] - 400);
			}
			act("#R$n#0 hurls a #7dagger #0at #r$N, #Rdigging #0into #0$S leg!#n", ch, NULL, victim, TO_NOTVICT);
			xprintf( buf, "#0You hurl a #7dagger #0at #R$N, digging #0into $S leg!#n   #r[#R%d#r]#n", dam);
			act(buf, ch, NULL, victim, TO_CHAR);
			xprintf( buf, "#0A #7dagger #0flies out of nowhere and #Rdigs #0deep into your leg!#n  #r[#R%d#r]#n\n\r#n", dam);
			stc(buf, victim);
		}

		else if (chance < 2  && victim->sex == 1) //OW thats just NOT right, what the hell do they teach you thieves anyhow??
		{
			dam = number_range(ch->damcap[DAM_CAP] * 2, ch->damcap[DAM_CAP] * 3);
			
act("#R$n#0 hurls a #7dagger #0at #r$N, #0hitting him in a rather.. uncompromising location!#n", ch, NULL, victim, TO_NOTVICT);
xprintf( buf, "#0You hurl a #7dagger #0at #R$N, #0hitting him in a rather.. uncompromising location!#n   #r[#R%d#r]#n", dam);
act(buf, ch, NULL, victim, TO_CHAR);
xprintf( buf, "#RYou whimper pitifully as you take a throwing dagger to the crotch!#n  #r[#R%d#r]#n\n\r#n", dam);
stc(buf, victim);
xprintf(buf,"#0HAHA #R%s #0has just taken a throwing dagger to the crotch!#n",IS_NPC(victim) ? victim->short_descr : victim->pcdata->switchname);
do_info(victim,buf);
		}

		else
		{
			stc("You failed!\n\r",ch);
			return;
		}

		hurt_person(ch,victim,dam);
		ch->move -= 1000;
		if (victim->hit < -10) {victim->hit = -10;}		
		if ( IS_SET(ch->act, PLR_WIZINVIS) && vischance >= 65) //The check for coming out of hiding.
		{
		stc("#0You take a moment to catch your balance and remain unnoticed in the shadows!#n\n\r",ch);
		WAIT_STATE( ch, 8 );
		return;
		}
		if ( IS_SET(ch->act, PLR_WIZINVIS) && vischance < 65)
		{
		stc("#0You come out of hiding!#n\n\r",ch);
		REMOVE_BIT(ch->act, PLR_WIZINVIS);
		}
		WAIT_STATE( ch, 5);
		if (ch->fighting == NULL) {ch->fighting = victim;}
		return;
	}

void do_dtrick(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;	
	OBJ_DATA *obj;
	char arg1[MAX_INPUT_LENGTH];
	char buf [MAX_STRING_LENGTH];	
	int randnum;
	int successchance;

	argument = one_argument( argument, arg1 );
	
	if ( IS_NPC(ch) )
	return;

	if (!IS_CLASS(ch, CLASS_THIEF) || !IS_SET(ch->pcdata->powers[1],THIEF_DTRICK))
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (ch->move < 2500)
	{
	stc("#0You don't have the required amount of move to preform a Dirty Trick!#n\n\r",ch);
	return;
	}

	if ( arg1[0] == '\0')
	{
	stc( "#0Preform a Dirty Trick on whom?#n\n\r", ch );
	return;
	}

	if ( ( victim = get_char_room( ch, NULL, arg1 ) ) == NULL )
    {
	send_to_char( "#0They aren't here.#n\n\r", ch );
	return;
    }


	randnum = number_range(1,4); //Disarm, Trip, Foot Stomp, Hilt Smash.

   if (randnum == 1) //<----Disarm
{
	if(((obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
  {
    if(((obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
      stc("They're not wielding anything to disarm!!#n\n\r",ch);
	  return;
  }
	
	if (IS_IMMUNE(victim, IMM_DISARM))
    {
	successchance = number_range(1, 75);
	}

	else
	{
	successchance = number_range(1, 100);
	}
	
	if (successchance < 50)
	{
	stc("#0You try to preform a dirty trick, but fail!#n\n\r",ch);
	ch->move -= 500;
	WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
	return;
	}

	if (successchance >= 50)
	{
	xprintf(buf,"#R$n #0slices $s #7dagger #0sneakily accross your wrist, causing you too lose your grip!#n\n\r");
	act( buf, ch, NULL, victim, TO_VICT    );
	xprintf(buf,"#0You sneakily slice your dagger across #R$N's #0wrist, causing them to lose their grip!#n\n\r");
	act( buf,  ch, NULL, victim, TO_CHAR    );
	
	obj_from_char( obj );
	obj_to_room(obj, victim->in_room);
	ch->move -= 2500;
	hurt_person(ch,victim,250);
	WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
	return;
	}
	}

	else if (randnum == 2)//<----Trip
	{
	if (IS_AFFECTED(victim,AFF_FLYING)) return;
	
	successchance = number_range(1, 100);

	if (successchance < 50)
	{
	stc("#0You attempt a dirty trick but fail!#n\n\r",ch);
	ch->move -= 500;
	WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
	return;
	}
	if (successchance >= 50 && victim->wait == 0)
	{
    xprintf(buf,"#R$n #0kicks your shins, causing your legs to buckle!#n\n\r");
    act( buf, ch, NULL, victim, TO_VICT    );
    xprintf(buf,"#0You kick #R$N #0in the shins, causing $S legs to buckle!#n\n\r");
    act( buf,  ch, NULL, victim, TO_CHAR    );
    xprintf(buf,"#R$n #0kicks #R$N #0in the shins, causing $S legs to buckle!#n\n\r");
    act( buf,  ch, NULL, victim, TO_NOTVICT );
    ch->move -= 2500;
	WAIT_STATE( ch, 1 * PULSE_VIOLENCE );	
	hurt_person(ch,victim,500);
	WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
    victim->position = POS_RESTING;
	return;
  }
  }

  else if (randnum == 3) //<---Foot Stomp
  {
  if (IS_AFFECTED(victim,AFF_FLYING)) return;
  
  successchance = number_range(1, 100);

  if (successchance < 50)
  {
  stc("#0You attempt to preform a Dirty Trick but fail!#n\n\r",ch);
  ch->move -= 500;
  WAIT_STATE(ch, 1 * PULSE_VIOLENCE );
  return;
  }

  if (successchance >= 50)
  {
    xprintf(buf,"#R$n #0stomps on your feet! OW!#n\n\r");
    act( buf, ch, NULL, victim, TO_VICT    );
    xprintf(buf,"#0You stomp on #R$N's #0feet!#n\n\r");
    act( buf,  ch, NULL, victim, TO_CHAR    );
    xprintf(buf,"#R$n #0stomps on #R$N's #0feet!#n\n\r");
    act( buf,  ch, NULL, victim, TO_NOTVICT );
    ch->move -= 2500;
	WAIT_STATE( ch, 1 * PULSE_VIOLENCE );	
	hurt_person(ch,victim,500);
	WAIT_STATE( victim, 3 * PULSE_VIOLENCE );
	return;
  }
  }

  else if (randnum == 4) //<---Hilt Bash
  {
  successchance = number_range(1,100);
  {
  if (successchance < 50)
  {
  stc("#0You attempt a Dirty Trick, but fail!#n\n\r",ch);
  ch->move -= 500;
  WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
  return;
  }
  if (successchance >= 50)
  {
    xprintf(buf,"#R$n BASHES #0his jeweled dagger hilt into your face! You reel back, stunned!#n\n\r");
    act( buf, ch, NULL, victim, TO_VICT    );
    xprintf(buf,"#0You BASH your dagger hilt into #R$N's #0face!#n\n\r");
    act( buf,  ch, NULL, victim, TO_CHAR    );
    xprintf(buf,"#R$n #RBASHES #0his dagger hilt into #R$N's #0face, causing them to reel back stunned!#n\n\r");
    act( buf,  ch, NULL, victim, TO_NOTVICT );
    act("Your nose shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
    act("$n's nose shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
	ch->move -= 2500;
	hurt_person(ch,victim,1000);
	WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
	WAIT_STATE( victim, 1 * PULSE_VIOLENCE );
	victim->position = POS_STUNNED;
	return;
  }
  return;
  }
  }
  }

void do_poisonblade(CHAR_DATA *ch, char *argument)
{

  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg );

	 if (IS_NPC(ch)) return;
	 if (!IS_CLASS(ch,CLASS_THIEF))
	 {
	 stc("Huh?\n\r",ch);
	 return;
	 }

	if (!IS_SET(ch->pcdata->powers[1],THIEF_POISONBLADE))
	 {
	 stc("Huh?\n\r",ch);
	 return;
	 }

    if ( arg[0] == '\0' )
    {
    stc( "Poison What?\n\r", ch );
	return;
    }

  if ((obj = get_obj_carry(ch, arg,ch)) == NULL)
  {
    send_to_char("You dont have that item.\n\r",ch);
    return;
  }  
  
  if (obj->item_type != ITEM_WEAPON)
  {
    send_to_char("You can only poison weapons!\n\r", ch);
    return;
  }

  if (IS_SET(obj->quest, QUEST_WWPOISON))
  {
    send_to_char("#0This item has already been #gP#Goisone#gd!#n.\n\r", ch);
    return;
  }

  if (IS_SET(obj->extra_flags, ITEM_FIREENCHANT) || IS_SET(obj->extra_flags, ITEM_ICEENCHANT) || IS_SET(obj->extra_flags, ITEM_ELECENCHANT))
  {
  stc("That weapon has already been enchanted!\n\r",ch);
  return;
  }

  SET_BIT(obj->quest, QUEST_WWPOISON);
  stc("#0You coat your weapon in foul smelling #gp#Goiso#gn!#n\n\r",ch);
  obj->timer = 35;
  return;
 }

void do_shadowmove( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
    
  if ( IS_NPC(ch) )
    return;

  argument = one_argument (argument, arg);

  if (!IS_CLASS(ch, CLASS_THIEF) )
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    send_to_char( "Stalk whom?\n\r", ch );
    return;
  }

  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "You cannot find them.\n\r", ch );
    return;
  }
    
  if( ch->move < 500) 
  {
    stc( "You don't have enough movement points to stalk them.\n\r",ch);
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

  act("You pick up the trail of $N, and quickly find them.", ch, NULL, victim, TO_CHAR);
  act("$n slides into the shadows, stalking someone.", ch, NULL, victim, TO_ROOM);
  char_from_room(ch);
  char_to_room(ch,victim->in_room);
  ch->move -= 500;
  act("$n walks out of nowhere behind $N.", ch, NULL, victim, TO_NOTVICT);
  act("$n walks out of nowhere from behind.", ch, NULL, victim, TO_VICT);
  do_look(ch,"scry");
  return;
}
