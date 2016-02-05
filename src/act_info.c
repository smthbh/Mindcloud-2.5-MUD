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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "olc.h"


char *	const	where_name	[] =
{
    "#R[#CLight#R         ]#n ",
    "#R[#COn Finger     #R]#n ",
    "#R[#COn Finger     #R]#n ",
    "#R[#CAround Neck   #R]#n ",
    "#R[#CAround Neck   #R]#n ",
    "#R[#COn Body       #R]#n ",
    "#R[#COn Head       #R]#n ",
    "#R[#COn Legs       #R]#n ",
    "#R[#COn Feet       #R]#n ",
    "#R[#COn Hands      #R]#n ",
    "#R[#COn Arms       #R]#n ",
    "#R[#COff Hand      #R]#n ",
    "#R[#CAround Body   #R]#n ",
    "#R[#CAround Waist  #R]#n ",
    "#R[#CAround Wrist  #R]#n ",
    "#R[#CAround Wrist  #R]#n ",
    "#R[#CRight Hand    #R]#n ",
    "#R[#CLeft Hand     #R]#n ",
    "#R[#CThird Hand    #R]#n ",
    "#R[#CFourth Hand   #R]#n ",
    "#R[#COn Face       #R]#n ",
    "#R[#CLeft Scabbard #R]#n ",
    "#R[#CRight Scabbard#R]#n ",
    "#R[#CSpecial       #R]#n ",
    "#R[#CFloating      #R]#n ",
    "#R[#CMedal         #R]#n ",
    "#R[#CBodyart       #R]#n ",
};

const char *exitname[6] = {
	"#Cn#0orth#n",
	"#Ce#0ast#n",
	"#Cs#0outh#n",
	"#Cw#0est#n",
	"#Cu#0p#n",
	"#Cd#0own#n"
};

const char *exitname2[6] = {
	"to the north",
	"to the east",
	"to the south",
	"to the west",
	"above you",
	"below you"
};


/*
 * Local functions.
 */
char *format_obj_to_char   args (( OBJ_DATA * obj, CHAR_DATA * ch, bool fShort ));
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
void	evil_eye		args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void 	check_left_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_left_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

void	obj_score		args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );


char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';
    if ( IS_SET(obj->quest, QUEST_ARTIFACT)) strcat(buf, "#w(#BArtifact#w)#n ");
    else if ( IS_SET(obj->quest, QUEST_PRIZE)) strcat(buf, "#Y(#CPrize#Y)#n ");
    else if ( IS_SET(obj->quest, QUEST_RELIC)) strcat(buf,"#Y(#wRelic#Y)#n " );
    else if ( obj->points < 750 && obj->points != 0) strcat(buf,"#Y(Legendary)#n " );
    else if ( obj->points < 1250 && obj->points != 0 ) strcat(buf, "#w(#GMythical#w)#n " );
    else if ( obj->points != 0) strcat(buf, "#C(#YPriceless#C)#n " );
    if ( IS_OBJ_STAT(obj, ITEM_ELECENCHANT))  strcat( buf, "#0<#oS#ypa#7rk#yin#og#0>#n " );
    else if ( IS_OBJ_STAT(obj, ITEM_ICEENCHANT))  strcat( buf, "#0<#W*#cF#Cr#7oz#Ce#cn#W*#0>#n " );
    else if ( IS_OBJ_STAT(obj, ITEM_FIREENCHANT))  strcat( buf, "#0<#rF#Rl#ya#7m#yi#Rn#rg#0>#n " );
    if ( IS_SET(obj->quest, QUEST_WWPOISON))  strcat( buf, "#y(#gP#GoI#7s#GOnE#gd#y)#n " );	
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)) strcat(buf,"#Y(#rGlow#Y)#n ");
    if ( IS_OBJ_STAT(obj, ITEM_HUM))  strcat(buf,"#Y(#rHum#Y)#n ");
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "#C(Invis)#n " );
    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "#B(Blue Aura)#n "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "#R(Red Aura)#n "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "#Y(Yellow Aura)#n "  );
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "#B(Magical)#n " );
        if (IS_SET(obj->spectype, SITEM_COPPER))  strcat(buf,"#r(Copper)#n ");
    if (IS_SET(obj->spectype, SITEM_IRON))  strcat(buf,"#c(Iron)#n ");
    if (IS_SET(obj->spectype, SITEM_STEEL))  strcat(buf,"#C(Steel)#n ");
    if (IS_SET(obj->spectype, SITEM_ADAMANTITE))  strcat(buf,"#0(#CAdamantite#0)#n ");
    if (IS_SET(obj->spectype, SITEM_MITHRIL))  strcat(buf,"#0(#wMithril#0)#n ");
    if (IS_SET(obj->spectype, SITEM_HILT))  strcat(buf,"#P(Hilted)#n ");
    if (IS_SET(obj->spectype, SITEM_GEMSTONE)) strcat(buf,"#C(#YGemstoned#C)#n ");
    if ( IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	!IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf,
"#0(Shadowplane)#n " );
    if (!IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	 IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf, "#w(Normal plane)#n " );

    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
    if (obj->condition < 100)
      strcat(buf, " #r(Damaged)#n");
    }
    else
    {
	if ( obj->description != NULL )
	    strcat( buf, obj->description );
    }

    return buf;
}

int char_damroll(CHAR_DATA *ch)
{
  int value = 0;

  value += ch->damroll;

  if (!IS_NPC(ch)) 
  {
    value += get_curr_str(ch) / 2;
  }
  if( ch-> level < LEVEL_AVATAR ) return value;
  if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->rage > 0)
    value += ch->rage;
    
  if (IS_CLASS(ch, CLASS_VAMPIRE))
	{
	  if(ch->played > 0)
      {
      	if((ch->played / 3600) < 50)
      	{
      	value += ((ch->played / 3600) * 6);
    	}
    	else
    	{
    	value += 1300;
    	}
      }
    }
  if (IS_CLASS(ch, CLASS_DRACONIAN))
  {
  	value +=  (75 * ch->pcdata->powers[ORB_YELLOW]);
  	value +=  (50 * ch->pcdata->powers[ORB_GREEN]);
  }
  if (IS_CLASS(ch, CLASS_SKYBLADE))
  {
	if (IS_SET(ch->newbits, NEW2_EAGLES))
	{
		if (ch->pcdata->powers[SKYBLADE_POWER] <= 3)
		value += 75;
		else if (ch->pcdata->powers[SKYBLADE_POWER] >= 4)
		value += 250;
	}
  
	if (IS_SET(ch->newbits, NEW_CLOUDBLESS))
	value += 250;    
  }
  if (IS_CLASS(ch, CLASS_GHOUL) && ch->pcdata->powers[GPOWER_SHADOW] > 3)
  value += 350;
  if( IS_CLASS(ch, CLASS_MONK) && IS_ITEMAFF(ch, ITEMA_CHAOSHANDS ) )
  	value += 450;
	if (IS_CLASS(ch, CLASS_NINJA) && ch->rage > 0)
  	value += ch->rage;
  if (IS_SET(ch->special,SPC_WOLFMAN) && ch->rage > 99)
    value += ch->rage;
  if (IS_CLASS(ch, CLASS_DEMON) && ch->rage > 0)
    value += ch->rage;

        if (IS_CLASS( ch,CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
	value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
	else if (IS_CLASS(ch, CLASS_DROW) && ch->pcdata->stats[DEMON_POWER] > 0)
	value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);

	if (!IS_NPC(ch))
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 6
	&& ch->pcdata->powers[HARA_KIRI] > 0)
	value += 150;

    if (IS_CLASS(ch, CLASS_SAMURAI) && !IS_NPC(ch))
    {
	int wpn = ch->wpn[1];
	int total = ch->wpn[0] + ch->wpn[2] + ch->wpn[3] + ch->wpn[4] + ch->wpn[5] +
		    ch->wpn[6] + ch->wpn[7] + ch->wpn[8] + ch->wpn[9] + ch->wpn[10] +
		    ch->wpn[11] + ch->wpn[12];
	
	  if (wpn >= 700)
		value += 700;
	  else
		value += wpn/1.5; 
	total = total / 12;
	value += total;
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI))
    {
      if (ch->tier > 0) value += ch->tier * 300;
    }
    if( value > 7000 ) value = 7000;
    return value;
}

int char_hitroll(CHAR_DATA *ch)
{
    int value = 0;

    value += ch->hitroll;

    if (!IS_NPC(ch))
        value += get_curr_str(ch) / 2;

    if( ch-> level < LEVEL_AVATAR ) return value;

    /* Vampire Rage */
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->rage > 0)
	  value += ch->rage;
	  
	if (IS_CLASS(ch, CLASS_VAMPIRE))
	{
	  if(ch->played > 0)
      {
      	if((ch->played / 3600) < 50)
      	{
      	value += ((ch->played / 3600) * 6);
    	}
    	else
    	{
    	value += 1300;
    	}
      }
    }
    
      if (IS_CLASS(ch, CLASS_DRACONIAN))
  {
  	value +=  (75 * ch->pcdata->powers[ORB_YELLOW]);
  	value +=  (75 * ch->pcdata->powers[ORB_GREEN]);
  }
    if (IS_CLASS(ch, CLASS_SKYBLADE))
  {
	if (IS_SET(ch->newbits, NEW2_HAWKEYES))
	{
		if (ch->pcdata->powers[SKYBLADE_POWER] <= 5)
		value += 75;
		else if (ch->pcdata->powers[SKYBLADE_POWER] >= 6)
		value += 250;
	}

	if (IS_SET(ch->newbits, NEW_CLOUDBLESS))
	value += 250;  
  }
    if (IS_CLASS(ch, CLASS_GHOUL) && ch->pcdata->powers[GPOWER_SHADOW] > 3)
    value += 350;
    
    if( IS_CLASS(ch, CLASS_MONK) && IS_ITEMAFF(ch, ITEMA_CHAOSHANDS ) )
	value += 450;


	if (IS_CLASS(ch, CLASS_NINJA) && ch->rage > 0)
	value += ch->rage;

	if (!IS_NPC(ch))
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 6
	&& ch->pcdata->powers[HARA_KIRI] > 0)
	value += 350;

    /* Werewolf Rage */
    if (IS_SET(ch->special,SPC_WOLFMAN) && ch->rage > 99)
	  value += ch->rage;

    if (IS_CLASS(ch, CLASS_DEMON) && ch->rage > 0)
	  value += ch->rage;
    if (IS_CLASS( ch,CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
       value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
        else if (IS_CLASS(ch, CLASS_DROW) && ch->pcdata->stats[DEMON_POWER] > 0)
        value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);

    /* Samurai */
    if (IS_CLASS(ch, CLASS_SAMURAI) && !IS_NPC(ch))
    {
	int wpn = ch->wpn[1];
	int total = ch->wpn[0] + ch->wpn[2] + ch->wpn[3] + ch->wpn[4] + ch->wpn[5] +
		    ch->wpn[6] + ch->wpn[7] + ch->wpn[8] + ch->wpn[9] + ch->wpn[10] +
		    ch->wpn[11] + ch->wpn[12];
	
        if (wpn >= 700)
		value += 700;
        else
		value += wpn/1.50;
	total = total / 12;
	value += total;
    }
    
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI))
    {
      if (ch->tier > 0) value += ch->tier * 300; 
    }
    if( value > 7000 ) value = 7000;
    return value;
}

int char_ac(CHAR_DATA *ch)
{
    int value = 0;

    value = GET_AC(ch);
   	
   if( IS_AWAKE(ch) )
    {
	if( get_curr_dex(ch) <= 35 )
		value += dex_app[get_curr_dex(ch)].defensive;
	else
		value -= ( 300 + get_curr_dex(ch));
   }

    /* Samurai */
    if (IS_CLASS(ch, CLASS_SAMURAI) && (get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	int wpn = ch->wpn[1] * 1.5;
	value -= wpn;
    }
    if (IS_CLASS(ch,CLASS_DRACONIAN) && ch->pcdata->stats[DRACONIAN_SPIKES] > 0)
	{
	value -= (ch->pcdata->stats[DRACONIAN_SPIKES] * 2);
    }
    if (IS_CLASS(ch, CLASS_SKYBLADE))
  {
	if (IS_SET(ch->newbits, NEW2_VVIGOR))
	{
		if (ch->pcdata->powers[SKYBLADE_POWER] <= 7)
		value -= 75;
		else if (ch->pcdata->powers[SKYBLADE_POWER] >= 8)
		value -= 250;
	}
  
	if (IS_SET(ch->newbits, NEW_CLOUDBLESS))
	value -= 250;
  }

    if( value > 7000 ) value = 7000;
    return value;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow, iShow, count;
    bool fCombine;

    if ( !ch->desc )
	return;

    /*
     * Alloc space for output lines.
     */
    count = 0;

    for ( obj = list; obj; obj = obj->next_content )
		count++;

	CREATE( prgpstrShow, char *, count );
    CREATE( prgnShow, int, count );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj; obj = obj->next_content )
    { 
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );
	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {

	if(prgpstrShow[iShow][0] == '\0')
	{
		DISPOSE( prgpstrShow[iShow] );
		continue;
	}

	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
		{
			ch_printf( ch, "#R(#W%2d#R)#n ", prgnShow[iShow] );
	    }
	    else
	    {
			send_to_char( "     #n", ch );
	    }
    }

	send_to_char( prgpstrShow[iShow], ch );
	send_to_char( "\n\r", ch );
	DISPOSE( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	    if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    send_to_char( "     ", ch );
	    send_to_char( "Nothing.\n\r", ch );
    }

    /*
     * Clean up.
     */
    DISPOSE( prgpstrShow );
    DISPOSE( prgnShow );
    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char buf4[MAX_STRING_LENGTH];
    char buf6[MAX_STRING_LENGTH];
    char buf7[MAX_STRING_LENGTH];
    char mount2[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    buf[0] = '\0';
    buf2[0] = '\0';
    buf3[0] = '\0';
    buf6[0] = '\0';


    if (!IS_NPC(victim) && victim->pcdata->chobj != NULL )
	return;

    if ((mount = victim->mount) != NULL && IS_SET(victim->mounted, IS_MOUNT))
	return;

    if ( !IS_NPC(victim) && IS_SET(victim->flag2, VAMP_OBJMASK) )
    {
	xprintf(buf, "     %s\n\r",victim->objdesc);
	stc(buf,ch);
	return;
    }


    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
	strcat( buf, "     " );
    else {
         if (!IS_NPC(ch) && IS_NPC(victim) && is_quest_target(ch, victim))
      strcat(buf, "#Y(#RQuest#Y)#n ");
        if (victim->desc && (victim->desc->editor > 0)  ) strcat( buf, "#B(#wOLC#B)#n ");
	if (!IS_NPC(victim) && victim->desc==NULL ) strcat( buf,"#0(#wLink-Dead#0)#n ");
    	if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "#B(#wInvis#B)#n "   );
    	if ( victim->invis_level >= LEVEL_IMMORTAL) strcat( buf, "#0(WWWizi#0)#n" );
    	if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "#0(#wHide#0)#n "    );
    	if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "#R(#pCharmed#R)#n " );
    	if ( IS_AFFECTED(victim, AFF_PASS_DOOR)  ||
         IS_AFFECTED(victim, AFF_ETHEREAL)    )
	 strcat( buf, "#b(Translucent)#n ");
    	if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "#0(#PPink Aura#0)#n "  );
    	if ( IS_EVIL(victim)
    	&&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "#0(#RR#red #RA#rura#0)#n "   );
    	if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "#0(#WWhite Aura#0)#n " );
        if (!IS_NPC(victim))
        {
          if (IS_SET(victim->pcdata->jflags, JFLAG_DRONEWORLD)) strcat(buf, "#c(Unreal)#n ");
        }
    }
    if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE))
	strcat( buf, "#0(#CNormal plane#0)#n "     );
    else if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE))
	strcat( buf, "#C(#0Shadowplane#C)#n "     );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim,CLASS_WEREWOLF) && IS_HERO(victim) && IS_HERO(ch))
	strcat( buf, "#Y(#BWerewolf#Y)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_DEMON) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#R(#0Demon#R)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_NINJA) && IS_HERO(victim) && IS_HERO(ch))     
        strcat( buf, "#R(#YNinja#R)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_MONK) && IS_HERO(victim) && IS_HERO(ch))     
        strcat( buf, "#C(#nMonk#C)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_CYBORG) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#D(#WCyborg#D)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_ANGEL) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#0(#wAngel#0)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_TANARRI) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#0(#RTanar'ri#0)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_LICH) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#0(#GLich#0)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#Y(#0Death Knight#Y)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_SAMURAI) && IS_HERO(victim) && IS_HERO(ch))
 	strcat( buf,"#C(#YSamu#Rrai#C)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_SKYBLADE) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#0(#CSky#WBlade#0)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_MAGE) && IS_HERO(victim) && IS_HERO(ch))
	strcat( buf,"-|#CBattlemage#n|-} " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_DRONE) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf,"#G<#0=#Y{#0Drone#Y}#0=#G>#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_GIANT) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf,"#c<>#GG#gian#Gt#c<>#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_FAE) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#G<<#R(#YFae#R)#G>>#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_DROW) && IS_HERO(victim) && IS_HERO(ch))
	strcat(buf, "#P(#0Drow#P)#n ");
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_GHOUL) && IS_HERO(victim) && IS_HERO(ch))
	strcat(buf, "#G(#cGhoul#G)#n ");
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_DRACONIAN) && IS_HERO(victim) && IS_HERO(ch))
	strcat(buf, "#y(#gDr#Gac#Wo#Gni#gan#y)#n ");
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_THIEF) && IS_HERO(victim) && IS_HERO(ch))
	strcat(buf, "#r<#R)#0Thief#R(#r>#n ");
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_SHADOW) && IS_HERO(victim) && IS_HERO(ch))
	strcat(buf, "#0-#Y>#0)#pShadow#0(#Y<#0-#n ");
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_PRIEST) && IS_HERO(victim) && IS_HERO(ch))
        strcat(buf, "#B(#WPriest#B)#n ");
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_JEDI) && IS_HERO(victim) && IS_HERO(ch))
        strcat(buf, "#g(#CJedi#g)#n ");
    if ( !IS_NPC(ch) && IS_VAMPAFF(ch,VAM_AUSPEX) && 
	 !IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) &&
	  IS_VAMPAFF(victim, VAM_DISGUISED) )
    {
	strcat( buf, "(");
	strcat( buf, victim->name);
	strcat( buf, ") ");
    }
if (victim->embracing!= NULL)
{
xprintf(buf7,"\n\r...%s is holding %s in a stern embrace!",victim->name,victim->embracing->name);
}
else if (victim->embraced!=NULL)
{
xprintf(buf7,"\n\r...%s is being embraced by %s!",victim->name,victim->embraced->name);
}
    if ( IS_EXTRA(victim, EXTRA_ROT))
     {
        if (IS_NPC(victim)) 
          xprintf(buf6,"\n\r...%s is coated with a layer of rotten skin!",victim->short_descr);
        else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
           xprintf(buf6,"\n\r...%s is coated with a layer of rotten skin!",victim->morph);
        else 
           xprintf(buf6,"\n\r...%s is coated with a layer of rotten skin!",victim->name);
       }


    if ( IS_AFFECTED(victim, AFF_FLAMING) ) 
    {
	if (IS_NPC(victim))
	    xprintf( buf2, "\n\r...%s is engulfed in blazing flames!", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    xprintf( buf2, "\n\r...%s is engulfed in blazing flames!", victim->morph );
	else
	    xprintf( buf2, "\n\r...%s is engulfed in blazing flames!", victim->name );
    }

    if ( !IS_NPC(victim) && IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
    {
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    xprintf( buf3, "...%s is gagged and blindfolded, kinky!", victim->morph );
	else if (IS_EXTRA(victim,GAGGED))
	    xprintf( buf3, "...%s is gagged!", victim->morph );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    xprintf( buf3, "...%s is blindfolded!", victim->morph );
    }
    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
    {
	strcat( buf, victim->morph );
	strcat( buf, " is lying here." );
    	strcat( buf, buf2 );
    	strcat( buf, buf3 );
    	strcat( buf, buf6 );
        strcat(buf,buf7);   
     strcat( buf, "\n\r" );
    	buf[5] = UPPER(buf[5]);
    	send_to_char( buf, ch );
	return;
    }
    
    if ( IS_EXTRA(victim, TIED_UP) ) 
    {
	if (IS_NPC(victim))
	    xprintf( buf3, "\n\r...%s is tied up", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    xprintf( buf3, "\n\r...%s is tied up", victim->morph );
	else
	    xprintf( buf3, "\n\r...%s is tied up", victim->name );
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, ", gagged and blindfolded!" );
	else if (IS_EXTRA(victim,GAGGED))
	    strcat( buf3, " and gagged!" );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, " and blindfolded!" );
	else
	    strcat( buf3, "!" );
    }

    if ( IS_AFFECTED(victim, AFF_WEBBED) ) 
    {
	if (IS_NPC(victim))
	    xprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    xprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->morph );
	else
	    xprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->name );
	strcat( buf3, buf4 );
    }

    if ( !IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH) )
	strcat( buf, victim->morph );
    else if ( victim->position == POS_STANDING && victim->long_descr[0] != '\0' && (mount = victim->mount) == NULL )
    {
	strcat( buf, victim->long_descr );
	send_to_char( buf, ch );
	MOBtrigger = FALSE;
	if ( IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF) )
	{
	    if ( IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) ) 
		act( "...$N is surrounded by a crackling shield of #Ylightning#n.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_FIRESHIELD) ) 
		act( "...$N is surrounded by a burning shield of #Rf#Yi#Rr#Ye#n.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_ICESHIELD) ) 
		act( "...$N is surrounded by a shimmering shield of #Cice#n.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) ) 
		act( "...$N is surrounded by a bubbling shield of #La#Rc#Li#Rd#n.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) ) 
		act( "...$N is surrounded by a swirling shield of #0c#Rh#0a#Ro#0s#n.", ch,NULL,victim,TO_CHAR );
	}
	MOBtrigger = TRUE;
	return;
    }
    else
    	strcat( buf, PERS( victim, ch ) );

    if ((mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
	if (IS_NPC(mount))
	    xprintf( mount2, " is here riding %s", mount->short_descr );
	else
	    xprintf( mount2, " is here riding %s", mount->name );
	strcat( buf, mount2 );
	if (victim->position == POS_FIGHTING)
	{
	    strcat( buf, ", fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	}
	else strcat( buf, "." );
    }
    else if ( victim->position == POS_STANDING && IS_AFFECTED(victim, AFF_FLYING) )
	strcat( buf, " is hovering here" );
    else if ( victim->position == POS_STANDING && (!IS_NPC(victim) && (IS_VAMPAFF(victim, VAM_FLYING))) )
	strcat( buf, " is hovering here" );
    else if (victim->position == POS_STANDING && (!IS_NPC(victim) &&
    IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)))
	strcat(buf, " is hovering here" );
	else if (victim->position == POS_STANDING && (!IS_NPC(victim)) && (IS_SET(ch->special, SPC_JEDI_LEVITATE)) && IS_CLASS(ch, CLASS_JEDI))
		strcat(buf, " is hovering here");
    else
    {
    	switch ( victim->position )
    	{
    	case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
        case POS_MORTAL:   strcat( buf, " is #Rmortally wounded#n." );   break; 
    	case POS_INCAP:    strcat( buf, " is #rincapacitated#n." );      break;
    	case POS_STUNNED:  strcat( buf, " is lying here #Cstunned#n." ); break;
    	case POS_SLEEPING: strcat( buf, " is sleeping here." );      break;
    	case POS_RESTING:  strcat( buf, " is resting here." );       break;
    	case POS_MEDITATING: strcat( buf, " is meditating here." );  break;
    	case POS_SITTING:  strcat( buf, " is sitting here." );       break;
    	case POS_STANDING:
	    if      (!IS_NPC(victim) && victim->stance[0] == STANCE_NORMAL)
		strcat( buf, " is here, crouched in a fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_VIPER)
		strcat( buf, " is here, crouched in a viper fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_CRANE)
		strcat( buf, " is here, crouched in a crane fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_CRAB)
		strcat( buf, " is here, crouched in a crab fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MONGOOSE)
		strcat( buf, " is here, crouched in a mongoose fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_BULL)
		strcat( buf, " is here, crouched in a bull fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MANTIS)
		strcat( buf, " is here, crouched in a mantis fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_DRAGON)
		strcat( buf, " is here, crouched in a dragon fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_TIGER)
		strcat( buf, " is here, crouched in a tiger fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MONKEY)
		strcat( buf, " is here, crouched in a monkey fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_SWALLOW)
		strcat( buf, " is here, crouched in a swallow fighting stance." );
	    else
		strcat( buf, " is here." );
	    break;
    	case POS_FIGHTING:
	    strcat( buf, " is here, fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	    break;
	}
    }

    strcat( buf, buf2 );
    strcat( buf, buf3 );
    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF) ) return;
    MOBtrigger = FALSE;
    if ( IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) ) 
	act( "...$N is surrounded by a crackling shield of lightning.", ch,NULL,victim,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_FIRESHIELD) ) 
	act( "...$N is surrounded by a burning shield of fire.", ch,NULL,victim,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_ICESHIELD) ) 
	act( "...$N is surrounded by a shimmering shield of ice.", ch,NULL,victim,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) ) 
	act( "...$N is surrounded by a bubbling shield of acid.", ch,NULL,victim,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) ) 
	act( "...$N is surrounded by a swirling shield of chaos.", ch,NULL,victim,TO_CHAR );
	MOBtrigger = TRUE;
    return;
}



void evil_eye( CHAR_DATA *victim, CHAR_DATA *ch )
{
  char buf [MAX_STRING_LENGTH];
  int tabletype;

  if (IS_NPC(victim)) return;
  if (victim == ch) return;
  if (ch->level > 6) return;
  if (!IS_CLASS(victim, CLASS_DRONE) && !IS_CLASS(victim, CLASS_DEMON)) return;
  if (ch->level < 3 || victim->level < 3) return;
  if (IS_CLASS(victim, CLASS_DEMON) && victim->power[DISC_DAEM_DISC] < 2) return;
  if (IS_CLASS(victim, CLASS_DEMON) && victim->power[DISC_DAEM_DISC] < dice(1, 10)) return;
  if (is_safe(ch, victim)) return;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DRONE))
  {
    if (IS_SET(ch->newbits2, NEW2_MENTALBLOCK))
    {
      send_to_char("A strange power blocks your attempt to control them.\n\r", victim);
      xprintf(buf, "You sense that %s is trying to control your actions.\n\r", victim->name);
      send_to_char(buf, ch);
      return;
    }
  }
  if (IS_CLASS(victim, CLASS_DRONE))
  {
    if (victim->pcdata->powers[DRONE_EYE] == DRONE_EYE_OFF) return;
    else if (victim->pcdata->powers[DRONE_EYE] == DRONE_EYE_LEVEL)
    {
      int templevel = victim->level;

      victim->level = MAX_LEVEL;
      xprintf(buf, "%s level", ch->name);
      do_asperson(victim, buf);
      victim->level = templevel;
      send_to_char("You feel a presence in your mind.\n\r", ch);
      return;
    }
    else if (victim->pcdata->powers[DRONE_EYE] == DRONE_EYE_READAURA)
    {
      int sn;

      if ((sn = skill_lookup("readaura")) > 0)
        (*skill_table[sn].spell_fun) (sn,50,victim,ch);
      return;
    }
    else if (victim->pcdata->powers[DRONE_EYE] == DRONE_EYE_CUSTOM)
    {
      if (strlen(victim->poweraction) < 3) return;
      interpret(ch, victim->poweraction);
      send_to_char("You feel a presence in your mind.\n\r", ch);

      xprintf(buf, "Mindseye : %s forces %s to %s", victim->name, ch->name, victim->poweraction);
      log_string(LOG_COMMAND, buf);

      return;
    }
    else return;
  }
  if (victim->powertype != NULL && strlen(victim->powertype) > 1)
  {
    xprintf(buf,"\n\r%s\n\r",victim->powertype);
    send_to_char(buf,ch);
  }
  if (IS_SET(victim->spectype,EYE_SELFACTION) && victim->poweraction != NULL)
    interpret(victim,victim->poweraction);
  if (IS_SET(victim->spectype,EYE_ACTION) && victim->poweraction != NULL)
  {
    interpret(ch,victim->poweraction);
    xprintf(buf, "Evileye : %s forces %s to %s", victim->name, ch->name, victim->poweraction);
    log_string(LOG_COMMAND, buf);
  }
  if (IS_SET(victim->spectype,EYE_SPELL) && victim->specpower > 0)
  {
    tabletype = skill_table[victim->specpower].target;
    (*skill_table[victim->specpower].spell_fun) (victim->specpower,victim->spl[tabletype],victim,ch);
  }
  return;
}


void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;

    if ( can_see( victim, ch ) )
    {
	act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
    }

    if (!IS_NPC(ch) && IS_HEAD(victim,LOST_HEAD))
    {
	act( "$N is lying here.", ch, NULL, victim, TO_CHAR );
	return;
    }
    if ( victim->description[0] != '\0' )
    {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, PERS(victim, ch) );

         if ( percent >= 100 ) strcat( buf, " is in perfect health.\n\r"  );
    else if ( percent >=  90 ) strcat( buf, " is slightly scratched.\n\r" );
    else if ( percent >=  80 ) strcat( buf, " has a few bruises.\n\r"     );
    else if ( percent >=  70 ) strcat( buf, " has some cuts.\n\r"         );
    else if ( percent >=  60 ) strcat( buf, " has several wounds.\n\r"    );
    else if ( percent >=  50 ) strcat( buf, " has many nasty wounds.\n\r" );
    else if ( percent >=  40 ) strcat( buf, " is bleeding freely.\n\r"    );
    else if ( percent >=  30 ) strcat( buf, " is covered in blood.\n\r"   );
    else if ( percent >=  20 ) strcat( buf, " is leaking guts.\n\r"       );
    else if ( percent >=  10 ) strcat( buf, " is almost dead.\n\r"        );
    else                       strcat( buf, " is DYING.\n\r"              );

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
 
    if (!IS_NPC( victim ))
    {
    	if (IS_AFFECTED(victim, AFF_INFRARED) || IS_VAMPAFF(victim, VAM_NIGHTSIGHT)) act("$N's eyes are glowing bright red.",ch,NULL,victim,TO_CHAR);
    	if (IS_AFFECTED(victim, AFF_FLYING)) act("$N is hovering in the air.",ch,NULL,victim,TO_CHAR);
		if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION))
			act("$N is hovering in the air.", ch,NULL,victim,TO_CHAR);
		if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_ARMS))
			act("$N has 6 extra spidery arms protruding from $S body.",ch,NULL,victim,TO_CHAR);
	    if (IS_VAMPAFF(victim, VAM_FANGS)) act("$N has a pair of long, pointed fangs.",ch,NULL,victim,TO_CHAR);
	    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim, VAM_CLAWS))
			act("$N has razor sharp claws protruding from under $S finger nails.",ch,NULL,victim,TO_CHAR);
		else if (IS_CLASS(victim, CLASS_NINJA) && IS_VAMPAFF(victim, VAM_CLAWS))
			act("$N has IronClaws protruding from $S wrists.",ch,NULL,victim,TO_CHAR);
	    else if (!IS_NPC(victim) && IS_VAMPAFF(victim, VAM_CLAWS))
			act("$N has razor sharp talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
	    if (!IS_NPC(victim))
	    {
			if (!IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->newbits, NEW_SKIN)) act("$N has scales all over $S body.",ch,NULL,victim,TO_CHAR);
			if (IS_VAMPAFF(victim, VAM_HORNS)) act("$N has a pair of pointed horns extending from $S head.",ch,NULL,victim,TO_CHAR);
			if (IS_VAMPAFF(victim, VAM_WINGS))
			{
	    		if (IS_DEMAFF(victim, DEM_UNFOLDED))
					act("$N has a pair of batlike wings spread out from behind $S back.",ch,NULL,victim,TO_CHAR);
	    		else
			 		act("$N has a pair of batlike wings folded behind $S back.",ch,NULL,victim,TO_CHAR);
			}
			if (IS_VAMPAFF(victim, VAM_HEAD))
			 {
			   act("$N's head resembles that of a fierce lion.",ch,NULL,victim,TO_CHAR);
			 }
		   if (IS_VAMPAFF(victim, VAM_TAIL))
				act("$N has a sharp tail extending from $S spine.",ch,NULL,victim,TO_CHAR); 
		   if (IS_VAMPAFF(victim, VAM_EXOSKELETON))
				act("$N's skin is covered by a hard exoskeleton.",ch,NULL,victim,TO_CHAR);
 
	  	}
	}
	found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    if (IS_NPC(ch) || ch->pcdata->chobj == NULL || ch->pcdata->chobj != obj)
	    {
	    	send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    	send_to_char( "\n\r", ch );
	    }
	    else
		send_to_char( "you\n\r", ch);
	}
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    &&   !IS_HEAD(victim, LOST_HEAD)
    &&   number_percent( ) < ch->pcdata->learned[gsn_peek] )
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
    }

    return;
}



void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;
	    
	if ( get_trust ( ch ) < rch->invis_level )
            continue;

        if ( get_trust ( ch ) < rch->ghost_level )
            continue;

	if ( IS_IMMORTAL(ch) || IS_IMMORTAL(rch) )
	{
	    if ( !IS_NPC(rch)
	    &&   IS_SET(rch->act, PLR_WIZINVIS)
	    &&   get_trust( ch ) < get_trust( rch ) )
		continue;
	}
	else
	{
	    if ( !IS_NPC(rch)
	    && ( IS_SET(rch->act, PLR_WIZINVIS)
	    ||   IS_ITEMAFF(rch, ITEMA_VANISH) )
	    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
	    &&   !IS_ITEMAFF(ch, ITEMA_VISION) )
		continue;
	}

	if ( !IS_NPC(rch) && IS_HEAD(rch,LOST_HEAD))
	    continue;

	if ( !IS_NPC(rch) && IS_EXTRA(rch,EXTRA_OSWITCH))
	    continue;

	if ( can_see( ch, rch ) )
	    show_char_to_char_0( rch, ch );
	else if ( room_is_dark( ch->in_room )
	&&      ( IS_AFFECTED(rch, AFF_INFRARED)||(!IS_NPC(rch)&&IS_VAMPAFF(rch, VAM_NIGHTSIGHT))))
	{
	    send_to_char( "You see glowing #Rred#n eyes watching YOU!\n\r", ch );
	}
    }

    return;
} 



bool check_blind( CHAR_DATA *ch )
{

    if ( IS_EXTRA(ch, BLINDFOLDED) )
    {
        send_to_char( "You can't see a thing through the blindfold!\n\r", ch );
        return FALSE;
    }     

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) )
	return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
    {
	send_to_char( "You have no eyes!\n\r", ch );
	return FALSE;
    }

    if ( IS_AFFECTED(ch, AFF_BLIND) && !IS_AFFECTED(ch, AFF_SHADOWSIGHT))
    {
	send_to_char( "You can't see a thing!\n\r", ch );
	return FALSE;
    }

    return TRUE;
}



void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *wizard;
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    char *pdesc;
    int door;
    bool found, partial;
    
    
    if ( ch->desc == NULL && (wizard = ch->wizard) == NULL) return;

    if (ch->in_room == NULL) return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( IS_SET(ch->flag2,AFF_TOTALBLIND))
    {
    send_to_char("You can't see anything because you're blinded!\n\r",ch);
    return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch) && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) &&
	!IS_ITEMAFF(ch, ITEMA_VISION) && !IS_IMMORTAL(ch) && !IS_CLASS(ch, CLASS_CYBORG) &&
!(IS_CLASS(ch, CLASS_DROW)))
    {
	send_to_char( "It is pitch black, yeah, really dark... \n\r", ch );
	return;
    }

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   !IS_ITEMAFF(ch, ITEMA_VISION)
    &&   !IS_VAMPAFF(ch, VAM_NIGHTSIGHT)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE)
    &&   !(ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
    &&   !IS_NPC(ch) && ch->pcdata->chobj != NULL
    &&   ch->pcdata->chobj->in_obj != NULL)
    &&   room_is_dark( ch->in_room ) )
    {
	send_to_char( "It is pitch black, yeah.. really dark\n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) || !str_cmp( arg1, "scry"))
    {
	/* 'look' or 'look auto' */
	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	    act( "$p",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
    	else if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "The shadow plane\n\r", ch );
    else if (!IS_SET(ch->act, PLR_MAP))
	{
            if (!IS_NPC(ch) && ch->level > 6)
            {
              xprintf(buf, "#B%s #0[#R%d#0][#w%s#0][#w%s#0]#n\n\r", 
              ch->in_room->name, 
              ch->in_room->vnum,
              flag_string( sector_flags, ch->in_room->sector_type ),
              flag_string( room_flags, ch->in_room->room_flags));
              send_to_char( buf, ch );
            }
            else
            {
  	      xprintf(buf, "#B%s#n\n\r", ch->in_room->name);
  	      send_to_char( buf, ch );
            }
	}

    if (!IS_NPC(ch) && IS_SET(ch->act2, PLR_AUTOEXIT) && !IS_SET(ch->act, PLR_MAP))
	    do_exits( ch, "auto" );
	if (IS_SET(ch->act, PLR_MAP) && IS_SET(ch->act, PLR_BRIEF))
	{
	    
	    if (!IS_NPC(ch) && ch->level > 6)
            {
              xprintf(buf, "#B%s #0[#R%d#0][#w%s#0][#w%s#0]#n\n\r", 
              ch->in_room->name, 
              ch->in_room->vnum,
              flag_string( sector_flags, ch->in_room->sector_type ),
              flag_string( room_flags, ch->in_room->room_flags));
              send_to_char( buf, ch );
            }
            else
            {
  	      xprintf(buf, "#B%s#n\n\r", ch->in_room->name);
  	      send_to_char( buf, ch );
            }
            do_exits( ch, "auto" );
        }

	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	{
	    act( "You are inside $p.",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
	    show_list_to_char( ch->pcdata->chobj->in_obj->contains, ch, FALSE, FALSE );
	}
	else if ( (arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) && IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "You are standing in complete darkness.\n\r", ch );
	else if ( ( !IS_NPC(ch) && !IS_SET(ch->act, PLR_BRIEF) ) &&
		( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) || !str_cmp(arg1, "scry") ) )
	{
      if (IS_SET(ch->act, PLR_MAP))
      {
        draw_map(ch, ch->in_room->description);
        }
      else
      {
            send_to_char( "#w", ch );
	    send_to_char( ch->in_room->description , ch );
	    if ( ch->in_room->sector_type != SECT_INSIDE && IS_OUTSIDE(ch))
                {
                    if (number_range(0,20) < 5 )
                    {
                        if (weather_info.sky == SKY_RAINING)
                            send_to_char("#cA cool wind howls and brings a light rain.#n\n\r",ch);
                        else if (weather_info.sky == SKY_CLOUDY)
                            send_to_char("#cA cold wind is blowing.#n\n\r",ch);
                        else if (weather_info.sky == SKY_LIGHTNING)
                            send_to_char("#cStreaks of #WLightning #clight up the #Csky#n\n\r",ch);
                    }
                }
       }
	    if (ch->in_room->blood == 1000)
		xprintf(buf,"You notice that the room is completely drenched in blood.\n\r");
	    else if (ch->in_room->blood > 750)
		xprintf(buf,"You notice that there is a very large amount of blood around the room.\n\r");
	    else if (ch->in_room->blood > 500)
		xprintf(buf,"You notice that there is a large quantity of blood around the room.\n\r");
	    else if (ch->in_room->blood > 250)
		xprintf(buf,"You notice a fair amount of blood on the floor.\n\r");
	    else if (ch->in_room->blood > 100)
		xprintf(buf,"You notice several blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 50)
		xprintf(buf,"You notice a few blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 25)
		xprintf(buf,"You notice a couple of blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 0)
		xprintf(buf,"You notice a few drops of blood on the floor.\n\r");
	    else xprintf(buf,"You notice nothing special in the room.\n\r");
	    ADD_COLOUR(ch, buf, L_RED);
	    if (ch->in_room->blood > 0) send_to_char(buf,ch);
	}
            if (IS_SET(ch->in_room->room_flags, ROOM_FLAMING))
            send_to_char("#r..This room is engulfed in #Yflames!#n\n\r",ch);

	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );

        for (door=0 ; door < 6 ; door++)
	{	
	    if (ch->in_room == NULL) continue;
	    if (ch->in_room->exit[door] == NULL) continue;

	    if (IS_SET(ch->in_room->exit[door]->exit_info,EX_PRISMATIC_WALL)) 	    
	    {
		xprintf(buf, "#w     You see a shimmering wall of many #sc#so#sl#so#su#sr#ss #w%s.\n\r#n", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ICE_WALL))
	    {
		xprintf(buf, "     You see a glacier of ice %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_CALTROP_WALL))
	    {
		xprintf(buf, "     You see a wall of caltrops %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_FIRE_WALL))
	    {
		xprintf(buf, "     You see a blazing wall of fire %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_SWORD_WALL))
	    {
		xprintf(buf, "     You see a spinning wall of swords %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_MUSHROOM_WALL))
	    {
		xprintf(buf, "     You see a vibrating mound of mushrooms %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_IRON_WALL))
	    {
		xprintf(buf, "    You see a solid wall of iron %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ASH_WALL))
	    {
		xprintf(buf, "    You see a deadly wall of ash %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	}
	show_char_to_char( ch->in_room->people,   ch );
        if (str_cmp(arg1, "scry")) aggr_test(ch);
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) )
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, NULL, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "It doesn't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    if (obj->value[2] == 1 || obj->value[2] == 3)
	    {
		send_to_char( "It seems to be closed.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"scry");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"scry");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"scry");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

    case ITEM_SLOT_MACHINE:
           if(obj->value[3] == 1)
             partial = TRUE;
           else
             partial = FALSE;
           xprintf(buf,"This is a %d bar slot machine costing %d bones with a jackpot of %d bones.\n\rThe jackpot has %spartial winnings.\n\r", obj->value[2], obj->value[0], obj->value[1],  partial ? "" : "no ");
           send_to_char(buf, ch);
           break;
           
	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    if (obj->value[1] < obj->value[0] / 5)
	    	xprintf( buf, "There is a little %s liquid left in it.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	xprintf( buf, "It contains a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	xprintf( buf, "It's about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	xprintf( buf, "It's about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	xprintf( buf, "It is almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	xprintf( buf, "It's completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	xprintf( buf, "Somehow it is MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p contains:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
	evil_eye(victim,ch);
	return;
    }

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if (!IS_NPC(vch) && !str_cmp(arg1,vch->morph))
	    {
		show_char_to_char_1( vch, ch );
		evil_eye(vch,ch);
		return;
	    }
	    continue;
	}
    }

    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
    {
	obj = get_obj_in_obj(ch,arg1);
	if (obj != NULL)
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    pdesc = get_extra_descr( arg1, ch->in_room->extra_descr );
    if ( pdesc != NULL )
    {
	send_to_char( pdesc, ch );
	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }
/*
    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );
*/

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
	else
	{
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
    }
    else
    {
	if ((pexit = ch->in_room->exit[door]) == NULL) return;
	if ((pRoomIndex = pexit->to_room) == NULL) return;
	location = ch->in_room;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
	char_from_room(ch);
	char_to_room(ch,location);
    }

    return;
}



void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    do_look( ch, arg );

    if ( ( obj = get_obj_here( ch, NULL, arg ) ) != NULL )
    {
	     if (obj->condition >= 100) 
	    xprintf( buf, "You notice that %s is in perfect condition.\n\r",obj->short_descr );
	else if (obj->condition >= 75 )
	    xprintf( buf, "You notice that %s is in good condition.\n\r",obj->short_descr );
	else if (obj->condition >= 50 )
	    xprintf( buf, "You notice that %s is in average condition.\n\r",obj->short_descr );
	else if (obj->condition >= 25 )
	    xprintf( buf, "You notice that %s is in poor condition.\n\r",obj->short_descr );
	else
	    xprintf( buf, "You notice that %s is in awful condition.\n\r",obj->short_descr );
	send_to_char(buf,ch);
	switch ( obj->item_type )
	{
	default:
	    break;

	case ITEM_DRINK_CON:
	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    send_to_char( "When you look inside, you see:\n\r", ch );
	    xprintf( buf, "in %s", arg );
	    do_look( ch, buf );
	}
    }

    return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    buf[0] = '\0';
    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

    xcatf( buf, fAuto ? "#R[#GExits#7:#C" : "Obvious exits:\n\r" );

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
    if ((pexit = ch->in_room->exit[door]) != NULL && pexit->to_room != NULL)
	{
	    found = TRUE;
	    if ( fAuto )
	    {
        if (IS_SET(pexit->exit_info, EX_CLOSED))
        {
          xcatf(buf, " #0(#C");
          xcatf(buf, dir_name[door]);
          xcatf(buf, "#0)#C");
        }
        else
        {
		xcatf( buf, " " );
		xcatf( buf, dir_name[door] );
	    }
      }
	    else
	    {
        xcatf(buf, "%-5s - %s\n\r", capitalize(dir_name[door]), room_is_dark(pexit->to_room) ? "Too dark to tell" : pexit->to_room->name);
	    }
	}
    }

    if ( !found )
	xcatf( buf, fAuto ? " none" : "None.\n\r" );

    if ( fAuto )
    xcatf(buf, "#0]#n\n\r");

    send_to_char( buf, ch );
    return;
}


const char * transformation_message[2][12] =
{
{
"\n\rYour body shudders and convulses as your form transforms. Arms and legs grow out of your slimy torso, your skin turns a sickly veined green.\n\r" ,
"\n\rYour demonic form is reduced to a pool of pulsating jelly, the fires of hell heat and char your semi-liquid form until you stiffen.  Limbs grow and spiny talons extend from your arms and back.\n\r" ,
"\n\rYou start the transformation.  Your gargoylish apperance is changed as you grow another two feet in height.  Scales, dripping with acid, begin to cover your skin.\n\r" ,
"\n\rThe fires of hell surround you once more.  Sharp claws start to grow from your hands, and you grow a viscious barbed tail.  Small flames play over your burnt and blackened skin.\n\r" ,
"\n\rYour next form is that of a sickly, gaunt skeleton.  Your leathery flesh held tight over a demonic frame.  A huge tail, with a poisonous barb on the end protrudes out of the base of your spine.\n\r" ,
"\n\rAs you transform, you grow another four feet in height. This time, wicked barbs cover your entire body.  You find you have the ability to wield weapons, and use armor.\n\r" ,
"\n\rWhat?  What is happening, with the transformation into an Erinyes, your form is no longer repulsive and demonic, but you are now a beautiful mortal, with long auburn hair and huge white wings growing out of your back.  You can see the world again with your mortal eyes, but the  passions of a demon will always stay.\n\r",
"\n\rYour skin melts and you transform once more, but this time you become a gruesome pug nosed, vile dwarf-like creature with large leathery wings and a mouth filled of razor sharp teeth, an Amnizu.\n\r" ,
"\n\rThe intense heat of the fires of Baator surround you. Your skin is wrenched apart and you transform into a grotesque 9-foot tall monstrosity with huge wings, a snaking, prehensile tail, and a long barbed whip.  A true demon, a Cornugon.\n\r",
"\n\rThe power of Baator now fills your very soul.  You grow again as the chill winds of Caina tear the flesh from your bones.  You look to see what you have become, you look alien, with a twelve foot long icy insect-like body, your head bulging with multi-faceted eyes and you have a long tail covered with razor-sharp spikes.\n\r",
"\n\rAaargh!  You are cast into the pit of flame. The fires, more intense than ever before sear and scar your flesh as it bubbles and boils. You have become the most terrible demon, the Pit Fiend, a huge bat winged  humanoid, twelve feet tall, with large fangs that drip with a vile green liquid and a hulking red scaly body that bursts into flame when you are angered or excited.\n\r",
"\n\rYour fiery skin blackens as you are infused with the embodiment of ultimate evil.  You are the most powerful demon, the lord of all.  You are everything, nothing can oppose you.  Twenty foot tall, your torso drips with the vile acid of the Abyss.  A ravenous hunger is gnawing at your bones.  Welcome to Immortality..."
	},
	{
"Your first transformation is quite remarkable, you grow to over seven feet tall, and your skin starts to dull and lose all signs of vitality.  Your teeth begin to extend.  The world starts to look different, you have left mortality behind.\n\r",
"You start to grow, the base of your spine slowly extends into a short stump of a tail.  The metamorphosis is tearing you apart.  Your skin begin to harden, and your hands twist into claws.\n\r",
"Once again the painful metamorphosis strikes.  Your bones rend through your scaled flesh as jagged spines shoot through your back.  You feel as if you are being wrenched apart.  You are now over ten feet tall, and your skin is covered with thick green scales.\n\r",
"You transform once again.  This time a small pair of wings burst through your back.  Your hands and feet have become viscious claws, and your nose has extended into a snout.  Long jagged teeth begin to grow in your mouth, and your tail now reaches the floor.\n\r",
"You fall to the floor in agony.  Clutching your chest as the transformation strikes.  You grow to over fifteen feet in length, towering above all the mortals that you have left behind.  Impenetrable scales now cover your body,  and smoke issues from your nostrils when you breathe.\n\r",
"You grow to over twenty feet in length, your tail and claws grow at an alarming rate.  Your wings are now so powerful that you can fly.  Nobody can oppose you now.  Suddenly it hits you, a hunger so intense that you cannot ignore it. You must feed.\n\r",
"",
"",
"",
"",
"You have reached the pinnacle of power for a dragon.  As your body undergoes the by now familiar transformation you grow to over fifty feet tall, with incredibly sharp claws and fangs, a powerful tail and a fiery breath weapon. Nobody can oppose you now.  You are the lord of the realm.\n\r",
""
	}
};


void do_weaplist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    if (IS_NPC(ch)) return;
 
 
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char(
 "                    Weapon, Stance, and Spell Skill           \n\r",ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char(
 "                            Weapon\n\r",ch);
xprintf( buf,
"Unarm:%-4d Slice:%-4d Stab:%-4d Slash:%-4d Whip:%-4d Claw:%-4d Blast:%-4d\n\r",
ch->wpn[0],ch->wpn[1],ch->wpn[2],ch->wpn[3],ch->wpn[4],ch->wpn[5],ch->wpn[6]);
send_to_char(buf,ch);
xprintf( buf,
"Pound  :%-4d Crush  :%-4d Grep   :%-4d Bite    :%-4d Pierce:%-4d Suck   :%-4d\n\r",
ch->wpn[7],ch->wpn[8],ch->wpn[9],ch->wpn[10],ch->wpn[11],ch->wpn[12]);
send_to_char(buf,ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char(
 "                            Stances\n\r",ch);
xprintf( buf,
"Viper : %-4d Crane: %-4d Crab  : %-4d Mongoose: %-4d Bull: %-4d Mantis: %-4d\n\r",
ch->stance[1],ch->stance[2],ch->stance[3],ch->stance[4],ch->stance[5],ch->stance[6]);
send_to_char(buf,ch);
xprintf( buf,
"Dragon: %-4d Tiger: %-4d Monkey: %-4d Swallow: %-4d Leopard: %-4d\n\r",
ch->stance[7],ch->stance[8],ch->stance[9],ch->stance[10],ch->stance[11]);
send_to_char(buf,ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char(
 "                            Magic\n\r",ch);
xprintf( buf,
"Purple: %-4d  Red: %-4d  Blue: %-4d  Green: %-4d  Yellow: %-4d\n\r",ch->spl[0],ch->spl[1],ch->spl[2],ch->spl[3],ch->spl[4]);
send_to_char(buf,ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
 
 
return;
}

 
void do_far( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   ROOM_INDEX_DATA *chroom;
   ROOM_INDEX_DATA *victimroom;
   char arg1[MAX_STRING_LENGTH];

   argument = one_argument (argument, arg1);

    if ( IS_NPC(ch) )
	return;

	if (arg1[0] == '\0')
	{
	   send_to_char("Who do you wish to use farcommand on?\n\r",ch);
	   return;
	}
/*  
        if (arg2[0] == '\0')
	{
	   send_to_char("What do you wish to do to them?\n\r",ch);
	   return;
	}

	if (arg3[0] == '\0')
	{
	   send_to_char("Possess or command who?\n\r",ch);
	   return;
	}
*/
  	if (!IS_CLASS(ch, CLASS_VAMPIRE))
  	{
  	send_to_char("Huh?\n\r", ch);
  	return;
  	}
  	
  	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_PRES] < 9 )
  	{
  	send_to_char("You need level 9 Presence to use this power.\n\r", ch);
	return;
	}

      if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
     if ( !IS_NPC(victim) )
	{
	   send_to_char("Their mind is too strong!\n\r",ch);
	   return;
	}
    if ( victim->level > 250)
    {
    send_to_char("They are too big!\n\r",ch);
    return;
    } 
  
    chroom = ch->in_room;
    victimroom = victim->in_room;
    char_from_room(ch);
    char_to_room(ch,victimroom);
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    send_to_char("You possess their body.\n\r",ch);
    WAIT_STATE(ch, 50);
    char_from_room(ch);
    char_to_room(ch,chroom);  	
    
    return;
}

void do_vstat( CHAR_DATA *ch, char *argument )
{
    char buf   [MAX_STRING_LENGTH];
    char lin   [MAX_STRING_LENGTH];
    char age   [MAX_STRING_LENGTH];
    int blood;
    int bloodpool;

    if (ch->generation <= 0)
	ch->generation = 4;

    {
    if (IS_SET(ch->newbits, NEW_TIDE))
    bloodpool = (3000 / ch->generation);
    else bloodpool = (2000 / ch->generation);
    }

    blood = ch->pcdata->condition[COND_THIRST];
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE)) return;
    if (ch->pcdata->rank == AGE_ANCILLA)         xprintf( age, "Ancilla");
    else if (ch->pcdata->rank == AGE_CHILDE)     xprintf( age, "Childe");
    else if (ch->pcdata->rank == AGE_NEONATE)    xprintf( age, "Neonate");
    else if (ch->pcdata->rank == AGE_ELDER)      xprintf( age, "Elder");
    else if (ch->pcdata->rank == AGE_METHUSELAH) xprintf( age, "Methuselah");
    else if( ch->pcdata->rank == AGE_LA_MAGRA)   xprintf( age, "La Magra");
    else if( ch->pcdata->rank == AGE_TRUEBLOOD)  xprintf( age, "TrueBlood");
    xprintf( lin,
"---------------------------------------------------------------------------\n\r");
    send_to_char( lin,ch);
    send_to_char(
"                              Vampire Status\n\r",ch);
send_to_char(lin,ch);
xprintf(buf,
"Generation:%d  Bloodpool:%d  Age:%s  Blood:%d\n\r",
ch->generation,bloodpool,age,blood);
send_to_char(buf,ch);
send_to_char(lin,ch);
send_to_char(
"                              Disciplines\n\r",ch);
send_to_char(lin,ch);
xprintf(buf,
"Animalism:    [%d]             Celerity:   [%d]             Fortitude: [%d]\n\r",
ch->power[DISC_VAMP_ANIM],ch->power[DISC_VAMP_CELE],
ch->power[DISC_VAMP_FORT]);
send_to_char(buf,ch);
xprintf(buf,
"Obtenebration:[%d]             Presence:   [%d]             Quietus:   [%d]\n\r",
ch->power[DISC_VAMP_OBTE],ch->power[DISC_VAMP_PRES],
ch->power[DISC_VAMP_QUIE]);
send_to_char(buf,ch);
xprintf(buf,
"Thaumaturgy:  [%d]             Auspex:     [%d]             Dominate:  [%d]\n\r",
ch->power[DISC_VAMP_THAU],ch->power[DISC_VAMP_AUSP],
ch->power[DISC_VAMP_DOMI]);
send_to_char(buf,ch);
xprintf(buf,
"Obfuscate:    [%d]             Potence:    [%d]             Protean:   [%d]\n\r",
ch->power[DISC_VAMP_OBFU],ch->power[DISC_VAMP_POTE],
ch->power[DISC_VAMP_PROT]);
send_to_char(buf,ch);
xprintf(buf,
"Serpentis:    [%d]             Vicissitude:[%d]             Daimoinon: [%d]\n\r",
ch->power[DISC_VAMP_SERP],ch->power[DISC_VAMP_VICI],
ch->power[DISC_VAMP_DAIM]);
send_to_char(buf,ch);
xprintf(buf,			 
"Chimerstry:   [%d]             Obeah:      [%d]             Melpominee:[%d]\n\r",
ch->power[DISC_VAMP_CHIM], ch->power[DISC_VAMP_OBEA], ch->power[DISC_VAMP_MELP]);
send_to_char(buf,ch);
xprintf(buf,
"Necromancy:   [%d]                                         Thanatosis:[%d]\n\r",
ch->power[DISC_VAMP_NECR], ch->power[DISC_VAMP_THAN] );
stc(buf,ch);
send_to_char(lin,ch);
 
return;
}

void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char ss1[MAX_STRING_LENGTH];
    char ss2[MAX_STRING_LENGTH];
    int a_c = char_ac(ch);

	if (IS_NPC(ch)) return;

    if (!IS_NPC(ch) && (IS_EXTRA(ch,EXTRA_OSWITCH) || IS_HEAD(ch,LOST_HEAD)))
	{obj_score(ch,ch->pcdata->chobj);return;}
    xprintf( buf,
	"#R[#nYou are #C%s%s#R][#nYou have been playing for #C%d #nhours#R]#n\n\r",
	ch->pcdata->switchname,
	IS_NPC(ch) ? "" : ch->pcdata->title,
        (ch->played + (int) (current_time - ch->logon))/3600);
    send_to_char( buf, ch );
    birth_date(ch, TRUE);
    if (!IS_NPC(ch) && IS_EXTRA(ch, EXTRA_PREGNANT)) birth_date(ch, FALSE);

    if ( get_trust( ch ) != ch->level )
    {
	xprintf( buf, "You are trusted at level %d.\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }

    xprintf( buf,
	"#R[#nYou have #C%d#n/#C%d #nhit#R][#C%d#n/#C%d#n mana#R][#C%d#n/#C%d#n movement#R][#C%d#n primal energy#R]#n\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move,
	ch->practice );
    send_to_char( buf, ch );

    xprintf( buf,
	"#R[#nYou are carrying #C%d#n/#C%d #nitems with weight #C%d#n/#C%d#n kg#R]\n\r",
	ch->carry_number, can_carry_n(ch),
	ch->carry_weight, can_carry_w(ch) );
    send_to_char( buf, ch );

    xprintf( buf,
	"#R[#nStr: #C%d#R][#nInt: #C%d#R][#nWis: #C%d#R][#nDex: #C%d#R][#nCon: #C%d#R]\n\r",
	get_curr_str(ch),
	get_curr_int(ch),
	get_curr_wis(ch),
	get_curr_dex(ch),
	get_curr_con(ch) );
    send_to_char( buf, ch );

    xprintf( buf,
	"#R[#nYou have scored #C%d#n exp#R]\n\r",
	ch->exp);
    send_to_char( buf, ch );
    
    xprintf( buf,
	"#R[#wYou have #y%d #0bones #win your bag#R]\n\r",
	ch->bones);
    send_to_char( buf, ch );
    
    if(!IS_NPC(ch))
    {
    xprintf( buf,
	"#R[#wYou have #y%d #0bones #win the bank#R]\n\r",
	ch->pcdata->bank);
    send_to_char( buf, ch );
    }

    if (!IS_NPC(ch) && IS_CLASS( ch, CLASS_DEMON))
    {
	xprintf( buf,
	"#R[#nYou have #C%d#n out of #C%d #npoints of demonic power stored#R]\n\r",
	ch->pcdata->stats[DEMON_CURRENT],  ch->pcdata->stats[DEMON_TOTAL] );
	send_to_char( buf, ch );
    }
    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_CYBORG)))
    {
        xprintf( buf,"#R[#nYou have #C%d #ncyborg points stored#R]\n\r",ch->pcdata->stats[CYBORG_POWER] );
        send_to_char( buf, ch );
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW))
  {
    xprintf(buf, "#R[#nYou have #C%d #nShadow Points stored#R]#n\n\r", ch->pcdata->powers[SHADOW_POWER]);
    send_to_char(buf, ch);
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PRIEST))
  {
    xprintf(buf, "#R[#nYou have #W%d #nFaith Points stored#R]#n\n\r", ch->pcdata->stats[FAITH_PTS]);
    send_to_char(buf, ch);
  }
    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_TANARRI)))
    {
	xprintf( buf, "#R[#nYou have #C%d #nTanar'ri points stored#R]\n\r", ch->pcdata->stats[TPOINTS] );
	send_to_char( buf, ch);
    }
    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_GIANT)) )
    {
        xprintf(buf, "#R[#nYou have #C%d #GGiant #nPoints stored#R]#n\n\r", ch->pcdata->stats[GIANT_POINTS]);
    send_to_char(buf, ch);
    }
    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_GHOUL)) )
    {
	xprintf( buf,
	"#R[#nYou have #C%d #GGhoul #nPoints stored#R]#n\n\r",
	ch->pcdata->stats[GHOUL_PTS] );
	send_to_char( buf, ch );
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON) && ch->generation < 13)
    {
	xprintf( buf,
	"#R[#nYou have obtained #C%d #nsouls, which you keep in a little jar#R]\n\r",
	ch->pcdata->souls);
	stc(buf, ch);
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW)) {
	xprintf( buf, "#R[#nYou have #C%d#n drow power points stored#R]\n\r",
	ch->pcdata->stats[DROW_POWER] );
	send_to_char( buf, ch );
	xprintf( buf, "#R[#nYou have #C%d #npoints of magic resistance#R]\n\r",
	ch->pcdata->stats[DROW_MAGIC] );
	send_to_char( buf, ch ); 
	if (weather_info.sunlight == SUN_DARK) 
	send_to_char("#R[#nYou feel strong in the night#R]\n\r", ch );}

    xprintf( buf,
	"#R[#nAutoexit: #C%s#R][#nAutoloot: #C%s#R][#nAutosac: #C%s#R]\n\r",
	(!IS_NPC(ch) && IS_SET(ch->act2, PLR_AUTOEXIT)) ? "yes" : "no",
	(!IS_NPC(ch) && IS_SET(ch->act2, PLR_AUTOLOOT)) ? "yes" : "no",
	(!IS_NPC(ch) && IS_SET(ch->act2, PLR_AUTOSAC) ) ? "yes" : "no" );
    send_to_char( buf, ch );
    
    if(!IS_NPC(ch))
    {
       xprintf(buf,"#R[#nArena Stats#R][#nWins: #C%d#R][#nlosses: #C%d#R]\n\r",
       ch->pcdata->awins, ch->pcdata->alosses );
       send_to_char( buf, ch);
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      xprintf(buf,"#R[#nYour shapeshifter counter is : #C%d#R]\n\r",ch->pcdata->powers[SHAPE_COUNTER]);
      stc(buf,ch);
      xprintf(buf,"#R[#nYour phase counter is        : #C%d#R]\n\r",ch->pcdata->powers[PHASE_COUNTER]);
      stc(buf,ch);
    }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
    {
      xprintf(buf,"#R[#nAngelic Justice   : #C%d#R]\n\r",ch->pcdata->powers[ANGEL_JUSTICE]);
      stc(buf,ch);
      xprintf(buf,"#R[#nAngelic Love      : #C%d#R]\n\r",ch->pcdata->powers[ANGEL_LOVE]);
      stc(buf,ch);
      xprintf(buf,"#R[#nAngelic Harmony   : #C%d#R]\n\r",ch->pcdata->powers[ANGEL_HARMONY]);
      stc(buf,ch);
      xprintf(buf,"#R[#nAngelic Peace     : #C%d#R]\n\r",ch->pcdata->powers[ANGEL_PEACE]);
      stc(buf,ch);
   }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
    {
      xprintf(buf,"#R[#nYour current beast is : #C%d#R]\n\r",ch->beast);
      stc(buf,ch);

    }
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) )
    {
      xprintf(buf,"#R[#nYour block counter is currently: #C%d#R]\n\r",ch->monkblock);
      stc(buf,ch);
      xprintf(buf,"#R[#nYour current level of chi:       #C%d#R]\n\r",ch->chi[CURRENT]);
      stc(buf,ch);
      xprintf(buf,"#R[#nYour maximum level of chi:       #C%d#R]\n\r",ch->chi[MAXIMUM]);
      stc(buf,ch);
      xprintf(buf,"#R[#nYour current focus points:       #C%d#R]\n\r",ch->focus[CURRENT]);
      stc(buf,ch);
      xprintf(buf,"#R[#nYour maximum focus points:       #C%d#R]\n\r",ch->focus[MAXIMUM]);
      stc(buf,ch);
    }
    if( !IS_NPC(ch) && (IS_CLASS(ch, CLASS_WEREWOLF)) && ch->gnosis[GMAXIMUM] > 0 )
    {
	xprintf( buf, "#R[#nCurrent Gnosis:            #C%d#R]\n\r", ch->gnosis[GCURRENT] );
	stc( buf, ch);
	xprintf( buf, "#R[#nMaximum gnosis:            #C%d#R]\n\r", ch->gnosis[GMAXIMUM] );
	stc( buf, ch);
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "#R[#nYou are drunk#R]\n\r",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] ==  0 )
	send_to_char( "#R[#nYou are thirsty#R]\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   ==  0 )
	send_to_char( "#R[#nYou are hungry#R]\n\r",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 100 )
	send_to_char( "#R[#nYou are feeling extremely horny#R]\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 50 )
	send_to_char( "#R[#nYou are feeling pretty randy#R]\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 1 )
	send_to_char( "#R[#nYou are feeling rather kinky#R]\n\r",  ch );

    if ( !IS_NPC(ch) && ch->pcdata->stage[1] > 0 && ch->position == POS_STANDING )
    {
	send_to_char( "#R[#nYou are having sexual intercourse#R]\n\r",  ch );
	if (!IS_NPC(ch) && (ch->pcdata->stage[2] + 25) >= ch->pcdata->stage[1])
	    send_to_char( "#R[#nYou are on the verge of having an orgasm#R]\n\r",  ch );
    }
    else switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char( "#R[#nYou are #RD#0E#RA#0D#n!!#R]\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "#R[#nYou are #Rmortally #nwounded#R]\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "#R[#nYou are incapacitated#R]\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "#R[#nYou are stunned#R]\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "#R[#nYou are sleeping#R]\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "#R[#nYou are resting#R]\n\r",		ch );
	break;
    case POS_MEDITATING:
	send_to_char( "#R[#nYou are meditating#R]\n\r",	ch );
	break;
    case POS_SITTING:
	send_to_char( "#R[#nYou are sitting#R]\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "#R[#nYou are standing#R]\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "#R[#nYou are fighting#R]\n\r",		ch );
	break;
    }

    if ( ch->level >= 0 )
    {
	xprintf( buf, "#R[#nAC: #C%d#R]", a_c );
	send_to_char( buf, ch );
    }

    send_to_char( "#R[#nYou are ", ch );
         if ( a_c >=  101 ) send_to_char( "naked#R]\n\r", ch );
    else if ( a_c >=   80 ) send_to_char( "barely clothed#R]\n\r",   ch );
    else if ( a_c >=   60 ) send_to_char( "wearing clothes#R]\n\r",  ch );
    else if ( a_c >=   40 ) send_to_char( "slightly armored#R]\n\r", ch );
    else if ( a_c >=   20 ) send_to_char( "somewhat armored#R]\n\r", ch );
    else if ( a_c >=    0 ) send_to_char( "armored#R]\n\r",          ch );
    else if ( a_c >=  -50 ) send_to_char( "well armored#R]\n\r",     ch );
    else if ( a_c >= -100 ) send_to_char( "strongly armored#R]\n\r", ch );
    else if ( a_c >= -250 ) send_to_char( "heavily armored#R]\n\r",  ch );
    else if ( a_c >= -500 ) send_to_char( "superbly armored#R]\n\r", ch );
    else if ( a_c >= -749 ) send_to_char( "divinely armored#R]\n\r", ch );
    else                    send_to_char( "ultimately armored#R]\n\r",ch );

    xprintf( buf, "#R[#nHitroll: #C%d#R][#nDamroll: #C%d#R][#nDamcap: #C%d#R] ",
    char_hitroll(ch), char_damroll(ch), ch->damcap[0] );
    send_to_char( buf, ch );
    

    if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) )
    {
	xprintf( buf, "#R[#nBlood: #C%d#R]\n\r", ch->pcdata->condition[COND_THIRST] );
	send_to_char( buf, ch );

    }
    else if (ch->level >= 0)
	send_to_char( "\n\r", ch );

    if ( ch->level >= 0 )
    {
	xprintf( buf, "#R[#nAlignment: #C%d#R]", ch->alignment );
	send_to_char( buf, ch );
    }

    send_to_char( "#R[#nYou are ", ch );
         if ( ch->alignment >  900 ) send_to_char( "#Cangelic#R]\n\r", ch );
    else if ( ch->alignment >  700 ) send_to_char( "#Csai#Yn#Ctly#R]\n\r", ch );
    else if ( ch->alignment >  350 ) send_to_char( "#Yg#Coo#Yd#R]\n\r",    ch );
    else if ( ch->alignment >  100 ) send_to_char( "#Ykind#R]\n\r",    ch );
    else if ( ch->alignment > -100 ) send_to_char( "#Bneutral#R]\n\r", ch );
    else if ( ch->alignment > -350 ) send_to_char( "#0mean#R]\n\r",    ch );
    else if ( ch->alignment > -700 ) send_to_char( "#0e#Rvi#0l#R]\n\r",    ch );
    else if ( ch->alignment > -900 ) send_to_char( "#Rdem#0o#Rnic]\n\r", ch );
    else                             send_to_char( "#Rsatanic]\n\r", ch );
    
    if ( !IS_NPC(ch) && ch->level >= 0 )
    {
	xprintf( buf, "#R[#nStatus: #C%d#R]", ch->race );
	send_to_char( buf, ch );
    }

    xprintf(buf,"#R[#nYou are currently on Tier #R%d#R]#n\n\r",ch->tier);
    stc(buf,ch);
	
    xprintf(buf,"#R[#nYou have #Y%d #nTier Kills for this tier#R]#n\n\r",ch->tks);
    stc(buf,ch);

    if (!IS_NPC(ch)) send_to_char( "#R[#nYou are ", ch );

         if (!IS_NPC(ch) && ch->level == 1 ) send_to_char( "a Mortal#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 2 ) send_to_char( "a Mortal#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 3 ) send_to_char( "an Avatar#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 7 ) send_to_char( "a Builder#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 8 ) send_to_char( "a Quest Maker#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 9 ) send_to_char( "an Enforcer#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 10) send_to_char( "a Judge#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 11) send_to_char( "a High Judge#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 12) send_to_char( "an Implementor#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 0 ) send_to_char( "an Avatar#R]\n\r",ch);
    else if (!IS_NPC(ch) && ch->race <= 4 ) send_to_char( "Immortal#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 9 ) send_to_char( "Godling#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 14) send_to_char( "Demigod#R]\n\r",ch);
    else if (!IS_NPC(ch) && ch->race <= 19) send_to_char( "Lesser God#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 24) send_to_char( "Greater God#R]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race >= 25) send_to_char( "Supreme God#R]\n\r", ch);
    else if (!IS_NPC(ch)) send_to_char( "a Bugged Character!\n\r", ch);

    if ( !IS_NPC(ch) )
    {
	if      (ch->pkill  == 0) xprintf(ss1,"#Cno#n players");
	else if (ch->pkill  == 1) xprintf(ss1,"#C%d#n player",ch->pkill);
	else                      xprintf(ss1,"#C%d#n players",ch->pkill);
	if      (ch->pdeath == 0) xprintf(ss2,"#Cno#n players");
	else if (ch->pdeath == 1) xprintf(ss2,"#C%d#n player",ch->pdeath);
	else                      xprintf(ss2,"#C%d#n players",ch->pdeath);
	xprintf( buf, "#R[#nYou have killed %s and have been killed by %s#R]\n\r", ss1, ss2 );
	send_to_char( buf, ch );
	if      (ch->mkill  == 0) xprintf(ss1,"#Cno#n mobs");
	else if (ch->mkill  == 1) xprintf(ss1,"#C%d#n mob",ch->mkill);
	else                      xprintf(ss1,"#C%d#n mobs",ch->mkill);
	if      (ch->mdeath == 0) xprintf(ss2,"#Cno#n mobs");
	else if (ch->mdeath == 1) xprintf(ss2,"#C%d#n mob",ch->mdeath);
	else                      xprintf(ss2,"#C%d#n mobs",ch->mdeath);
	xprintf( buf, "#R[#nYou have killed %s and have been killed by %s#R]\n\r", ss1, ss2 );
	send_to_char( buf, ch );
    }

    if( !IS_NPC(ch) )
    {
	xprintf( buf, "#R[#nYou have done #C%d #nquests for #C%d #nbones#R]\n\r", ch->pcdata->questsrun, ch->pcdata->questtotal );
	stc( buf, ch);
    }

    if (IS_AFFECTED(ch,AFF_HIDE)) send_to_char( "#R[#nYou are keeping yourself hidden from those around you#R]\n\r", ch );

    if ( !IS_NPC(ch) )
    {
	if ( IS_CLASS(ch,CLASS_WEREWOLF) && ch->siltol > 0)
	{
	    xprintf(buf,"#R[#nYou have attained #C%d #npoints of silver tolerance#R]\n\r",ch->siltol);
	    send_to_char( buf, ch );
	}
	if ( IS_CLASS(ch,CLASS_VAMPIRE) && ch->rage > 0)
	{
	    xprintf(buf,"#R[#nThe beast is in control of your actions:  Affects Hitroll and Damroll by #C+%d#R]\n\r",ch->rage);
	    send_to_char( buf, ch );
	}
	if ( (IS_CLASS(ch, CLASS_WEREWOLF)) && IS_SET(ch->special,SPC_WOLFMAN) && ch->rage > 0)
	{
	    xprintf(buf,"#R[#nYou are raging:  Affects Hitroll and Damroll by #C+%d#R]\n\r",ch->rage);
	    send_to_char( buf, ch );
	}
	if (IS_CLASS(ch, CLASS_NINJA) && ch->rage > 0) {
	xprintf(buf,"#R[#nYour michi gives you an extra #C%d#n hitroll and damroll#R]\n\r", ch->rage);
	send_to_char(buf,ch);
	}
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 6
	&& ch->pcdata->powers[HARA_KIRI] > 0) {
	xprintf(buf,"#R[#nYou receive the power of HaraKiri for #C%d#n more ticks#R]\n\r",ch->pcdata->powers[HARA_KIRI]);
	send_to_char(buf,ch);
	}

        if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_VAMPIRE) &&ch->rage > 0 && !IS_CLASS(ch, CLASS_NINJA))
	{
	   xprintf(buf,"#R[#nYou are in a mad frenzy, adding #C%d#n Hitroll and Damroll#R]\n\r",ch->rage);
	   stc(buf, ch);
	}

	else if ( IS_CLASS( ch, CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
	{
	    xprintf(buf,"#R[#nYou are wearing demonic armour:  Affects Hitroll and Damroll by #C+%d#R]\n\r",
		((ch->pcdata->stats[DEMON_POWER]) *ch->pcdata->stats[DEMON_POWER]));
	    send_to_char( buf, ch );
	}
        else if ( IS_CLASS( ch, CLASS_DROW) && ch->pcdata->stats[DEMON_POWER] > 0)
        {
            xprintf(buf,"#R[#nYou are wearing drow armour:  Affects Hitroll and Damroll by #C+%d#R]\n\r",
                ((ch->pcdata->stats[DEMON_POWER]) *ch->pcdata->stats[DEMON_POWER]));  
            send_to_char( buf, ch );
        }

    }	
	if (ch->fight_timer > 0) {
	xprintf(buf,"#R[#nYou have #C%d#n rounds left on your fight timer#R]#n\n\r", ch->fight_timer);
	send_to_char(buf,ch);
	}
    return;
}



char *	const	day_name	[] =
{
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};

char *	const	month_name	[] =
{
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    xprintf( buf,
	"It is %d o'clock %s, Day of %s, %d%s the Month of %s.\n\r%s started up at %s\rThe system time is %s\r",
	(time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month],
        MUDNAME,
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );
    return;
}

/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
  char faith[MAX_STRING_LENGTH];
  char invisbuf[MSL];
  char sexbuf[MSL];
  char buf3[MAX_STRING_LENGTH];
  char buf4[MAX_STRING_LENGTH];
  char buf5[MAX_STRING_LENGTH];
  char buf6[MAX_STRING_LENGTH];
  char buf7[MAX_STRING_LENGTH];
    char buf8[MAX_STRING_LENGTH];
  char buf9[MAX_STRING_LENGTH];
  char buf10[MAX_STRING_LENGTH];
  char buf11[MAX_STRING_LENGTH];
  char buf12[MAX_STRING_LENGTH];
  char buf13[MAX_STRING_LENGTH];
  char buf14[MAX_STRING_LENGTH];
  char buf15[MAX_STRING_LENGTH];
  char buf16[MAX_STRING_LENGTH];
    char buf17[MAX_STRING_LENGTH];
    char buf18[MAX_STRING_LENGTH];
    char buf19[MAX_STRING_LENGTH];
    char buf20[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    char king[MAX_STRING_LENGTH];
    char openb[15];
    char closeb[15];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *wch;
    int iLevelLower;
    int iLevelUpper;
    int nPlayerAll = 0;
    int nPlayerVis = 0;
    int nImmVis = 0;
    char pkratio[MAX_STRING_LENGTH];
    bool fClassRestrict;
    bool fImmortalOnly;
    bool immortal = FALSE,avatar = FALSE,mortal = FALSE, mains = FALSE;
    bool imm = FALSE,ava = FALSE, mor = FALSE;
  bool a1 = FALSE;
  bool a2 = FALSE;
  bool a3 = FALSE;
  bool a4 = FALSE;
  bool a5 = FALSE;
  bool a6 = FALSE;
  bool a7 = FALSE;
  bool a8 = FALSE;
  bool a9 = FALSE;
  bool a10 = FALSE;
  bool a11 = FALSE;
  bool a12 = FALSE;
  bool a13 = FALSE;
  bool a14 = FALSE;
  bool a15 = FALSE;
  bool a16 = FALSE;

  if (IS_NPC(ch)) return;

    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    fClassRestrict = FALSE;
    fImmortalOnly  = FALSE;

    /*
     * Parse arguments.
     */
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	if ( is_number( arg ) )
	{
	    send_to_char("Enter 'Avatar' for level 3's, or 'God' for level 4's and 5's.\n\r",ch);
	    return;
	}
	else
	{
	    /*
	     * Look for classes to turn on.
	     */
	    arg[3]    = '\0';
	    if (   !str_cmp( arg, "imm" ) || !str_cmp( arg, "immortal" )
		|| !str_cmp( arg, "ava" ) || !str_cmp( arg, "avatar"   ) )
	    {
		fClassRestrict = TRUE;
	    }
	    else if ( !str_cmp( arg, "god" ) || !str_cmp( arg, "imp" ) )
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
		send_to_char("Enter 'Avatar' for level 3's, or 'God' for level 4's and 5's.\n\r",ch);
		return;
	    }
	}
    }

    /*
     * Now show matching chars.
     */
    pkratio[0] = '\0';
    buf[0] = '\0';
  buf3[0] = '\0';
  buf4[0] = '\0';
  buf5[0] = '\0';
  buf6[0] = '\0';
  buf7[0] = '\0';
  buf8[0] = '\0';
  buf9[0] = '\0';
  buf10[0] = '\0';
  buf11[0] = '\0';
  buf12[0] = '\0';   
  buf13[0] = '\0';
  buf14[0] = '\0';
  buf15[0] = '\0';
  buf16[0] = '\0';
  buf17[0] = '\0';
    buf18[0] = '\0';
    buf19[0] = '\0';
    buf20[0] = '\0';

  for (d = descriptor_list; d != NULL; d = d->next)
  {
	char const *class;

    /*
	 * Check for match against restrictions.
	 * Don't use trust as that exposes trusted mortals.
     */

	if ( (d->connected != CON_PLAYING && d->connected != CON_EDITING) 
	   || (!can_see( ch, d->character )  )  )
	    continue;
        
     wch   = ( d->original != NULL ) ? d->original : d->character;
	if ( wch->level < iLevelLower
	||   wch->level > iLevelUpper
	|| ( fImmortalOnly  && wch->level <  LEVEL_IMMORTAL )
	|| ( fClassRestrict && wch->level != LEVEL_HERO     ) )
	    continue;
	
    
     if (wch->level < 7) nPlayerAll++;
       /*
        * Visibility :-)
        */
    if (!can_see(ch, wch)) continue;
    
    //if (wch->level > 6 && IS_SET(wch->act, PLR_WIZINVIS)) continue;

    if (wch->level < 7) nPlayerVis++;
    else nImmVis++;

        /* Show Status */
        xprintf(pkratio,"#R(#w%3d#R)#n", wch->race);

    if ( wch->sex == 1 )
                    {
                        xprintf ( sexbuf, "#B[#n%s#B]#n ", "#cM" );
                    }
                    if ( wch->sex == 2 )
                    {
                        xprintf ( sexbuf, "#B[#n%s#B]#n ", "#pF" );
                    }
                    if ( wch->sex == 0 )
                    {
                        xprintf ( sexbuf, "#B[#n%s#B]#n ", "#wN" );
                    }
                    
                    xprintf ( invisbuf, "#B[#0.#B]" );
                    if ( ( wch->ghost_level >= LEVEL_IMMORTAL ) &&
                         ( ch->level >= wch->level ) )
                        xprintf ( invisbuf, "#B[#wG#B]" );
                    if ( wch->incog_level >= LEVEL_IMMORTAL )
                        xprintf ( invisbuf, "#B[#RI#B]" );
                    if ( wch->invis_level >= LEVEL_IMMORTAL )
                        xprintf ( invisbuf, "#B[#bW#B]" );

    /*
	 * Figure out what to print for class.
     */
	class = " ";

        if ((IS_HEAD(wch,LOST_HEAD) || IS_EXTRA(wch,EXTRA_OSWITCH)) && wch->pcdata->chobj != NULL)
    {
	    if (wch->pcdata->chobj->pIndexData->vnum == 12)
	    	class = "[#RA Head#n    ]";
	    else if (wch->pcdata->chobj->pIndexData->vnum == 30005)
	    	class = "[#PA Brain#n   ]";
    else
	    	class = "[#GAn Object#n ]";
	} else
	switch ( wch->level )
      {
	default: break;
	case MAX_LEVEL -  0:
		     if ( IS_EXTRA(wch, EXTRA_AFK) )
			class = "#1      AFK#n       ";
	        else if (wch->prefix != NULL)
			class = wch->prefix;
            else 
            class = "     IMP     "; 
            break;
	case MAX_LEVEL -  1: 
	  if ( IS_EXTRA(wch, EXTRA_AFK) )
			class = "#1       AFK#n       ";
	        else if (wch->prefix != NULL)
			class = wch->prefix;
	  else
	  	  class = "  #B[#G*#B] #YGOD  #B[#G*#B]#n  "; 
	  break;
	case MAX_LEVEL -  2: class = "  #B[#G*#B] #YGOD  #B[#G*#B]#n  "; break;
	case MAX_LEVEL -  3: class = "Sherriff        "; break;
	case MAX_LEVEL -  4: class = "Alterboy        "; break;
	case MAX_LEVEL -  5: class = "  #CQ#yuest#CM#yaker#n    "; break;
        case MAX_LEVEL - 6:
        case MAX_LEVEL - 7:
        case MAX_LEVEL - 8:
        case MAX_LEVEL - 9:
                     if (wch->prefix != NULL) class = wch->prefix;
                else if (IS_EXTRA(wch, EXTRA_AFK)) class = "   #C*#0***A#CF#0K***#C*#n  ";
                else if (IS_SET(wch->pcdata->jflags, JFLAG_BULLY)) class = "  #0B#Rully#n       ";
		else if (wch->race <= 0 ) class = "#B[ #gU#Gseless#b #YS#ycum#B ]#n";
		else if (wch->race <= 3 ) class = "#B[  #RD#reg#Re#rner#Ra#rte#B  ]#n";
		else if (wch->race <= 6 ) class = "#B[    #0L#Wac#wk#0y#B     ]#n";
		else if (wch->race <= 9 ) class = "#B[   #0H#Wench#0m#Wen#B   ]#n";
		else if (wch->race <= 12) class = "#B[#CGr#coun#Cd#WPo#0und#Wer#B ]#n";
		else if (wch->race <= 15) class = "#B[    #rB#Rada#rss#B    ]#n";
	        else if (wch->race <= 18) class = "#B[     #CT#chu#Cg#B     ]#n";
		else if (wch->race <= 21) class = "#B[    #yC#Yo#ynvi#Yc#yt#B   ]#n";
		else if (wch->race <= 24) class = "#B[  #gBa#Gc#gkS#Gt#gabbe#Gr#B ]#n";
		else if (wch->race <= 27) class = "#B[    #YO#yut#RL#raw#B    ]#n";
		else if (wch->race <= 30) class = "#B[   #PM#par#Pk#ps#PM#pan#B   ]#n";
		else if (wch->race <= 33) class = "#B[#0C#won#Wt#wr#0#Wac#0t#WK#wi#0l#Wl#wa#B]#n";
		else if (wch->race <= 36) class = "#B[  #BHe#ba#Bd#b Hun#Bter#B ]#n";
		else if (wch->race <= 39) class = "#B[  GS#gpe#Gc#gia#Glis#gt#B  ]#n";
		else if (wch->race <= 42) class = "#B[  #0M#Wa#Ds#Wte#0rM#Wi#0nd#B  ]#n";
		else if (wch->race <= 45) class = "#B[     #0D#Wo#0n#B      ]#n";
		else if (wch->race <= 50) class = "#B[  #pC#Pri#pm#Pe #pL#Por#pd#B  ]#n";
		else if (wch->race <= 55) class = "#B[   #YK#yi#Yn#yg #YP#yi#Yn#B   ]#n";   
          break;
      
	case MAX_LEVEL - 10: class = "#B[  #gMortal#B  ]#n"; break;
	case MAX_LEVEL - 11: class = "#B[  #6Newbie#B  ]#n"; break;
	case MAX_LEVEL - 12: class = "#B[#7Undefined#B ]#n"; break;
	}
    /*
	 * Format it up.
     */
    if (IS_CLASS(wch, CLASS_TANARRI))
      {strcpy(openb, "#Y{#n"); strcpy(closeb, "#Y}#n");}
    else if (IS_CLASS(wch, CLASS_ANGEL))
      {strcpy(openb, "#Y.x#0[#n" ); strcpy( closeb, "#0]#Yx.#n" );}
    else if (IS_CLASS(wch, CLASS_UNDEAD_KNIGHT))
      {strcpy(openb, "#0|[#n" ); strcpy( closeb, "#0]|#n" );}
    else if (IS_CLASS(wch, CLASS_SHAPESHIFTER))
      {strcpy(openb, "#0[#P*#0]#n" ); strcpy( closeb, "#0[#P*#0]#n" );}
    else if (IS_CLASS(wch, CLASS_VAMPIRE))
      {strcpy(openb, "#R<*#n" ); strcpy( closeb, "#R*>#n" );}
    else if (IS_CLASS(wch, CLASS_LICH))
      {strcpy(openb, "#G>*<#n" ); strcpy( closeb, "#G>*<#n" );}
    else if (IS_CLASS(wch, CLASS_WEREWOLF))
      {strcpy(openb, "#Y((#n" ); strcpy( closeb, "#Y))#n" );}
    else if (IS_CLASS(wch,CLASS_MAGE))
      {strcpy(openb, "#p*#P^#n" ); strcpy( closeb, "#P^#p*#n" );}
    else if (IS_CLASS(wch,CLASS_DRONE))
      {strcpy(openb, "#G<#0=#Y{#n" ); strcpy( closeb, "#Y}#0=#G>#n" );}
    else if (IS_CLASS(wch,CLASS_GIANT))
      {strcpy(openb, "#c<>#n" ); strcpy( closeb, "#c<>#n" );}
    else if (IS_CLASS(wch,CLASS_FAE))
      {strcpy(openb, "#G<<#R(#n" ); strcpy( closeb, "#R)#G>>#n" );}
    else if (IS_CLASS(wch,CLASS_CYBORG))
      {strcpy(openb, "#p{#0-#p}#n");strcpy( closeb, "#p{#0-#p}#n");}
    else if (IS_CLASS(wch, CLASS_SAMURAI))
      {strcpy(openb, "#C-=#n" ); strcpy( closeb, "#C=-#n" );}
    else if (IS_CLASS(wch, CLASS_DROW))
      {strcpy(openb, "#P.o0"); strcpy(closeb, "#P0o.#n" );}
    else if (IS_CLASS(wch, CLASS_NINJA))
      {strcpy(openb, "#C***#n"); strcpy(closeb, "#C***#n" );}
    else if (IS_CLASS(wch, CLASS_SKYBLADE))
      {strcpy(openb, "#B=#R*#W>#n"); strcpy(closeb, "#W<#R*#B=#n" );}
    else if (IS_CLASS(wch, CLASS_MONK))
      {strcpy(openb, "#0.x[#n"); strcpy(closeb, "#0]x.#n");}
   else if (IS_CLASS(wch, CLASS_GHOUL))
    {strcpy(openb, "#GX#gx#G(#n"); strcpy(closeb, "#G)#gx#GX#n");}
   else if (IS_CLASS(wch, CLASS_DRACONIAN))
    {strcpy(openb, "#R+#Y(#n"); strcpy(closeb, "#Y)#R+#n");}
   else if (IS_CLASS(wch, CLASS_THIEF))
    {strcpy(openb, "#r<#R)#n"); strcpy(closeb, "#R(#r>#n");}
   else if (IS_CLASS(wch, CLASS_SHADOW))
    {strcpy(openb, "#0-#Y>#0)#n"); strcpy(closeb, "#0(#Y<#0-#n");}
   else if (IS_CLASS(wch, CLASS_PRIEST))
    {strcpy(openb, "#W]#w>#B*#n"); strcpy(closeb, "#B*#w<#W[#n");}
   else if (IS_CLASS(wch, CLASS_JEDI))
    {strcpy(openb, "#B+#0=#B+#n"); strcpy(closeb, "#B+#0=#B+#n");}
    else
      {strcpy(openb, "#0[#n" ); strcpy( closeb, "#0]#n" );}
  if ( wch->class > 0)
    {
  if (IS_CLASS(wch, CLASS_MAGE) )
      {
    if (wch->generation == 1)
          xprintf(kav, "%s#CHigh Archmage#n%s        ", openb, closeb);
    else if(wch->generation == 2)
          xprintf(kav, "%s#CArchmage#n%s             ", openb, closeb);
    else if(wch->generation == 3)
          xprintf(kav, "%s#CLord of Spells#n%s       ", openb, closeb);
    else if(wch->generation == 4)
          xprintf(kav, "%s#CHigh Invoker#n%s         ", openb, closeb);
    else if(wch->generation == 5)
          xprintf(kav, "%s#CWizard of War#n%s        ", openb, closeb);
    else if(wch->generation == 6)
          xprintf(kav, "%s#CBattlemage#n%s           ", openb, closeb);
    else if(wch->generation == 7)
          xprintf(kav, "%s#CApprentice#n%s           ", openb, closeb);
        else
          xprintf(kav, "%s#CMagician#n%s             ", openb, closeb);
      }
  else if (IS_CLASS(wch, CLASS_CYBORG))
      {
    if (wch->generation == 1)
          xprintf(kav,"%s#0Killing Machine%s    ",openb,closeb);
    else if(wch->generation == 2)
          xprintf(kav,"%s#0Terminator%s         ",openb,closeb);
        else
          xprintf(kav,"%s#0Bionic Man%s         ",openb,closeb);
      }
    else if( IS_CLASS(wch, CLASS_PRIEST) )
  {
    if (wch->generation == 1)
          xprintf( kav, "%s#WHoly Priest#n%s        ", openb, closeb );
    else
          xprintf( kav, "%s#WPriest#n%s             ", openb, closeb );
  }
  else if( IS_CLASS(wch, CLASS_SHAPESHIFTER) )
      {
    if (wch->generation == 1)
          xprintf( kav, "%s#RSpawn of Malaug#n%s    ", openb, closeb );
    else if (wch->generation == 2)
          xprintf( kav, "%s#RShadowmaster#n%s       ", openb, closeb );
    else if (wch->generation == 3)
          xprintf( kav, "%s#RMalaugrym Elder#n%s    ", openb, closeb );
    else if (wch->generation == 4)
          xprintf( kav, "%s#RMalaugrym#n%s          ", openb, closeb );
    else if (wch->generation == 5)
          xprintf( kav, "%s#RShapeshifter#n%s       ", openb, closeb );
    else if (wch->generation == 6)
          xprintf( kav, "%s#RDoppleganger#n%s       ", openb, closeb );
    else if (wch->generation == 7)
          xprintf( kav, "%s#RMass of tentacles#n%s  ", openb, closeb );
        else
          xprintf( kav, "%s#RPlay-Doh#n%s           ", openb, closeb );
      }
  else if( IS_CLASS(wch, CLASS_JEDI) )
      {
    if (wch->generation == 1)
          xprintf( kav, "%s#CJedi #cMaster#n%s        ", openb, closeb );
    else if (wch->generation == 2)
          xprintf( kav, "%s#CJedi #BKnight#n%s        ", openb, closeb );
    else if (wch->generation == 3)
          xprintf( kav, "%s#BForce #RLord#n%s         ", openb, closeb );
    else if (wch->generation == 4)
          xprintf( kav, "%s#CJedi#n%s               ", openb, closeb );
    else if (wch->generation == 5)
          xprintf( kav, "%s#BForce #RMaster#n%s       ", openb, closeb );
    else if (wch->generation == 6)
          xprintf( kav, "%s#0Jedi #BTrainee#n%s       ", openb, closeb );
    else
          xprintf( kav, "%s#CSpoon Bender#n%s       ", openb, closeb );
      }
  else if( IS_CLASS(wch, CLASS_ANGEL) )
      {
    if (wch->generation == 1)
      xprintf( kav, "%s#7Arch Angel#n%s         ", openb, closeb );
    else if (wch->generation == 2)
      xprintf( kav, "%s#7Cherubim#n%s           ", openb, closeb );
    else if (wch->generation == 3)
      xprintf( kav, "%s#7Seraphim#n%s           ", openb, closeb );
    else if (wch->generation == 4)
      xprintf( kav, "%s#7Guardian Angel#n%s     ", openb, closeb );
    else if (wch->generation == 5)
      xprintf( kav, "%s#7Angel#n%s              ", openb, closeb );
        else 
      xprintf( kav, "%s#7Nephalim#n%s           ", openb, closeb );
}
  else if( IS_CLASS(wch, CLASS_SHADOW) )
      {
    if (wch->generation == 1)
      xprintf( kav, "%s#7Murderer#n%s           ", openb, closeb );
    else if (wch->generation == 2)
      xprintf( kav, "%s#7Slicer#n%s             ", openb, closeb );
    else if (wch->generation == 3)
      xprintf( kav, "%s#7Violator#n%s           ", openb, closeb );
    else if (wch->generation == 4)
      xprintf( kav, "%s#0Assassin#n%s           ", openb, closeb );
    else if (wch->generation == 5)
      xprintf( kav, "%s#0Slinker#n%s            ", openb, closeb );
        else 
      xprintf( kav, "%s#0Nightstalker#n%s       ", openb, closeb );
}
  else if( IS_CLASS(wch, CLASS_SKYBLADE) )
  {
    if (wch->generation == 1)
      xprintf( kav, "%s#WWind Sword#n%s         ", openb, closeb );
    else if (wch->generation == 2)
      xprintf( kav, "%s#WCloud Defender#n%s     ", openb, closeb );
    else if (wch->generation == 3)
      xprintf( kav, "%s#WCloud #0Knight#n%s       ", openb, closeb );
    else if (wch->generation == 4)
      xprintf( kav, "%s#WTempest Knight#n%s     ", openb, closeb );
    else if (wch->generation == 5)
      xprintf( kav, "%s#WSkyBlade#n%s           ", openb, closeb );
    else if (wch->generation == 6)
       xprintf( kav, "%s#CSky#wTrainee#n%s         ", openb, closeb );
        else 
      xprintf( kav, "%s#CSky#WWimp#n%s             ", openb, closeb );
  }
  else if( IS_CLASS(wch, CLASS_THIEF) )
  {
    if (wch->generation == 1)
      xprintf( kav, "%s#wM#0a#ws#0t#we#0r #wT#0h#wi#0e#wf#n%s         ", openb, closeb );
    else if (wch->generation == 2)
      xprintf( kav, "%s#wSilent #0Killer#n%s        ", openb, closeb );
    else if (wch->generation == 3)
      xprintf( kav, "%s#0Thief#n%s                ", openb, closeb );
    else if (wch->generation == 4)
      xprintf( kav, "%s#0Burglar#n%s              ", openb, closeb );
    else if (wch->generation == 5)
      xprintf( kav, "%s#wNight #0Blade#n%s          ", openb, closeb );
    else if (wch->generation == 6)
       xprintf( kav, "%s#0Cut#wPurse#n%s            ", openb, closeb );
        else 
      xprintf( kav, "%s#C0Pick Pocket#n%s         ", openb, closeb );
  }
  else if( IS_CLASS(wch, CLASS_GHOUL) )
      {
    if (wch->generation == 1)
      xprintf( kav, "%s#gNec#Grite#n%s            ", openb, closeb );
    else if (wch->generation == 2)
      xprintf( kav, "%s#wSwamp #0Thing#n%s        ", openb, closeb );
    else if (wch->generation == 3)
      xprintf( kav, "%s#gUndead#n%s             ", openb, closeb );
    else if (wch->generation == 4)
      xprintf( kav, "%s#0Zom#wbie#n%s             ", openb, closeb );
    else if (wch->generation == 5)
      xprintf( kav, "%s#GFlesh #0Eater#n%s        ", openb, closeb );
    else if (wch->generation == 6)
      xprintf( kav, "%s#gGhoul#n%s              ", openb, closeb );
        else 
      xprintf( kav, "%s#GScum Bag#n%s           ", openb, closeb );
}
  else if( IS_CLASS(wch, CLASS_DRACONIAN) )
      {
    if (wch->generation == 1)
      xprintf( kav, "%s#gDragon #Ylord#n%s          ", openb, closeb );
    else if (wch->generation == 2)
      xprintf( kav, "%s#wWar #BCaptain#n%s          ", openb, closeb );
    else if (wch->generation == 3)
      xprintf( kav, "%s#gWar #0General#n%s          ", openb, closeb );
    else if (wch->generation == 4)
      xprintf( kav, "%s#0Slave #wDriver#n%s         ", openb, closeb );
    else if (wch->generation == 5)
      xprintf( kav, "%s#GBig Lizard#n%s           ", openb, closeb );
    else if (wch->generation == 6)
      xprintf( kav, "%s#gWhelp#n%s                ", openb, closeb );
        else 
      xprintf( kav, "%s#GLizard#n%s               ", openb, closeb );
      }
  else if( IS_CLASS(wch, CLASS_UNDEAD_KNIGHT) )
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#BFallen Paladin#n%s       ", openb, closeb );
        else if (wch->generation == 2)
          xprintf( kav, "%s#BUndead Lord#n%s          ", openb, closeb );
        else if (wch->generation == 3)
          xprintf( kav, "%s#BUndead Knight#n%s        ", openb, closeb );
        else if (wch->generation == 4)
          xprintf( kav, "%s#BUndead Knight#n%s        ", openb, closeb );
        else if (wch->generation == 5)
      xprintf(     kav, "%s#LUndead Knight#n%s        ", openb, closeb );
    else if (wch->generation == 6)
      xprintf(     kav, "%s#LUndead Knight#n%s        ", openb, closeb );
    else if (wch->generation == 7)
      xprintf(     kav, "%s#LUndead Knight#n%s        ", openb, closeb );
        else 
          xprintf( kav, "%s#BSkeleton Knight#n%s      ", openb, closeb );
      }
      else if ( IS_CLASS(wch, CLASS_WEREWOLF))
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#BChieftain#n%s            ", openb, closeb );
        else if (wch->generation == 2)
          xprintf( kav, "%s#BTribe Shaman#n%s         ", openb, closeb );
        else if (wch->generation == 3)
          xprintf( kav, "%s#BPack Leader#n%s          ", openb, closeb );
        else if (wch->generation == 4)
          xprintf( kav, "%s#BFenris Wolf#n%s          ", openb, closeb );
        else if (wch->generation == 5)
          xprintf( kav, "%s#BStalker#n%s              ", openb, closeb );
        else if (wch->generation == 6)
          xprintf( kav, "%s#BHunter#n%s               ", openb, closeb );
        else if (wch->generation == 7) 
          xprintf( kav, "%s#BPup of Gaia#n%s          ", openb, closeb );
        else
          xprintf( kav, "%s#BHalfbreed Bastard#n%s    ", openb, closeb );
      }
      else if ( IS_CLASS(wch, CLASS_MONK))
      {
        if (wch->generation == 1)
          xprintf(kav,"%s#bAbbot#n%s              ", openb, closeb);   
        else if (wch->generation == 2)
          xprintf(kav,"%s#bHigh Priest#n%s        ", openb, closeb);   
        else if (wch->generation == 3)
          xprintf(kav,"%s#bPriest#n%s             ", openb, closeb);   
        else if (wch->generation == 4)
          xprintf(kav,"%s#bFather#n%s             ", openb, closeb);   
        else if (wch->generation == 5)
          xprintf(kav,"%s#bMonk#n%s               ", openb, closeb);   
        else if (wch->generation == 6)
          xprintf(kav,"%s#bBrother#n%s            ", openb, closeb);   
        else if (wch->generation == 7)
          xprintf(kav,"%s#bAcolyte#n%s            ", openb, closeb);   
        else
          xprintf(kav,"%s#bFanatic#n%s            ", openb, closeb);
      }
      else if ( IS_CLASS(wch, CLASS_DROW)) 
      {
        if (wch->generation == 1)
          xprintf(kav,"%s#0Matron Mother#n%s      ", openb, closeb);   
        else if (wch->generation == 2)
          xprintf(kav,"%s#0Weaponmaster#n%s       ", openb, closeb);   
        else if (wch->generation == 3)
          xprintf(kav,"%s#0High Priestess#n%s     ", openb, closeb);   
        else if (wch->generation == 4)
          xprintf(kav,"%s#0Favored of Lloth#n%s   ", openb, closeb);   
        else if (wch->generation == 5)
          xprintf(kav,"%s#0Black Widow#n%s        ", openb, closeb);   
        else if (wch->generation == 6)
          xprintf(kav,"%s#0Drow#n%s               ", openb, closeb);   
        else if (wch->generation == 7)
          xprintf(kav,"%s#0Drow Male#n%s          ", openb, closeb);
        else
          xprintf(kav,"%s#0Drider#n%s             ", openb, closeb);
      }
      else if ( IS_CLASS(wch, CLASS_VAMPIRE))
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#RI#0nner #RC#0ircle#n%s         ", openb, closeb);
        else if (wch->generation == 2)
          xprintf( kav, "%s#RV#0ampire #RJ#0usticar#n%s     ", openb, closeb);
        else if (wch->generation == 3)
          xprintf( kav, "%s#RV#0ampire #RP#0rince#n%s       ", openb, closeb);
        else if (wch->generation == 4)
          xprintf( kav, "%s#RV#0ampire #RA#0ncilla#n%s      ", openb, closeb);
        else if (wch->generation == 5)
          xprintf( kav, "%s#RV#0ampire #RA#0rchon#n%s       ", openb, closeb);
        else if (wch->generation == 6)
          xprintf( kav, "%s#RV#0ampire#n%s              ", openb, closeb);
        else if (wch->generation == 7)
          xprintf( kav, "%s#RV#0ampire #RA#0narch#n%s       ", openb, closeb);
        else
          xprintf( kav, "%s#RB#0loodsucker#n%s          ", openb, closeb);
      }
      else if ( IS_CLASS(wch, CLASS_TANARRI))
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#RTanar'ri Balor#n%s         ", openb, closeb );
        else if (wch->generation == 2)
          xprintf( kav, "%s#RTanar'ri Marilith#n%s      ", openb, closeb );
        else if (wch->generation == 3) 
          xprintf( kav, "%s#RGreater Tanar'ri#n%s       ", openb, closeb );
        else if (wch->generation == 4)
          xprintf( kav, "%s#RTrue Tanar'ri#n%s          ", openb, closeb );
        else if (wch->generation == 5)
          xprintf( kav, "%s#RTanar'ri#n%s               ", openb, closeb );
        else
          xprintf( kav, "%s#RCambion#n%s                ", openb, closeb );
      }
      else if (IS_CLASS(wch, CLASS_LICH))
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#7Demilich#n%s           ", openb, closeb );
        else if (wch->generation == 2 && wch->sex != SEX_FEMALE)
          xprintf( kav, "%s#7Lich Lord#n%s          ", openb, closeb );
        else if (wch->generation == 2 && wch->sex == SEX_FEMALE)
          xprintf( kav, "%s#7Lich Bitch#n%s         ", openb, closeb );
        else if (wch->generation == 3)
          xprintf( kav, "%s#7Power Lich#n%s         ", openb, closeb );
        else if (wch->generation == 4)
          xprintf( kav, "%s#7Ancient Lich#n%s       ", openb, closeb );
        else 
          xprintf( kav, "%s#7Lich#n%s               ", openb, closeb );
      }
      else if (IS_CLASS(wch, CLASS_FAE))
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#YFae Lord#n%s           ", openb, closeb );
        else if (wch->generation == 2)
           xprintf( kav, "%s#YAncient Fae#n%s        ", openb, closeb );
        else if (wch->generation == 3)
          xprintf( kav, "%s#YFae Elder#n%s          ", openb, closeb );
        else if (wch->generation == 4)
          xprintf( kav, "%s#YSpirit Fae#n%s         ", openb, closeb );
        else
          xprintf( kav, "%s#YFae#n%s                ", openb, closeb );
      }
      else if (IS_CLASS(wch, CLASS_GIANT))
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#GS#gtorm Gian#Gt#n%s          ", openb, closeb );
        else if (wch->generation == 2) 
          xprintf( kav, "%s#GC#gloud Gian#Gt#n%s          ", openb, closeb );
        else if (wch->generation == 3)
          xprintf( kav, "%s#GF#gire Gian#Gt#n%s           ", openb, closeb );
        else if (wch->generation == 4)
          xprintf( kav, "%s#GS#gtone Gian#Gt#n%s          ", openb, closeb );
        else if (wch->generation == 5)
          xprintf( kav, "%s#GH#gill Gian#Gt#n%s           ", openb, closeb );
        else if (wch->generation == 6)
          xprintf( kav, "%s#GG#gian#Gt#n%s                ", openb, closeb );
        else
          xprintf( kav, "%s#GO#ggr#Ge#n%s                 ", openb, closeb );
      }
      else if (IS_CLASS(wch, CLASS_DRONE))
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#0Horrid Reality#n%s     ", openb, closeb );
        else if (wch->generation == 2)
          xprintf( kav, "%s#0Horrid Nightmare#n%s   ", openb, closeb );
        else if (wch->generation == 3)
          xprintf( kav, "%s#0Nightmare#n%s          ", openb, closeb );
        else if (wch->generation == 4)
          xprintf( kav, "%s#0Dream Spawn#n%s        ", openb, closeb );
        else if (wch->generation == 5)
          xprintf( kav, "%s#0Drone#n%s              ", openb, closeb );
        else
          xprintf( kav, "%s#0Figment#n%s            ", openb, closeb );
      }
      else if ( IS_CLASS(wch, CLASS_SAMURAI) )
      {
        if (wch->generation == 1)
          xprintf(kav, "%s#RSho#Ygun#n%s               ", openb, closeb);
        else if (wch->generation == 2)
          xprintf(kav, "%s#RKata#Yna Mas#Rter#n%s        ", openb, closeb);
        else if (wch->generation == 3) 
          xprintf(kav, "%s#YSamu#Yrai Mas#Rter#n%s       ", openb, closeb);
        else if (wch->generation == 4)
          xprintf(kav, "%s#RSamu#Yrai El#Rite#n%s        ", openb, closeb);
        else if (wch->generation == 5)
          xprintf( kav, "%s#RSamu#Yrai#n%s              ", openb, closeb);
        else
          xprintf( kav, "%s#RTrai#Ynee#n%s              ", openb, closeb);
      }
      else if ( IS_CLASS(wch, CLASS_NINJA) )
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#YMaster Assassin#n%s    ", openb, closeb);
        else if (wch->generation == 2)
          xprintf( kav, "%s#YExpert Assassin#n%s    ", openb, closeb);
        else if (wch->generation == 3)
          xprintf( kav, "%s#YShadowlord#n%s         ", openb, closeb);
        else if (wch->generation == 4)
          xprintf( kav, "%s#YShadow Warrior#n%s     ", openb, closeb);
        else if (wch->generation == 5)
          xprintf( kav, "%s#YShadow#n%s             ", openb, closeb);
        else if (wch->generation == 6)
          xprintf( kav, "%s#YNinja#n%s              ", openb, closeb);
        else if (wch->generation == 7)
          xprintf( kav, "%s#YNinja Apprentice#n%s   ", openb, closeb);
        else 
          xprintf( kav, "%s#YThug#n%s               ", openb, closeb);
      }
      else if ( IS_CLASS(wch, CLASS_DEMON) )
      {
        if (wch->generation == 1)
          xprintf( kav, "%s#RPit Lord#n%s               ", openb, closeb);
        else if (wch->generation == 2)
          xprintf( kav, "%s#RPit Fiend#n%s              ", openb, closeb);
        else if (wch->generation == 3)
          xprintf( kav, "%s#RGateKeeper#n%s             ", openb, closeb);
        else if (wch->generation == 4)
          xprintf( kav, "%s#RImp Lord#n%s               ", openb, closeb);
        else if (wch->generation == 5)
          xprintf( kav, "%s#RHell Spawn#n%s             ", openb, closeb);
        else if (wch->generation == 6)
          xprintf( kav, "%s#RDemon#n%s                  ", openb, closeb);
        else if (wch->generation == 7)
          xprintf( kav, "%s#RSpawnling#n%s              ", openb, closeb);
        else 
          xprintf( kav, "%s#RImp#n%s                    ", openb, closeb);
      }
      else 
              xprintf( kav, "#nNone#n                     " );
    }
          
      else
	      xprintf( kav, "#nNone#n                     " );

    /* 
     * Religions
     */
    if (IS_SET(wch->newbits, NEW_MASTERY))
    {
      if (!IS_SET(wch->special, SPC_HAS_CHOSEN_RELIGION)) xprintf_2(faith,"#C-=#pFaithless#C=-#n");
      else xprintf(faith,"%s %s",
        religion_table[wch->pcdata->religion].rankname[wch->pcdata->relrank],religion_table[wch->pcdata->religion].name);
    }
    else xprintf_2(faith, " ");
        
      xprintf( king, "%s", kingdom_table[wch->pcdata->kingdom].whoname);
      if (!str_cmp(kingdom_table[wch->pcdata->kingdom].general, wch->name))
      {
	if( wch->sex == SEX_FEMALE ) xprintf( king, "Princess of %s", kingdom_table[wch->pcdata->kingdom].whoname);
        else xprintf( king, "General of %s", kingdom_table[wch->pcdata->kingdom].whoname);
      }
      else if ( !strcmp(wch->pcdata->switchname,kingdom_table[wch->pcdata->kingdom].leader) )
      {
	if( wch->sex == SEX_FEMALE ) xprintf( king, "Queen of %s",kingdom_table[wch->pcdata->kingdom].whoname );
	else xprintf( king, "King of %s",kingdom_table[wch->pcdata->kingdom].whoname );
      }
    if (wch->level > 6 )
    {
	xcatf( buf3, "#0[#n%s#0]%s %s %s %s %s %s\n\r",
	    class, invisbuf,
	    kav,
	    wch->pcdata->switchname,
	    wch->pcdata->title,
	    king, faith);
	  imm = TRUE;
    }

	else if ( wch->level >= 3 && wch->level <= 6 )
    {
          if ( wch->race >= 43 )
          {
            xcatf( buf4, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
              pkratio,
              kav,
              wch->pcdata->switchname,
              king, faith);
		a16 = TRUE;
          }
          else if ( wch->race >= 40 && wch->race <= 42 )
      {
            xcatf( buf6, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king, faith);
		a15 = TRUE;
      }
          else if ( wch->race >= 37 && wch->race <= 39 )
      {
            xcatf( buf7, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king, faith);
		a14 = TRUE;
      }
          else if ( wch->race >= 34 && wch->race <= 36 )
      {
            xcatf( buf8, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king,faith);
		a13 = TRUE;
      }
          else if ( wch->race >= 31 && wch->race <= 33 )
      {
            xcatf( buf9, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king, faith);
		a12 = TRUE;
      }
          else if ( wch->race >= 28 && wch->race <= 30 )
      {
            xcatf( buf10, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king,faith);
		a11 = TRUE;
      }
          else if ( wch->race >= 25 && wch->race <= 27 )
      {
            xcatf( buf11, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king, faith);
		a10 = TRUE;
      }
          else if ( wch->race >= 22 && wch->race <= 24 )
      {
            xcatf( buf12, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf, pkratio, kav,
              wch->pcdata->switchname, king, faith);
			a9 = TRUE;
      }
          else if ( wch->race >= 19 && wch->race <= 21 )
      {
            xcatf( buf13, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king,
faith);
		a8 = TRUE;
      }
          else if ( wch->race >= 16 && wch->race <= 18 )
      {
            xcatf( buf14, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio, 
              kav,
              wch->pcdata->switchname,
              king,
faith);
		a7 = TRUE;
      }
          else if ( wch->race >= 13 && wch->race <= 15 )
      {
            xcatf( buf15, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king,
faith);
		a6 = TRUE;
      }
          else if ( wch->race >= 10 && wch->race <= 12 )
      {
            xcatf( buf16, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king,
faith);
		a5 = TRUE;
      }
          else if ( wch->race >= 7 && wch->race <= 9 )
      {
            xcatf( buf17, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king,
faith);
	    a4 = TRUE;
      }
          else if ( wch->race >= 4 && wch->race <= 6 )
      {
            xcatf( buf18, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king,
faith);
	    a3 = TRUE;
      }
          else if ( wch->race >= 1 && wch->race <= 3 )
      {
            xcatf( buf19, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king,
 faith);
	    a2 = TRUE;
      }
          else if ( wch->race == 0 )
      {
            xcatf( buf20, "%-12s%s%s %-28s%-12s %s %s\n\r",
              class, sexbuf,
		pkratio,
              kav,
              wch->pcdata->switchname,
              king,
faith);
            a1 = TRUE;
      }
	  ava = TRUE;
    }

        else if ( wch->level < 3 )
    {
        xcatf( buf5, "%-12s%s%s %-28s%-12s %s %s\n\r",
            class, sexbuf,
		pkratio,
            kav,
            wch->pcdata->switchname,
            king,
 faith);
          mor = TRUE;
    }

   if ( !mains )
   {
      xprintf(buf, " %s\n\r", get_dystopia_banner("", 76));
  send_to_char(buf, ch);
    send_to_char("                   Mindcloud 2.5 #n\n\r", ch);
      mains = TRUE;
  }

  }

    if ( imm )
  {
      if ( !immortal )
      { 
      xprintf(buf, "\n\r %s\n\r", get_dystopia_banner("#0Gods of #CMindcloud#n", 80));
    send_to_char(buf, ch);
      immortal = TRUE; 
      }
      stc(buf3,ch);
  }         

    if ( ava )
  {
      if ( immortal ) stc("\n\r",ch);
      if ( !avatar )
      { 
      xprintf(buf, " %s\n\r", get_dystopia_banner("    Players     ", 80));
    send_to_char(buf, ch);
      avatar = TRUE; 
      }
      if ( a16 ) stc(buf4, ch);
      if ( a15 ) stc(buf6, ch);
      if ( a14 ) stc(buf7, ch);
      if ( a13 ) stc(buf8, ch);
      if ( a12 ) stc(buf9, ch);
      if ( a11 ) stc(buf10,ch);
      if ( a10 ) stc(buf11,ch);
      if ( a9  ) stc(buf12,ch);
      if ( a8  ) stc(buf13,ch);
      if ( a7  ) stc(buf14,ch);
      if ( a6  ) stc(buf15,ch);
      if ( a5  ) stc(buf16,ch);
      if ( a4  ) stc(buf17,ch);
      if ( a3  ) stc(buf18,ch);
      if ( a2  ) stc(buf19,ch);
      if ( a1  ) stc(buf20,ch);
  }

    if ( mor )
  {
      if ( avatar ) stc("\n\r",ch);
      if ( immortal && !avatar ) stc("\n\r",ch);
      if ( !mortal )
      { 
      xprintf(buf, " %s\n\r", get_dystopia_banner("    Mortals     ", 80));
    send_to_char(buf, ch);
      mortal = TRUE; }
      stc(buf5,ch);
  }
    xprintf(buf, "\n\r %s\n\r", get_dystopia_banner("", 80));
  send_to_char(buf, ch);
  xprintf(buf, "       #C%d#0/#C%d #GVisible players and #C%d #Gvisible immortals connected to %s#n\n\r",
    nPlayerVis, nPlayerAll, nImmVis, MUDNAME);
  send_to_char(buf, ch);
   xprintf(buf, " %s\n\r", get_dystopia_banner("", 80));
  send_to_char(buf, ch);
  return;

}


void do_inventory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    bool found;

    if (!IS_NPC(ch) && IS_HEAD(ch,LOST_HEAD))
        {send_to_char( "You are not a container.\n\r", ch ); return;}
    else if (!IS_NPC(ch) && IS_EXTRA(ch,EXTRA_OSWITCH))
    {
        if ( !IS_NPC(ch) && (obj = ch->pcdata->chobj) == NULL)
	    {send_to_char( "You are not a container.\n\r", ch ); return;}
	switch ( obj->item_type )
	{
	default:
	    send_to_char( "You are not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "You don't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "You are empty.\n\r", ch );
		break;
	    }
	    if (obj->value[1] < obj->value[0] / 5)
	    	xprintf( buf, "There is a little %s liquid left in you.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	xprintf( buf, "You contain a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	xprintf( buf, "You're about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	xprintf( buf, "You're about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	xprintf( buf, "You are almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	xprintf( buf, "You're completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	xprintf( buf, "Somehow you are MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    act( "$p contain:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	send_to_char( where_name[iWear], ch );
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	{
            send_to_char ( "#7[#LE#lmpt#Ly#7]#n\n\r", ch );
        }
	else if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
	else
	{
	    send_to_char( "something.\n\r", ch );
	}
	found = TRUE;
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}


void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}

void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( ( d->connected == CON_PLAYING
	    ||   d->connected == CON_EDITING)
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   victim->pcdata->chobj == NULL
	    &&   can_see( ch, victim ) )
	    {
		found = TRUE;
		xprintf( buf, "%-28s %s\n\r",
		    victim->name, victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		xprintf( buf, "%-28s %s\n\r",
		    PERS(victim, ch), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}


/*
 * Dystopian consider
 */
void do_consider( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
    char *msg;
    int diff;
    int overall;
    int con_hit;
    int con_dam;
    int con_ac;
    int con_hp;

  one_argument( argument, arg );
    overall = 0;

  if ( arg[0] == '\0' )
  {
    send_to_char( "Consider killing whom?\n\r", ch );
    return;
  }

  if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
  {
    send_to_char( "They're not here.\n\r", ch );
    return;
  }

  act( "You examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_CHAR );
  act( "$n examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_NOTVICT );
  act( "$n examines you closely, looking for your weaknesses.", ch, NULL, victim, TO_VICT );

    if (!IS_NPC(victim)) do_skill(ch,victim->name);

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && 
	IS_EXTRA(victim, EXTRA_FAKE_CON))
  {
	con_hit = victim->pcdata->fake_hit;
	con_dam = victim->pcdata->fake_dam;
	con_ac = victim->pcdata->fake_ac;
	con_hp = victim->pcdata->fake_hp;
    }
    else
    {
	con_hit = char_hitroll(victim);
	con_dam = char_damroll(victim);
	con_ac = char_ac(victim);
	con_hp = victim->hit;
  }
    if (con_hp < 1) con_hp = 1;

    diff = victim->level - ch->level + con_hit - char_hitroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more skilled than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as skilled as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as skilled as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as skilled as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more skilled than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more skilled than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more skilled than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = victim->level - ch->level + con_dam - char_damroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more powerful than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as powerful as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as powerful as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as powerful as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more powerful than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more powerful than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more powerful than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = ch->hit * 100 / con_hp;
         if ( diff <=  10 ) {msg = "$E is currently FAR healthier than you are."; overall = overall - 3;}
    else if ( diff <=  50 ) {msg = "$E is currently much healthier than you are."; overall = overall - 2;}
    else if ( diff <=  75 ) {msg = "$E is currently slightly healthier than you are."; overall = overall - 1;}
    else if ( diff <= 125 ) {msg = "$E is currently about as healthy as you are.";}
    else if ( diff <= 200 ) {msg = "You are currently slightly healthier than $M."; overall = overall + 1;}
    else if ( diff <= 500 ) {msg = "You are currently much healthier than $M."; overall = overall + 2;}
    else                    {msg = "You are currently FAR healthier than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = con_ac - char_ac(ch);
         if ( diff <= -100) {msg = "$E is FAR better armoured than you."; overall = overall - 3;}
    else if ( diff <= -50 ) {msg = "$E looks much better armoured than you."; overall = overall - 2;}
    else if ( diff <= -25 ) {msg = "$E looks better armoured than you."; overall = overall - 1;}
    else if ( diff <=  25 ) {msg = "$E seems about as well armoured as you.";}
    else if ( diff <=  50 ) {msg = "You are better armoured than $M."; overall = overall + 1;}
    else if ( diff <=  100) {msg = "You are much better armoured than $M."; overall = overall + 2;}
    else                    {msg = "You are FAR better armoured than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = overall;
         if ( diff <= -11 ) msg = "Conclusion: $E would kill you in seconds.";
    else if ( diff <=  -7 ) msg = "Conclusion: You would need a lot of luck to beat $M.";
    else if ( diff <=  -3 ) msg = "Conclusion: You would need some luck to beat $N.";
    else if ( diff <=   2 ) msg = "Conclusion: It would be a very close fight.";
    else if ( diff <=   6 ) msg = "Conclusion: You shouldn't have a lot of trouble defeating $M.";
    else if ( diff <=  10 ) msg = "Conclusion: $N is no match for you.  You can easily beat $M.";
    else                    msg = "Conclusion: $E wouldn't last more than a few seconds against you.";
    act( msg, ch, NULL, victim, TO_CHAR );

  return;
}


void set_prefix( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->prefix );
    ch->prefix = str_dup( buf );
    return;
}



void do_prefix( CHAR_DATA *ch, char *argument )
{
 
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your prefix to what?\n\r", ch );
	return;
    }
    
    if (!str_cmp(arg, "none"))
    {
    	ch->prefix = str_dup("");
    	send_to_char( "Prefix Set to Default.\n\r", ch);
    	return;
    }
      

    if ( strlen(argument) > 13 )
	argument[13] = '\0';

    smash_tilde( argument );
    xprintf(buf,"#n");
    strcpy(buf,argument);
    set_prefix( ch, argument );
    send_to_char( "Prefix Set.\n\r", ch );
}

void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}
void set_kingtitle( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];
    
    if ( IS_NPC(ch) )
    {
        bug( "Set_title: NPC.", 0 );
        return;
    }
     
    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
        buf[0] = ' ';
        strcpy( buf+1, title );
    }
    else
    {
        strcpy( buf, title );
    }
    
    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );   
    return;
}    


void do_title( CHAR_DATA *ch, char *argument )
{
 
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your title to what?\n\r", ch );
	return;
    }

    if ( strlen(argument) > 25 )
	argument[25] = '\0';

    smash_tilde( argument );
    xprintf(buf,"#n");
    strcpy(buf,argument);
    set_title( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}

void do_kingrank( CHAR_DATA *ch, char *argument )
{
     
    char buf[MAX_STRING_LENGTH];
        
    if ( IS_NPC(ch) )
        return;
    
    if ( argument[0] == '\0' )
    {
        send_to_char( "Change your title to what?\n\r", ch );
        return;
    }   
     
    if ( strlen(argument) > 17 )
        argument[17] = '\0';
 
    smash_tilde( argument );
    xprintf(buf,"#n");
    strcpy(buf,argument);
    set_kingtitle( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}
void do_afk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

	if ( IS_NPC(ch) )
	return;

 
       if (ch->fight_timer>0)
	{
	send_to_char("not with a fighttimer.\n\r",ch);
	return;
	}
       WAIT_STATE(ch,36);
        if (IS_SET(ch->extra, EXTRA_AFK))
	{
	if(ch->tells)
      {
         xprintf( buf, "AFK mode removed.  You have #R%d#n tells waiting.\n\r", ch->tells );
         send_to_char( buf, ch );
         send_to_char("Type 'replay' to see tells.\n\r",ch);
      }
      else
      {
         send_to_char("AFK mode removed.  You have no tells waiting.\n\r",ch);
      }
	xprintf(buf,"%s is back",ch->name);
	do_info(ch,buf);
	REMOVE_BIT(ch->extra,EXTRA_AFK);
	
        return;
	}
  if ((ch->in_room == NULL || !IS_SET(ch->in_room->room_flags, ROOM_SAFE)) && ch->level < 6)
        {
    send_to_char("You can only go AFK in a saferoom.\n\r", ch);
          return;
        }
	if (!IS_SET(ch->extra,EXTRA_AFK))
	{

    if (ch->master)
      stop_follower(ch, FALSE);
	send_to_char( "You are now AFK.\n\r", ch );
	xprintf(buf,"%s is afk",ch->name);
	do_info(ch,buf);
	SET_BIT(ch->extra,EXTRA_AFK);
        return;
	}
   return;
}


void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

    if (strlen2(argument) > 80)
    {
      send_to_char("80 chars per line, thanks.\n\r", ch);
      return;
    }

    if (strlen2(buf) + strlen2(argument) >= MAX_INPUT_LENGTH - 2)
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    char hit_str[MAX_INPUT_LENGTH];
    char mana_str[MAX_INPUT_LENGTH];
    char move_str[MAX_INPUT_LENGTH];
    char mhit_str[MAX_INPUT_LENGTH];
    char mmana_str[MAX_INPUT_LENGTH];
    char mmove_str[MAX_INPUT_LENGTH];
    char exp_str[MAX_INPUT_LENGTH];
    xprintf(hit_str, "%d", ch->hit);
    COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
    xprintf(mana_str, "%d", ch->mana);
    COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
    xprintf(move_str, "%d", ch->move);
    COL_SCALE(move_str, ch, ch->move, ch->max_move);
    xprintf(exp_str, "%d", ch->exp);
    COL_SCALE(exp_str, ch, ch->exp, 1000);
    xprintf(mhit_str, "%d", ch->max_hit);
    ADD_COLOUR(ch, mhit_str, L_CYAN);
    xprintf(mmana_str, "%d", ch->max_mana);
    ADD_COLOUR(ch, mmana_str, L_CYAN);
    xprintf(mmove_str, "%d", ch->max_move);
    ADD_COLOUR(ch, mmove_str, L_CYAN);
    xprintf( buf,
	"You report: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	hit_str,  mhit_str,
	mana_str, mmana_str,
	move_str, mmove_str,
	exp_str   );

    send_to_char( buf, ch );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch == NULL ) continue;
	if ( vch == ch ) continue;
	if ( vch->in_room == NULL ) continue;
	if ( vch->in_room != ch->in_room ) continue;
    	xprintf(hit_str, "%d", ch->hit);
    	COL_SCALE(hit_str, vch, ch->hit, ch->max_hit);
    	xprintf(mana_str, "%d", ch->mana);
    	COL_SCALE(mana_str, vch, ch->mana, ch->max_mana);
    	xprintf(move_str, "%d", ch->move);
    	COL_SCALE(move_str, vch, ch->move, ch->max_move);
    	xprintf(exp_str, "%d", ch->exp);
    	COL_SCALE(exp_str, vch, ch->exp, 1000);
    	xprintf(mhit_str, "%d", ch->max_hit);
    	ADD_COLOUR(vch, mhit_str, L_CYAN);
    	xprintf(mmana_str, "%d", ch->max_mana);
    	ADD_COLOUR(vch, mmana_str, L_CYAN);
    	xprintf(mmove_str, "%d", ch->max_move);
    	ADD_COLOUR(vch, mmove_str, L_CYAN);
    	if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
    	    xprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    ch->morph,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
        else
    	    xprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    IS_NPC(ch) ? capitalize(ch->short_descr) : ch->name,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
	buf[0] = UPPER(buf[0]);
    	send_to_char( buf, vch );
    }
    return;
}



void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;
    
    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
      int col, i;

	col    = 0;
      for (i = 0; i < 5; i++)
      {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    if ( ch->level < skill_table[sn].skill_level )
		continue;
            if (skill_table[sn].target != i)
	    	continue;
            switch(i)
            {
              case 0:
                xprintf( buf, "#p%18s #7%3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
		break;
              case 1:
                xprintf( buf, "#R%18s #7%3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
		break;
              case 2:
                xprintf( buf, "#L%18s #7%3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
		break;
              case 3:
                xprintf( buf, "#G%18s #7%3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
		break;
              case 4:
                xprintf( buf, "#Y%18s #7%3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
                break;
              default:
                xprintf( buf, " ");
                break;
            }
	    send_to_char( buf, ch );
	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
	}
      }
      send_to_char("#n\n\r", ch);

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

	xprintf( buf, "You have %d exp left.\n\r", ch->exp );
	send_to_char( buf, ch );
    }
    else if (!strcmp(argument,"all"))
    {
    for (sn = 0; sn < MAX_SKILL; sn++)
        {
      if (ch->exp < 10000)
        break;
      if ((skill_table[sn].name != NULL) && (ch->level >= skill_table[sn].skill_level) && ch->pcdata->learned[sn] < 100)
        {
                ch->pcdata->learned[sn]     = 100; 
        ch->exp -= 10000;
        }
    }
    send_to_char("Ok.\n\r", ch);
        return;
    }
    else
    {
	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

	if ( ch->exp <= 0 )
	{
	    send_to_char( "You have no exp left.\n\r", ch );
	    return;
	}

	if ( ( sn = skill_lookup( argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   ch->level < skill_table[sn].skill_level ) )
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}
	
	
	if ( ch->pcdata->learned[sn] >= SKILL_ADEPT )
	{
	    xprintf( buf, "You are already an adept of %s.\n\r",
	         skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else if ( ch->pcdata->learned[sn] > 0 &&
		ch->exp < 10000 )
	{
	    xprintf( buf, "You need 10000 exp to increase %s any more.\n\r",
		(skill_table[sn].name));
	    send_to_char( buf, ch );
	}
	else if ( ch->pcdata->learned[sn] == 0 && ch->exp < 10000 )
	{
	    xprintf( buf, "You need 10000 exp to increase %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    if (ch->pcdata->learned[sn] == 0)
	    {
	    	ch->exp -= 10000;
	    	ch->pcdata->learned[sn] += 100;
	    }
	    else
	    {
	    	ch->exp -= (ch->pcdata->learned[sn]/2);
	    	ch->pcdata->learned[sn] += int_app[(get_curr_int(ch)>25)?25:get_curr_int(ch)].learn;	    	
	    }
	    if ( ch->pcdata->learned[sn] < SKILL_ADEPT )
	    {
		act( "You practice $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = SKILL_ADEPT;
		act( "You are now an adept of $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
	    }
	}
    }
    return;
}

void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp( arg1, ch->pcdata->pwd ) &&
#ifdef OLDPFILES
         strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ))
#else
         strcmp( mc_crypt( arg1 ), ch->pcdata->pwd ))
#endif
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */

#ifdef OLDPFILES
    pwdnew = crypt( arg2, ch->pcdata->switchname );
#else
    pwdnew = mc_crypt( arg2 );
#endif

    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    if (!IS_EXTRA(ch,EXTRA_NEWPASS)) SET_BIT(ch->extra,EXTRA_NEWPASS);
    save_char_obj( ch );
    if (ch->desc != NULL && ch->desc->connected == CON_PLAYING )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void do_socials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	xprintf( buf, "%-12s", social_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}



void do_xsocials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; xsocial_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	xprintf( buf, "%-12s", xsocial_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}



void do_spells( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;
    int col;

    col = 0;
    for ( sn = 0; sn < MAX_SKILL && skill_table[sn].name != NULL; sn++ )
    {
	xprintf( buf, "%-12s", skill_table[sn].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level == 0
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    xprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



void do_channels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MSL];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( arg[0] == '\0' )
    {
	send_to_char( "#WChannels#b:#n\n\r", ch );
	send_to_char( "#R-------------------------------------------#n\n\r", ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_CHAT)
	    ? "[+CHAT     ] General Chat Channel ON.\n\r"
	    : "[-chat     ] General Chat Channel OFF.\n\r",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_FLAME)
	    ? "[+FLAME    ] Flame Channel ON.\n\r"
	    : "[-flame    ] Flame Channel OFF.\n\r",
	    ch );
	    
	send_to_char( !IS_SET(ch->deaf, CHANNEL_AUCTION)
	    ? "[+AUCTION  ] You will see auctions as they occur.\n\r"
	    : "[-auction  ] You will NOT see auctions as they occur.\n\r",
	    ch );
	    
        send_to_char( !IS_SET(ch->deaf, CHANNEL_NEWBIE)
            ? "[+NEWBIE   ] Newbie Channel ON.\n\r"
	    : "[-newbie   ] Newbie Channel OFF.\n\r",
            ch );

                send_to_char( !IS_SET(ch->deaf, CHANNEL_ROLEPLAY)
            ? "[+ROLEPLAY ] General Roleplay Channel ON.\n\r"
	    : "[-roleplay ] General Roleplay Channel OFF.\n\r",
            ch );
            
        send_to_char( !IS_SET(ch->deaf, CHANNEL_SOCIAL)
            ? "[+SOCIAL   ] Global Social channel ON.\n\r"
            : "[-social   ] Global Social channel OFF.\n\r",
            ch );
            
         send_to_char( !IS_SET(ch->deaf, CHANNEL_QUEST)
            ? "[+QTALK    ] Quest chat channel ON.\n\r"
            : "[-qtalk    ] Quest chat channel OFF.\n\r",
            ch );
            
          send_to_char( !IS_SET(ch->deaf, CHANNEL_TRIVIA)
            ? "[+TRIVIA   ] Trivia chat channel ON.\n\r"
            : "[-trivia   ] Trivia chat channel OFF.\n\r",
            ch );

	if ( IS_IMMORTAL(ch) )
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_IMMTALK)
	    ? "[+IMMTALK  ] Immortal chat channel ON.\n\r"
	    : "[-immtalk  ] Immortal chat channel OFF.\n\r",
		ch );
	}

	send_to_char( !IS_SET(ch->deaf, CHANNEL_MUSIC)
	    ? "[+MUSIC    ] Music channel ON.\n\r"
	    : "[-music    ] Music channel OFF.\n\r",
	    ch );

        if (IS_SET(ch->special, SPC_HAS_CHOSEN_RELIGION))
        {
          send_to_char( !IS_SET(ch->deaf, CHANNEL_RELIGION)
            ? "[+RELTALK  ] Religion channel ON.\n\r"
	    : "[-reltalk  ] Religion channel OFF.\n\r",
            ch );
        }

        send_to_char( !IS_SET(ch->deaf, CHANNEL_KINGDOM)
            ? "[+KTALK    ] Kingdom chat channel ON.\n\r"
            : "[-ktalk    ] Kingdom chat channel OFF.\n\r",
            ch );

	if (IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_LOG)
	    ? "[+LOG      ] You see LOG entries.\n\r"
	    : "[-log      ] You do NOT see LOG entries.\n\r",
		ch );
	}

	if (ch->class != 0 || IS_IMMORTAL(ch))
	{
		send_to_char( !IS_SET(ch->deaf, CHANNEL_CLASS)
	        ? "[+CLASS    ] You see Class Chat channel.\n\r"
	        : "[-class    ] You do NOT see Class Chat channel.\n\r",
			ch );
	}

	send_to_char( !IS_SET(ch->deaf, CHANNEL_INFO)
	    ? "[+INFO     ] You see INFO messages.\n\r"
	    : "[-info     ] You do NOT see INFO messages.\n\r",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_TELL)
	    ? "[+TELL     ] You see tells from other players.\n\r"
	    : "[-tell     ] You do NOT see tells from other players.\n\r",
	    ch );
	    
    send_to_char( "#R-------------------------------------------#n\n\r", ch );
        xprintf ( buf,
              "#WYour current talk color is #R%d#W.#n\n\r#0--= #BSee the tcolor command#0 =--#n.\n\r",
              ch->talkcolor );
        send_to_char ( buf, ch );
        
        if(!IS_NPC(ch))
        {
        	if(ch->pcdata->pagelen > 0)
        	{
        xprintf( buf, "#0Your pager is set to #W%d#0 lines\n\r", ch->pcdata->pagelen + 2);
        send_to_char( buf, ch);
                }
                else
                {
                send_to_char( "#0Your pager is not set, type #RSCROLL #0 to set\n\r#n", ch);
        }
}
        
        
        

	send_to_char( "#n\n\r", ch );
    }
    else
    {
	bool fClear;
	int bit;

	     if ( arg[0] == '+' ) fClear = TRUE;
	else if ( arg[0] == '-' ) fClear = FALSE;
	else
	{
	    send_to_char( "Channels -channel or +channel?\n\r", ch );
	    return;
	}

        if ( !str_cmp( arg+1, "chat"     ) ) bit = CHANNEL_CHAT;
	else if ( !str_cmp( arg+1, "immtalk"  ) ) bit = CHANNEL_IMMTALK;
        else if ( !str_cmp( arg+1, "flame"  ) ) bit = CHANNEL_FLAME;
	else if ( !str_cmp( arg+1, "music"    ) ) bit = CHANNEL_MUSIC;
        else if ( !str_cmp( arg+1, "newbie"    ) ) bit = CHANNEL_NEWBIE;
        else if ( !str_cmp( arg+1, "roleplay"    ) ) bit = CHANNEL_ROLEPLAY;
	else if ( !str_cmp( arg+1, "yell"     ) ) bit = CHANNEL_YELL;
        else if ( !str_cmp( arg+1, "social"     ) ) bit = CHANNEL_SOCIAL;
	else if ( !str_cmp( arg+1, "class"     ) ) bit = CHANNEL_CLASS;
	else if (IS_IMMORTAL(ch) && !str_cmp( arg+1, "log") ) bit = CHANNEL_LOG;
        else if ( !str_cmp( arg+1, "ktalk"))      bit = CHANNEL_KINGDOM;
        else if (IS_SET(ch->special, SPC_HAS_CHOSEN_RELIGION) && !str_cmp( arg+1, "reltalk")) bit = CHANNEL_RELIGION;
	else if ( !str_cmp( arg+1, "info"     ) ) bit = CHANNEL_INFO;
	else if ( !str_cmp( arg+1, "tell"     ) ) bit = CHANNEL_TELL;
	else if ( !str_cmp( arg+1, "auction"  ) ) bit = CHANNEL_AUCTION;
	else if ( !str_cmp( arg+1, "qtalk"    ) ) bit = CHANNEL_QUEST;
	else if ( !str_cmp( arg+1, "trivia"   ) ) bit = CHANNEL_TRIVIA;
	else if ( !str_cmp( arg+1, "hint"     ) ) bit = CHANNEL_HINT;
	else if ( !str_cmp( arg+1, "social"   ) ) bit = CHANNEL_SOCIAL;
	else
	{
	    send_to_char( "Set or clear which channel?\n\r", ch );
	    return;
	}

	if ( fClear )
	    REMOVE_BIT (ch->deaf, bit);
	else
	    SET_BIT    (ch->deaf, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}



/*
 * Contributed by Grodyn.
 */
void do_config( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
  	int bit;
  	bool fSet;

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "#B[ #0Keyword  #B] #0Option#n\n\r", ch );
        send_to_char( "#R---------------------------\n\r", ch );

	send_to_char(  IS_SET(ch->act, PLR_ANSI)
            ? "#B[#G+ANSI     #B] #0You have ansi colour on.#n\n\r"
	    : "#B[#r-ansi     #B] #0You have ansi colour off.#n\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_BRIEF)
	    ? "#B[#G+BRIEF    #B] #0You see brief descriptions.\n\r"
	    : "#B[#r-brief    #B] #0You see long descriptions.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act, PLR_BRIEF2)
	    ? "#B[#G+BRIEF2   #B] #0You do not see zero damage attacks.\n\r"
	    : "#B[#r-brief2   #B] #0You see zero damage attacks.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act, PLR_BRIEF3)
	    ? "#B[#G+BRIEF3   #B] #0You do not see dodges and parrys.\n\r"
	    : "#B[#r-brief3   #B] #0You see dodges and parrys.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act, PLR_BRIEF4)
	    ? "#B[#G+BRIEF4   #B] #0You do not see class point gains.\n\r"
	    : "#B[#r-brief4   #B] #0You see class point gains.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act, PLR_BRIEF5)
	    ? "#B[#G+BRIEF5   #B] #0You see only amount damage dealt and received.\n\r"
	    : "#B[#r-brief5   #B] #0You see long battle messages.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act, PLR_BRIEF6)
	    ? "#B[#G+BRIEF6   #B] #0You do not see weap/stance/spell advances.\n\r"
	    : "#B[#r-brief6   #B] #0You see weap/stance/spell advances.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act, PLR_BRIEF7)
	    ? "#B[#G+BRIEF7   #B] #0You do not see others battles.\n\r"
	    : "#B[#r-brief7   #B] #0You see others battles.\n\r"
	    , ch );
         
	send_to_char(  IS_SET(ch->act, PLR_COMBINE)
	    ? "#B[#G+COMBINE  #B] #0You see object lists in combined format.\n\r"
	    : "#B[#r-combine  #B] #0You see object lists in single format.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_PROMPT)
	    ? "#B[#G+PROMPT   #B] #0You have a prompt.\n\r"
	    : "#B[#r-prompt   #B] #0You don't have a prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_TELNET_GA)
	    ? "#B[#G+TELNETGA #B] #0You receive a telnet GA sequence.\n\r"
	    : "#B[#r-telnetga #B] #0You don't receive a telnet GA sequence.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_SILENCE)
	    ? "#B[#G+SILENCE  #B] #0You are silenced.\n\r"
	    : ""
	    , ch );

	send_to_char( !IS_SET(ch->act, PLR_NO_TELL)
	    ? ""
	    : "#B[#r-tell     #B] #0You can't use 'tell'.\n\r"
	    , ch );
        send_to_char(IS_SET(ch->act, PLR_MAP)
      ? "#B[#G+MAP      #B] #0You see the automapper.\n\r"
      : "#B[#r-map      #B] #0You don't see the automapper.\n\r", ch);

/* Lets see what messages they have set */
      stc("\n\r   #BYour set messages#n\n\r#0------------------------\n\r", ch);
      xprintf(buf, "#wLogin : #n%s#n\n\r",
      	(IS_SET(ch->pcdata->jflags, JFLAG_SETLOGIN)) ? ch->pcdata->loginmessage : "None");
      stc(buf, ch);
      xprintf(buf, "#wLogout: #n%s#n\n\r",
      	(IS_SET(ch->pcdata->jflags, JFLAG_SETLOGOUT)) ? ch->pcdata->logoutmessage : "None");
      stc(buf, ch);
      xprintf(buf, "#wDecap : #n%s#n\n\r",
				(IS_SET(ch->pcdata->jflags, JFLAG_SETDECAP)) ? ch->pcdata->decapmessage : "None");
      stc(buf, ch);
      xprintf(buf, "#wAvatar: #n%s#n\n\r",
      	(IS_SET(ch->pcdata->jflags, JFLAG_SETAVATAR)) ? ch->pcdata->avatarmessage : "None");
      stc(buf, ch);
      xprintf(buf, "#wTie   : #n%s#n\n\r",
      	(IS_SET(ch->pcdata->jflags, JFLAG_SETTIE)) ? ch->pcdata->tiemessage : "None");
      stc(buf, ch);
    }
    else
    {
    if (arg[0] == '+')
      fSet = TRUE;
    else if (arg[0] == '-')
      fSet = FALSE;
	else
	{
	    send_to_char( "Config -option or +option?\n\r", ch );
	    return;
	}

             if ( !str_cmp( arg+1, "ansi"     ) ) bit = PLR_ANSI;
	else if ( !str_cmp( arg+1, "brief"    ) ) bit = PLR_BRIEF;
        else if ( !str_cmp( arg+1, "brief2"    ) ) bit = PLR_BRIEF2;
        else if ( !str_cmp( arg+1, "brief3"    ) ) bit = PLR_BRIEF3;
        else if ( !str_cmp( arg+1, "brief4"    ) ) bit = PLR_BRIEF4;
        else if ( !str_cmp( arg+1, "brief5"    ) ) bit = PLR_BRIEF5;
        else if ( !str_cmp( arg+1, "brief6"    ) ) bit = PLR_BRIEF6;
        else if ( !str_cmp( arg+1, "brief7"    ) ) bit = PLR_BRIEF7;
        else if ( !str_cmp( arg+1, "prompt"   ) ) bit = PLR_PROMPT;
        else if ( !str_cmp( arg+1, "combine"  ) ) bit = PLR_COMBINE;
	else if ( !str_cmp( arg+1, "telnetga" ) ) bit = PLR_TELNET_GA;
        else if (!str_cmp(arg + 1, "map"))
      bit = PLR_MAP;
	else
	{
	    send_to_char( "Config which option?\n\r", ch );
	    return;
	}

	if ( fSet )
        {
	    SET_BIT    (ch->act, bit);
            xprintf(buf,"%s is now ON\n\r",arg+1);
            send_to_char(buf,ch);
        }
	else
        {
	    REMOVE_BIT (ch->act, bit);
            xprintf(buf,"%s is now OFF\n\r",arg+1);
            send_to_char(buf,ch);
        }
    }

    return;
}

void do_autoset( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
  int bit;
  bool fSet;

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "#B[ #0Keyword  #B] #0Option#n\n\r", ch );
        send_to_char( "#R---------------------------\n\r", ch );

	send_to_char(  IS_SET(ch->act2, PLR_AUTOEXIT)
            ? "#B[#G+AUTOEXIT #B] #0You automatically see exits.\n\r"
	    : "#B[#r-autoexit #B] #0You don't automatically see exits.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act2, PLR_AUTOLOOT)
	    ? "#B[#G+AUTOLOOT #B] #0You automatically loot corpses.\n\r"
	    : "#B[#r-autoloot #B] #0You don't automatically loot corpses.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act2, PLR_AUTOSLAB)
	    ? "#B[#G+AUTOSLAB #B] #0You automatically loot slabs from corpses.\n\r"
	    : "#B[#r-autoslab #B] #0You don't automatically loot slabs from corpses.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act2, PLR_AUTOBONES)
	    ? "#B[#G+AUTOBONES#B] #0You automatically grab bones after battle.\n\r"
	    : "#B[#r-autobones#B] #0You don't automatically grab bones after battle.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act2, PLR_AUTOSAC)
	    ? "#B[#G+AUTOSAC  #B] #0You automatically sacrifice corpses.\n\r"
	    : "#B[#r-autosac  #B] #0You don't automatically sacrifice corpses.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act2, PLR_AUTOHEAD)
	    ? "#B[#G+AUTOHEAD #B] #0You automatically sacrifice heads.\n\r"
	    : "#B[#r-autohead #B] #0You don't automatically sacrifice heads.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act2, PLR_AUTOHILT)
	    ? "#B[#G+AUTOHILT #B] #0You automatically loot hilts.\n\r"
	    : "#B[#r-autohilt #B] #0You don't automatically loot hilts.\n\r"
	    , ch );
	    
	send_to_char(  IS_SET(ch->act2, PLR_AUTOGEM)
	    ? "#B[#G+AUTOGEM  #B] #0You automatically loot gems.\n\r"
	    : "#B[#r-autogem  #B] #0You don't automatically loot gems.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act2, PLR_AUTOSTORE)
	    ? "#B[#G+AUTOSTORE#B] #0You automatically store tells during fights.\n\r"
	    : "#B[#r-autostore#B] #0You don't automatically store tells during fights.\n\r"
	    , ch );	
    }
    else
    {
    if (arg[0] == '+')
      fSet = TRUE;
    else if (arg[0] == '-')
      fSet = FALSE;
	else
	{
	    send_to_char( "autoset -option or +option?\n\r", ch );
	    return;
	}
    	 if ( !str_cmp( arg+1, "autoexit" ) ) bit = PLR_AUTOEXIT;
	else if ( !str_cmp( arg+1, "autoloot" ) ) bit = PLR_AUTOLOOT;
	else if ( !str_cmp( arg+1, "autosac"  ) ) bit = PLR_AUTOSAC;
	else if ( !str_cmp( arg+1, "autobones") ) bit = PLR_AUTOBONES;
	else if ( !str_cmp( arg+1, "autohead" ) ) bit = PLR_AUTOHEAD;
	else if ( !str_cmp( arg+1, "autoslab" ) ) bit = PLR_AUTOSLAB;
	else if ( !str_cmp( arg+1, "autogem" ) )  bit =  PLR_AUTOGEM;
	else if ( !str_cmp( arg+1, "autohilt" ) ) bit = PLR_AUTOHILT;
	else if ( !str_cmp( arg+1, "autostore" ) ) bit = PLR_AUTOSTORE;
	else
	{
	    send_to_char( "Config which option?\n\r", ch );
	    return;
	}

	if ( fSet )
        {
	    SET_BIT    (ch->act2, bit);
            xprintf(buf,"%s is now ON\n\r",arg+1);
            send_to_char(buf,ch);
        }
	else
        {
	    REMOVE_BIT (ch->act2, bit);
            xprintf(buf,"%s is now OFF\n\r",arg+1);
            send_to_char(buf,ch);
        }
    }

    return;
}

void do_ansi( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_ANSI)) do_config(ch,"-ansi");
    else do_config(ch,"+ansi");
    return;
}

void do_autoexit( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act2, PLR_AUTOEXIT)) do_autoset(ch,"-autoexit");
    else do_autoset(ch,"+autoexit");
    return;
}

void do_autoloot( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act2, PLR_AUTOLOOT)) do_autoset(ch,"-autoloot");
    else do_autoset(ch,"+autoloot");
    return;
}

void do_autoslab( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act2, PLR_AUTOSLAB)) do_autoset(ch,"-autoslab");
    else do_autoset(ch,"+autoslab");
    return;
}

void do_autohead( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act2, PLR_AUTOHEAD)) do_autoset(ch,"-autohead");
    else do_autoset(ch,"+autohead");
    return;
}

void do_autogem( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act2, PLR_AUTOGEM)) do_autoset(ch,"-autogem");
    else do_autoset(ch,"+autogem");
    return;
}

void do_autohilt( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act2, PLR_AUTOHILT)) do_autoset(ch,"-autohilt");
    else do_autoset(ch,"+autohilt");
    return;
}

void do_autostore( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act2, PLR_AUTOSTORE)) do_autoset(ch,"-autostore");
    else do_autoset(ch,"+autostore");
    return;
}

void do_autosac( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act2, PLR_AUTOSAC)) do_autoset(ch,"-autosac");
    else do_autoset(ch,"+autosac");
    return;
}

void do_map(CHAR_DATA * ch, char *argument)
{
  if (IS_NPC(ch))
    return;
  if (IS_SET(ch->act, PLR_MAP))
    do_config(ch, "-map");
  else
    do_config(ch, "+map");
  return;
}

void do_autoall( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (!IS_SET(ch->act2, PLR_AUTOBONES)) do_autoset(ch,"+autobones");
    if (!IS_SET(ch->act2, PLR_AUTOEXIT)) do_autoset(ch,"+autoexit");
    if (!IS_SET(ch->act2, PLR_AUTOHILT)) do_autoset(ch,"+autohilt");
    if (!IS_SET(ch->act2, PLR_AUTOHEAD)) do_autoset(ch,"+autohead");
    if (!IS_SET(ch->act2, PLR_AUTOSAC)) do_autoset(ch,"+autosac");
    if (!IS_SET(ch->act2, PLR_AUTOGEM)) do_autoset(ch,"+autogem");
    if (!IS_SET(ch->act2, PLR_AUTOSLAB)) do_autoset(ch,"+autoslab");
    if (!IS_SET(ch->act2, PLR_AUTOSTORE)) do_autoset(ch,"+autostore");
}

void do_autobones( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act2, PLR_AUTOBONES)) do_autoset(ch,"-autobones");
    else do_autoset(ch,"+autobones");
    return;
}

void do_brief( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BRIEF)) do_config(ch,"-brief");
    else do_config(ch,"+brief");
    send_to_char("Please read HELP BRIEF.\n\r",ch);
    return;
}

void do_brief2( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BRIEF2)) do_config(ch,"-brief2");
    else do_config(ch,"+brief2");
    return;
}

void do_brief3( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BRIEF3)) do_config(ch,"-brief3");
    else do_config(ch,"+brief3");
    return;
}

void do_brief4( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch))
    return;
  if (IS_SET(ch->act, PLR_BRIEF4))
    do_config(ch, "-brief4");
  else
    do_config(ch, "+brief4");
  return;
}

void do_brief5(CHAR_DATA * ch, char *argument)
{
  if (IS_NPC(ch))
    return;
  if (IS_SET(ch->act, PLR_BRIEF5))
    do_config(ch, "-brief5");
  else
    do_config(ch, "+brief5");
  return;
}


void do_brief6(CHAR_DATA * ch, char *argument)
{
  if (IS_NPC(ch))
    return;
  if (IS_SET(ch->act, PLR_BRIEF6))
    do_config(ch, "-brief6");
  else
    do_config(ch, "+brief6");
  return;
}

void do_brief7(CHAR_DATA * ch, char *argument)
{
  if (IS_NPC(ch))
    return;
  if (IS_SET(ch->act, PLR_BRIEF7))
    do_config(ch, "-brief7");
  else
    do_config(ch, "+brief7");
  return;
}

void do_diagnose( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg  [MAX_INPUT_LENGTH];
    int teeth = 0;
    int ribs = 0;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (arg == '\0')
    {
	send_to_char("Who do you wish to diagnose?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char("Nobody here by that name.\n\r",ch);
	return;
    }
    act("$n examines $N carefully, diagnosing $S injuries.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you carefully, diagnosing your injuries.",ch,NULL,victim,TO_VICT);
    act("Your diagnoses of $N reveals the following...",ch,NULL,victim,TO_CHAR);
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    if ( ( victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + 
	   victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
	   victim->loc_hp[6] ) == 0 )
    {
	act("$N has no apparent injuries.",ch,NULL,victim,TO_CHAR);
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
	return;
    }
    /* Check head */
    if (IS_HEAD(victim,LOST_EYE_L) && IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost both of $S eyes.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_L))
	act("$N has lost $S left eye.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost $S right eye.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_EAR_L) && IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost both of $S ears.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_L))
	act("$N has lost $S left ear.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost $S right ear.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_NOSE))
	act("$N has lost $S nose.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,BROKEN_NOSE))
	act("$N has got a broken nose.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,BROKEN_JAW))
	act("$N has got a broken jaw.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
	act("$N has had $S head cut off.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HEAD))
	    act("...Blood is spurting from the stump of $S neck.",ch,NULL,victim,TO_CHAR);
    }
    else
    {
	if (IS_BODY(victim,BROKEN_NECK))
	    act("$N has got a broken neck.",ch,NULL,victim,TO_CHAR);
	if (IS_BODY(victim,CUT_THROAT))
	{
	    act("$N has had $S throat cut open.",ch,NULL,victim,TO_CHAR);
	    if (IS_BLEEDING(victim,BLEEDING_THROAT))
		act("...Blood is pouring from the wound.",ch,NULL,victim,TO_CHAR);
	}
    }
    if (IS_HEAD(victim,BROKEN_SKULL))
	act("$N has got a broken skull.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_TOOTH_1 )) teeth += 1;
    if (IS_HEAD(victim,LOST_TOOTH_2 )) teeth += 2;
    if (IS_HEAD(victim,LOST_TOOTH_4 )) teeth += 4;
    if (IS_HEAD(victim,LOST_TOOTH_8 )) teeth += 8;
    if (IS_HEAD(victim,LOST_TOOTH_16)) teeth += 16;
    if (teeth > 0)
    {
	xprintf(buf,"$N has had %d teeth knocked out.",teeth);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_HEAD(victim,LOST_TONGUE))
	act("$N has had $S tongue ripped out.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
    	send_to_char("----------------------------------------------------------------------------\n\r",ch);
    	return;
    }
    /* Check body */
    if (IS_BODY(victim,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(victim,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(victim,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(victim,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(victim,BROKEN_RIBS_16)) ribs += 16;
    if (ribs > 0)
    {
	xprintf(buf,"$N has got %d broken ribs.",ribs);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_BODY(victim,BROKEN_SPINE))
	act("$N has got a broken spine.",ch,NULL,victim,TO_CHAR);
    /* Check arms */
    check_left_arm(ch,victim);
    check_right_arm(ch,victim);
    check_left_leg(ch,victim);
    check_right_leg(ch,victim);
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    return;
}

void check_left_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S arms.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L) && IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_ARM))
    {
	act("$N has lost $S left arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N arms are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,BROKEN_ARM))
	act("$N's left arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND) &&
	!IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S hands.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L) && IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_HAND))
    {
	act("$N has lost $S left hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) xprintf(finger,"finger");
    else xprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,LOST_THUMB))
    {
	xprintf(buf,"$N has lost %d %s and $S thumb from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	xprintf(buf,"$N has lost %d %s from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,LOST_THUMB))
    {
	xprintf(buf,"$N has lost the thumb from $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_L(victim,BROKEN_FINGER_I) && !IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_M) && !IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_R) && !IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_L) && !IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) xprintf(finger,"finger");
    else xprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	xprintf(buf,"$N has broken %d %s and $S thumb on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	xprintf(buf,"$N has broken %d %s on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	xprintf(buf,"$N has broken the thumb on $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	return;
    if (IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost $S right arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND))
	return;
    if (IS_ARM_R(victim,LOST_HAND))
    {
	act("$N has lost $S right hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) xprintf(finger,"finger");
    else xprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,LOST_THUMB))
    {
	xprintf(buf,"$N has lost %d %s and $S thumb from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	xprintf(buf,"$N has lost %d %s from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,LOST_THUMB))
    {
	xprintf(buf,"$N has lost the thumb from $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_R(victim,BROKEN_FINGER_I) && !IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_M) && !IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_R) && !IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_L) && !IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) xprintf(finger,"finger");
    else xprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	xprintf(buf,"$N has broken %d %s and $S thumb on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	xprintf(buf,"$N has broken %d %s on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	xprintf(buf,"$N has broken the thumb on $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_left_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost both of $S legs.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L) && IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_LEG))
    {
	act("$N has lost $S left leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N legs are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_LEG_L(victim,BROKEN_LEG))
	act("$N's left leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost both of $S feet.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L) && IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_FOOT))
    {
	act("$N has lost $S left foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) xprintf(toe,"toe");
    else xprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,LOST_TOE_BIG))
    {
	xprintf(buf,"$N has lost %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	xprintf(buf,"$N has lost %d %s from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,LOST_TOE_BIG))
    {
	xprintf(buf,"$N has lost the big toe from $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_L(victim,BROKEN_TOE_A) && !IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_B) && !IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_C) && !IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_D) && !IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) xprintf(toe,"toe");
    else xprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	xprintf(buf,"$N has broken %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	xprintf(buf,"$N has broken %d %s on $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	xprintf(buf,"$N has broken the big toe on $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	return;
    if (IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost $S right leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N's right leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
	return;
    if (IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost $S right foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) xprintf(toe,"toe");
    else xprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,LOST_TOE_BIG))
    {
	xprintf(buf,"$N has lost %d %s and $S big toe from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	xprintf(buf,"$N has lost %d %s from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,LOST_TOE_BIG))
    {
	xprintf(buf,"$N has lost the big toe from $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_R(victim,BROKEN_TOE_A) && !IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_B) && !IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_C) && !IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_D) && !IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) xprintf(toe,"toe");
    else xprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	xprintf(buf,"$N has broken %d %s and $S big toe on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	xprintf(buf,"$N has broken %d %s on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	xprintf(buf,"$N has broken the big toe on $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void obj_score( CHAR_DATA *ch, OBJ_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int itemtype;

    /*
     * very anoying bug with peoples heads not fading.
     */
  if (!obj)
    return;
  if (!obj->short_descr)
    return;

    xprintf( buf,"You are %s.\n\r",obj->short_descr);
    send_to_char( buf, ch );

  xprintf(buf, "Type %s, Extra flags %s.\n\r", item_type_name(obj), extra_bit_name(obj->extra_flags));
    send_to_char( buf, ch );

    xprintf( buf,"You weigh %d pounds and are worth %d gold coins.\n\r",obj->weight,obj->cost);
    send_to_char( buf, ch );

  if (obj->questmaker != NULL && strlen(obj->questmaker) > 1 && obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	xprintf( buf, "You were created by %s, and are owned by %s.\n\r", obj->questmaker,obj->questowner );
	send_to_char( buf, ch );
    }
    else if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	xprintf( buf, "You were created by %s.\n\r", obj->questmaker );
	send_to_char( buf, ch );
    }
    else if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	xprintf( buf, "You are owned by %s.\n\r", obj->questowner );
	send_to_char( buf, ch );
    }

    switch ( obj->item_type )
    {
    case ITEM_SCROLL: 
    case ITEM_POTION:
	xprintf( buf, "You contain level %d spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_QUEST:
	xprintf( buf, "Your quest point value is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_WAND: 
    case ITEM_STAFF: 
	xprintf( buf, "You have %d(%d) charges of level %d",
	    obj->value[1], obj->value[2], obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;
      
    case ITEM_WEAPON:
	xprintf( buf, "You inflict %d to %d damage in combat (average %d).\n\r",
	    obj->value[1], obj->value[2],
	    ( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] - ((obj->value[0] / 1000) * 1000);
	else
	    itemtype = obj->value[0];

	if (itemtype > 0)
	{
	    if (obj->level < 10)
		xprintf(buf,"You are a minor spell weapon.\n\r");
	    else if (obj->level < 20)
		xprintf(buf,"You are a lesser spell weapon.\n\r");
	    else if (obj->level < 30)
		xprintf(buf,"You are an average spell weapon.\n\r");
	    else if (obj->level < 40)
		xprintf(buf,"You are a greater spell weapon.\n\r");
	    else if (obj->level < 50)
		xprintf(buf,"You are a major spell weapon.\n\r");
	    else
		xprintf(buf,"You are a supreme spell weapon.\n\r");
	    send_to_char(buf,ch);
	}

	if (itemtype == 1)
	    xprintf (buf, "You are dripping with corrosive acid.\n\r");
	else if (itemtype == 4)
	    xprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 30)
	    xprintf (buf, "You are the bane of all evil.\n\r");
	else if (itemtype == 34)
	    xprintf (buf, "You drink the souls of your victims.\n\r");
	else if (itemtype == 37)
	    xprintf (buf, "You have been tempered in hellfire.\n\r");
	else if (itemtype == 48)
	    xprintf (buf, "You crackle with sparks of lightning.\n\r");
	else if (itemtype == 53)
	    xprintf (buf, "You are dripping with a dark poison.\n\r");
	else if (itemtype > 0)
	    xprintf (buf, "You have been imbued with the power of %s.\n\r",skill_table[itemtype].name);
	if (itemtype > 0)
	    send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] / 1000;
	else
	    break;

	if (itemtype == 4 || itemtype == 1)
	    xprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 27 || itemtype == 2)
	    xprintf (buf, "You allow your wielder to see invisible things.\n\r");
	else if (itemtype == 39 || itemtype == 3)
	    xprintf (buf, "You grant your wielder the power of flight.\n\r");
	else if (itemtype == 45 || itemtype == 4)
	    xprintf (buf, "You allow your wielder to see in the dark.\n\r");
	else if (itemtype == 46 || itemtype == 5)
	    xprintf (buf, "You render your wielder invisible to the human eye.\n\r");
	else if (itemtype == 52 || itemtype == 6)
	    xprintf (buf, "You allow your wielder to walk through solid doors.\n\r");
	else if (itemtype == 54 || itemtype == 7)
	    xprintf (buf, "You protect your wielder from evil.\n\r");
	else if (itemtype == 57 || itemtype == 8)
	    xprintf (buf, "You protect your wielder in combat.\n\r");
	else if (itemtype == 9)
	    xprintf (buf, "You allow your wielder to walk in complete silence.\n\r");
	else if (itemtype == 10)
	    xprintf (buf, "You surround your wielder with a shield of lightning.\n\r");
	else if (itemtype == 11)
	    xprintf (buf, "You surround your wielder with a shield of fire.\n\r");
	else if (itemtype == 12)
	    xprintf (buf, "You surround your wielder with a shield of ice.\n\r");
	else if (itemtype == 13)
	    xprintf (buf, "You surround your wielder with a shield of acid.\n\r");
	else if (itemtype == 14)
	    xprintf (buf, "You channel the power of god into your wielder.\n\r");
	else if (itemtype == 15)
	    xprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (itemtype == 16)
	    xprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (itemtype == 17)
	    xprintf (buf, "You enable your wielder to move at supernatural speed.\n\r");
	else if (itemtype == 18)
	    xprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (itemtype == 19)
	    xprintf (buf, "You protect your wielder from player attacks.\n\r");
	else if (itemtype == 20)
	    xprintf (buf, "You surround your wielder with a shield of darkness.\n\r");
	else if (itemtype == 21)
	    xprintf (buf, "You grant your wielder superior protection.\n\r");
	else if (itemtype == 22)
	    xprintf (buf, "You grant your wielder supernatural vision.\n\r");
	else if (itemtype == 23)
	    xprintf (buf, "You make your wielder fleet-footed.\n\r");
	else if (itemtype == 24)
	    xprintf (buf, "You conceal your wielder from sight.\n\r");
	else if (itemtype == 25)
	    xprintf (buf, "You invoke the power of your wielders beast.\n\r");
	else
	    xprintf (buf, "You are bugged...please report it.\n\r");
	if (itemtype > 0)
	    send_to_char( buf, ch );
	break;

    case ITEM_ARMOR:
	xprintf( buf, "Your armor class is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	if (obj->value[3] < 1)
	    break;
	if (obj->value[3] == 4 || obj->value[3] == 1)
	    xprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (obj->value[3] == 27 || obj->value[3] == 2)
	    xprintf (buf, "You allow your wearer to see invisible things.\n\r");
	else if (obj->value[3] == 39 || obj->value[3] == 3)
	    xprintf (buf, "You grant your wearer the power of flight.\n\r");
	else if (obj->value[3] == 45 || obj->value[3] == 4)
	    xprintf (buf, "You allow your wearer to see in the dark.\n\r");
	else if (obj->value[3] == 46 || obj->value[3] == 5)
	    xprintf (buf, "You render your wearer invisible to the human eye.\n\r");
	else if (obj->value[3] == 52 || obj->value[3] == 6)
	    xprintf (buf, "You allow your wearer to walk through solid doors.\n\r");
	else if (obj->value[3] == 54 || obj->value[3] == 7)
	    xprintf (buf, "You protect your wearer from evil.\n\r");
	else if (obj->value[3] == 57 || obj->value[3] == 8)
	    xprintf (buf, "You protect your wearer in combat.\n\r");
	else if (obj->value[3] == 9)
	    xprintf (buf, "You allow your wearer to walk in complete silence.\n\r");
	else if (obj->value[3] == 10)
	    xprintf (buf, "You surround your wearer with a shield of lightning.\n\r");
	else if (obj->value[3] == 11)
	    xprintf (buf, "You surround your wearer with a shield of fire.\n\r");
	else if (obj->value[3] == 12)
	    xprintf (buf, "You surround your wearer with a shield of ice.\n\r");
	else if (obj->value[3] == 13)
	    xprintf (buf, "You surround your wearer with a shield of acid.\n\r");
	else if (obj->value[3] == 14)
	    xprintf (buf, "You protect your wearer from attacks from DarkBlade clan guardians.\n\r");
	else if (obj->value[3] == 15)
	    xprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (obj->value[3] == 16)
	    xprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (obj->value[3] == 17)
	    xprintf (buf, "You enable your wearer to move at supernatural speed.\n\r");
	else if (obj->value[3] == 18)
	    xprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (obj->value[3] == 19)
	    xprintf (buf, "You protect your wearer from player attacks.\n\r");
	else if (obj->value[3] == 20)
	    xprintf (buf, "You surround your wearer with a shield of darkness.\n\r");
	else if (obj->value[3] == 21)
	    xprintf (buf, "You grant your wearer superior protection.\n\r");
	else if (obj->value[3] == 22)
	    xprintf (buf, "You grant your wearer supernatural vision.\n\r");
	else if (obj->value[3] == 23)
	    xprintf (buf, "You make your wearer fleet-footed.\n\r");
	else if (obj->value[3] == 24)
	    xprintf (buf, "You conceal your wearer from sight.\n\r");
	else if (obj->value[3] == 25)
	    xprintf (buf, "You invoke the power of your wearers beast.\n\r");
	else
	    xprintf (buf, "You are bugged...please report it.\n\r");
	if (obj->value[3] > 0)
	    send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    xprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    xprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }
    return;
}

/* Do_prompt from Morgenes from Aldara Mud */
void do_prompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "prompt" );
       return;
   }

   if( !strcmp( argument, "on" ) )
   {
      if (IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt on!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         SET_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "off" ) )
   {
      if (!IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt off!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         REMOVE_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->prompt);
      ch->prompt = str_dup( "" );
      return;
   }
   else
   {
    if (strlen(argument) > 80)
      argument[80] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 

/* Do_prompt from Morgenes from Aldara Mud */
void do_cprompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "cprompt" );
       return;
   }

   if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->cprompt);
      ch->cprompt = str_dup( "" );
      return;
   }
   else
   {
    if (strlen(argument) > 80)
      argument[80] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->cprompt );
   ch->cprompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 


void do_finger( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_STRING_LENGTH];
  char strsave[MAX_INPUT_LENGTH];
  char *buf;
  char buf2[MAX_INPUT_LENGTH];
  FILE *fp; 
  int num;
  int num2;
  int extra;
    
  if ( IS_NPC(ch) ) return;
  one_argument(argument,arg);
    
  if (!check_parse_name( argument ))
  {
    send_to_char( "Thats an illegal name.\n\r", ch );
    return;
  }
  if ( !char_exists(TRUE,argument) )
  {
    send_to_char( "That player doesn't exist.\n\r", ch );
    return;
  }
  fclose( fpReserve );
  xprintf( strsave, "%s%c/backup/%s", PLAYER_DIR, tolower( argument[0]), capitalize(argument) );
  if ( ( fp = fopen( strsave, "r" ) ) != NULL )
  {
    xprintf(buf2, "%s\n\r", get_dystopia_banner(MUDNAME, 68));
    send_to_char(buf2, ch);
    buf=fread_string( fp );
    send_to_char(buf,ch);
    send_to_char(" ",ch);
    buf=fread_string( fp );
    send_to_char(buf,ch);
    if (ch->level > 6) send_to_char(" @",ch);
    buf=fread_string( fp );
    if (ch->level > 6) send_to_char(buf,ch);
    xprintf(buf2, "\n\r%s\n\r", get_dystopia_banner("", 70));
    send_to_char(buf2, ch);
    send_to_char("Last connected ",ch);
    send_to_char("at ",ch);
    buf=fread_string( fp );
    send_to_char(buf,ch);
    xprintf(buf2, ".\n\r%s\n\r", get_dystopia_banner("", 70));
    send_to_char(buf2, ch);  
    extra=fread_number( fp );
    num=fread_number( fp );
    send_to_char("Sex: ",ch);
    buf=fread_string( fp );
    if (num == SEX_MALE) send_to_char("Male. ",ch);
    else if (num == SEX_FEMALE)
    {
      send_to_char("Female. ",ch);
      if (IS_SET(extra, EXTRA_PREGNANT)) other_age(ch,extra,TRUE,buf);
    }
    else send_to_char("None. ",ch);
    buf=fread_string( fp );
    other_age(ch,extra,FALSE,buf);
    num=fread_number( fp );
    num2=fread_number( fp );
    switch ( num )
    {
      default:
        xprintf(buf2,"They are mortal, ");
        break;
      case LEVEL_AVATAR:   
        {
		if (num2 < 1)
                    xprintf(buf2,"They are a Piss Ant with #C%d#n status, ",num2);
		else if (num2 < 4)
                    xprintf(buf2,"They are a Squire with #C%d#n status, ",num2);
		else if (num2 < 7)
                    xprintf(buf2,"They are an Apprentice with #C%d#n status, ",num2);
		else if (num2 < 10)
                    xprintf(buf2,"They are a Scout with #C%d#n status, ",num2);
		else if (num2 < 13)
                    xprintf(buf2,"They are a Soldier with #C%d#n status, ",num2);
		else if (num2 < 16)
                    xprintf(buf2,"They are a Lieutenant with #C%d#n status, ",num2);
                else if (num2 < 19)
                    xprintf(buf2,"They are a Captain with #C%d#n status, ",num2);
                else if (num2 < 22)
                    xprintf(buf2,"They are a Guard with #C%d#n status, ",num2);
		else if (num2 < 25)
                    xprintf(buf2,"They are a Knight with #C%d#n status, ",num2);
  		else if (num2 < 28)
                    xprintf(buf2,"They are a Ranger with #C%d#n status, ",num2);
  		else if (num2 < 31)
                    xprintf(buf2,"They are a Hunter with #C%d#n status, ",num2);
		else if (num2 < 34)
                    xprintf(buf2,"They are an Assassin with #C%d#n status, ",num2);
		else if (num2 < 37)
                    xprintf(buf2,"They are a Lord with #C%d#n status, ",num2);
		else if (num2 < 40)
                    xprintf(buf2,"They are a Baron with #C%d#n status, ",num2);
		else if (num2 < 43)
                    xprintf(buf2,"They are a Duke with #C%d#n status, ",num2);
		else 
                    xprintf(buf2,"They are a PRINCE with #C%d#n status, ",num2);
		break;
	    }
      case LEVEL_BUILDER:  
      case LEVEL_QUESTMAKER:
      case LEVEL_ENFORCER:
      case LEVEL_JUDGE:
      case LEVEL_HIGHJUDGE:
      case LEVEL_IMPLEMENTOR:
        xprintf(buf2,"They are a God, ");
        break;
    }
    send_to_char(buf2,ch);   
    num=fread_number( fp );
    if (num > 0) num2 = (num / 3600); else num2 = 0;
    xprintf(buf2,"and have been playing for %d hours.\n\r",num2);
    send_to_char(buf2,ch);
    buf=fread_string( fp );
    if (strlen(buf) > 2)
    {
      if (IS_SET(extra, EXTRA_MARRIED))
        xprintf(buf2,"They are married to %s.\n\r",buf);
      else
        xprintf(buf2,"They are engaged to %s.\n\r",buf);
      send_to_char(buf2,ch);
    }
    num=fread_number( fp );
    num2=fread_number( fp );
    xprintf(buf2,"Player kills: %d, Player Deaths: %d.\n\r",num,num2);
    send_to_char(buf2,ch);
    num=fread_number( fp );
    num2=fread_number( fp );
    xprintf(buf2,"Arena kills: %d, Arena Deaths: %d.\n\r",num,num2);
    send_to_char(buf2,ch);
    num=fread_number(fp);  
    num2=fread_number(fp);  
    xprintf(buf2,"Mob Kills: %d, Mob Deaths: %d.\n\r",num,num2);
    send_to_char(buf2,ch);
    xprintf(buf2, "%s\n\r", get_dystopia_banner("", 70));
    send_to_char(buf2, ch);
  }
  else
  {
    bug( "Do_finger: fopen", 0 );
  }
  fclose( fp );
  fpReserve = fopen( NULL_FILE, "r" );
  return;
}

void do_viewtier( CHAR_DATA *ch, char *argument )
{
       	char buf0[MAX_STRING_LENGTH];
	char buf1[MAX_STRING_LENGTH];
       	char buf2[MAX_STRING_LENGTH];
       	char buf3[MAX_STRING_LENGTH];
       	char buf4[MAX_STRING_LENGTH];
       	char buf5[MAX_STRING_LENGTH];
       	char buf6[MAX_STRING_LENGTH];
       	char buf7[MAX_STRING_LENGTH];
       	char buf8[MAX_STRING_LENGTH];
       	char buf9[MAX_STRING_LENGTH];
       	char buf10[MAX_STRING_LENGTH];
	bool t0 = FALSE;
	bool t1 = FALSE;
	bool t2 = FALSE;
	bool t3 = FALSE;
	bool t4 = FALSE;
	bool t5 = FALSE;
	bool t6 = FALSE;
	bool t7 = FALSE;
	bool t8 = FALSE;
	bool t9 = FALSE;
       bool t10 = FALSE;	
	DESCRIPTOR_DATA *d;
	DESCRIPTOR_DATA *d2;
	

        if (IS_NPC(ch)) return;


	for (d = descriptor_list; d != NULL; d = d->next)
	{
	if (d->connected != CON_PLAYING) continue;
	if (d->character == NULL) continue;
	if (d->character->level > 6) continue;
        if (d->character->tier == 10) t10 = TRUE;
  	if (d->character->tier == 9)  t9 = TRUE;
	if (d->character->tier == 8)  t8 = TRUE;
        if (d->character->tier == 7)  t7 = TRUE;
  	if (d->character->tier == 6)  t6 = TRUE;
	if (d->character->tier == 5)  t5 = TRUE;
	if (d->character->tier == 4)  t4 = TRUE;
	if (d->character->tier == 3)  t3 = TRUE;
	if (d->character->tier == 2)  t2 = TRUE;
	if (d->character->tier == 1)  t1 = TRUE;
	if (d->character->tier <= 0)  t0 = TRUE;
	}
	for (d = descriptor_list; d != NULL; d = d->next)
	{
		CHAR_DATA *wch;
		if (d->connected != CON_PLAYING) continue;
		if (d->character == NULL) continue;
		if (d->character->level > 6) continue;
		wch = ( d->original != NULL) ? d->original : d->character;
                
       if (t10 == TRUE){       
		stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n  #RT#rier #RX#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->invis_level > get_trust(ch)) continue;
        if (d2->character->tier >= 10)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf10, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf10,ch);
		}
		}


 	
	if (t9 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n  #RT#rier #RIX#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
     	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
	if (d2->character->invis_level > get_trust(ch)) continue;
   if (d2->character->tier == 9)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf9, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf9,ch);
		}
		}

	if (t8 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n #RT#rier #RIIX#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
       	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
	if (d2->character->invis_level > get_trust(ch)) continue;
 if (d2->character->tier == 8)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf8, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf8,ch);
		}
		}
	
	if (t7 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n #RT#rier #RVII#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
     	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
	if (d2->character->invis_level > get_trust(ch)) continue;
   if (d2->character->tier == 7)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf7, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf7,ch);
		}
		}

	if (t6 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n  #RT#rier #RVI#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
       	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
 if (d2->character->tier == 6)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf6, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf6,ch);
		}
		}

	if (t5 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n  #RT#rier #RV#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
        if (d2->character->tier == 5)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf5, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf5,ch);
		}
		}

	if (t4 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n  #RT#rier #RIV#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
        if (d2->character->tier == 4)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf4, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf4,ch);
		}
		}

	if (t3 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n #RT#rier #RIII#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
        if (d2->character->tier == 3)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf3, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf3,ch);
		}
		}

	if (t2 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n  #RT#rier #RII#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
        if (d2->character->tier == 2)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf2, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf2,ch);
		}
		}

	if (t1 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n  #RT#rier #RI#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
        if (d2->character->tier == 1)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf1, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf1,ch);
		}
		}
  
	if (t0 == TRUE){
	stc("\n\r#7(#y0#7)#oxxxxx#y}#7|#0=========#R==#r>#n  #RT#rier #R0#r<#R==#0=========#7|#y{#oxxxxx#7(#y0#7)#n\n\r",ch);}
        for (d2 = descriptor_list; d2 != NULL; d2= d2->next)
		{
	if (d2->connected != CON_PLAYING) continue;
	if (d2->character == NULL) continue;
	if (d2->character->level > 6) continue;
        if (d2->character->tier == 0)
		{
		wch = ( d2->original != NULL) ? d2->original : d2->character;
        xprintf(buf0, "#c%14s                     #RS#rtatus: #c%4d\n\r", wch->pcdata->switchname, wch->race); 
        stc(buf0,ch);
		}
		}
	return;
	}	
return;
}

char *PERS(CHAR_DATA *ch, CHAR_DATA *looker)
{
    if (can_see(looker, ch))
    {
	if (IS_NPC(ch))
	{
	    return ch->short_descr;
	}
	else
	{
	    if (IS_AFFECTED(ch, AFF_POLYMORPH))
	    {
	    	return ch->morph;
	    }
	    else
	    {
	        return ch->name;
            }
	}
    }
    else
    {
	if (IS_IMMORTAL(ch))
	    return "#WA Mystical Being#n";
	else
	    return "Someone";
    }
return "";
}
