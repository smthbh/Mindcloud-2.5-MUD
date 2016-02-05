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
 *  Dystopian Questcode copyright (C) 2001 by Brian Graversen, users must  *
 *  follow the DIKU, Merc and Godwars license as well as the license       *
 *  distributed with Dystopia                                              *
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

int  get_rand_mob              args (( void ));
int  get_rand_hard_mob         args (( void ));
int  get_rand_item             args (( void ));
int  load_special_item         args (( CHAR_DATA *ch ));
int  get_lbound                args (( void ));
int  get_ubound                args (( int bound ));
void give_token                args (( CHAR_DATA *questmaster, CHAR_DATA *ch, int value ));

QUEST_DATA *quest_free;

DECLARE_QUEST_FUN( questspec_generic		);
DECLARE_QUEST_FUN( questspec_newbie		);
DECLARE_QUEST_FUN( questspec_special_item	);
DECLARE_QUEST_FUN( questspec_rand_mob		);
DECLARE_QUEST_FUN( questspec_mob_and_item	);
DECLARE_QUEST_FUN( questspec_hard_mob		);
DECLARE_QUEST_FUN( questspec_mass_kill		);
DECLARE_QUEST_FUN( questspec_pk                 );

const struct quest_type quest_table [] =
{
  { "questspec_generic",         questspec_generic          },
  { "questspec_special_item",    questspec_special_item     },
  { "questspec_rand_mob",        questspec_rand_mob         },
  { "questspec_mob_and_item",    questspec_mob_and_item     },
  { "questspec_hard_mob",        questspec_hard_mob         },
  { "questspec_mass_kill",       questspec_mass_kill        },
  { "questspec_pk",              questspec_pk               },
  { "questspec_newbie",          questspec_newbie           },

  /* end of table */
  { "", 0 }
};

/*
 * Used for show_char_to_char_0()
 */
bool is_quest_target(CHAR_DATA *ch, CHAR_DATA *victim)
{
  QUEST_DATA *quest;

  if (IS_NPC(ch) || !IS_NPC(victim)) return FALSE;

  for (quest = ch->pcdata->quests; quest; quest = quest->next)
  {
    switch(quest->type)
    {
      case QT_MOB:
        if (victim->pIndexData->vnum == quest->vnums[0])
          return TRUE;
        break;
      case QT_MOB_AND_OBJ:
        if (victim->pIndexData->vnum == quest->vnums[0])
          return TRUE;
        break;
      case QT_MASS_KILL:
        if (victim->pIndexData->vnum >= quest->vnums[0] &&
            victim->pIndexData->vnum <= quest->vnums[1])
          return TRUE;
        break;
      default: continue;
    }
  }
  return FALSE;
}

/* 
 * This is a temp function for creating the database of quest
 * monsters, that needs to be slain. Please don't run it
 * while the mud is operationel - it takes quite a while
 * to execute.
 */
void do_createbase(CHAR_DATA *ch, char *argument)
{
  AREA_DATA *area;
  DESCRIPTOR_DATA *d;
  ROOM_INDEX_DATA *pRoom;
  MOB_INDEX_DATA *pMobIndex;
  RESET_DATA *pReset;
  FILE *fp;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int vnum, vnum2, iMin, iMax, col = 0;
  bool found = FALSE;

  if ((d = ch->desc) == NULL)
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  
  one_argument(argument, arg);

  /* put in what ever levels you want here */
  if (!str_cmp(arg, "big"))
  {
    iMin = 800;
    iMax = 2600;
  }
  else if (!str_cmp(arg, "small"))
  {
    iMin = 10;
    iMax = 400;
  }
  else
  {
    send_to_char("Big or Small moblist ??\n\r", ch);
    return;
  }

  if ((fp = fopen("../txt/questlist.txt", "w")) == NULL)
  {
    send_to_char("Unable to create questlist.txt - terminating attempt\n\r", ch);
    return;
  }

  for (area = area_first; area && !found; area = area->next)
  {
    if (area == area_first) write_to_descriptor(d, "\n\r", 0);
    xprintf(buf, "Processing %s....", area->name);
    write_to_descriptor(d, buf, 0);

    fprintf(fp, "/* %s */\n", area->name);
    found = FALSE; // reset
    for (vnum = area->lvnum; vnum <= area->uvnum; vnum++)
    {
      if ((pMobIndex = get_mob_index(vnum)) != NULL)
      {
        if (pMobIndex->level < iMin || pMobIndex->level > iMax) continue;

        for (vnum2 = area->lvnum; vnum2 <= area->uvnum; vnum2++)
        {
          if ((pRoom = get_room_index(vnum2)) != NULL)
          {
            for (pReset = pRoom->reset_first; pReset; pReset = pReset->next)
            {
              switch (pReset->command)
              {
                default: break;
                case 'M':
                  if ((pMobIndex = get_mob_index(pReset->arg1)) != NULL)
                  {
                    if (pMobIndex->vnum == vnum) found = TRUE;
                  }
              }
            }
          }
        }
        if (found)
        {
          found = FALSE; // reset for later use
          fprintf(fp, " %6d,", vnum);
          if (!(++col % 6)) fprintf(fp, "\n");
        }
      }
    }
    if (col % 6) fprintf(fp, "\n");
    if (area->next) write_to_descriptor(d, "Ok!\n\r", 0);
    else write_to_descriptor(d, "Ok!", 0);
  }
  fclose(fp);
  return;
}

int get_lbound()
{
  int vnum, count = 0;

  static const int mob_table[] = {
  14003, 31108, 1105, 1122, 1602, 1702, 1710, 2007,
  2101, 2201, 2307, 2315, 2801, 2904, 3406, 3601, 4150,
  4700, 5000, 5006, 6303, 6507, 6601, 7200, 7801,
  8000, 8308, 8904, 9203, 9401, 11006, 7705, 93000, 80001,

  /* END */
    -1
  };

  /* counting the size of the table */
  for (count = 0; mob_table[count] != -1; count++) ;

  vnum = number_range(0, count-1);
  return mob_table[vnum];
}

int get_ubound(int bound)
{
  int vnum;

  switch(bound)
  {
    case 14003 : vnum = 14011; break;
    case 31108 : vnum = 31118; break;
    case 1105 : vnum = 1109; break;
    case 1122 : vnum = 1125; break;
    case 1602 : vnum = 1605; break;
    case 1702 : vnum = 1708; break;
    case 1710 : vnum = 1716; break;
    case 2007 : vnum = 2013; break;
    case 2101 : vnum = 2108; break;
    case 2201 : vnum = 2205; break;
    case 2307 : vnum = 2311; break;
    case 2315 : vnum = 2321; break;
    case 2801 : vnum = 2808; break;
    case 2904 : vnum = 2913; break;
    case 3406 : vnum = 3408; break;
    case 3601 : vnum = 3604; break;
    case 4150 : vnum = 4154; break;
    case 4700 : vnum = 4708; break;
    case 5000 : vnum = 5003; break;
    case 5006 : vnum = 5008; break;
    case 2612 : vnum = 2616; break;
    case 6303 : vnum = 6310; break;
    case 6507 : vnum = 6510; break;
    case 6601 : vnum = 6606; break;
    case 7200 : vnum = 7202; break;
    case 7801 : vnum = 7808; break;
    case 8000 : vnum = 8005; break;
    case 8308 : vnum = 8312; break;
    case 8904 : vnum = 8908; break;
    case 9203 : vnum = 9207; break;
    case 9401 : vnum = 9407; break;
    case 11006 : vnum = 11008; break;
    case 7705 : vnum = 7709; break;
    case 93000 : vnum = 93004; break;
    case 80001 : vnum = 80005; break;
    default:
      vnum = bound;
      bug("Bad Quest Vnum : %d", bound);
      break;
  }
  return vnum;
}

/*
 * Returns the vnum of a random object that actually pops on the mud.
 * It's a bit ugly, but it works :)
 */
int get_rand_item()
{
  OBJ_INDEX_DATA *pObjIndex;
  ROOM_INDEX_DATA *pRoom;
  RESET_DATA *pReset;
  AREA_DATA *area;
  int vnum, i;
  bool found = FALSE;

  while (!found)
  {
    vnum = number_range(101, 90000);
    if ((pObjIndex = get_obj_index(vnum)) != NULL)
    {
      if (!CAN_WEAR(pObjIndex, ITEM_TAKE)) continue;
      if (pObjIndex->item_type == ITEM_MONEY) continue;
      if ((area = pObjIndex->area) != NULL)
      {
        for (i = area->lvnum; i <= area->uvnum; i++)
        {
          if ((pRoom = get_room_index(i)) != NULL)
          {
            for (pReset = pRoom->reset_first; pReset; pReset = pReset->next)
            {
              switch (pReset->command)
              {
                default: break;
                case 'O':
                  if ((pObjIndex = get_obj_index(pReset->arg1)) != NULL)
                  {
                    if (pObjIndex->vnum == vnum) found = TRUE;
                  }
              }
            }
          }
        }
      }
    }
  }
  return vnum;
}

int load_special_item(CHAR_DATA *ch)
{
  OBJ_DATA *obj;
  OBJ_INDEX_DATA *pObjIndex;
  int vnum;

  vnum = number_range(99501, 99505);
  if ((pObjIndex = get_obj_index(vnum)) == NULL)
  {
    bug("Load_special_item : %d", vnum);
    return 103;  // Just to be on the safe side
  }
  obj = create_object(pObjIndex, 50);
  obj->ownerid = ch->pcdata->playerid;
  SET_BIT(obj->extra_flags, ITEM_NOLOCATE);
  obj->timer = 25; // so it will eventually decay...
  obj_to_room(obj, get_rand_room());
  return vnum;
}

int get_rand_mob()
{
  int vnum, count = 0;

  static const int questmob_table[] = {
   /* KaVir   Odds and ends */
    105,    600,    902,    903,    904,    905,
    907,    908,    912,    913,    914,    915,
    916,    917,    918,    922,    924,    926,
   1000,   1341,   1347,   1348,   1349,   1512,
   2015,   2108,   2301,   2323,   2325,   2410,
   2433,   2436,   2437,   2446,   2452,   2453,
   2454,   2456,   2458,   2460,   2462,   2463,
   2464,   2468,   2469,   2470,   2605,   2608,
   2611,   2612,   2613,   2614,   2615,   2617,
   2803,   2804,   2906,   3009,   3603,   4100,
   4101,   4707,   5008,   5010,   5100,   5111,
   5315,   6311,   6600,   6601,   6602,   6603,
   6604,   6605,   6606,   6607,   6608,   7008,
   7013,   7703,   8000,   8101,   8102,   8105,
   8120,   8123,   8600,   9208,   9233,   9322,
  11001,  11004,  11005,  11006,  11007,  11008,
  11009,  11010,  11011,  11101,  11104,  11115,
  11118,  11121,  11124,  11125,  11126,  11127,
  11133,  11136,  11137,  11138,  11139,  14004,
  14005,  14006,  14008,  14009,  14010,  14011,
  14012,  19101,  19103,  19108,  25000,  25002,
  25003,  25007,
/* Anon    DragonSpyre */
  14004,  14005,  14006,  14008,
  14009,  14010,  14011,  14012,
/* Wynfair Castle Dragonheart */
  19101,  19103,
  19108,
/* Xara    Sunyata */
  31053,  31056,  31057,  31059,  31060,
  31061,  31064,  31065,  31066,  31067,  31068,
/* Xara    Easy Evil Ones */
  11101,  11104,  11115,  11118,  11121,  11124,
  11125,  11126,  11127,  11133,  11136,  11137,
  11138,  11139,
/* Xara    Mountain of the Evil Ones */
  31100,  31101,  31102,  31103,
  31104,  31105,  31106,  31107,  31109,  31110,
  31111,  31113,  31114,  31115,  31119,  31120,
  31121,  31122,  31123,  31124,  31125,  31126,
  31127,
/* Diku    Midgaard */
   3009,   3603,   4100,   4101,   4707,
   5008,   5010,   5100,   5111,   5315,   6311,
   6600,   6601,   6602,   6603,   6604,   6605,
   6606,   6607,   6608,   7008,   7013,   7703,
   8000,   8101,   8102,   8105,   8120,   8123,
   8600,   9208,   9233,   9322,  11001,  11004,
  11005,  11006,  11007,  11008,  11009,  11010,
  11011,  11101,  11104,  11115,  11118,  11121,
  11124,  11125,  11126,  11127,  11133,  11136,
  11137,  11138,  11139,  14004,  14005,  14006,
  14008,  14009,  14010,  14011,  14012,  19101,
  19103,  19108,  25000,  25002,  25003,  25007,
/* Jaromir Atlantis */
   8101,   8102,   8105,   8120,   8123,
/* Copper  In the Air */
   1000,
/* Diku    Limbo */
/* Generic Smurf Village */
    105,
/* Copper  Plains of the North */

/* Casret  Ultima */
   2410,   2433,   2436,   2437,   2446,
   2452,   2453,   2454,   2456,   2458,   2460,
   2462,   2463,   2464,   2468,   2469,   2470,
/* Hatchet New Ofcol */
    600,
/* Anon    High Tower of Sorcery */
   1341,   1347,   1348,   1349,
/* Vougon  Gnome Village */
   1512,
/* Tyrst   Wyvern's Tower */

/* Raff    Dwarven Catacombs */
   2015,
/* Raff    Dangerous Neighborhood */
   2108,
/* Wench   Dragon Tower */
/* Chris   The Keep of Mahn-Tor */
   2301,   2323,   2325,
/* Merc    Troll Den */
   2803,   2804,
/* Nirrad  Land of the Fire Newts */
   2906,
/* Copper  Chapel Catacombs */
/* Copper  Miden'nir */
/* Alfa    Graveyard */
   3603,
/* Hatchet Mud School */

/* Alfa    Moria */
   4100,   4101,
/* Anon    Kingdom of Juargan */
   4707,
/* Anon    Great Eastern Desert */
   5008,   5010,
/* Andi    The Great Pyramid */
   2605,   2608,   2611,   2612,   2613,   2614,
   2615,   2617,
/* Anon    Drow City */
   5100,
/* Anon    Thalos */

/* Kahn    Old Thalos */
   5315,
/* Alfa    Ofcol */

/* Anon    Arachnos */
   6311,
/* Diku    Haon Dor */

/* Anon    Dwarven Kingdom */

/* Sandman Dwarven Day Care */
   6600,
   6601,   6602,   6603,   6604,   6605,   6606,
   6607,   6608,
/* Diku    Sewer */
   7008,   7013,
/* Hatchet Valley of the Elves */

/* Diku    Redferne's Residence */

/* Glop    Mega-City One  */
   8000,
/* Generic Old Marsh */

/* Furey   Machine Dreams */
   8600,
/* Alfa    Holy Grove */
/* Dylan   Dylan's Area */
/* Raff    Elemental Canyon */
   9208,   9233,
/* Doctor  Galaxy */
   9322,
/* PinkF   Mob Factory */

/* KaVir   The learning centre */
  25000,  25002,  25003,
  25007,
/* KaVir   Artifacts */
    600,    902,    903,    904,    905,
    907,    908,    912,    913,    914,    915,
    916,    917,    918,    922,    924,    926,
   1000,   1341,   1347,   1348,   1349,   1512,
   2015,   2108,   2301,   2323,   2325,   2410,
   2433,   2436,   2437,   2446,   2452,   2453,
   2454,   2456,   2458,   2460,   2462,   2463,
   2464,   2468,   2469,   2470,   2605,   2608,
   2611,   2612,   2613,   2614,   2615,   2617,
   2803,   2804,   2906,   3009,   3603,   4100,
   4101,   4707,   5008,   5010,   5100,   5111,
   5315,   6311,   6600,   6601,   6602,   6603,
   6604,   6605,   6606,   6607,   6608,   7008,
   7013,   7703,   8000,   8101,   8102,   8105,
   8120,   8123,   8600,   9208,   9233,   9322,
  11001,  11004,  11005,  11006,  11007,  11008,
  11009,  11010,  11011,  11101,  11104,  11115,
  11118,  11121,  11124,  11125,  11126,  11127,
  11133,  11136,  11137,  11138,  11139,  14004,
  14005,  14006,  14008,  14009,  14010,  14011,
  14012,  19101,  19103,  19108,  25000,  25002,
  25003,  25007,
/* Spawn   Doom */
  11001,  11004,  11005,  11006,
  11007,  11008,  11009,  11010,  11011,
/* KaVir   Vallandar's Tomb */
  30330,
/* Garth   Cannabis */
  30232,  30234,  30235,  30237,  30238,  30240,
/* Dunkirk The Arena */
/* Andersen Astral/Githyanki */
   7703,
/* Jobo     Jobo's Hell */
  30100,  30101,  30102,  30103,  30104,
  30105,  30106,  30107,  30108,  30109,  30110,
  30111,
/* Jobo     Jobo's Heaven */
  99000,  99002,  99003,  99004,
/* Jobo     Old Dystopia */
  30406,
  30409,
/* Jobo     Realm of the Dawnbringers */
  80001,  80002,  80003,  80004,  80005,
  80006,
/* Jobo     Class EQ and other Relics */

/* Antibody Dome Ship StarSword */
  93000,  93001,  93002,  93003,  93004,
  93007,  93008,  93009,  93010,  93011,  93012,
  93013,  93014,  93015,  93016,  93017,  93018,
  93029,  93030,
/* Antibody Disneyworld */
  50002,  50003,  50004,  50005,
  50006,  50007,  50008,  50011,  50012,  50013,
  50014,  50018,  50019,  50022,  50024,  50026,
  50027,  50029,

/* Willaz   Lair of the black Dragon */
  32000,
/* Nyria    Santa's Workshop */
  50201,  50204,  50205,  50206,
  50207,  50208,  50209,  50210,  50211,  50212,
  50213,  50214,  50216,  50217,  50218,  50219,
/* Jatr     Village of Kakakaru */
  77002,  77003,  77004,  77005,  77006,  77008,
  77009,  77010,  77011,  77012,  77013,  77014,
  77015,  77016,
/* THE END */
  -1
  };

  /* counting the size of the table */
  for (count = 0; questmob_table[count] != -1; count++) ;

  vnum = number_range(0, count-1);
  return questmob_table[vnum];
}

int get_rand_hard_mob()
{
  int vnum, count = 0;
  
  static const int questmob_table[] = {
  /* The Forbidden Fortress */
/* KaVir   Odds and ends */
   5105,   5106,   5107,   5108,   5109,   8103,
   8106,   8107,   8108,   8109,   8112,   8114,
   8115,   8116,   8118,   8119,   8121,   8124,
/* Anon    DragonSpyre */
/* Wynfair Castle Dragonheart */
/* Xara    Sunyata */
  31054,
/* Xara    Easy Evil Ones */

/* Xara    Mountain of the Evil Ones */

/* Diku    Midgaard */
   5105,   5106,   5107,   5108,   5109,
   8103,   8106,   8107,   8108,   8109,   8112,
   8114,   8115,   8116,   8118,   8119,   8121,
   8124,
/* Jaromir Atlantis */
   8103,   8106,   8107,   8108,   8109,
   8112,   8114,   8115,   8116,   8118,   8119,
   8121,   8124,

/* Anon    Drow City */
   5105,   5106,   5107,   5108,
   5109,



/* Jobo     Old Dystopia */
  30400,  30401,  30402,  30403,  30408,
  30500,  30501,  30502,  30503,  30506,  30508,
  30509,
/* Jobo     Realm of the Dawnbringers */

/* THE END */
  -1 
  };

  /* counting the size of the table */
  for (count = 0; questmob_table[count] != -1; count++) ;
  
  vnum = number_range(0, count-1);
  return questmob_table[vnum];
}

void do_showquest(CHAR_DATA *ch, char *argument)
{
  QUEST_DATA *quest;
  OBJ_INDEX_DATA *pObjIndex;
  MOB_INDEX_DATA *pMobIndex1 = NULL;
  MOB_INDEX_DATA *pMobIndex2;
  MOB_INDEX_DATA *pMobIndex3;
  CHAR_DATA *gch;
  CHAR_DATA *ich = NULL;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;
  bool found2 = FALSE;

  if (IS_NPC(ch)) return;
  for (quest = ch->pcdata->quests; quest; quest = quest->next)
  {
    found = TRUE;
    switch(quest->type)
    {
      default:
        xprintf(buf, "Do_showquests: %s has bad quest type %d.", ch->name, quest->type);
        bug(buf, 0);
        break;
      case QT_MOB:
		if (quest->vnums[0] != -1 && (pMobIndex1 = get_mob_index(quest->vnums[0])) == NULL) 
        {
          xprintf(buf, "Do_showquests: %s has bad quest mob %d.", ch->name, quest->vnums[0]);
          bug(buf, 0);
          break;
        }
        if ((pMobIndex2 = get_mob_index(quest->giver)) == NULL)
        {
          xprintf(buf, "Do_showquests: %s has bad quest mob %d.", ch->name, quest->giver);
          bug(buf, 0);
          break; 
        }
        xprintf(buf, "You have been charged by #G%s#n to complete the following quest\n\r",
          pMobIndex2->short_descr);
        send_to_char(buf, ch);
        if (quest->vnums[0] != -1)
        {
          xprintf(buf, " * Find and slay #R%s#n.\n\r", pMobIndex1->short_descr);
          send_to_char(buf, ch);
          xprintf(buf, " * You have #C%d#n hours to complete the quest.\n\r", quest->time);
          send_to_char(buf, ch);
        }
        else
        {
          send_to_char(" * This quest has been #Ccompleted#n.\n\r", ch);
          xprintf(buf, " * You have #C%d#n hours to return to the questgiver.\n\r", quest->time);
          send_to_char(buf, ch);
        }
        break;
      case QT_PK:
        if ((pMobIndex2 = get_mob_index(quest->giver)) == NULL)
        {
          xprintf(buf, "Do_showquests: %s has bad quest mob %d.", ch->name, quest->giver);
          bug(buf, 0);
          break;
        }
        for (gch = char_list; gch && !found2; gch = gch->next)
        {
          if (IS_NPC(gch)) continue;
          if (gch->pcdata->playerid == quest->vnums[0])
          {
            ich = gch;
            found2 = TRUE;
          }
        }
        xprintf(buf, "You have been charged by #G%s#n to complete the following quest\n\r",
          pMobIndex2->short_descr);
        send_to_char(buf, ch);
        if (quest->vnums[0] != -1)
        {
          xprintf(buf, " * Hunt and slay #R%s#n.\n\r",
            found2 ? ich->name : "someone");
          send_to_char(buf, ch);
          xprintf(buf, " * You have #C%d#n hours to complete the quest.\n\r", quest->time);
          send_to_char(buf, ch);
        }
        else
        {
          send_to_char(" * This quest has been #Ccompleted#n.\n\r", ch);
          xprintf(buf, " * You have #C%d#n hours to return to the questgiver.\n\r", quest->time);
          send_to_char(buf, ch);
        }
        break;
      case QT_OBJ:
        if ((pObjIndex = get_obj_index(quest->vnums[0])) == NULL)
        {
          xprintf(buf, "Do_showquests: %s has bad quest item %d.", ch->name, quest->vnums[0]);
          bug(buf, 0);
          break;
        }
        if ((pMobIndex2 = get_mob_index(quest->giver)) == NULL)
        {
          xprintf(buf, "Do_showquests: %s has bad quest mob %d.", ch->name, quest->giver);
          bug(buf, 0);
          break;
        }
        xprintf(buf, "You have been charged by #G%s#n to complete the following quest\n\r",
          pMobIndex2->short_descr);
        send_to_char(buf, ch);
        xprintf(buf, " * Find and return #R%s#n.\n\r", pObjIndex->short_descr);
        send_to_char(buf, ch);
        xprintf(buf, " * You have #C%d#n hours to complete the quest.\n\r", quest->time);
        send_to_char(buf, ch);
        break;
      case QT_MOB_AND_OBJ:
		if (quest->vnums[0] != -1 && (pMobIndex1 = get_mob_index(quest->vnums[0])) == NULL) 
        {
          xprintf(buf, "Do_showquests: %s has bad quest mob %d.", ch->name, quest->vnums[0]); 
          bug(buf, 0);
          break;
        }
        if ((pObjIndex = get_obj_index(quest->vnums[1])) == NULL)
        {
          xprintf(buf, "Do_showquests: %s has bad quest item %d.", ch->name, quest->vnums[1]);
          bug(buf, 0);
          break;
        }
        if ((pMobIndex2 = get_mob_index(quest->giver)) == NULL)
        {
          xprintf(buf, "Do_showquests: %s has bad quest mob %d.", ch->name, quest->giver);
          bug(buf, 0);
          break;
        }
        xprintf(buf, "You have been charged by #G%s#n to complete the following quest\n\r",
          pMobIndex2->short_descr);
        send_to_char(buf, ch);
        if (quest->vnums[0] != -1)
        {
          xprintf(buf, " * Find and slay #R%s#n.\n\r", pMobIndex1->short_descr);
          send_to_char(buf, ch);
        }
        xprintf(buf, " * Find and return #R%s#n.\n\r", pObjIndex->short_descr);
        send_to_char(buf, ch);
        xprintf(buf, " * You have #C%d#n hours to complete the quest.\n\r", quest->time);
        send_to_char(buf, ch);
        break;
      case QT_MASS_KILL:
	    if (quest->vnums[0] != -1 && (pMobIndex1 = get_mob_index(quest->vnums[0])) == NULL)
        {
          xprintf(buf, "Do_showquests: %s has bad quest mob %d.", ch->name, quest->vnums[0]);
          bug(buf, 0);
          break;
        }
        if ((pMobIndex3 = get_mob_index(quest->vnums[1])) == NULL)
        {
          xprintf(buf, "Do_showquests: %s has bad quest mob %d.", ch->name, quest->vnums[1]);
          bug(buf, 0);
          break;
        }
        if ((pMobIndex2 = get_mob_index(quest->giver)) == NULL)
        {
          xprintf(buf, "Do_showquests: %s has bad quest mob %d.", ch->name, quest->giver);
          bug(buf, 0);
          break;
        }
        xprintf(buf, "You have been charged by #G%s#n to complete the following quest\n\r",
          pMobIndex2->short_descr);
        send_to_char(buf, ch);
        if (quest->vnums[3] >= quest->vnums[2])
        {
          send_to_char(" * This quest has been #Ccompleted#n.\n\r", ch);
          xprintf(buf, " * You have #C%d#n hours to return to the questgiver.\n\r", quest->time);
          send_to_char(buf, ch);
        }
        else
        {
          xprintf(buf, " * Find and slay #C%d#n monsters ranging from\n\r", quest->vnums[2] - quest->vnums[3]);
          send_to_char(buf, ch);
          xprintf(buf, "   #R%s#n to #R%s#n.\n\r", pMobIndex1->short_descr, pMobIndex3->short_descr);
          send_to_char(buf, ch);
          xprintf(buf, " * You have #C%d#n hours to complete the quest.\n\r", quest->time);
          send_to_char(buf, ch);
        }
        break;
    }
  }
  if (!found) send_to_char("You are currently undertaking no quests.\n\r", ch);
  return;
}

void quest_to_char(CHAR_DATA *ch, QUEST_DATA *quest)
{
  QUEST_DATA *quest_new;

  if (IS_NPC(ch))
  {
    bug("Quest_to_char: on npc", 0);
    return;
  }

  if (quest_free == NULL)
    quest_new = alloc_perm(sizeof(*quest_new));
  else
  {
    quest_new  = quest_free;
    quest_free = quest_free->next;
  }

 *quest_new          = *quest;
  quest_new->next    = ch->pcdata->quests;
  ch->pcdata->quests = quest_new;

  return;
}

/*
 * Remove an affect from a char.
 */
void quest_from_char(CHAR_DATA *ch, QUEST_DATA *quest)
{
  if (IS_NPC(ch))
  {
    bug("Quest_from_char: on npc", 0);
    return;
  }
  if (ch->pcdata->quests == NULL)
  {
    bug("Quest_from_char: No quest", 0);
    return;
  }
  if (quest == ch->pcdata->quests)
    ch->pcdata->quests = quest->next;
  else
  {
    QUEST_DATA *prev;
   
    for (prev = ch->pcdata->quests; prev; prev = prev->next)
    {
      if (prev->next == quest)
      {
        prev->next = quest->next;
        break;
      }
    }
    if (prev == NULL)
    {
      bug("Quest_from_char: No quest", 0);
      return;
    }
  }
  quest->next = quest_free;
  quest_free  = quest;
  return;
}

QUEST_FUN *quest_lookup(const char *name)
{
  int cmd;

  for (cmd = 0; *quest_table[cmd].quest_name; cmd++)
    if (!str_cmp(name, quest_table[cmd].quest_name))
      return quest_table[cmd].quest_fun;
  return 0;
}

char *quest_string(QUEST_FUN *fun)
{
  int cmd;

  for (cmd = 0; *quest_table[cmd].quest_fun; cmd++)
    if (fun == quest_table[cmd].quest_fun)
      return quest_table[cmd].quest_name;

  return 0;
}

void give_token(CHAR_DATA *questmaster, CHAR_DATA *ch, int value)
{
  OBJ_DATA *obj;
  char buf[MAX_STRING_LENGTH];

  value *= ccenter[CCENTER_QPS_LEVEL];
  value /= 100;
  if (IS_SET(ch->pcdata->tempflag, TEMP_EDGE)) value *= 1.1;
  if (ch->pcdata->time_tick > 49)
  {
    value *= 100 + ch->pcdata->time_tick/10;
    value /= 100;
  }
  if (global_qp == TRUE)
        value *= 2;
   
  obj = create_object(get_obj_index(OBJ_VNUM_PROTOPLASM), 50);
  obj->value[0] = value;
  obj->level = value; 
  obj->cost = 1000;
  obj->item_type = ITEM_QUEST;
  obj_to_char(obj, ch);
  free_string(obj->name);
  obj->name = str_dup("bone token");
  free_string(obj->short_descr);
  xprintf(buf, "a %d point bone token", value);
  obj->short_descr = str_dup(buf);
  free_string(obj->description);
  xprintf(buf, "A %d point bone token lies on the floor.", value);
  obj->description = str_dup(buf);
  act("You recieve $p from $N.", ch, obj, questmaster, TO_CHAR);
  act("$n recieves $p from $N.", ch, obj, questmaster, TO_ROOM);
  ch->pcdata->questsrun++;
  ch->pcdata->questtotal += value;
  do_autosave(ch, "");
}

void do_qcomplete(CHAR_DATA *ch, char *argument)  
{
  CHAR_DATA *questmaster;
  QUEST_DATA *quest;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  bool found = FALSE;

  if (IS_NPC(ch)) return;
  argument = one_argument(argument, arg);
  one_argument(argument, arg2);
  if ((questmaster = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("You cannot seem to find that questmaster.\n\r", ch);
    return;
  }
  if (!IS_NPC(questmaster))   
  {
    send_to_char("Players cannot give quests.\n\r", ch);
    return;
  }
  if (questmaster->quest_fun != 0)
  {
    for (quest = ch->pcdata->quests; quest && !found; quest = quest->next)
    {
      if (quest->giver != questmaster->pIndexData->vnum) continue;
      found = TRUE;

      /* Let's check if the quest is actually completed */
      switch (quest->type)
      {
        default:
          bug("Quest_complete: Bad Quest Type", 0);
          return;
        case QT_MOB:
          if (quest->vnums[0] != -1)
          {
            send_to_char("You have not completed that quest yet.\n\r", ch);
            return;
          }
          break;
        case QT_PK:
          if (quest->vnums[0] != -1)
          {
            send_to_char("You have not completed that quest yet.\n\r", ch);
            return;
          }
          break;
        case QT_OBJ:
          if ((obj = get_obj_carry(ch, arg2,ch)) == NULL)
          {
            send_to_char("What object do you wish to return?\n\r", ch);
            return;
          }
          if (obj->pIndexData->vnum != quest->vnums[0])
          {
            send_to_char("That is not the object of the quest.\n\r", ch);
            return;
          }
          extract_obj(obj);
          break;
        case QT_MOB_AND_OBJ:
          if (quest->vnums[0] != -1)
          {
            send_to_char("You have not completed that quest yet.\n\r", ch);
            return;
          }
          if ((obj = get_obj_carry(ch, arg2,ch)) == NULL)
          {
            send_to_char("What object do you wish to return?\n\r", ch);
            return;   
          }
          if (obj->pIndexData->vnum != quest->vnums[1])
          {
            send_to_char("That is not the object of the quest.\n\r", ch);
            return;
          }
          extract_obj(obj);
          break;
        case QT_MASS_KILL:
          if (quest->vnums[2] > quest->vnums[3])
          {
            send_to_char("You have not completed that quest yet.\n\r", ch);
            return;
          }
          break;
      }
      (*questmaster->quest_fun)(questmaster, ch, "complete");
      quest_from_char(ch, quest);
    }
    if (!found) send_to_char("You are not questing for this questmaster.\n\r", ch);
  }
  else
    send_to_char("Doesn't seem like that's a questmaster.\n\r", ch);
  return;
}

void do_qgain(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *questmaster;
  QUEST_DATA *quest;
  char arg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;
  one_argument(argument, arg);
  if ((questmaster = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("You cannot seem to find that questmaster.\n\r", ch);
    return;
  }
  if (!IS_NPC(questmaster))
  {
    send_to_char("Players cannot give quests.\n\r", ch);
    return;
  }
  if (questmaster->quest_fun != 0)
  {
    for (quest = ch->pcdata->quests; quest; quest = quest->next)
    {
      if (quest->giver == questmaster->pIndexData->vnum)
      {
        send_to_char("You have already been given a quest from this questmaster.\n\r", ch);
        return;
      }
    }
    /* ROCK THEM! */
    (*questmaster->quest_fun)(questmaster, ch, "gain");  
  }
  else
    send_to_char("Doesn't seem like that's a questmaster.\n\r", ch);
}

void questspec_generic(CHAR_DATA *questmaster, CHAR_DATA *ch, char *argument)
{
  if (!str_cmp(argument, "gain"))
  {
    QUEST_DATA new_quest;

    do_say(questmaster, "Will you smurf this smurf for me?");
    new_quest.type      = QT_MOB;
    new_quest.time      = 20;
    new_quest.giver     = questmaster->pIndexData->vnum;
    new_quest.vnums[0]  = number_range(103, 109);
    quest_to_char(ch, &new_quest);
    return;
  }
  else if (!str_cmp(argument, "complete"))
  {
    int value = 20;

    give_token(questmaster, ch, value);
    do_say(questmaster, "That's the spirit, thanks for solving my quest.");
    return;
  }
}

void questspec_newbie(CHAR_DATA *questmaster, CHAR_DATA *ch, char *argument)
{
  if (!str_cmp(argument, "gain"))
  {
    QUEST_DATA new_quest;

    do_say(questmaster, "There you go, type 'showquest' to see the quest.");
    new_quest.type      = QT_MOB;
    new_quest.time      = 20;
    new_quest.giver     = questmaster->pIndexData->vnum;
    new_quest.vnums[0]  = number_range(957, 960);
    quest_to_char(ch, &new_quest);
    return;
  }
  else if (!str_cmp(argument, "complete"))
  {
    int value = 50;

    give_token(questmaster, ch, value);
    do_say(questmaster, "You can spend the questpoints at the shop on floor 3.");
    return;
  }
}

void questspec_rand_mob(CHAR_DATA *questmaster, CHAR_DATA *ch, char *argument)
{
  if (!str_cmp(argument, "gain"))
  {
    QUEST_DATA new_quest;

    do_say(questmaster, "I charge you to find and kill this monster!");
    new_quest.type      = QT_MOB;
    new_quest.time      = 15;
    new_quest.giver     = questmaster->pIndexData->vnum;
    new_quest.vnums[0]  = get_rand_mob();
    quest_to_char(ch, &new_quest);
    return;
  } 
  else if (!str_cmp(argument, "complete"))
  {
    int value = 100;

    give_token(questmaster, ch, value);
    do_say(questmaster, "Thanks for solving my quest, come back again if you want.");
    return;
  }
}

void questspec_hard_mob(CHAR_DATA *questmaster, CHAR_DATA *ch, char *argument)
{
  if (!str_cmp(argument, "gain"))
  {
    QUEST_DATA new_quest;

    do_say(questmaster, "I charge you to find and kill this horrible monster!");
    new_quest.type      = QT_MOB;
    new_quest.time      = 25;
    new_quest.giver     = questmaster->pIndexData->vnum;
    new_quest.vnums[0]  = get_rand_hard_mob();        
    quest_to_char(ch, &new_quest);
    return;
  }
  else if (!str_cmp(argument, "complete"))
  {
    int value = 200;

    give_token(questmaster, ch, value);
    do_say(questmaster, "Thanks for solving my quest, come back again if you want.");
    return;
  } 
}  

void questspec_special_item(CHAR_DATA *questmaster, CHAR_DATA *ch, char *argument)
{
  if (!str_cmp(argument, "gain"))
  {
    QUEST_DATA new_quest;

    do_say(questmaster, "I charge you to retrive this ancient artifact of mine!");
    new_quest.type      = QT_OBJ;   
    new_quest.time      = 25;
    new_quest.giver     = questmaster->pIndexData->vnum;
    new_quest.vnums[0]  = load_special_item(ch);
    quest_to_char(ch, &new_quest);
    return;
  }
  else if (!str_cmp(argument, "complete"))
  {
    int value = 250;

    give_token(questmaster, ch, value);
    do_say(questmaster, "Thanks for solving my quest, come back again if you want.");
    return;
  }
}

void questspec_mob_and_item(CHAR_DATA *questmaster, CHAR_DATA *ch, char *argument)
{
  if (!str_cmp(argument, "gain"))
  {
    QUEST_DATA new_quest;   

    do_say(questmaster, "Please slay this monster and retrive this item for me!");
    new_quest.type      = QT_MOB_AND_OBJ;
    new_quest.time      = 25;
    new_quest.giver     = questmaster->pIndexData->vnum;
    new_quest.vnums[0]  = get_rand_mob();
    new_quest.vnums[1]  = get_rand_item();
    quest_to_char(ch, &new_quest);
    return;
  }
  else if (!str_cmp(argument, "complete"))
  {
    int value = 550;

    give_token(questmaster, ch, value);
    do_say(questmaster, "Thanks for solving my quest, come back again if you want.");
    return;
  }
}

void questspec_mass_kill(CHAR_DATA *questmaster, CHAR_DATA *ch, char *argument)
{
  if (!str_cmp(argument, "gain"))
  {
    QUEST_DATA new_quest;
    int bound;

    bound = get_lbound();

    do_say(questmaster, "Please slay these monsters.");
    new_quest.type      = QT_MASS_KILL;
    new_quest.time      = 30;
    new_quest.giver     = questmaster->pIndexData->vnum;
    new_quest.vnums[0]  = bound;
    new_quest.vnums[1]  = get_ubound(bound);
    new_quest.vnums[2]  = number_range(8, 16);   // amount needed to be killed
    new_quest.vnums[3]  = 0;                     // counter
    quest_to_char(ch, &new_quest);
    return;
  }
  else if (!str_cmp(argument, "complete"))
  {
    int value = 175;

    give_token(questmaster, ch, value);
    do_say(questmaster, "Thanks for solving my quest, come back again if you want.");
    return;
  }
}  

void questspec_pk(CHAR_DATA *questmaster, CHAR_DATA *ch, char *argument)
{
  if (!str_cmp(argument, "gain"))
  {
    QUEST_DATA new_quest;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *gch = NULL;
    bool found = FALSE;

    for (d = descriptor_list; d && !found; d = d->next)
    {
      if (d->connected == CON_PLAYING && d->lookup_status == STATUS_DONE)
      {
        if ((gch = d->character) == NULL) continue;
        if (ch == gch) continue;
        found = TRUE;
      }
    }
    if (!found)
    {
      do_say(questmaster, "Sorry, I have no quest for you at this time.");
      return;
    }

    do_say(questmaster, "I charge you to find and destroy this player!");
    new_quest.type      = QT_PK;
    new_quest.time      = 8;
    new_quest.giver     = questmaster->pIndexData->vnum;
    new_quest.vnums[0]  = gch->pcdata->playerid;
    quest_to_char(ch, &new_quest);
    return;
  }
  else if (!str_cmp(argument, "complete"))
  {
    int value = 400;

    give_token(questmaster, ch, value);
    do_say(questmaster, "Thanks for solving my quest, come back again if you want.");
    interpret(questmaster, "clap" );
    return;
  }
}
