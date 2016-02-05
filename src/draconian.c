/*			  <(()Seeds of Hate:  Swords & Sorcery Edition())>					 |
|					   _\|::>< Draconian Class ><::|/_							 |
|						  |Class Tier Number: I|								 |
|							  |Coder: Trent|									 |
|  *Draconian is an original class made for Seeds of Hate, Swords & Sorcery      |
|   Edition!                                                                     |
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
#include "magic.h"

void do_dragonrun( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_DRACONIAN) )
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Run to whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not of this world.\n\r", ch );
	return;
    }
    
   if( ch->move < 1000) 
    {
     stc( "The path escapes your senses as you lose control over your inner energies.\n\r",ch);
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

    act("You run to $N.", ch, NULL, victim, TO_CHAR);
    act("$n runs off into the distance", ch, NULL, victim, TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    ch->move -= 1000;
    act("$n runs from over the hills to $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n runs over the hills and up to you.", ch, NULL, victim, TO_VICT);
    do_look(ch,"auto");
    return;
}

void do_dragonorb( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRACONIAN))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
	{
	stc("#G _______\n\r",ch);
	stc("#G   \\    #Y\\\\  #G)#Y\\    #rD#Rragon #rO#Rrbs\n\r",ch);
	stc("#G    )   __#Y\\\\ ||\n\r",ch);
	stc("#G   /      #Y//#G_#Y/        |\n\r",ch);
	stc("#G_/     __#Y||#G_#Y|#g(__#Y      |\n\r",ch);
	stc("#G  \\       #Y\\\\#g_)  #R`#g-\\#Y   |      /\n\r",ch);
	stc("#G   )       #g/'  /^^^  #P___   #Y/\n\r",ch);
	xprintf(buf,"#G  /__     #g/   \\'    #P/  #W*#P\\            #yY#Yellow: #R+#Y(#G%d#Y)#R+\n\r",ch->pcdata->powers[ORB_YELLOW]);
	stc(buf,ch);
	xprintf(buf,"#G /   `\\  #g|  \\  #G|#y-- #p|     #P| #Y---        #gG#Green: #R+#Y(#G%d#Y)#R+\n\r",ch->pcdata->powers[ORB_GREEN]);
	stc(buf,ch);
	stc("#G       |/#g\\   \\  \\_#G__#p\\,_,#P/\n\r",ch);
	stc("#G       |   #g\\   \\----#G---/'  #Y\\\n\r",ch);
	stc("#g             \\   \\    #Y|      \\\n\r",ch);
	stc("#g           #R)  #g)  |    #Y|\n\r",ch);
	stc("#g            \\/  /     #Y|\n\r",ch);
	stc("#g            / /\\\\\n\r",ch);
	stc("#g           | (_)|\n\r",ch);
	stc("#g            \\___/#n\n\r",ch);
	return;
	}
	
		
		if (arg2[0] == '\0')
    {
		if (!str_cmp(arg1,"yellow"))
	{
		stc("#oY#yellow Dragon Orb Powers#n\n\r",ch);
		
		if (ch->pcdata->powers[ORB_YELLOW] >= 1)
		stc("Fiery Eyes   -  Truesight.\n\r",ch);
		if (ch->pcdata->powers[ORB_YELLOW] >= 2)
		stc("Claws        -  Gain razor sharp claws.\n\r",ch);
		if (ch->pcdata->powers[ORB_YELLOW] >= 3)
		stc("Headbash     -  A ramming headbutt.\n\r",ch);
		if (ch->pcdata->powers[ORB_YELLOW] >= 4)
		stc("Alacrity     -  Increases speed.\n\r",ch);
		if (ch->pcdata->powers[ORB_YELLOW] >= 5)
		stc("Acid Blood   -  Turn your blood to acid!\n\r",ch);
		if (ch->pcdata->powers[ORB_YELLOW] >= 6)
		stc("Lizardform   -  Shift into draconian form!\n\r",ch);

		return;
	}

	else if (!str_cmp(arg1,"green"))
	{
		stc("#gG#Green Dragon Orb Powers#n#n\n\r",ch);
		if (ch->pcdata->powers[ORB_GREEN] >= 1)
		stc("Tail Lash    -  A dextrous tail attack.\n\r",ch);
		if (ch->pcdata->powers[ORB_GREEN] >= 2)
		stc("Venomspit    -  Spit venom into your opponent's eyes.\n\r",ch);
		if (ch->pcdata->powers[ORB_GREEN] >= 3)
		stc("Dragonmight  -  Embody a sliver of power of the true dragon.\n\r",ch);
		if (ch->pcdata->powers[ORB_GREEN] >= 4)
		stc("Scales       -  Grow hard, reflective scales onto your body.\n\r",ch);
		if (ch->pcdata->powers[ORB_GREEN] >= 5)
		stc("Gasbreath    -  A draconian breath weapon.\n\r",ch);
		if (ch->pcdata->powers[ORB_GREEN] >= 6)
		stc("Firebreath   -  A red dragon breath weapon.\n\r",ch);
		return;
	}
	else 
	{
	stc("To view powers you've gained from an orb, type : dragonorb <yellow/green>\n\r",ch);
	stc("To learn powers, type: Dragonorb <yellow/green> Tap\n\r",ch);
	return;
	}

	}

    if (!str_cmp(arg2,"tap"))
    {
	int improve;
	int cost;
	int max;

	     if (!str_cmp(arg1,"yellow"   )) {improve = ORB_YELLOW;   max=6;}
	else if (!str_cmp(arg1,"green"   ))  {improve = ORB_GREEN;   max=6;}
	else
	{
	    send_to_char("#gS#Gyntax: #YDragonorb <yellow/green> Tap#n\n\r",ch);
	    return;
	}							

	cost = (ch->pcdata->powers[improve]+1) * 1000000;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    xprintf(buf,"#GYou've tapped as much power from the #y%s #GDragon Orb as you can!#n\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}

	if ( cost > ch->exp )
	{
	    xprintf(buf,"#GYou require #y%d #Gexp to tap the #y%s #GDragon Orb!#n\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}

	ch->pcdata->powers[improve] += 1;
	ch->exp -= cost;
	xprintf(buf,"#GYou successfully tap the energies from the #y%s #GDragon Orb!#n\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("#gSyntax: #GDragonorb #y<yellow/green> #GTap.#n\n\r",ch);
	return;
}

void do_spikes( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;	
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int cost = 0;
	int spikes = 0;
	int urin = 0;
	int urin_counter = 0;
	int dam;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	if (IS_NPC(ch))
	return;

	if (!IS_CLASS(ch,CLASS_DRACONIAN))
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (arg1[0] == '\0' && arg2[0] == '\0')
	{
	stc("#R+#Y------#G------#g---#G------#Y------#R+#n\n\r",ch);
	xprintf(buf,"    #gY#Gou have #Y%d #Gspikes!#n\n\r",ch->pcdata->stats[DRACONIAN_SPIKES]);
	stc(buf,ch);
	stc("#R+#Y------#G------#g---#G------#Y------#R+#n\n\r",ch);
	return;
	}

	if (!str_cmp(arg1,"release"))
	{
		if (ch->fighting == NULL)
		{
		stc("Not unless fighting!\n\r",ch);
		return;
		}
		
		if (ch->pcdata->stats[DRACONIAN_SPIKES] < 1)
		{
		stc("You have no spikes to shoot!\n\r",ch);
		return;
		}
		victim = ch->fighting;
		dam = number_range(ch->pcdata->stats[DRACONIAN_SPIKES] * 20, ch->pcdata->stats[DRACONIAN_SPIKES] * 40);
		
		xprintf(buf,"#GYour spikes shoot from your body, #Rimpaling #y$N!  #0[#R%d#0]#n",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		xprintf(buf,"#GA plethora of sharp spikes shoot from #y$n's #Gbody, #Rimpaling #Gyou!  #0[#R%d#0]#n",dam);
		act(buf,ch,NULL,victim,TO_VICT);
		act("#g$n #Gshoots a plethora of sharp spikes from $s body at #y$N, #Rimpaling #Gthem!#n",ch,NULL,victim,TO_NOTVICT);

		hurt_person(ch,victim,dam);
		ch->pcdata->stats[DRACONIAN_SPIKES] = 0;
		return;
	}

	else if (!str_cmp(arg1,"grow"))
	{
		if (arg2[0] == '\0')
		{
		stc("#gSyntax: #Gspikes grow <amount>#n\n\r",ch);
		return;
		}
		
		if (ch->position <= POS_FIGHTING)
		{
		stc("You must be idle and standing to do this!\n\r",ch);
		return;
		}

		if (ch->fight_timer > 0)
		{
		stc("Not with a fight timer!\n\r",ch);
		return;
		}

		if (arg2[0] == '\0')
		{
		stc("#GHow many spikes do you wish to grow?#n\n\r",ch);
		return;
		}
	
		if (!is_number(arg2))
		{
		stc("Please enter a numeric value between 1 and 200!\n\r",ch);
		return;
		}
		
		if (ch->pcdata->stats[DRACONIAN_SPIKES] >= 200)
		{
		stc("You cannot grow any more spikes!\n\r",ch);
		return;
		}

		if (is_number(arg2))
		{

			spikes = atoi(arg2);

			cost = spikes * 10000;
			
			if (spikes > 200 || spikes < 1)
			{
			stc("Please enter a numeric value between 1 and 200!\n\r",ch);
			return;
			}

			for (urin = 0; urin < spikes ; urin++)
			{
				if (ch->exp >= 10000 && ch->pcdata->stats[DRACONIAN_SPIKES] < 200)
				{
				ch->exp -= 10000;
				ch->pcdata->stats[DRACONIAN_SPIKES]++;
				urin_counter+=1;
				}
			}	
			
			if (urin_counter==0)
			{
				send_to_char( "You need more exp to gain any more spikes.\n\r", ch );
			}
			if (urin_counter==1)
			{
				send_to_char( "#GYou gain #y1 #Gspike.#n\n\r", ch );
			}
			if (urin_counter>1)
			{
				xprintf( buf, "#GYou gain #y%d #Gspikes.#n\n\r", urin_counter );
				send_to_char( buf, ch );
			}
		return;
		}		


	}
	else
	{
	stc("Syntax: spikes grow <amount>\n\r",ch);
	stc("        spikes release\n\r",ch);
        stc("        spikes\n\r",ch);
        return;
	}
}

void do_fieryeyes(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_DRACONIAN))
	{
	send_to_char("Huh?\n\r", ch );
	return;
	}

	if (ch->pcdata->powers[ORB_YELLOW] < 1)
	{
		stc("Huh?\n\r",ch);
		return;
	}	

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {   
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "Your vision re-adjusts to normal.\n\r", ch );
    }
    else
    {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "#RYour eyes burn with a hateful fire.#n\n\r", ch);
    }

	return;
}

void do_dclaws(CHAR_DATA *ch, char *argument)
{

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRACONIAN))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (ch->pcdata->powers[ORB_YELLOW] < 2)
	{
	stc("Huh?\n\r",ch);
	return;
	}

    if (IS_VAMPAFF(ch, VAM_CLAWS))
   {
	send_to_char("#GYou retract your claws.#n\n\r",ch);
	act("#y$n #Gretracts #g$s #Gclaws.#n\n\r",ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   return;
   }

      else
   {
	send_to_char("#GRazor sharp claws extend from your fingers!#n\n\r",ch);
	act("#GRazor sharp claws extend from #y$n's #Gfingers.#n\n\r",ch, NULL, NULL, TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   return;
   }
}

void do_headbash(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
	char arg [MAX_INPUT_LENGTH];
	char buf [MAX_STRING_LENGTH];
	int dam;
	int stun;

	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
	return;

	if (!IS_CLASS(ch,CLASS_DRACONIAN))
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (ch->pcdata->powers[ORB_YELLOW] < 3)
	{
	stc("Huh?\n\r",ch);
	return;
	}
	
	

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

	if (ch->fighting == NULL)
	{
	stc("You must be fighting to use this!\n\r",ch);
	return;
	}
	
	if (is_safe(ch,victim))
		{
		stc("You can't attack them!\n\r",ch);
		return;	
		}

    if ( ch == victim )
    {
        send_to_char( "Not on yourself.\n\r", ch );
        return;
    }

	if (ch->move < 500) 
	{
		send_to_char("You don't have enough move.\n\r", ch );
		return;
	}

	stun = number_percent();

if (stun < 60)
{
	dam = number_range(3000,7000);
	xprintf(buf,"#GYou bash your head into #y$N's! #R*#yCraCk#R* #0[#R%d#0]#n",dam);
	act(buf,ch,NULL,victim,TO_CHAR);
	xprintf(buf,"#y$n #Gbashes $s head into yours! #R*#yCraCk#R* #0[#R%d#0]#n",dam);
	act(buf,ch,NULL,victim,TO_VICT);
	act("#g$n #Gbashes $s head into #y$N's!  #R*#yCraCk#R*#n",ch,NULL,victim,TO_NOTVICT);

	hurt_person(ch,victim,dam);
}

else if (stun >= 60)
{
	dam = number_range(4500,7000);
	xprintf(buf,"#GYou bash your head into #Y$N's! #R*#YCraCk#R* #0[#R%d#0]#n",dam);
	act(buf,ch,NULL,victim,TO_CHAR);
	xprintf(buf,"#Y$n #Gbashes $s head into yours! #R*#YCraCk#R* #0[#R%d#0]#n",dam);
	act(buf,ch,NULL,victim,TO_VICT);
	act("#g$n #Gbashes $s head into #y$N's!  #R*#YCraCk#R*#n",ch,NULL,victim,TO_NOTVICT);

	act("#y$N #Gkeels over and falls to the ground, #Rstunned!#n",ch,NULL,victim,TO_CHAR);
	act("#GYou keel over and fall to the ground, #Rstunned!#n",ch,NULL,victim,TO_VICT);
	act("#y$N #Gkeels over and falls to the ground, #Rstunned!#n",ch,NULL,victim,TO_NOTVICT);

	hurt_person(ch,victim,dam);
	stop_fighting(victim,TRUE);
	victim->position = POS_STUNNED;
}
    ch->move -= 500;
	WAIT_STATE(ch, 12);
}

void do_acidblood( CHAR_DATA *ch, char *argument ) 
{
	if (IS_NPC(ch)) return;

	if(!IS_CLASS(ch, CLASS_DRACONIAN))
	{
	send_to_char("Huh?\n\r", ch );
	return;
	}

	if (ch->pcdata->powers[ORB_YELLOW] < 5)
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (IS_SET(ch->newbits2, NEW2_ACIDBLOOD))
	{
	stc("#0The deadly #Gacidity #0fades from your blood.#n\n\r",ch);
	REMOVE_BIT(ch->newbits2, NEW2_ACIDBLOOD);
	return;
	}

	stc("#0Your blood becomes #Gvile #0and terribly #Gacidic!#n\n\r",ch);
	SET_BIT(ch->newbits2, NEW2_ACIDBLOOD);

	WAIT_STATE(ch,12);
	return;
}

void do_lizardform( CHAR_DATA *ch, char *argument ) 
{
	char buf[MAX_STRING_LENGTH];	
	
	if (IS_NPC(ch)) return;

	if(!IS_CLASS(ch, CLASS_DRACONIAN))
	{
	send_to_char("Huh?\n\r", ch );
	return;
	}

	if (ch->pcdata->powers[ORB_YELLOW] < 6)
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (ch->move < 5000)
	{
	stc("You don't have the required 10000 move to use this!\n\r",ch);
	return;
	}

	if (ch->mana < 5000)
	{
	stc("You don't have the required 10000 mana to use this!\n\r",ch);
	return;
	}

	if (IS_POLYAFF(ch, POLY_LIZARDFORM))
	{
	act("#gYou return to your human form.#n",ch,NULL,NULL,TO_CHAR);
	act("#Y$n #Greverts to $s normal form.#n",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->polyaff, POLY_LIZARDFORM);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	ch->hitroll -= 900;
	ch->damroll -= 900;
	ch->armor += 950;
    free_string( ch->morph );
    ch->morph = str_dup( "" );	
	return;
	}

    ch->move -= 5000;
    ch->mana -= 5000;
	stc("#GYou feel your skin begin to harden and grow cold!#n\n\r",ch);
	stc("#GYour snout extends, and razor sharp teeth grow from your gums!#n\n\r",ch);
	act("#GYou roar as you polymorph into a ferocious draconian lizard!!#n",ch,NULL,NULL,TO_CHAR);
	act("#y$n #Groars as they transform into a ferocious draconian lizard!#n",ch,NULL,NULL,TO_ROOM);
	ch->hitroll += 900;
	ch->damroll += 900;
	ch->armor -= 950;
    SET_BIT(ch->polyaff, POLY_LIZARDFORM);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
    
	if (ch->generation == 6)
	xprintf(buf, "#R%s#G, the scrawny lizard man#n",ch->pcdata->switchname);
	else if (ch->generation == 5)
	xprintf(buf, "#R%s#G, the muscular lizard man#n",ch->pcdata->switchname);
	else if (ch->generation == 4)
	xprintf(buf, "#R%s#G, the giant lizard#n",ch->pcdata->switchname);
	else if (ch->generation == 3)
	xprintf(buf, "#R%s#G, the vicious draconian#n",ch->pcdata->switchname);
	else
	xprintf(buf, "#R%s#G, the bloodthirsty draconian#n",ch->pcdata->switchname);
    free_string( ch->morph );
    ch->morph = str_dup( buf );	

	WAIT_STATE(ch,12);
	return;
}

void do_dtail(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
	char arg [MAX_INPUT_LENGTH];

	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
	return;

	if (!IS_CLASS(ch,CLASS_DRACONIAN))
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (ch->pcdata->powers[ORB_GREEN] < 1)
	{
	stc("Huh?\n\r",ch);
	return;
	}

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

	if (ch->fighting == NULL)
	{
	stc("You must be fighting to use this!\n\r",ch);
	return;
	}

    if ( ch == victim )
    {
        send_to_char( "Not on yourself.\n\r", ch );
        return;
    }
    
    if (is_safe(ch,victim))
		{
		stc("You can't attack them!\n\r",ch);
		return;	
		}

	if (ch->move < 500) 
	{
		send_to_char("You don't have enough move.\n\r", ch );
		return;
	}

	if (number_range(1,10) > 6)
	{
	act("#GYou sweep your tail at #Y$N!",ch,NULL,victim,TO_CHAR);
	act("#Y$n #Gsweeps $s tail at you!#n",ch,NULL,victim,TO_VICT);
	act("#g$n #Gsweeps $s tail at $Y$N#n",ch,NULL,victim,TO_NOTVICT);

	trip(ch,victim);
	}

	else
	{
	act("#GYou sweep your tail at #Y$N, #Gbut they jump over it!#n",ch,NULL,victim,TO_CHAR);
	act("#Y$n #Gsweeps $s tail at you, but you jump over it!#n",ch,NULL,victim,TO_VICT);
	act("#g$n #Gsweeps $s tail at #Y$N, #Gbut they jump over it!#n",ch,NULL,victim,TO_NOTVICT);
	}

	ch->move -= 500;
	WAIT_STATE(ch,8);
	return;
}

void do_venomspit(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;	
	char arg [MAX_INPUT_LENGTH];
	char buf [MAX_STRING_LENGTH];
	int dam;

	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
	return;

	if (!IS_CLASS(ch,CLASS_DRACONIAN))
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (ch->pcdata->powers[ORB_GREEN] < 2)
	{
	stc("Huh?\n\r",ch);
	return;
	}

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "Not on yourself.\n\r", ch );
        return;
    }

	if (is_safe(ch,victim))
		{
		stc("You can't attack them!\n\r",ch);
		return;	
		}

	if (ch->move < 1000) 
	{
		send_to_char("You don't have the required 1000 move.\n\r", ch );
		return;
	}

	dam = number_range(ch->pcdata->powers[ORB_GREEN] * 200,ch->pcdata->powers[ORB_GREEN] * 600);

	xprintf(buf,"#0You spit a #Gvenomous substance #0at #R$N!  #0[#R%d#0]#n",dam);
	act(buf,ch,NULL,victim,TO_CHAR);
	xprintf(buf,"#R$n #0spits a #Gvenomous substance #0at you!  #0[#R%d#0]#n",dam);
	act(buf,ch,NULL,victim,TO_VICT);
	act("#R$n #0spits a #Gvenomous substance #0at #R$N!#n",ch,NULL,victim,TO_NOTVICT);

	hurt_person(ch,victim,dam);

	if (!IS_AFFECTED(victim, AFF_BLIND) && number_percent() > 60)
	{
	    spell_blindness( skill_lookup( "blindness" ), ch->generation * 10, ch, victim );
	}

	if (!IS_AFFECTED(victim, AFF_POISON) && number_percent() > 50)
	{
    af.type      = gsn_poison;
    af.duration  = ch->generation * 10;
    af.location  = APPLY_STR;
    af.modifier  = 0 - number_range(1,3);
    af.bitvector = AFF_POISON;
    affect_join( victim, &af );
    send_to_char( "You feel very sick.\n\r", victim );
    if (!IS_NPC(victim))
	xprintf(buf,"%s looks very sick as your poison takes affect.\n\r",victim->name);
    else
	xprintf(buf,"%s looks very sick as your poison takes affect.\n\r",victim->short_descr);
    send_to_char(buf,ch);
	}

	WAIT_STATE(ch,12);

	if (ch->fighting != victim)
	ch->fighting = victim;

	ch->move -= 1000;

return;
}

void do_dscales( CHAR_DATA *ch, char *argument ) 
{
	if (IS_NPC(ch)) return;

	if(!IS_CLASS(ch, CLASS_DRACONIAN))
	{
	send_to_char("Huh?\n\r", ch );
	return;
	}

	if (ch->pcdata->powers[ORB_GREEN] < 4)
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (IS_SET(ch->newbits2, NEW2_SCALES))
	{
	stc("#GYour scales #7shimmer and vanish.#n\n\r",ch);
	REMOVE_BIT(ch->newbits2, NEW2_SCALES);
	ch->armor += 650;
	return;
	}

	stc("#GYour skin #7shimmers, #Gand is coated in a thick layer of #Yglossy scales!#n\n\r",ch);
	SET_BIT(ch->newbits2, NEW2_SCALES);
	ch->armor -= 650;

	WAIT_STATE(ch,12);
	return;
}

void do_gasbreath(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;	
	char buf [MAX_STRING_LENGTH];
	int dam;

	if (IS_NPC(ch))
	return;

	if (!IS_CLASS(ch,CLASS_DRACONIAN))
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (ch->pcdata->powers[ORB_GREEN] < 5)
	{
	stc("Huh?\n\r",ch);
	return;
	}
	

	
	if (ch->move < 500)
	{
	stc("You have not the 250 move to use this!\n\r",ch);
	return;
	}

	act("You rear back your head and blow forth a powerful cloud of gas!",ch,NULL,NULL,TO_CHAR);
	act("$n rears back $s head and blows forth a powerful cloud of gas!",ch,NULL,NULL,TO_ROOM);

	dam = (ch->pcdata->powers[ORB_GREEN] * 300) + (ch->pcdata->powers[ORB_YELLOW] * 300) + number_range(100, 300);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;
	if (!can_see(ch, vch)) continue;
	if (!is_safe(ch, vch))
	if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
	{
		dam = ((ch->pcdata->powers[ORB_GREEN] * 300) + (ch->pcdata->powers[ORB_YELLOW] * 300) + number_range(-500, 100));

	   if ( IS_AFFECTED(vch, AFF_SANCTUARY) ) dam /=2;
	    
		xprintf(buf,"#0Your gasbreath overcomes #R%s!  #0[#R%d#0]#n\n\r",vch->short_descr,dam);
		stc(buf,ch);
		
		if (vch->fighting != ch)
		set_fighting(vch, ch, 1);
		hurt_person(ch,vch,dam);
	}
	continue;
    }
	ch->move -= 250;
	WAIT_STATE(ch,12);
    return;
}

void do_firebreath(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
	char arg [MAX_INPUT_LENGTH];
	char buf [MAX_STRING_LENGTH];
	int dam;

	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
	return;

	if (!IS_CLASS(ch,CLASS_DRACONIAN))
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (ch->pcdata->powers[ORB_GREEN] < 6)
	{
	stc("Huh?\n\r",ch);
	return;
	}

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "Not on yourself.\n\r", ch );
        return;
    }

	if (is_safe(ch,victim))
		{
		stc("You can't shoot them!\n\r",ch);
		return;	
		}

	if (ch->move < 5000) 
	{
		send_to_char("You don't have the required 5000 move.\n\r", ch );
		return;
	}

	if (ch->mana < 5000)
	{
	stc("You don't have the required 5000 mana!\n\r",ch);
	return;
	}

	dam = number_range(ch->pcdata->powers[ORB_GREEN] * 500,ch->pcdata->powers[ORB_GREEN] * 1200);
	dam += number_range(500,2000);
	if (IS_NPC(victim)) dam *= 2;

	xprintf(buf,"#gYou rear back your head and blow forth a huge ball of #Rf#Yl#Wa#Ym#Re #gat #G$N!  #0[#R%d#0]#n",dam);
	act(buf,ch,NULL,victim,TO_CHAR);
	xprintf(buf,"#G$n #grears back $s head and blows forth a huge ball of #Rf#Yl#Wa#Ym#Re #gat you!  #0[#R%d#0]#n",dam);
	act(buf,ch,NULL,victim,TO_VICT);
	act("#G$n #grears back $s head and blows forth a huge ball of #Rf#Yl#Wa#Ym#Re #gat #G$N!#n",ch,NULL,victim,TO_NOTVICT);

	if (!IS_AFFECTED(victim,AFF_FLAMING) )
	{
	SET_BIT(victim->affected_by, AFF_FLAMING);
	act("#r$N #Rcatches on #Yfire #Rand begins to burn!#n",ch,NULL,victim,TO_CHAR);
	act("#RYou catch on #Yfire #Rand begin to burn!#n",ch,NULL,victim,TO_VICT);
	act("#r$N #Rcatches on fire and begins to burn!#n",ch,NULL,victim,TO_NOTVICT);
	}

	if (ch->fighting != victim)
	set_fighting(ch,victim, 1);

	hurt_person(ch,victim,dam);

	ch->move -= 5000;
	ch->mana -= 5000;

	WAIT_STATE(ch, 15);
	return;
	}


void do_deathstate( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	int state;

    argument = one_argument( argument, arg );

	if (IS_NPC(ch))
	return;

	if (!IS_CLASS(ch,CLASS_DRACONIAN))
	{
	stc("Huh?\n\r",ch);
	return;
	}

	if (ch->exp < 500000)
	{
	stc("It costs 500,000 exp to set or change your deathstate!\n\r",ch);
	return;
	}

	if (arg[0] == '\0')
	{
	stc("#R+#y------#G------#g---#G------#y------#R+#n\n\r",ch);
	stc("#RYour art of self destruction:#n\n\r",ch);
	if (IS_SET(ch->pcdata->stats[1],DRACONIAN_PETRIFY))
	stc("#7P#Wetrification#n\n\r",ch);
	else if (IS_SET(ch->pcdata->stats[1],DRACONIAN_ACIDBATH))
	stc("#gA#Gcid #gB#Gath#n\n\r",ch);
	else if (IS_SET(ch->pcdata->stats[1],DRACONIAN_FLAMEDEATH))
	stc("#RF#Ylaming #RD#Yeath#n\n\r",ch);
	else if (IS_SET(ch->pcdata->stats[1],DRACONIAN_CHLOROFORM))
	stc("#GC#0hloroform #GE#0ffusion#n\n\r",ch);
	else
	{
	stc("#RYou have not yet set a deathstate!#n\n\r",ch);
	stc("#RSyntax: Deathstate <state>#n\n\r",ch);
	stc("#rStates: Petrify, Acid, Flame, Chloroform.#n\n\r",ch);
	}
	stc("#R+#y------#G------#g---#G------#y------#R+#n\n\r",ch);
	return;
	}

	if (!str_cmp(arg,"petrify"))
	state = DRACONIAN_PETRIFY;
	else if (!str_cmp(arg,"acid"))
	state = DRACONIAN_ACIDBATH;
	else if (!str_cmp(arg,"flame"))
	state = DRACONIAN_FLAMEDEATH;
	else if (!str_cmp(arg,"chloroform"))
	state = DRACONIAN_CHLOROFORM;
	else
	{
	stc("Syntax: Deathstate <petrify/acid/flame/chloroform>\n\r",ch);
	return;
	}

	if (IS_SET(ch->pcdata->stats[1],DRACONIAN_PETRIFY))
	REMOVE_BIT(ch->pcdata->stats[1],DRACONIAN_PETRIFY);
	else if (IS_SET(ch->pcdata->stats[1],DRACONIAN_ACIDBATH))
	REMOVE_BIT(ch->pcdata->stats[1],DRACONIAN_ACIDBATH);
	else if (IS_SET(ch->pcdata->stats[1],DRACONIAN_FLAMEDEATH))
	REMOVE_BIT(ch->pcdata->stats[1],DRACONIAN_FLAMEDEATH);
	else if (IS_SET(ch->pcdata->stats[1],DRACONIAN_CHLOROFORM))
	REMOVE_BIT(ch->pcdata->stats[1],DRACONIAN_CHLOROFORM);
	
	SET_BIT(ch->pcdata->stats[1],state);
	ch->exp -= 500000;
	stc("Deathstate set!\n\r",ch);
	return;
}

void draconian_death( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *damaged;
    OBJ_DATA *obj;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;	
    AFFECT_DATA af;		
    char buf[MAX_STRING_LENGTH];
    int dam; //object damage
    int dam2;
 
       	if (IS_NPC(victim)) return;
	if (!IS_CLASS(victim,CLASS_DRACONIAN)) return;


	//Acid Bath - hurts player and dmgs their eq.
	if (IS_SET(victim->pcdata->stats[1],DRACONIAN_ACIDBATH))
	{
		dam = number_range(50,99);
		dam2 = number_range(2000,3000);
		
		xprintf(buf,"#G$N's #0body #REXPLODES, #0splashing #Ghot acid #0all over you!  #0[#R%d#0]#n",dam2);
		act(buf,ch,NULL,victim,TO_CHAR);
		xprintf(buf,"#0Your body #REXPLODES, #0splashing #Ghot acid #0all over #G%s!  #0[#R%d#0]#n\n\r",ch->pcdata->switchname,dam2);
		stc(buf,victim);
		act("#G$N's #0body #REXPLODES, #0splashing #Ghot acid #0all over #G$n!#n",ch,NULL,victim,TO_NOTVICT);
		
		if (is_safe(ch,victim))
		{
		 return;
		}
		else
		{
		hurt_person(victim,ch,dam2);
		}

		if ( (damaged = get_eq_char(ch,WEAR_FACE) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_BODY) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_ABOUT) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_FEET) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_LEGS) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_HEAD) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_ARMS) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_HOLD) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_WIELD) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_HANDS) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_WRIST_L) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_WRIST_R) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_FINGER_L) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		if ( (damaged = get_eq_char(ch,WEAR_FINGER_R) ) != NULL && damaged->toughness > 0)
		{
			act("$p takes acid damage!",ch,damaged,NULL,TO_CHAR);
			
			damaged->condition -= dam;

			if (damaged->condition < 1)
			{
				act("$p disintegrates!",ch,damaged,NULL,TO_CHAR);
				act("$n gasps as $p disintegrates!",ch,damaged,NULL,TO_ROOM);
				obj_from_char(damaged);
				extract_obj(damaged);
			}
		}

		}
		//End of Acid Bath
	
	else if (IS_SET(victim->pcdata->stats[1],DRACONIAN_FLAMEDEATH)) //Flaming Death : Good dmg, fire affect.
	{
		dam = number_range(3000,6000);
		
		xprintf(buf,"#R$N's #0body #rEX#RP#7LO#RD#rES #0in #Rf#Yl#Wam#Ye#Rs, #0searing your flesh!  [#R%d#0]#n",dam);
		act(buf,ch,NULL,victim,TO_CHAR);
		xprintf(buf,"#0Your body #rEX#RP#7LO#RD#rES #0in #Rf#Yl#Wam#Ye#Rs, #0searing #R$n's #0flesh!  [#R%d#0]#n",dam);
		stc(buf,victim);
		act("#R$N's #0body #rEX#RP#7LO#RD#rES #0in #Rf#Yl#Wam#Ye#Rs, #0searing #R$n's #0flesh!#n",ch,NULL,victim,TO_NOTVICT);
		
		hurt_person(victim,ch,dam);
		
		if (!IS_AFFECTED(ch,AFF_FLAMING) )
		{
		SET_BIT(ch->affected_by, AFF_FLAMING);
		stc("#RYou catch #Yfire!#n\n\r",ch);
		xprintf(buf,"#r%s #Rcatches #Yfire!#n\n\r",ch->pcdata->switchname);
		stc(buf,victim);
		}
	}

	else if (IS_SET(victim->pcdata->stats[1],DRACONIAN_PETRIFY)) //Petrify: ch loses wielded wpn, and is 'stunned'
	{
		
		act("#7$N's #0body turns to #7stone, #0petrifying you!",ch,NULL,victim,TO_CHAR);
		xprintf(buf,"#0Your body turns to #7stone, #0petrifying #W%s!#n\n\r",ch->pcdata->switchname);
		stc(buf,victim);
		act("#7$N's #0body turns to #7stone, #0petrifying #W$n!",ch,NULL,victim,TO_NOTVICT);

		WAIT_STATE(ch,24);

		if ( (obj= get_eq_char(ch,WEAR_WIELD) ) != NULL)
		{
			act("#0Your weapon is wrenched from your hands as #7$N's #0corpse falls limp to the ground!#n",ch,NULL,victim,TO_CHAR);
			xprintf(buf,"#7%s's #0weapon is wrenched from their hands as your corpse falls limp to the ground!#n\n\r",ch->pcdata->switchname);
			stc(buf,victim);
			act("#7$n's #0weapon is wrenched from $s hands as $N's corpse falls limp to the ground!#n",ch,NULL,victim,TO_NOTVICT);

				obj_from_char(obj);
				obj_to_char(obj, victim);
		}
	}

	else if (IS_SET(victim->pcdata->stats[1],DRACONIAN_CHLOROFORM)) //Chloroform Effusion: gasses room and chance to stun/poison
	{
	
		act("#P$N's #0corpse releases a foul smelling cloud of #Pgas!#n",ch,NULL,victim,TO_CHAR);
		stc("#0Your corpse releases a foul smelling cloud of #Pgas!#n\n\r",victim);
		act("#P$N's #0corpse releases a foul smelling cloud of #Pgas#n!",ch,NULL,victim,TO_NOTVICT);

		
		for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
		{
		vch_next = vch->next_in_room;
		if ( !IS_NPC(vch) && vch != victim )
		{
			dam = number_range(1000,3000);
			
			xprintf(buf,"#0Your #Pchloroform gas #0overcomes #R%s!  #0[#R%d#0]#n\n\r",vch->pcdata->switchname,dam);
			stc(buf,victim);
			xprintf(buf,"#R%s's #Pchloroform gas #0overcomes you!  #0[#R%d#0]#n\n\r",victim->pcdata->switchname,dam);
			stc(buf,vch);

			hurt_person(victim,vch,dam);

			if (!IS_AFFECTED(vch, AFF_POISON) && number_range(1,3) > 2)
			{
			af.type      = gsn_poison;
			af.duration  = victim->generation * 10;
			af.location  = APPLY_STR;
			af.modifier  = 0 - number_range(1,3);
			af.bitvector = AFF_POISON;
			affect_join( vch, &af );
			send_to_char( "You feel very sick.\n\r", victim );
			if (!IS_NPC(vch))
			{
			xprintf(buf,"%s looks very sick as your poison takes affect.\n\r",vch->pcdata->switchname);
			stc("#0You have been #Gpoisoned!#n\n\r",vch);
			}
			else
			xprintf(buf,"%s looks very sick as your poison takes affect.\n\r",vch->short_descr);
			send_to_char(buf,victim);
			}

			if (vch->position != POS_STUNNED && number_range(1,3) < 2)
			{
			vch->position = POS_STUNNED;
			stc("#0You pass out from the stench!\n\r",vch);
			WAIT_STATE(vch,12);
			}	
		}
		}
	}
		return;
}
