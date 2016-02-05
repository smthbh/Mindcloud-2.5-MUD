/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
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

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h" 

/**** Local Functions ****/
CHAR_DATA *get_quest_target 	args( ( int min_level, int max_level ) );
CHAR_DATA *get_quest_giver	args( ( int min_level, int max_level ) );
OBJ_DATA  *load_aquest_object    args( ( CHAR_DATA *target ) );
void	   clear_quest		args( ( void ) );

extern int quest_timer;
extern bool quest;
extern CHAR_DATA *quest_mob;
extern CHAR_DATA *quest_target;
extern OBJ_DATA *aquest_object;
extern int quest_wait;
extern sh_int quest_personality;
extern bool auto_quest;


/* 17 messages, organised by blocks for each personality 
   indented messages are for when the target mob gets killed  */
struct	qmessage_type
{
	char * const message1;
	char * const message2;
};


 const struct qmessage_type  qmessages[5][17] =

 {
   {
      {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""},
	{"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""},
	{"", ""}
   },
 {     
	{ "Oh my! My %s has been stolen from me, and I am too young to find it!",	 	"" },
	{ "Would someone please search for my %s? I'm sure that it will be much too hard for me to find.", 							"" },
	{ "What will I do without my %s?",								"" },
	{ "Can't anybody find my %s for me?",  							"" },
	{ "Help me! My %s is still missing!", 							"" },
	{ "Keep searching for my %s, and i'll ask my mom if she knows who stole it!",	"" },
	{ "Does anyone know who stole my %s?", 							"" },
	{ "My mom says %s stole my %s. I know it is too hard for me to get back. Oh my, what will I do?",
		"Yeay!! It looks like whoever stole my %s is now dead!! Thank you very much!" 					},
	{ "Please, can you go kill %s? I need my %s!! I can't sleep without it!!",
		"Well, thank you for killing the nasty thief, but can you please return my %s to me?" 				},
	{ "My mom says %s stole my %s. I know it is too hard for me to get back. Oh my, what will I do?",
		"Yeay!! It looks like whoever stole my %s is now dead!! Thank you very much!" 					},
	{ "It's time for my nap now, and %s still has my %s!! Can anyone please get it back for me?",
		"Please, time is running out! Return my %s to me!" 				},
	{ "%s is a real meanie for stealing my %s! Can you pretty pretty pretty please get it back for me?",
		"Please, time is running out! Return my %s to me!" 				},
	{ "It's time for my nap now, and %s still has my %s!! Can anyone please get it back for me?",
		"Please, time is running out! Return my %s to me!" 				},
	{ "Oh my, I'll never get up to watch cartoons tomorrow now!  %s is still holding my %s ransom, and I need it for my nap!",
		"How can I sleep without my %s?"					},
	{ "I give up! %s can keep my %s for all I care! I didn't want to take a nap, anyway!", 
		"I give up! I never want to see my %s again!"					},
	{ "Mommy, can I watch cartoons now, instead of taking a nap?? PLEASE??" , 				"" },
	{ "Oh THANK YOU, %s!! Now that I have my %s again, I can go sleepy sleep!",	"" }

    },


   {
	{ "Hmm, I seem to have lost my %s. Oh well, didn't really need it much, anyway.", 	"" },
	{ "I wonder where I could find another %s? Anyone have any ideas?", 							"" },
	{ "Where can my %s have gone?", 									"" },
	{ "I guess no one really cares, anyway, but I think I might need a %s later.",  							"" },
	{ "I guess I should try and find my %s, but I really don't feel like it.", 						"" },
	{ "If anyone has an extra %s, I might be willing to reward them for it.", 			"" },
	{ "Doesn't anyone out there know where to find a %s?", 					"" },
	{ "Hmm, maybe %s knew something I didn't, and thought it was a good idea to steal my %s. Maybe he could use it, I know I can't.",
		"I guess my %s didn't help him much, since he is now dead!  I do miss it though." 					},
	{ "Hmm, maybe it IS worth something.  Could someone go kill %s and get my %s back for me?",
		"I guess my %s didn't help him much, since he is now dead!  I do miss it though." 					},
	{ "I would pay a lot if someone would kill %s and get my %s back. I don't really know where it went.",
		"Even though it's not worth very much, my %s is kind of important to me. Oh, well, guess I will never see it again." 				},
	{ "Hmm, maybe it IS worth something.  Could someone go kill %s and get my %s back for me?",
		"I guess my %s didn't help him much, since he is now dead!  I do miss it though." 					},
	{ "I would pay a lot if someone would kill %s and get my %s back. I don't really know where it went.",
		"Even though it's not worth very much, my %s is kind of important to me. Oh, well, guess I will never see it again." 				},
	{ "Oh well, since no one will help me, I guess %s can keep my %s.",
		"It must be cursed, since everyone who has it is dead. I don't think I want my %s after all!" 					},
	{ "Oh well, since no one will help me, I guess %s can keep my %s.",
		"It must be cursed, since everyone who has it is dead. I don't think I want my %s after all!" 					},
	{ "I give up! %s can keep my %s for all I care!", 
		"I give up! I never want to see my %s again!"					},
	{ "Well, I will stop asking now, but don't ever ask ME for any favors, ok?" , 				"" },
	{ "Well, looks like %s has recovered my %s for me. Not sure I want it anymore, but thanks anyway.", 						"" }

    },

{
	{ "Alright, what the heck, I wake up from my nap and my %s is gone!", 	"" },
	{ "Come on you freaks, find my %s for me!",					"" },
	{ "Where can my %s have gone?", 									"" },
	{ "Get off your asses and find my %s!",  							"" },
	{ "Help me! My %s has not yet been recovered!", 						"" },
	{ "Keep searching for my %s, and i'll find out who stole it!", 			"" },
	{ "Were there no witnesses to the theft of my %s?", 					"" },
	{ "It was %s who stole my %s from me!  Someone help me!",
		"It looks like whoever stole my %s is now dead!!" 					},
	{ "Please, time is running out! Kill %s and recover my %s for me NOW!",
		"Please, time is running out! Return my %s to me!" 				},
	{ "Please, time is running out! Kill %s and recover my %s for me NOW!",
		"Please, time is running out! Return my %s to me!" 				},
	{ "Please, time is running out! Kill %s and recover my %s for me NOW!",
		"Please, time is running out! Return my %s to me!" 				},
	{ "Please, time is running out! Kill %s and recover my %s for me NOW!",
		"Please, time is running out! Return my %s to me!" 				},
	{ "I give up! %s can keep my %s for all I care!",
		"I give up! I never want to see my %s again!" 					},
	{ "I give up! %s can keep my %s for all I care!",
		"I give up! I never want to see my %s again!"					},
	{ "I give up! %s can keep my %s for all I care!", 
		"I give up! I never want to see my %s again!"					},
	{ "Shoot! Just forget about recovering ANYTHING for me, ok?" , 				"" },
	{ "At Last! %s has recovered %s for me!", 						"" }

    },  

{
	{ "Muuaahhhaaahaaaa! Some puny mortal has stolen my %s!  I shall seek revenge!!", 	"" },
	{ "I shall send many minions to seek my %s! All that steal from me shall die!!", 							"" },
	{ "SO, you have defeated my servants.  I shall still regain my %s!!", 									"" },
	{ "I am prepared to reward well anyone that aids the return of my %s. Are any of you puny mortals willing to attempt my challenge?",  							"" },
	{ "If you are worthy, I will grant many favors upon anyone that returns my %s.", 						"" },
	{ "Methlok, By the dark powers, I command you to seek my %s! Now, if any of you worthless mortals wish to attempt to return it, I shall grant you many powers!", 			"" },
	{ "I sense that Methlok is nearing the witless thief who stole my %s. Now, my vengence shall be sweet!", 					"" },
	{ "Ahhh, my servant has returned, and informs me that %s stole my %s. They shall be incinerated by the powers that I command!!!",
		"Methlok has informed me that the weakling that stole my %s has met his maker!!" 					},
	{ "Are none of you powerful enough to kill %s and regain my %s? Bah!! Mortals are useless, except as side dishes!!",
		"Though my taste for blood has been satiated, my %s still evades my grasp!" 				},
	{ "Are none of you powerful enough to kill %s  and regain my %s ? Bah!! Mortals are useless, except as side dishes!!",
		"Though my taste for blood has been satiated, my %s still evades my grasp!" 				},
	{ "Are none of you powerful enough to kill %s  and regain my %s ? Bah!! Mortals are useless, except as side dishes!!",
		"Though my taste for blood has been satiated, my %s still evades my grasp!" 				},
	{ "I should have known that a powerless, puny mortal could never be the servant of my vengence against %s, or regain my %s!!",
		"I shall rain death upon all of you for refusing to return my %s!!!" 				},
	{ "I should have known that a powerless, puny mortal could never be the servant of my vengence against %s, or regain my %s!!",
		"I shall rain death upon all of you for refusing to return my %s!!!" 				},
	{ "I shall slay your brothers and poison your fields for refusing to seek %s and return my %s!!!!",
		"Though my vengeance has been served, I shall drink your souls for your failure to return my %s!!!"					},
	{ "I shall slay your brothers and poison your fields for refusing to seek %s and return my %s!!!!",
		"Though my vengeance has been served, I shall drink your souls for your failure to return my %s!!!"					},
	{ "Death and great suffering shall be your punishment for failing me!!!?" , 				"" },
	{ "Well done.  It seems that %s at least has a modicum of strength, unlike you worthless idiots who failed to return my %s! My curse shall lie upon you for the rest of your short days!", "" }

    }




  };


void do_mobquest( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char new_long_desc[MAX_STRING_LENGTH];
   
   if ( argument[0] == '\0' )	/* Display status */
   {
      if ( !quest )
      {
         send_to_char( "There is no quest currently running.\n\r", ch );
         if ( auto_quest )
            send_to_char( "Quests are currently running automatically.\n\r", ch );
         if ( quest_wait > 0 )
         {
            xprintf( buf, "The next quest may occur in %d minutes.\n\r", quest_wait );
            send_to_char( buf, ch );
         }
         return;
      }
      else
         send_to_char( "There is currently a quest running ", ch );
      
      if ( auto_quest )
         send_to_char( "(Automatically)", ch );
      
      send_to_char( "\n\rQuest Details:\n\r\n\r", ch );
      if ( quest_mob )
      {

        xprintf( buf, "The questing mobile is: %s [In Room %d]\n\r", 
           quest_mob->short_descr, quest_mob->in_room->vnum );
        send_to_char( buf, ch );
      }
      else
      {
        send_to_char( "The questing mobile is dead!\n\r", ch );
      }
      if ( quest_target )
      {
        xprintf( buf, "Target Mobile is: %s [In Room %d]\n\r",
           quest_target->short_descr, quest_target->in_room->vnum );
        send_to_char( buf, ch );
      }
      else
        send_to_char( "The target mobile is dead!\n\r", ch );

      xprintf( buf, "Target Object is: %s.\n\r", aquest_object->short_descr );
      send_to_char( buf, ch );
      
      xprintf( buf, "The Quest has been running for %d/15 minutes.\n\r", quest_timer );
      send_to_char( buf, ch );
      
      return;
   }
   if ( !strcmp( argument, "stop" ) )
   {
     if ( quest )
     {
       xprintf_2( buf, "The quest has been stopped by an Immortal. Please speak up if you have already gotten the item.\n\r" );
       do_qtalk( ch, buf );
       clear_quest();
     }
     return;
   }

   if ( !strcmp( argument, "start" ) )
   {
      
      /* generate a new quest! */
      if ( quest )
      {
         send_to_char( "There is already a quest running...\n\r", ch );
         return;
      }
      
      if ( auto_quest )
      {
         auto_quest = FALSE;
         send_to_char( "Automatic Quests now OFF.\n\r", ch );
      }
      
      quest_mob    = get_quest_giver(1, 2500);
      if ( quest_mob == NULL )
      {
         send_to_char( "Failed to find a quest mob\n\r", ch );
         return;
      }
      quest_personality = number_range( 1, 4 );
      quest_target = get_quest_target( 1, 400 );
      if (  ( quest_target == NULL )
         || ( quest_target == quest_mob )  )
      {
         send_to_char( "Failed to find a quest target\n\r", ch );
         return;
      }
      aquest_object = load_aquest_object( quest_target );
      if ( aquest_object == NULL )
      {
         send_to_char( "An invalid quest object was encountered.  Check log files.\n\r", ch );
         quest = FALSE;
         return;
      }
      
      quest_timer = 0;
      quest = TRUE;
      new_long_desc[0] = '\0';
      if ( quest_mob->long_descr_orig != NULL )
        free_string( quest_mob->long_descr_orig );
      quest_mob->long_descr_orig = str_dup(quest_mob->long_descr );
      xprintf( new_long_desc, "%s says have you found my %s ?\n\r",
         quest_mob->short_descr, aquest_object->short_descr );
      if ( quest_mob->long_descr != NULL )      
        free_string( quest_mob->long_descr );    
      quest_mob->long_descr = str_dup( new_long_desc );
        SET_BIT( quest_mob->act, ACT_NOTRAVEL );
  		SET_BIT( quest_mob->act, ACT_NOSUMMON );

      new_long_desc[0] = '\0';
      if ( quest_target->long_descr_orig != NULL )
        free_string( quest_target->long_descr_orig );
      quest_target->long_descr_orig = str_dup( quest_target->long_descr );
      xprintf( new_long_desc, "%s says I stole the %s !!!\n\r",
        quest_target->short_descr, aquest_object->short_descr );
      if ( quest_target->long_descr != NULL )     
        free_string( quest_target->long_descr );   
      quest_target->long_descr = str_dup( new_long_desc );
  		SET_BIT( quest_mob->act, ACT_NOTRAVEL );
  		SET_BIT( quest_mob->act, ACT_NOSUMMON );

      send_to_char( "QUEST STARTED!\n\r\n\r", ch );
            
      xprintf( buf, "The questing mobile is: %s [In Room %d]\n\r", 
         quest_mob->short_descr, quest_mob->in_room->vnum );
      send_to_char( buf, ch );
      
      xprintf( buf, "Target Mobile is: %s [In Room %d]\n\r",
         quest_target->short_descr, quest_target->in_room->vnum );
      send_to_char( buf, ch );
      
      xprintf( buf, "Target Object is: %s.\n\r", aquest_object->short_descr );
      send_to_char( buf, ch );
      
      xprintf( buf, "Mob Personality is: %d.\n\r", quest_personality );
      send_to_char( buf, ch );
     
      return;
   }
   if ( !str_cmp( argument, "auto" ) )
   {
      send_to_char( "AutoQuest now initiated!\n\r", ch );
      auto_quest = TRUE;
      return;
   }
   
   
   return;
}      
      
            

/*
 * get_quest_target : This attempts to pick a random mobile to hold the quest
 * item for the player (questor).  Various checks are made to ensure that the
 * questor has a chance of killing the mobile, etc.
 * Returns NULL if it didn't get a mobile this time.
 */

CHAR_DATA * get_quest_target( int min_level, int max_level )
{
   CHAR_DATA *target;
   int min_index = 0;  /* the minimum number of times to go through the list */

   min_index = number_range(1, 1000);

   for ( target = char_list; target != NULL; target = target->next )
   {
      if ( !IS_NPC( target ) )
         continue;
      min_index -= 1;

      if ( min_index > 0 )
         continue;

      
     if (    ( target->level < min_level                 	  )
          || ( target->level > max_level                 	  )
          || ( IS_SET( target->act, ACT_PET ) ) )
             continue;
       
     /* Lastly, some random choice */
     if ( number_percent() < 2 )
        break;
   } 
   
   return target;
}       

/*
 * load_aquest_object : This takes a pointer to OBJ_INDEX_DATA and places the
 * object onto the target.
 */

OBJ_DATA * load_aquest_object( CHAR_DATA *target )
{
   OBJ_INDEX_DATA *pObj;
   OBJ_DATA *object;
   int foo;
   
   foo = number_range( OBJ_VNUM_QUEST_MIN, OBJ_VNUM_QUEST_MAX );
   
   pObj = get_obj_index( foo );
   
   if ( pObj == NULL )
   {
      bug( "load_aquest_object : Invalid object vnum %d.", foo );
      return NULL;
   }
      
   object = create_object( pObj, 1 );
   SET_BIT(object->quest, QUEST_MQUEST);
   obj_to_char( object, target );
   
   return object;
}
   
CHAR_DATA * get_quest_giver( int min_level, int max_level )
{
   CHAR_DATA *target;
   int    min_index = 0;

   min_index = number_range(0, 1000);

   for ( target = char_list; target != NULL; target = target->next )
   {
      if ( !IS_NPC( target ) )
         continue;
      min_index -= 1;

      if ( min_index > 0 )
        continue;
      
     if (    ( target->level < min_level                 	  )
          || ( target->level > max_level                 	  )
          || ( IS_SET( target->act, ACT_PET ) ) )

             continue;
     {
       if ( number_percent() < 2 )
        break;
     }
      
   }  

   return target;
}       

/*
 * quest_inform : Makes the questing mobile give out information to the
 * players on the mud.  Starts off real simple, and gets more helpful as
 * time runs out :P
 */

void quest_inform( void )
{
   char buf[MAX_STRING_LENGTH];

   /* Work out what the mob should tell the players.... */
   /* Add random element to each case so quests look different each time? */
   if ( quest_timer < 7 )
   {
    xprintf( buf, qmessages[quest_personality][quest_timer].message1, aquest_object->short_descr );
   }
   else
   { 
   if (quest_target)
	xprintf ( buf, qmessages[quest_personality][quest_timer].message1,
			 quest_target->short_descr,aquest_object->short_descr );
   else
      xprintf( buf, qmessages[quest_personality][quest_timer].message2,
		 aquest_object->short_descr );
   }



   quest_timer++;
   if ( quest_mob && quest_timer < 16 )
        do_qtalk( quest_mob, buf );
   if ( quest_timer == 1 )
   {
     xprintf( buf, " %s is questing for %s ", NAME( quest_mob ), aquest_object->short_descr);
     do_info( quest_mob, buf );
   }
   if ( !quest_mob )
   {
     clear_quest();
   }    
   return;
}

void quest_complete( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];

   xprintf( buf, qmessages[quest_personality][16].message1, NAME(ch), aquest_object->short_descr );
   do_qtalk( quest_mob, buf );
   clear_quest();
   return;
}   
   

void quest_cancel()
{
    if ( quest_mob )
       do_qtalk( quest_mob, "Shoot! Just forget about recovering ANYTHING for me, ok?" );
    
    clear_quest();
    return;   
}   

void clear_quest()
{
   /* Clear ALL values, ready for next quest */
   
   quest = FALSE;
   extract_obj ( aquest_object );
   if ( quest_mob )
   {
     free_string( quest_mob->long_descr );
     quest_mob->long_descr =  str_dup( quest_mob->long_descr_orig );
     free_string( quest_mob->long_descr_orig );
     quest_mob->long_descr_orig = NULL;
   }
   if ( quest_target )
   {
     free_string( quest_target->long_descr );
     quest_target->long_descr =  str_dup( quest_target->long_descr_orig );
     free_string( quest_target->long_descr_orig );
     quest_target->long_descr_orig = NULL;
   };


   quest_mob = NULL;
   quest_target = NULL;
   aquest_object = NULL;
   quest_timer = 0;
   quest_wait = 2 + number_range( 1, 4);
   quest_personality = 0;
   
   return;
}
   
   
void generate_auto_quest()
{

  char new_long_desc[MAX_STRING_LENGTH];
  sh_int loop_counter = 0; 
      
  /* generate a new quest! */
  if ( quest )
  {
    return;
  }

  quest_mob = NULL;
  quest_target = NULL;

  quest_personality = number_range( 1, 4 );
 
  while (  ( quest_mob == NULL )
        && ( loop_counter < 500 )  )
  {
    loop_counter++;
    quest_mob    = get_quest_giver(1, 2500);
  }

  if ( quest_mob == NULL )
  {
    quest = FALSE;
    quest_wait = number_range( 1, 3 );
    return;
  }
  loop_counter = 0;
  while (  ( quest_target == NULL )
        && ( loop_counter < 500 )  )
  {
    loop_counter++;
    quest_target = get_quest_target( 40, 400 );
  }

  if ( quest_target == NULL )
  {
    quest = FALSE;
    quest_wait = number_range( 1, 3 );
    return;
  }
  
  aquest_object = load_aquest_object( quest_target );
  
  if ( aquest_object == NULL )
  {
    quest = FALSE;
    quest_wait = number_range( 1, 3 );
    return;
  }
      
  quest_timer = 0;
  quest = TRUE;
  new_long_desc[0] = '\0';
  if ( quest_mob->long_descr_orig != NULL )
    free_string( quest_mob->long_descr_orig );
  quest_mob->long_descr_orig = str_dup(quest_mob->long_descr );
  xprintf( new_long_desc, "%s says have you found my %s ?\n\r",
         quest_mob->short_descr, aquest_object->short_descr );
  if ( quest_mob->long_descr != NULL )      
    free_string( quest_mob->long_descr );    
  quest_mob->long_descr = str_dup( new_long_desc );
  SET_BIT( quest_mob->act, ACT_NOTRAVEL );
  SET_BIT( quest_mob->act, ACT_NOSUMMON );

  new_long_desc[0] = '\0';
  if ( quest_target->long_descr_orig != NULL )
    free_string( quest_target->long_descr_orig );
  quest_target->long_descr_orig = str_dup( quest_target->long_descr );
  xprintf( new_long_desc, "%s says I stole the %s !!!\n\r",
        quest_target->short_descr, aquest_object->short_descr );
  if ( quest_target->long_descr != NULL )     
        free_string( quest_target->long_descr );   
  quest_target->long_descr = str_dup( new_long_desc );
  SET_BIT( quest_mob->act, ACT_NOTRAVEL );
  SET_BIT( quest_mob->act, ACT_NOSUMMON );
  return;
}

void ask_quest_question( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
    buf[0] = '\0';

  if ( !quest || IS_NPC( ch ) )
   return;

  if (  ( !str_cmp( argument, "who is the thief?")		)
     || ( !str_cmp( argument, "who was the thief?" )		)
     || ( !str_cmp( argument, "who stole it?" )			)
     || ( !str_cmp( argument, "what mob?" )			)
     || ( !str_cmp( argument, "who stole the item?" )		)  )
  {
    if ( quest_mob )
    {
      if ( quest_timer < 7 )
      {
        xprintf( buf, "I don't even know who stole it yet!" );
      }
      else
      if ( aquest_object && quest_target )
      {
        xprintf( buf, "It was %s who stole my %s.",
         quest_target->short_descr, aquest_object->short_descr );
      }
    }
    else
    if ( aquest_object )
    {
      xprintf( buf, "Don't worry about who stole my %s, they have recieved their just reward!",
        aquest_object->short_descr );
    }
    if ( quest_mob != NULL )
      do_qtalk( quest_mob, buf );
    return;
  }

  if ( !str_cmp( argument, "what item?" ) )
  {
    if (quest_mob && aquest_object )
    {
      xprintf(buf, "My %s was stolen from me, find it!.", aquest_object->short_descr );
      do_qtalk( quest_mob, buf );
      return;
    }
  }

  if ( !str_cmp( argument, "where are you?" ) )
    if ( quest_mob)
    {
      xprintf( buf, "You can find me in %s, please hurry!!", quest_mob->in_room->area->name);
      do_qtalk( quest_mob, buf );
      return;
    }

  if  ( !str_cmp( argument, "where is the thief?" ) )
  {
    if ( quest_mob )
    {
      if ( ( quest_target ) && ( quest_timer > 7 ) )
      {  
        if ( quest_timer < 10 )
        {
          xprintf( buf, "I don't really know where %s is, let me try and find out.", 
            quest_target->short_descr );
        }
        else
        if ( quest_target )
        {
          xprintf( buf, "I'm not really sure, but I THINK %s is in %s", 
           quest_target->short_descr, quest_target->in_room->area->name);
        }
        
      }
      else if ( ( quest_target ) && ( quest_timer <= 7 )  )
      {
        xprintf( buf, "I don't even know who stole it yet!" );
      }
      else
      {
        xprintf( buf, "Don't worry about where the thief who stole my %s is, they have recieved their just reward",
          aquest_object->short_descr );
      }
      do_qtalk( quest_mob, buf );
    }
  }
  return;
}
