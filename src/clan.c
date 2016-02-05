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
#include "magic.h"

/*
 * Local functions.
 */
 
void do_level( CHAR_DATA *ch, char *argument )
{
    char arg       [MAX_INPUT_LENGTH];
        char      skill[MSL];
        char      stance[MSL];
        char      buf[MSL];
    float chgen = 0;
    float chgenexp = 0;
    int percent = 0;
    int nextgen = 0;
    int expneeded = 0;

    one_argument( argument, arg );
    ROOM_INDEX_DATA *room;

    if (IS_NPC(ch)) return;  
    
	if (arg[0] == '\0')
	{
	stc("#0x#r----#R=====#r----#R=====#r----#R=====#r----#0x#n\n\r",ch);
                xprintf(buf, "#0You are currently generation #W%d#n\n\r",
                        ch->generation);
                send_to_char(buf, ch);
        chgen = ch->generation;
	chgenexp = ch->genexp;
	if (ch->generation == 6)
       expneeded = 13000000.00;
    else if(ch->generation == 5)
       expneeded = 150000000.00;
    else if(ch->generation == 4)
       expneeded = 400000000.00;
    else if(ch->generation == 3)
       expneeded = 900000000.00;
    else if(ch->generation == 2)
       expneeded = 1800000000.00;
	percent = (chgenexp / expneeded)*100.00;

	nextgen = (ch->generation - 1);
    if (ch->generation > 2 && ch->level > 2)
{
                        int       x, howmany;

                        howmany = percent / 2;
                        xprintf(buf, "#0 Progress to gen %d #W[#R", nextgen);

                        for (x = 1; x <= 50; x++)
                        {
                                if (x <= howmany)
                                        xcatf(buf, "#R*#n");
                                else
                                        xcatf(buf, "#0-#n");
                        }
                        xcatf(buf, "#W]#n\n\r");
                        send_to_char(buf, ch);
	}
	


	stc("#0x#r----#R=====#r----#R=====#r----#R=====#r----#0x#n\n\r",ch);
	if ( ch->stance[12] == 1 )
      xprintf(stance, " Viper" );
     else if ( ch->stance[12] == 2 )
      xprintf(stance, " Crane" );
     else if ( ch->stance[12] == 3 )
      xprintf(stance, " Crab" );  
     else if ( ch->stance[12] == 4 )
      xprintf(stance, " Mongoose" );  
     else if ( ch->stance[12] == 5 )
      xprintf(stance, " Bull" );  
     else if ( ch->stance[12] == 6 )
      xprintf(stance, " Mantis" );  
     else if ( ch->stance[12] == 7 )
      xprintf(stance, " Dragon" );  
     else if ( ch->stance[12] == 8 )
      xprintf(stance, " Tiger" );  
     else if ( ch->stance[12] == 9 )
      xprintf(stance, " Monkey" );  
     else if ( ch->stance[12] == 10 )
      xprintf(stance, " Swallow" ); 
     else if ( ch->stance[12] == 11 )
      xprintf(stance, " Wolf" );
     else if ( ch->stance[12] == 13 )
      xprintf( stance, " SS1");
     else if ( ch->stance[12] == 14 )
      xprintf( stance, " SS2");
     else if ( ch->stance[12] == 15 )
      xprintf( stance, " SS3");
     else if ( ch->stance[12] == 16 )
      xprintf( stance, " Pizza");
     else if ( ch->stance[12] == 17 )
      xprintf( stance, " Zarius");
     else if ( ch->stance[12] == -1 )
      xprintf(stance, " None" );
     else
      xprintf(stance, " None" );
                xprintf(buf, "#0Autostance#n:  #R%s #n\n\r", stance);
                send_to_char(buf, ch);
    if (IS_SET(ch->act, PLR_RIGHTHAND))
      send_to_char("#wYou favor your right arm in combat.#n\n\r",ch);
    else if(IS_SET(ch->act, PLR_LEFTHAND))
      send_to_char("#wYou favor your left arm in combat.#n\n\r",ch);
    else
      send_to_char("#wYou fight well with both your arms.#n\n\r",ch);

    stc("#0x#r----#R=====#r----#R=====#r----#R=====#r----#0x#n\n\r",ch);
    
    if ( IS_CLASS(ch, CLASS_DEMON) )
     send_to_char("You are a demon!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_MAGE) )
     send_to_char("You are a battlemage!\n\r", ch);    
     if ( IS_CLASS(ch ,CLASS_SHAPESHIFTER))
       send_to_char("You are a shapeshifter!\n\r",ch);
     if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
       send_to_char("You are an undead knight!\n\r",ch);
     if ( IS_CLASS(ch, CLASS_WEREWOLF) )
     send_to_char("You are a werewolf!\n\r", ch);
     if ( IS_CLASS(ch, CLASS_NINJA) )
     send_to_char("You are a ninja!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_VAMPIRE) )
     send_to_char("You are a vampire!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_SAMURAI) )
     send_to_char("You are a Samurai!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_DROW) )
     send_to_char("You are a drow!\n\r", ch);    
    if (IS_CLASS(ch, CLASS_ANGEL))
     send_to_char("You are an angel!\n\r", ch);
    if (IS_CLASS(ch, CLASS_LICH))
      send_to_char("You are a lich!\n\r",ch);
    if (IS_CLASS(ch, CLASS_SKYBLADE))
     send_to_char("You are a SkyBlade!!\n\r",ch);
    if (IS_CLASS(ch, CLASS_GHOUL))
     send_to_char("You are a foul smelling #GGhoul#n!!\n\r",ch);
    if (IS_CLASS(ch, CLASS_DRACONIAN))
     send_to_char("You are a Draconian!!\n\r",ch);
    if (IS_CLASS(ch, CLASS_TANARRI))
     send_to_char("You are a warrior of The Bloodwars.\n\r",ch);
    if( IS_CLASS(ch, CLASS_MONK) )
	stc( "You are a monk!\n\r",ch);
    if( IS_CLASS(ch, CLASS_CYBORG))
	stc( "You are a Mix Between Man and Machine!\n\r",ch);
    if( IS_CLASS(ch, CLASS_SHADOW))
	stc( "You are a Shadow!\n\r",ch);
    if( IS_CLASS(ch, CLASS_PRIEST))
        stc( "You are a holy Priest\n\r", ch);
    if( IS_CLASS(ch, CLASS_JEDI))
        stc( "You are a Jedi Warrior!\n\r", ch);
 if (IS_CLASS(ch, CLASS_TANARRI))
 {
  if (ch->pcdata->rank == TANARRI_FODDER)
    send_to_char("You are nothing but fodder for the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_FIGHTER)
    send_to_char("You are a fighter in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_ELITE)
    send_to_char("You are an elite warrior in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_CAPTAIN)
    send_to_char("You are a captain in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_WARLORD)
    send_to_char("You are a warlord in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_BALOR)
    send_to_char("You are a Tanar'ri Balor.\n\r",ch);
 } 

 if (IS_CLASS(ch, CLASS_VAMPIRE) )
 {

	if (ch->pcdata->rank == AGE_TRUEBLOOD)
	  send_to_char("You are a TrueBlood!\n\r",ch);
	else if (ch->pcdata->rank == AGE_LA_MAGRA)
	  send_to_char("You are a La Magra!\n\r",ch);
	else if (ch->pcdata->rank == AGE_METHUSELAH)
	  send_to_char("You are a Methuselah!\n\r",ch);
	else if (ch->pcdata->rank == AGE_ELDER)
	  send_to_char("You are an Elder!\n\r",ch);
	else if (ch->pcdata->rank == AGE_ANCILLA)
	  send_to_char("You are an Ancilla!\n\r",ch);
	else if (ch->pcdata->rank == AGE_NEONATE)
	  send_to_char("You are a Neonate!\n\r", ch);
	else
	  send_to_char("You are a Childe!\n\r", ch);
    }
    room = get_room_index( ch->home );
    
                xprintf(buf, "#0Recall Room#R : #Y%s#n\n\r", room->name);
                send_to_char(buf, ch);
	stc("#0x#r----#R=====#r----#R=====#r----#R=====#r----#0x#n\n\r",ch);	
	stc("#wTo view weapons, use the argument weapons ('#Rlevel weapons#w')#n\n\r",ch);	
	stc("#wTo view stances, use the argument stances ('#Blevel stances#w')#n\n\r",ch);
	stc("#wTo view spells, use the argument spells ('#Plevel spells#w')#n\n\r",ch);
	stc("#wTo view using old style dystopia method ('#Woldlevel#w')#n",ch);
        }

	else if (!str_cmp(arg,"weapons"))
	{
     stc("     ,/|   #G(__                   #0/|#l    /(_)\\    #0|\\                  #G__)#g     |\\\n\r",ch);
     stc("   ,/  \\   #G, _#R`#G-\\              #0/' `\\#b   \\#R`#b,#R'#b/   #0/' `\\             #G/-#R`#G__ >,#g  /  `\\\n\r",ch);
     stc("  /     \\_#G/ / #7^^^            #0/' / | `\\#l_/\\^/\\#l_#0/' | \\ `\\           #7^^^#G  \\ \\#g_/    `\\\n\r",ch);
     stc(" /    |   #G||                #0O  |   \\#b/'   #0V#b   `\\#0/   |  O                #G||   #g|    \\\n\r",ch);
     stc("|  |  |  /#G||               #0O   |#l,-,|   ,_;_,   |,-,#0|   O               #G||#g\\  |  |  |\n\r",ch);
     stc("|  |   \\ | #G\\\\             #0oO#l    \\  \\\\ '\\ I /` //  /    #0Oo             #G// #g| /   |  |\n\r",ch);
     stc("|  \\   /\\| #G| #Y)            #0oO#l     \\ \\#0`#b\\  \\ /  /#0'#l/ /     #0Oo            #Y( #G| #g|/\\   /  |\n\r",ch);
     stc("|   \\ |  ` #G/#Y/              #0O    /^#l\\ \\#0,#b\\  |  /#0,#l/ /#0^#l\\    #0O              #Y\\#G\\ #g'  | /   |\n\r",ch);
     stc("|  /'\\|   #G( #Y|     #0__________#0O  /#0__#l/ /#0__#b| I |#0__#l\\ \\#0__#0\\  O#0__________     #Y| #G)   #g|/`\\  |\n\r",ch);
     stc(" \\ |  `    #G\\\\    #0|           #0\\|  #7'''  ''' ```  ```  #0|/           #0|    #Y/#G/    #g'  | /\n\r",ch);
     stc("  \\|    #r(  #G||    #0|                 #rW#Reapon #rL#Revels                 #0|   #Y( #G|  #r)    #g|/\n\r",ch);
	 xprintf(skill,"   #G\\    #G\\\\_//    #0|Slice: #7%-4d #0Stab: #7%-4d #0Whip:  #7%-4d #0Pierce: #7%-4d#0|    #G\\\\_//    #g/\n\r",ch->wpn[1],ch->wpn[2],ch->wpn[4],ch->wpn[11]);
	 stc(skill,ch);
	 xprintf(skill,"    #G`    `v'     #0|Crush: #7%-4d #0Claw: #7%-4d #0Slash: #7%-4d #0Pound:  #7%-4d#0|     #G'v'    #g'\n\r",ch->wpn[8],ch->wpn[5],ch->wpn[3],ch->wpn[7]);
	 stc(skill,ch);
	 xprintf(skill,"    #G`    `v'     #0|Blast: #7%-4d #0Grep: #7%-4d #0Suck: #7%-4d  #0Bite:  #7%-4d #0|     #G'v'    #g'\n\r",ch->wpn[6],ch->wpn[9],ch->wpn[12],ch->wpn[10]);
	 stc(skill,ch);    
	 xprintf(skill,"                 #0|                   Unarm: #7%-4d                 #0|\n\r",ch->wpn[0]);
	 stc(skill,ch);
         stc("                 #0|_______________________________________________#0|#n\n\r",ch);
	}

	else if (!str_cmp(arg,"magic") || !str_cmp(arg,"spells"))
	{
	stc(" #y_________________________________\n\r",ch);
	stc("#y/  _\\                             \\\n\r",ch);
	stc("#y\\ (_/_____________________________/\n\r",ch);
	if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill," #y\\   #0*#rR#Red Magic            #0[#R%-3d#0]#y\\\n\r",ch->spl[1] + 20);
	else xprintf(skill," #y\\   #0*#rR#Red Magic            #0[#R%-3d#0]#y\\\n\r",ch->spl[1]);
	stc(skill,ch);
	if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill,"  #y\\   #0*#bB#Blue Magic           #0[#R%-3d#0]#y\\\n\r",ch->spl[2] + 20);
	else xprintf(skill,"  #y\\   #0*#bB#Blue Magic           #0[#R%-3d#0]#y\\\n\r",ch->spl[2]);
	stc(skill,ch);
	if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill,"   #y\\   #0*#gG#Green Magic          #0[#R%-3d#0]#y\\\n\r",ch->spl[3] + 20);
	else xprintf(skill,"   #y\\   #0*#gG#Green Magic          #0[#R%-3d#0]#y\\\n\r",ch->spl[3]);
	stc(skill,ch);
	if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill,"  #y__\\   #0*#pP#Purple Magic         #0[#R%-3d#0]#y\\\n\r",ch->spl[0] + 20);
	else xprintf(skill,"  #y__\\   #0*#pP#Purple Magic         #0[#R%-3d#0]#y\\\n\r",ch->spl[0]);
	stc(skill,ch);
	if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill," #y/ )_\\   #0*#yY#Yellow Magic         #0[#R%-3d#0]#y\\\n\r",ch->spl[4] + 20);
	else xprintf(skill," #y/ )_\\   #0*#yY#Yellow Magic         #0[#R%-3d#0]#y\\\n\r",ch->spl[4]);
	stc(skill,ch);
	stc(" #y\\___/______________________________/#n\n\r",ch);
	}

	else if (!str_cmp(arg,"stances"))
	{    
	stc("#r                                      _.--.\n\r",ch);
	stc("                                     #r.' ,--.`.\n\r",ch);
	stc("                                   #r,' ,'    `|\n\r",ch);
	stc("                                #r,'  ,'      '\n\r",ch);
	stc("                              #r,'   '\n\r",ch);
	stc("                            #r,'    '\n\r",ch);
	stc("                         #r_,-    ,'\n\r",ch);
	stc("                      #r_,'       |                           ____,-------.\n\r",ch);
	stc("                   #r_,'           `.                   _,---'   ___,----. `.\n\r",ch);
	stc("                #r_,'             #R_,---.#r             ,-'      ,-'         `.|\n\r",ch);
	stc("             #r_,'            #R_,-'  _   `.#r        ,' #R__#r     ,'             |'\n\r",ch);
	stc("           #r,'   .--.    #R_,-'__,--' `.   `.#r   ,'#R_,-'  `.#r ,'              ,'\n\r",ch);
	stc("        #r,'  , '    `. #R,'_,-'        `.    .,'-'-.      `.\n\r",ch);
	stc("     #r ,', '         #R,','             `.          `-. `. `.\n\r",ch);
	stc("    #r,','          #0,''`)#r`.#R            ,`.         `.  `.`-.`.\n\r",ch);
	stc("   #r,,'           #0((  '   #r`.#R        ,'     _,-=-.  `\\  `\\ #0|`.\\\n\r",ch);
	stc("  #r' (             #0``       #r`.#R    ,'     ,'-,'  `.  `)  `)`  #0))\n\r",ch);
	stc(" #r(   `                       #r` #R.'     ,'#y-,#R'     |  #Y,#R;   ;  #0''\n\r",ch);
	stc("  #r`#R                           #R`:     |---|      `.     ,'\n\r",ch);
	stc("                               #R:     |---|       '.    :\n\r",ch);
	stc("                               #R:     `.--`.       '.   :\n\r",ch);
	stc("                               #R`      `    `       ',`__)\n\r",ch);
	stc("#0X#r==============================#0[#rS#Rtance #rL#Revels#0]#r==============================#0X\n\r",ch);
	xprintf(skill,"#r|#0    Viper #7%-4d    #0Crane #7%-4d    #0Crab #7%-4d    #0Mongoose #7%-4d    #0Bull #7%-4d#r    |\n\r",ch->stance[1],ch->stance[2],ch->stance[3],ch->stance[4],ch->stance[5]);
	stc(skill,ch);
	xprintf(skill,"#r|#0   Mantis #7%-4d   #0Dragon #7%-4d   #0Tiger #7%-4d    #0Swallow  #7%-4d  #0Monkey #7%-4d#r    |\n\r",ch->stance[6],ch->stance[7],ch->stance[8],ch->stance[10],ch->stance[9]);
	stc(skill,ch);
	stc("#0X#r==============================#0[#rS#Ruper #rS#Rtances#0]#r==============================#0X#n\n\r",ch);
	xprintf(skill,"#r|#0      SS1 #7%-4d      #0SS2 #7%-4d     #0SS3 #7%-4d       #0Pizza #7%-4d  #0Zarius #7%-4d#r    |#n\n\r",ch->stance[13],ch->stance[14],ch->stance[15],ch->stance[16],ch->stance[17]);
	stc(skill,ch);
	stc("#r|                                                                           |\n\r",ch);
	stc("#r|       #7To gain super stances, you must max all your current stances.       #r|\n\r",ch);
	stc("#r|      #7After this, use 'setstance' to beging editing your superstance.      #r|\n\r",ch);
	stc("#0X#r==============================#0[#r+#R-#r+#R-#r+#R-#r+#R-#r+#R-#r+#R-#r+#0]#r==============================#0X#n\n\r",ch);
    }    

	else
	{
	do_level(ch,"");
	}

return;
}

void do_oldlevel( CHAR_DATA *ch, char *argument )
{
	ROOM_INDEX_DATA *room;
    char arg       [MAX_INPUT_LENGTH];
    char      skill[MSL];
    char      stance[MSL];
    int percent = 0;
    int nextgen = 0;
    int expneeded = 0;
    float chgen = 0;
    float chgenexp = 0;

	one_argument( argument, arg );

    if (IS_NPC(ch)) return;  
    
    send_to_char("---------------------------=[#CWeapon Levels#n]=--------------------------------\n\r", ch);
    xprintf(skill,"Slice  : %-4d", ch->wpn[1]);
    send_to_char( skill, ch );
    xprintf(skill," Stab   : %-4d", ch->wpn[2]);
    send_to_char( skill, ch );      
    xprintf(skill," Slash  : %-4d", ch->wpn[3]);
    send_to_char( skill, ch );  
    xprintf(skill," Whip   : %-4d", ch->wpn[4]);
    send_to_char( skill, ch ); 
    xprintf(skill," Claw   : %-4d\n\r", ch->wpn[5]);
    send_to_char( skill, ch );
    xprintf(skill,"Blast  : %-4d", ch->wpn[6]);
    send_to_char( skill, ch );
    xprintf(skill," Pound  : %-4d", ch->wpn[7]);
    send_to_char( skill, ch );
    xprintf(skill," Crush  : %-4d", ch->wpn[8]);
    send_to_char( skill, ch );
    xprintf(skill," Grep   : %-4d", ch->wpn[9]);
    send_to_char( skill, ch ); 
    xprintf(skill," Bite   : %-4d\n\r", ch->wpn[10]);
    send_to_char( skill, ch );  
    xprintf(skill,"Pierce : %-4d", ch->wpn[11]);
    send_to_char( skill, ch );
    xprintf(skill," Suck   : %-4d", ch->wpn[12]);
    send_to_char( skill, ch );        
    xprintf(skill," Unarmed: %-4d\n\r\n", ch->wpn[0]);
    send_to_char( skill, ch );    
    
    send_to_char("---------------------------=[#CStance Levels#n]=--------------------------------\n\r",ch);
    xprintf(skill,"Viper    : %-4d", ch->stance[1]);
    send_to_char( skill, ch );
    xprintf(skill, "Crane   : %-4d", ch->stance[2]);
    send_to_char( skill, ch );
    xprintf(skill,  "Crab   : %-4d", ch->stance[3]);
    send_to_char( skill, ch );
    xprintf(skill,"Mongoose : %-4d", ch->stance[4]);
    send_to_char( skill, ch );
    xprintf(skill,"Bull     : %-4d\n\r", ch->stance[5]);
    send_to_char( skill, ch );
    xprintf(skill, "Mantis   : %-4d", ch->stance[6]);
    send_to_char( skill, ch );
    xprintf(skill,"Dragon  : %-4d", ch->stance[7]);
    send_to_char( skill, ch );
    xprintf(skill,"Tiger  : %-4d", ch->stance[8]);
    send_to_char( skill, ch );
    xprintf(skill,"Monkey   : %-4d", ch->stance[9]);
    send_to_char( skill, ch );
    xprintf(skill,"Swallow  : %-4d\n\r", ch->stance[10]);
    send_to_char( skill, ch );

    send_to_char("\n\r---------------------------=[#CSuper Stances#n]=--------------------------------\n\r",ch);
    xprintf(skill,"SS1      : %-4d", ch->stance[13]);
    send_to_char( skill, ch );
    xprintf(skill,"SS2     : %-4d", ch->stance[14]);
    send_to_char( skill, ch );
    xprintf(skill,"SS3    : %-4d", ch->stance[15]);
    send_to_char( skill, ch );
    xprintf(skill,"Pizza    : %-4d", ch->stance[16]);
    send_to_char( skill, ch );
    xprintf(skill,"Zarius   : %-4d\n\r", ch->stance[17]);
    send_to_char( skill, ch );

    send_to_char("\n\r---------------------------=[#CSpell Levels#n]=---------------------------------\n\r", ch);
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill,"#PPurple#n : %-4d", ch->spl[0] + 20);
    else xprintf(skill,"#PPurple#n : %-4d", ch->spl[0]);
    send_to_char( skill, ch );
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill,"#BBlue#n : %-4d", ch->spl[2] + 20);
    else xprintf(skill,"#BBlue#n : %-4d", ch->spl[2]);
    send_to_char( skill, ch );
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill,"#RRed#n : %-4d", ch->spl[1] + 20);
    else xprintf(skill,"#RRed#n : %-4d", ch->spl[1]);
    send_to_char( skill, ch );
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill,"#GGreen#n : %-4d", ch->spl[3] + 20);
    else xprintf(skill,"#GGreen#n : %-4d", ch->spl[3]);
    send_to_char( skill, ch );
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) xprintf(skill,"#YYellow#n : %-4d\n\r\n", ch->spl[4] + 20);
    else xprintf(skill, "#YYellow#n : %-4d\n\r\n", ch->spl[4]);
    send_to_char( skill, ch );
    
    
    send_to_char("---------------------------=[#CMisc. Things#n]=---------------------------------\n\r", ch);
    
        room = get_room_index( ch->home );

    xprintf(skill, "Recall Room : %s     \n\r", room->name);
    send_to_char( skill, ch );
    if ( ch->stance[12] == 1 )
      xprintf(stance, " Viper" );
     else if ( ch->stance[12] == 2 )
      xprintf(stance, " Crane" );
     else if ( ch->stance[12] == 3 )
      xprintf(stance, " Crab" );  
     else if ( ch->stance[12] == 4 )
      xprintf(stance, " Mongoose" );  
     else if ( ch->stance[12] == 5 )
      xprintf(stance, " Bull" );  
     else if ( ch->stance[12] == 6 )
      xprintf(stance, " Mantis" );  
     else if ( ch->stance[12] == 7 )
      xprintf(stance, " Dragon" );  
     else if ( ch->stance[12] == 8 )
      xprintf(stance, " Tiger" );  
     else if ( ch->stance[12] == 9 )
      xprintf(stance, " Monkey" );  
     else if ( ch->stance[12] == 10 )
      xprintf(stance, " Swallow" ); 
     else if ( ch->stance[12] == 11 )
      xprintf(stance, " Wolf" );
     else if ( ch->stance[12] == 13 )
      xprintf( stance, " SS1");
     else if ( ch->stance[12] == 14 )
      xprintf( stance, " SS2");
     else if ( ch->stance[12] == 15 )
      xprintf( stance, " SS3");
     else if ( ch->stance[12] == 16 )
      xprintf( stance, " Pizza");
     else if ( ch->stance[12] == 17 )
      xprintf( stance, " Zarius");
     else if ( ch->stance[12] == -1 )
      xprintf(stance, " None" );
     else
      xprintf(stance, " None" );
      xprintf(skill, "#pAutostance#n:  %s #n\n\r",stance);
     send_to_char(skill, ch);
    if (IS_SET(ch->act, PLR_RIGHTHAND))
      send_to_char("You favor your right arm in combat.\n\r",ch);
    else if(IS_SET(ch->act, PLR_LEFTHAND))
      send_to_char("You favor your left arm in combat.\n\r",ch);
    else
      send_to_char("You fight well with both your arms.\n\r",ch);

    send_to_char("---------------------------=[#CClass Things#n]=---------------------------------\n\r", ch);
    
    if ( IS_CLASS(ch, CLASS_DEMON) )
     send_to_char("You are a demon!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_MAGE) )
     send_to_char("You are a battlemage!\n\r", ch);    
     if ( IS_CLASS(ch ,CLASS_SHAPESHIFTER))
       send_to_char("You are a shapeshifter!\n\r",ch);
     if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
       send_to_char("You are an undead knight!\n\r",ch);
     if ( IS_CLASS(ch, CLASS_WEREWOLF) )
     send_to_char("You are a werewolf!\n\r", ch);
     if ( IS_CLASS(ch, CLASS_NINJA) )
     send_to_char("You are a ninja!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_VAMPIRE) )
     send_to_char("You are a vampire!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_SAMURAI) )
     send_to_char("You are a Samurai!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_DROW) )
     send_to_char("You are a drow!\n\r", ch);    
    if (IS_CLASS(ch, CLASS_ANGEL))
     send_to_char("You are an angel!\n\r", ch);
    if (IS_CLASS(ch, CLASS_GIANT))
      send_to_char("#R[#nYou are a #Cgiant#R]\n\r",ch);
    if (IS_CLASS(ch, CLASS_LICH))
      send_to_char("You are a lich!\n\r",ch);
    if (IS_CLASS(ch, CLASS_FAE))
        stc( "#R[#nYou are a #CFae#R]\n\r",ch);
    if (IS_CLASS(ch, CLASS_DRONE))
        stc( "#R[#nYou are a #CDrone#R]\n\r",ch);
    if (IS_CLASS(ch, CLASS_TANARRI))
     send_to_char("You are a warrior of The Bloodwars.\n\r",ch);
    if( IS_CLASS(ch, CLASS_MONK) )
	stc( "You are a monk!\n\r",ch);
    if( IS_CLASS(ch, CLASS_CYBORG))
	stc( "You are a Mix Between Man and Machine!\n\r",ch);

    
     chgen = ch->generation;
	chgenexp = ch->genexp;
		if (ch->generation == 6)
       expneeded = 13000000.00;
    else if(ch->generation == 5)
       expneeded = 150000000.00;
    else if(ch->generation == 4)
       expneeded = 400000000.00;
    else if(ch->generation == 3)
       expneeded = 900000000.00;
    else if(ch->generation == 2)
       expneeded = 1800000000.00;
	percent = (chgenexp / expneeded)*100.00;

	nextgen = (ch->generation - 1);
	if (ch->generation > 1 && ch->level > 2)
	{
	xprintf(skill,"You are #R%d#W%% #non your way to generation #Y%d.#n\n\r",percent,nextgen);
	//stc(skill,ch);
    //xprintf(skill, "Generation : %d\n\r", ch->generation);
    send_to_char( skill, ch );
      }

 if (IS_CLASS(ch, CLASS_TANARRI))
 {
  if (ch->pcdata->rank == TANARRI_FODDER)
    send_to_char("You are nothing but fodder for the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_FIGHTER)
    send_to_char("You are a fighter in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_ELITE)
    send_to_char("You are an elite warrior in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_CAPTAIN)
    send_to_char("You are a captain in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_WARLORD)
    send_to_char("You are a warlord in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_BALOR)
    send_to_char("You are a Tanar'ri Balor.\n\r",ch);
 } 

 if (IS_CLASS(ch, CLASS_VAMPIRE) )
 {

	if (ch->pcdata->rank == AGE_TRUEBLOOD)
	  send_to_char("You are a TrueBlood!\n\r",ch);
	else if (ch->pcdata->rank == AGE_LA_MAGRA)
	  send_to_char("You are a La Magra!\n\r",ch);
	else if (ch->pcdata->rank == AGE_METHUSELAH)
	  send_to_char("You are a Methuselah!\n\r",ch);
	else if (ch->pcdata->rank == AGE_ELDER)
	  send_to_char("You are an Elder!\n\r",ch);
	else if (ch->pcdata->rank == AGE_ANCILLA)
	  send_to_char("You are an Ancilla!\n\r",ch);
	else if (ch->pcdata->rank == AGE_NEONATE)
	  send_to_char("You are a Neonate!\n\r", ch);
	else
	  send_to_char("#R[You are a #CChilde#R]\n\r", ch);
	if ( IS_SET(ch->special, SPC_PRINCE) )
	  send_to_char("#R[#nYou are a #Cprince#R]\n\r", ch);
 } 
}


void do_smother( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *inroom;
    one_argument( argument, arg );
    if (IS_NPC( ch )) return;
    inroom=ch->in_room;
    if ( arg[0] == '\0' && !IS_SET(inroom->room_flags,ROOM_FLAMING))
    {
	send_to_char( "Smother whom?\n\r", ch );
	return;
    }
    
    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_AFFECTED(victim, AFF_FLAMING))
    {
	send_to_char( "But they are not on fire!\n\r", ch );
	return;
    }

    if ( number_percent() > (ch->level*10) )
    {
	act( "You try to smother the flames around $N but fail!",  ch, NULL, victim, TO_CHAR    );
	act( "$n tries to smother the flames around you but fails!", ch, NULL, victim, TO_VICT    );
	act( "$n tries to smother the flames around $N but fails!",  ch, NULL, victim, TO_NOTVICT );
	if ( number_percent() > 98 && !IS_AFFECTED(ch,AFF_FLAMING) )
	{
	    act( "A spark of flame from $N's body sets you on fire!",  ch, NULL, victim, TO_CHAR    );
	    act( "A spark of flame from your body sets $n on fire!", ch, NULL, victim, TO_VICT    );
	    act( "A spark of flame from $N's body sets $n on fire!",  ch, NULL, victim, TO_NOTVICT );
	    SET_BIT(ch->affected_by, AFF_FLAMING);
	    do_humanity(ch,"");
	}
	return;
    }

    act( "You manage to smother the flames around $M!",  ch, NULL, victim, TO_CHAR    );
    act( "$n manages to smother the flames around you!", ch, NULL, victim, TO_VICT    );
    act( "$n manages to smother the flames around $N!",  ch, NULL, victim, TO_NOTVICT );
    REMOVE_BIT(victim->affected_by, AFF_FLAMING);
    do_humanity(ch,"");
    return;
}

void do_introduce( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );


    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->generation > 0 && ch->generation < 14)
    {
      if ( ch->generation == 1 )
      {
	xprintf(buf,"My name is %s.  I am of the First Generation.  I am the Sire of all Kindred.\n\r", ch->name);
	do_say(ch,buf);
      }
      else if ( ch->generation == 2 )
      {
	xprintf(buf,"My name is %s.  I am of the Second Generation.  My sire is Caine.\n\r",ch->name );
	do_say(ch,buf);
      }
      else if ( ch->generation == 3 )
      {
	xprintf(buf,"My name is %s.  I am of the Third Generation.  My sire is Caine.\n\r",ch->name );
	do_say(ch, buf);
      }
      else if ( ch->generation > 3 )
      {
	xprintf(buf,"My name is %s.  I am of the %dth Generation.  My sire is Caine.\n\r",ch->name, ch->generation );
	do_say(ch,buf);
      }
      else return;
    }
    else if (IS_CLASS(ch, CLASS_WEREWOLF) && IS_HERO(ch) &&
        ch->generation > 0 && ch->generation < 7)
    {
	if ( ch->generation == 1 )
	{
	  xprintf(buf,"My name is %s.  I am the chosen champion of Gaia.\n\r",ch->name );
	  do_say(ch,buf);
	}
	else if ( ch->generation > 1 )
	{
	  xprintf(buf,"My name is %s.  I am the pup of Gaia.",ch->name );
	  do_say(ch,buf);
	}
        return;
    }
    else send_to_char("Huh?\n\r",ch);
    return;
}  

void do_dragon (CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        char buf[MAX_STRING_LENGTH];
        int dam = 0;
 
        if (IS_NPC(ch)) return;
        if (!IS_CLASS(ch, CLASS_VAMPIRE))
        {
         send_to_char("Huh?\n\r", ch);
         return;
        }
        if (ch->power[DISC_VAMP_VICI] < 8)
        {
         send_to_char("You must have at least level 8 Vicissitude to use Dragon Breath.\n\r",ch);
         return;
        }
        if ((victim = ch->fighting) == NULL)
	{
	send_to_char("You aren't fighting anyone.\n\r", ch);
	return;
	}
        if (!IS_EXTRA(ch,EXTRA_DRAGON))
        {
        send_to_char("You must be in Dragonform to use Dragonbreath.\n\r",ch);
        return;
        }
        if (ch->pcdata->condition[COND_THIRST] < 6)
        {
         send_to_char("You have insufficient blood.\n\r", ch);
         return;
        }
        WAIT_STATE(ch, 12);
        if (!IS_NPC(victim))
        {
         dam = ch->power[DISC_VAMP_VICI] * 50;
        }
        if (IS_NPC(victim))
        {
         dam = ch->power[DISC_VAMP_VICI] * 200;
        }
        ch->pcdata->condition[COND_THIRST] -= 6;
        if ((!IS_NPC(victim)) && IS_CLASS(victim, CLASS_VAMPIRE))
        {
         dam *= 1.5;
        }
        if ((!IS_NPC(victim)) && IS_CLASS(victim, CLASS_WEREWOLF))
        {
         if (ch->power[DISC_WERE_BOAR] > 2) dam *= 1.5;
        }
 
        if (dam <= 0)
          dam = 1;
xprintf(buf,"Your flame fries $N! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_CHAR);
xprintf(buf,"$n's flame fries you! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_VICT);
xprintf(buf,"$n's flame fries $N! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_NOTVICT);
 

      hurt_person(ch,victim, dam);
     if(!IS_AFFECTED(victim,AFF_FLAMING)) SET_BIT(victim->affected_by,AFF_FLAMING);
 
       return;
}         


 
void do_taste( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      buf [MAX_STRING_LENGTH];
    char      age [MAX_STRING_LENGTH];
    char      lin [MAX_STRING_LENGTH];
    char     lord [MAX_STRING_LENGTH];
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument (argument, arg );

    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->power[DISC_VAMP_THAU] < 1)
    {
        send_to_char("You must obtain at least level 1 in Thaumaturgy to use Taste of Blood.\n\r",ch);
	return;
    }
 
       if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
        send_to_char( "Only useful on vampire targets.\n\r",ch);
        return;
    }

    if (IS_NPC(victim))
    {
        send_to_char( "Only useful on vampire targets.\n\r",ch);
        return;
    }                               

xprintf(buf,"You examine $N's blood carefully.\n\r");
act(buf,ch,NULL,victim,TO_CHAR);
xprintf(buf,"$n examines your blood carefully.\n\r");
act(buf,ch,NULL,victim,TO_VICT);
xprintf(buf,"$n examines $N's blood carefully.\n\r");
act(buf,ch,NULL,victim,TO_NOTVICT);
 

send_to_char("\n\r",ch);
send_to_char("\n\r",victim);


if (victim->pcdata->rank == AGE_ANCILLA)         xprintf( age, "Ancilla");
else if (victim->pcdata->rank == AGE_CHILDE)     xprintf( age, "Childe");
else if (victim->pcdata->rank == AGE_NEONATE)    xprintf( age, "Neonate");
else if (victim->pcdata->rank == AGE_ELDER)      xprintf( age, "Elder");
else if (victim->pcdata->rank == AGE_METHUSELAH) xprintf( age, "Methuselah");
else if (victim->pcdata->rank == AGE_LA_MAGRA)   xprintf( age, "La Magra");
else if (victim->pcdata->rank == AGE_TRUEBLOOD)  xprintf( age, "TrueBlood");
if (victim->lord == NULL) xprintf(lord, "None");
    else xprintf(lord, "%s",victim->lord);
    xprintf( lin,
"---------------------------------------------------------------------------\n\r");
send_to_char( lin,ch);
send_to_char(
"                              Vampire Status\n\r",ch);
send_to_char(lin,ch);
xprintf(buf,
"Generation:%d  Bloodpool:%d  Age:%s  Lord:%s\n\r",
victim->generation,victim->pcdata->condition[COND_THIRST],
age,lord);
send_to_char(buf,ch);
send_to_char(lin,ch);
send_to_char(
"                              Disciplines\n\r",ch);
send_to_char(lin,ch);
xprintf(buf,
"Animalism:    [%d]             Celerity:   [%d]             Fortitude: [%d]\n\r",
victim->power[DISC_VAMP_ANIM],
victim->power[DISC_VAMP_CELE],
victim->power[DISC_VAMP_FORT]);
send_to_char(buf,ch);
xprintf(buf,
"Obtenebration:[%d]             Presence:   [%d]             Quietus:   [%d]\n\r",
victim->power[DISC_VAMP_OBTE],
victim->power[DISC_VAMP_PRES],
victim->power[DISC_VAMP_QUIE]);
send_to_char(buf,ch);
xprintf(buf,
"Thaumaturgy:  [%d]             Auspex:     [%d]             Dominate:  [%d]\n\r",
victim->power[DISC_VAMP_THAU],
victim->power[DISC_VAMP_AUSP],
victim->power[DISC_VAMP_DOMI]);
send_to_char(buf,ch);
xprintf(buf,
"Obfuscate:    [%d]             Potence:    [%d]             Protean:   [%d]\n\r",
victim->power[DISC_VAMP_OBFU],
victim->power[DISC_VAMP_POTE],
victim->power[DISC_VAMP_PROT]);
send_to_char(buf,ch);
xprintf(buf,
"Serpentis:    [%d]             Vicissitude:[%d]             Daimoinon: [%d]\n\r",
victim->power[DISC_VAMP_SERP],
victim->power[DISC_VAMP_VICI],
victim->power[DISC_VAMP_DAIM]);
send_to_char(buf,ch);
send_to_char(lin,ch);
 
return;
}
 
 




void do_shadowstep( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if (ch->power[DISC_VAMP_OBTE] < 4)
	{
           send_to_char("You must obtain at least level 4 in Obtenebration to use this power.\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Shadowstep to who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
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

 if (IS_IMMUNE(victim, IMM_TRAVEL)) 
    {
    send_to_char("He doesnt want you near him!\n\r",ch);
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

    if (IS_NPC(victim) && (!room_is_dark(victim->in_room)))
    {
	send_to_char("You cant find a shadow in his room.\n\r",ch);
	return;
    }

    if ( room_is_private(victim->in_room ) )
    {
        send_to_char( "That room is private right now.\n\r", ch );
        return;
    }
    if (victim->in_room == ch->in_room)
    {
	send_to_char("But you're already there!\n\r",ch);
        return;
    }

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    act("You step into the shadows and appear before $N.", ch, NULL, victim, TO_CHAR);
    act("$n appears out of the shadows before $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n appears from the shadows in front of you.", ch, NULL, victim, TO_VICT);
    do_look(ch,"auto");
    return;
}

void do_earthmeld( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ch->power[DISC_VAMP_PROT] < 4 )
    {
        send_to_char("You must obtain at least level 4 Protean to use Earthmeld.\n\r",ch);
	return;
    }
    

    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

	if (has_timer(ch)) return;

    if ( IS_NPC(ch) || IS_EXTRA(ch,EXTRA_EARTHMELD) )
    {
            REMOVE_BIT(ch->affected_by, AFF_SHIFT);
            REMOVE_BIT(ch->extra, EXTRA_EARTHMELD);
            REMOVE_BIT(ch->act, PLR_WIZINVIS);
           if (IS_HEAD(ch,LOST_HEAD)) REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
            free_string(ch->morph);
            ch->morph = str_dup("");
           send_to_char( "You rise up from the ground.\n\r", ch );
            xprintf(buf, "%s rises up from the ground",ch->name);
            act(buf,ch,NULL,NULL,TO_ROOM);

            return;

    }

    if ( (mount = ch->mount) != NULL) do_dismount(ch,"");
    SET_BIT(ch->affected_by, AFF_SHIFT);
    SET_BIT(ch->extra, EXTRA_EARTHMELD);
    SET_BIT(ch->act, PLR_WIZINVIS);
    free_string(ch->morph);
    ch->morph = str_dup("Someone");
    send_to_char( "You sink into the ground.\n\r", ch );
    xprintf(buf, "%s sinks into the ground.",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    return;
}

void do_serenity( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;
    if(!IS_CLASS(ch, CLASS_VAMPIRE))
    {
        send_to_char("huh?.\n\r",ch);
        return;
    }
    if (ch->power[DISC_VAMP_ANIM] < 2)
    {
        send_to_char("You must obtain at least level 2 in Animalism to use Song of Serenity.\n\r",ch);
	return;
    }



    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->beast < 1)
    {
	if ( ch->rage < 1 )
	{
	    send_to_char("Your beast doesn't control your actions.\n\r",ch);
	    return;
	}
        send_to_char("You chant a soft tune and pacify your inner beast.\n\r",ch);
        act("$n chants a soft tune and pacifies their inner beast.",ch,NULL,NULL,TO_ROOM);
	ch->rage = 0;
	if (IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) do_nightsight(ch,"");
	if (IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	if (IS_VAMPAFF(ch, VAM_CLAWS)) do_claws(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    else 
    {
     send_to_char("Only those who achieve Golconda can sing the Song of Serenity.\n\r",ch);
     return;
    }
}

void do_theft( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    int bloodpool;
	int blpr; /* Blood sucked storage variable. Shakti */

    argument = one_argument( argument, arg );

    if (ch->generation <= 0)
        ch->generation = 4;

    {
    if (IS_SET(ch->newbits, NEW_TIDE))
    bloodpool = (3000 / ch->generation);
    else bloodpool = (2000 / ch->generation);
    } 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->power[DISC_VAMP_THAU] < 4)
    {
        send_to_char("You must obtain at least level 4 in Thaumaturgy to use Theft of Vitae.\n\r",ch);
	return;
    }


    if ( arg[0] == '\0' )
    {
        send_to_char( "Steal blood from whom?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
	if (IS_NPC(victim) && IS_SET(victim->act, ACT_NOAUTOKILL)) {
	send_to_char("You can't do that to them.\n\r", ch );
	return;}

    if (!IS_NPC(victim) && victim->pcdata->condition[COND_THIRST] <= 0)
    {
        send_to_char( "There isn't enough blood to steal.\n\r", ch);
        return;
    }
    if (!IS_NPC(victim) && IS_IMMORTAL(victim) && victim != ch )
    {
        send_to_char( "You can only steal blood from Avatar's or lower.\n\r", ch );
	return;
    }
if (is_safe(ch,victim) == TRUE) return;

if (!IS_NPC(victim)) {
        xprintf(buf,"A stream of blood shoots from %s into your body.",victim->name);
	act(buf,ch,NULL,victim,TO_CHAR);
        xprintf(buf,"A stream of blood shoots from %s into %s.",victim->name,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
        xprintf(buf,"A stream of blood shoots from your body into %s.",ch->name);
        act(buf,ch,NULL,victim,TO_VICT);
        } else {
        xprintf(buf,"A stream of blood shoots from %s into your body.",victim->short_descr);
	act(buf,ch,NULL,victim,TO_CHAR);
        xprintf(buf,"A stream of blood shoots from %s into %s.",victim->short_descr,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
        xprintf(buf,"A stream of blood shoots from your body into %s.",ch->name);
        act(buf,ch,NULL,victim,TO_VICT);


	}
        ch->pcdata->condition[COND_THIRST] += number_range(10,15);
     
		
		if (IS_NPC(victim))
        {
/* Raw-killing it from one theft is stupid. Im going to use the primal        */
/* stat on the mobs for blood its quick, effective, and straightford, AND	  */
/* no new fields have to be added to the mob.Shakti 09/07/98				  */
			(blpr = number_range (30,40) );
			(victim->practice -=blpr);
            (ch->pcdata->condition[COND_THIRST] += blpr);
		if (victim->practice < 0) {
		xprintf(buf,"%s falls over dead.",victim->short_descr);
		act(buf,ch,NULL,victim,TO_ROOM);
		act(buf,ch,NULL,victim,TO_CHAR);
		raw_kill(victim, ch);}		

		 if (ch->pcdata->condition[COND_THIRST] >= bloodpool /ch->generation)
             {
              ch->pcdata->condition[COND_THIRST] = bloodpool / ch->generation;
             }
	if (ch->fighting == NULL) set_fighting(ch,victim, TRUE);

         return;
        }
        if (IS_SET(victim->act, PLR_ACID))
        {
        send_to_char("Their blood is a deadly acid!\n\r", ch);
        hurt_person(victim,ch,300);
        victim->pcdata->condition[COND_THIRST] -= 30;
        return;
        }

    if (!IS_NPC(victim)) 
{
victim->pcdata->condition[COND_THIRST] -=number_range(30,40);
}
    if (ch->pcdata->condition[COND_THIRST] >= bloodpool / ch->generation)
    {
       ch->pcdata->condition[COND_THIRST] = bloodpool /ch->generation;
    }

   if (victim->pcdata->condition[COND_THIRST] <= 0)
    {
       victim->pcdata->condition[COND_THIRST] = 0; 
    }
	if (ch->fighting == NULL) set_fighting(ch,victim, TRUE);

   return;
}


void do_demonform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    
    if (!IS_DEMPOWER( ch, DEM_FORM) && IS_CLASS(ch,CLASS_DEMON))
    {
	send_to_char("You haven't been granted the gift of demonform.\n\r",ch);
	return;
    }
    
    if (IS_POLYAFF(ch, POLY_ZULOFORM))
    {   
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM);
        ch->damroll -= 200;
        ch->hitroll -= 200;
        ch->armor   += 300;
	free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
send_to_char("You cant demon form when changed.\n\r",ch);
return;
}
  
  
        if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
        act( "You transform into a huge demon.", ch, NULL, NULL, TO_CHAR);
        act( "$n's body grows and distorts into a huge demon.", ch, NULL, NULL, TO_ROOM );
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->polyaff, POLY_ZULOFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        xprintf(buf, "%s the huge hulking demon", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
	ch->damroll += 200;
	ch->hitroll += 200;
        ch->armor   -= 300;
        return;
    }
  




void do_zuloform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;
   

    if (ch->power[DISC_VAMP_VICI] < 2)
    {
        send_to_char("You must obtain at least level 2 in Vicissitude to use Zuloform.\n\r",ch);
	return;
    }

    if (IS_EXTRA(ch,EXTRA_DRAGON))
    {
    send_to_char("You cannot unzulo while a dragon.\n\r",ch);
    return;
    }
    if(IS_POLYAFF(ch, POLY_DRAGON))
    {
	stc( "You can't do that.\n\r",ch);
	return;
    }

    if (IS_POLYAFF(ch, POLY_ZULOFORM))
    {   
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM );
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 150;
        ch->hitroll = ch->hitroll - 150; 
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
send_to_char("You cant zulo when changed.\n\r",ch);
return;
}
  
  
    if ( ch->pcdata->condition[COND_THIRST] < 200 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}

        if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(100,200);
        act( "You transform into large beast.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body grows and distorts into a large beast.", ch, NULL, NULL, TO_ROOM );
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->polyaff, POLY_ZULOFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        xprintf(buf, "A big black monster");
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->damroll = ch->damroll + 150;
        ch->hitroll = ch->hitroll + 150;
        return;
    }
  


void horn args((CHAR_DATA *ch));

/* claw and fang commands */

void declaw(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_CLAWS)) return;

    send_to_char("You retract your claws.\n\r",ch);
    act("$n retracts $s claws.",ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
}

void dehorn(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_HORNS)) return;

    send_to_char("Your horns burrow back into your skull.\n\r",ch);
    act("$n's horns retract into $s skull.",ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
}

void horn(CHAR_DATA *ch)
{
    OBJ_DATA *obj;
  
    if (IS_VAMPAFF(ch, VAM_HORNS)) return;

    /* Only certain demons may use their horns */
    if (ch->power[DISC_DAEM_ATTA] < 4) return;

    send_to_char("Sharp horns grow out of your skull.\n\r",ch);
    act("Sharp horns grow out of $n's skull.",ch, NULL, NULL, TO_ROOM);

    if ( (obj = get_eq_char(ch, WEAR_HEAD)) != NULL) 
    { 
        act("Your horns shred $p.", ch, obj, NULL, TO_CHAR); 
        act("$n's horns shred $p.", ch, obj, NULL, TO_ROOM); 
        extract_obj(obj); 
    } 

    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
}

void defang(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_FANGS)) return;

    send_to_char("Your fangs slide back into your gums.\n\r",ch);
    act("$n retracts $s fangs back into $s gums.",ch, NULL, NULL, TO_ROOM);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
}

void do_isauto( CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  send_to_char("That power is automatic.\n\r",ch);
  return;
}

void do_claws(CHAR_DATA *ch, char *argument)
{
   if (IS_CLASS(ch, CLASS_WEREWOLF))
   {
	if (ch->power[DISC_WERE_WOLF] < 1)
      {
	    stc("Huh?\n\r",ch);
	    return;
	}
   }
   else if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (!IS_DEMPOWER( ch, DEM_CLAWS) && ch->power[DISC_DAEM_ATTA] < 1)
	{
	    send_to_char("You haven't been granted the gift of claws or attack is below level 1.\n\r",ch);
	    return;
	}
    }
   else if (IS_CLASS(ch, CLASS_VAMPIRE))
   {
	if (ch->power[DISC_VAMP_PROT] < 2)
	{
	    stc("Huh?\n\r",ch);
	    return;
	}
   }
   else if (IS_CLASS(ch, CLASS_TANARRI))
   {
   if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_CLAWS))
     {
       send_to_char("Huh?\n\r",ch);
       return;
     }
  }
   else
   {
	stc("Huh?\n\r",ch);
	return;
   }

   if (IS_VAMPAFF(ch, VAM_CLAWS))
   {
     if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_DEMON))
     {
	if (ch->rage >0)
	{    
	    send_to_char("You cannot retract your claws while the beast is within you.\n\r", ch);
	    return;
	}
     }
	send_to_char("You retract your claws.\n\r",ch);
	act("$n retracts $s claws.",ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   }
   else
   {
	send_to_char("Wicked claws extend from your fingers.\n\r",ch);
	act("Long sharp claws extend from $n's fingers.",ch, NULL, NULL, TO_ROOM);
	remove_obj( ch, WEAR_WIELD, TRUE );
	remove_obj( ch, WEAR_HOLD, TRUE );
	remove_obj( ch, WEAR_THIRD, TRUE );
	remove_obj( ch, WEAR_FOURTH, TRUE );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   }
}

void do_fangs(CHAR_DATA *ch, char *argument)
{
   if (IS_CLASS(ch, CLASS_WEREWOLF))
   {
	if (ch->power[DISC_WERE_WOLF] < 2)
        {
	    stc("Huh?\n\r",ch);
	    return;
	}
   }
	else if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (!IS_DEMPOWER( ch, DEM_FANGS) && ch->power[DISC_DAEM_ATTA] < 2)
	{
	    send_to_char("You haven't been granted the gift of fangs.\n\r",ch);
	    return;
	}
    }

   else if (!IS_CLASS(ch, CLASS_VAMPIRE))
   {
	stc("Huh?\n\r",ch);
	return;
   }

   if (IS_VAMPAFF(ch, VAM_FANGS))
   {
     if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_DEMON))
     {
	if (ch->rage >0)
	{    
	    send_to_char("You cannot retract your fangs while the beast is within you.\n\r", ch);
	    return;
	}
     }
	send_to_char("Your fangs slide back into your gums.\n\r",ch);
	act("$n retracts $s fangs back into $s gums.",ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
   }
   else
   {
	send_to_char("Sharp fangs slide out of your gums.\n\r",ch);
	act("Sharp fangs slide out of $n's gums.",ch, NULL, NULL, TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
   }
}

void do_nightsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_WEREWOLF))
    {/*supposed to be < but changedd while gifts are out */
	if ( ch->power[DISC_WERE_HAWK] < 1 )
	{
	    send_to_char("Your power in hawk is not great enough yet.\n\r",ch);
	    return;
	}
    }
    else if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (!IS_DEMPOWER( ch, DEM_EYES))
	{
	    send_to_char("You haven't been granted the gift of nightsight.\n\r",ch);
	    return;
	}
    }
    else if (IS_CLASS(ch, CLASS_MONK) )
    {
	if ( ch->monkab[AWARE] < 1 )
	{
	    stc("You must obtain level one in Awareness to use Nightsight.\n\r",ch);
	    return;
        }
    }
    
    else if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_PROT] < 1 &&
       ch->power[DISC_VAMP_OBTE] < 3) 
    {
        send_to_char("You must obtain at least level 1 in Protean or 3 in Obtenebration.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->rage > 0)
    {
	send_to_char("Not while your beast is in control.\n\r",ch);
	return;
    }

    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
    {
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
	return;
    }

    send_to_char("Your eyes start glowing red.\n\r",ch);
    act("$n's eyes start glowing red.", ch, NULL, NULL, TO_ROOM);
	
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    return;
}

void do_inconnu( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }


    if (ch->pcdata->rank < AGE_ELDER)
    {
	send_to_char("You must be at least an Elder to become an Inconnu.\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_INCONNU))
    {
	send_to_char("But you are already an Inconnu!\n\r",ch);
	return;
    }

    if (ch->exp < 1000000)
    {
	send_to_char("It costs 1000000 exp to become an Inconnu.\n\r",ch);
	return;
    }

    ch->exp -= 1000000;
    send_to_char("You are now an Inconnu.\n\r",ch);
    xprintf(buf,"%s is now an Inconnu!",ch->name);
    do_info(ch,buf);
    SET_BIT(ch->special, SPC_INCONNU);
    return;
}

void do_shadowsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_HAWK] < 2)
    {
	stc("Huh?\n\r",ch);
	return;
    }
    else if ( IS_CLASS(ch, CLASS_GHOUL) && ch->pcdata->powers[GPOWER_SHADOW] < 1 )
    {
    	stc("You need level 2 in Shadow Power\n\r",ch);
	return;
    }
    else if ( IS_CLASS(ch, CLASS_SKYBLADE) && ch->pcdata->powers[SKYBLADE_POWER] < 2)
    {
	stc("You need level 2 in Skyblade Power\n\r",ch);
	return;
    }
    else if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[AWARE] < 2 )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    else if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBTE] < 2)
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
    if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
    {
	send_to_char("You can no longer see between planes.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 10)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(5,10);
    send_to_char("You can now see between planes.\n\r",ch);
    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    return;
}

void do_class( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char      arg1 [MAX_INPUT_LENGTH];
  char      arg2 [MAX_INPUT_LENGTH];
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
        
  if ( IS_NPC(ch) ) return;
    
  if ( arg1[0] == '\0' || arg2[0] == '\0' )
  {
    send_to_char( "Syntax: class <char> <class>.\n\r", ch );
    send_to_char("#W--==[ #RMindcloud Classes #W]==--#n\n\r",ch);
  send_to_char("#0Demon,  	Werewolf,  	Drow,  		\n\r",ch);
  send_to_char("#0Ninja,  	Vampire,   	Shapeshifter\n\r",ch);
  send_to_char("#0Battlemage,	Tanar'ri,  	Undead Knight#n\n\r", ch);
  send_to_char("#0Cyborg,  	Samurai, 	Monk\n\r", ch);
  send_to_char("#0Angel,  	Lich,           Drone#n\n\r", ch);
  send_to_char("#0SkyBlade, 	Ghoul, 		Draconian#n\n\r", ch);
  send_to_char("#0Thief, 	Shadow,         Giant#n\n\r", ch);
  send_to_char("#0Priest, 	Jedi,           Fae\n\r", ch);
    return;
  }
  if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
  {
    send_to_char( "That player is not here.\n\r", ch);
    return;
  }
  do_clearcrap(ch,"");
  if ( !str_cmp( arg2, "demon"      ) ) 
  {
    victim->class = CLASS_DEMON;
    set_learnable_disciplines(victim);
    send_to_char("You are now a demon!\n\r", victim);
  }
  else if( !str_cmp(arg2,"fae"))
  {
    victim->class = CLASS_FAE;
    set_learnable_disciplines(victim);
    send_to_char("You are one of the Fae.\n\r",victim);
  }
  else if (!str_cmp(arg2, "drone"))
  {
    victim->class = CLASS_DRONE;
    send_to_char("You are now a drone.\n\r",victim);
  }
    else if (!str_cmp(arg2,"tanarri"))
   {
   victim->class = CLASS_TANARRI;
   send_to_char("You are now a Tanar'ri.\n\r",victim);
}
   else if ( !str_cmp(arg2, "draconian"))
{
	victim->class = CLASS_DRACONIAN;
	stc("You are now a Draconian!\n\r",victim);
}
   else if ( !str_cmp(arg2, "jedi"))
   {
	victim->class = CLASS_JEDI;
	stc("You are now a Jedi\n\r",victim);
   }
   else if ( !str_cmp(arg2, "priest"))
   {
        victim->class = CLASS_PRIEST;
        stc("You are now a priest!\n\r", victim);
   }
   else if ( !str_cmp(arg2, "shadow"))
   {
	victim->class = CLASS_SHADOW;
	stc("You are now a Dark Assasin Shadow!\n\r",victim);
   }
   else if ( !str_cmp(arg2, "thief"))
   {
	victim->class = CLASS_THIEF;
	stc("You are now a Dirty Bird Thief!\n\r",victim);
   }
   else if (!str_cmp(arg2,"ghoul"))
   {
   victim->class = CLASS_GHOUL;
   send_to_char("You are now a Freaky Ghoul.\n\r",victim);
   }
    else if (!str_cmp(arg2,"undeadknight"))
   {
   victim->class = CLASS_UNDEAD_KNIGHT;
   send_to_char("You are now an Undead Knight.\n\r",victim);
   }
    else if (!str_cmp(arg2,"shapeshifter"))
   {
   victim->class = CLASS_SHAPESHIFTER;
   send_to_char("You are now a Shapeshifter.\n\r",victim);
   }
    else if (!str_cmp(arg2,"angel"))
   {
   victim->class = CLASS_ANGEL;
   send_to_char("You are now an Angel.\n\r",victim);
   }
	else if (!str_cmp(arg2,"lich"))
   {
   victim->class = CLASS_LICH;
   send_to_char("You are now a Lich.\n\r",victim);
   }
   else if ( !str_cmp( arg2, "mage"  ) )
    {
    victim->class = CLASS_MAGE;
    send_to_char("You are now a mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "werewolf"   ) ) 
    {
    victim->class = CLASS_WEREWOLF;
    set_learnable_disciplines(victim);
    send_to_char("You are now a werewolf!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "vampire"    ) ) 
    {
    victim->class = CLASS_VAMPIRE;
    victim->beast = 30;
    set_learnable_disciplines(victim);
    send_to_char("You are now a vampire!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "cyborg"))
    {
      	victim->class = CLASS_CYBORG;
        send_to_char("You are now a Cyborg!\n\r", victim);
     }
    else if ( !str_cmp( arg2, "samurai" ) ) 
    {
    victim->class = CLASS_SAMURAI;
    send_to_char("You are now a Samurai!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "monk"      ) )
    {
     victim->class = CLASS_MONK;
     send_to_char("You are now a monk!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "skyblade"      ) )
    {
     victim->class = CLASS_SKYBLADE;
     send_to_char("You are now a skyblade!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "giant" ))
    {
      victim->class = CLASS_GIANT;
      send_to_char("You are now a giant.\n\r",victim);
    }
     else if ( !str_cmp( arg2, "drow"       ) ) 
    {
    victim->class = CLASS_DROW;
    send_to_char("You are now a drow!\n\r", victim);
    }  
   else if ( !str_cmp( arg2, "ninja"       ) )
    {
    victim->class = CLASS_NINJA;
    send_to_char("You are now a ninja!\n\r", victim);
    }
   else if ( !str_cmp( arg2, "none") )
    {
      victim->class=0;
      do_clearcrap(victim,"");
      send_to_char("Your class have been removed.\n\r",victim);
    }
   else
    {
        do_class(ch,"");
	return;
    }
    //send_to_char("Class Set.\n\r",ch);
    return;
}

void do_clearcrap( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int sn = 0;
    if (IS_NPC(ch)) return;
    

    powerdown(ch); /* remove class shit */

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
        if ( obj->wear_loc != WEAR_NONE )
        {obj_from_char(obj); obj_to_char(obj,ch);}
    }
     
    while ( ch->affected )
      affect_remove( ch, ch->affected );
      
    if (IS_SET(ch->affected_by, AFF_POLYMORPH)) REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    if (IS_SET(ch->affected_by, AFF_ETHEREAL)) REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);  
    if (IS_EXTRA(ch,EXTRA_DRAGON)) REMOVE_BIT(ch->extra, EXTRA_DRAGON);
    
    ch->affected_by      = 0;
    ch->armor            = 100;
    ch->hit              = UMAX( 1, ch->hit  );
    ch->mana             = UMAX( 1, ch->mana );
    ch->move             = UMAX( 1, ch->move );
    ch->hitroll          = 0;
    ch->damroll          = 0;
    ch->saving_throw     = 0;
    ch->pcdata->mod_str  = 0;
    ch->pcdata->mod_int  = 0;
    ch->pcdata->mod_wis  = 0;
    ch->pcdata->mod_dex  = 0;
    ch->pcdata->mod_con  = 0;
    ch->pcdata->followers = 0;
    for( sn=0 ; sn < 12 ; sn++ )
     if(ch->wpn[sn] > 100)
         ch->wpn[sn] = 100;
    for( sn=0 ; sn < 5 ; sn++ )
     if(ch->spl[sn] > 100)
         ch->spl[sn] = 100;
    for( sn=0 ; sn < 12 ; sn++ )
     if(ch->stance[sn] > 100)
         ch->stance[sn] = 100;
    for( sn=13 ; sn < 18 ; sn++ )
       ch->stance[sn] = 0;
    ch->stance[19] = -1;
    ch->stance[20] = -1;
    ch->stance[21] = -1;
    ch->stance[22] = -1;
    ch->stance[23] = -1;
    for (sn=0; sn < 20; sn++)
    {
	ch->pcdata->powers[sn] = 0;
    }
    ch->newbits               = 0;
    ch->newbits2              = 0;
    ch->sohbits               = 0;
    ch->rage                  = 0;
    ch->extra                           = 0;
    ch->morph                           = str_dup( "" );
    ch->vampgen_a                       = 0;
    ch->paradox[0]                      = 0;    /* Total paradox */
    ch->paradox[1]                      = 0;    /* Current Paradox */
    ch->paradox[2]                      = 0;    /* Paradox Ticker */
    ch->damcap[0]                       = 1000;
    ch->damcap[1]                       = 0; 
    ch->vampaff_a                       = 0;
    ch->itemaffect                      = 0;
    ch->polyaff                         = 0;
    save_char_obj( ch );
    return;
}

void do_generation( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int gen;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
        
    if ( IS_NPC(ch) ) return;
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: generation <char> <generation>.\n\r", ch );
    send_to_char("Generation 1 is a Master <Class> and 2 is clan leader.\n\r", ch);
	return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    
    gen = is_number( arg2 ) ? atoi( arg2 ) : -1;
    
    send_to_char("Generation Set.\n\r",ch);
    victim->generation = gen;
    return;
}



void reset_weapon( CHAR_DATA *ch, int dtype ) {
	if ( ch->wpn[dtype] > 200 )
		ch->wpn[dtype] = 200;
	return;
}

void reset_spell( CHAR_DATA *ch, int dtype )
{
	if ( ch->spl[dtype] > 200 )
		ch->spl[dtype] = 200;
	return;
}


void do_stake( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *stake;
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    

	return;

    argument = one_argument( argument, arg );
    if (IS_NPC( ch )) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stake whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    stake = get_eq_char( ch, WEAR_HOLD );
    if ( stake == NULL || stake->item_type != ITEM_STAKE )
    {
	stake = get_eq_char( ch, WEAR_WIELD );
	if ( stake == NULL || stake->item_type != ITEM_STAKE )
	{
	    send_to_char( "How can you stake someone down without holding a stake?\n\r", ch );
	    return;
	}
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot stake yourself.\n\r", ch );
	return;
    }

    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
	send_to_char( "You can only stake vampires.\n\r", ch );
	return;
    }

    if (victim->position > POS_MORTAL)
    {
	send_to_char( "You can only stake down a vampire who is mortally wounded.\n\r", ch );
	return;
    }

    act("You plunge $p into $N's heart.", ch, stake, victim, TO_CHAR);
    act("$n plunges $p into $N's heart.", ch, stake, victim, TO_NOTVICT);
    send_to_char( "You feel a stake plunged through your heart.\n\r", victim );
    if (IS_IMMUNE(victim,IMM_STAKE)) return;

    /* Have to make sure they have enough blood to change back */
    blood = victim->pcdata->condition[COND_THIRST];
    victim->pcdata->condition[COND_THIRST] = 666;

    /* To take care of vampires who have powers in affect. */
    if (IS_VAMPAFF(victim,VAM_DISGUISED) ) do_mask(victim,"self");
    if (IS_IMMUNE(victim,IMM_SHIELDED) ) do_shield(victim,"");
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) ) do_shadowplane(victim,"");
    if (IS_VAMPAFF(victim,VAM_FANGS) ) do_fangs(victim,"");
    if (IS_VAMPAFF(victim,VAM_CLAWS) ) do_claws(victim,"");
    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) ) do_nightsight(victim,"");
    if (IS_AFFECTED(victim,AFF_SHADOWSIGHT) ) do_shadowsight(victim,"");
    if (IS_SET(victim->act,PLR_HOLYLIGHT) ) do_truesight(victim,"");
    if (IS_VAMPAFF(victim,VAM_CHANGED) ) do_change(victim,"human");
    if (IS_POLYAFF(victim,POLY_SERPENT) ) do_serpent(victim,"");
    victim->rage = 0;
    victim->pcdata->condition[COND_THIRST] = blood;

    REMOVE_BIT(victim->class, CLASS_VAMPIRE);
    obj_from_char(stake);
    obj_to_char(stake,victim);
    ch->exp = ch->exp + 1000;
    victim->home = 3001;
    return;
}

void do_mask( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    if (ch->power[DISC_VAMP_OBFU] < 2 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
        send_to_char("You must obtain at least level 2 in Obfuscate to use Mask.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && ch->pcdata->powers[SHAPE_POWERS] < 4)
    {
      send_to_char("You need level 4 in general shapeshifting powers.\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Change to look like whom?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL(victim) && victim != ch )
    {
	send_to_char( "You can only mask avatars or lower.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 40 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE)) ch->pcdata->condition[COND_THIRST] -= number_range(30,40);

    if ( ch == victim )
    {
	if (!IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
	{
	    send_to_char( "You already look like yourself!\n\r", ch );
	    return;
	}
	xprintf(buf,"Your form shimmers and transforms into %s.",ch->name);
	act(buf,ch,NULL,victim,TO_CHAR);
	xprintf(buf,"%s's form shimmers and transforms into %s.",ch->morph,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    if (IS_VAMPAFF(ch,VAM_DISGUISED))
    {
    	xprintf(buf,"Your form shimmers and transforms into a clone of %s.",victim->name);
    	act(buf,ch,NULL,victim,TO_CHAR);
    	xprintf(buf,"%s's form shimmers and transforms into a clone of %s.",ch->morph,victim->name);
    	act(buf,ch,NULL,victim,TO_NOTVICT);
    	xprintf(buf,"%s's form shimmers and transforms into a clone of you!",ch->morph);
    	act(buf,ch,NULL,victim,TO_VICT);
    	free_string( ch->morph );
    	ch->morph = str_dup( victim->name );
	return;
    }
    xprintf(buf,"Your form shimmers and transforms into a clone of %s.",victim->name);
    act(buf,ch,NULL,victim,TO_CHAR);
    xprintf(buf,"%s's form shimmers and transforms into a clone of %s.",ch->name,victim->name);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    xprintf(buf,"%s's form shimmers and transforms into a clone of you!",ch->name);
    act(buf,ch,NULL,victim,TO_VICT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string( ch->morph );
    ch->morph = str_dup( victim->name );
    return;
}

void do_change( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_PROT] < 3  )
    {
        send_to_char("You must obtain at least level 3 in Protean to use Minor Change.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "You can change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r", ch );
	return;
    }


    if ( !str_cmp(arg,"bat") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r", ch );
	    return;
	}
    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
    clear_stats(ch);
	act( "You transform into bat form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a bat.", ch, NULL, NULL, TO_ROOM );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_BAT);*/
	SET_BIT(ch->polyaff, POLY_BAT);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	xprintf(buf, "%s the vampire bat", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"wolf") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r", ch );
	    return;
	}
    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act( "You transform into wolf form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a dire wolf.", ch, NULL, NULL, TO_ROOM );
    clear_stats(ch);
    	if (ch->wpn[0] > 0)
    	{
	    ch->hitroll += (ch->wpn[0]);
	    ch->damroll += (ch->wpn[0]);
	    ch->armor   -= (ch->wpn[0] * 3);
    	}
    	ch->pcdata->mod_str = 10;
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);*/
	SET_BIT(ch->polyaff, POLY_WOLF);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	xprintf(buf, "%s the dire wolf", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"mist") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r",
ch );
	    return;
	}

	if (has_timer(ch)) return;

    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}

	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act( "You transform into mist form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a white mist.", ch, NULL, NULL, TO_ROOM );
	if (IS_EXTRA(ch, TIED_UP))
	{
	    act("The ropes binding you fall through your ethereal form.",ch,NULL,NULL,TO_CHAR);
	    act("The ropes binding $n fall through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
	    REMOVE_BIT(ch->extra, TIED_UP);
	    REMOVE_BIT(ch->extra, GAGGED);
	    REMOVE_BIT(ch->extra, BLINDFOLDED);
	}
	if (is_affected(ch, gsn_web))
	{
	    act("The webbing entrapping $n falls through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
	    send_to_char("The webbing entrapping you falls through your ethereal form.\n\r",ch);
	    affect_strip(ch, gsn_web);
	}
    clear_stats(ch);
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_MIST);*/
	SET_BIT(ch->polyaff, POLY_MIST);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	SET_BIT(ch->affected_by, AFF_ETHEREAL);
	xprintf(buf, "%s the white mist", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"human") )
    {
       if (!IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You are already in human form.\n\r", ch );
	    return;
	}
	if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_BAT))
	{
	    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
	    REMOVE_BIT(ch->polyaff, POLY_BAT);
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_BAT);*/
	}
	else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WOLF))
	{
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);*/
	    REMOVE_BIT(ch->polyaff, POLY_WOLF);
	    if (ch->hit < 1) ch->hit = 1;
	}
	else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_MIST))
	{
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_MIST);*/
	    REMOVE_BIT(ch->polyaff, POLY_MIST);
	    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
	}
	else
	{
	    /* In case they try to change to human from a non-vamp form */
	    send_to_char( "You seem to be stuck in this form.\n\r", ch );
	    return;
	}
	act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into human form.", ch, NULL, NULL, TO_ROOM );
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
    clear_stats(ch);
  	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    else
	send_to_char( "You can change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r", ch );
    return;
}

void do_clandisc( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        do_stat(ch,"");
        return;
    }
    if (arg2[0] == '\0')
    {
        if (!str_cmp(arg1,"animalism"))
	{
            send_to_char("Animalism: The discipline of nature control.\n\r",ch);
            if (ch->power[DISC_VAMP_ANIM] < 1) 
                send_to_char("You have none of the Animalism powers.\n\r",ch);
            if (ch->power[DISC_VAMP_ANIM] > 0) 
                send_to_char("Serenity: You can sooth the most aggressive soul (Golconda Required).\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"auspex"))
	{
            send_to_char("Auspex: The discipline of extrasensory awareness.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] < 1) 
                send_to_char("You have none of the Auspex discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] > 0) 
                send_to_char("Truesight: The user can see everything, including invis.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] > 1) 
                send_to_char("Scry: Allows you to view the room the specified target is in.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] > 2) 
                send_to_char("Readaura: Gives you detained information about a creature or object.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"celerity"))
	{
            send_to_char("Celerity: The discipline of superior speed.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"chimerstry"))
        {
            send_to_char("chimerstry: The discipline of illusion.\n\r",ch);
            return;
        }
	else if (!str_cmp(arg1,"daimoinon"))
	{
            send_to_char("Daimoinon: The discipline of dark corruption.\n\r",ch);
            if (ch->power[DISC_VAMP_DAIM] < 1) 
                send_to_char("You have none of the Daimoinon discipline powers.\n\r",ch);
	    return;
        }

        else if (!str_cmp(arg1,"dominate"))
	{
            send_to_char("Dominate: The discipline of control.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] < 1) 
                send_to_char("You have none of the Dominate discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] > 0) 
                send_to_char("Command: You can plant suggestions in the mind of others.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] > 1) 
                send_to_char("Shield: You are able to shield your mind from scrying and aura-reading.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] > 3) 
                send_to_char("Possession: You can take control of mobs by feeding them blood.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"fortitude"))
	{
            send_to_char("Fortitude: The discipline of supernatural toughness and vigor.\n\r",ch);
            if (ch->power[DISC_VAMP_FORT] < 1) 
		send_to_char("You have no fortitude.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"obfuscate"))
	{
            send_to_char("Obfuscate: The discipline of stealth.\n\r",ch);
            if (ch->power[DISC_VAMP_OBFU] < 1) 
                send_to_char("You have none of the Obfuscate discipline powers.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"obtenebration"))
	{
            send_to_char("Obtenebration: The discipline of darkness manipulation.\n\r",ch);
            if (ch->power[DISC_VAMP_OBTE] < 1) 
                send_to_char("You have none of the Obtenebration discipline powers.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"potence"))
	{
            send_to_char("Potence: The discipline of superior strength.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"presence"))
	{
            send_to_char("Presence: The discipline of appeal and attractiveness.\n\r",ch);
            if (ch->power[DISC_VAMP_PRES] < 1) 
                send_to_char("You have none of the Presence discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"protean"))
	{
            send_to_char("Protean: The discipline of neutral transformation.\n\r",ch);
            if (ch->power[DISC_VAMP_PROT] < 1) 
                send_to_char("You have none of the Protean discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"quietus"))
	{
            send_to_char("Quietus: The discipline of death.\n\r",ch);
            if (ch->power[DISC_VAMP_QUIE] < 1) 
                send_to_char("You have none of the Quietus discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_QUIE] > 3)
                send_to_char("Spit: You can project your deadly spit at a foe\n\r",ch); 
           return;
        }
        else if (!str_cmp(arg1,"serpentis"))
	{
            send_to_char("Serpentis: The discipline of temptation and corruption.\n\r",ch);
            if (ch->power[DISC_VAMP_SERP] < 1) 
                send_to_char("You have none of the Serpentis discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"thaumaturgy"))
	{
            send_to_char("Thaumaturgy: The discipline of blood magic.\n\r",ch);
            if (ch->power[DISC_VAMP_THAU] < 1) 
                send_to_char("You have none of the Thaumaturgy discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_THAU] > 0) 
                send_to_char("Theft: You can take from the blood pool of other players.\n\r",ch);

            return;
        }
        else if (!str_cmp(arg1,"vicissitude"))
	{
            send_to_char("Vicissitude: The discipline of evil body control.\n\r",ch);
            if (ch->power[DISC_VAMP_VICI] < 1) 
                send_to_char("You have none of the Vicissitude discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_VICI] > 3) 
                send_to_char("Zuloform: Transformation into a powerful being.\n\r",ch);

            return;
        }
        do_stat(ch,"");
        return;

    }
    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;
        int maxlevel;

	stc("Please use the new commands, Disciplines and research.\n\r",ch);
	return;

          maxlevel=10;
             if (!str_cmp(arg1,"animalism"       )) {improve = DISC_VAMP_ANIM;          max=maxlevel;}
        else if (!str_cmp(arg1,"auspex"          )) {improve = DISC_VAMP_AUSP;          max=maxlevel;}
        else if (!str_cmp(arg1,"celerity"        )) {improve = DISC_VAMP_CELE;        max=maxlevel;}
        else if (!str_cmp(arg1,"chimerstry"      )) {improve = DISC_VAMP_CHIM; max=maxlevel;}
	else if (!str_cmp(arg1,"dominate"        )) {improve = DISC_VAMP_DOMI;        max=maxlevel;}
        else if (!str_cmp(arg1,"daimoinon"       )) {improve = DISC_VAMP_DAIM;max=maxlevel;}
        else if (!str_cmp(arg1,"fortitude"       )) {improve = DISC_VAMP_FORT;       max=12;}
        else if (!str_cmp(arg1,"obfuscate"       )) {improve = DISC_VAMP_OBFU;       max=maxlevel;}
        else if (!str_cmp(arg1,"obtenebration"   )) {improve = DISC_VAMP_OBTE;   max=maxlevel;}
        else if (!str_cmp(arg1,"potence"         )) {improve = DISC_VAMP_POTE;         max=maxlevel;}
        else if (!str_cmp(arg1,"presence"        )) {improve = DISC_VAMP_PRES;        max=maxlevel;}
        else if (!str_cmp(arg1,"protean"         )) {improve = DISC_VAMP_PROT;         max=maxlevel;}
        else if (!str_cmp(arg1,"quietus"         )) {improve = DISC_VAMP_QUIE;         max=maxlevel;}
        else if (!str_cmp(arg1,"serpentis"       )) {improve = DISC_VAMP_SERP;       max=maxlevel;}
        else if (!str_cmp(arg1,"thaumaturgy"     )) {improve = DISC_VAMP_THAU;            max=maxlevel;}
        else if (!str_cmp(arg1,"vicissitude"     )) {improve = DISC_VAMP_VICI;     max=maxlevel;}
	else if (!str_cmp(arg1,"obeah"		 )) {improve =
DISC_VAMP_OBEA; max = maxlevel; }
	else if (!str_cmp(arg1,"melpominee"	 )) {improve =
DISC_VAMP_MELP; max = maxlevel; }
	else if (!str_cmp(arg1,"necromancy"	 )) {improve =
DISC_VAMP_NECR; max = maxlevel; }
	else if (!str_cmp(arg1,"thanatosis"	 )) {improve =
DISC_VAMP_THAN; max = maxlevel; }
        else
	{
            send_to_char("You can improve: Animalism, Auspex, Celerity, Dominate, Fortitude, Obfuscate,\n\r Obtenebration, Potence, Presence, Protean, Quietus, Serpentis,\n\r Thaumaturgy or Vicissitude.\n\r",ch);
	    return;
	}
        cost = (ch->power[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->power[improve] >= max && ch->pcdata->rank ==AGE_METHUSELAH)
	{
            xprintf(buf,"You have already gained all the powers of the %s discipline.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
        if (ch->power[improve] >= max)
        {
           xprintf(buf,"You have already maxed %s.\n\r",arg1);
           send_to_char(buf,ch);
           return;
        }
	if ( cost > ch->practice )
	{
            xprintf(buf,"It costs you %d primal to improve your %s discipline.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->power[improve] += 1;
	ch->practice -= cost;
        xprintf(buf,"You improve your ability in the %s discipline.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a discipline, type: Clandisc <discipline type> improve.\n\r",ch);
    return;
}


void do_vampire( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_IMMUNE(ch, IMM_VAMPIRE))
    {
	send_to_char("You will now allow vampires to bite you.\n\r",ch);
	SET_BIT(ch->immune, IMM_VAMPIRE);
	return;
    }
    send_to_char("You will no longer allow vampires to bite you.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_VAMPIRE);
    return;
}

void do_shadowplane( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) || ch->power[DISC_WERE_OWL] < 3)
    {
	if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_GHOUL))
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
	if (ch->pcdata->powers[GPOWER_SHADOW] < 3  && IS_CLASS(ch, CLASS_GHOUL))
	{
            send_to_char("You must obtain at least level 3 in Shadow to use the Shadowplane.\n\r",ch);
            return;
    }
        if (ch->power[DISC_VAMP_OBTE] < 3 && IS_CLASS(ch, CLASS_VAMPIRE))
	{
            send_to_char("You must obtain at least level 3 in Obtenebration to use Shadowplane.\n\r",ch);
	    return;
	}
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 75)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    
    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(65,75);

    if ( arg[0] == '\0' )
    {
    	if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	{
            if (has_timer(ch)) return;
	    send_to_char("You fade into the plane of shadows.\n\r",ch);
	    act("The shadows flicker and swallow up $n.",ch,NULL,NULL,TO_ROOM);
	    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    do_look(ch,"auto");
	    return;
    	}
    	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    	send_to_char("You fade back into the real world.\n\r",ch);
	act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM);
	do_look(ch,"auto");
	return;
    }

    if ( ( obj = get_obj_here( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "What do you wish to toss into the shadow plane?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	send_to_char( "You toss it to the ground and it vanishes.\n\r", ch );
    else
    	send_to_char( "You toss it into a shadow and it vanishes.\n\r", ch );
    return;
}


void do_darkheart( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_SERP] < 1  )
    {
        send_to_char("You must obtain at least level 1 in Serpentis to use Darkheart.\n\r",ch);
	return;
    }
    if (IS_IMMUNE(ch,IMM_STAKE) )
    {
	send_to_char("But you've already torn your heart out!\n\r",ch);
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 1000 / (10 - ch->generation) )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= 100;
    send_to_char("You rip your heart from your body and toss it to the ground.\n\r",ch);
    act("$n rips $s heart out and tosses it to the ground.", ch, NULL, NULL, TO_ROOM);
    make_part( ch, "heart" );
    ch->hit = ch->hit - number_range(10,20);
    update_pos(ch);
    if (ch->position == POS_DEAD && !IS_HERO(ch))
    {
	send_to_char( "You have been KILLED!!\n\r\n\r", ch );
	raw_kill(ch, ch);
	return;
    }
    SET_BIT(ch->immune, IMM_STAKE);
    return;
}

void do_truesight( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;

  if (ch->class == 0)
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }

  if (IS_CLASS(ch, CLASS_TANARRI) && !IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_TRUESIGHT))
  {
    send_to_char("You don't have that power yet",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_PRIEST))
    {
     if (!IS_PRIPOWER( ch, PRI_SIGHT))
     {
       send_to_char("You need to learn that skill first.\n\r",ch);
       return;
     }
    }
  if (IS_CLASS(ch, CLASS_SHADOW) && !IS_SET(ch->pcdata->powers[SHADOW_POWERS], NSHADOWS_SIGHT))
  {
  	stc("You haven't gained that skill yet\n\r",ch);
  	return;
  }
 if ((ch->power[DISC_VAMP_AUSP] < 1) && IS_CLASS(ch, CLASS_VAMPIRE))
	{
	send_to_char("You must obtain at least level 1 in Auspex to use Truesight.\n\r", ch);
        return;
}
     if (IS_CLASS(ch, CLASS_FAE) && ch->power[DISC_FAE_ARCANE] < 6)
  {
    send_to_char("Your mastery of the arcane is pathetic.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_DRONE) && ch->pcdata->powers[DRONE_HORRID_REALITY] < 1)
  {
    send_to_char("You cannot grasp the truth of this horrid reality.\n\r", ch);
    return;
  }
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && ch->pcdata->powers[SHAPE_POWERS] < 1)
       {
        send_to_char("You must obtain at least level 1 in shiftpowers to use Truesight.\n\r", ch);
        return;
       }
    if ((ch->power[DISC_WERE_HAWK] < 3) && IS_CLASS(ch,CLASS_WEREWOLF))
        {
	send_to_char("You must obtain level 3 in the Hawk totem to use Truesight.\n\r",ch);
	return;
	}
    
	if (!IS_DEMPOWER( ch, DEM_TRUESIGHT) && IS_CLASS(ch,CLASS_DEMON))
    {
        stc("You must inpart truesight first.\n\r",ch);
        return;
    }

   if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[AWARE] < 3 )
    {
	stc("You must obtain level 4 in Awareness first.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_GHOUL) && ch->pcdata->powers[GPOWER_SHADOW] < 2)
    {
    	stc("You don't have that power yet.\n\r", ch);
    	return;
    }
    if (IS_CLASS(ch, CLASS_SKYBLADE) && ch->pcdata->powers[SKYBLADE_POWER] < 1)
    {
    	stc("You must raise your skyblade power first .\n\r", ch);
    	return;
    }
   
    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses return to normal.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses increase to incredible proportions.\n\r", ch );
    }

    return;
}

void do_majesty( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_PRES] < 5  )
    {
        send_to_char("You must obtain at least level 5 in Presence to use Majesty.\n\r",ch);
	return;
    }

    if (has_timer(ch)) return;

    if (IS_AFFECTED(ch, AFF_PEACE))
    {
        REMOVE_BIT(ch->affected_by, AFF_PEACE);
        act( "You lower your majesty.",  ch, NULL, NULL, TO_CHAR    );
        act( "$n looks less imposing.",  ch, NULL, NULL, TO_NOTVICT );
        return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 45 )
    {
	send_to_char( "You have insufficient blood.\n\r", ch );
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= number_range(35,45);
    SET_BIT(ch->affected_by, AFF_PEACE);
    act( "You raise your majesty.",  ch, NULL, NULL, TO_CHAR    );
    act( "$n suddenly looks very imposing.",  ch, NULL, NULL, TO_NOTVICT);
    return;
}

void do_scry( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];
    int rand_room;
    bool chaosvalue = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_ITEMAFF(ch, ITEMA_VISION) && !ragnarok)
    {
	if (!IS_CLASS(ch, CLASS_VAMPIRE) 
	 && !IS_CLASS(ch, CLASS_MONK)
	 && !IS_CLASS(ch, CLASS_DEMON)
         && !IS_CLASS(ch, CLASS_MAGE)
         && !IS_CLASS(ch, CLASS_FAE)
         && !IS_CLASS(ch, CLASS_GIANT)
         && !IS_CLASS(ch, CLASS_SHADOW)
         && !IS_CLASS(ch, CLASS_CYBORG)
         && !IS_CLASS(ch, CLASS_PRIEST))
	{
	    stc("Huh?\n\r",ch);
	    return;
	}
    }

  if ( !IS_ITEMAFF(ch, ITEMA_VISION) && !ragnarok)
  {
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_AUSP] < 2 )
    {
        send_to_char("You must obtain at least level 2 Auspex to use Scry.\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 3)
    {
	send_to_char("You need the trio mantra to use scry.\n\r",ch);
	return;
    }
     
    if (IS_CLASS(ch, CLASS_GIANT) && ch->pcdata->rank < FOOT_25)
    {
      send_to_char("You are not that high, the trees are blocking your view.\n\r",ch);
      return;
    }

    if (IS_CLASS(ch, CLASS_FAE) && ch->power[DISC_FAE_NATURE] < 3)
    {
      send_to_char("You don't have that power yet.\n\r",ch);
      return;
    }

    if (IS_CLASS(ch, CLASS_SHADOW) && !IS_SET(ch->pcdata->powers[SHADOW_POWERS], NSHADOWS_SCRY))
    {
      stc("You don't have that power.\n\r", ch);
      return;
    }
    if (IS_CLASS(ch, CLASS_PRIEST))
    {
        if (!IS_PRIPOWER(ch, PRI_SCRY))
        {
            send_to_char("You haven't mastered scry yet!\n\r",ch);
            return;
        }
    }
  if( IS_CLASS(ch, CLASS_MAGE) && ch->pcdata->powers[PINVOKE] < 3 )
	{
	stc( "You don't have the power to scry yet.\n\r", ch);
	return;
	}
	
	if (IS_CLASS(ch,CLASS_CYBORG) &&
	ch->pcdata->powers[CYBORG_FACE] < 5)
	{
	stc("You must have full facial implants to scry.\n\r",ch);
	return;
	}

    if (IS_CLASS(ch, CLASS_DEMON) && !IS_DEMPOWER(ch, DEM_SCRY) )
    {
	stc("You have not been granted the power of Scry.\n\r",ch);
	return;
    }
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

    if (!IS_NPC(victim) && IS_IMMUNE(victim,IMM_SHIELDED) && !ragnarok)
    {
       send_to_char( "You cannot find them.\n\r",ch);
       return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_SHIELD2)) chaosvalue = TRUE;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 25 
	&& !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    if (!IS_ITEMAFF(ch, ITEMA_VISION) && IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(15,25);
    
    if (!IS_NPC(victim) && (victim->pcdata->powers[JPOWER_SPIRIT] < 2) && IS_CLASS(victim, CLASS_JEDI))
    {
		send_to_char("#GYou sense the force moving as you are being watched.#n\n\r", victim);
	}
    chroom = ch->in_room;
    victimroom = victim->in_room;
    if (chaosvalue && !ragnarok)
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
    if (!IS_NPC(victim)) ch->fight_timer += 5;
    
    if ( IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->flag2, AFF_SPIRITGUARD))
    {
      act("Your spirit guardian sends you a message:",ch,NULL,victim,TO_VICT);
      act("\"$n is scrying you.\"",ch,NULL,victim,TO_VICT);
    }
    return;
}

void do_readaura( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    bool chaosvalue = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ITEMAFF(ch, ITEMA_VISION)
	&& !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch,CLASS_CYBORG)
	&&  !IS_CLASS(ch, CLASS_LICH) && !IS_CLASS(ch, CLASS_FAE) 
	&&  !IS_CLASS(ch, CLASS_PRIEST) && !IS_CLASS(ch, CLASS_DRONE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_AUSP] < 3 && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
        send_to_char("You must obtain at least level 3 in Auspex to use Readaura.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_DRONE) && ch->pcdata->powers[DRONE_MINDCONTROL] < 2)
    {
      send_to_char("You don't have the skills to read their mind.\n\r", ch);
      return;
    }
    if (IS_CLASS(ch, CLASS_LICH) && ch->pcdata->powers[DEATH_LORE] < 1)
    {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
    }
    if (IS_CLASS(ch, CLASS_PRIEST))
    {
        if (!IS_PRIPOWER( ch, PRI_READ))
        {
           send_to_char("Try mastering readaura first, Genius.\n\r",ch);
           return;
        }
    }
    
    if (IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 2)
    {
	send_to_char("You must obtain the level 2 Mantra first!\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_FAE) && ch->power[DISC_FAE_NATURE] < 5)
    {
      stc("Nay, not yet kid.\n\r",ch);
      return;
    }

    if (IS_CLASS(ch,CLASS_CYBORG) && ch->pcdata->powers[CYBORG_FACE] < 5)
    {
	stc("You must have full face implants to readaura.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Read the aura on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg, ch) ) == NULL )
	{
	    send_to_char( "Read the aura on what?\n\r", ch );
	    return;
	}

	act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
	spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	return;
    }

  if (victim->in_room)
  {
    if (has_area_affect(victim->in_room->area, AREA_AFF_BADMOON, 0))
    {
      send_to_char("Some powerful magic prevent your aura reading attempt.\n\r", ch);
      return;
    }
  }

    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
(IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("You are unable to read their aura.\n\r",ch);
	return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_SHIELD2))
      chaosvalue = TRUE;

    act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you intently.",ch,NULL,victim,TO_VICT);
    if (IS_NPC(victim)) xprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
    else 
    {
	if      (victim->level == 12) xprintf(buf, "%s is an Implementor.\n\r", victim->name);
	else if (victim->level == 11) xprintf(buf, "%s is a High Judge.\n\r", victim->name);
	else if (victim->level == 10) xprintf(buf, "%s is a Judge.\n\r", victim->name);
	else if (victim->level == 9 ) xprintf(buf, "%s is an Enforcer.\n\r", victim->name);
	else if (victim->level == 8 ) xprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
	else if (victim->level == 7 ) xprintf(buf, "%s is a Builder.\n\r", victim->name);
	else if (victim->level >= 3 ) xprintf(buf, "%s is an Avatar.\n\r", victim->name);
	else xprintf(buf, "%s is a Mortal.\n\r", victim->name);
    }
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	xprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
	send_to_char(buf,ch);
    }
    if (chaosvalue) xprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r", number_range(1000,5000), number_range(10000,20000) , number_range(1000,5000),
number_range(10000,20000), number_range(1000,5000), number_range(10000,20000));
    else
      xprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
      if (chaosvalue) xprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r", number_range(500,2500), number_range(500,2500), number_range(-2000,0));
      else
        xprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
    }
    else xprintf(buf,"AC:%d.\n\r",char_ac(victim));
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	xprintf(buf,"Status:%d, ",victim->race);
	send_to_char(buf,ch);
	if (IS_CLASS(victim, CLASS_VAMPIRE))    
	{
	    xprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
	    send_to_char(buf,ch);
	}
    }
    xprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("$N is pregnant.",ch,NULL,victim,TO_CHAR);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))    
    {
	send_to_char("Disciplines:",ch);
	if (IS_VAMPAFF(victim, VAM_PROTEAN)) send_to_char(" Protean",ch);
	if (IS_VAMPAFF(victim, VAM_CELERITY)) send_to_char(" Celerity",ch);
	if (IS_VAMPAFF(victim, VAM_FORTITUDE)) send_to_char(" Fortitude",ch);
	if (IS_VAMPAFF(victim, VAM_POTENCE)) send_to_char(" Potence",ch);
	if (IS_VAMPAFF(victim, VAM_OBFUSCATE)) send_to_char(" Obfuscate",ch);
	if (IS_VAMPAFF(victim, VAM_OBTENEBRATION)) send_to_char(" Obtenebration",ch);
	if (IS_VAMPAFF(victim, VAM_SERPENTIS)) send_to_char(" Serpentis",ch);
	if (IS_VAMPAFF(victim, VAM_AUSPEX)) send_to_char(" Auspex",ch);
	if (IS_VAMPAFF(victim, VAM_DOMINATE)) send_to_char(" Dominate",ch);
	if (IS_VAMPAFF(victim, VAM_PRESENCE)) send_to_char(" Presence",ch);
        if (IS_VAMPAFF(victim, VAM_VICISSITUDE)) send_to_char(" Vicissitude",ch);
        if (IS_VAMPAFF(victim, VAM_THAU)) send_to_char(" Thaumaturgy",ch);
        if (IS_VAMPAFF(victim, VAM_ANIMAL)) send_to_char(" Animalism",ch);
        if (IS_VAMPAFF(victim, VAM_QUIETUS)) send_to_char(" Quietus",ch);
        send_to_char(".\n\r",ch);
    }
    return;
}

void do_mortal( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_VAMPAFF(ch, VAM_MORTAL))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_OBFU] < 4)
    {
        send_to_char("You must obtain at least level 4 in Obfuscate to use Mortal Facade.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch,CLASS_VAMPIRE) )
    {
        if ( ch->pcdata->condition[COND_THIRST] < 1000 / (10 - ch->generation) )
        {
	    send_to_char("You must be at full blood to use this power.\n\r",ch);
	    return;
    	}
    	/* Have to make sure they have enough blood to change back */
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;

	/* Remove physical vampire attributes when you take mortal form */
    	if (IS_VAMPAFF(ch,VAM_DISGUISED) ) do_mask(ch,"self");
    	if (IS_IMMUNE(ch,IMM_SHIELDED) ) do_shield(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) ) do_shadowplane(ch,"");
    	if (IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) ) do_shadowsight(ch,"");
    	if (IS_SET(ch->act,PLR_HOLYLIGHT) ) do_truesight(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CHANGED) ) do_change(ch,"human");
    	if (IS_POLYAFF(ch,POLY_SERPENT) ) do_serpent(ch,"");
	ch->rage = 0;
    	ch->pcdata->condition[COND_THIRST] = blood;

    	send_to_char("Colour returns to your skin and you warm up a little.\n\r",ch);
    	act("Colour returns to $n's skin.", ch, NULL, NULL, TO_ROOM);

    	REMOVE_BIT(ch->class, CLASS_VAMPIRE);
    	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
	return;
    }
    send_to_char("You skin pales and cools.\n\r",ch);
    act("$n's skin pales slightly.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->class, CLASS_VAMPIRE);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
    return;
}

void do_mortalvamp( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (IS_CLASS(ch,CLASS_VAMPIRE) )
    {
    	/* Have to make sure they have enough blood to change back */
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;

	/* Remove physical vampire attributes when you take mortal form */
    	if (IS_VAMPAFF(ch,VAM_DISGUISED) ) do_mask(ch,"self");
    	if (IS_IMMUNE(ch,IMM_SHIELDED) ) do_shield(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) ) do_shadowplane(ch,"");
    	if (IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) ) do_shadowsight(ch,"");
    	if (IS_SET(ch->act,PLR_HOLYLIGHT) ) do_truesight(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CHANGED) ) do_change(ch,"human");
    	if (IS_POLYAFF(ch,POLY_SERPENT) ) do_serpent(ch,"");
	ch->rage = 0;
    	ch->pcdata->condition[COND_THIRST] = blood;

    	send_to_char("You loose your vampire powers.\n\r",ch);

	return;
    }
    send_to_char("You regain your vampire powers.\n\r",ch);
    return;
}

void do_shield( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_WEREWOLF) &&
      !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_MONK) &&
      !IS_CLASS(ch, CLASS_CYBORG) && !IS_CLASS(ch, CLASS_SHADOW) &&
      !IS_CLASS(ch, CLASS_DRONE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_OWL] < 2)
  {
    send_to_char("Sorry, you don't have that power yet.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_SHADOW) && !IS_SET(ch->pcdata->powers[SHADOW_POWERS], NSHADOWS_SHIELD))
  {
    send_to_char("You don't have that power yet.\n\r", ch);
    return;
  }
  if (!IS_DEMPOWER( ch, DEM_SHIELD) && IS_CLASS(ch,CLASS_DEMON))
  {
    send_to_char("You must inpart it first!\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_PRIEST))
    {
        if (!IS_PRIPOWER( ch, PRI_SHIELD))
        {
            send_to_char("You should probably pgain shield first.\n\r",ch);
            return;
        }
    }
  if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->pcdata->powers[1],DPOWER_DROWSHIELD))
  {
    send_to_char("You don't have that power.\n\r", ch );
    return;
  }
  if (IS_CLASS(ch, CLASS_MONK) && !IS_NPC(ch) && ch->pcdata->powers[PMONK] < 2)
  {
    send_to_char("You must improve your Mantras to level 2 first!\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_CYBORG) && ch->pcdata->powers[CYBORG_FACE]<4)
  {
    stc("You must have 4 implants before using shield.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_DRONE) && ch->pcdata->powers[DRONE_WILLPOWER] < 2)
  {
    send_to_char("You don't have the willpower to invoke the fortress of the mind.\n\r", ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBFU] < 3 && ch->power[DISC_VAMP_DOMI] < 2)
  {
    send_to_char("You must obtain at least level 3 in Obfuscate or level 2 in Dominate.\n\r",ch);
    return;
  }
  if (!IS_IMMUNE(ch,IMM_SHIELDED) )
  {
    send_to_char("You shield your aura from those around you.\n\r",ch);
    SET_BIT(ch->immune, IMM_SHIELDED);
    return;
  }
  send_to_char("You stop shielding your aura.\n\r",ch);
  REMOVE_BIT(ch->immune, IMM_SHIELDED);
  return;
}

void do_serpent( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_SERP] < 2)
    {
        send_to_char("You must obtain at least level 2 in Serpentis to use Serpent Form.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch, AFF_POLYMORPH))
    {
	if (!IS_POLYAFF(ch,POLY_SERPENT))
	{
	    send_to_char( "You cannot polymorph from this form.\n\r", ch );
	    return;
	}
	act( "You transform back into human.", ch, NULL, NULL, TO_CHAR );
	act( "$n transform into human form.", ch, NULL, NULL, TO_ROOM );
    clear_stats(ch);
	REMOVE_BIT(ch->polyaff, POLY_SERPENT);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	free_string( ch->morph );
	ch->morph = str_dup( "" );
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 50 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
    clear_stats(ch);
    if (ch->wpn[0] > 0)
    {
    	ch->hitroll += ch->wpn[0] * 0.75;
    	ch->damroll += ch->wpn[0] * 0.75;
    	ch->armor   -= ch->wpn[0] * 3;
    }
    if (ch->stance[0] != -1) do_stance(ch,"");
    if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    ch->pcdata->mod_str = 10;
    act( "You transform into a huge serpent.", ch, NULL, NULL, TO_CHAR );
    act( "$n transforms into a huge serpent.", ch, NULL, NULL, TO_ROOM );
    SET_BIT(ch->polyaff, POLY_SERPENT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    xprintf(buf, "%s the huge serpent", ch->name);
    free_string( ch->morph );
    ch->morph = str_dup( buf );
    return;
}

void do_poison( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_SERP] < 3)
    {
        send_to_char("You must obtain at least level 3 in Serpentis to use Poison.\n\r",ch);
	return;
    }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) )
    {
	send_to_char( "You must wield the weapon you wish to poison.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 )
    {
	send_to_char( "This weapon cannot be poisoned.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 15 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(5,15);
    act("You run your tongue along $p, poisoning it.",ch,obj,NULL,TO_CHAR);
    act("$n runs $s tongue along $p, poisoning it.",ch,obj,NULL,TO_ROOM);
    obj->value[0] = 53;
    obj->timer = number_range(10,20);

    return;
}

void do_regenerate( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->rage > 0)
    {
	send_to_char("You cannot control your regenerative powers while the beast is so strong.\n\r",ch);
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char("You cannot regenerate while fighting.\n\r",ch);
	return;
    }

    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
	send_to_char("But you are already completely regenerated!\n\r",ch);
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 5 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= number_range(10,20);
    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
    	send_to_char("Your body has completely regenerated.\n\r",ch);
    	act("$n's body completely regenerates itself.",ch,NULL,NULL,TO_ROOM);
    }
    else
    	send_to_char("Your body slowly regenerates itself.\n\r",ch);
    if (ch->hit < 1 )
    {
	ch->hit = ch->hit + 1;
	update_pos(ch);
	WAIT_STATE( ch, 24 );
    }
    else
    {
    	ch->hit = UMIN(ch->hit + 1500, ch->max_hit);
	WAIT_STATE(ch,8);
        update_pos(ch);
    }
    return;
}


void reg_mend( CHAR_DATA *ch )
{
    int ribs = 0;
    int teeth = 0;

    if (IS_BODY(ch,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(ch,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(ch,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(ch,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(ch,BROKEN_RIBS_16)) ribs += 16;
    if (IS_HEAD(ch,LOST_TOOTH_1  )) teeth += 1;
    if (IS_HEAD(ch,LOST_TOOTH_2  )) teeth += 2;
    if (IS_HEAD(ch,LOST_TOOTH_4  )) teeth += 4;
    if (IS_HEAD(ch,LOST_TOOTH_8  )) teeth += 8;
    if (IS_HEAD(ch,LOST_TOOTH_16 )) teeth += 16;

    if (ribs > 0)
    {
    	if (IS_BODY(ch,BROKEN_RIBS_1 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_1);
    	if (IS_BODY(ch,BROKEN_RIBS_2 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_2);
	if (IS_BODY(ch,BROKEN_RIBS_4 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_4);
	if (IS_BODY(ch,BROKEN_RIBS_8 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_8);
	if (IS_BODY(ch,BROKEN_RIBS_16))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_16);
	ribs -= 1;
	if (ribs >= 16) {ribs -= 16;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_16);}
	if (ribs >= 8 ) {ribs -= 8;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_8);}
	if (ribs >= 4 ) {ribs -= 4;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_4);}
	if (ribs >= 2 ) {ribs -= 2;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_2);}
	if (ribs >= 1 ) {ribs -= 1;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_1);}
	act("One of $n's ribs snap back into place.",ch,NULL,NULL,TO_ROOM);
	act("One of your ribs snap back into place.",ch,NULL,NULL,TO_CHAR);
    }
    else if (IS_HEAD(ch,LOST_EYE_L))
    {
	act("An eyeball appears in $n's left eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your left eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_L);
    }
    else if (IS_HEAD(ch,LOST_EYE_R))
    {
	act("An eyeball appears in $n's right eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your right eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_R);
    }
    else if (IS_HEAD(ch,LOST_EAR_L))
    {
	act("An ear grows on the left side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the left side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_L);
    }
    else if (IS_HEAD(ch,LOST_EAR_R))
    {
	act("An ear grows on the right side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the right side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_R);
    }
    else if (IS_HEAD(ch,LOST_NOSE))
    {
	act("A nose grows on the front of $n's face.",ch,NULL,NULL,TO_ROOM);
	act("A nose grows on the front of your face.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_NOSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (teeth > 0)
    {
    	if (IS_HEAD(ch,LOST_TOOTH_1 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);
    	if (IS_HEAD(ch,LOST_TOOTH_2 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);
    	if (IS_HEAD(ch,LOST_TOOTH_4 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);
    	if (IS_HEAD(ch,LOST_TOOTH_8 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);
    	if (IS_HEAD(ch,LOST_TOOTH_16))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);
	teeth -= 1;
	if (teeth >= 16) {teeth -= 16;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);}
	if (teeth >= 8 ) {teeth -= 8;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);}
	if (teeth >= 4 ) {teeth -= 4;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);}
	if (teeth >= 2 ) {teeth -= 2;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);}
	if (teeth >= 1 ) {teeth -= 1;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);}
	act("A missing tooth grows in your mouth.",ch,NULL,NULL,TO_CHAR);
	act("A missing tooth grows in $n's mouth.",ch,NULL,NULL,TO_ROOM);
    }
    else if (IS_HEAD(ch,BROKEN_NOSE))
    {
	act("$n's nose snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your nose snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (IS_HEAD(ch,BROKEN_JAW))
    {
	act("$n's jaw snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your jaw snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    else if (IS_HEAD(ch,BROKEN_SKULL))
    {
	act("$n's skull knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your skull knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_SKULL);
    }
    else if (IS_BODY(ch,BROKEN_SPINE))
    {
	act("$n's spine knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your spine knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_SPINE);
    }
    else if (IS_BODY(ch,BROKEN_NECK))
    {
	act("$n's neck snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your neck snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_NECK);
    }
    else if (IS_ARM_L(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's left shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your left shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
    }
    else if (IS_ARM_R(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's right shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your right shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
    }
    else if (IS_LEG_L(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's left hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your left hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's right hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your right hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,BROKEN_ARM))
    {
	act("$n's left arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
    }
    else if (IS_ARM_R(ch,BROKEN_ARM))
    {
	act("$n's right arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
    }
    else if (IS_LEG_L(ch,BROKEN_LEG))
    {
	act("$n's left leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
    }
    else if (IS_LEG_R(ch,BROKEN_LEG))
    {
	act("$n's right leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
    }
    else if (IS_ARM_L(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's left wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your left wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's right wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your right wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
    }
    else if (IS_LEG_L(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's left ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your left ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's right ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your right ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,BROKEN_THUMB))
    {
	act("$n's left thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_I))
    {
	act("$n's left index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_M))
    {
	act("$n's left middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_R))
    {
	act("$n's left ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_L))
    {
	act("$n's left little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,BROKEN_THUMB))
    {
	act("$n's right thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_I))
    {
	act("$n's right index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_M))
    {
	act("$n's right middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_R))
    {
	act("$n's right ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if ( IS_ARM_R(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if ( IS_ARM_R(ch,BROKEN_FINGER_L))
    {
	act("$n's right little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if (IS_BODY(ch,CUT_THROAT))
    {
    	if (IS_SET(ch->loc_hp[6], BLEEDING_THROAT)) return;
	act("The wound in $n's throat closes up.",ch,NULL,NULL,TO_ROOM);
	act("The wound in your throat closes up.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],CUT_THROAT);
    }
    return;
}

void vamp_rage( CHAR_DATA *ch )
{
    if (IS_NPC(ch)) return;

    send_to_char("You scream with rage as the beast within consumes you!\n\r",ch);
    act("$n screams with rage as $s inner beast consumes $m!.", ch, NULL, NULL, TO_ROOM);
    do_beastlike(ch,"");
    do_frenzy(ch,"");
    return;
}

void do_humanity( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC( ch )) return;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && ch->beast > 0
	&& ch->beast < 100 && number_range(1, 300) <= ch->beast)
    {
	if (ch->beast == 1)
	{
	    send_to_char("You have attained Golconda!\n\r", ch);
	    ch->exp += 1000000;
	    if (!IS_IMMUNE(ch, IMM_SUNLIGHT))
		SET_BIT(ch->immune, IMM_SUNLIGHT);
	}
	else
	    send_to_char("You feel slightly more in control of your beast.\n\r", ch);
	ch->beast -= 1;
    }
    return;
}

void do_beastlike( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];
    int blood;

    argument = one_argument( argument, arg );

    if (IS_NPC( ch )) return;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && ch->beast < 100
	&& ch->beast > 0 && number_range(1, 500) <= ch->beast)
    {
	if (ch->beast < 99)
	    send_to_char("You feel your beast take more control over your actions.\n\r", ch);
	else
	    send_to_char("Your beast has fully taken over control of your actions!\n\r", ch);
	ch->beast += 1;
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;
    	if ((IS_VAMPAFF(ch,VAM_PROTEAN) || (IS_VAMPAFF(ch,VAM_OBTENEBRATION)))
		&& !IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (!IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_PROTEAN) &&
		!IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	ch->pcdata->condition[COND_THIRST] = blood;
    }
    return;
}


void do_werewolf( CHAR_DATA *ch, char *argument )
{
    char       buf[MAX_INPUT_LENGTH];
    char       arg[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA  *obj;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF)) return;
    if (IS_SET(ch->special, SPC_WOLFMAN)) return;
    SET_BIT(ch->special, SPC_WOLFMAN);

    send_to_char("You throw back your head and howl with rage!\n\r",ch);
    act("$n throws back $s head and howls with rage!.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Coarse dark hair sprouts from your body.\n\r",ch);
    act("Coarse dark hair sprouts from $n's body.",ch,NULL,NULL,TO_ROOM);
    if (!IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("Your eyes start glowing red.\n\r",ch);
	act("$n's eyes start glowing red.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    if (!IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("A pair of long fangs extend from your mouth.\n\r",ch);
	act("A pair of long fangs extend from $n's mouth.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (!IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Razor sharp talons extend from your fingers.\n\r",ch);
	act("Razor sharp talons extend from $n's fingers.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    if ((obj = get_eq_char(ch,WEAR_WIELD)) != NULL && (!IS_SET(obj->spectype, SITEM_WOLFWEAPON) || obj->pIndexData->vnum ==
OBJ_VNUM_PROTOPLASM))
    {
	act("$p drops from your right hand.",ch,obj,NULL,TO_CHAR);
	act("$p drops from $n's right hand.",ch,obj,NULL,TO_ROOM);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    if ((obj = get_eq_char(ch,WEAR_HOLD)) != NULL && (!IS_SET(obj->spectype, SITEM_WOLFWEAPON) || obj->pIndexData->vnum ==
OBJ_VNUM_PROTOPLASM) )
    {
	act("$p drops from your left hand.",ch,obj,NULL,TO_CHAR);
	act("$p drops from $n's left hand.",ch,obj,NULL,TO_ROOM);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    xprintf(buf,"%s the werewolf", ch->name);
    free_string(ch->morph);
    ch->morph = str_dup(buf);
    ch->rage += 25;
    ch->hitroll += 50;
    ch->damroll += 50;
    if (ch->power[DISC_WERE_WOLF] > 3) ch->rage += 100;
    if (ch->rage > 300) 
	ch->rage = 300;

    ch->hit += 2000;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( ch == vch )
	{
	    act("You throw back your head and howl with rage!", ch, NULL, NULL, TO_CHAR);
	    continue;
	}
	if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
	    continue;
	if (!IS_NPC(vch))
	{
	    if (vch->in_room == ch->in_room)
		act("$n throws back $s head and howls with rage!", ch, NULL, vch, TO_VICT);
	    else if (vch->in_room->area == ch->in_room->area)
		send_to_char("You hear a fearsome howl close by!\n\r", vch);
	    else
	    if (!CAN_PK(vch)) continue;
	}
	if ( vch->in_room == ch->in_room && can_see( ch, vch) )
	{
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	    if (vch == NULL || vch->position <= POS_STUNNED) continue;
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	}
    }
    return;
}

void do_unwerewolf( CHAR_DATA *ch, char *argument )
{
    char       arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF)) return;
    if (!IS_SET(ch->special, SPC_WOLFMAN)) return;
    REMOVE_BIT(ch->special, SPC_WOLFMAN);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string(ch->morph);
    ch->morph = str_dup("");
    if (IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Your talons slide back into your fingers.\n\r",ch);
	act("$n's talons slide back into $s fingers.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    if (IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("Your fangs slide back into your mouth.\n\r",ch);
	act("$n's fangs slide back into $s mouth.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    send_to_char("Your coarse hair shrinks back into your body.\n\r",ch);
    act("$n's coarse hair shrinks back into $s body.",ch,NULL,NULL,TO_ROOM);
    ch->rage -= 25;
    ch->hitroll -= 50;
    ch->damroll -= 50;
    if (ch->rage < 0) ch->rage = 0;
    if (ch->hit > 2000) ch->hit -= 2000;
    return;
}
void do_possession( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_DOMI] < 3 && IS_CLASS(ch, CLASS_MAGE) )
    {
        send_to_char("You must obtain at least level 3 in Dominate to use Possession.\n\r",ch);
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL)
    {
	xprintf(buf,"You break your hold over %s.\n\r",familiar->short_descr);
	send_to_char( buf, ch );
	familiar->wizard = NULL;
	ch->pcdata->familiar = NULL;
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to Possess?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "What an intelligent idea!\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if (victim->wizard != NULL)
    {
	send_to_char( "You are unable to possess them.\n\r", ch );
	return;
    }

    if (victim->level > (ch->spl[RED_MAGIC] * 0.25))
    {
	send_to_char( "They are too powerful.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 50 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= 50;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    act("You concentrate on $N.",ch,NULL,victim,TO_CHAR);
    act("$n is staring at you!",ch,NULL,victim,TO_VICT);
    act("$n starts staring at $N",ch,NULL,victim,TO_NOTVICT);
    return;
}

void do_familiar( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Become your own familiar?\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
	familiar->wizard = NULL;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    send_to_char("Ok.\n\r",ch);

    return;
}

void do_fcommand( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && (ch->power[DISC_VAMP_DOMI] < 3) )
    {
	send_to_char("You are not trained in the Dominate discipline.\n\r",ch);
	return;
    }

    if ( ( victim = ch->pcdata->familiar ) == NULL )
    {
	send_to_char( "But you don't have a familiar!\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar do?\n\r", ch );
	return;
    }
    interpret(victim,argument);
    return;
}

void do_vanish( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
       !IS_CLASS(ch, CLASS_FAE) && !IS_CLASS(ch, CLASS_SHADOW))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_OWL] < 1)
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBFU] < 1)
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    
    if (IS_CLASS(ch, CLASS_SHADOW) && !IS_SET(ch->pcdata->powers[SHADOW_POWERS], NSHADOWS_HIDE))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    if (ch->fight_timer >0) {
      send_to_char("Not until your fight timer expires.\n\r", ch );
      return;
    }

    if (IS_CLASS(ch, CLASS_FAE) && ch->power[DISC_FAE_ARCANE] < 4)
    {
      send_to_char("You can't do that, yet.\n\r",ch);
      return;
    }

    if (ch->fight_timer >0) {
      send_to_char("Not until your fight timer expires.\n\r", ch );
      return;
    }

    if ( IS_AFFECTED(ch, AFF_HIDE) )
    {
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	send_to_char( "You slowly fade into existance.\n\r", ch );
	act("$n slowly fades into existance.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
	send_to_char( "You slowly fade out of existance.\n\r", ch );
	act("$n slowly fades out of existance.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->affected_by, AFF_HIDE);
    }
    return;
}

void do_flex( CHAR_DATA *ch, char *argument )
{
    act("You flex your bulging muscles.",ch,NULL,NULL,TO_CHAR);
    act("$n flexes $s bulging muscles.",ch,NULL,NULL,TO_ROOM);
    if (IS_NPC(ch)) return;

    if ( IS_EXTRA(ch, TIED_UP) )
    {
	act("The ropes restraining you snap.",ch,NULL,NULL,TO_CHAR);
	act("The ropes restraining $n snap.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->extra, TIED_UP);
    }
    if (is_affected(ch, gsn_web))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	affect_strip(ch, gsn_web);
    }
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_WEBBED);
    }
    if (IS_AFF2(ch, AFF2_THORNS))
    {
      act("The thorns and brambles holding $n breaks away.", ch, NULL, NULL, TO_ROOM);
      send_to_char("The brambles break away.\n\r", ch);
      REMOVE_BIT(ch->affected_by2, AFF2_THORNS);
      return;
    }

    WAIT_STATE(ch,12);
    return;
}

void do_rage( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

  if (IS_CLASS(ch, CLASS_WEREWOLF))
  {

    if (!IS_SET(ch->special, SPC_WOLFMAN))
    {
	send_to_char("You start snarling angrilly.\n\r",ch);
	act("$n starts snarling angrilly.",ch,NULL,NULL,TO_ROOM);
	ch->rage += number_range(40,60);
	if (ch->rage >= 100) do_werewolf(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    send_to_char("But you are already in a rage!\n\r",ch);
    }

    if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (ch->power[DISC_DAEM_ATTA] < 3)
        {
	    stc("You try, but cannot summon your beast.\n\r",ch);
	    return;
	}

     if (ch->beast < 1)
	ch->beast = 1;

        if (ch->beast > ch->rage || ch->rage < 125)
	{
	    ch->rage += number_range(ch->generation,25);
	    stc("You scream as you plunge into a mad attacking frenzy.\n\r",ch);
	    act("$n screams in frenzied rage.",ch,NULL,NULL,TO_ROOM);
	    if (number_percent() < (ch->beast + 1))
	    {
		stc("You feel the beast take more control.\n\r",ch);
		ch->beast += 1;
		if (ch->beast > 100)
		  ch->beast = 100;
	    }
	    WAIT_STATE(ch, 12);
	    return;
	}
       
        stc("You have reached the pinnacle of your frenzy.\n\r",ch);
    }

    return;
}

void do_calm( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) &&
        !IS_CLASS(ch, CLASS_DEMON) && 
        !IS_CLASS(ch, CLASS_NINJA))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

 if (IS_CLASS(ch, CLASS_WEREWOLF))
 {

    if ((ch->power[DISC_WERE_WOLF] < 3) && (IS_CLASS(ch, CLASS_WEREWOLF)))
    {
	stc("You need level three in the Wolf totem to Calm.\n\r",ch);
	return;
    }  

    if (IS_SET(ch->special, SPC_WOLFMAN))
    {
	send_to_char("You take a deep breath and calm yourself.\n\r",ch);
	act("$n takes a deep breath and tries to calm $mself.",ch,NULL,NULL,TO_ROOM);
	ch->rage -= number_range(60,90);
        if (ch->rage < 0) ch->rage = 0;
	if (ch->rage < 100) do_unwerewolf(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    send_to_char("But you are not in crinos form!\n\r",ch);
 }
if (IS_CLASS(ch, CLASS_NINJA))
   {
send_to_char("You take a deep breath and calm yourself.\n\r",ch);
        act("$n takes a deep breath and tries to calm $mself.",ch,NULL,NULL,TO_ROOM);
ch->rage = 0;
WAIT_STATE(ch,12);
        return;
    }
   if (IS_CLASS(ch, CLASS_DEMON))
   {

    if (ch->power[DISC_DAEM_ATTA] < 4)
    {
	stc("You need level four in Attack to calm your frenzy.\n\r",ch);
	return;
    }

    if (ch->beast >= 100) ch->beast = 100;
    if (ch->beast <= 0) ch->beast = 0;

    ch->rage -= (40 - (ch->beast / 3) +dice (1, 10));
    if (ch->rage <= 0)
    {
	ch->rage = 0;
	send_to_char("You have calmed out of your rage completely.\n\r", ch);
    }
    else
    {
      send_to_char("You hold your breath and try to calm your frenzy.\n\r", ch);
      WAIT_STATE(ch, 8);
    }
    return;
  } 
    return;
}

void do_totems( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch)) return;


    send_to_char("Please use the new commands, Disciplines and research.\n\r",ch);
	return;

}

void do_web( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_CYBORG)
     && !IS_CLASS(ch, CLASS_TANARRI) && !IS_CLASS(ch, CLASS_SAMURAI)
     && !IS_CLASS(ch, CLASS_PRIEST))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_SPID] < 2) 
	{send_to_char("You don't have that power yet.\n\r", ch );
	return;}

	if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->pcdata->powers[1],DPOWER_WEB))
	{send_to_char("You don't have that power yet.\n\r", ch );
	return;}

    if (IS_CLASS(ch, CLASS_TANARRI) && ch->pcdata->rank < 3)
    {
      send_to_char("You don't have that power yet.\n\r",ch);
      return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot web yourself.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
	send_to_char( "You cannot web an ethereal person.\n\r", ch );
	return;
    }

    if ( ( sn = skill_lookup( "web" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 0.25;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}

void do_mitsukeru( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powers[NPOWER_SORA] <1) {
	send_to_char("Huh?\n\r", ch );
	return;}
	if (arg[0] == '\0')
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
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
    if (!IS_NPC(victim)) ch->fight_timer += 5;
    return;
}

void do_koryou( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powers[NPOWER_SORA] <2) {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Read the aura on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg, ch) ) == NULL )
	{
	    send_to_char( "Read the aura on what?\n\r", ch );
	    return;
	}
	act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
	spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	return;
    }

    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
(IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("You are unable to read their aura.\n\r",ch);
	return;
    }

    act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you intently.",ch,NULL,victim,TO_VICT);
    if (IS_NPC(victim)) xprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
    else 
    {
	if      (victim->level == 12) xprintf(buf, "%s is an Implementor.\n\r", victim->name);
	else if (victim->level == 11) xprintf(buf, "%s is a High Judge.\n\r", victim->name);
	else if (victim->level == 10) xprintf(buf, "%s is a Judge.\n\r", victim->name);
	else if (victim->level == 9 ) xprintf(buf, "%s is an Enforcer.\n\r", victim->name);
	else if (victim->level == 8 ) xprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
	else if (victim->level == 7 ) xprintf(buf, "%s is a Builder.\n\r", victim->name);
	else if (victim->level >= 3 ) xprintf(buf, "%s is an Avatar.\n\r", victim->name);
	else xprintf(buf, "%s is a Mortal.\n\r", victim->name);
    }
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	xprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
	send_to_char(buf,ch);
    }
    xprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
    if (!IS_NPC(victim)) xprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
    else xprintf(buf,"AC:%d.\n\r",char_ac(victim));
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	xprintf(buf,"Status:%d, ",victim->race);
	send_to_char(buf,ch);
	if (IS_CLASS(victim, CLASS_VAMPIRE))    
	{
	    xprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
	    send_to_char(buf,ch);
	}
    }
    xprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("$N is pregnant.",ch,NULL,victim,TO_CHAR);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))    
    {
	send_to_char("Disciplines:",ch);
	if (IS_VAMPAFF(victim, VAM_PROTEAN)) send_to_char(" Protean",ch);
	if (IS_VAMPAFF(victim, VAM_CELERITY)) send_to_char(" Celerity",ch);
	if (IS_VAMPAFF(victim, VAM_FORTITUDE)) send_to_char(" Fortitude",ch);
	if (IS_VAMPAFF(victim, VAM_POTENCE)) send_to_char(" Potence",ch);
	if (IS_VAMPAFF(victim, VAM_OBFUSCATE)) send_to_char(" Obfuscate",ch);
	if (IS_VAMPAFF(victim, VAM_OBTENEBRATION)) send_to_char(" Obtenebration",ch);
	if (IS_VAMPAFF(victim, VAM_SERPENTIS)) send_to_char(" Serpentis",ch);
	if (IS_VAMPAFF(victim, VAM_AUSPEX)) send_to_char(" Auspex",ch);
	if (IS_VAMPAFF(victim, VAM_DOMINATE)) send_to_char(" Dominate",ch);
	if (IS_VAMPAFF(victim, VAM_PRESENCE)) send_to_char(" Presence",ch);
        if (IS_VAMPAFF(victim, VAM_VICISSITUDE)) send_to_char(" Vicissitude",ch);
        if (IS_VAMPAFF(victim, VAM_THAU)) send_to_char(" Thaumaturgy",ch);
        if (IS_VAMPAFF(victim, VAM_ANIMAL)) send_to_char(" Animalism",ch);
        if (IS_VAMPAFF(victim, VAM_QUIETUS)) send_to_char(" Quietus",ch);
        send_to_char(".\n\r",ch);
    }
    return;
}



