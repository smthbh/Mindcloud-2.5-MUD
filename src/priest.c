/******************************************************************************
 Class/Snippet -> Priest
 Creator -> Tracker (xxcuthbertxx@home.com)
 Date -> September 09, 2000
 ------------------------------------------------------------------------------
 This code is copyright (C) 2000 by Tracker. It may be altered and used freely,
 but may not be publicly distributed by anyone that does not have permission from
 Tracker.

 If you use this code you need to do the following:

 * You are to put "Priest class coded by Tracker" in your 'help changes'
 * You are not to remove the last line of the helpfile in priest.are
   (The line about 'This class has been coded by Tracker')
 * Please e-mail me if you are using this code, so I can see how many people
   like it.
 * You are -not- to re-do the class and claim it's yours. I worked very hard
   on creating this class.

 * Thanks for using my class! - Tracker
 ******************************************************************************/
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


struct priest_type
{
    const char * name;
    const int    bit;
    const int    cost;
    char * msg;
};


void do_pgain( CHAR_DATA *ch, char *argument)
{
    char     arg1 [MAX_INPUT_LENGTH];
    int      gift = 0;
    int      cost = 0;
    int      p;
    bool     flag;
    
const struct priest_type priest_tab [ ] =
{
     { "truesight", PRI_SIGHT, 0,
     "#w|%s Truesight    #0|%s Free #0|%s Truesight     #0|%s Truesight            #w|#n\n\r"
     },
     { "minheal", PRI_MIHE, 6000,
     "#w|%s Minheal      #0|%s 6000 #0|%s Heals         #0|%s Minor healing        #w|#n\n\r"
     },
     { "majorheal", PRI_MAHE, 3000,
     "#w|%s Majorheal    #0|%s 3000 #0|%s Heals x2      #0|%s Major Healing        #w|#n\n\r"
     },
     { "grheal", PRI_GRHE, 3000,
     "#w|%s Grheal       #0|%s 3000 #0|%s Heals x3      #0|%s Greater Healing      #w|#n\n\r"
     },
     { "healstorm", PRI_STRM, 5000,
     "#w|%s HealStorm    #0|%s 5000 #0|%s Heals group   #0|%s Healing Storm        #w|#n\n\r"
     },
     { "minstorm", PRI_MSTM, 5000,
     "#w|%s Minstorm     #0|%s 5000 #0|%s Heal group x2 #0|%s Healing Storm x2     #w|#n\n\r"
     },
     { "grstorm", PRI_GSTM, 5000,
     "#w|%s Grstorm      #0|%s 5000 #0|%s Heal group x3 #0|%s Healing Storm x3     #w|#n\n\r"
     },
     { "godbless", PRI_GBLS, 5500,
     "#w|%s Godbless     #0|%s 5500 #0|%s Blesses       #0|%s Greater Bless        #w|#n\n\r"
     },
     { "godstrike", PRI_GODSTRIKE, 5500,
     "#w|%s Godstrike    #0|%s 5500 #0|%s Damage skill  #0|%s Godstrike            #w|#n\n\r"
     },
     { "godheal", PRI_GDHE, 5000,
     "#w|%s Godheal      #0|%s 5000 #0|%s Healing x4    #0|%s Gods Heal            #w|#n\n\r"
     },
     { "turnundead", PRI_TURN, 5200,
     "#w|%s Turnundead   #0|%s 5200 #0|%s Like Zombie   #0|%s Turn Undead          #w|#n\n\r"
     },
     { "holyaura", PRI_HAUR, 5000,
     "#w|%s Holyaura     #0|%s 5000 #0|%s Sanct x2      #0|%s Holy Aura            #w|#n\n\r"
     },
     { "godpeace", PRI_GPEC, 9000,
     "#w|%s Godpeace     #0|%s 9000 #0|%s Stops fights  #0|%s Godly Peace          #w|#n\n\r"
     },
     { "scry", PRI_SCRY, 5000,
     "#w|%s Scry         #0|%s 5000 #0|%s Scry          #0|%s Scry                 #w|#n\n\r"
     },
     { "read", PRI_READ, 5000,
     "#w|%s Readaura     #0|%s 5000 #0|%s Readaura      #0|%s Readaura             #w|#n\n\r"
     },
     { "shield", PRI_SHIELD, 5000,
     "#w|%s Shield       #0|%s 5000 #0|%s Shield        #0|%s Shield               #w|#n\n\r"
     },
     { "priestform", PRI_PFORM, 5300,
     "#w|%s Priestform   #0|%s 5300 #0|%s Gives stats   #0|%s Priest form          #w|#n\n\r"
     },
     { NULL, 0, 0, NULL }
};

    smash_tilde(argument);
     argument = one_argument( argument, arg1 );

     if (IS_NPC(ch)) return;

     if (!IS_CLASS(ch, CLASS_PRIEST))
     {
       send_to_char("Huh?\n\r",ch);
       return;
     }

     if ( arg1[0] == '\0' )
     {
         char * col;
         col = "#w";
         stc("#C*#w---------------------#C*#n\n\r",ch);
         stc("#w| #WSyntax: pgain <s/p> #w|#n\n\r",ch);
         stc("#C*#w---------------------#C*#n\n\r",ch);
         stc("\n\r",ch);
         stc("#C*#w--------------#C*#w------#C*#w---------------#C*#w----------------------#C*#n\n\r",ch);
         stc("#w|#W Spell/Skill  #w|#W Cost #w|#W What it does  #w|#W Long name            #w|#n\n\r",ch);
         stc("#C*#w--------------#C*#w------#C*#w---------------#C*#w----------------------#C*#n\n\r",ch);
         stc("#w|#w              #0|#w      #0|#w               #0|#w                      #w|#n\n\r",ch);
         stc("#C*#w--------------#C*#w------#C*#w---------------#C*#w----------------------#C*#n\n\r",ch);
         for( p = 0; priest_tab[p].name; p++ )
             ch_printf( ch, priest_tab[p].msg,
                        IS_PRIPOWER(ch,priest_tab[p].bit) ? "#W" : "#0",
                        IS_PRIPOWER(ch,priest_tab[p].bit) ? "#W" : "#0",
                        IS_PRIPOWER(ch,priest_tab[p].bit) ? "#W" : "#0",
                        IS_PRIPOWER(ch,priest_tab[p].bit) ? "#W" : "#0" );
         stc( "#w|              #0|      |               |                      #w|#n\n\r", ch );
         stc( "#C*#w--------------#C*#w------#C*#w---------------#C*#w----------------------#C*#n\n\r", ch );
         return;
     }

     flag = FALSE;
     for( p = 0; priest_tab[p].name; p++ )
         if( !str_cmp( priest_tab[p].name, arg1 ) )
         {
             flag = TRUE;
             gift = priest_tab[p].bit;
             cost = priest_tab[p].cost;
         }

     if( !flag )
     {
         do_pgain( ch, "" );
         return;
     }

     if (IS_PRIPOWER(ch, gift))
     {
       send_to_char("You already have that!\n\r",ch);
       return;
     }

     if ( ch->pcdata->stats[FAITH_PTS] < cost)
     {
       send_to_char("You need more cps to train that.\n\r",ch);
       return;
     }
     SET_BIT( ch->pcdata->powers[PRIPOWER_FLAGS], gift);
     ch->pcdata->stats[FAITH_PTS] -= cost;
     send_to_char("Ok.\n\r",ch);
     save_char_obj(ch);
     return;
}

void do_minheal(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    int heal,mcost;

    argument = one_argument(argument, arg);
    heal = number_range(1200,1600);
    mcost = number_range(1,500);

    if (IS_NPC(ch)) return;
 
     if (!IS_CLASS(ch, CLASS_PRIEST))
    {
    send_to_char("Huh?\n\r", ch);
    return;
    }

     if (!IS_PRIPOWER( ch, PRI_MIHE))
     {
       send_to_char("You haven't mastered minor healing yet!\n\r",ch);
       return;
     }

    if (!IS_NPC(ch) && ch->position == POS_STUNNED)
    {
    send_to_char("Not while you are mortally wounded!.\n\r", ch);
    return;
    }

    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Cast Minor Healing on whom?\n\r", ch);
    return;
    }
    
    if ((victim = get_char_room(ch, NULL, arg)) == NULL)
    {
    send_to_char("They are not here.\n\r", ch);
    return;
    }   

    if ((victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] +
     victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
     victim->loc_hp[6]) == 0 && victim->hit == victim->max_hit)
    {
    send_to_char("They don't require any healing.\n\r",ch);
    return;
    }
    
    act("$n's eyes start to glow #wbright white#n.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Your eyes start to glow #wbright white#n.\n\r", ch);
    if (victim->hit < victim->max_hit)
    {
    victim->hit += heal;
    send_to_char("You have been healed!\n\r",victim);
    act("$n has been healed!",victim,NULL,NULL,TO_ROOM);
    }
    
    WAIT_STATE(ch,12);
    ch->mana -= mcost;
    return;
}

void do_majorheal(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    int heal,mcost;

    argument = one_argument(argument, arg);
    heal = number_range(1700,2100);
    mcost = number_range(500,1000);

    if (IS_NPC(ch)) return;
 
     if (!IS_CLASS(ch, CLASS_PRIEST))
    {
    send_to_char("Huh?\n\r", ch);
    return;
    }

     if (!IS_PRIPOWER( ch, PRI_MIHE))
     {
       send_to_char("You need to gain minor healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MAHE))
     {
       send_to_char("You haven't mastered major healing yet!\n\r",ch);
       return;
     }

    if (!IS_NPC(ch) && ch->position == POS_STUNNED)
    {
    send_to_char("Not while you are mortally wounded!.\n\r", ch);
    return;
    }

/*
    if (ch->position == POS_FIGHTING)
    {
    send_to_char("No way!  You are still fighting!\n\r",ch);
    return;
    }
*/    
    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Cast Major Healing on whom?\n\r", ch);
    return;
    }
    
    if ((victim = get_char_room(ch, NULL, arg)) == NULL)
    {
    send_to_char("They are not here.\n\r", ch);
    return;
    }   

    if ((victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] +
     victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
     victim->loc_hp[6]) == 0 && victim->hit == victim->max_hit)
    {
    send_to_char("They don't require any healing.\n\r",ch);
    return;
    }
    
    act("$n's eyes start to glow #wbright white#n.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Your eyes start to glow #wbright white#n.\n\r", ch);
    if (victim->hit < victim->max_hit)
    {
    victim->hit += heal;
    send_to_char("You feel better!\n\r",victim);
    act("$n has been healed!",victim,NULL,NULL,TO_ROOM);
    }
    
    WAIT_STATE(ch,12);
    ch->mana -= mcost;
    return;
}

void do_grheal(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    int heal,mcost;

    argument = one_argument(argument, arg);
    heal = number_range(5600,7000);
    mcost = number_range(1200,3000);

    if (IS_NPC(ch)) return;
 
     if (!IS_CLASS(ch, CLASS_PRIEST))
    {
    send_to_char("Huh?\n\r", ch);
    return;
    }

     if (!IS_PRIPOWER( ch, PRI_MIHE))
     {
       send_to_char("You need to gain minor healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MAHE))
     {
       send_to_char("You need to gain major healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GRHE))
     {
       send_to_char("You haven't mastered major healing yet!\n\r",ch);
       return;
     }

    if (!IS_NPC(ch) && ch->position == POS_STUNNED)
    {
    send_to_char("Not while you are mortally wounded!.\n\r", ch);
    return;
    }

/*
    if (ch->position == POS_FIGHTING)
    {
    send_to_char("No way!  You are still fighting!\n\r",ch);
    return;
    }
*/    
    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Cast Greater Healing on whom?\n\r", ch);
    return;
    }
    
    if ((victim = get_char_room(ch, NULL, arg)) == NULL)
    {
    send_to_char("They are not here.\n\r", ch);
    return;
    }   

    if ((victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] +
     victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
     victim->loc_hp[6]) == 0 && victim->hit == victim->max_hit)
    {
    send_to_char("They don't require any healing.\n\r",ch);
    return;
    }
    
    act("$n's eyes start to glow #wbright white#n.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Your eyes start to glow #wbright white#n.\n\r", ch);
    if (victim->hit < victim->max_hit)
    {
    victim->hit += heal;
    send_to_char("You feel MUCH better!\n\r",victim);
    act("$n has been healed!",victim,NULL,NULL,TO_ROOM);
    }
    
    WAIT_STATE(ch,12);
    ch->mana -= mcost;
    return;
}

void do_godheal(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    int heal,mcost;

    argument = one_argument(argument, arg);
    heal = number_range(12000,20000);
    mcost = number_range(12000,20000);

    if (IS_NPC(ch)) return;
 
     if (!IS_CLASS(ch, CLASS_PRIEST))
    {
    send_to_char("Huh?\n\r", ch);
    return;
    }

     if (!IS_PRIPOWER( ch, PRI_MIHE))
     {
       send_to_char("You need to gain minor healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MAHE))
     {
       send_to_char("You need to gain major healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GRHE))
     {
       send_to_char("You need to gain greater healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_STRM))
     {
       send_to_char("You need to gain healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MSTM))
     {
       send_to_char("You need to master major healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GSTM))
     {
       send_to_char("You need to master greater healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GDHE))
     {
       send_to_char("You haven't mastered godsheal yet!\n\r",ch);
       return;
     }

    if (!IS_NPC(ch) && ch->position == POS_STUNNED)
    {
    send_to_char("Not while you are mortally wounded!.\n\r", ch);
    return;
    }

/*
    if (ch->position == POS_FIGHTING)
    {
    send_to_char("No way!  You are still fighting!\n\r",ch);
    return;
    }
*/    
    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Cast Greater Healing on whom?\n\r", ch);
    return;
    }
    
    if ((victim = get_char_room(ch, NULL, arg)) == NULL)
    {
    send_to_char("They are not here.\n\r", ch);
    return;
    }   

    if ((victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] +
     victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
     victim->loc_hp[6]) == 0 && victim->hit == victim->max_hit)
    {
    send_to_char("They don't require any healing.\n\r",ch);
    return;
    }
    
    act("$n's eyes start to glow #wbright white#n.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Your eyes start to glow #wbright white#n.\n\r", ch);
    if (victim->hit < victim->max_hit)
    {
    victim->hit += heal;
    send_to_char("You feel -*MUCH*- better!\n\r",victim);
    act("$n has been healed!",victim,NULL,NULL,TO_ROOM);
    }
    
    WAIT_STATE(ch,12);
    ch->mana -= mcost;
    return;
}


void do_healstorm( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int heal,mcost;

    argument = one_argument(argument, arg);
    heal = number_range(3100,3800);
    mcost = number_range(1200,3000);

    if (IS_NPC(ch)) return;
 
     if (!IS_CLASS(ch, CLASS_PRIEST))
    {
    send_to_char("Huh?\n\r", ch);
    return;
    }

     if (!IS_PRIPOWER( ch, PRI_MIHE))
     {
       send_to_char("You need to gain minor healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MAHE))
     {
       send_to_char("You need to gain major healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GRHE))
     {
       send_to_char("You need to gain greater healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_STRM))
     {
       send_to_char("You haven't mastered healing storm yet!\n\r",ch);
       return;
     }

    if (!IS_NPC(ch) && ch->position == POS_STUNNED)
    {
    send_to_char("Not while you are mortally wounded!.\n\r", ch);
    return;
    }

    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

    act("$n's eyes start to glow #wbright white#n.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Your eyes start to glow #wbright white#n.\n\r", ch);
                
    if ( arg[0] == '\0' )
    {
        CHAR_DATA *gch;
             
        for ( gch = char_list; gch != NULL; gch = gch->next )
        {
            if ( is_same_group( gch, ch ) )
            {  
               gch->hit += heal;
               send_to_char("You are healed by the storm!\n\r",gch);
            }
        }
    }

    WAIT_STATE(ch,12);
    ch->mana -= mcost;
    return;
}

void do_minstorm( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int heal,mcost;

    argument = one_argument(argument, arg);
    heal = number_range(4000,4600);
    mcost = number_range(1600,4000);

    if (IS_NPC(ch)) return;
 
     if (!IS_CLASS(ch, CLASS_PRIEST))
    {
    send_to_char("Huh?\n\r", ch);
    return;
    }

     if (!IS_PRIPOWER( ch, PRI_MIHE))
     {
       send_to_char("You need to gain minor healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MAHE))
     {
       send_to_char("You need to gain major healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GRHE))
     {
       send_to_char("You need to gain greater healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_STRM))
     {
       send_to_char("You need to gain healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_STRM))
     {
       send_to_char("You haven't mastered major healing storm yet!\n\r",ch);
       return;
     }

    if (!IS_NPC(ch) && ch->position == POS_STUNNED)
    {
    send_to_char("Not while you are mortally wounded!.\n\r", ch);
    return;
    }

    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

    act("$n's eyes start to glow #wbright white#n.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Your eyes start to glow #wbright white#n.\n\r", ch);
                
    if ( arg[0] == '\0' )
    {
        CHAR_DATA *gch;
             
        for ( gch = char_list; gch != NULL; gch = gch->next )
        {
            if ( is_same_group( gch, ch ) )
            {  
               gch->hit += heal;
               send_to_char("You are healed by the storm!\n\r",gch);
            }
        }
    }

    WAIT_STATE(ch,12);
    ch->mana -= mcost;
    return;
}

void do_grstorm( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int heal,mcost;

    argument = one_argument(argument, arg);
    heal = number_range(5000,6000);
    mcost = number_range(3600,10000);

    if (IS_NPC(ch)) return;
 
     if (!IS_CLASS(ch, CLASS_PRIEST))
    {
    send_to_char("Huh?\n\r", ch);
    return;
    }

     if (!IS_PRIPOWER( ch, PRI_MIHE))
     {
       send_to_char("You need to gain minor healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MAHE))
     {
       send_to_char("You need to gain major healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GRHE))
     {
       send_to_char("You need to gain greater healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_STRM))
     {
       send_to_char("You need to gain healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MSTM))
     {
       send_to_char("You need to gain major healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GSTM))
     {
       send_to_char("You haven't mastered greater healing storm yet!\n\r",ch);
       return;
     }

    if (!IS_NPC(ch) && ch->position == POS_STUNNED)
    {
    send_to_char("Not while you are mortally wounded!.\n\r", ch);
    return;
    }

    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

    act("$n's eyes start to glow #wbright white#n.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Your eyes start to glow #wbright white#n.\n\r", ch);
                
    if ( arg[0] == '\0' )
    {
        CHAR_DATA *gch;
             
        for ( gch = char_list; gch != NULL; gch = gch->next )
        {
            if ( is_same_group( gch, ch ) )
            {  
               gch->hit += heal;
               send_to_char("You are healed by the storm!\n\r",gch);
            }
        }
    }

    WAIT_STATE(ch,25);
    ch->mana -= mcost;
    return;
}

void do_godbless( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char      arg [MAX_INPUT_LENGTH];
  int sn,level;
  int mcost;
    
    argument = one_argument(argument, arg);
    mcost = number_range(2000,3000);

  if ( !IS_CLASS(ch, CLASS_PRIEST) ) return;
        

     if (!IS_PRIPOWER( ch, PRI_GBLS))
     {
       send_to_char("You haven't mastered greater bless yet!\n\r",ch);
       return;
     }

    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

  if ( ( sn = skill_lookup( "greater bless" ) ) < 0 ) return;

    if (arg[0] == '\0')
    {
    send_to_char("Cast Greater Bless on whom?\n\r", ch);
    return;
    }
    
    if ((victim = get_char_room(ch, NULL, arg)) == NULL)
    {
    send_to_char("They are not here.\n\r", ch);
    return;
    }   


  if ( is_affected(victim, sn) )
  {
    stc("They're already affected by greater bless!\n\r",ch);  
    return;
  }
 
  stc("You godbless them!\n\r",ch);
  act("#nA bright #wwhite#n beam of light surrounds $n, as $n is filled with a greater bless.",ch,NULL,NULL,TO_ROOM);
  level = 50;
  (*skill_table[sn].spell_fun) ( sn, level, victim, victim );
  WAIT_STATE( ch, 12 );
  ch->mana -= mcost;
  return;  
}


void do_godstrike( CHAR_DATA *ch, char *argument )
{
           
        char buf[MAX_STRING_LENGTH];
        CHAR_DATA *victim;  
        int atkdam;        
        int mcost;
        
        mcost = number_range(5000,10000);

    if (!IS_CLASS(ch, CLASS_PRIEST))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

        if (!IS_PRIPOWER( ch, PRI_GODSTRIKE))
        {
            send_to_char("You haven't learned that skill.\n\r",ch);
            return;
        }  
        
     if (!IS_PRIPOWER( ch, PRI_MIHE))
     {
       send_to_char("You need to gain minor healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MAHE))
     {
       send_to_char("You need to gain major healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GRHE))
     {
       send_to_char("You need to gain greater healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_STRM))
     {
       send_to_char("You need to gain healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MSTM))
     {
       send_to_char("You need to gain major healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GSTM))
     {
       send_to_char("You need to gain greater healing strom to use this!\n\r",ch);
       return;
     }

        if ( argument[0] == '\0' )
        {
         stc("Whom do you wish to godstrike?\n\r",ch);
         return;
        }
         
        if ( !( victim = get_char_room( ch, NULL, argument )))   
        {
         stc("They are not here.\n\r",ch);
         return;
        }
        if (victim == ch)
        {
         stc("I doubt that would be wise.\n\r",ch);
         return;
        }
         
    if ( is_safe( ch, victim ))
    {
         stc("Not in a safe room.\n\r",ch);
         return;
    }
    
    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }
         
    if ( ch->position != POS_FIGHTING )
    {
         atkdam = number_range(8000,15000);
        if (!IS_NPC(victim))
           atkdam /= 2;
         xprintf( buf, "Your GodStrike #w-#w-#0-#b*#B*#0*#w(#s MUTILATES #w)#0*#B*#b*#0-#w-#w-#n %s. #B[#w%d#B]#n\n\r",
                 victim->name,
                 atkdam );
         stc( buf, ch );
         xprintf( buf, "%s's GodStrike #w-#w-#0-#b*#B*#0*#w(#s MUTILATES #w)#0*#B*#b*#0-#w-#w-#n you! #B[#w%d#B]#n\n\r",
                 ch->name,
                 atkdam );
         stc( buf, victim );
	 hurt_person( ch, victim, atkdam );
         xprintf( buf, "%s", victim->name );
         do_kill( ch, buf );
         WAIT_STATE( ch, 12 );
         return;
    }
    else
    {
         atkdam = number_range(5000,10000);
         xprintf( buf, "Your GodStrike #w-#w-#0-#b*#B*#0*#w(#s MUTILATES #w)#0*#B*#b*#0-#w-#w-#n %s. #B[#w%d#B]#n\n\r",
                 victim->name,
                 atkdam );
         stc( buf, ch );
         xprintf( buf, "%s's GodStrike #w-#w-#0-#b*#B*#0*#w(#s MUTILATES #w)#0*#B*#b*#0-#w-#w-#n you! #B[#w%d#B]#n\n\r",
                 ch->name,
                 atkdam );
	 hurt_person( ch, victim, atkdam );
         WAIT_STATE( ch, 18 );
       }
       ch->mana -= mcost;
         return;
}
void do_ascend( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    
    argument = one_argument( argument, arg );
    
    if (IS_NPC(ch)) return;
     
    if (!IS_CLASS(ch, CLASS_PRIEST))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    
  if ( ch->move < 3000)
  {
    stc("You don't have enough movement points!\n\r",ch);
    return;
  }

    /*
    if (!str_cmp( arg, "heaven" ) )
    {
        send_to_char("You spread your wings and begin to fly away.\n\r",ch);
        act("$n spreads its wings and begins to fly away.\n\r",ch,NULL,NULL,TO_ROOM);
        char_from_room(ch);
        char_to_room(ch, get_room_index(ROOM_VNUM_HEAVEN));
        do_look(ch,"");
        send_to_char("You slowly float to the ground after a long trip.\n\r",ch);
        act("$n slowly floats to the ground after a long trip.\n\r",ch,NULL,NULL,TO_ROOM);
        return;
    }
    */
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "Nobody by that name.\n\r", ch );
        return;
    }
        
    if (IS_IMMUNE(victim, IMM_TRAVEL))
    {
        send_to_char("I don't think they want you to do that.\n\r",ch);
        return;
    }
      if (IS_SET(victim->act, ACT_NOTRAVEL))
      {
  	send_to_char("No Can Do.\n\r", ch);
  	return;
      }
    if (IS_IMMUNE(victim, IMM_SUMMON))
    {
        send_to_char("I don't think they want you to do that.\n\r",ch);
        return;
    }
    
    if ( victim == ch ) {
                send_to_char( "Nothing happens.\n\r", ch);
                return; }
    
    if (victim->in_room == NULL)
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }
     
    send_to_char("You utter some holy words vanish.\n\r",ch);
    act("$n utters holy words and vanishes.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    send_to_char("You appear at your location and raise your head.\n\r",ch);
    act("You hear chanting and see $n appear.",ch,NULL,NULL,TO_ROOM);
    ch->move -= 3000;
    return;
}    
void do_turnundead(CHAR_DATA *ch, char *argument )
{
    char buf2[MAX_STRING_LENGTH];
    char buf [MAX_INPUT_LENGTH];
    char arg [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    OBJ_DATA *obj;
    one_argument(argument,arg);
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_PRIEST))
     {
     send_to_char("Huh?\n\r",ch);
     return;
     }

     if (!IS_PRIPOWER( ch, PRI_TURN))
     {
       send_to_char("You need to gain turn undead to use this!\n\r",ch);
       return;
     }

    if (arg[0] == '\0')
    {
    send_to_char("Turn undead what corpse?\n\r",ch);
    return;
    }

    if (ch->pcdata->followers > 5)
    {
        send_to_char("Nothing happens.\n\r",ch);
        return;
    }

    if ((obj = get_obj_carry(ch,arg, ch)) == NULL)
    {
    send_to_char("You dont have that corpse.",ch);
    return;
    }
    if (obj->item_type != ITEM_CORPSE_NPC || IS_SET(obj->quest, QUEST_ZOMBIE))
    { 
    send_to_char("You can only Turn undead original corpses.\n\r",ch);
    return;  
    } 

    
    ch->pcdata->followers++;
        
        
    victim=create_mobile( get_mob_index( obj->value[2] ) );
    xprintf(buf,"Undead %s",victim->name);
    xprintf(buf2,"(Undead) %s is here.\n\r",victim->short_descr);
    free_string(victim->short_descr);
    victim->short_descr = str_dup(buf);
    free_string(victim->name);
    victim->name = str_dup(buf);
    free_string(victim->long_descr);
    victim->long_descr= str_dup(buf2);
    SET_BIT(victim->extra, EXTRA_ZOMBIE);
        victim->spec_fun = NULL;
    strcpy(buf,"Untha Hokagoo Mance!");
    do_say( ch, buf );
    xprintf(buf, "%s clambers back up to its feet.\n\r",obj->short_descr );
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char(buf,ch);
    
    
    char_to_room( victim, ch->in_room );
    
 if (victim->level < 100)
{
    add_follower( victim, ch ); 
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;   
    affect_to_char( victim, &af );
}
WAIT_STATE(ch,10);
extract_obj(obj);
    return;
}   

void do_holyaura( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char      arg [MAX_INPUT_LENGTH];
  int sn,level;
  int mcost;
    
    argument = one_argument(argument, arg);
    mcost = number_range(2000,3000);

  if ( !IS_CLASS(ch, CLASS_PRIEST) ) return;
        

     if (!IS_PRIPOWER( ch, PRI_HAUR))
     {
       send_to_char("You haven't mastered holy aura yet!\n\r",ch);
       return;
     }

    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

  if ( ( sn = skill_lookup( "holy aura" ) ) < 0 ) return;

    if (arg[0] == '\0')
    {
    send_to_char("Cast Holy Aura on whom?\n\r", ch);
    return;
    }
    
    if ((victim = get_char_room(ch, NULL, arg)) == NULL)
    {
    send_to_char("They are not here.\n\r", ch);
    return;
    }   


  if ( is_affected(victim,skill_lookup("holy aura")) )
  {
    stc("They're already affected by holy aura!\n\r",ch);  
    return;
  }
 
  stc("You cast Holy Aura on them!\n\r",ch);
  act("$n starts glowing bright #wwhite#n.",ch,NULL,NULL,TO_ROOM);
  level = 50;
  (*skill_table[sn].spell_fun) ( sn, level, victim, victim );
  WAIT_STATE( ch, 12 );
  ch->mana -= mcost;
  return;  
}

void do_godpeace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
    int mcost;

     mcost = number_range(5000,10000);

  if ( !IS_CLASS(ch, CLASS_PRIEST) ) return;

     if (!IS_PRIPOWER( ch, PRI_MIHE))
     {
       send_to_char("You need to gain minor healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MAHE))
     {
       send_to_char("You need to gain major healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GRHE))
     {
       send_to_char("You need to gain greater healing to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_STRM))
     {
       send_to_char("You need to gain healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_MSTM))
     {
       send_to_char("You need to gain major healing storm to use this!\n\r",ch);
       return;
     }

     if (!IS_PRIPOWER( ch, PRI_GSTM))
     {
       send_to_char("You need to gain greater healing storm to use this!\n\r",ch);
       return;
     }
           
     if (!IS_PRIPOWER( ch, PRI_GPEC))
     {
       send_to_char("You haven't mastered godly peace yet!\n\r",ch);
       return;
     }
     
    if (ch->mana < mcost)
    {
       send_to_char("You don't have enough mana for that!\n\r",ch);
       return;
    }

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
        if ( rch->fighting != NULL )
            stop_fighting( rch, TRUE );
    }
 
    send_to_char( "Ok.\n\r", ch );
    act("$n starts chanting some forein language.\n\rAll fighting in the room comes to a stop!",ch,NULL,NULL,TO_ROOM);
    ch->mana -= mcost;
    return;
}

void do_pwho( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
        
    one_argument( argument, arg );
        
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_PRIEST))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
 
    xprintf( buf, "Fellow Priests online:\n\r");
    send_to_char( buf, ch );
    send_to_char("#0[#C      Name      #0] [#C Hits #0] [#C Mana #0] [#C Move #0]#n\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
        if ( IS_NPC(gch) ) continue;
        if ( !IS_CLASS(gch, CLASS_PRIEST))
            continue;
        if ( !IS_NPC(gch))
        {
            xprintf( buf,
            "#0[#w%-16s#0] [#w%-6d#0] [#w%-6d#0] [#w%-6d#0]\n\r",
                capitalize( gch->name ),      
                gch->hit,
                gch->mana,
                gch->move);
                send_to_char( buf, ch );
        }
    }
    return;
}

void do_priestform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );
        
    if (IS_NPC(ch)) return;
        
  if (!IS_CLASS(ch, CLASS_PRIEST))
        {send_to_char("Huh?\n\r", ch );
        return;}

     if (!IS_PRIPOWER( ch, PRI_PFORM))
     {
       send_to_char("You haven't mastered priest form yet!\n\r",ch);
       return;
     }
                  
    if (IS_POLYAFF(ch, POLY_PFORM))
    {
        REMOVE_BIT(ch->polyaff, POLY_PFORM);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM );
        ch->damroll -= 500;
        ch->hitroll -= 500;
        ch->armor   += 500;
        clear_stats(ch);
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return; 
    }   
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
send_to_char("You cant priest form when changed.\n\r",ch);
return;
}
        if (ch->stance[0] != -1) do_stance(ch,"");
        if (ch->mounted == IS_RIDING) do_dismount(ch,"");
        act( "You transform into a huge priest.", ch, NULL, NULL, TO_CHAR );      
        act( "$n's body grows and distorts into a huge priest.", ch, NULL, NULL, TO_ROOM );
 
        SET_BIT(ch->polyaff, POLY_PFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        xprintf(buf, "#0(#C-#C{#W%s#C}#C-#0)#w the holy priest", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->damroll += 500;
        ch->hitroll += 500;
        ch->armor   -= 500;
        return;
    }


