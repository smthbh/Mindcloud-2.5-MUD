/********************************************************************
 * This code was written from scratch by Listy (listy@godwars.com)  *
 * Use of this code is prohibited unless with written permission by *
 * Listy. This acknowledgement must not be removed from this code   *
 * (c) 1999 Listy from God Wars Rogue Edition                       *
 * Jedi, Lightsaber are trademarks of Lucasfilm                     *
 ********************************************************************/
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
#include <time.h>
#include <string.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"


void do_jediskill( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
	int maxbody;
	int maxmind;
	int maxspirit;
	int maxtech;
	int learncost;

	maxbody = 7;
	maxmind = 5;
	maxspirit = 6;
	maxtech = 10;
    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch))
		return;
    else if (!IS_CLASS(ch, CLASS_JEDI))
    {
		send_to_char("Huh?\n\r",ch);
		return;
    }
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    else if (arg1[0] == '\0')
    {
		xprintf(buf,"#CB#cody#0 (#7%d#0/#77#0)#7,#C M#cind#0 (#7%d#0/#75#0)#7,#C S#cpirit#0 (#7%d#0/#76#0)#7,#C T#cechnique#0 (#7%d#0/#710#0)#n\n\r",
		ch->pcdata->powers[JPOWER_BODY], ch->pcdata->powers[JPOWER_MIND],
		ch->pcdata->powers[JPOWER_SPIRIT], ch->pcdata->powers[JPOWER_TECHNIQUE]);
		send_to_char(buf,ch);
		xprintf(buf, "#CYou have achieved#R %d#C jedi states, and are currently at#R %d#C.#n\n\r", ch->pcdata->powers[JEDI_STATE_MAX],ch->pcdata->powers[JEDI_STATE]);
		send_to_char(buf, ch);
		xprintf(buf2, "#CYou have achieved#R %d#C jedi points, and are currently at#R %d#C.#n\n\r", ch->pcdata->powers[JEDI_FOCUS_MAX],ch->pcdata->powers[JEDI_FOCUS]);
		send_to_char(buf2, ch);
		if ( IS_IMMUNE(ch,IMM_SHIELDED) || IS_SET(ch->special, SPC_JEDI_FSHIELD) || IS_ITEMAFF(ch, ITEMA_SPEED) )
		{
			send_to_char("#1Active Body Powers.#n\n\r", ch);
			if (IS_IMMUNE(ch,IMM_SHIELDED))
				send_to_char("#1You are shielding your aura from those around you.#n\n\r", ch);
			if (IS_SET(ch->special, SPC_JEDI_FSHIELD))
				send_to_char("#1You are surrounded in a shield of the force.#n\n\r", ch);
			if (IS_ITEMAFF(ch, ITEMA_SPEED))
				send_to_char("#1You are in a state of enhanced speed.#n\n\r", ch);
		}
		if ( IS_SET(ch->act, PLR_HOLYLIGHT) || (IS_SET(ch->special, SPC_JEDI_LEVITATE))) 
		{
			send_to_char("#2Active Mind Powers.#n\n\r", ch);
			if (IS_SET(ch->special, SPC_JEDI_LEVITATE))
				send_to_char("#2You are levitating in the force.#n\n\r", ch);
			if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
				send_to_char("#2Your vision has been enhanced.#n\n\r", ch);
		}
		return;
    }
	else if (!str_cmp(arg1,"body"))
	{
	    send_to_char("Body - Mastering the Force channelling through your body.\n\r",ch);
		if (ch->pcdata->powers[JPOWER_BODY] > 0)
			send_to_char("1  - Jedi Meditation (jedisleep) An enhanced form of sleep.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_BODY] > 1)
			send_to_char("2  - Shield (jedishield) Prevents people from reading your aura.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_BODY] > 2)
			send_to_char("3  - Enhanced Dodge (Auto).\n\r", ch);
		if (ch->pcdata->powers[JPOWER_BODY] > 3)
			send_to_char("4  - Enhanced Parry (Auto).\n\r", ch);
		if (ch->pcdata->powers[JPOWER_BODY] > 4)
			send_to_char("5  - Shield of the Force (fshield) Using the force to protect your mortal body.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_BODY] > 5)
			send_to_char("6  - Enhanced Speed (fspeed) Using the force to speed up your actions.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_BODY] > 6)
			send_to_char("7  - Jedi Touch (jtouch) Stunning an opponent with the force blow.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_BODY] >= maxbody)
			send_to_char("You have mastered all the skills of the body.\n\r",ch);
		else
		{
			learncost = ((ch->pcdata->powers[JPOWER_BODY] + 1) * 10000000);
			xprintf(buf,"Improvement of body will cost : %d", learncost);
			send_to_char(buf, ch);
		}
		return;
	}
	else if (!str_cmp(arg1,"mind"))
	{
	    send_to_char("Mind - Using the Force to control other things\n\r",ch);
		if (ch->pcdata->powers[JPOWER_MIND] > 0)
			send_to_char("1  - Levitate (jlevitate) Using the force to lift you off the ground.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_MIND] > 1)
			send_to_char("2  - Jedi Sight (jsight) Allows you to perceive more.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_MIND] > 2)
			send_to_char("3  - Jedi Sense (jsense) Read your opponent's statistics.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_MIND] > 3)
			send_to_char("4  - Jedi Eyes (jeyes) Look through your target's eyes.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_MIND] > 4)
			send_to_char("5  - Jedi Mental Focus (jfocus) Allows you to perceive different planes of existence.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_SPIRIT] > 2 && ch->pcdata->powers[JPOWER_MIND] > 4)
			send_to_char("5  - Jedi Mind Walk (jwalk) Moves you into a different plane of existence.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_MIND] >= maxmind)
			send_to_char("You have mastered all the skills of the mind.\n\r", ch);
		else
		{
			learncost = ((ch->pcdata->powers[JPOWER_MIND] + 1) * 10000000);
			xprintf(buf,"Improvement of mind will cost : %d", learncost);
			send_to_char(buf, ch);
		}
	    return;
	}
	else if (!str_cmp(arg1,"spirit"))
	{
	    send_to_char("Spirit - Feeling the Force in all things.\n\r",ch);
		if (ch->pcdata->powers[JPOWER_SPIRIT] > 0)
			send_to_char("1  - Jedi Healing (jhealing) Uses the force to reinforce your vitality.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_SPIRIT] > 1)
			send_to_char("2  - Jedi Perception (Auto) Feels the fluctuations in the force of an outside presence.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_SPIRIT] > 2 && ch->pcdata->powers[JPOWER_MIND] > 4)
			send_to_char("3  - Jedi Mind Walk (jwalk) Moves you into a different plane of existence.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_SPIRIT] > 3)
			send_to_char("4  - Enhanced Defence (Auto).\n\r", ch);
		if (ch->pcdata->powers[JPOWER_SPIRIT] > 4)
			send_to_char("5  - Enhanced Attack (Auto).\n\r", ch);
		if (ch->pcdata->powers[JPOWER_SPIRIT] > 5)
			send_to_char("6  - Jedi Summon (jsummon) Summons the force to your aid in a battle.\n\r", ch);
		if (ch->pcdata->powers[JPOWER_SPIRIT] >= maxspirit)
			send_to_char("You have mastered all the skills of the spirit.\n\r", ch);
		else
		{
			learncost = ((ch->pcdata->powers[JPOWER_SPIRIT] + 1) * 10000000);
			xprintf(buf, "Improvement of spirit will cost : %d", learncost);
			send_to_char(buf, ch);
		}
	    return;
	}
	else if (!str_cmp(arg1,"technique"))
	{
	    send_to_char("Technique - Mastery of Force combat.\n\r",ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 0)
			send_to_char("1  - Jedi Somersault (jsault)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 1)
			send_to_char("2  - Jedi Reverse Spin (jreverse)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 2)
			send_to_char("3  - Jedi Kneel (jkneel)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 3)
			send_to_char("4  - Jedi Stab (jstab)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 4)
			send_to_char("5  - Jedi High Strike (jhigh)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 5)
			send_to_char("6  - Jedi Upper Cut (jupper)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 6)
			send_to_char("7  - Jedi Butterfly Cut (jbutterfly)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 7)
			send_to_char("8  - Jedi Cross (jcross)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 8)
			send_to_char("9  - Jedi Slash (jslash)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] > 9)
			send_to_char("10 - Jedi Circle (jcircle)\n\r", ch);
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] >= maxtech)
			send_to_char("You have mastered all the techniques of the jedi.\n\r", ch);
		else
		{
			learncost = ((ch->pcdata->powers[JPOWER_TECHNIQUE] + 1) * 5000000);
			xprintf(buf, "Improvement in technique will cost : %d", learncost);
			send_to_char(buf, ch);
		}
	    return;
	}
	else
	{
		xprintf(buf,"#RBody #0(#7%d#0)#R Mind #0(#7%d#0)#R Spirit#0 (#7%d#0)#R Technique#0 (#7%d#0)#n.\n\r",
		ch->pcdata->powers[JPOWER_BODY], ch->pcdata->powers[JPOWER_MIND], ch->pcdata->powers[JPOWER_SPIRIT], ch->pcdata->powers[JPOWER_TECHNIQUE]);
		send_to_char(buf,ch);
		xprintf(buf,"#CYou have achieved#R %d#C jedi states, and are curently at#R %d#C.#n\n\r",ch->pcdata->powers[JEDI_STATE_MAX],ch->pcdata->powers[JEDI_STATE]);
		send_to_char(buf, ch);
		xprintf(buf2, "You have achieved %d points of jedi points, and are currently at %d\n\r", ch->pcdata->powers[JEDI_FOCUS_MAX],ch->pcdata->powers[JEDI_FOCUS]);
		send_to_char(buf2, ch);
		send_to_char("Active Body Powers.\n\r", ch);
		if ( IS_IMMUNE(ch,IMM_SHIELDED) || IS_SET(ch->special, SPC_JEDI_FSHIELD) || IS_ITEMAFF(ch, ITEMA_SPEED) )
		{
			send_to_char("#1Active Body Powers.#n\n\r", ch);
			if (IS_IMMUNE(ch,IMM_SHIELDED))
				send_to_char("#1You are shielding your aura from those around you.#n\n\r", ch);
			if (IS_SET(ch->special, SPC_JEDI_FSHIELD))
				send_to_char("#1You are surrounded in a shield of the force.#n\n\r", ch);
			if (IS_ITEMAFF(ch, ITEMA_SPEED))
				send_to_char("#1You are in a state of enhanced speed.#n\n\r", ch);
		}
		if ( IS_SET(ch->act, PLR_HOLYLIGHT) || (IS_SET(ch->special, SPC_JEDI_LEVITATE))) 
		{
			send_to_char("#2Active Mind Powers.#n\n\r", ch);
			if (IS_SET(ch->special, SPC_JEDI_LEVITATE))
				send_to_char("#2You are levitating in the force.#n\n\r", ch);
			if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
				send_to_char("#2Your vision has been enhanced.#n\n\r", ch);
		}
	}
}/* End of void do_jediskill */
    
void do_sense( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI) )
    {
		send_to_char( "Huh?\n\r", ch );
		return;
    }
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    else if ( arg[0] == '\0' )
    {
		send_to_char( "Who do you wish to sense?\n\r", ch );
		return;
    }
    else if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }
	else if ( !IS_CLASS(victim, CLASS_JEDI) )
	{
		send_to_char("They are not jedi.\n\r", ch);
		return;
	}
    else if ( IS_NPC(victim) )
    {
		send_to_char( "They are not jedi.\n\r", ch );
		return;
    }
    else if ( ch == victim )
    {
		send_to_char( "Not on yourself!\n\r", ch );
		return;
    }
	else
	{
		xprintf(buf,"%s : Body(%d) Mind(%d) Spirit(%d) Technique(%d).\n\r",victim->name,
		victim->pcdata->powers[JPOWER_BODY], victim->pcdata->powers[JPOWER_MIND],
		victim->pcdata->powers[JPOWER_SPIRIT], victim->pcdata->powers[JPOWER_TECHNIQUE]);
		send_to_char(buf,ch);
	}
}/* End of void do_sense */
void do_jedilearn( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
	int learncost;
	int maxbody;
	int maxmind;
	int maxspirit;
	int maxtech;

    argument = one_argument( argument, arg1 );
	maxbody = 7;
	maxmind = 5;
	maxspirit = 6;
	maxtech = 10;

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    else if (arg1[0] == '\0')
	{
		send_to_char("#RSyntax#7:#R Jedilearn #0<#LBody#0/#CMind#0/#YSpirit#0/#GTechnique#0/#PState#0/#7Point#0>#n\n\r", ch);
		return;
	}
	else if (!str_cmp(arg1, "body"))
	{
		if (ch->pcdata->powers[JPOWER_BODY] >= maxbody)
		{
			send_to_char("You have already mastered the body.\n\r", ch);
			return;
		}
		else 
		{
			learncost = ((ch->pcdata->powers[JPOWER_BODY] + 1) 
* 2000000);
			if (ch->exp < learncost)
			{
				xprintf(buf, "Body Power : %d will cost %d", (ch->pcdata->powers[JPOWER_BODY] + 1), learncost);
				send_to_char(buf, ch);
				return;
			}
			else
			{
				ch->exp -= learncost;
				ch->pcdata->powers[JPOWER_BODY] += 1;
				send_to_char("You have advanced to the next level of the body.\n\r", ch);
				return;
			}
		}
	}
	else if (!str_cmp(arg1, "mind"))
	{
		if (ch->pcdata->powers[JPOWER_MIND] >= maxmind)
		{
			send_to_char("You have already mastered the mind.\n\r", ch);
			return;
		}
		else
		{
			learncost = ((ch->pcdata->powers[JPOWER_MIND] + 1) * 2000000);
			if (ch->exp < learncost)
			{
				xprintf(buf, "Mind Power : %d will cost %d", (ch->pcdata->powers[JPOWER_MIND] + 1), learncost);
				send_to_char(buf, ch);
				return;
			}
			else
			{
				ch->exp -= learncost;
				ch->pcdata->powers[JPOWER_MIND] += 1;
				send_to_char("You have advanced to the next level of the mind.\n\r", ch);
				return;
			}
		}
	}
	else if (!str_cmp(arg1, "spirit"))
	{
		if (ch->pcdata->powers[JPOWER_SPIRIT] >= maxspirit)
		{
			send_to_char("You have already mastered the spirit.\n\r", ch);
			return;
		}
		else
		{
			learncost = ((ch->pcdata->powers[JPOWER_SPIRIT] + 1) * 2000000);
			if (ch->exp < learncost)
			{
				xprintf(buf, "Spirit Power : %d will cost %d", (ch->pcdata->powers[JPOWER_SPIRIT] + 1), learncost);
				send_to_char(buf, ch);
				return;
			}
			else
			{
				ch->exp -= learncost;
				ch->pcdata->powers[JPOWER_SPIRIT] += 1;
				send_to_char("You have advanced to the next level of the spirit.\n\r", ch);
				return;
			}
		}
	}
	else if (!str_cmp(arg1, "technique"))
	{
		if (ch->pcdata->powers[JPOWER_TECHNIQUE] >= maxtech)
		{
			send_to_char("You have already mastered the techniques.\n\r", ch);
			return;
		}
		else
		{
			learncost = ((ch->pcdata->powers[JPOWER_TECHNIQUE] + 1) * 2000000);
			if (ch->exp < learncost)
			{
				xprintf(buf, "Technique Level : %d will cost %d", (ch->pcdata->powers[JPOWER_TECHNIQUE] + 1), learncost);
				send_to_char(buf, ch);
				return;
			}
			else
			{
				ch->exp -= learncost;
				ch->pcdata->powers[JPOWER_TECHNIQUE] += 1;
				send_to_char("You have advanced to the next technique.\n\r", ch);
				return;
			}
		}
	}
	else if (!str_cmp(arg1, "state"))
	{
		if (ch->pcdata->powers[JEDI_STATE_MAX] >= 5)
		{
			send_to_char("You have learned all the states available to the jedi.\n\r", ch);
			return;
		}
		else
		{
			learncost = ((ch->pcdata->powers[JEDI_STATE_MAX] + 1) * 2000000);
			if (ch->exp < learncost)
			{
				xprintf(buf, "State Level : %d will cost %d.\n\r", (ch->pcdata->powers[JEDI_STATE_MAX] + 1), learncost);
				send_to_char(buf, ch);
				return;
			}
			else
			{
				ch->exp -= learncost;
				ch->pcdata->powers[JEDI_STATE_MAX] += 1;
				send_to_char("You can now access the next jedi state.\n\r", ch);
				return;
			}
		}
	}
	else if (!str_cmp(arg1, "point"))
	{
		if (ch->pcdata->powers[JEDI_FOCUS_MAX] >= 100)
		{
			send_to_char("You have gained all the points possible for a jedi.\n\r", ch);
			return;
		}
		else
		{
			learncost = 1000000;
			if (ch->exp < learncost)
			{
				send_to_char("Jedi points are 1,000,000 experience each.\n\r", ch);
				return;
			}
			else
			{
				ch->exp -= learncost;
				ch->pcdata->powers[JEDI_FOCUS_MAX] += 1;
				send_to_char("You gained a jedi point.\n\r", ch);
				return;
			}
		}
	}
	send_to_char("#RSyntax#7:#R Jedilearn #0<#cBody#0/#cMind#0/#cSpirit#0/#cTechnique#0/#cState#0/#cPoint#0>#n\n\r", ch);
	return;
}/* End of void do_learn */
void do_jstate(CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JEDI_STATE] == ch->pcdata->powers[JEDI_STATE_MAX])
	{
		send_to_char("You are already at your maximum state.\n\r", ch);
		return;
	}
	else if (ch->move < ch->max_move / 2)
	{
		send_to_char("You are not vital enough to achieve jedi state.\n\r", ch);
		return;
	}
	else
	{
		send_to_char( "You concentrate upon your state.\n\r", ch );
		act( "$n concentrates deeply and is encompassed by a glow.", ch, NULL, NULL, TO_ROOM );
		ch->pcdata->powers[JEDI_STATE] += 1;
	}
	return;
}/* End of void do_jstate */
void do_jediarmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch))
		return;
    if (!IS_CLASS(ch, CLASS_JEDI))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

    if (arg[0] == '\0')
    {
		send_to_char("#RPlease specify what kind of equipment you want to make#7:#n\n\r", ch );
		send_to_char("#RLightsaber#0,#R Belt#0,#R Robes#0,#R Cloak#0.#n\n\r", ch );
		return;
    }
    if (!str_cmp(arg,"lightsaber"))
		vnum = 33416;
    else if (!str_cmp(arg,"belt"))
		vnum = 33420;
    else if (!str_cmp(arg,"robes"))
		vnum = 33419;
    else if (!str_cmp(arg,"cloak"))
		vnum = 33421;
    else
    {
		send_to_char("That is an invalid type.\n\r", ch );
		return;
    }
    if ( ch->practice < 140)
    {
		send_to_char("It costs 140 points of primal to make a piece of jedi equipment.\n\r",ch);
		return;
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
		send_to_char("Missing object, please inform an Implementor.\n\r",ch);
		return;
    }
    ch->practice -= 140;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

/* Jedi Body Powers */
void do_jedisleep( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch))
		return;
    else if (!IS_CLASS(ch, CLASS_JEDI))
    {
		send_to_char("Huh?\n\r",ch);
		return;
    }
	else if (ch->pcdata->powers[JPOWER_BODY] < 1)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else
	{
	    switch ( ch->position )
		{
			case POS_SLEEPING:
				send_to_char( "You close your eyes and fall into a jedi sleep.\n\r", ch );
				act( "$n closes $s eyes and falls into a jedi sleep.", ch, NULL, NULL, TO_ROOM );
				ch->position = POS_MEDITATING;
			break;
			case POS_RESTING:
				send_to_char( "You close your eyes and fall into a jedi sleep.\n\r", ch );
				act( "$n closes $s eyes and falls into a jedi sleep.", ch, NULL, NULL, TO_ROOM );
				ch->position = POS_MEDITATING;
			break;
			case POS_MEDITATING:
				send_to_char( "You are already in a state of jedi sleep.\n\r", ch );
			break;
			case POS_SITTING:
				send_to_char( "You close your eyes and fall into a jedi sleep.\n\r", ch );
				act( "$n closes $s eyes and falls into a jedi sleep.", ch, NULL, NULL, TO_ROOM );
				ch->position = POS_MEDITATING;
			break;
			case POS_STANDING:
				send_to_char( "You close your eyes and fall into a jedi sleep.\n\r", ch );
				act( "$n closes $s eyes and falls into a jedi sleep.", ch, NULL, NULL, TO_ROOM );
				ch->position = POS_MEDITATING;
			break;
			case POS_FIGHTING:
				send_to_char( "You are fighting!\n\r", ch );
			break;
		}
	}
    return;
}/*End of void do_jedisleep*/
void do_jedishield( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    
	argument = one_argument( argument, arg );

    if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
	}
	else if (ch->pcdata->powers[JPOWER_BODY] < 2)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    else if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
    	send_to_char("You use the force to shield your aura from those who would invade it.\n\r",ch);
    	SET_BIT(ch->immune, IMM_SHIELDED);
		return;
    }
	else
	{
	    send_to_char("You let your shield drop.\n\r",ch);
		REMOVE_BIT(ch->immune, IMM_SHIELDED);
		return;
	}
}/* End of void do_jedishield */
void do_fshield( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_BODY] < 5)
	{
		send_to_char("You need at least Jedi Body power of 5\n\r", ch);
		return;
	}
	else if (IS_SET(ch->special, SPC_JEDI_FSHIELD))
	{
		act("You remove yourself from the protection of the force",ch,NULL,NULL,TO_CHAR);
		act("$n removes themself from the force",ch,NULL,NULL,TO_NOTVICT);
		REMOVE_BIT(ch->special,SPC_JEDI_FSHIELD);
		ch->armor += 1000;
		return;
	}
	else
	{
		act("You surround yourself in the force",ch,NULL,NULL,TO_CHAR);
		act("$n surrounds themself in the force",ch,NULL,NULL,TO_NOTVICT);
		SET_BIT(ch->special, SPC_JEDI_FSHIELD);
		ch->armor -= 1000;
		return;
	}
}
/* End of do_fshield */
void do_fspeed(CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_BODY] < 6)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->move < (ch->max_move / 2))
	{
		send_to_char("You are not vital enough to use this power.\n\r", ch);
		return;
	}
	else if (IS_ITEMAFF(ch, ITEMA_SPEED))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_SPEED);
	    send_to_char( "Your actions slow down to normal speed as you release yourself from the force.\n\r", ch );
	    act("$n releases themself from the force.",ch,NULL,NULL,TO_ROOM);
	}
	else
	{
		SET_BIT(ch->itemaffect, ITEMA_SPEED);
	    send_to_char( "You let the speed of the force envelope you.\n\r", ch );
	    act("$n concentrates and starts to move faster.",ch,NULL,NULL,TO_ROOM);
	}
}/* End of void do_fspeed*/
void do_jtouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];
    char arg [MAX_INPUT_LENGTH];
	int touchdamage;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) 
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->level < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_BODY] < 7)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->move < 2000)
	{
		send_to_char("You do not have the vitality to do this\n\r", ch);
		return;
	}
    else if ( arg[0] == '\0' )
    {
		send_to_char( "Who do you wish to attack with the force?\n\r", ch );
		return;
    }
    else if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }
	else if (victim->level < 3)
	{
		send_to_char("You cannot do that to them.\n\r", ch);
		return;
	}
	else if ( victim->position == POS_FIGHTING)
	{
		send_to_char("They are already in a fight.\n\r", ch);
		return;
	}
	else if ( is_safe( ch, victim ) )
	{
		send_to_char("You cannot seem to do it.\n\r", ch);
		return;
	}
	else if (victim->hit < (victim->max_hit) )
	{
		send_to_char("They are hurt.\n\r", ch);
		return;
	}
	else
	{
		ch->move -= 500;
		if (IS_ITEMAFF(ch, ITEMA_SPEED))
			touchdamage = number_range(100,(victim->max_hit *.13));
		else
			touchdamage = number_range(0,(victim->max_hit * 0.1));
		if (touchdamage == 0)
		{
			xprintf(buf,"You lash out at $N blindly with the force [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n lashes out blindly with the force at you[%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n lashes out with the force at $N [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			WAIT_STATE(ch, 10);
		}
		else if ((touchdamage > 0) && (touchdamage < 101))
		{
			xprintf(buf,"You lash out at with the force, just scathing $N [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n lashes out with the force and scathes you [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n lashes out with the force at $N [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, touchdamage);
//			victim->hit -= touchdamage;
			WAIT_STATE(ch, 10);
		}
		else if ((touchdamage > 100) && (touchdamage < 751))
		{
			xprintf(buf,"You lash out viciously at $N with the force [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n lashes out vicously with the force at you, throwing you back [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n lashes out with the force at $N knocking them over [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, touchdamage);
//			victim->hit -= touchdamage;
			victim->position = POS_STUNNED;
			WAIT_STATE(ch, 10);
		}
		else if ((touchdamage > 750) && (touchdamage < 1501))
		{
			xprintf(buf,"You channel the force violently at $N [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"You feel yourself thrown backwards as a force wave strikes you from $n [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n lashes out with the force at $N knocking them over [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, touchdamage);
//			victim->hit -= touchdamage;
			victim->position = POS_STUNNED;
			WAIT_STATE(ch, 10);
		}
		else
		{
			xprintf(buf,"You concentrate and create a force wave engulfing $N [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"You are thrown off your feet as a wave of energy is hurled at you from $n [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n lashes out with the force at $N knocking them over [%d]\n\r",touchdamage);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, touchdamage);
//			victim->hit -= touchdamage;
			victim->position = POS_STUNNED;
			WAIT_STATE(ch, 10);
		}
	}
}
/* End of Jedi Body Powers */
/* Jedi Mind Powers */
void do_jlevitate(CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_MIND] < 1)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    else if (IS_SET(ch->special, SPC_JEDI_LEVITATE))
    {
		REMOVE_BIT(ch->special, SPC_JEDI_LEVITATE);
		send_to_char( "You let the force move you to the ground.\n\r", ch );
		return;
    }
    else
    {
		SET_BIT(ch->special, SPC_JEDI_LEVITATE);
		send_to_char( "You concentrate on the force to lift you from the floor.\n\r", ch );
		return;
    }
    return;

}/* End of void do_levitate*/
void do_jsight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_MIND] < 2)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    else if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
		REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
		send_to_char( "You release your jedi vision.\n\r", ch );
		return;
    }
    else
    {
		SET_BIT(ch->act, PLR_HOLYLIGHT);
                send_to_char( "Your jedi senses enhance your vision.\n\r", ch );
		
		return;
    }
    return;
}/* End of void do_jsight */
void do_jsense( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_MIND] < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    else if ( arg[0] == '\0' )
    {
		send_to_char( "Sense the aura on what?\n\r", ch );
		return;
    }
    else if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
		{
			send_to_char( "Sense the aura on what?\n\r", ch );
			return;
		}
		act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
		spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
		return;
	}
	else if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
		(IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD)))
		&& !IS_ITEMAFF(ch, ITEMA_VISION))
	{
		send_to_char("You are unable to read their aura.\n\r",ch);
		return;
	}
	act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you intently.",ch,NULL,victim,TO_VICT);
	if (IS_NPC(victim))
		xprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
	else
		xprintf(buf, "%s's Stats : ", victim->pcdata->switchname);
    send_to_char(buf,ch);
    xprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
    return;
}/* End of void do_jsense */
void do_jeyes( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_MIND] < 4)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    if ( arg[0] == '\0' )
    {
		send_to_char( "Whose eyes do you wish to look through?\n\r", ch );
		return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }
	if (IS_NPC(victim))
	{
		send_to_char("You cannot scry a mobile .\n\r", ch);
		return;
	}
    chroom = ch->in_room;
    victimroom = victim->in_room;

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
    ch->fight_timer = 10;
	if (IS_CLASS(victim, CLASS_MONK) && victim->pcdata->powers[PMONK] > 1 && victim->level > 2)
	{
		send_to_char("You feel your soul being invaded by an outside force.\n\r", victim);
		return;
	}

	if ((victim->pcdata->powers[JPOWER_SPIRIT] < 2) && IS_CLASS(victim, CLASS_JEDI) && victim->level > 2)
		send_to_char("You sense the force moving as you are being watched.\n\r", victim);
    return;
}/* End of void do_jeyes */
void do_jfocus( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_MIND] < 5)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    else if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
    {
		send_to_char("You can no longer see between planes.\n\r",ch);
		REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
		return;
    }
    send_to_char("You can now see between planes.\n\r",ch);
    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    return;
}/* End of void do_jfocus */
/* End of Jedi Mind Powers */
/* Jedi Spirit Powers */
void do_jhealing(CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI) ) 
    {
		send_to_char("Huh?\n\r", ch );
		return;
    }
	else if (ch->pcdata->powers[JPOWER_SPIRIT] < 1)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->move < 100)
	{
		send_to_char("You don't have enough move.\n\r", ch );
		return;
	}
	else if (ch->position == POS_FIGHTING)
	{
		send_to_char("You cannot do this whilst fighting.\n\r", ch);
		return;
	}
	else if (ch->fight_timer > 0)
	{
		send_to_char("You cannot concentrate enough after a fight.\n\r", ch);
		return;
	}
	ch->move = ch->move - 4000;
	ch->hit = ch->hit + 4000;
	if (ch->hit > ch->max_hit)
		ch->hit = ch->max_hit;
	send_to_char("The Force encompasses you.\n\r",ch);
	act("$n is surrounded in the Force.",ch,NULL,NULL,TO_ROOM);
	WAIT_STATE(ch, 5);
	return;
}/* End of void do_jhealing */
void do_jwalk(CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_SPIRIT] < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_MIND] < 5)
	{
		send_to_char("This power must be used in conjuntion with Jedi Mind Power Level 5\n\r", ch);
		return;
	}
	else if (!IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
	{
		send_to_char("You must have jedi focus active to use this power\n\r", ch);
		return;
	}
	else if (has_timer(ch))
		return;
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
    {
		send_to_char("You fade into the plane of jedi thought.\n\r",ch);
		act("The force flicker and swallow up $n.",ch,NULL,NULL,TO_ROOM);
		SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
		do_look(ch,"auto");
		return;
    }
    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    send_to_char("You fade back into the real world.\n\r",ch);
	act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM);
	do_look(ch,"auto");
	return;
}/* End of void do_jwalk */
void do_jfight(CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->move < 2000)
	{
		send_to_char("You do not have the vitality to do this.\n\r", ch);
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
	else if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
	{
		send_to_char("You are already in a state of jedi fighting.\n\r", ch);
		return;
	}
	else
	{
		SET_BIT(ch->special, SPC_JEDI_QUICKEN);
		act("Your actions become guided by the force.",ch,NULL,NULL,TO_CHAR);
		act("$n's become guided in the force.",ch,NULL,NULL,TO_NOTVICT);
		ch->move -= 2000;
		return;
	}
}
void do_jsummon(CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];	
	int dam;
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];	
	argument = one_argument(argument, arg1);

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_SPIRIT] < 6)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if (ch->move < 2000)
	{
		send_to_char("You do not have the vitality to do this.\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JEDI_FOCUS] < 50)
	{
		send_to_char("You do not have the 50 jedi points necessary to do this\n\r", ch);
		return;
	}
	else 
	{
		ch->move -= 500;
		ch->pcdata->powers[JEDI_FOCUS] -= 50;
		if (ch->generation == 6)
			dam = number_range(0, (victim->max_hit * 0.05));
		else if (ch->generation == 5)
			dam = number_range(0, (victim->max_hit * 0.07));
		else if (ch->generation == 4)
			dam = number_range(0, (victim->max_hit * 0.10));
		else if (ch->generation == 3)
			dam = number_range(0, (victim->max_hit * 0.13));
		else if (ch->generation == 2)
			dam = number_range(0, (victim->max_hit * 0.16));
		else if (ch->generation == 1)
			dam = number_range(0, (victim->max_hit * 0.20));
		else if (ch->generation == 0)
			dam = number_range(0, (victim->max_hit * 0.25));
		else
			dam = number_range(0, (victim->max_hit * 0.03));

		
		send_to_char("You calm your thoughts and call upon the force to guide your blows\n\r", ch);
		xprintf(buf,"$n closes their eyes, a look of concentration surrounds them");
		act(buf,ch,NULL,NULL,TO_NOTVICT);
		
		if (dam == 0)
		{
			xprintf(buf,"Your blow against $N miss entirely[%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"You watch as $n flails wildly with the force [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n flails wildly with the force [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else
		{
			xprintf(buf,"Your summon all the force in a single blow against $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"You feel the force from $n [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n unleashes a single blow at $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		hurt_person(ch, victim, dam);
//		victim->hit -= dam;
		update_pos(victim);
		WAIT_STATE(ch, 10);
	} } /* End of Jedi Spirit Powers */ /* Jedi Technique Powers */
void do_jsault( CHAR_DATA *ch, char *argument )  {
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 1)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch,6);
		}
		else
		{
			WAIT_STATE(ch,9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 0)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 1;
			dam = 890 + number_range(50, 350);
			act("A intensity of the Force grows around you(1-1).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 14)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 15;
			dam = 850 + number_range(75, 550);
			act("A intensity of the Force grows around you(5-4).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 24)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			dam = 2490 + number_range(500, 1500);
			act("A intensity of the Force explodes around you(6-F).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force explodes around $n.",ch,NULL,NULL,TO_NOTVICT);
			act("You launch yourself into the air.",ch,NULL,NULL,TO_CHAR);
			act("$n leaps high into the air.",ch,NULL,NULL,TO_NOTVICT);
			xprintf(buf,"You somersault over $N's scathing their back with your sabre [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"You feel a burning as $n cuts into you with their lightsaber [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n cuts a wound into $N's back as they somersault over them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, dam);
//			victim->hit -= dam;
			return;
			
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 830 + number_range(50, 250);
		}
		act("You launch yourself into the air.",ch,NULL,NULL,TO_CHAR);
		act("$n leaps high into the air.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi somersault [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi somersault [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi somersault [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N with your lightsaber as you somersault over them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they somersault over you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they jedi somersault [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you somersault over them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they somersault over you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they jedi somersault [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);
	return;
}/* End of void do_jsault */

void do_jreverse( CHAR_DATA *ch, char *argument )
{
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 2)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch, 6);
		}
		else
		{
			WAIT_STATE(ch, 9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 1)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 2;
			dam = 860 + number_range(50, 375);
			act("A intensity of the Force grows around you(1-2).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 8)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 9;
			dam = 870 + number_range(50, 395);
			act("A intensity of the Force grows around you(4-2).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 840 + number_range(50, 290);
		}
		act("You turn around swinging your lightsaber.",ch,NULL,NULL,TO_CHAR);
		act("$n turns aroudn swinging their lightsaber.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi reverse [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi reverse [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi reverse [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N with your lightsaber as you turn in the jedi reverse [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they turn around [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they turn around [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you turn around [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they turn around [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they turn around [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);
	return;
}
void do_jkneel( CHAR_DATA *ch, char *argument )
{
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 3)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch, 6);
		}
		else
		{
			WAIT_STATE(ch, 9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 2)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			dam = 960 + number_range(200, 550);
			act("A intensity of the Force grows around you(1-F).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
			act("You drop to one knee swinging your lightsaber.",ch,NULL,NULL,TO_CHAR);
			act("$n drops to one knee swinging their lightsaber.",ch,NULL,NULL,TO_NOTVICT);
			xprintf(buf,"You unleash the force solidly at $N's legs buckling them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"You feel your knees buckle as $n swings at them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$N looks in pain as $n buckles their knees [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, dam);
//			victim->hit -= dam;
			return;
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 13)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 14;
			dam = 850 + number_range(50, 450);
			act("A intensity of the Force grows around you(5-3).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 3)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 4;
			dam = 840 + number_range(50, 410);
			act("A intensity of the Force grows around you(2-2).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 15)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 16;
			dam = 860 + number_range(50, 470);
			act("A intensity of the Force grows around you(5-5).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 18)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 19;
			dam = 860 + number_range(30, 434);
			act("A intensity of the Force grows around you(6-3).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 850 + number_range(20, 340);
		}
		act("You drop to one knee swinging your lightsaber.",ch,NULL,NULL,TO_CHAR);
		act("$n drops to one knee swinging their lightsaber.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi kneel [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi kneel [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi kneel [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N with your lightsaber as you kneel [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they kneel [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they kneel [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you kneel [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they kneel [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they kneel [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);

	return;
}
void do_jstab( CHAR_DATA *ch, char *argument )
{
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 4)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch, 6);
		}
		else
		{
			WAIT_STATE(ch, 9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 0)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 3;
			dam = 860 + number_range(50, 350);
			act("A intensity of the Force grows around you(2-1).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 10)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 11;
			dam = 830 + number_range(40, 320);
			act("A intensity of the Force grows around you(4-4).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 16)
		{
			dam = 2590 + number_range(400, 900);
			ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			act("A intensity of the Force grows around you(5-F).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
			act("You reverse your grip on your light sabre lightsaber.",ch,NULL,NULL,TO_CHAR);
			act("$n reveses their grip on their light sabre.",ch,NULL,NULL,TO_NOTVICT);
			xprintf(buf,"You stab deep into $N's body [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n stabs you with their light sabre [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n stabs $N in one swift strike [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, dam);
//			victim->hit -= dam;
			return;
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 850 + number_range(50, 370);
		}
		act("You reverse your grip on your light sabre lightsaber.",ch,NULL,NULL,TO_CHAR);
		act("$n reveses their grip on their light sabre.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi stab [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi stab [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi stab [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N as you stab with your lightsaber [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they stab [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they stab [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you stab them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they stab at you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they stab [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);
	return;
}
void do_jhigh( CHAR_DATA *ch, char *argument )
{
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 5)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch, 6);
		}
		else
		{
			WAIT_STATE(ch, 9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 0)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 8;
			dam = 880 + number_range(40, 320);
			act("A intensity of the Force grows around you(4-1).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 11)
		{
			dam = 980 + number_range(300, 800);
			ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			act("A intensity of the Force grows around you(4-F).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
			act("You raise your lightsaber high above your head.",ch,NULL,NULL,TO_CHAR);
			act("$n raises their light sabre high above their head.",ch,NULL,NULL,TO_NOTVICT);
			xprintf(buf,"You bring your light sabre down solidly on $N's head [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n strikes at you with his light sabre [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n strikes $N down in one cleaving strike [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, dam);
//			victim->hit -= dam;
			return;
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 23)
		{
			dam = 880 + number_range(40, 690);
			ch->pcdata->stats[JEDI_COMBO_CURR] = 24;
			act("A intensity of the Force grows around you(6-8).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 870 + number_range(50, 340);
		}
		act("You raise your lightsaber high above your head.",ch,NULL,NULL,TO_CHAR);
		act("$n raises their light sabre high above their head.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi strike [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi strike [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi strike [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N as you strike with your lightsaber [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they strike [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they strike [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you strike them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they strike at you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they strike [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);
	return;
}
void do_jupper( CHAR_DATA *ch, char *argument )
{
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 6)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch, 6);
		}
		else
		{
			WAIT_STATE(ch, 9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 4)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			dam = 890 + number_range(300, 750);
			act("A intensity of the Force grows around you(2-F).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
			act("You lunge into a searing uppercut with your lightsaber.",ch,NULL,NULL,TO_CHAR);
			act("$n leaps into an uppercut holding his lightsaber high.",ch,NULL,NULL,TO_NOTVICT);
			xprintf(buf,"You unleash the force solidly at $N's head in a sweeping uppercut [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"You feel yourself be throw back as $n's uppercut strikes you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n strikes deep into $N with an uppercut [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, dam);
//			victim->hit -= dam;
			return;

		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 19)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 20;
			dam = 890 + number_range(50, 350);
			act("A intensity of the Force grows around you(6-4).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 860 + number_range(40, 340);
		}
		act("You lunge into a searing uppercut with your lightsaber.",ch,NULL,NULL,TO_CHAR);
		act("$n leaps into an uppercut holding his lightsaber high.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi uppercut [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi uppercut [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi uppercut [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N as you stab with your uppercut [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they upercut [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they uppercut [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you uppercut them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they uppercut at you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they uppercut [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);
	return;
}
void do_jbutterfly( CHAR_DATA *ch, char *argument )
{
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 7)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch, 6);
		}
		else
		{
			WAIT_STATE(ch, 9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 0)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 3;
			dam = 870 + number_range(50, 380);
			act("A intensity of the Force grows around you(2-1).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 12)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR]= 13;
			dam = 860 + number_range(50, 630);
			act("A intensity of the Force grows around you(5-2).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 7)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			dam = 3000 + number_range(260, 550);
			act("A intensity of the Force grows around you(3-F).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
			act("You swing your light sabre around your body.",ch,NULL,NULL,TO_CHAR);
			act("$n swings their light sabre around their body.",ch,NULL,NULL,TO_NOTVICT);
			xprintf(buf,"You unleash the force solidly at $N [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"You feel your strength leave as $n strikes at you with the force [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$N looks weaker as $n strikes out with the force [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
			hurt_person(ch, victim, (dam/2));
//			victim->hit -= (dam/2);
			victim->move -= dam;
			if (victim->move < 0) victim->move = 0;
			ch->move += dam;
			if (ch->move > (ch->max_move * 1.3))
				(ch->move = ch->max_move * 1.3);
			return;
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 20)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 21;
			dam = 870 + number_range(50, 524);
			act("A intensity of the Force grows around you(6-5).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 22)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 23;
			dam = 860 + number_range(50, 470);
			act("A intensity of the Force grows around you(6-7).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 860 + number_range(60, 290);
		}
		act("You swing your light sabre around your body.",ch,NULL,NULL,TO_CHAR);
		act("$n swings their light sabre around their body.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi butterfly [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi butterfly [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi butterfly [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N as you swing with your lightsaber [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they swing it around their body [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they swing their light sabre around their body [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you swing it [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they swing at you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they swing it [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);
	return;
}
void do_jcross( CHAR_DATA *ch, char *argument )
{
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 8)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch, 6);
		}
		else
		{
			WAIT_STATE(ch, 9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 6)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 7;
			dam = 870 + number_range(50, 450);
			act("A intensity of the Force grows around you(3-3).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 0)
		{
			dam = 880 + number_range(50, 270);
			act("A intensity of the Force grows around you(5-1).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
			ch->pcdata->stats[JEDI_COMBO_CURR] = 12;
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 21)
		{
			dam = 890 + number_range(60,470);
			ch->pcdata->stats[JEDI_COMBO_CURR] = 22;
			act("A intensity of the Force grows around you(6-6).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 860 + number_range(40, 680);
		}
		act("You bring your light sabre back at waist height.",ch,NULL,NULL,TO_CHAR);
		act("$n brings their lightsaber back at waist height.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi cross [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi cross [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi cross[%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N as you cross with your lightsaber [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they cross you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they cross them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you cross them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they cross at you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they cross [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);
	return;
}
void do_jslash( CHAR_DATA *ch, char *argument )
{
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 9)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch, 6);
		}
		else
		{
			WAIT_STATE(ch, 9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 5)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 6;
			dam = 880 + number_range(50, 340);
			act("A intensity of the Force grows around you(3-2).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 9)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 10;
			dam = 880 + number_range(50, 760);
			act("A intensity of the Force grows around you(4-3).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 17)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 18;
			dam = 880 + number_range(150, 930);
			act("A intensity of the Force grows around you(6-2).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else if (ch->pcdata->stats[JEDI_COMBO_CURR] == 0)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 17;
			dam = 890 + number_range(160, 570);
			act("A intensity of the Force grows around you(6-1).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 850 + number_range(150, 660);
		}
		act("You bring your lightsaber down upon your opponent.",ch,NULL,NULL,TO_CHAR);
		act("$n brings their light sabre down.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi slash [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi slash [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi slash [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N as you slash with your lightsaber [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they slash [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they slash [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you slash them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they slash at you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they slash [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);
	return;
}
void do_jcircle( CHAR_DATA *ch, char *argument )
{
	int dam;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];	

	if (IS_NPC(ch))
		return;
	else if (!IS_CLASS(ch, CLASS_JEDI))
	{
		send_to_char("Huh?.\n\r", ch);
		return;
	}
	else if (ch->pcdata->powers[JPOWER_TECHNIQUE] < 10)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ((victim = ch->fighting) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	else if (ch->position != POS_FIGHTING)
	{
		send_to_char("You must be fighting.\n\r", ch);
		return;
	}
    else if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL ||   obj->pIndexData->vnum != 33416 )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ||   obj->pIndexData->vnum != 33416 ) )
    {
        send_to_char( "You need to wield a lightsaber.\n\r", ch );
        return;
    }
	else if (ch->move < 1500)
	{
		send_to_char("You are not vital enough to use combo moves.\n\r", ch);
		return;
	}
	else
	{
		if (IS_SET(ch->special, SPC_JEDI_QUICKEN))
		{
			WAIT_STATE(ch, 6);
		}
		else
		{
			WAIT_STATE(ch, 9);
		}
		ch->move -= 100;
		if (ch->pcdata->stats[JEDI_COMBO_CURR] == 0)
		{
			ch->pcdata->stats[JEDI_COMBO_CURR] = 5;
			dam = 860 + number_range(50, 210);
			act("A intensity of the Force grows around you(3-1).",ch,NULL,NULL,TO_CHAR);
  			act("A intensity of the Force grows around $n.",ch,NULL,NULL,TO_NOTVICT);
		}
		else
		{
			if (ch->pcdata->stats[JEDI_COMBO_CURR] > 0)
			{
				act("A intensity of the Force fizzles out.",ch,NULL,NULL,TO_CHAR);
	  			act("A intensity of the Force dissipates around $n.",ch,NULL,NULL,TO_NOTVICT);
				ch->pcdata->stats[JEDI_COMBO_CURR] = 0;
			}
			dam = 880 + number_range(0, 830);
		}
		act("You circle around your opponent.",ch,NULL,NULL,TO_CHAR);
		act("$n circles behind their opponent.",ch,NULL,NULL,TO_NOTVICT);
		if (dam == 0)
		{
			xprintf(buf,"You miss $N with your jedi circle [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n misses you with their jedi circle [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n misses $N with their jedi circle [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if ((dam >0) && (dam < 100))
		{
			xprintf(buf,"You clip $N as you circle behind them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n clips you with their lightsaber as they circle behind you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n clips $N with their lightsaber as they circle behind them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
		else if (dam >= 100)
		{
			xprintf(buf,"You slice $N with your lightsaber as you circle them [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_CHAR);
			xprintf(buf,"$n slice you with their lightsaber as they circle you [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_VICT);
			xprintf(buf,"$n slice $N with their lightsaber as they circle [%d]\n\r",dam);
			act(buf,ch,NULL,victim,TO_NOTVICT);
		}
	}
	hurt_person(ch, victim, dam);
//	victim->hit -= dam;
WAIT_STATE(ch, 8);
	return;
}

/* End of Jedi Technique Powers */
/* JStep Jedi Travel Technique 3-17 Berelain */
void do_jstep(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *location;
	CHAR_DATA *victim;

	one_argument (argument, arg);

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_JEDI))
	{
	    stc("Huh?\n\r", ch);
	    return;
	}

	if ((victim = get_char_world(ch, arg)) == NULL) {
	send_to_char("Jstep to whom?\n\r", ch );
	return;}
	if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
	{
	stc( "You can't find it's room.\n\r",ch);
	return;
	}
	if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
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

	if (ch->move < 2000) {
	send_to_char("You are too tired to go through the force.\n\r", ch );
	return;}
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
		return;
	}
	if ((victim->in_room->vnum >= 50) && (victim->in_room->vnum <= 68))
	{
		send_to_char("You cannot transport inside the arena.\n\r", ch);
		return;
	}

	if (ch->fight_timer > 0)
        {
        send_to_char("Not with a fight timer!\n\r",ch);
        return;
        }
	if (!IS_NPC(victim))
	{
	if (ch->max_move < victim->max_move)
	{send_to_char("Get more move!\n\r",ch);
	return;}
	}
	act("You enter the force.", ch, NULL, NULL, TO_CHAR);
	act("$n walks into the force.", ch, NULL, NULL, TO_ROOM);
	ch->move -= 2000;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");	
	act("You walk out of the force.", ch, NULL, NULL, TO_CHAR);
	act("$n walks out of the force.", ch, NULL, NULL, TO_CHAR);

	return;
}

void do_channelforce( CHAR_DATA *ch, char *argument )
 {
    int sn;
    int spelltype;

    if ( IS_NPC(ch)) return;

        if (!IS_CLASS(ch, CLASS_JEDI))
        {
        send_to_char("Huh?\n\r", ch);
        return;
        }

        if (ch->mana < 500)
        {
        send_to_char("You don't have enough mana.\n\r", ch );
        return;
       }

    if ( ( sn = skill_lookup( "forcebless" ) ) < 0 ) return;

      spelltype = skill_table[sn].target;

    (*skill_table[sn].spell_fun) ( sn, 300, ch, ch );

    WAIT_STATE( ch, 12 );

    ch->mana = ch->mana - 500;
  
    return;

}

