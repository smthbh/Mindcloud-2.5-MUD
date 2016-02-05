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


bool is_valid_room         args( ( int vnum ) );

char * CHAR2 ( long iNum )
{
	static char csdf [ 256 ];

	xprintf( csdf, "%ld", iNum );
	return csdf;
}

void do_scatter( CHAR_DATA *ch, char *argument )
{
    char arg [ MIL ];
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    bool found;

    argument = one_argument( argument, arg );
   
    if( arg[0] == '\0' )
    {
	send_to_char("Syntax: scatter <item>\n\r", ch );
	send_to_char("   Will loop until you have no more in inventory.\n\r", ch );
	return;
    }
  
    found = FALSE;

    for( obj = get_obj_carry(ch,arg,ch); obj; obj = get_obj_carry(ch,arg,ch) )
    {
	if( !obj )
	{
		send_to_char("There are no more objects in your inventory.\n\r",ch);
		return;
	}
	for ( victim = char_list; victim; victim = victim->next )
        {
		if( number_range(1,1000) == 1 )
		{
			found = TRUE;
			obj_from_char(obj);
			obj_to_char( obj, victim);
		}
	}
    }
    if( found )
	send_to_char( "Done\n\r",ch);
    return;
}

void do_loop( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int startvnum, endvnum, i;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' )
    {
       send_to_char( "#CSyntax: loop <command> <start##> <end##> <params>\n\r", ch );
       send_to_char( "  Where <command> is a valid command to execute,\n\r", ch );
       send_to_char( "  <start##> and <end##> are numbers/vnums,\n\r", ch );
       send_to_char( "  and <params> is a parameter list for <command>.\n\r", ch );
       send_to_char( "#GEXAMPLE: LOOP MSHOW 22000 22100#n\n\r", ch );
       return;
    }

    if ( arg2[0] == '\0' )
    {
       send_to_char( "You must specify a start number/vnum.\n\r", ch );
       return;
    }

    if ( arg3[0] == '\0' )
    {
       send_to_char( "You must specify an end number/vnum.\n\r", ch );
       return;
    }

    startvnum = (is_number(arg2) ? atoi(arg2) : 1);
    endvnum   = (is_number(arg3) ? atoi(arg3) : 1);

    if ( endvnum < 0 )
      endvnum = 1;

    if ( startvnum < 0 )
      startvnum = 1;

    if ( startvnum > endvnum )
    {
       i = endvnum;
       endvnum = startvnum;
       startvnum = i;
    }

    xprintf( buf, "Beginning loop for %s command, vnums %d to %d (%s).\n\r",
         arg1, startvnum, endvnum, argument );

    send_to_char( buf, ch );

    for ( i = startvnum ; i <= endvnum ; i++ )
    {
       xprintf( buf, "%s %d %s", arg1, i,
         (!str_cmp(arg1,"mstat") || !str_cmp(arg1,"ostat")) ?
          "\b" : argument );
       interpret( ch, buf );
    }

    send_to_char( "Done.\n\r", ch );

    return;
}

void do_allpeace( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   ch->level >= d->character->level
	&&   can_see( ch, d->character ) )
	{
	    char buf[MAX_STRING_LENGTH];
	    xprintf( buf, "%s peace", d->character->name );
	    do_at( ch, buf );
	}
    }
}

void do_gemote( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
 
    if ( argument[0] == '\0' )
    {
        return;
    }
 
 
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected == CON_PLAYING &&
                !IS_SET(d->character->deaf, CHANNEL_INFO) )
        {
            send_to_char( "#B[#0Gemote#B] ", d->character );
            xprintf(buf, "#0%s #w%s#n", ch->name, argument);
            send_to_char( buf, d->character);
            send_to_char( "#n\n\r",   d->character );
        }
    }
 
    return;
}

void do_freevnums(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];
    int iA, vfree = 0, start = 0, count = 0, needed = atoi(argument);

 for (iA = 0; iA < 30000; iA++)
 {
    if (!get_room_index(iA) && !get_obj_index(iA) && !get_mob_index(iA))
    { 
       if (!vfree) start = iA;
       vfree++;
    }
    else if (vfree)
    {
	xprintf(buf, "VNUMS %7d-%7d    ", start, start+vfree);
      if (vfree >= needed)
      {
	stc(buf, ch);
	count++;
      }
	if (count % 3 == 2)
	{
	    count = 0;
	    stc("\n\r", ch);
	}
	start = 0;
	vfree = 0;
    }
 }

 stc("\n\r", ch);

 return;
}

void do_unrestore(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vict = get_char_world(ch,argument);
    char      buf[MSL];

 if (!str_cmp(argument, "all"))
 {
  DESCRIPTOR_DATA *d;
 for (d = descriptor_list; d; d = d->next)
 {
    if (!d->character || d->connected > CON_PLAYING || IS_NPC(d->character))
	continue;
    do_unrestore(ch,d->character->pcdata->switchname);
 }
 return;
 }
 if (!*argument || vict == NULL)
 {
    stc("Unrestore who??\n\r", ch);
    return;
 }

 vict->hit = 1;
 vict->mana = 1;
 vict->move = 1;

 xprintf(buf, "#R%s #DHas been #sUN#s-#sRESTORED#W!! #sK#sI#sL#sL#s!#n",
    IS_NPC(vict) ? vict->short_descr : vict->pcdata->switchname);
 do_info(ch, buf);
 return;
}

void do_xpup(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict = get_char_world(ch,argument);

 if (!str_cmp(argument, "all"))
 {
  DESCRIPTOR_DATA *d;
 for (d = descriptor_list; d; d = d->next)
 {
    if (!d->character || d->connected > CON_PLAYING || IS_NPC(d->character))
	continue;
    do_xpup(ch,d->character->pcdata->switchname);
 }
 return;
 }
 if (!*argument || vict == NULL)
 {
    stc("XPup who??\n\r", ch);
    return;
 }

     SET_TIMER(vict, TIMER_EXPERIENCE, 15);

    send_to_char( "You feel at ease with your experience and learn quicker.\n\r", vict );
    act( "$n starts learning from their mistakes quicker.", vict, NULL, NULL, TO_ROOM );
 return;
}



void do_daynight(CHAR_DATA *ch, char *argument)
{
 if (weather_info.sunlight == SUN_LIGHT)
 {
    stc("It is now night.\n\r", ch);
    weather_info.sunlight = SUN_DARK;
 }
 else
 {
    stc("It is now day.\n\r", ch);
    weather_info.sunlight = SUN_LIGHT;
 }
return;
}

void do_clearflags(CHAR_DATA *ch, char *argument)
{
 char arg1[MIL];
 CHAR_DATA *vict;

 argument = one_argument(argument, arg1);

 if((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("They're not here!\n\r", ch);
    return;
 }
 vict->act2 = 0;
 vict->act  = 17952;
 
 stc("Their ACT flags have been cleared",ch);
 return;
}

void do_unpreg(CHAR_DATA *ch, char *argument)
{
//(imm only)
 char arg1[MIL];
 CHAR_DATA *vict;

 argument = one_argument(argument, arg1);

 if((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("They're not here!\n\r", ch);
    return;
 }
 
  free_string(vict->pcdata->conception);
  REMOVE_BIT(vict->extra, EXTRA_PREGNANT);
  REMOVE_BIT(vict->extra, EXTRA_LABOUR);  
  free_string(vict->pcdata->cparents);
  save_char_obj(vict);

 stc("You are not pregnant anymore\n\r", vict);
 stc("Done\n\r", ch);

 return;
}

void do_beep(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL];
    char buf[MSL];
    CHAR_DATA *vict;
    
 argument = one_argument(argument, arg1);

 if((vict = get_char_world(ch, arg1)) == NULL)
 {
    stc("They're not here!\n\r", ch);
    return;
 }
 

 xprintf(buf, "\a%s\a", argument);
 stc(buf, vict);

 return;
}

void do_startragbid(CHAR_DATA *ch, char *argument)
{
 char buf[MSL];
 
 if(ragnarok || ragnarok_timer == 120 * PULSE_AREA)
 {
 	stc("Ragnarok is already going or bidding is begun",ch);
 	return;
 }
 
 ragnarok_timer = 120 * PULSE_AREA;
 xprintf_2(buf, "Bidding on Ragnarok has begun, place your bids now.");
 do_info(ch, buf);
 return;
}

void do_endragnarok(CHAR_DATA *ch, char *argument)
{
 
 if(!ragnarok)
 {
 	stc("There is no ragnarok happening",ch);
 	return;
 }
 
	ragnarok_stop();
    ragnarok_timer = 0;
 return;
}

void do_checkpk(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *check1;
    CHAR_DATA *check2;
    char buf[MAX_STRING_LENGTH];

for (d = descriptor_list;d != NULL; d = d->next)
{
 if ((check1 = d->character) == NULL) continue;
if (d->connected > CON_PLAYING) continue;

 if ((check2 = d->character->fighting) == NULL) continue;

 if (IS_NPC(check2)) continue;

	xprintf(buf, "%s(%0.2f) is fighting %s(%0.2f)\n\r", check1->name,
atof(CHAR2((check1->hit / check1->max_hit )))
,check2->name,atof(CHAR2((check2->hit/check2->max_hit))));
	send_to_char(buf, ch);
}
for (d = descriptor_list; d; d = d->next)
{if ((check1 = d->character) == NULL) continue;
if (d->connected > CON_PLAYING) continue;

 if (check1->hit <500)
    {
	xprintf(buf, "%s's hp is at %d/%d(%0.2f)\n\r", check1->name,
check1->hit, check1->max_hit,
atof(CHAR2((check1->hit/check1->max_hit))));
	send_to_char(buf, ch);
    }
}
send_to_char("Done.\n\r", ch);
return;
}

char *centerjob (char *fill, const char *txt, int maxlen )
{
 int Tot;
 int tofill;
 int iA;
 const char *txt2 = txt;
 static char buf[MSL];
 char Lik[MSL];

 strcpy(buf,"");
 tofill = maxlen;
 tofill -= strlen(txt);
 tofill += (strlen(txt) - strlen(bash_color(txt)));

 tofill /= 2;

 for (Tot = 0, iA = 0; iA < tofill && Tot < maxlen; iA++, Tot++)
    xcatf(buf, fill);

 for ( ; *txt; txt++ )
 {
    xprintf(Lik, "%s%c", buf, *txt);
    strcpy(buf, Lik);
 }

 Tot += strlen(bash_color(txt2));

 for (iA = Tot; iA < maxlen; iA++)
    xcatf(buf, fill);

 return buf;
}

char * bash_color( const char *txt )
{
    const char *point;
    char *point2;
 static   char buf[MSL*4];

buf[0] = '\0';
point2 = buf;

for (point = txt; *point; point++)
{
    if (*point == '#' && *(point+1) != '\0')
    {
	point++; continue;
    }
    *point2 = *point;
    *++point2 = '\0';
}
*point2 = '\0';
return buf;
}

void randomize_object(int vnum)
{
 OBJ_DATA *obj;
 ROOM_INDEX_DATA	*rm = NULL;
 EXIT_DATA *pexit = NULL;
 int count = 0;
 int a, to_room;
 bool PLACE = 0;

 while (!rm)          
    { 
        to_room = number_range(500, 90000); 
     
        if (is_valid_room(to_room)) 
            rm = get_room_index(number_range(500, 90000)); 
    }     
 
 
    for (a = 0; a < 6; a++) 
    { 
        if ((pexit = rm->exit[a]) != NULL && pexit->to_room != NULL) 
            count++; 
    } 
         
    if (rm && count > 0) 
        PLACE = TRUE; 
     
    if (PLACE){ 
        obj = create_object(get_obj_index(vnum), 0); 
 
        if (!obj)   return; 
         
        obj_to_room(obj, rm);   
    } 
    else    randomize_object(vnum);
return;
}

bool is_valid_room(int vnum) 
{   
    //  Arena 
    if (vnum >= 151 && vnum <= 170)     return FALSE; 
  
    /* 
     *  Kingdoms 
     */ 
    //  Kingdom 1 
    if (vnum >= 40300 && vnum <= 40399) return FALSE; 
    //  Kingdom 2 
    if (vnum >= 40400 && vnum <= 40499) return FALSE; 
    //  Kingdom 3 
    if (vnum >= 40500 && vnum <= 40599) return FALSE; 
    //  Kingdom 4 
    if (vnum >= 40600 && vnum <= 40699) return FALSE; 
    //  Kingdom 5 
    if (vnum >= 40700 && vnum <= 40799) return FALSE; 
  
    //  Class HQs 
    if (vnum >= 93300 && vnum <= 93600) return FALSE; 
  
    return TRUE; 
}

void do_randomload(CHAR_DATA *ch, char *argument)
{
 if (!get_obj_index(atoi(argument)))
 {
    stc("That object doesn't exist!\n\r", ch);
    return;
 }
 stc(get_obj_index((atoi(argument)))->short_descr, ch);
 if(ch->level > 6)
 {
 stc(" randomly loaded to a room on the mud.\n\r", ch);
}

 randomize_object(atoi(argument));

 return;
}

void do_prename(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 char buf[MSL];
 char buf2[MSL];
 OBJ_DATA *obj;

 argument = one_argument(argument, buf);

 if ((vict = get_char_world(ch, buf)) == NULL || IS_NPC(vict))
 {
    stc("THey aren't here!\n\r", ch);
    return;
 }
 else strcpy(buf, argument);

 xprintf(buf2, "%s%c/%s", PLAYER_DIR, tolower(buf[0]),capitalize(buf));

 if (!check_parse_name(buf) || fopen(buf2,"r"))
 {
    stc("Illegal name. Choose another.\n\r", ch);
    return;
 }

 xprintf(buf2, "%s%c/%s", PLAYER_DIR, tolower(vict->pcdata->switchname[0]),capitalize(vict->pcdata->switchname));
 unlink(buf2);

 for (obj = object_list; obj; obj = obj->next)
 {
    if (!str_cmp(obj->questowner, vict->pcdata->switchname))
    {
	free_string(obj->questowner);
	obj->questowner = str_dup(buf);
    }
    if (!str_cmp(obj->questmaker, vict->pcdata->switchname))
    {
	free_string(obj->questmaker);
	obj->questmaker = str_dup(buf);
    }
 }

 free_string(vict->name);
 vict->name = str_dup(buf);
 free_string(vict->pcdata->switchname);
 vict->pcdata->switchname = str_dup(buf);

 act("#6You feel like $n!#n", vict, 0, 0, TO_CHAR);
 act("You turn the poor bastard into $t!", ch, buf, 0, TO_CHAR);

 return;
}

void do_stanceset( CHAR_DATA *ch, char *argument )
{
	char arg1 [MIL];
	char arg2 [MIL];
	CHAR_DATA *victim;
	int sn = 0, amount = 0;
	
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	
	if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
	send_to_char( "#wSyntax: #Bstanceset #w<person> <amount>#n\n\r",	ch);
	return;
    }
	
	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    
    if ( !is_number( arg2 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
	
	amount = atoi( arg2 );
	
	if (amount > 200 || amount < 0 )
    {
    	stc("Max is 200 for stances.\n\r", ch);
    	return;
    }
    
    for( sn=0 ; sn < 12 ; sn++ )
         victim->stance[sn]                    = amount;
         
    send_to_char( "Done.\n\r", ch );
}
  
void do_spellset( CHAR_DATA *ch, char *argument )
{
	char arg1 [MIL];
	char arg2 [MIL];
	CHAR_DATA *victim;
	int sn = 0, amount = 0;
	
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	
	if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
	send_to_char( "#wSyntax: #Bspellset #w<person> <amount>#n\n\r",	ch);
	return;
    }
    
	
	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    
    if ( !is_number( arg2 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
	
    amount = atoi( arg2 );
    
    if (amount > 600 || amount < 0 )
    {
    	stc("Max is 600 for spells.\n\r", ch);
    	return;
    }
    
    for( sn=0 ; sn < 5 ; sn++ )
         victim->spl[sn]                    = amount;
         
    send_to_char( "Done.\n\r", ch );
}

void do_weapset( CHAR_DATA *ch, char *argument )
{
	char arg1 [MIL];
	char arg2 [MIL];
	CHAR_DATA *victim;
	int sn = 0, amount = 0;
	
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	
	if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
	send_to_char( "#wSyntax: #Bweapset #w<person> <amount>#n\n\r",	ch);
	return;
    }
    
	
	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    
    if ( !is_number( arg2 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
	
    amount = atoi( arg2 );
    
    if (amount > 2000 || amount < 0 )
    {
    	stc("Max is 2000 for weapons.\n\r", ch);
    	return;
    }
    
    for( sn=0 ; sn <= 12 ; sn++ )
         victim->wpn[sn] = amount;
         
    send_to_char( "Done.\n\r", ch );
}

void do_dismember(CHAR_DATA *ch, char *argument)
{
  char arg[MSL];
  CHAR_DATA *victim;
  argument=one_argument(argument,arg);

  if(arg[0]== '\0')                                                             
  {
    stc("Dismember whom?\n\r",ch);
    return;
  }
  if((victim = get_char_room(ch,NULL,arg))==NULL)
  {
    stc("They are not here.\n\r",ch);
    return;
  }
  if(victim->trust > ch->trust)
  {
    stc("I don't THINK so!\n\r",ch);
    return;
  }

                                                                                
            SET_BIT(victim->loc_hp[2],LOST_ARM);                                
            SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);                          
            SET_BIT(victim->loc_hp[0],LOST_EYE_L);                              
            SET_BIT(victim->loc_hp[0],LOST_EYE_R);                              
            SET_BIT(victim->loc_hp[0],LOST_EYE_L);                              
            SET_BIT(victim->loc_hp[0],LOST_EAR_L);                              
            SET_BIT(victim->loc_hp[0],LOST_EAR_R);                              
            SET_BIT(victim->loc_hp[0],LOST_EAR_L);                              
            SET_BIT(victim->loc_hp[0],LOST_NOSE);                               
            SET_BIT(victim->loc_hp[0],BROKEN_NOSE);                             
            SET_BIT(victim->loc_hp[0],BROKEN_JAW);                              
            SET_BIT(victim->loc_hp[2],LOST_ARM);                                
            SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);                          
            SET_BIT(victim->loc_hp[3],LOST_ARM);                                
            SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
            SET_BIT(victim->loc_hp[2],BROKEN_ARM);                              
            SET_BIT(victim->loc_hp[3],BROKEN_ARM);                              
            SET_BIT(victim->loc_hp[2],LOST_HAND);                               
            SET_BIT(victim->loc_hp[6],BLEEDING_HAND_L);                         
            SET_BIT(victim->loc_hp[3],LOST_HAND);                               
            SET_BIT(victim->loc_hp[6],BLEEDING_HAND_R);                         
            SET_BIT(victim->loc_hp[2],LOST_THUMB);                              
            SET_BIT(victim->loc_hp[2],LOST_FINGER_I);                           
            SET_BIT(victim->loc_hp[2],LOST_FINGER_M);                           
            SET_BIT(victim->loc_hp[2],LOST_FINGER_R);                           
            SET_BIT(victim->loc_hp[2],LOST_FINGER_L);                           
            SET_BIT(victim->loc_hp[2],BROKEN_THUMB);                            
            SET_BIT(victim->loc_hp[2],BROKEN_FINGER_I);                         
            SET_BIT(victim->loc_hp[2],BROKEN_FINGER_M);                         
            SET_BIT(victim->loc_hp[2],BROKEN_FINGER_R);                         
            SET_BIT(victim->loc_hp[2],BROKEN_FINGER_L);                         
            SET_BIT(victim->loc_hp[3],LOST_THUMB);                              
            SET_BIT(victim->loc_hp[3],LOST_FINGER_I);
            SET_BIT(victim->loc_hp[3],LOST_FINGER_M);                           
            SET_BIT(victim->loc_hp[3],LOST_FINGER_R);                           
            SET_BIT(victim->loc_hp[3],LOST_FINGER_L);                           
            SET_BIT(victim->loc_hp[3],BROKEN_THUMB);                            
            SET_BIT(victim->loc_hp[3],BROKEN_FINGER_I);                         
            SET_BIT(victim->loc_hp[3],BROKEN_FINGER_M);                         
            SET_BIT(victim->loc_hp[3],BROKEN_FINGER_R);                         
            SET_BIT(victim->loc_hp[3],BROKEN_FINGER_L);                         
            SET_BIT(victim->loc_hp[4],LOST_LEG);                                
            SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);                          
            SET_BIT(victim->loc_hp[5],LOST_LEG);                                
            SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);                          
            SET_BIT(victim->loc_hp[4],BROKEN_LEG);                              
            SET_BIT(victim->loc_hp[5],BROKEN_LEG);                              
            SET_BIT(victim->loc_hp[4],LOST_FOOT);                               
            SET_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);                         
            SET_BIT(victim->loc_hp[5],LOST_FOOT);                               
            SET_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
            SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);                          
            SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);                           
            SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);                           
            SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);                           
            SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);                           
            SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_16);                    
            SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_8);                     
            SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_4);                     
            SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_2);                     
            SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_1);                     
        SET_BIT(victim->loc_hp[LOC_LEG_L],BROKEN_LEG);                          
        SET_BIT(victim->loc_hp[LOC_LEG_R],BROKEN_LEG);                          
        SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_SPINE);                         
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);                          
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);                           
        SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);                          
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);                          
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
        SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);                          
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);                          
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);                           
        SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);                          
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);                          
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);                           
        SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);                          
make_part(victim,"arm");                                                        
make_part(victim,"arm");                                                        
make_part(victim,"leg");                                                        
make_part(victim,"leg");                                                        
make_part(victim,"blood");                                                      
make_part(victim,"blood");                                                      
                                                                                
        act("You dismembered $N!",ch,NULL,victim,TO_CHAR);
        send_to_char("You have been #RDISMEMBERED#n\n\r", victim );                     
        victim->in_room->blood += 200;

}

void do_spinebreak(CHAR_DATA *ch, char *argument)
{
  char arg[MSL];
  CHAR_DATA *victim;
  argument=one_argument(argument,arg);

  if(arg[0]== '\0')                                                             
  {
    stc("Break Whos spine?\n\r",ch);
    return;
  }
  if((victim = get_char_room(ch,NULL,arg))==NULL)
  {
    stc("They are not here.\n\r",ch);
    return;
  }
  if(victim->trust > ch->trust)
  {
    stc("I don't THINK so!\n\r",ch);
    return;
  }
              
        SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_SPINE);                         
                                               
        act("You break $N's spine!",ch,NULL,victim,TO_CHAR);
        send_to_char("Your spine spontaneously snaps and fills you with #RPAIN#n\n\r", victim );                     
}

void do_dblbuy( CHAR_DATA *ch, char *argument )
{
  char arg[MIL];
  char buf[MSL];
  char arg2[MIL];             
  int amount;

  argument = one_argument( argument, arg );
  argument = one_argument( argument, arg2);

  if (arg[0] == '\0')
  {
    send_to_char("Syntax: dblbuy <exp|train|cp> <bones>.\n\r",ch);
    send_to_char("Or      dblbuy <exp|train|cp> finish.\n\r",ch);
    send_to_char("These will be on for 30 minutes once turned on.\n\r",ch);
    return;
  }      
  if (!str_cmp(arg, "exp"))
  {
  	if (global_exp)
  	{
  		send_to_char("Double Exp is already on Freak!\n\r", ch);
  		return;
  	}
    
    if(!str_cmp(arg2, "finish"))
    {
      amount = exp_cost;	
    }
    else if (!is_number(arg2))
    {
      send_to_char("A number please, how many bones are you adding??\n\r",ch);
      return;
    }
    else
    {
    	amount = atoi(arg2);
    } 
  	if (amount > exp_cost)
  	{
         amount = exp_cost;
  	}
    if (ch->bones < amount)
  	{
    send_to_char("You don't have the bones.\n\r",ch);
    return;
  	}
    ch->bones -= amount;
    exp_cost -= amount;
  if (exp_cost <= 0)
  {
    exp_cost = 25000;
    pulse_exp = 30;
    do_toggle(ch, "exp on");
  }
  else
  {
  xprintf(buf, "#B%s #0adds #Y%d #0Bones to #RDouble Exp, #Y%d #0left to go!#n\n\r", ch->pcdata->switchname, amount, exp_cost);
  do_info(ch, buf);
  }
  return;
  }
  if (!str_cmp(arg, "cp"))
  {
  	if (global_cp)
  	{
  		send_to_char("Double Class Points is already on Freak!\n\r", ch);
  		return;
  	}
    
    if(!str_cmp(arg2, "finish"))
    {
      amount = cp_cost;	
    }
    else if (!is_number(arg2))
    {
      send_to_char("A number please, how many bones are you adding??\n\r",ch);
      return;
    }
    else
    {
    	amount = atoi(arg2);
    } 
  	if (amount > cp_cost)
  	{
    amount = cp_cost;
  	}
    if (ch->bones < amount)
  	{
    send_to_char("You don't have the bones.\n\r",ch);
    return;
  	}
    ch->bones -= amount;
    cp_cost -= amount;
  if (cp_cost <= 0)
  {
    cp_cost = 25000;
    pulse_cp = 30;
    do_toggle(ch, "cp on");
  }
  else
  {
  xprintf(buf, "#B%s #0adds #Y%d #0Bones to #PDouble Class Points, #Y%d #0left to go!#n\n\r", ch->pcdata->switchname, amount, cp_cost);
  do_info(ch, buf);
  }
  return;
  }
  if (!str_cmp(arg, "train"))
  {
  	if (global_dt)
  	{
  		send_to_char("Super Training Points is already on Freak!\n\r", ch);
  		return;
  	}
    
    if(!str_cmp(arg2, "finish"))
    {
      amount = dt_cost;	
    }
    else if (!is_number(arg2))
    {
      send_to_char("A number please, how many bones are you adding??\n\r",ch);
      return;
    }
    else
    {
    	amount = atoi(arg2);
    } 
  	if (amount > dt_cost)
  	{
    amount = dt_cost;
  	}
    if (ch->bones < amount)
  	{
    send_to_char("You don't have the bones.\n\r",ch);
    return;
  	}
    ch->bones -= amount;
    dt_cost -= amount;
  if (dt_cost <= 0)
  {
    dt_cost = 50000;
    pulse_dt = 30;
    do_toggle(ch, "dt on");
  }
  else
  {
  xprintf(buf, "#B%s #0adds #Y%d #0Bones to #WSuper Training, #Y%d #0left to go!#n\n\r", ch->pcdata->switchname, amount, dt_cost);
  do_info(ch, buf);
  }
  return;
  }
  else do_dblbuy(ch,"");
  return;
}

void update_exp()
{
  if (pulse_exp > 0)
  {
    if (--pulse_exp == 0)
    {
      global_exp = FALSE;
      pulse_exp = -1;
      do_info(NULL,"#RDouble Exp #0is now #YOFF#n");
    }
  }
}

void update_dt()
{
  if (pulse_dt > 0)
  {
    if (--pulse_dt == 0)
    {
      global_dt = FALSE;
      pulse_dt = -1;
      do_info(NULL,"#WSuper Training #0now #ROFF#n");
    }
  }
}

void update_cp()
{
  if (pulse_cp > 0)
  {
    if (--pulse_cp == 0)
    {
      global_cp = FALSE;
      pulse_cp = -1;
      do_info(NULL,"#wDouble Class Points now #ROFF#n");
    }
  }
}


