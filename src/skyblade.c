//     ***********                                             ***********
//     ***********                                             ***********
//      *       *               Seeds of Hate                   *       *
//       *     *             Swords & Sorcery ed.                *     *
//        *   *               Skyblades (TCL: I)                  *   *
//         * *                                                     * *
//          *      This is an original class coded by:              *
//         * *       Magus Locke, TimeMaster                       * *
//        *   *      						  *   *
//       *     *     No part(s) of this code cannot be           *     *
//      *       *    used or duplicated without acknowledging   *       *
//     ***********   Seeds of Hate and Magus.                  ***********
//     ***********                                             ***********

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


void do_abilities( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_SKYBLADE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	xprintf(buf,"Abilities: #LPower (%d), #WMagic (%d), #CSkills (%d).#n\n\r",
	ch->pcdata->powers[SKYBLADE_POWER], ch->pcdata->powers[SKYBLADE_MAGIC],
	ch->pcdata->powers[SKYBLADE_SKILLS]);
	send_to_char(buf,ch);
	return;
    }
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"power"))
	{
/*	    send_to_char("-=^=-=^=-=^=-=^=-=^=-   Skyblade Powers
-=^=-=^=-=^=-=^=-=^=-#n\n\r",ch);
       send_to_char("| Powers            Learned             Improved              |#n\n\r",ch);
   if (ch->pcdata->powers[SKYBLADE_POWER] > 0) {
       if (ch->pcdata->powers[SKYBLADE_POWER] > 0)
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  No                 |#n\n\r",ch);
   else if (ch->pcdata->powers[SKYBLADE_POWER] > 1)
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  Yes                |#n\n\r",ch);
       if (ch->pcdata->powers[SKYBLADE_POWER] > 2){
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  Yes                |#n\n\r",ch);
       send_to_char("| EagleStrength       Yes                  No                 |#n\n\r",ch);}
   else if (ch->pcdata->powers[SKYBLADE_POWER] > 3){
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  Yes                |#n\n\r",ch);
   	 send_to_char("| EagleStrength       Yes                  Yes                |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_POWER] > 4){
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  Yes                |#n\n\r",ch);
       send_to_char("| EagleStrength       Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HawkEyes            Yes                  No                 |#n\n\r",ch);
}
   else if (ch->pcdata->powers[SKYBLADE_POWER] > 5){
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  Yes                |#n\n\r",ch);
       send_to_char("| EagleStrength       Yes                  Yes                |#n\n\r",ch);
   	 send_to_char("| HawkEyes            Yes                  Yes                |#n\n\r",ch);
}
          if (ch->pcdata->powers[SKYBLADE_POWER] > 6){
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  Yes                |#n\n\r",ch);
       send_to_char("| EagleStrength       Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HawkEyes            Yes                  Yes                |#n\n\r",ch);
       send_to_char("| Vulture's Vigor     Yes                  No                 |#n\n\r",ch);
}
   else if (ch->pcdata->powers[SKYBLADE_POWER] > 7){
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  Yes                |#n\n\r",ch);
       send_to_char("| EagleStrength       Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HawkEyes            Yes                  Yes                |#n\n\r",ch);
   	 send_to_char("| Vulture's Vigor     Yes                  Yes                |#n\n\r",ch);
}
          if (ch->pcdata->powers[SKYBLADE_POWER] > 8){
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  Yes                |#n\n\r",ch);
       send_to_char("| EagleStrength       Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HawkEyes            Yes                  Yes                |#n\n\r",ch);
		 send_to_char("| Vulture's Vigor     Yes                  Yes                |#n\n\r",ch);
       send_to_char("| FalconBlade         Yes                  No                 |#n\n\r",ch);
}
   else if (ch->pcdata->powers[SKYBLADE_POWER] > 9){
       send_to_char("| OwlEyes(Leye,Reye)  Yes                  Yes                |#n\n\r",ch);
       send_to_char("| EagleStrength       Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HawkEyes            Yes                  Yes                |#n\n\r",ch);
       send_to_char("| Vulture's Vigor     Yes                  Yes                |#n\n\r",ch);
   	 send_to_char("| FalconBlade         Yes                  Yes                |#n\n\r",ch);}
}
   else{
   	 send_to_char("| OwlEyes(Leye,Reye)  No                   No                 |#n\n\r",ch);
		 send_to_char("| EagleStrength       No                   No                 |#n\n\r",ch);
		 send_to_char("| HawkEyes            No                   No                 |#n\n\r",ch);
		 send_to_char("| Vulture's Vigor     No                   No                 |#n\n\r",ch);
	 	 send_to_char("| FalconBlade         No                   No                 |#n\n\r",ch);}

send_to_char("-=^=-=^=-=^=-=^=-=^=-=^=-=^=-=^=-^-=^=-=^=-=^=-=^=-=^=-=^=-=^=-#n\n\r",ch);*/
	xprintf(buf, "You have %d power.\n\r", ch->pcdata->powers[SKYBLADE_POWER]);
	stc(buf, ch);
	    return;
	}
	else if (!str_cmp(arg1,"magic"))
	{/*
	    send_to_char("-=^=-=^=-=^=-=^=-=^=-   Skyblade Magics   -=^=-=^=-=^=-=^=-=^=-#n\n\r",ch);
       send_to_char("| Magics            Learned             Improved              |#n\n\r",ch);
   if (ch->pcdata->powers[SKYBLADE_MAGIC] > 0) {
       if (ch->pcdata->powers[SKYBLADE_MAGIC] > 0)
       send_to_char("| Skyforge            Yes                  No                 |#n\n\r",ch);
   else if (ch->pcdata->powers[SKYBLADE_MAGIC] > 1)
       send_to_char("| Skyforge            Yes                  Yes                |#n\n\r",ch);
       if (ch->pcdata->powers[SKYBLADE_MAGIC] > 2){
       send_to_char("| Skyforge            Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HeavenlyRain        Yes                  No                 |#n\n\r",ch);
}
   else if (ch->pcdata->powers[SKYBLADE_MAGIC] > 3){
	    send_to_char("| Skyforge            Yes                  Yes                |#n\n\r",ch);
   	 send_to_char("| HeavenlyRain        Yes                  Yes                |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_MAGIC] > 4){
       send_to_char("| Skyforge            Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HeavenlyRain        Yes                  Yes                |#n\n\r",ch);
       send_to_char("| Climax              Yes                  No                 |#n\n\r",ch);
}
   else if (ch->pcdata->powers[SKYBLADE_MAGIC] > 5){
       send_to_char("| Skyforge            Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HeavenlyRain        Yes                  Yes                |#n\n\r",ch);
   	 send_to_char("| Climax              Yes                  Yes                |#n\n\r",ch);
}
          if (ch->pcdata->powers[SKYBLADE_MAGIC] > 6){
       send_to_char("| Skyforge            Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HeavenlyRain        Yes                  Yes                |#n\n\r",ch);
       send_to_char("| Climax              Yes                  Yes                |#n\n\r",ch);
       send_to_char("| DragonAura          Yes                  No                 |#n\n\r",ch);
}
   else if (ch->pcdata->powers[SKYBLADE_MAGIC] > 7){
       send_to_char("| Skyforge            Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HeavenlyRain        Yes                  Yes                |#n\n\r",ch);
       send_to_char("| Climax              Yes                  Yes                |#n\n\r",ch);
   	 send_to_char("| DragonAura          Yes                  Yes                |#n\n\r",ch);
}
          if (ch->pcdata->powers[SKYBLADE_MAGIC] > 8){
       send_to_char("| Skyforge            Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HeavenlyRain        Yes                  Yes                |#n\n\r",ch);
       send_to_char("| Climax              Yes                  Yes                |#n\n\r",ch);
       send_to_char("| DragonAura          Yes                  Yes                |#n\n\r",ch);
       send_to_char("| WyrmAura            Yes                  No                 |#n\n\r",ch);
}
   else if (ch->pcdata->powers[SKYBLADE_MAGIC] > 9){
       send_to_char("| Skyforge            Yes                  Yes                |#n\n\r",ch);
       send_to_char("| HeavenlyRain        Yes                  Yes                |#n\n\r",ch);
       send_to_char("| Climax              Yes                  Yes                |#n\n\r",ch);
       send_to_char("| DragonAura          Yes                  Yes                |#n\n\r",ch);
   	 send_to_char("| WyrmAura            Yes                  Yes                |#n\n\r",ch);}
}
   else{
   	 send_to_char("| Skyforge            No                   No                 |#n\n\r",ch);
		 send_to_char("| HeavenlyRain        No                   No                 |#n\n\r",ch);
		 send_to_char("| Climax              No                   No                 |#n\n\r",ch);
		 send_to_char("| DragonAura          No                   No                 |#n\n\r",ch);
	 	 send_to_char("| WyrmAura            No                   No                 |#n\n\r",ch);}
   	 send_to_char("-=^=-=^=-=^=-=^=-=^=-=^=-=^=-=^=-^-=^=-=^=-=^=-=^=-=^=-=^=-=^=-#n\n\r",ch);
	   */
	 xprintf(buf, "You have %d magic.\n\r",ch->pcdata->powers[SKYBLADE_MAGIC]);
	 stc(buf, ch);
	 return;
	}
		if (!str_cmp(arg1,"skills"))
	{
	    send_to_char("-=^=-=^=-=^=-=^=-=^=-   Skyblade Skills   -=^=-=^=-=^=-=^=-=^=-#n\n\r",ch);
       send_to_char("| Skills                                Learned               |#n\n\r",ch);
   if (ch->pcdata->powers[SKYBLADE_SKILLS] > 0) {
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 1){
       send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   No                 |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           No                 |#n\n\r",ch);
       send_to_char("| RapierStrike                             No                 |#n\n\r",ch);
       send_to_char("| Retort                                   No                 |#n\n\r",ch);
   	 send_to_char("| LeapOfFaith                              No                 |#n\n\r",ch);
		 send_to_char("| DragonCharge                             No                 |#n\n\r",ch);
		 send_to_char("| DragonX                                  No                 |#n\n\r",ch);
		 send_to_char("| WyrmRoar                                 No                 |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 2){
       send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   Yes                |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           No                 |#n\n\r",ch);
       send_to_char("| RapierStrike                             No                 |#n\n\r",ch);
       send_to_char("| Retort                                   No                 |#n\n\r",ch);
   	 send_to_char("| LeapOfFaith                              No                 |#n\n\r",ch);
		 send_to_char("| DragonCharge                             No                 |#n\n\r",ch);
		 send_to_char("| DragonX                                  No                 |#n\n\r",ch);
		 send_to_char("| WyrmRoar                                 No                 |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 3){
       send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   Yes                |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           Yes                |#n\n\r",ch);
       send_to_char("| RapierStrike                             No                 |#n\n\r",ch);
       send_to_char("| Retort                                   No                 |#n\n\r",ch);
   	 send_to_char("| LeapOfFaith                              No                 |#n\n\r",ch);
		 send_to_char("| DragonCharge                             No                 |#n\n\r",ch);
		 send_to_char("| DragonX                                  No                 |#n\n\r",ch);
		 send_to_char("| WyrmRoar                                 No                 |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 4){
       send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   Yes                |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           Yes                |#n\n\r",ch);
   	 send_to_char("| RapierStrike                             Yes                |#n\n\r",ch);
       send_to_char("| Retort                                   No                 |#n\n\r",ch);
   	 send_to_char("| LeapOfFaith                              No                 |#n\n\r",ch);
		 send_to_char("| DragonCharge                             No                 |#n\n\r",ch);
		 send_to_char("| DragonX                                  No                 |#n\n\r",ch);
		 send_to_char("| WyrmRoar                                 No                 |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 5){
       send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   Yes                |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           Yes                |#n\n\r",ch);
       send_to_char("| RapierStrike                             Yes                |#n\n\r",ch);
       send_to_char("| Retort                                   Yes                |#n\n\r",ch);
   	 send_to_char("| LeapOfFaith                              No                 |#n\n\r",ch);
		 send_to_char("| DragonCharge                             No                 |#n\n\r",ch);
		 send_to_char("| DragonX                                  No                 |#n\n\r",ch);
		 send_to_char("| WyrmRoar                                 No                 |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 6){
       send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   Yes                |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           Yes                |#n\n\r",ch);
       send_to_char("| RapierStrike                             Yes                |#n\n\r",ch);
       send_to_char("| Retort                                   Yes                |#n\n\r",ch);
   	 send_to_char("| LeapOfFaith                              Yes                |#n\n\r",ch);
		 send_to_char("| DragonCharge                             No                 |#n\n\r",ch);
		 send_to_char("| DragonX                                  No                 |#n\n\r",ch);
		 send_to_char("| WyrmRoar                                 No                 |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 7){
       send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   Yes                |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           Yes                |#n\n\r",ch);
       send_to_char("| RapierStrike                             Yes                |#n\n\r",ch);
       send_to_char("| Retort                                   Yes                |#n\n\r",ch);
       send_to_char("| LeapOfFaith                              Yes                |#n\n\r",ch);
       send_to_char("| DragonCharge                             Yes                |#n\n\r",ch);
		 send_to_char("| DragonX                                  No                 |#n\n\r",ch);
		 send_to_char("| WyrmRoar                                 No                 |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 8){
       send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   Yes                |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           Yes                |#n\n\r",ch);
       send_to_char("| RapierStrike                             Yes                |#n\n\r",ch);
       send_to_char("| Retort                                   Yes                |#n\n\r",ch);
       send_to_char("| LeapOfFaith                              Yes                |#n\n\r",ch);
       send_to_char("| DragonCharge                             Yes                |#n\n\r",ch);
       send_to_char("| DragonX                                  Yes                |#n\n\r",ch);
		 send_to_char("| WyrmRoar                                 No                 |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 9){
       send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   Yes                |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           Yes                |#n\n\r",ch);
       send_to_char("| RapierStrike                             Yes                |#n\n\r",ch);
       send_to_char("| Retort                                   Yes                |#n\n\r",ch);
       send_to_char("| LeapOfFaith                              Yes                |#n\n\r",ch);
       send_to_char("| DragonCharge                             Yes                |#n\n\r",ch);
       send_to_char("| DragonX                                  Yes                |#n\n\r",ch);
       send_to_char("| WyrmRoar                                 Yes                |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);
}
       if (ch->pcdata->powers[SKYBLADE_SKILLS] == 10){
		 send_to_char("| BladeJump                                Yes                |#n\n\r",ch);
       send_to_char("| Pummel                                   Yes                |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           Yes                |#n\n\r",ch);
       send_to_char("| RapierStrike                             Yes                |#n\n\r",ch);
       send_to_char("| Retort                                   Yes                |#n\n\r",ch);
       send_to_char("| LeapOfFaith                              Yes                |#n\n\r",ch);
       send_to_char("| DragonCharge                             Yes                |#n\n\r",ch);
       send_to_char("| DragonX                                  Yes                |#n\n\r",ch);
       send_to_char("| WyrmRoar                                 Yes                |#n\n\r",ch);
   	 send_to_char("| DiveStrike                               Yes                |#n\n\r",ch);}
}

   else{
       send_to_char("| BladeJump                                No                 |#n\n\r",ch);
       send_to_char("| Pummel                                   No                 |#n\n\r",ch);
       send_to_char("| CloudsBlessing                           No                 |#n\n\r",ch);
       send_to_char("| RapierStrike                             No                 |#n\n\r",ch);
       send_to_char("| Retort                                   No                 |#n\n\r",ch);
   	 send_to_char("| LeapOfFaith                              No                 |#n\n\r",ch);
		 send_to_char("| DragonCharge                             No                 |#n\n\r",ch);
		 send_to_char("| DragonX                                  No                 |#n\n\r",ch);
		 send_to_char("| WyrmRoar                                 No                 |#n\n\r",ch);
	 	 send_to_char("| DiveStrike                               No                 |#n\n\r",ch);}
   	 send_to_char("-=^=-=^=-=^=-=^=-=^=-=^=-=^=-=^=-^-=^=-=^=-=^=-=^=-=^=-=^=-=^=-#n\n\r",ch);
	    return;
	}
}/*else {
	xprintf(buf,"Abilities: #LPower (%d), #nMagic (%d), #CSkills (%d).#n\n\r",
	ch->pcdata->powers[SKYBLADE_POWER], ch->pcdata->powers[SKYBLADE_MAGIC],
	ch->pcdata->powers[SKYBLADE_SKILLS]);
	send_to_char(buf,ch);
	return;
    }*/
    if (!str_cmp(arg2,"gain"))
    {
	int improve;
	int cost;
	int max;

	     if (!str_cmp(arg1,"power"   )) {improve = SKYBLADE_POWER;   max=10;}
	else if (!str_cmp(arg1,"magic"   )) {improve = SKYBLADE_MAGIC;   max=10;}
	else if (!str_cmp(arg1,"skills"   )) {improve = SKYBLADE_SKILLS; max=10;}
	else
	{
	    send_to_char("Abilities: Power, Magic, Skills.\n\r",ch);
	    return;
	}
	cost = (ch->pcdata->powers[improve]+1) * 15;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    xprintf(buf,"You have mastered all the abilites in the field of %s.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->practice )
	{
	    xprintf(buf,"It costs you %d primal to improve that ability.\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	xprintf(buf,"You improve your mastery of %s abilities.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve an ability, type: abilities <ability type> gain.\n\r",ch);
	return;
}
void do_bladejump( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  if ( IS_NPC(ch) )
    return;

  argument = one_argument (argument, arg);

  if (!IS_CLASS(ch, CLASS_SKYBLADE) )
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    send_to_char( "BladeJump to whom?\n\r", ch );
    return;
  }

  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "You cannot find them.\n\r", ch );
    return;
  }

  if( ch->move < 1000)
  {
    stc( "You don't have enough movement points to bladejump to them.\n\r",ch);
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
    if (IS_SET(victim->act, ACT_NOTRAVEL))
  {
  	send_to_char("No Can Do.\n\r", ch);
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
  if (!IS_NPC(victim) && ch->move < victim->max_move)
  {
    if ( !IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    {
      send_to_char( "They are hiding from you.\n\r", ch );
      return;
    }
    else
    send_to_char("You need more move than you're opponent.\n\r",ch);
  }
  if (victim->in_room == ch->in_room)
  {
    send_to_char("But you're already there!\n\r",ch);
    return;
  }

  act("You jump into the clouds", ch, NULL, victim, TO_CHAR);
  act("$n jumps into the clouds", ch, NULL, victim, TO_ROOM);
  char_from_room(ch);
  char_to_room(ch,victim->in_room);
  ch->move -= 1000;
  act("$n jumps out of the clouds and lands infront of $N.", ch, NULL, victim, TO_NOTVICT);
  act("$n jumps out of the clouds and lands infront of you.", ch, NULL, victim, TO_VICT);
  do_look(ch,"auto");
  return;
}

void do_eaglestrength(  CHAR_DATA *ch, char *argument  )
{
    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_SKYBLADE) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

  if ( IS_CLASS(ch, CLASS_SKYBLADE) )
  {
   if (ch->pcdata->powers[SKYBLADE_POWER] < 3)
   {
   send_to_char("You don't have that ability yet.\n\r",ch);
   return;
   }
	if ( ch->move < 1000 )
	{
		send_to_char("You don't have enough movement to gain the strength of an eagle.\n\r", ch);
		return;
	}
   if (!IS_SET(ch->newbits2, NEW2_EAGLES))
   {
   SET_BIT(ch->newbits2, NEW2_EAGLES);
	send_to_char("You flex your arms and legs as you gain the strength of an eagle.\n\r",ch);
	act("$n's legs and arms grow as they gain the strength of an eagle.",ch,NULL,NULL,TO_ROOM);
   if (ch->pcdata->powers[SKYBLADE_POWER] <= 3)
   ch->damroll += 75;
   if (ch->pcdata->powers[SKYBLADE_POWER] >= 4)
   {
	  ch->damroll += 250;
   }
   ch->move -= 1000;
	WAIT_STATE(ch,12);
	return;
  }
	else if (IS_SET(ch->newbits2, NEW2_EAGLES)){
    REMOVE_BIT(ch->newbits2, NEW2_EAGLES);
    send_to_char("Your arms and legs lose the strength of an eagle.\n\r",ch);
    act("$n's legs and arms shrink as they lose the strength of an eagle.",ch,NULL,NULL,TO_ROOM);
    if (ch->pcdata->powers[SKYBLADE_POWER] <= 3) ch->damroll -= 75;
    if (ch->pcdata->powers[SKYBLADE_POWER] >= 4) ch->damroll -= 250;
    return;}
}
}
void do_hawkeyes(  CHAR_DATA *ch, char *argument  )
{
    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_SKYBLADE) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

  if ( IS_CLASS(ch, CLASS_SKYBLADE) )
  {
   if (ch->pcdata->powers[SKYBLADE_POWER] < 5)
   {
   send_to_char("You don't have that ability yet.\n\r",ch);
   return;
   }
	if ( ch->move < 1000 )
	{
		send_to_char("You don't have enough movement to gain the eyes of a hawk.\n\r", ch);
		return;
	}
   if (!IS_SET(ch->newbits2, NEW2_HAWKEYES))
   {
   SET_BIT(ch->newbits2, NEW2_HAWKEYES);
	send_to_char("Your eyes flicker as they gain the accuracy of a hawk.\n\r",ch);
	act("$n's eyes flicker as they gain the accuracy of a hawk.",ch,NULL,NULL,TO_ROOM);
   if (ch->pcdata->powers[SKYBLADE_POWER] <= 5)
   ch->hitroll += 75;
   if (ch->pcdata->powers[SKYBLADE_POWER] >= 6)
   {
	  ch->hitroll += 250;
   }
   ch->move -= 1000;
	WAIT_STATE(ch,12);
	return;
  }
    else if (IS_SET(ch->newbits2, NEW2_HAWKEYES)){
    REMOVE_BIT(ch->newbits2, NEW2_HAWKEYES);
    send_to_char("Your eyes flicker as they lose the accuracy of a hawk.\n\r",ch);
    act("$n's eyes flicker as they lose the accuracy of a hawk.",ch,NULL,NULL,TO_ROOM);
    if (ch->pcdata->powers[SKYBLADE_POWER] <= 5) ch->hitroll -= 75;
    if (ch->pcdata->powers[SKYBLADE_POWER] >= 6) ch->hitroll -= 250;
    return;}
}}
void do_vulturevigor(  CHAR_DATA *ch, char *argument  )
{
    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_SKYBLADE) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

  if ( IS_CLASS(ch, CLASS_SKYBLADE) )
  {
   if (ch->pcdata->powers[SKYBLADE_POWER] < 7)
   {
   send_to_char("You don't have that ability yet.\n\r",ch);
   return;
   }
	if ( ch->move < 1000 )
	{
		send_to_char("You don't have enough movement to gain the vigor of a vulture.\n\r", ch);
		return;
	}
   if (!IS_SET(ch->newbits2, NEW2_VVIGOR))
   {
   SET_BIT(ch->newbits2, NEW2_VVIGOR);
	send_to_char("Your body grows as it gains the vigor of the vulture.\n\r",ch);
	act("$n's body grows as it gains the vigor of the vulture.",ch,NULL,NULL,TO_ROOM);
   if (ch->pcdata->powers[SKYBLADE_POWER] <= 7)
   ch->armor -= 75;
   if (ch->pcdata->powers[SKYBLADE_POWER] >= 8)
   {
	  ch->armor -= 250;
   }
   ch->move -= 1000;
	WAIT_STATE(ch,12);
	return;
  }
	else if (IS_SET(ch->newbits2, NEW2_VVIGOR)){
    REMOVE_BIT(ch->newbits2, NEW2_VVIGOR);
    send_to_char("Your body shrinks as it loses the vigor of the vulture.\n\r",ch);
    act("$n's body shrinks as it loses the vigor of the vulture.",ch,NULL,NULL,TO_ROOM);
    if (ch->pcdata->powers[SKYBLADE_POWER] <= 7) ch->armor += 75;
    if (ch->pcdata->powers[SKYBLADE_POWER] >= 8) ch->armor += 250;
    return;}
}}
void do_cloudsblessing(  CHAR_DATA *ch, char *argument  )
{
    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_SKYBLADE) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

  if ( IS_CLASS(ch, CLASS_SKYBLADE) )
  {
   if (ch->pcdata->powers[SKYBLADE_SKILLS] < 3)
   {
   send_to_char("You don't have that ability yet.\n\r",ch);
   return;
   }
	if ( ch->move < 5000 )
	{
		send_to_char("You don't have enough movement to gain the a cloud blessing.\n\r", ch);
		return;
	}
   if (!IS_SET(ch->newbits, NEW_CLOUDBLESS))
   {
   SET_BIT(ch->newbits, NEW_CLOUDBLESS);
	send_to_char("You are blessed by the clouds in the sky.\n\r",ch);
	act("$n's eyes twinkle as the clouds roll in.",ch,NULL,NULL,TO_ROOM);
   ch->damroll += 50;
   ch->hitroll += 50;
   ch->armor   -= 50;
   ch->move -= 5000;
	WAIT_STATE(ch,12);
	return;
  }
	else if (IS_SET(ch->newbits, NEW_CLOUDBLESS)){
    REMOVE_BIT(ch->newbits, NEW_CLOUDBLESS);
    send_to_char("Your blessing is removed as the clouds leave the sky.\n\r",ch);
    act("$n's eyes lose their twinkle as the clouds leave.",ch,NULL,NULL,TO_ROOM);
    ch->damroll -= 50;
    ch->hitroll -= 50;
    ch->armor   += 50;
    return;}
}}
void do_pummel( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;

   if (IS_NPC(ch)) return;
   if (!IS_CLASS(ch, CLASS_SKYBLADE) )
   {
     send_to_char("Huh?\n\r",ch);
     return;
   }
   if (ch->pcdata->powers[SKYBLADE_SKILLS] < 2)
   {
     send_to_char("You haven't mastered the skills abilities enough.\n\r",ch);
     return;
   }
   if (!TIME_UP(ch, TIMER_PUMMEL))
   {
     send_to_char("You are too tired from the last time.\n\r",ch);
     return;
   }
   if ((victim = ch->fighting) == NULL)
   {
     send_to_char("You aren't fighting anyone though.\n\r",ch);
     return;
   }
   if (IS_NPC(victim))
   {
      send_to_char("They aren't stanced though.\n\r",ch);
      return;
   }
   SET_TIMER(ch, TIMER_PUMMEL, 5);
   do_stance(victim, "");
   WAIT_STATE(victim,12);
   return;
}

void do_skyforge( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int vnum = 0;



    if (IS_NPC(ch)) return;

   	if(!IS_IMMORTAL(ch))
	{
    if (!IS_CLASS(ch, CLASS_SKYBLADE))
    {
        send_to_char("What?\n\r",ch);
        return;
    }
	}

    if (ch->pcdata->powers[SKYBLADE_MAGIC] < 1)
    {
    		send_to_char("You haven't mastered enough of the magic ability yet.\n\r",ch);
         return;
    }
    if ( ch->practice < 250 )
    {
        send_to_char("You don't have the 250 primal needed.\n\r",ch);
        return;
    }
    if (ch->pcdata->powers[SKYBLADE_MAGIC] == 1) vnum = 33178;
    if (ch->pcdata->powers[SKYBLADE_MAGIC] >= 2) vnum = 33179;
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
    send_to_char("Missing object, please inform a God .\n\r",ch);
    return;
    }

    if( !IS_IMMORTAL(ch) )
	{
		ch->practice -= 250;
	}
    obj = create_object(pObjIndex, 50);
    SET_BIT(obj->spectype, SITEM_SKYBLADE);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj->ownerid = ch->pcdata->playerid;
    obj_to_char(obj, ch);
    act("$p falls from the heavens and lands before you.",ch,obj,NULL,TO_CHAR);
    act("$p falls from the heavens and lands before $n.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_wyrmroar(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  one_argument (argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SKYBLADE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SKYBLADE_SKILLS] < 9)
  {
    send_to_char("You have not mastered the skills ability enough yet.\n\r",ch);
    return;
  }
  if (!TIME_UP(ch, TIMER_WYRM_ROAR))
  {
    send_to_char("Your throat is still sore from the last time.\n\r",ch);
    return;
  }
  if (( victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    if (ch->fighting == NULL)
    {
    send_to_char("Yes release the roar of the wyrm, but at who?\n\r",ch);
    return;
    }
    else victim = ch->fighting;
  }
  if (victim == ch)
  {
    send_to_char("Quit that stupid ass.\n\r",ch);
    return;
  }
  if (is_safe(ch, victim)) return;
  SET_TIMER(ch, TIMER_WYRM_ROAR, 6);
  WAIT_STATE(victim, 18);
  act("$n roars with the power of the wyrm, stunning you in place.",ch,NULL,victim,TO_VICT);
  act("$n roars with the power of the wyrm, stunning $N in place.", ch, NULL, victim, TO_NOTVICT);
  act("You roar with the power of the wyrm, stunning $N in place.", ch, NULL, victim, TO_CHAR);
  return;
}
void do_divestrike ( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument (argument, arg);
        if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_SKYBLADE))
    {
            send_to_char("Huh?\n\r",ch);
            return;
    }

        if (ch->pcdata->powers[SKYBLADE_SKILLS] < 10)
        {
        send_to_char("You have not mastered the ability yet.\n\r",ch);
        return;
        }

        if ( arg[0] == '\0' )
    {
        send_to_char( "Divestrike on whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch ); return;
    }

    if ( victim == ch )
    {
        send_to_char( "#CWhatever#n\n\r", ch );
        return;
    }
    if (ch->move < 10000)
    {
        send_to_char( "#CYou need 10000 move to use this!#n\n\r", ch );
        return;
    }

    if (IS_NPC(victim))
        {

    send_to_char("#LYou jump down from the #Wclouds#L and strike down upon your opponent.#n\n\r",ch);
        act("#C$n #Ljumps down from the #Wclouds #Land strikes $s opponent with $s rapier.#n\n\r",ch,NULL,victim,TO_ROOM);
        WAIT_STATE( ch, 3 * PULSE_VIOLENCE );
        multi_hit( ch, victim, TYPE_UNDEFINED );
        multi_hit( ch, victim, TYPE_UNDEFINED );
        multi_hit( ch, victim, TYPE_UNDEFINED );
        }
    send_to_char("#LYou jump from the heavens down upon your opponent.#n\n\r",ch);
    act("#C$n #Ljumps down from the heavens and strikes upon you with $s rapier.#n\n\r",ch,NULL,victim,TO_VICT);
    ch->move -= 10000;
    ch->hitroll += 5000;
    WAIT_STATE( ch, 3 * PULSE_VIOLENCE);
    multi_hit( ch, victim, TYPE_UNDEFINED );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    ch->hitroll -= 5000;
    return;
}
void do_dragonx (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg1[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int dam;

  argument = one_argument(argument, arg1);

  if (IS_NPC(ch)) return;

  if (!IS_CLASS(ch, CLASS_SKYBLADE))
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SKYBLADE_SKILLS] < 8)
  {
    send_to_char("You haven't mastered that skill yet.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SKYBLADE_DTIMER] != 10)
  {
    send_to_char("Your dragon power has not fully charged.\n\r",ch);
    return;
  }
  if (( victim = get_char_room(ch, NULL, arg1)) == NULL)
  {
    if (( victim = ch->fighting) == NULL)
    {
      send_to_char("Use the power of the dragon on whom?\n\r",ch);
      return;
    }
    else victim = ch->fighting;
  }
  if (is_safe(ch, victim))
  {
    send_to_char("They are safe from such a hostile action.\n\r",ch);
    return;
  }
  if (ch->mana < 10000)
  {
    send_to_char("You do not have the 10000 mana required to call on the dragon.\n\r",ch);
    return;
  }
  dam = victim->hit/3;
  if (IS_NPC(victim)) dam = victim->hit * .9;
  if (ch->pcdata->powers[SKYBLADE_MAGIC] >= 5) dam *= 1.5;
  if (IS_NPC(victim))
  {
    if (dam > 15000) dam = 5000 + (number_range(1,5) * number_range(1,7));
  }
  else
  {
    if (dam > 6000) dam = 6000 + (number_range(1,5) * number_range(1,7));
  }
  do_say(ch, "#LMighty dragon give me your POWER!!!!!!#n");
  act("#C$n #Lleaps high into the air.......#n\n\r",ch,NULL,victim,TO_ROOM);
  stc("#LYou leap high into the air!#n\n\r",ch);
  act("#0The #ylightning crackles#0 as a large dragon is seen in the horizon.#n\n\r",ch,NULL,victim,TO_ROOM);
  WAIT_STATE(ch, 6);
  WAIT_STATE(victim, 6);
  stc("#LYou charge down upon your victim riding a huge dragon!#n\n\r",ch);
  act("#C$n #0charges down upon you, riding a huge dragon!#n\n\r",ch,NULL,victim,TO_VICT);
  act("#C$n #0charges down upon $N, riding a huge dragon!#n\n\r",ch,NULL,victim,TO_NOTVICT);
  ch->mana -= 10000;
  ch->pcdata->powers[SKYBLADE_DTIMER] = 0;
  WAIT_STATE(ch, 12);
  WAIT_STATE(victim, 12);
  xprintf(buf, "#LYou return to the ground and your dragon fades away.#W[%d]\n\r",dam);
  stc(buf,ch);
  xprintf(buf, "#L%s returns to the ground and the dragon fades away.#W[%d]\n\r",ch->name,dam);
  stc(buf, victim);
  hurt_person(ch, victim, dam);
  WAIT_STATE(ch, 8);
  if (ch->fighting == NULL) ch->fighting = victim;
  return;
}
void do_heavenlyrain(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument(argument, arg1);

  if (IS_NPC(ch)) return;

  if (!IS_CLASS(ch, CLASS_SKYBLADE))
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SKYBLADE_MAGIC] < 3)
  {
    send_to_char("You haven't mastered that magic yet.\n\r",ch);
    return;
  }
  if (( victim = get_char_room(ch, NULL, arg1)) == NULL)
  {
    if (( victim = ch->fighting) == NULL)
    {
      send_to_char("Call heavenlyrain upon whom?\n\r",ch);
      return;
    }
    else victim = ch->fighting;
  }
  if (is_safe(ch, victim))
  {
    send_to_char("They are safe from such a hostile action.\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You do not have the 5000 mana required to call on the dragon.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SKYBLADE_MAGIC] >= 4) dam = number_range(5000, 7000);
  else dam = number_range(3000, 5000);
  if (!IS_NPC(victim) && victim->alignment == 1000) dam *= .5;
  if (IS_NPC(victim)) dam += victim->hit/10;
  xprintf(buf, "#CYou call upon a heavenly rain to harm your opponent!#W[%d]\n\r",dam);
  stc(buf,ch);
  xprintf(buf, "#C%s calls upon a heavenly rain to harm you!#W[%d]\n\r",ch->name,dam);
  stc(buf,victim);
  act("#C$n calls upon a heavenly rain to harm $N!#n\n\r",ch,NULL,victim,TO_NOTVICT);
  ch->mana -= 5000;
  if (number_range(1,4) == 4){
  if (ch->pcdata->powers[SKYBLADE_MAGIC] >= 4)
  ch->hit += dam;
  else
  ch->hit += dam/2;
  stc ("#LYour heavenly rain has healed you.#n\n\r",ch);}
  hurt_person(ch, victim, dam);
  if (victim->hit < -10) victim->hit = -10;
  WAIT_STATE(ch, 8);
  if (ch->fighting == NULL) ch->fighting = victim;
  return;
}

void do_skyarmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SKYBLADE))
  {
      send_to_char("What?\n\r",ch);
      return;
  }
  
  if (ch->pcdata->powers[SKYBLADE_MAGIC] < 2)
    {
    		send_to_char("You haven't mastered enough of the magic ability yet.\n\r",ch);
         return;
    }
  
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of Skyblade armor you wish to make: Ring Collar Plate Helmet "
                 "Leggings Boots Gauntlets Sleeves Cape Belt Bracer Mask.\n\r",ch);
    return;
  }

    if ( ch->practice < 100 )
    {
        send_to_char("It costs 100 points of primal to create Skyblade equipment.\n\r",ch);
        return;
    }

    if      (!str_cmp(arg,"ring"	)) vnum = 33314;    
    else if (!str_cmp(arg,"plate"	)) vnum = 33317;
    else if (!str_cmp(arg,"helmet"	)) vnum = 33318;
    else if (!str_cmp(arg,"collar"	)) vnum = 33316;
    else if (!str_cmp(arg,"leggings" )) vnum = 33325;
    else if (!str_cmp(arg,"boots" )) vnum = 33322;
    else if (!str_cmp(arg,"gauntlets" )) vnum = 33323;
    else if (!str_cmp(arg,"sleeves" )) vnum = 33324;
    else if (!str_cmp(arg,"cape" )) vnum = 33319;
    else if (!str_cmp(arg,"belt" )) vnum = 33320;
    else if (!str_cmp(arg,"bracer" )) vnum = 33315;
    else if (!str_cmp(arg,"mask" )) vnum = 33321;
    else
    {
    do_skyarmor(ch,"");
    return;
    }
    
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
    send_to_char("Missing object, please inform a God.\n\r",ch);
    return;
    }

    ch->practice -= 100;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj->ownerid = ch->pcdata->playerid;
    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}
