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

#ifndef _MERC_HEADER_
#define _MERC_HEADER_

#include <stdlib.h>
#include <limits.h>
#include <sys/cdefs.h>
#include <sys/time.h>  
#include <pthread.h>
#include "jobo_quest.h"
#include "jobo_comm.h"
#include "kav_soundex.h" 
#include "monk.h"
#include "giant.h"
#include "fae.h"
#include "angel.h"
#include "lich.h"
#include "shapeshifter.h"
#include "undead_knight.h"
#include "tanarri.h"
#include "cyborg.h"
#include "mage.h"
#include "ninja.h"
#include "drow.h"
#include "demon.h"
#include "samurai.h"
#include "skyblade.h"
#include "drone.h"
#include "ghoul.h"
#include "draconian.h"
#include "thief.h"
#include "shadow.h"
#include "priest.h"
#include "jedi.h"
#include "class.h"
#include "ww.h"
#include "dlfcn.h"

/*
 * Accommodate old non-Ansi compilers.
 */
#if defined(TRADITIONAL)
#define const
#define args( list )			( )
#define DECLARE_DO_FUN( fun )		void fun( )
#define DECLARE_SPEC_FUN( fun )		bool fun( )
#define DECLARE_SPELL_FUN( fun )	void fun( )
#define DECLARE_QUEST_FUN( fun )        void fun( )
#define DECLARE_SHOP_FUN( fun )         void fun( )
#define DECLARE_OBJ_FUN( fun )		void fun( )
#define DECLARE_ROOM_FUN( fun )		void fun( )
#else
#define args( list )			list
#define DECLARE_DO_FUN( fun )		DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )		SPEC_FUN  fun
#define DECLARE_QUEST_FUN( fun )        QUEST_FUN fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun
#define DECLARE_SHOP_FUN( fun )         SHOP_FUN  fun
#define DECLARE_OBJ_FUN( fun )		OBJ_FUN	  fun
#define DECLARE_ROOM_FUN( fun )		ROOM_FUN  fun
#endif



/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	defined(_AIX)
#if	!defined(const)
#define const
#endif
typedef int				sh_int;
typedef int				bool;

#define unix
#else
typedef short int			sh_int;
typedef unsigned char			bool;
#endif

#define COMPILE_TIME    __DATE__ " "__TIME__
#define VERS            __DATE__

/* mccp: support bits */
   
#include <zlib.h>
#define TELOPT_COMPRESS 85
#define COMPRESS_BUF_SIZE 16384

/*
 * Structure types.
 */
typedef struct  area_affect         AREA_AFFECT;
typedef struct	affect_data			AFFECT_DATA;
typedef struct	affect2_data		AFFECT2_DATA;
typedef struct	area_data			AREA_DATA;
typedef struct	ban_data			BAN_DATA;
typedef struct	char_data			CHAR_DATA;
typedef struct  alias_data          ALIAS_DATA;
typedef struct  quest_data          QUEST_DATA;
typedef struct  top_board			TOP_BOARD;
typedef struct	wiz_data			WIZ_DATA;
typedef struct  history_data        HISTORY_DATA;
typedef struct  l_board				LEADER_BOARD;
typedef struct  religion_table      RELIGION_TABLE;
typedef struct  map_type            MAP_TYPE;
typedef	struct	editor_data			EDITOR_DATA;
typedef struct  channel_storage     CHANNEL_STORAGE;
typedef struct  kingdom_data		KINGDOM_DATA;
typedef struct  war_data			WAR_DATA;
typedef struct  dummy_arg           DUMMY_ARG;
typedef struct	descriptor_data		DESCRIPTOR_DATA;
typedef struct	exit_data			EXIT_DATA;
typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
typedef struct	help_data			HELP_DATA;
typedef struct	kill_data			KILL_DATA;
typedef struct	mob_index_data		MOB_INDEX_DATA;
typedef struct	note_data			NOTE_DATA;
typedef struct	obj_data			OBJ_DATA;
typedef struct	obj_index_data		OBJ_INDEX_DATA;
typedef struct	pc_data				PC_DATA;
typedef struct	reset_data			RESET_DATA;
typedef struct	room_index_data		ROOM_INDEX_DATA;
typedef struct	time_info_data		TIME_INFO_DATA;
typedef struct	weather_data		WEATHER_DATA;
typedef struct  prog_list           PROG_LIST;
typedef struct  prog_code           PROG_CODE;
typedef struct  disabled_data       DISABLED_DATA;
typedef struct	project_data		PROJECT_DATA;
typedef struct  mob_trigger         MOB_TRIGGER;
typedef struct	auction_data		AUCTION_DATA;
typedef struct	sleep_data	        SLEEP_DATA; /* mpsleep*/
typedef struct  hint_data 			HINT_DATA;
typedef enum {sCreate,sFree,sFreeList} structure_dowhat;

typedef struct  buf_type                BUFFER; 

struct buf_type
{
    BUFFER *    next;
    bool        valid;
    sh_int      state;  /* error state of the buffer */
    sh_int      size;   /* size in k */
    char *      string; /* buffer's string */
}; 

extern SLEEP_DATA *first_sleep;

/*
*  Sleeping prog data
*/
struct sleep_data
{
	SLEEP_DATA *next;
	SLEEP_DATA *prev;
	CHAR_DATA *ch;
	CHAR_DATA *mob;
	PROG_CODE *prog;
	int valid;
	int vnum;
	int line;
	int timer;
};

struct auction_data
{
	OBJ_DATA *	item;
	CHAR_DATA *	owner;
	CHAR_DATA *	high_bidder;
	sh_int		status;
	long		current_bid;
    long		minbid;
	long		bones_held;
};

#define AUCTION_LENGTH			  5
extern          AUCTION_DATA            auction_info;
extern          int                      arena;
extern		int			TOTAL_BID;



#define MINIMUM_BID			100

struct disabled_data
{
  DISABLED_DATA         * next;          /* pointer to next node          */
  struct cmd_type const * command;       /* pointer to the command struct */
  char                  * disabled_by;   /* name of disabler              */
  sh_int                  level;         /* level of disabler             */
};

struct	project_data
{
    PROJECT_DATA * next;		/* Next project in list*/
    PROJECT_DATA * prev;		/* Previous project in list*/
    NOTE_DATA *  first_log;		/* First log on project*/
    NOTE_DATA *  last_log;		/* Last log  on project*/
    char *name;
    char *owner;
    char *coder;
    char *status;
    char *date;
    char *description;
    bool taken;				/* Has someone taken project?*/
};

extern DISABLED_DATA *disabled_first;

/*
 * Function types.
 */
typedef	void DO_FUN	args( ( CHAR_DATA *ch, char *argument ) );
typedef bool SPEC_FUN	args( ( CHAR_DATA *ch ) );
typedef void QUEST_FUN args (( CHAR_DATA * questmaster, CHAR_DATA * ch, char *argument ));
typedef void SPELL_FUN	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
typedef void SHOP_FUN  args (( CHAR_DATA * shopkeeper, CHAR_DATA * ch, char *argument ));
typedef void OBJ_FUN	args( ( OBJ_DATA *obj, char *argument ) );
typedef void ROOM_FUN	args( ( ROOM_INDEX_DATA *room, char *argument ) );

/*
 * String and memory management parameters.
 */
#define	MAX_KEY_HASH		 1024
#define MAX_STRING_LENGTH	 8192
#define MAX_INPUT_LENGTH	  400
#define MAS				   262144

/* 
 * Rotains Gobal Procedures
 */
void clear_stats    args( (CHAR_DATA *ch) );
void room_is_total_darkness args( (ROOM_INDEX_DATA *pRoomIndex) );
void improve_wpn    args( (CHAR_DATA *ch,int dtype, int right_hand) );
void improve_stance args( (CHAR_DATA *ch) );
void skillstance    args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void show_spell     args( (CHAR_DATA *ch, int dtype) );
void fightaction    args( (CHAR_DATA *ch, CHAR_DATA *victim, int actype , \
int dtype, int wpntype) ); 
void crack_head     args( (CHAR_DATA *ch, OBJ_DATA *obj, char *argument) );
void critical_hit   args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam) );

void take_item      args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void raw_kill args((CHAR_DATA * victim, CHAR_DATA * killer));
void trip           args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void disarm         args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void make_corpse    args( (CHAR_DATA *ch) );
void one_hit        args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype) );

void special_hurl   args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void make_part      args( (CHAR_DATA *ch, char *argument) );
void home_write args((void));
void behead                 args( (CHAR_DATA *victim) );
void paradox                args( (CHAR_DATA *ch) );

bool fair_fight             args( (CHAR_DATA *ch, CHAR_DATA *victim) );

/*
 * file stuff
 */
void load_kingdoms     args( (void) );
void save_kingdoms     args( (void) );

/* 
 * Godwars Game Parameters
 * By Rotain
 */

#define SKILL_ADEPT               100
#define SKILL_THAC0_32             18
#define SKILL_THAC0_00              6
#define VERSION_NUMBER              1
#define DONATION_ROOM_WEAPON     3207
#define DONATION_ROOM_ARMOR      3207
#define DONATION_ROOM_REST       3207
#define MAX_VAMPIRE_POWER           3 
#define MAX_CLAN                   11
#define MAX_DISCIPLINES		   44
#define MAX_ART                    12

/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */
#define MUDNAME                   "Mindcloud 2.5b"
#define ADMINNAME				  "Your Name Here"
#define MAX_KNUDE                 500
#define MAX_HISTORY                15
#define MAX_IGNORE                 10
#define MAX_ALIAS                  30
#define MAX_KINGDOM                 5
#define MAX_WAR			           30
#define CURRENT_REVISION            1 // change this each time you update revision of pfiles
#define PARADOX_TICK               30
#define MAX_SKILL		  231
#define MAX_SPELL		   74
#define MAX_LEVEL		   12
#define MAX_TOP_PLAYERS	           20
#define MAX_RELIGIONS               5

/* Size of the map and depth of recursion to undertake */
#define MAPX     10
#define MAPY      8
#define MAXDEPTH  2

#define NO_WATCH		   10
#define LEVEL_HERO		   (MAX_LEVEL - 9)
#define LEVEL_IMMORTAL		   (MAX_LEVEL - 5)

#define LEVEL_MORTAL		   (MAX_LEVEL - 10)
#define LEVEL_AVATAR		   (MAX_LEVEL - 9)
#define LEVEL_BUILDER		   (MAX_LEVEL - 5)
#define LEVEL_QUESTMAKER	   (MAX_LEVEL - 4)
#define LEVEL_ENFORCER		   (MAX_LEVEL - 3)
#define LEVEL_JUDGE		   (MAX_LEVEL - 2)
#define LEVEL_HIGHJUDGE		   (MAX_LEVEL - 1)
#define LEVEL_IMPLEMENTOR	   (MAX_LEVEL)

#define PULSE_THIRTY              (30 * PULSE_PER_SECOND)
#define PULSE_PER_SECOND	    4
#define PULSE_VIOLENCE		  ( 3 * PULSE_PER_SECOND)
#define PULSE_SHAPESPEC             7
#define PULSE_DB_DUMP             (1800* PULSE_PER_SECOND ) /* 30 minutes  */
#define PULSE_AUCTION             (10 * PULSE_PER_SECOND)
#define PULSE_EMBRACE             ( 4 * PULSE_PER_SECOND)
#define PULSE_MOBILE		  ( 4 * PULSE_PER_SECOND)
#define PULSE_PLAYERS	 	  ( 4 * PULSE_PER_SECOND)
#define PULSE_TICK		  (30 * PULSE_PER_SECOND)
#define PULSE_AREA		  (60 * PULSE_PER_SECOND)
#define PULSE_WW                  ( 4 * PULSE_PER_SECOND)
#define PULSE_ARENA		  (30 * PULSE_AREA)  /* 120 minutes */
#define PULSE_RAGNAROK            ( 15 * PULSE_AREA)
#define PULSE_HINT 		  ( 3 * PULSE_AREA)
#define PULSE_MINUTE              ( 60 * PULSE_PER_SECOND)

#define HINT_UPDATEFREQ         1

// Log types
#define LOG_CRIT       (A)
#define LOG_ERR        (B)
#define LOG_BUG        (C)
#define LOG_SECURITY   (D)
#define LOG_CONNECT    (E)
#define LOG_GAME       (F)
#define LOG_COMMAND    (G)
#define LOG_ALL        127   // All the others added up

#ifdef IMC
    #include "imc.h"
#endif

/*
 * Let's just keep it down here - Jobo
 */
#include "board.h"

/*
 * Site ban structure.
 */
struct	ban_data
{
    BAN_DATA *	next;
    char *	name;
    char *      reason;
};

struct hint_data
{
        HINT_DATA       *next;
        HINT_DATA       *prev;
        char            *text;
        int             low;
        int             high;
};

extern          HINT_DATA       * hint;
extern          HINT_DATA       *first_hint;
extern          HINT_DATA       *last_hint;

struct struckdrunk
{
    int min_drunk_level;
    int number_of_rep;
    char *replacement[11];
};

struct	wiz_data
{
    WIZ_DATA *	next;
    bool	valid;
    sh_int	level;
    char *	name;
};

/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define SKY_CLOUDLESS		    0
#define SKY_CLOUDY		    1
#define SKY_RAINING		    2
#define SKY_LIGHTNING		    3

#define MTRIG_SAY         0
#define MTRIG_LOCATE      1
#define MTRIG_SCRY        2
#define MTRIG_HUNTER      4

struct	time_info_data
{
    int		hour;
    int		day;
    int		month;
    int		year;
};

struct	weather_data
{
    int		mmhg;
    int		change;
    int		sky;
    int		sunlight;
};


struct bit_type
{
  char *const         name;
  int                 bit_value;
};

struct war_data
{
	int one;
	int two;
};

struct kingdom_data
{
  char * whoname;    // the name used in do_who().
  char * name;       // the keyword name.
  char * leader;     // who runs the place.
  char * general;    // who's the right hand man.
  int    recall;
  int    kills;      // amount of pkills done by kingdom members.
  int    deaths;     // amount of pkills done agains kingdom members.
  int    qps;        // the size of the kingdoms wealth.
  int    req_hit;    // hps req to join.
  int    req_move;   // move req to join.
  int    req_mana;   // mana req to join.
  int    req_qps;    // qps cost to join (will be donated to the kingdom vault).
};

       
/*
 * Directions.
 * Used in #ROOMS.
 */
typedef enum
{
  DIR_NORTH, DIR_EAST, DIR_SOUTH, DIR_WEST, DIR_UP, DIR_DOWN,
  DIR_NORTHEAST, DIR_NORTHWEST, DIR_SOUTHEAST, DIR_SOUTHWEST,
DIR_SOMEWHERE
}
dir_types;

/*
 * Stable prompt ala Erwins howto.
 */
#define VT_SAVECURSOR            "\e7"   /* Save cursor and attrib         */
#define VT_RESTORECURSOR         "\e8"   /* Restore cursor pos and attribs */
#define VT_SETWIN_CLEAR          "\e[r"  /* Clear scrollable window size   */
#define VT_CLEAR_SCREEN          "\e[2J" /* Clear screen                   */
#define VT_CLEAR_LINE            "\e[2K" /* Clear this whole line          */
#define VT_RESET_TERMINAL        "\ec"

#define SUB_NORTH DIR_NORTH
#define SUB_EAST  DIR_EAST
#define SUB_SOUTH DIR_SOUTH
#define SUB_WEST  DIR_WEST
#define SUB_UP    DIR_UP
#define SUB_DOWN  DIR_DOWN
#define SUB_NE    DIR_NORTHEAST
#define SUB_NW    DIR_NORTHWEST
#define SUB_SE    DIR_SOUTHEAST
#define SUB_SW    DIR_SOUTHWEST

/*
 * threaded status - Jobo
 */
#define STATUS_LOOKUP       0   // New Descriptor, in lookup pr. default.   
#define STATUS_DONE         1   // The lookup is done.
#define STATUS_WAIT         2   // Closed while in thread.
#define STATUS_CLOSED       3   // Closed, ready to be recycled.

/*
 * Extra Descr bits - Jobo
 */
#define ED_TYPE_NONE        0
#define ED_TYPE_PULL        1
#define ED_TYPE_PRESS       2
#define ED_TYPE_PUSH        3
#define ED_TYPE_TOUCH       4

#define ED_ACTION_NONE      0
#define ED_ACTION_TELEPORT  1
#define ED_ACTION_OBJECT    2
#define ED_ACTION_SPELL     3
#define ED_ACTION_ELEVATOR  4

/*
 * Mudinfo Bits
 */
#define MUDINFO_UPDATED      0
#define MUDINFO_MCCP_USERS   1
#define MUDINFO_OTHER_USERS  2
#define MUDINFO_PEAK_USERS   3
#define MUDINFO_MBYTE        4
#define MUDINFO_BYTE         5
#define MUDINFO_DATA_PEAK    6
#define MUDINFO_MSP_USERS    7
#define MUDINFO_MBYTE_S      8
#define MUDINFO_BYTE_S       9
#define MUDINFO_MAX         10

/*
 * Taking care of the control center
 */
#define CCENTER_MIN_EXP     0   /* default 4K        */
#define CCENTER_MAX_EXP     1   /* default 3 mill    */
#define CCENTER_EXP_LEVEL   2   /* default 100 %     */
#define CCENTER_QPS_LEVEL   3   /* default 100 %     */
#define CCENTER_BONES_LEVEL 4   /* default 100 %     */
#define CCENTER_ARTI_MKILLS 5   /* default 3000      */
#define CCENTER_MAX         6

/*
 * Connected state for a channel.
 */
#define CON_PLAYING			 0
#define CON_GET_NAME			 1
#define CON_GET_OLD_PASSWORD		 2
#define CON_CONFIRM_NEW_NAME		 3
#define CON_GET_NEW_PASSWORD		 4
#define CON_CONFIRM_NEW_PASSWORD	 5
#define CON_GET_NEW_SEX			 6
#define CON_GET_NEW_CLASS		 7
#define CON_GET_NEW_ANSI                 9 
#define CON_READ_MOTD			 10
#define CON_NOT_PLAYING			 11
#define CON_EDITING			 12
#define CON_COPYOVER_RECOVER             13
#define CON_WIZ                          21
#define CON_CONTLOGIN                    22
#define CON_PFILE                        20


/*
 * Character substates
 */
typedef enum
{
  SUB_NONE, SUB_PAUSE, SUB_PERSONAL_DESC, SUB_OBJ_SHORT, SUB_OBJ_LONG,
  SUB_OBJ_EXTRA, SUB_MOB_LONG, SUB_MOB_DESC, SUB_ROOM_DESC, SUB_ROOM_EXTRA,
  SUB_ROOM_EXIT_DESC, SUB_WRITING_NOTE, SUB_MPROG_EDIT, SUB_HELP_EDIT,
  SUB_WRITING_MAP, SUB_PERSONAL_BIO, SUB_REPEATCMD, SUB_RESTRICTED,
  SUB_DEITYDESC,
  /* timer types ONLY below this point */
  SUB_TIMER_DO_ABORT = 128, SUB_TIMER_CANT_ABORT
} char_substates;

/*
 * Timer macros.
 */

#define TIMER(ch, tmr)		((ch)->tick_timer[(tmr)])
#define SET_TIMER(ch, tmr, tm)	((ch)->tick_timer[(tmr)] = (tm))
#define ADD_TIMER(ch, tmr, tm)  ((ch)->tick_timer[(tmr)] += (tm))
#define SUB_TIMER(ch, tmr, tm)  ((ch)->tick_timer[(tmr)] -= (tm))
#define TIME_UP(ch, tmr)	((ch)->tick_timer[(tmr)] == 0 ? TRUE : FALSE)

#define RTIMER(room, rtmr)	 ((room)->tick_timer[(rtmr)])
#define SET_RTIMER(room, rtmr, rtm) ((room)->tick_timer[(rtmr)] = (rtm))
#define ADD_RTIMER(room, rtmr, rtm)  ((room)->tick_timer[(rtmr)] += (rtm))
#define SUB_RTIMER(room, rtmr, rtm)  ((room)->tick_timer[(rtmr)] -= (rtm))
#define RTIME_UP(room, rtmr)	 ((room)->tick_timer[(rtmr)] == 0 ? TRUE : FALSE)

#define TIMER_LAYONHANDS	0
#define TIMER_CALLGOLEMS	1
#define TIMER_BADMOON 		2
#define TIMER_VAMPCALL 		3
#define TIMER_UNCONCIOUS 	4
#define TIMER_PESTILENCE 	5
#define TIMER_DAEMONIC_TRANSFORM 6
#define TIMER_MESMERISE		7
#define TIMER_MESMERISED	8
#define TIMER_DAMN		9
#define TIMER_DRONE_DISTORT	11
#define TIMER_REGEN		12
#define TIMER_SLOW		13
#define TIMER_HASTE		14
#define TIMER_THIRD_ARM_GROWING	15
#define TIMER_FOURTH_ARM_GROWING 16
#define TIMER_THIRD_ARM_GOING	17
#define TIMER_FOURTH_ARM_GOING  18
#define TIMER_ACCURACY		19
#define TIMER_HDESIRE    20
#define TIMER_CANT_BE_TURNED    21
#define TIMER_CANT_TURN    	22
#define TIMER_TREE_WALK    	23
#define TIMER_CAN_CHANGE_HAWK   24
#define TIMER_CAN_CREATE_SHARD  25
#define TIMER_CAN_GUST	        26
#define TIMER_CAN_ENTER_STASIS  27
#define TIMER_CALLWILD      28
#define TIMER_ENTOMB            29
#define TIMER_CAN_BREATHE_FROST 30
#define TIMER_HELLFIRE_SUMMON   31
#define TIMER_GFURY	   	32
#define TIMER_WYRM_ROAR         33
#define TIMER_CAN_CALL_WAR_HORSE 35
#define TIMER_EXPERIENCE        36
#define TIMER_CAN_SPIT_VENOM    37
#define TIMER_CAN_GAIN_VOODOO   38
#define TIMER_CAN_FEATHER       39
#define TIMER_MIGHT             40
#define TIMER_CAN_POLYMORPH     41
#define TIMER_DEJAVU            42
#define TIMER_VAMPIRE_GROW      43
#define TIMER_SKILL_LEV1        44
#define TIMER_SKILL_LEV2        45
#define TIMER_SUPERNOVA        46
#define TIMER_CANT_BORROWLIFE   47
#define TIMER_SHARDS		48
#define TIMER_DJINN	49
#define TIMER_HUNTER		50
#define TIMER_MADNESS		51
#define TIMER_GENIE	52
#define TIMER_CAN_DO_NEXUS	53
#define TIMER_CAN_USE_HEALER	54
#define TIMER_DISCORD		55
#define TIMER_UNUSED		56
#define TIMER_INFERNO		57
#define TIMER_CHAOSPORT		58
#define TIMER_CANMAJESTY	59
#define TIMER_MAJESTY		60
#define TIMER_DSLEEP		61
#define TIMER_CHALLENGE         62
#define TIMER_PUMMEL		63
#define MAX_TIMER		64


#define RTIMER_STINKING_CLOUD	0
#define RTIMER_LIFE_VORTEX	1
#define RTIMER_DEATH_VORTEX	2
#define RTIMER_GLYPH_PROTECTION	3
#define RTIMER_HIDE_ROOM	4
#define RTIMER_SWARM_BEES	5
#define RTIMER_SWARM_RATS	6
#define RTIMER_SWARM_BATS	7
#define RTIMER_GHOST_LIGHT	8
#define RTIMER_NEXUS_FLAME	9
#define RTIMER_NEXUS_WATER	10
#define RTIMER_NEXUS_AIR	11
#define RTIMER_NEXUS_EARTH	12
#define RTIMER_NEXUS_ENTROPY	13
#define RTIMER_WALL_NORTH	14
#define RTIMER_WALL_EAST	15
#define RTIMER_WALL_SOUTH	16
#define RTIMER_WALL_WEST	17
#define RTIMER_WALL_UP		18
#define RTIMER_WALL_DOWN	19
#define RTIMER_DISCORD		20
#define RTIMER_DARK_ROOM	21
#define RTIMER_SILENCE		22
#define MAX_RTIMER		30

/*
 * Needed for threads - Jobo
 */
struct dummy_arg {
  DUMMY_ARG        *next;
  DESCRIPTOR_DATA  *d;
  char             *buf;
  sh_int           status;
};

/*
 * Part of the communication storage system, Ye Jobo.
 */
struct channel_storage {
  char *last[MAX_STORAGE+1];
};

/*
 * Descriptor (channel) structure.
 */
struct descriptor_data
{
    DESCRIPTOR_DATA *	next;
    DESCRIPTOR_DATA *	snoop_by;
    CHAR_DATA *		character;
    CHAR_DATA *		original;
    char *		host;
  char                  *   showstr_head;           /* From ENVY code to compile */
  char                  *   showstr_point;          /* From ENVY code to compile */
  char                  *   outbuf;
  char                      inbuf[4 * MAX_INPUT_LENGTH];
  char                      incomm[MAX_INPUT_LENGTH];
  char                      inlast[MAX_INPUT_LENGTH];
  char                 **   pString;                /* OLC */
    sh_int		descriptor;
    sh_int		connected;
    sh_int              lookup_status;
    bool		fcommand;
    int			repeat;
    int			outsize;
    int			outtop; 
    int                       editor;                 /* OLC */
    void *              pEdit;          /* OLC */
  z_stream              *   out_compress;           /* MCCP */
  unsigned char         *   out_compress_buf;       /* MCCP */
};



/*
 * Attribute bonus structures.
 */
struct	str_app_type
{
    sh_int	tohit;
    sh_int	todam;
    sh_int	carry;
    sh_int	wield;
};

struct	int_app_type
{
    sh_int	learn;
};

struct	wis_app_type
{
    sh_int	practice;
};

struct	dex_app_type
{
    sh_int	defensive;
};

struct	con_app_type
{
    sh_int	hitp;
    sh_int	shock;
};

/*
 * TO types for act.
 */
#define TO_ROOM		    0
#define TO_NOTVICT	    1
#define TO_VICT		    2
#define TO_CHAR		    3
#define TO_ALL		    4

/*
 * Help table types.
 */
struct	help_data
{
    HELP_DATA *	next;
    HELP_DATA * prev;
    AREA_DATA * area;
    sh_int	level;
    char *	keyword;
    char *	text;
};

/*
 * Data structure for notes.
 */
struct	note_data
{
    NOTE_DATA *	next;
    char *	sender;
    char *	date;
    char *	to_list;
    char *	subject;
    char *	text;
    time_t	date_stamp;
    time_t	expire;
};

/*
 * An affect for areas
 */
struct area_affect
{
  AREA_AFFECT *next;
  sh_int       religion;
  sh_int       duration;
  sh_int       type;
  sh_int       level;
};


/*
 * An affect.
 */
struct	affect_data
{
    AFFECT_DATA *	next;
    sh_int		type;
    sh_int		duration;
    sh_int		location;
    sh_int		modifier;
    int			bitvector;
};

struct	affect2_data
{
    AFFECT2_DATA *	next;
    sh_int		type;
    sh_int		duration;
    sh_int		location;
    sh_int		modifier;
    int			bitvector;
};

/*
 * A quest
 */
struct quest_data
{
  QUEST_DATA *next;             // next quest player has
  sh_int time;                  // in mud hours
  sh_int type;                  // the type of quest
  int giver;                    // the questmasters vnum
  int vnums[4];                 // vnum of questor
};

/*
 * History data used for storing messages;
 */
struct history_data
{
  HISTORY_DATA *next;
  char *message;
};

/*
 * An alias
 */
struct  alias_data
{
  ALIAS_DATA * next;
  char       * short_n;
  char       * long_n;
};

/* Structure for the map itself */
struct map_type
{
  char tegn;                    /* Character to print at this map coord */
  int vnum;                     /* Room this coord represents */
  int depth;                    /* Recursive depth this coord was found at */
  int info;
  bool can_see;
};

typedef struct heap_data
{
  sh_int             iVertice;
  ROOM_INDEX_DATA  * knude[MAX_KNUDE];
} HEAP;
/*
 * A kill structure (indexed by level).
 */
struct	kill_data
{
    sh_int		number;
    sh_int		killed;
};

/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/

/* WIZnet flags */
#define WIZ_ON			(A)
#define WIZ_TICKS		(B)
#define WIZ_LOGINS		(C)
#define WIZ_SITES		(D)
#define WIZ_LINKS		(E)
#define WIZ_DEATHS		(F)
#define WIZ_RESETS		(G)
#define WIZ_MOBDEATHS	(H)
#define WIZ_FLAGS		(I)
#define WIZ_PENALTIES	(J)
#define WIZ_SACCING		(K)
#define WIZ_LEVELS		(L)
#define WIZ_SECURE		(M)
#define WIZ_SWITCHES	(N)
#define WIZ_SNOOPS		(O)
#define WIZ_RESTORE		(P)
#define WIZ_LOAD		(Q)
#define WIZ_NEWBIE		(R)
#define WIZ_PREFIX		(S)
#define WIZ_SPAM		(T)
#define WIZ_DEBUG       (U)
#define WIZ_IMC         (V)
#define WIZ_MEMCHECK    (W)

/*
 * MOBprog definitions
 */                   
#define TRIG_ACT	(A)
#define TRIG_BRIBE	(B)
#define TRIG_DEATH	(C)
#define TRIG_ENTRY	(D)
#define TRIG_FIGHT	(E)
#define TRIG_GIVE	(F)
#define TRIG_GREET	(G)
#define TRIG_GRALL	(H)
#define TRIG_KILL	(I)
#define TRIG_HPCNT	(J)
#define TRIG_RANDOM	(K)
#define TRIG_SPEECH	(L)
#define TRIG_EXIT	(M)
#define TRIG_EXALL	(N)
#define TRIG_DELAY	(O)
#define TRIG_SURR	(P)
#define TRIG_GET	(Q)
#define TRIG_DROP	(R)
#define TRIG_SIT	(S)
#define TRIG_REGEX	(T)

/*
 * Prog types
 */
#define PRG_MPROG	0
#define PRG_OPROG	1
#define PRG_RPROG	2

struct prog_list
{
    int                 trig_type;
    char *              trig_phrase;
    int              vnum;
    char *              code;
    PROG_LIST *         next;
    bool                valid;
};

struct prog_code
{
    int                vnum;
    char *              code;
    PROG_CODE *         next;
}; 

/* furniture flags */
#define STAND_AT		(A)
#define STAND_ON		(B)
#define STAND_IN		(C)
#define SIT_AT			(D)
#define SIT_ON			(E)
#define SIT_IN			(F)
#define REST_AT			(G)
#define REST_ON			(H)
#define REST_IN			(I)
#define SLEEP_AT		(J)
#define SLEEP_ON		(K)
#define SLEEP_IN		(L)
#define PUT_AT			(M)
#define PUT_ON			(N)
#define PUT_IN			(O)
#define PUT_INSIDE		(P)

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES. flags2
 */

#define VAMP_ASHES		(A)
#define VAMP_CLONE		(B)
#define VAMP_OBJMASK		(C)
#define AFF_TOTALBLIND		(D)
#define AFF_SPIRITGUARD		(E)

#define AFF_CLAW                (L)
#define AFF_BITE                (M)
#define AFF_TAIL                (N)
#define AFF_WING                (O)

// flag3
#define AFF3_BLINK_1ST_RD	(A)
#define AFF3_BLINK_2ND_RD	(B)


/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_VAMPIRE	   3404
#define MOB_VNUM_CANNIBAL	   30069
#define MOB_VNUM_WARLORD           3011 //FIX
/*
 * Immunities, for players.  KaVir.
 */
#define IMM_SLASH	      1	/* Resistance to slash, slice. 		*/
#define IMM_STAB	      2	/* Resistance to stab, pierce. 		*/
#define IMM_SMASH	      4	/* Resistance to blast, pound, crush. 	*/
#define IMM_ANIMAL	      8	/* Resistance to bite, claw. 		*/
#define IMM_MISC	     16	/* Resistance to grep, suck, whip. 	*/
#define IMM_CHARM	     32	/* Immune to charm spell. 		*/
#define IMM_HEAT	     64	/* Immune to fire/heat spells. 		*/
#define IMM_COLD	    128	/* Immune to frost/cold spells.		*/
#define IMM_LIGHTNING	    256	/* Immune to lightning spells.		*/
#define IMM_ACID	    512	/* Immune to acid spells.		*/
#define IMM_SUMMON	   1024	/* Immune to being summoned.		*/
#define IMM_VOODOO	   2048	/* Immune to voodoo magic.		*/
#define IMM_VAMPIRE	   4096	/* Allow yourself to become a vampire.	*/
#define IMM_STAKE	   8192	/* Immune to being staked (vamps only). */
#define IMM_SUNLIGHT	  16384	/* Immune to sunlight (vamps only).	*/
#define IMM_SHIELDED	  32768 /* For Obfuscate. Block scry, etc.	*/
#define IMM_HURL	  65536 /* Cannot be hurled.			*/
#define IMM_BACKSTAB	 131072 /* Cannot be backstabbed.		*/
#define IMM_KICK	 262144 /* Cannot be kicked.			*/
#define IMM_DISARM	 524288 /* Cannot be disarmed.			*/
#define IMM_STEAL	1048576 /* Cannot have stuff stolen.		*/
#define IMM_SLEEP	2097152 /* Immune to sleep spell.		*/
#define IMM_DRAIN       4194304 /* Immune to energy drain.		*/
#define IMM_SHIELD2	8388608 /* Chaotic shield			*/
#define IMM_TRANSPORT  16777216 /* Objects can't be transported to you.	*/
#define IMM_TRAVEL     33554432

/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC		    (A)		/* Auto set for mobs	*/
#define ACT_SENTINEL		(B)		/* Stays in one room	*/
#define ACT_SCAVENGER		(C)		/* Picks up objects	*/
#define ACT_AGGRESSIVE		(D)		/* Attacks PC's		*/
#define ACT_STAY_AREA		(E)		/* Won't leave area	*/
#define ACT_WIMPY		    (F)		/* Flees when hurt	*/
#define ACT_PET			    (G)		/* Auto set for pets	*/
#define ACT_TRAIN		    (H)		/* Can train PC's	*/
#define ACT_PRACTICE		(I)		/* Can practice PC's	*/
#define ACT_MOUNT		    (J)		/* Can be mounted	*/
#define ACT_NOPARTS		    (K)		/* Dead = no body parts	*/
#define ACT_NOEXP		    (L)		/* No exp for killing   */
#define ACT_PROTOTYPE		(M)
#define ACT_NOAUTOKILL		(N)
#define ACT_NOEXP2			(O)
#define ACT_NOTRAVEL		(P)
#define ACT_NOSUMMON		(Q)
#define ACT_NODAMAGE        (R)
#define ACT_DEALER		    (S)
#define ACT_INTELLIGENT             (T)

/*
 * Thingers for Demon Warps
 */

#define WARP_CBODY				1
#define WARP_SBODY				2
#define WARP_STRONGARMS				4
#define WARP_STRONGLEGS				8
#define WARP_VENOMTONG				16
#define WARP_SPIKETAIL				32
#define WARP_BADBREATH				64
#define WARP_QUICKNESS				128
#define WARP_STAMINA				256
#define WARP_HUNT				512
#define WARP_DEVOUR				1024
#define WARP_TERROR				2048
#define WARP_REGENERATE				4096
#define WARP_STEED				8192
#define WARP_WEAPON				16384
#define WARP_INFIRMITY				32768
#define WARP_GBODY				65536
#define WARP_SCARED				131072
#define WARP_MAGMA				262144
#define WARP_WEAK				524288
#define WARP_SLOW				1048576
#define WARP_VULNER				2097152
#define WARP_SHARDS				4194304
#define WARP_WINGS				8388608
#define WARP_CLUMSY				16777216
#define WARP_STUPID				33554432
#define WARP_SPOON				67108864
#define WARP_FORK				134217728
#define WARP_KNIFE				268435456
#define WARP_SALADBOWL				536870912



/* Bits for the Discie thing Numbers.. really.. not bits */

#define DISC_VAMP_CELE			2
#define DISC_VAMP_FORT			3
#define DISC_VAMP_OBTE			4
#define DISC_VAMP_PRES			5
#define DISC_VAMP_QUIE			6
#define DISC_VAMP_THAU			7
#define DISC_VAMP_AUSP			8
#define DISC_VAMP_DOMI			9
#define DISC_VAMP_OBFU			10
#define DISC_VAMP_POTE			11
#define DISC_VAMP_PROT			12
#define DISC_VAMP_SERP			13
#define DISC_VAMP_VICI			14
#define DISC_VAMP_DAIM			15
#define DISC_VAMP_ANIM			16

#define DISC_FAE_NATURE                  1
#define DISC_FAE_ARCANE                 17

#define DISC_WERE_BEAR			18
#define DISC_WERE_LYNX			19
#define DISC_WERE_BOAR			20
#define DISC_WERE_OWL			21
#define DISC_WERE_SPID			22
#define DISC_WERE_WOLF			23
#define DISC_WERE_HAWK			24
#define DISC_WERE_MANT			25
#define DISC_WERE_RAPT			26
#define DISC_WERE_LUNA			27
#define DISC_WERE_PAIN			28
#define DISC_WERE_CONG			29

#define DISC_DAEM_HELL			30
#define DISC_DAEM_ATTA			31
#define DISC_DAEM_TEMP			32
#define DISC_DAEM_MORP			33
#define DISC_DAEM_CORR			34
#define DISC_DAEM_GELU			35
#define DISC_DAEM_DISC			36
#define DISC_DAEM_NETH			37
#define DISC_DAEM_IMMU			38
#define DISC_VAMP_CHIM			39
#define DISC_VAMP_THAN			40
#define DISC_VAMP_OBEA			41
#define DISC_VAMP_NECR			42
#define DISC_VAMP_MELP			43

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */ 
#define AFF_BLIND		           1
#define AFF_INVISIBLE		       2
#define AFF_DETECT_EVIL		       4
#define AFF_DETECT_INVIS	       8
#define AFF_DETECT_MAGIC	      16
#define AFF_DETECT_HIDDEN	      32
#define AFF_SHADOWPLANE		      64 /* Creatures in shadow plane - KaVir */
#define AFF_SANCTUARY		     128
#define AFF_FAERIE_FIRE		     256
#define AFF_INFRARED		     512
#define AFF_CURSE		   		1024
#define AFF_FLAMING		   		2048 /* For burning creatures - KaVir */
#define AFF_POISON		   		4096
#define AFF_PROTECT		   		8192
#define AFF_ETHEREAL		  	16384 /* For ethereal creatures - KaVir */
#define AFF_SNEAK		  		32768
#define AFF_HIDE		  		65536
#define AFF_SLEEP		 		131072
#define AFF_CHARM		 		262144
#define AFF_FLYING		 		524288
#define AFF_PASS_DOOR			1048576
#define AFF_POLYMORPH			2097152 /* For polymorphed creatures - KaVir */
#define AFF_SHADOWSIGHT			4194304 /* Can see between planes - KaVir */
#define AFF_WEBBED		        8388608 /* Cannot move - KaVir */
#define AFF_PROTECT_GOOD       16777216
#define AFF_DROWFIRE           33554432 /* Drow Darkness - Rotain */   
#define AFF_ZULOFORM	       67108864
#define AFF_SHIFT             134217728
#define AFF_PEACE             268435456
#define AFF_INFIRMITY         536870912
#define AFF_HOLYAURA         1073741824

/* Affected_by 2 */
#define AFF_CONTRACEPTION   	    1
#define AFF_BEASTIE		          	2
#define PLR_IMPLAG		          	4
#define PLR_PSYCHO                	8
#define EXTRA_BLINKY       	     	16
#define AFF2_TENDRILS            	32
#define AFF2_BLINK_1ST_RD		 	64
#define AFF2_BLINK_2ND_RD			128
#define AFF2_BALOR					256
#define AFF2_FARMOR                   512
#define AFF2_THORNS                   1024
#define AFF2_DARMOR                   2048
#define AFF2_FROZEN                   4096

#define OBJ_VNUM_HOLYSYMBOL       2220
#define OBJ_VNUM_LEGS		     46

#define OBJ_VNUM_QUEST_MIN		9425
#define OBJ_VNUM_QUEST_MAX		9435

/*
 * Bits for 'itemaffect'.
 * Used in #MOBILES.
 */
#define ITEMA_SHOCKSHIELD	      1
#define ITEMA_FIRESHIELD	      2
#define ITEMA_ICESHIELD		      4
#define ITEMA_ACIDSHIELD	      8
#define ITEMA_MONKCHI                 16
#define ITEMA_CHAOSSHIELD            32
#define ITEMA_ARTIFACT               64
#define ITEMA_REGENERATE            128
#define ITEMA_SPEED                 256
#define ITEMA_VORPAL                512
#define ITEMA_TATTOO               1024
#define ITEMA_RIGHT_SILVER         2048
#define ITEMA_LEFT_SILVER          4096
#define ITEMA_MADNESS              8192
#define ITEMA_RESISTANCE          16384
#define ITEMA_VISION              32768
#define ITEMA_STALKER             65536
#define ITEMA_VANISH             131072
#define ITEMA_RAGER              262144
#define ITEMA_TALON		 524288
#define ITEMA_CHAOSHANDS	1048576
#define ITEMA_AFFMANTIS		2097152
#define ITEMA_AFFENTROPY	4194304 
#define ITEMA_AFFEYE		8388608
#define ITEMA_MAGESHIELD       16777216    /* Mage free attacks   */
#define ITEMA_STEELSHIELD      33554432        /* Mage tougness */
#define ITEMA_DEFLECTOR        67108864     /* Mage dodge/parry */
#define ITEMA_ILLUSIONS       134217728    /* Mage dodge/parry */
#define ITEMA_BEAST           268435456    /* Mage extra attacks */
#define ITEMA_HOLYSYMBOL      536870912    /* Religion Symobl     */
#define ITEMA_DROIDSPEED     1073741824    /* Droids Static Speed */



#define NO_COLOUR	""		/* Blank */
#define GREY		"[0;1;30m"	/* Dark Grey */
#define D_RED		"[0;0;31m"	/* Dark Red */
#define L_RED		"[0;1;31m"	/* Light Red */
#define D_GREEN		"[0;0;32m"	/* Dark Green */
#define L_GREEN		"[0;1;32m"	/* Light Green */
#define BROWN		"[0;0;33m"	/* Brown */
#define YELLOW		"[0;1;33m"	/* Yellow */
#define D_BLUE		"[0;0;34m"	/* Dark Blue */
#define L_BLUE		"[0;1;34m"	/* Light Blue */
#define MAGENTA		"[0;0;35m"	/* Magenta */
#define PINK		"[0;1;35m"	/* Pink */
#define D_CYAN		"[0;0;36m"	/* Dark Cyan */
#define L_CYAN		"[0;1;36m"	/* Light Cyan */
#define NORMAL		"[0;0;37m"	/* Light Grey */
#define WHITE		"[0;0;38m"	/* White */


#define ADD_COLOUR(_player, _str, _col)				\
{								\
  char swh_temp[255];						\
  if (!IS_NPC(_player) && IS_SET(_player->act, PLR_ANSI))	\
  {								\
     swh_temp[0] = '\0';					\
     strcpy(swh_temp, _col);					\
     strcat(swh_temp, _str);					\
     strcat(swh_temp, NORMAL);					\
     strcpy(_str, swh_temp);					\
  }								\
}

#define SCALE_COLS 4

#define COL_SCALE(_swh_str,_swh_ch,_swh_curr,_swh_max) \
	ADD_COLOUR(_swh_ch, _swh_str, \
	(_swh_curr < 1) ? L_RED : \
	(_swh_curr < _swh_max) ? \
	scale[(SCALE_COLS * _swh_curr) / ((_swh_max > 0) \
	? _swh_max : 1)] : L_CYAN)

extern char *scale[SCALE_COLS];

/*
 * Bits for 'vampire'.
 * Used for player vampires.
 */
#define VAM_FANGS		      1
#define VAM_CLAWS		      2
#define VAM_NIGHTSIGHT		      4
#define VAM_FLYING		      8 /* For flying creatures */
#define VAM_SONIC		     16 /* For creatures with full detect */
#define VAM_CHANGED		     32 /* Changed using a vampire power */
#define VAM_PROTEAN		     64 /* Claws, nightsight, and change */
#define VAM_CELERITY		    128 /* 66%/33% chance 1/2 extra attacks */
#define VAM_FORTITUDE		    256 /* 5 hp less per hit taken */
#define VAM_POTENCE		    512 /* Deal out 1.5 times normal damage */
#define VAM_OBFUSCATE		   1024 /* Disguise and invis */
#define VAM_AUSPEX		   2048 /* Truesight, etc */
#define VAM_OBTENEBRATION	   4096 /* Shadowplane/sight and shadowbody */
#define VAM_SERPENTIS		   8192 /* Eyes/serpent, heart/darkness, etc */
#define VAM_DISGUISED		  16384 /* For the Obfuscate disguise ability */
#define VAM_MORTAL		  32768 /* For Obfuscate mortal ability. */
#define VAM_DOMINATE		  65536 /* Evileye, command */
#define VAM_EVILEYE		 131072 /* Evileye, command */
#define VAM_PRESENCE		 262144 /* Presence discipline */
#define VAM_VICISSITUDE          524288 /* Vicissitude discipline */
#define VAM_THAU                1048576 /* Thaumaturgy discipline */
#define VAM_ANIMAL              2097152 /* Animalism discipline */
#define VAM_SHIFTED             4194304 /* Non-poly shift */
#define VAM_QUIETUS             8388608 /* Quietus discipline */
#define VAM_HEAD	       16777216
#define VAM_TAIL	       33554432
#define VAM_EXOSKELETON        67108864
#define VAM_HORNS	      134217728
#define VAM_WINGS	      268435456


/*
 * Bits for 'polymorph'.
 * Used for players.
 */
#define POLY_BAT		      1
#define POLY_WOLF		      2
#define POLY_MIST		      4
#define POLY_SERPENT		      8
#define POLY_RAVEN		     16
#define POLY_FISH		     32
#define POLY_FROG		     64
#define POLY_ZULOFORM               128
#define POLY_SHIFT                  256
#define POLY_SPIDER		    512
#define POLY_DRAGON		   1024
#define POLY_LIZARDFORM            2048
#define POLY_PFORM                 4096

/*
 * Languages.
 */
#define LANG_COMMON		      0
#define DIA_OLDE		      1
#define DIA_BAD			      2
#define LANG_DARK		      4

/*
 * Score.
 */
#define SCORE_TOTAL_XP		      0
#define SCORE_HIGH_XP		      1
#define SCORE_TOTAL_LEVEL	      2
#define SCORE_HIGH_LEVEL	      3
#define SCORE_QUEST		      4
#define SCORE_NUM_QUEST		      5



/*
 * Zombie Lord.
 */
#define ZOMBIE_NOTHING		      0
#define ZOMBIE_TRACKING		      1
#define ZOMBIE_ANIMATE		      2
#define ZOMBIE_CAST		      3
#define ZOMBIE_REST		      4



/*
 * Damcap values.
 */
#define DAM_CAP		      0
#define DAM_CHANGE	      1

/* return values for check_imm */
#define IS_NORMAL               0
#define IS_DIMMUNE              1
#define IS_RESISTANT            2
#define IS_VULNERABLE           3

/* damage classes */
#define DAM_NONE                0
#define DAM_BASH                1
#define DAM_PIERCE              2
#define DAM_SLASH               3
#define DAM_FIRE                4
#define DAM_COLD                5
#define DAM_LIGHTNING           6
#define DAM_ACID                7
#define DAM_POISON              8
#define DAM_NEGATIVE            9
#define DAM_HOLY                10
#define DAM_ENERGY              11
#define DAM_MENTAL              12
#define DAM_DISEASE             13
#define DAM_DROWNING            14
#define DAM_LIGHT               15
#define DAM_OTHER               16
#define DAM_HARM                17
#define DAM_CHARM               18
#define DAM_SOUND               19

/* IMM bits for mobs */
#define DIMM_SUMMON              (A)
#define DIMM_CHARM               (B)
#define DIMM_MAGIC               (C)
#define DIMM_WEAPON              (D)
#define DIMM_BASH                (E)
#define DIMM_PIERCE              (F)
#define DIMM_SLASH               (G)
#define DIMM_FIRE                (H)
#define DIMM_COLD                (I)
#define DIMM_LIGHTNING           (J)
#define DIMM_ACID                (K)
#define DIMM_POISON              (L)
#define DIMM_NEGATIVE            (M)
#define DIMM_HOLY                (N)
#define DIMM_ENERGY              (O)
#define DIMM_MENTAL              (P)
#define DIMM_DISEASE             (Q)
#define DIMM_DROWNING            (R)
#define DIMM_LIGHT               (S)
#define DIMM_SOUND               (T)
#define DIMM_WOOD                (X)
#define DIMM_SILVER              (Y)
#define DIMM_IRON                (Z)
 
/*
 * Mounts
 */
#define IS_ON_FOOT		      0
#define IS_MOUNT		      1
#define IS_RIDING		      2
#define IS_CARRIED		      4
#define IS_CARRYING		      8

/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL		      0
#define SEX_MALE		      1
#define SEX_FEMALE		      2

/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_MONEY_ONE	      2
#define OBJ_VNUM_MONEY_SOME	      3

/* Demonic Transformation */

#define OBJ_VNUM_DHELM		     27924
#define OBJ_VNUM_DGREAVES            27925
#define OBJ_VNUM_DCLOAK	             27926
#define OBJ_VNUM_DARMOR              27927
#define OBJ_VNUM_DRING               27928
#define OBJ_VNUM_DSWORD              27929
#define OBJ_VNUM_DSHIELD             27930

#define OBJ_VNUM_BROD		     30073
#define OBJ_VNUM_LKLAIVE             33114
#define OBJ_VNUM_GKLAIVE             29697
#define OBJ_VNUM_STAKE		     30011
#define OBJ_VNUM_MEDAL               29521
#define OBJ_VNUM_KHORNE		     29664
#define OBJ_VNUM_CORPSE_NPC	     10
#define OBJ_VNUM_CORPSE_PC	     11
#define OBJ_VNUM_SEVERED_HEAD	     12
#define OBJ_VNUM_TORN_HEART	     13
#define OBJ_VNUM_SLICED_ARM	     14
#define OBJ_VNUM_SLICED_LEG	     15
#define OBJ_VNUM_FINAL_TURD	     16

#define OBJ_VNUM_MUSHROOM	     20
#define OBJ_VNUM_LIGHT_BALL	     21
#define OBJ_VNUM_SPRING		     22
#define OBJ_VNUM_BLOOD_SPRING	     23

#define OBJ_VNUM_SCHOOL_MACE	   3700
#define OBJ_VNUM_SCHOOL_DAGGER	   3701
#define OBJ_VNUM_SCHOOL_SWORD	   3702
#define OBJ_VNUM_SCHOOL_VEST	   3703
#define OBJ_VNUM_SCHOOL_SHIELD	   3704
#define OBJ_VNUM_SCHOOL_BANNER     3716

/* For KaVir's stuff */
#define OBJ_VNUM_SOULBLADE	  30000
#define OBJ_VNUM_GATE             30042
#define OBJ_VNUM_GATE2		  30072
#define OBJ_VNUM_PORTAL		  30001
#define OBJ_VNUM_EGG		  30002
#define OBJ_VNUM_EMPTY_EGG	  30003
#define OBJ_VNUM_SPILLED_ENTRAILS 30004
#define OBJ_VNUM_QUIVERING_BRAIN  30005
#define OBJ_VNUM_SQUIDGY_EYEBALL  30006
#define OBJ_VNUM_SPILT_BLOOD      30007
#define OBJ_VNUM_VOODOO_DOLL      30010
#define OBJ_VNUM_RIPPED_FACE      30012
#define OBJ_VNUM_TORN_WINDPIPE    30013
#define OBJ_VNUM_CRACKED_HEAD     30014
#define OBJ_VNUM_SLICED_EAR	  30025
#define OBJ_VNUM_SLICED_NOSE	  30026
#define OBJ_VNUM_KNOCKED_TOOTH	  30027
#define OBJ_VNUM_TORN_TONGUE	  30028
#define OBJ_VNUM_SEVERED_HAND	  30029
#define OBJ_VNUM_SEVERED_FOOT	  30030
#define OBJ_VNUM_SEVERED_THUMB	  30031
#define OBJ_VNUM_SEVERED_INDEX	  30032
#define OBJ_VNUM_SEVERED_MIDDLE	  30033
#define OBJ_VNUM_SEVERED_RING	  30034
#define OBJ_VNUM_SEVERED_LITTLE	  30035
#define OBJ_VNUM_SEVERED_TOE	  30036
#define OBJ_VNUM_PROTOPLASM	  30037
#define OBJ_VNUM_COPPER           30049
#define OBJ_VNUM_IRON             30050
#define OBJ_VNUM_STEEL            30051
#define OBJ_VNUM_ADAMANTITE       30052
#define OBJ_VNUM_MITHRIL          30048

#define MOB_VNUM_GUARDIAN	  33001
#define MOB_VNUM_SERVANT          33002
#define MOB_VNUM_MOUNT		  6
#define MOB_VNUM_FROG		  7
#define MOB_VNUM_RAVEN		  8
#define MOB_VNUM_CAT		  9
#define MOB_VNUM_DOG		  10
#define MOB_VNUM_EYE		  12
#define MOB_VNUM_SATAN            30003
#define MOB_VNUM_DEMON		  30005
#define MOB_VNUM_SERPENT	  33003
#define MOB_VNUM_ILLUSION	  33004
#define MOB_VNUM_FIRE             93361  
#define MOB_VNUM_STONE            93362  
#define MOB_VNUM_IRON             93363  
#define MOB_VNUM_CLAY             93364
#define MOB_VNUM_HUNTER           78013

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT		      1
#define ITEM_SCROLL		      2
#define ITEM_WAND		      3
#define ITEM_STAFF		      4
#define ITEM_WEAPON		      5
#define ITEM_WINDOW                   7
#define ITEM_TREASURE		      8
#define ITEM_ARMOR		      9
#define ITEM_POTION		     10
#define ITEM_FURNITURE		     12
#define ITEM_TRASH		     13
#define ITEM_CONTAINER		     15
#define ITEM_DRINK_CON		     17
#define ITEM_KEY		     18
#define ITEM_FOOD		     19
#define ITEM_MONEY		     20
#define ITEM_BOAT		     22
#define ITEM_CORPSE_NPC		     23
#define ITEM_CORPSE_PC		     24
#define ITEM_FOUNTAIN		     25
#define ITEM_PILL		     26
#define ITEM_PORTAL		     27
#define ITEM_EGG		     28
#define ITEM_VOODOO		     29
#define ITEM_STAKE		     30
#define ITEM_MISSILE		 31 /* Ammo vnum, cur, max, type */
#define ITEM_AMMO		     32 /* ???, dam min, dam max, type */
#define ITEM_QUEST		     33
#define ITEM_PIECE		     34
#define ITEM_MITHRIL	             35
#define ITEM_SYMBOL		     36
#define ITEM_BOOK		     37
#define ITEM_PAGE		     38
#define ITEM_TOOL		     39
#define ITEM_WALL            40
#define ITEM_COPPER          41
#define ITEM_IRON		     42
#define ITEM_STEEL		     43
#define ITEM_ADAMANTITE		 44
#define ITEM_GEMSTONE        45
#define ITEM_HILT            46
#define ITEM_DTOKEN		     47
#define ITEM_HEAD		     48
#define ITEM_JUKEBOX		     49
#define ITEM_COOKINGPOT   	50
#define	ITEM_DRAGONGEM		 51
#define ITEM_WGATE			 52
#define ITEM_SLOT_MACHINE    54
#define ITEM_GRENADE         55
#define ITEM_HOLYSYMBOL      56
#define ITEM_FAITHTOKEN      57


/*
 * Weapon Stats
 */

#define WEAPON_FLAMING          (A)
#define WEAPON_FROST            (B)
#define WEAPON_VAMPIRIC         (C)
#define WEAPON_SHARP            (D)
#define WEAPON_VORPAL           (E)
#define WEAPON_TWO_HANDS        (F)
#define WEAPON_SHOCKING         (G)
#define WEAPON_POISON           (H)
#define WEAPON_SUNBLADE         (I)
#define WEAPON_DRAGONLANCE      (J)
#define WEAPON_SILVER           (K)
#define WEAPON_RUNE_FORCE_BOLT  (L)
#define WEAPON_RUNE_SMITE_EVIL  (M)
#define WEAPON_RUNE_BLAZE       (N)
#define WEAPON_RUNE_LIGHTNING   (O)
#define WEAPON_RUNE_DANCING     (P)
#define WEAPON_ELE_FLAME        (Q)
#define WEAPON_ELE_WATER        (R)
#define WEAPON_ELE_EARTH        (S)
#define WEAPON_ELE_AIR          (T)

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		      (A)
#define ITEM_HUM		      (B)
#define ITEM_THROWN		      (C)
#define ITEM_KEEP		      (D)
#define ITEM_VANISH		      (E)
#define ITEM_INVIS		      (F)
#define ITEM_MAGIC		      (G)
#define ITEM_NODROP		      (H)
#define ITEM_NOLOCATE		      (I)
#define ITEM_ANTI_GOOD		      (J)
#define ITEM_ANTI_EVIL		      (K)
#define ITEM_ANTI_NEUTRAL	      (L)
#define ITEM_NOREMOVE		   4096
#define ITEM_INVENTORY		   8192
#define ITEM_LOYAL		  16384
#define ITEM_SHADOWPLANE	  32768
#define ITEM_MENCHANT	          65536
#define ITEM_NOCLAIM             131072
#define ITEM_BLESS               262144
#define ITEM_PROTOTYPE		 524288 // 2048, same as anti_neutral.. FIX FIX
#define ITEM_ELECENCHANT	1048576
#define ITEM_ICEENCHANT		2097152
#define ITEM_FIREENCHANT	4194304
#define ITEM_MQUEST                   (X)

/* Item extra flags II, the return of the item flags!  */

#define ITEM_ARM		  (A)
#define ITEM_NYSTUL               (B)
#define ITEM_NO_INTERRUPT         (C)
#define ITEM_DAEMONSEED		  (D)
#define ITEM_JUJU_BAG		  (F)
#define ITEM_VOO_HEAD		  (G)
#define ITEM_VOO_DEAD		  (H)
#define ITEM_VOO_BODY		  (I)
#define ITEM_VOO_THREAD		  (J)
#define ITEM_INGRED_GLOIN	  (K)
#define ITEM_INGRED_FROGBREATH    (L)
#define ITEM_INGRED_PAPACOCO	  (M)
#define ITEM_INGRED_MULDALEAF	  (N)
#define ITEM_INGRED_SCULLYWEED    (O)
#define ITEM_INGRED_WORMWART	  (P)
#define ITEM_INGRED_TILLIFREEN    (Q)
#define ITEM_INGRED_BAJUJU        (R)
#define ITEM_ATTACK_GOOD          (S)
#define ITEM_ITEMHIDE	          (T)
#define ITEM_ICE                  (U)

/* artifact and relic flags */

#define ITEM_TELEPORTS		   (W)  /* teleports upon owner death */
#define ITEM_DESTROYED		   (X)  /* destroyed upon owner death */
#define ITEM_UNIQUE		   (Y)  /* only 1 in game ever */
#define ITEM_DESIRED		   (Z)  /* you can't let go of it ever */
#define ITEM_INDESTRUCTABLE	   (aa) /* guess */
#define ITEM_TELEPORT_PROTECTION   (bb) /* teleports when attempts to destroy it */
#define ITEM_KNOW_OWNER 	   (cc) /* owner displayed on artifact command */

#define ITEM_FLYING	           (dd)
#define ITEM_FORM_MELDED	   (ee)

/*More New Seeds of Hate custom class Bits (Trent)  This is mostly mages*/
#define SOH_TALONS					1
#define SOH_SPELLREADY				2 //<==A LOT of mage flags from here on down
#define SOH_SPELLTRUESIGHT			4 //<--Magic Group //(SPELLasdf : the spell ready flag for that spell
#define SOH_SPELLSCRY				8 //(asdf) : the affect
#define SOH_SPELLTELEPORT			16
#define SOH_SPELLSLOW				32
#define SOH_SPELLHASTE				64
#define SOH_SPELLACCURACY			128 //<-- Naturalism.. EagleEye/MagicEye
#define SOH_SPELLSHARDS				256 //  Shards Spells
#define SOH_SPELLHEAL				512 //Healing Spells
#define SOH_SPELLREGEN				1024 //Regen Spells
#define SOH_SPELLSORC1				2048 //<--Sorcery Spells  (yes i know its superficial)
#define SOH_SPELLSORC2				4096
#define SOH_SPELLSORC3				8192
#define SOH_SPELLSORC4				16384
#define SOH_SPELLSORC5				32768
#define SOH_SPELLENCHANT1			65536 //<--Enchant Spells
#define SOH_SPELLENCHANT2			131072
#define SOH_SPELLENCHANT3			262144
#define SOH_SPELLENCHANT4			524288
#define SOH_SPELLENCHANT5			1048576
#define SOH_SPECFIRE				2097152 //<--Elemental Specializations
#define SOH_SPECICE				4194304
#define SOH_SPECLIGHTNING			8388608
#define SOH_SLOW				16777216 //<--And now.. the affects!
#define SOH_HASTE				33554432
#define SOH_ACCURACY				67108864
#define SOH_SPELLMIGHT				134217728 //<--Woops forgot that one
#define SOH_MIGHT				268435456
#define SOH_REGEN				536870912
#define SOH_SHARDS				1073741824
/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		      1
#define ITEM_WEAR_FINGER	      2
#define ITEM_WEAR_NECK		      4
#define ITEM_WEAR_BODY		      8
#define ITEM_WEAR_HEAD		     16
#define ITEM_WEAR_LEGS		     32
#define ITEM_WEAR_FEET		     64
#define ITEM_WEAR_HANDS		    128 
#define ITEM_WEAR_ARMS		    256
#define ITEM_WEAR_SHIELD	    512
#define ITEM_WEAR_ABOUT		   1024 
#define ITEM_WEAR_WAIST		   2048
#define ITEM_WEAR_WRIST		   4096
#define ITEM_WIELD		   8192
#define ITEM_HOLD		  16384
#define ITEM_WEAR_FACE		  32768
#define ITEM_WEAR_SPECIAL	  65536
#define ITEM_WEAR_BODYART 	 131072
#define ITEM_WEAR_MEDAL          262144
#define ITEM_WEAR_FLOAT          524288


/*
 * Special types.
 * Used in #OBJECTS for special items - KaVir.
 */
#define SITEM_ACTIVATE		      1
#define SITEM_TWIST		      2
#define SITEM_PRESS		      4
#define SITEM_PULL		      8
#define SITEM_TARGET		      16
#define SITEM_SPELL		      32
#define SITEM_TRANSPORTER	      64
#define SITEM_TELEPORTER	      128
#define SITEM_DELAY1		      256
#define SITEM_DELAY2		      512
#define SITEM_OBJECT		     1024
#define SITEM_MOBILE		     2048
#define SITEM_ACTION		     4096
#define SITEM_MORPH		     8192
#define SITEM_SILVER		    16384
#define SITEM_WOLFWEAPON	    32768
#define SITEM_DROW		    65536
#define SITEM_CHAMPWEAPON	   131072
#define SITEM_DEMONIC		   262144
#define SITEM_MITHRIL              524288
#define SITEM_COPPER              1048576
#define SITEM_MAGE		  2097152
#define SITEM_STEEL		  4194304
#define SITEM_ADAMANTITE	  8388608
#define SITEM_GEMSTONE           16777216
#define SITEM_HILT               33554432
#define SITEM_PDEMONIC		 67108864
#define SITEM_MONK		134217728 
#define SITEM_IRON		268435456
#define SITEM_SKYBLADE          536870912

/*
 * Apply types (for quest affects).
 * Used in #OBJECTS.
 */
#define QUEST_STR		      1
#define QUEST_DEX		      2
#define QUEST_INT		      4
#define QUEST_WIS		      8
#define QUEST_CON		     16
#define QUEST_HITROLL		     32
#define QUEST_DAMROLL		     64
#define QUEST_HIT		    128
#define QUEST_MANA		    256
#define QUEST_MOVE		    512
#define QUEST_AC		   1024
#define QUEST_ENCHANTED		   2048
#define QUEST_SPELLPROOF	   4096
#define QUEST_ARTIFACT		   8192
#define QUEST_IMPROVED		  16384
#define QUEST_PRIZE      	  32768
#define QUEST_RELIC               65536
#define QUEST_NAME		 131072
#define QUEST_BLOODA             262144
#define QUEST_CLONED            1048576
#define QUEST_ZOMBIE            2097152
#define QUEST_FORGE             4194304
#define QUEST_MQUEST	        8388608
#define QUEST_SHORT	       16777216
#define QUEST_LONG	       33554432
#define QUEST_FREENAME	       67108864
#define QUEST_WWPOISON        134217728
#define QUEST_PLATING         268435456
#define QUEST_HOLYSYMBOL      536870912
#define QUEST_GIANTSTONE     1073741824

/*
 * Tool types.
 */
#define TOOL_PEN		      1
#define TOOL_PLIERS		      2
#define TOOL_SCALPEL		      4

/* furniture flags */
#define STAND_AT		(A)
#define STAND_ON		(B)
#define STAND_IN		(C)
#define SIT_AT			(D)
#define SIT_ON			(E)
#define SIT_IN			(F)
#define REST_AT			(G)
#define REST_ON			(H)
#define REST_IN			(I)
#define SLEEP_AT		(J)
#define SLEEP_ON		(K)
#define SLEEP_IN		(L)
#define PUT_AT			(M)
#define PUT_ON			(N)
#define PUT_IN			(O)
#define PUT_INSIDE		(P)

/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE		      0
#define APPLY_STR		      1
#define APPLY_DEX		      2
#define APPLY_INT		      3
#define APPLY_WIS		      4
#define APPLY_CON		      5
#define APPLY_SEX		      6
#define APPLY_CLASS		      7
#define APPLY_LEVEL		      8
#define APPLY_AGE		      9
#define APPLY_HEIGHT		     10
#define APPLY_WEIGHT		     11
#define APPLY_MANA		     12
#define APPLY_HIT		     13
#define APPLY_MOVE		     14
#define APPLY_GOLD		     15
#define APPLY_EXP		     16
#define APPLY_AC		     17
#define APPLY_HITROLL		     18
#define APPLY_DAMROLL		     19
#define APPLY_SAVING_PARA	     20
#define APPLY_SAVING_ROD	     21
#define APPLY_SAVING_PETRI	     22
#define APPLY_SAVING_BREATH	     23
#define APPLY_SAVING_SPELL	     24
#define APPLY_POLY		     25



/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		      1
#define CONT_PICKPROOF		      2
#define CONT_CLOSED		      4
#define CONT_LOCKED		      8



/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO		      2
#define ROOM_VNUM_CHAT		   1200
#define ROOM_VNUM_TEMPLE	   3005
#define ROOM_VNUM_ALTAR		   3054
#define ROOM_VNUM_SCHOOL	   3700
#define ROOM_VNUM_HELL		  93420
#define ROOM_VNUM_CRYPT		  30001
#define ROOM_VNUM_DISCONNECTION	  3
#define ROOM_VNUM_IN_OBJECT	  33000
#define ROOM_VNUM_AWINNER            70
#define ROOM_VNUM_ALOSER             69
#define ROOM_VNUM_CAINE		  27000
#define ROOM_VNUM_DEVOUR	  30006
#define ROOM_VNUM_ELKOR 	  100300
#define ROOM_VNUM_FORTRESS2         151
#define ROOM_VNUM_VICTORY           155
#define ROOM_VNUM_FORTRESS1         156

#define ROOM_VNUM_SAM_HOME        93320
#define ROOM_VNUM_LICH_HOME       93360
#define ROOM_VNUM_SHIFTER_HOME    93312
#define ROOM_VNUM_DROID_HOME      93350
#define ROOM_VNUM_ANGEL_HOME      93340
#define ROOM_VNUM_UK_HOME         93300
#define ROOM_VNUM_TAN_HOME        93330
#define ROOM_VNUM_VAMP_HOME       93450
#define ROOM_VNUM_WW_HOME         93430
#define ROOM_VNUM_DEMON_HOME      93420
#define ROOM_VNUM_NINJA_HOME      93460
#define ROOM_VNUM_MONK_HOME       93410
#define ROOM_VNUM_SKY_HOME        93480
#define ROOM_VNUM_DRAC_HOME       93500
#define ROOM_VNUM_GHOUL_HOME      93490
#define ROOM_VNUM_THIEF_HOME      93510
#define ROOM_VNUM_WIZARD_HOME     93520
#define ROOM_VNUM_MAGE_HOME       93400
#define ROOM_VNUM_HOBBIT_HOME     93590
#define ROOM_VNUM_DROW_HOME       93440
#define ROOM_VNUM_SHADOW_HOME     93540
#define ROOM_VNUM_PRIEST_HOME     93550
#define ROOM_VNUM_JEDI_HOME       93570
#define ROOM_VNUM_FAE_HOME        93580
#define ROOM_VNUM_DRONE_HOME      93530
#define ROOM_VNUM_GIANT_HOME      93560


#define ROOM_VNUM_REL_HALL1       99091
#define ROOM_VNUM_REL_HALL2       99092
#define ROOM_VNUM_REL_HALL3       99093

/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK		      1
#define ROOM_NO_OTRANS		      2
#define ROOM_NO_MOB		      4
#define ROOM_INDOORS		      8
#define ROOM_SEX		     16
#define ROOM_HOME                    32
#define ROOM_PRIVATE		    512
#define ROOM_SAFE		   1024
#define ROOM_SOLITARY		   2048
#define ROOM_BANK		   4096
#define ROOM_NO_RECALL		   8192
#define ROOM_NO_TELEPORT	  16384
#define ROOM_TOTAL_DARKNESS       32768
#define ROOM_BLADE_BARRIER        65536
#define ROOM_ARENA               131072
#define ROOM_FLAMING             262144
#define ROOM_SILENCE             524288
#define ROOM_ASTRAL		1048576
#define ROOM_PROTOTYPE		2097152
#define ROOM_ORDER		4194304
#define	ROOM_NO_CHANT           8388608 
#define ROOM_LIGHT             16777216

/*
 * Room text flags (KaVir).
 * Used in #ROOMS.
 */
#define RT_LIGHTS		      1 /* Toggles lights on/off */
#define RT_SAY			      2 /* Use this if no others powers */
#define RT_ENTER		      4
#define RT_CAST			      8
#define RT_THROWOUT		     16 /* Erm...can't remember ;) */
#define RT_OBJECT		     32 /* Creates an object */
#define RT_MOBILE		     64 /* Creates a mobile */
#define RT_LIGHT		    128 /* Lights on ONLY */
#define RT_DARK			    256 /* Lights off ONLY */
#define RT_OPEN_LIFT		    512 /* Open lift */
#define RT_CLOSE_LIFT		   1024 /* Close lift */
#define RT_MOVE_LIFT		   2048 /* Move lift */
#define RT_SPELL		   4096 /* Cast a spell */
#define RT_PORTAL		   8192 /* Creates a one-way portal */
#define RT_TELEPORT		  16384 /* Teleport player to room */
#define RT_ACTION		  32768
#define RT_BLANK_1		  65536
#define RT_BLANK_2		 131072
#define RT_TEXT                  262144 /* Send a block of text to char */
#define RT_RETURN		1048576 /* Perform once */
#define RT_PERSONAL		2097152 /* Only shows message to char */
#define RT_TIMER		4194304 /* Sets object timer to 1 tick */

/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH		      0
#define DIR_EAST		      1
#define DIR_SOUTH		      2
#define DIR_WEST		      3
#define DIR_UP			      4
#define DIR_DOWN		      5

/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		      1
#define EX_CLOSED		      2
#define EX_LOCKED		      4
#define EX_PICKPROOF		     32
#define EX_NOPASS                    64
#define EX_EASY                     128
#define EX_HARD                     256
#define EX_INFURIATING              512  
#define EX_NOCLOSE                 1024
#define EX_NOLOCK                  2048
#define EX_ICE_WALL                4096
#define EX_FIRE_WALL               8192
#define EX_SWORD_WALL             16384
#define EX_PRISMATIC_WALL         32768
#define EX_IRON_WALL              65536	
#define EX_MUSHROOM_WALL         131072
#define EX_CALTROP_WALL          262144
#define EX_ASH_WALL              524288
#define EX_WARDING              1048576

#define MAX_EXFLAG		      20
#define MAX_WALL		       8

/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE		      0
#define SECT_CITY		      1
#define SECT_FIELD		      2
#define SECT_FOREST		      3
#define SECT_HILLS		      4
#define SECT_MOUNTAIN		      5
#define SECT_WATER_SWIM		      6
#define SECT_WATER_NOSWIM	      7
#define SECT_UNUSED		      8
#define SECT_AIR		      9
#define SECT_DESERT		     10
#define SECT_MAX		     11

/*
 * Equipment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE		     -1
#define WEAR_LIGHT		      0
#define WEAR_FINGER_L		      1
#define WEAR_FINGER_R		      2
#define WEAR_NECK_1		      3
#define WEAR_NECK_2		      4
#define WEAR_BODY		      5
#define WEAR_HEAD		      6
#define WEAR_LEGS		      7
#define WEAR_FEET		      8
#define WEAR_HANDS		      9
#define WEAR_ARMS		     10
#define WEAR_SHIELD		     11
#define WEAR_ABOUT		     12
#define WEAR_WAIST		     13
#define WEAR_WRIST_L		     14
#define WEAR_WRIST_R		     15
#define WEAR_WIELD		     16
#define WEAR_HOLD		     17
#define WEAR_THIRD		     18
#define WEAR_FOURTH		     19
#define WEAR_FACE		     20
#define WEAR_SCABBARD_L		     21
#define WEAR_SCABBARD_R		     22
#define WEAR_SPECIAL                 23
#define WEAR_FLOAT                   24
#define WEAR_MEDAL                   25
#define WEAR_BODYART                 26
#define MAX_WEAR		     27

/*
 * Locations for damage.
 */
#define LOC_HEAD		      0
#define LOC_BODY		      1
#define LOC_ARM_L		      2
#define LOC_ARM_R		      3
#define LOC_LEG_L		      4
#define LOC_LEG_R		      5

/*
 * For Head
 */
#define LOST_EYE_L		       1
#define LOST_EYE_R		       2
#define LOST_EAR_L		       4
#define LOST_EAR_R		       8
#define LOST_NOSE		      16
#define BROKEN_NOSE		      32
#define BROKEN_JAW		      64
#define BROKEN_SKULL		     128
#define LOST_HEAD		     256
#define LOST_TOOTH_1		     512 /* These should be added..... */
#define LOST_TOOTH_2		    1024 /* ...together to caculate... */
#define LOST_TOOTH_4		    2048 /* ...the total number of.... */
#define LOST_TOOTH_8		    4096 /* ...teeth lost.  Total..... */
#define LOST_TOOTH_16		    8192 /* ...possible is 31 teeth.   */
#define LOST_TONGUE		   16384

/*
 * For Body
 */
#define BROKEN_RIBS_1		       1 /* Remember there are a total */
#define BROKEN_RIBS_2		       2 /* of 12 pairs of ribs in the */
#define BROKEN_RIBS_4		       4 /* human body, so not all of  */
#define BROKEN_RIBS_8		       8 /* these bits should be set   */
#define BROKEN_RIBS_16		      16 /* at the same time.          */
#define BROKEN_SPINE		      32
#define BROKEN_NECK		      64
#define CUT_THROAT		     128
#define CUT_STOMACH		     256
#define CUT_CHEST		     512

/*
 * For Arms
 */
#define BROKEN_ARM		       1
#define LOST_ARM		       2
#define LOST_HAND		       4
#define LOST_FINGER_I		       8 /* Index finger */
#define LOST_FINGER_M		      16 /* Middle finger */
#define LOST_FINGER_R		      32 /* Ring finger */
#define LOST_FINGER_L		      64 /* Little finger */
#define LOST_THUMB		     128
#define BROKEN_FINGER_I		     256 /* Index finger */
#define BROKEN_FINGER_M		     512 /* Middle finger */
#define BROKEN_FINGER_R		    1024 /* Ring finger */
#define BROKEN_FINGER_L		    2048 /* Little finger */
#define BROKEN_THUMB		    4096

/*
 * For Legs
 */
#define BROKEN_LEG		       1
#define LOST_LEG		       2
#define LOST_FOOT		       4
#define LOST_TOE_A		       8
#define LOST_TOE_B		      16
#define LOST_TOE_C		      32
#define LOST_TOE_D		      64 /* Smallest toe */
#define LOST_TOE_BIG		     128
#define BROKEN_TOE_A		     256
#define BROKEN_TOE_B		     512
#define BROKEN_TOE_C		    1024
#define BROKEN_TOE_D		    2048 /* Smallest toe */
#define BROKEN_TOE_BIG		    4096

/*
 * For Bleeding
 */
#define BLEEDING_HEAD		       1
#define BLEEDING_THROAT		       2
#define BLEEDING_ARM_L		       4
#define BLEEDING_ARM_R		       8
#define BLEEDING_HAND_L		      16
#define BLEEDING_HAND_R		      32
#define BLEEDING_LEG_L		      64
#define BLEEDING_LEG_R		     128
#define BLEEDING_FOOT_L		     256
#define BLEEDING_FOOT_R		     512


/*
 * For Spec powers on players
 */
#define EYE_SPELL		       1 /* Spell when they look at you */
#define EYE_SELFACTION		       2 /* You do action when they look */
#define EYE_ACTION		       4 /* Others do action when they look */




/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK		      0
#define COND_FULL		      1
#define COND_THIRST		      2



/*
 * Stats - KaVir.
 */
#define STAT_STR		      0
#define STAT_END		      1
#define STAT_REF		      2
#define STAT_FLE		      2



/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_MEDITATING		      5
#define POS_SITTING		      6
#define POS_RESTING		      7
#define POS_FIGHTING                  8
#define POS_STANDING                  9
/* Status of Arena */

#define FIGHT_OPEN                    0
#define FIGHT_START                   1
#define FIGHT_BUSY                    2
#define FIGHT_LOCK                    3

/* ACT bits for Autos */

#define PLR_AUTOEXIT		          1
#define PLR_AUTOLOOT		          2
#define PLR_AUTOSAC                   4
#define PLR_AUTOBONES            	  8   // Can use both hands well.
#define PLR_AUTOSLAB                 16
#define PLR_AUTOHEAD	             32
#define PLR_AUTOGEM	                 64
#define PLR_AUTOHILT	            128
#define PLR_AUTOSTORE               256
#define PLR_AUTOSPLIT               512
#define PLR_WIPED                  1024

/*
 * ACT bits for players.
 */
#define PLR_IS_NPC		              1		/* Don't EVER set.	*/
#define PLR_BRIEF3		              2
#define PLR_LEFTHAND                          4
#define PLR_BRIEF5		              8
#define PLR_BRIEF		             16
#define PLR_MAP	               		     32
#define PLR_BRIEF2	       	   	     64
#define PLR_BRIEF4                          128
#define PLR_HOLYLIGHT		            256
#define PLR_COMBINE		            512
#define PLR_PROMPT		           1024
#define PLR_TELNET_GA		           2048
#define PLR_WIZINVIS		           8192
#define PLR_ANSI		          16384
#define	PLR_SILENCE		          32768
#define PLR_AMBI			  65536
#define PLR_RIGHTHAND                    131072
#define PLR_NO_TELL		         262144
#define PLR_LOG			         524288
#define PLR_DENY		        1048576
#define PLR_FREEZE		        2097152
#define PLR_SOUND                       4194304
#define PLR_MUSIC                       8388608
#define PLR_ACID                       16777216
#define PLR_BRIEF6		       33554432
#define PLR_BRIEF7                     67108864
#define PLR_NOMUSIC                        (aa)

/*New bits for playrs (Infidel)*/
#define NEW_SLAM       1
#define NEW_QUILLS     2
#define NEW_JAWLOCK    4
#define NEW_PERCEPTION 8
#define NEW_SKIN       16
#define NEW_TIDE       32
#define NEW_COIL       64
#define NEW_REND       128
#define NEW_MONKFLAME  256
#define NEW_SCLAWS     512
#define NEW_IRONMIND   1024
#define NEW_MONKCLOAK 2048
#define NEW_MONKADAM  4096
#define NEW_MONKSKIN  8192
#define NEW_MONKFAVOR 16384
#define NEW_CLOAK     32768
#define NEW_DROWHATE  65536
#define NEW_DARKNESS  131072
#define THIRD_HAND    262144
#define FOURTH_HAND   524288
#define NEW_CLOUDBLESS 1048576
#define NEW_VISION    2097152
#define NEW_NATURAL   4194304
#define NEW_POWER     8388608
#define NEW_DFORM     16777216
#define NEW_MASTERY     33554432
#define NEW_DARKTENDRILS 67108864
#define NEW_MULTIARMS   134217728
#define NEW_BLADESPIN   268435456
#define NEW_FIGHTDANCE	536870912
#define NEW_CUBEFORM	1073741824



#define NEW2_EAGLES    1
#define NEW2_HAWKEYES  2
#define NEW2_VVIGOR    4
#define NEW2_SCALES    8
#define NEW2_ACIDBLOOD 16
#define NEW2_VIOLATION  32
#define NEW2_NEUTRAL    64
#define NEW2_REINA     128
#define NEW2_MENTALBLOCK 256

// exactly like newbits wheeeeee!
#define MORE_ANIMAL_MASTER 	1
#define MORE_SPIRIT			2
#define MORE_GUARDIAN   	4

#define      GAME_NONE       0
#define      GAME_SLOTS      1
#define      GAME_HIGH_DICE  2

void do_slots(CHAR_DATA *ch, char *argument );
void do_high_dice(CHAR_DATA *ch, char *argument);

/*
 * JFLAGS : ch->pcdata->jflags
 */
#define JFLAG_SETDECAP                1
#define JFLAG_SETLOGIN                2
#define JFLAG_SETLOGOUT               4
#define JFLAG_SETAVATAR               8
#define JFLAG_BULLY                  16
#define JFLAG_SETTIE                 32
#define JFLAG_NOSET                  64
#define JFLAG_SS1                   128
#define JFLAG_SS2                   256
#define JFLAG_SS3                   512
#define JFLAG_SS4                  1024
#define JFLAG_SS5                  2048
#define JFLAG_MADNESS              4096
#define JFLAG_POLICY               8192
#define JFLAG_POLICE              16384
#define JFLAG_WANT_KINGDOM        32768
#define JFLAG_KNOWS_FORGING       65536
#define JFLAG_DRONEWORLD         131072
#define JFLAG_DRONE_HORRID       262144
#define JFLAG_DRONE_DISTORT      524288
#define JFLAG_NOHINT		1048576
/*
 * RELPOWERS : ch->pcdata->rel_powers
 */
#define REL_DARMOR                    1
#define REL_HOLYWORD                  2
#define REL_GZONE                     4
#define REL_GAIA                      8
#define REL_THORNS                   16
#define REL_CALLWILD                 32
#define REL_DEJA                     64
#define REL_PESTILENCE              128
#define REL_MADNESS                 256
#define REL_AREAHEAL                512
#define REL_BANISHMENT             1024
#define REL_STARSIGHT              2048
#define REL_RPURGE                 4096
#define REL_DBATTLE                8192
#define REL_GFURY                 16384
#define REL_BADMOON               32768
#define REL_HDESIRE               65536
#define REL_DSCRY                131072
#define REL_SUBVERT              262144
#define REL_REBIRTH              524288
#define REL_CHAOS               1048576
#define REL_NOVA                2097152
#define REL_FARMOR              4194304

#define RELIGION_LOA                  1
#define RELIGION_STARS                2
#define RELIGION_ONE                  3
#define RELIGION_WILD                 4
#define RELIGION_GAHR                 5

/*
 * Religion Area Affects
 */
#define AREA_AFF_GZONE                1
#define AREA_AFF_THORNS               2
#define AREA_AFF_PESTILENCE           3
#define AREA_AFF_BADMOON              4
#define AREA_AFF_GAIA                 5
#define AREA_AFF_CHAOS                6

/*
 * special flags
 */

#define SPC_CHOSE_RELIGION      1       /* To set when new religion must be chosen */
#define SPC_HAS_CHOSEN_RELIGION 2       /* Have chosen the religion */
#define SPC_WOLFMAN     4
#define SPC_PRINCE      8  /* No longer used, is just kept because of old pfiles */
#define SPC_INCONNU     64
#define SPC_DROW_WAR    128
#define SPC_DROW_MAG    256
#define SPC_DROW_CLE    512
#define SPC_JEDI_FSHIELD	1024
#define SPC_JEDI_LEVITATE	2048
#define SPC_JEDI_QUICKEN	4096
#define SPC_LOG2       8192 // silent logging

/*
 * TEMPFLAGS : ch->pcdata->tempflag
 */
#define TEMP_ARTIFACT                 1
#define TEMP_VT100                    2
#define TEMP_AGGRESSIVE               4
#define TEMP_EDGE                     8

/*
 * EXTRA bits for players. (KaVir)
 */
				/*    1 */
#define EXTRA_NOAUCTION		      2
#define EXTRA_TRUSTED		      4
#define EXTRA_NEWPASS		      8
#define EXTRA_OSWITCH		     16
#define EXTRA_SWITCH		     32
#define EXTRA_FAKE_CON		     64
#define TIED_UP			    128
#define GAGGED			    256
#define BLINDFOLDED		    512
#define EXTRA_STANCE		   1024 
#define EXTRA_DONE		   2048
#define EXTRA_EXP		   4096
#define EXTRA_PREGNANT		   8192
#define EXTRA_LABOUR		  16384
#define EXTRA_BORN		  32768
#define EXTRA_PROMPT		  65536
#define EXTRA_MARRIED		 131072
#define EXTRA_AFK        	 262144
#define EXTRA_DRAGON	         524288 
#define EXTRA_CALL_ALL		1048576
#define EXTRA_CODING            2097152 /* unused */
#define EXTRA_BSD	        4194304
#define EXTRA_EARTHMELD         8388608
#define EXTRA_PLASMA           16777216
#define EXTRA_BUILDING         33554432 /* This one is free for use */
#define EXTRA_AWE              67108864
#define EXTRA_ROT             134217728
#define EXTRA_ZOMBIE          268435456
#define EXTRA_BAAL            536870912
#define EXTRA_FLASH          1073741824


/* 
* AGE Bits.
 */
#define AGE_CHILDE		      0
#define AGE_NEONATE		      1
#define AGE_ANCILLA		      2
#define AGE_ELDER		      3
#define AGE_METHUSELAH		      4
#define AGE_LA_MAGRA		      5
#define AGE_TRUEBLOOD                 6
#define BELT_ONE			7
#define BELT_TWO                        8
#define BELT_THREE                        9
#define BELT_FOUR                        10
#define BELT_FIVE                        11
#define BELT_SIX                       12
#define BELT_SEVEN                        13
#define BELT_EIGHT                      14
#define BELT_NINE                      15
#define BELT_TEN                       16

/*
 * Stances for combat
 */
#define STANCE_NONE		     -1
#define STANCE_NORMAL		      0
#define STANCE_VIPER		      1
#define STANCE_CRANE		      2
#define STANCE_CRAB		      3
#define STANCE_MONGOOSE		      4
#define STANCE_BULL		      5
#define STANCE_MANTIS		      6
#define STANCE_DRAGON		      7
#define STANCE_TIGER		      8
#define STANCE_MONKEY		      9
#define STANCE_SWALLOW		     10
#define STANCE_WOLF                  11
#define STANCE_AUTOSTANCE	     12
#define STANCE_SS1		     13
#define STANCE_SS2                   14
#define STANCE_SS3                   15
#define STANCE_SS4                   16
#define STANCE_SS5                   17

/*
 *  Bits for superstances
 */
#define STANCEPOWER_DODGE              1  /* more dodge */
#define STANCEPOWER_PARRY              2  /* more parry */
#define STANCEPOWER_SPEED              4  /* 2 extra attack */
#define STANCEPOWER_BYPASS             8  /* bypass dodge/parry */
#define STANCEPOWER_DAMAGE_1          16  /* lesser increase damage and chance to hit */
#define STANCEPOWER_DAMAGE_2          32  /* greater increase damage and chance to hit */ 
#define STANCEPOWER_DAMAGE_3          64  /* supreme increase damage and chance to hit */
#define STANCEPOWER_RESIST_1         128  /* lesser resist damage */
#define STANCEPOWER_RESIST_2         256  /* greater resist damage */
#define STANCEPOWER_RESIST_3         512  /* supreme resist damage */
#define STANCEPOWER_DAMCAP_1        1024  /* lesser damcap bonus */
#define STANCEPOWER_DAMCAP_2        2048  /* greater damcap bonus */
#define STANCEPOWER_DAMCAP_3        4096  /* supreme damcap bonus */
#define STANCEPOWER_REV_DAMCAP_1    8192  /* lesser damcap penalty for opponent */
#define STANCEPOWER_REV_DAMCAP_2   16384  /* greater damcap penalty for opponent */
#define STANCEPOWER_REV_DAMCAP_3   32768  /* supreme damcap penalty for opponent */


/*
 * Channel bits.
 */

#define CHANNEL_KINGDOM               1
#define	CHANNEL_CHAT		      2
#define CHANNEL_CLASS                 4
#define	CHANNEL_IMMTALK		      8
#define	CHANNEL_MUSIC		     16
#define CHANNEL_AUCTION              32
#define CHANNEL_HINT                 64
#define	CHANNEL_YELL		    128
#define	CHANNEL_TRIVIA		    256
#define	CHANNEL_HOWL		    512
#define	CHANNEL_LOG		   1024
#define	CHANNEL_PRAY		   2048
#define	CHANNEL_INFO		   4096
#define	CHANNEL_FLAME		   8192
#define	CHANNEL_TELL		  16384
#define	CHANNEL_RELIGION          32768
#define CHANNEL_ROLEPLAY	  65536
#define CHANNEL_NEWBIE		 131072
#define CHANNEL_QUEST		 262144
#define	CHANNEL_SOCIAL           524288


struct top_board
{
  int pkscore;
  char *name;
};

struct l_board
{
  char   * pk_name;
  char   * time_name;
  char   * quest_name;
  char   * mobkills_name;
  char   * pkscore_name;
  char   * arena_name;
  int      arena_number;
  int      pk_number;
  int      time_number;
  int      quest_number;
  int      mobkills_number;
  int      pkscore_number;
};


struct religion_table
{
  char * name;
  char * truename;
  char * channel;
  char * decapmessage;
  char * rankname[5];
};

/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct	mob_index_data
{
    MOB_INDEX_DATA *	next;
    SPEC_FUN *		spec_fun;
    QUEST_FUN *         quest_fun;
    SHOP_FUN  *         shop_fun;
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    AREA_DATA *         area;           /* OLC */
    PROG_LIST *         mprogs;
    char *		hunting;
    MOB_TRIGGER *       triggers;
    char *		player_name;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		lord;
    char *		morph;
    char *		createtime;
    char *		pload;
    char *		lasttime;
    char *		lasthost;
    char *		powertype;
    char *		poweraction;
    char *		prompt;
    char *		cprompt;
    sh_int		spectype;
    sh_int		specpower;
    int		        loc_hp	[7];
    int			vnum;
    sh_int		count;
    sh_int		killed;
    sh_int		sex;
    sh_int		mounted;
    int                 home;
    sh_int              natural_attack;
    int		        level;
    int			immune;
    int			polyaff;
    int			vampaff;
    int			itemaffect;
    int			form;
    int			act;
    int			extra;
    int                 bones;
    int	                affected_by;
    int	                affected_by2;
    int		        alignment;
    /* values 0-2 in OLC */
    int                 toughness;
    int                 dam_modifier;
    int                 extra_attack;
    long		mprog_flags;
    sh_int		default_pos;
};

struct	editor_data
{
    sh_int		numlines;
    sh_int		on_line;
    sh_int		size;
    char		line[49][81];
};

/*
 * One character (PC or NPC).
 */
struct char_data
{
    CHAR_DATA *		next;
    CHAR_DATA *		prev_in_room;
    CHAR_DATA *		next_in_room;
    CHAR_DATA *		master;
    CHAR_DATA *		leader;
    CHAR_DATA *		fighting;
    CHAR_DATA *         embracing;
    CHAR_DATA *         embraced;
    CHAR_DATA *		blinkykill;
    CHAR_DATA *		reply;
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    CHAR_DATA *         challenger; /*  person who challenged you */
    CHAR_DATA *         challenged; /*  person who you challenged */
    CHAR_DATA *         gladiator;  /*  ARENA player wagered on */
    CHAR_DATA *		mprog_target;
    SPEC_FUN *		spec_fun;
    QUEST_FUN        *  quest_fun;
    SHOP_FUN         *  shop_fun;
    MOB_INDEX_DATA *	pIndexData;
    DESCRIPTOR_DATA *	desc;
    AFFECT_DATA *	affected;
    AFFECT2_DATA *	affected2;
    OBJ_DATA *		carrying;
    OBJ_DATA *		on;
    ROOM_INDEX_DATA *	in_room;
    ROOM_INDEX_DATA *	was_in_room;
    PC_DATA *		pcdata;
    DO_FUN *		last_cmd;
    NOTE_DATA *		pnote;
    DO_FUN           *  prev_cmd;
    MOB_TRIGGER  *      triggers;
    char *		hunting;
    char *		name;
    char *	        pload;
    char *		short_descr;
    char *		long_descr;
    char *      	long_descr_orig;
    char *		description;
    char *		lord;
    char *		morph;
    char *		createtime;
    char *		lasttime;
    char *		lasthost;
    char *		poweraction;
    char *		powertype;
    char *		prompt;
    char *		cprompt;
    char *		prefix;
    bool        	deathmatch;
    bool        	tracking;
    sh_int		mprog_delay;
    sh_int		sex;
    sh_int      	amount_attacks_recieved;
    sh_int      	amount_attacks_dealt;
    sh_int      	decay_pulse;
    sh_int      	hunt_pointer;
    sh_int      	hunt_playerid;
    sh_int		agg;
    int			tier;
    int			tks;
    int         	amount_damage_recieved;
    int         	amount_damage_dealt;
    int                 total_exp_gained;
    int                 total_disc_gained;
    int                 total_cp_gained;
    int                 total_kills;
    int			class;
    int			tells;
    sh_int              invis_level;
    sh_int              incog_level;
    sh_int              ghost_level;
    sh_int      	race;
    sh_int		default_pos;
    int			immune;
    int 		polyaff;
    int			vampaff_a;
    int			fight_timer;
    int			itemaffect;
    int			form;
    int			warp;
    int			explevel;
    int			expgained;
    int                 talkcolor;
    int         	power[MAX_DISCIPLINES];
/* SMAUUUUUUUUUUUG */
    void *		dest_buf;
    void *		spare_ptr;
    int			tempnum;
    EDITOR_DATA *	editor;
    sh_int		substate;
    int			pagelen;                        /* BUILD INTERFACE */
    sh_int		inter_page;                     /* BUILD INTERFACE */
    sh_int		inter_type;                     /* BUILD INTERFACE */
    char  		*inter_editing;                 /* BUILD INTERFACE */
    int			inter_editing_vnum;             /* BUILD INTERFACE */
    sh_int		inter_substate;                 /* BUILD INTERFACE */
    int			cclan;
    int			flag2;
    int			flag3;
    int			flag4;
    sh_int 	        generation;
    int			genexp;
    sh_int		primary;
    sh_int		proper_size;
    sh_int		size;
    sh_int		cur_form;
    sh_int	        dragtype;
    sh_int		rage;
    sh_int		siltol;
    sh_int		tick_timer[MAX_TIMER];
    sh_int		warpcount;
    sh_int		vampgen_a;
    sh_int		spectype;  
    sh_int		specpower;
    sh_int		loc_hp	[7];
    sh_int		wpn	[13];
    sh_int		spl	[8];
    sh_int		cmbt	[8];
    int  		stance	[24];
    sh_int		beast;
    sh_int		mounted;
    int			mflags;
    int   		more;
    sh_int		spheres[10];
    sh_int		imms[3];
    int		        home;
    int		        level;
    int			majesty_timer;
    int         	obeah_timer;
    int			reina_timer;
    int			cobra_timer;
    int			wyld_timer;
    sh_int		trust;
    int			played;
    time_t		logon;
    time_t		save_time;
    sh_int		timer;
    sh_int		wait;
    sh_int		wait2;
    int			pkill;
    int			pdeath;
    int			mkill;
    int			mdeath;
    int		        hit;
    int		        max_hit;
    int			mana;
    int			max_mana;
    int			move;
    int			max_move;
    int			exp;
    int			act;
    int         	act2;
    int			extra;
    int                 newbits;
    int                 newbits2;
    int			sohbits;
    int                 special;
    int 		affected_by;
    int 		affected_by2;
    int			position;
    int			practice;
    int			carry_weight;
    int			carry_number;
    int			saving_throw;
    int			alignment;
    int			hitroll;
    int			damroll;
    int			armor;
    int			wimpy;
    int			deaf;
    int			paradox		[3];
    int			damcap		[2];
    int			monkstuff;
    int			monkcrap;
    int			monkab [4];
    sh_int		chi [2];
    char *		clan;
    int			garou1;
    sh_int		gnosis[2];
    CHAR_DATA *		unveil;
    char *		objdesc;
    sh_int		monkblock;
    sh_int		focus[2];
    int         	bones;
    long		wiznet;
};

/*
 * Data which only PC's have.
 */
struct pc_data
{
    PC_DATA *		next;
    BUFFER * 		buffer;
    CHAR_DATA *		familiar;
    CHAR_DATA *		partner;
    CHAR_DATA *		propose;
    CHAR_DATA *         pfile;
    OBJ_DATA *		chobj;
    BOARD_DATA *	board; /* The current board */
    time_t		last_note[MAX_BOARD]; /* last note for the boards */
    NOTE_DATA *		in_progress;
    ALIAS_DATA *        alias;
    QUEST_DATA *        quests;
    AREA_DATA  *        prev_area;
    HISTORY_DATA *      history;
#ifdef IMC
    IMC_CHARDATA *      imcchardata;
#endif
    char *              last_decap[2];
    char *              retaliation;
    char *		pwd;
    char *		bamfin;
    char *		bamfout;
    char *		title;
    char *		conception;
    char *		parents;
    char *		cparents;
    char *		marriage;
    char *		switchname;
    char *              decapmessage;
    char *              loginmessage;
    char *              logoutmessage;
    char *              avatarmessage;
    char               *last_global;
    char *              tiemessage;
    char *              soultarget;
    char *		host;	/* Used to tell PC last login site */
    char *		love;
    time_t              denied;
    sh_int              revision;
    sh_int              alias_count;
    sh_int              vt100_size;
    sh_int              rune_count;
    sh_int		souls;
    sh_int		voted;
    sh_int		relrank;
    sh_int              faith;
    sh_int              current_faith;
    sh_int              agg_counter;
    sh_int              bully_points;
    sh_int              bully_counter;
    int                 aggress_towards;
    int                 aggress_from;  
    int                 rel_powers;
    int                 tempflag;
    sh_int		safe_counter;
        sh_int    logout_counter;
    sh_int		perm_str;
    sh_int		perm_int;
    sh_int		perm_wis;
    sh_int		perm_dex;
    sh_int		perm_con;
    sh_int		mod_str;
    sh_int		mod_int;
    sh_int		mod_wis;
    sh_int		mod_dex;
    sh_int		mod_con;
    sh_int              time_tick;
    sh_int              log_count;
    sh_int		willpower[2];
    sh_int		resist[5];
    int           	betting_amount;
    int           	betting_char;
    int                 ignore[MAX_IGNORE];
    int           	playerid;
    int                 jflags;
    int        		bank;
    int                 membership;
    int 		questsrun;
    int			questtotal;
    int			quest;
    sh_int 		kingdom;
    sh_int		religion;
    int 		pagelen;
    int			sit_safe;
    sh_int		mortal;
    int			powers[20];
    int 		stats[12];
    int			disc_points;
    int			disc_research;
    bool                lwrgen;
    sh_int		wolf;
    sh_int		rank;
    sh_int		demonic_a;
    sh_int		stage[3];
        sh_int    language[2];
    sh_int		wolfform[2];
    int			score[6];
    sh_int		disc_a[11];
	int			genes[10];
    sh_int		fake_skill;
    sh_int		fake_stance;
    sh_int		fake_hit;
    sh_int		fake_dam;
    sh_int		fake_hp;
    sh_int		fake_mana;
    sh_int		fake_move;
    int			fake_ac;
    int			obj_vnum;
    sh_int		condition	[3];
    sh_int		learned		[MAX_SKILL];
    sh_int		stat_ability	[4];
    sh_int		stat_amount	[4];
    sh_int		stat_duration	[4];
    sh_int		exhaustion;
    sh_int      followers;
    int         awins;     /*  ARENA number of wins     */
    int         alosses;   /*  ARENA number of losses  */
    int			comm;
    int         security;       /* OLC - Builder security */
    int			bounty;


};

/*
 * Liquids.
 */
#define LIQ_WATER        0
#define LIQ_MAX		19

struct	liq_type
{
    char *	liq_name;
    char *	liq_color;
    sh_int	liq_affect[3];
};

/*
 * Extra description data for a room or object.
 */
struct	extra_descr_data
{
    EXTRA_DESCR_DATA *next;	/* Next in list                     */
    EXTRA_DESCR_DATA *prev;	/* Previous			    */
    char *keyword;              /* Keyword in look/examine          */
    char *description;          /* What to see                      */
  char *buffer1;                /* special action buffer 1          */
  char *buffer2;                /* special action buffer 2          */
  sh_int type;                  /* activation type                  */
  int vnum;                  /* action related vnum              */
  sh_int action;                /* action type                      */
};

/*
 * Prototype for an object.
 */
struct obj_index_data
{
    OBJ_INDEX_DATA *	next;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AFFECT_DATA *	first_affect;
    AFFECT_DATA *	last_affect; 
    AFFECT_DATA *	affected;
    AREA_DATA *         area;
    char *		name;
    char *		short_descr;
    char *		description;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    int			vnum;
    sh_int		item_type;
    int		        extra_flags;
    int		        extra_flags2;
    int 		wear_flags;
    sh_int		count;
    sh_int		weight;
    sh_int		weapflags;
    int			spectype;
    int			specpower;
    sh_int		condition;
    sh_int		toughness;
    sh_int		resistance;
    int			quest;
    sh_int		points;
    int			cost;			/* Unused */
    int			value	[4];
    PROG_LIST *		oprogs;
    long		oprog_flags;
};

/*
 * One object.
 */
struct obj_data
{
    OBJ_DATA *		next;
    OBJ_DATA *		next_content;
    OBJ_DATA *		contains;
    OBJ_DATA *		in_obj;
    OBJ_DATA *          on;
    CHAR_DATA *		carried_by;
    CHAR_DATA *		chobj;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AFFECT_DATA *	first_affect;
    AFFECT_DATA *	last_affect;
    AFFECT_DATA *	affected;
    OBJ_INDEX_DATA *	pIndexData;
    ROOM_INDEX_DATA *	in_room;
    CHAR_DATA *	        oprog_target;
    sh_int		oprog_delay;
    char *		name;
    char *		short_descr;
    char *		description;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    int		        item_type;
    int                 ownerid;
    int		        extra_flags;
    int		extra_flags2;
    int		wear_flags;
    int		wear_loc;
    int		weight;
    int		weapflags;
    int			spectype;
    int			specpower;
    int		condition;
    int		toughness;
    int		resistance;
    int			quest;
    sh_int		points;
    int			cost;
    int		level;
    int		timer;
    int			value	[4];
};

/*
 * Exit data.
 */
struct	exit_data
{
    union
    {
	ROOM_INDEX_DATA *	to_room;
	sh_int			vnum;
    } u1;
    EXIT_DATA *		prev;		/* previous exit in linked list	*/
    EXIT_DATA *		next;		/* next exit in linked list	*/
    EXIT_DATA *		rexit;		/* Reverse exit pointer		*/
    ROOM_INDEX_DATA *	to_room;	/* Pointer to destination room	*/
    char *		keyword;	/* Keywords for exit or door	*/
    char *		description;	/* Description of exit		*/
    int			vnum;		/* Vnum of room exit leads to	*/
    int			rvnum;		/* Vnum of room in opposite dir	*/
    int			exit_info;	/* door states & other flags	*/
    int			key;		/* Key vnum			*/
    sh_int		vdir;		/* 0,5 N\E\S\W\U\D shit		*/
    int                 rs_flags;       /* OLC */
    int                 orig_door;      /* OLC */
    bool                color;          /* pathfinding                   */
};

/*
 * Room text checking data.
 */
typedef struct roomtext_data
{
    int				type;
    int				power;
    int				mob;
    char *			input;
    char *			output;
    char *			choutput;
    char *			name;
    struct roomtext_data	*next;
} ROOMTEXT_DATA;

struct mob_trigger
{
  int                   type;
  int                   vnum;
  char               *  keywords;
  char               *  roomOutput;
  char               *  chOutput;
  MOB_TRIGGER        *  next;
};

/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile 
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct	reset_data
{
    RESET_DATA *	next;
    char		command;
    int		arg1;
    int		arg2;
    int		arg3;
};

/*
 * Area definition.
 */
struct  area_data   
{
    AREA_DATA *         next;
  AREA_AFFECT *       affects;
    char *              name;
    int                 age;
    int                 nplayer;
    char *              filename;       /* OLC */
    char *              builders;       /* OLC - Listing of builders */
    char *              music;
    int                 security;       /* OLC - Value 0-infinity  */
    int                 lvnum;          /* OLC - Lower vnum */
    int                 uvnum;          /* OLC - Upper vnum */
    int                 vnum;           /* OLC - Area vnum  */
    int                 cvnum;
    int                 area_flags;     /* OLC */
    int                 areabits;
    bool		empty;
};

/*
 * Room type.
 */
struct	room_index_data
{
    ROOM_INDEX_DATA *	next;
    ROOM_INDEX_DATA *   next_room;
    CHAR_DATA *		people;
    CHAR_DATA *		first_person;
    CHAR_DATA *		last_person;
    OBJ_DATA *		contents;
    OBJ_DATA *		first_content;
    OBJ_DATA *		last_content;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AREA_DATA *		area;
    EXIT_DATA *		exit	  [6];
    EXIT_DATA *		first_exit;
    EXIT_DATA *		last_exit;
    ROOMTEXT_DATA *	roomtext;
    RESET_DATA *        reset_first;    /* OLC */
    RESET_DATA *        reset_last;     /* OLC */
    char *		track     [5];
    char *		name;
    char *		description;
    int			vnum;
    int		        room_flags;
    sh_int		light;
    sh_int		blood;
    sh_int		track_dir [5];
    sh_int		sector_type;
    sh_int		tick_timer[MAX_RTIMER];
    sh_int              heap_index;
    sh_int              steps;
    bool                visited;
    PROG_LIST *		rprogs;
    CHAR_DATA *		rprog_target;
    long		rprog_flags;
    sh_int		rprog_delay;
};

/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000
#define TYPE_ADD_AGG                 3000
#define TYPE_AGG 					 3000


/*
 *  Target types.
 */
#define TAR_IGNORE		    0
#define TAR_CHAR_OFFENSIVE	    1
#define TAR_CHAR_DEFENSIVE	    2
#define TAR_CHAR_SELF		    3
#define TAR_OBJ_INV		    4

#define TAR_OBJ_CHAR_DEF            5
#define TAR_OBJ_CHAR_OFF            6
#define TAR_OBJ_ROOM		    7
#define TAR_EXIT		    8
#define TAR_CHAR_WORLD		    9

#define TARGET_CHAR                 0
#define TARGET_OBJ                  1
#define TARGET_ROOM                 2
#define TARGET_NONE                 3

#define PURPLE_MAGIC		    0
#define RED_MAGIC		    1
#define BLUE_MAGIC		    2
#define GREEN_MAGIC		    3
#define YELLOW_MAGIC		    4

/*
 * Skills include spells as a particular case.
 */
struct	skill_type
{
    char *	name;			/* Name of skill		*/
    sh_int	skill_level;	        /* Level needed by class	*/
    SPELL_FUN *	spell_fun;		/* Spell pointer (for spells)	*/
    sh_int	target;			/* Legal targets		*/
    sh_int	minimum_position;	/* Position for caster / user	*/
    sh_int *	pgsn;			/* Pointer to associated gsn	*/
    sh_int	slot;			/* Slot for #OBJECT loading	*/
    sh_int	min_mana;		/* Minimum mana used		*/
    sh_int	beats;			/* Waiting time after use	*/
    char *	noun_damage;		/* Damage message		*/
    char *	msg_off;		/* Wear off message		*/
};

/*
 * These are skill_lookup return values for common skills and spells.
 */
extern sh_int		gsn_tattoo;
extern sh_int		gsn_bash;
extern sh_int           gsn_smack;
extern sh_int           gsn_thwack;
extern sh_int		gsn_telekinetic;
extern sh_int		gsn_plasma;
extern sh_int		gsn_potato;
extern sh_int		gsn_mocha;
extern sh_int		gsn_thrownpie;
extern sh_int		gsn_stuntubes;
extern sh_int		gsn_laser;
extern sh_int   gsn_stinger;
extern  sh_int    gsn_quills;
extern  sh_int    gsn_cheapshot;
extern  sh_int    gsn_shred;
extern  sh_int    gsn_heavenlyaura;
extern  sh_int    gsn_bladespin;
extern sh_int		gsn_nova;
extern  sh_int    gsn_faith;
extern   sh_int   gsn_thorns;
extern	sh_int	gsn_fiery;
extern  sh_int  gsn_hooves;
extern  sh_int	gsn_claws;
extern  sh_int    gsn_fireball;
extern  sh_int    gsn_tentacle;
extern  sh_int    gsn_lightning;
extern  sh_int    gsn_supreme;
extern  sh_int    gsn_deathaura;
extern  sh_int    gsn_lightningslash;
extern  sh_int    gsn_wrathofgod;
extern sh_int  gsn_darktendrils;
extern sh_int  gsn_mageshield;
extern sh_int  gsn_breath;
extern  sh_int  gsn_venomtong;
extern  sh_int  gsn_spiketail;
extern  sh_int  gsn_badbreath;
extern  sh_int  gsn_magma;
extern  sh_int  gsn_hellfire;
extern  sh_int  gsn_shards;
extern  sh_int  gsn_spiderform;
extern  sh_int  gsn_garotte;
extern	sh_int	gsn_backstab;
extern	sh_int	gsn_hide;
extern	sh_int	gsn_peek;
extern	sh_int	gsn_pick_lock;
extern	sh_int	gsn_sneak;
extern	sh_int	gsn_steal;
extern  sh_int	gsn_godbless;  /* Vic - Monks */
extern  sh_int	gsn_totalblind; /* Vic - Monks */
extern	sh_int	gsn_tendrils;
extern	sh_int	gsn_berserk;
extern	sh_int	gsn_punch;
extern	sh_int	gsn_headbutt;
extern  sh_int  gsn_spiket;
extern  sh_int  gsn_venomt;
extern sh_int gsn_shiroken;
extern  sh_int  gsn_blinky;
extern  sh_int  gsn_inferno;
extern  sh_int  gsn_fangs;
extern  sh_int  gsn_buffet;
extern  sh_int  gsn_rfangs;
extern	sh_int	gsn_sweep;
extern	sh_int	gsn_knee;
extern  sh_int  gsn_spinkick;
extern  sh_int  gsn_backfist;
extern  sh_int  gsn_jumpkick;
extern  sh_int  gsn_monksweep;
extern  sh_int  gsn_thrustkick;
extern  sh_int  gsn_elbow;
extern  sh_int  gsn_palmstrike;
extern  sh_int  gsn_shinkick;
extern  sh_int  gsn_lightningkick;
extern  sh_int  gsn_tornadokick;
extern	sh_int	gsn_disarm;
extern	sh_int	gsn_hurl;
extern	sh_int	gsn_kick;
extern  sh_int  gsn_chillhand;
extern sh_int gsn_circle;
extern sh_int gsn_booming;
extern	sh_int	gsn_rescue;
extern	sh_int	gsn_track;
extern	sh_int	gsn_polymorph;
extern	sh_int	gsn_web;
extern  sh_int  gsn_infirmity;
extern sh_int	gsn_drowfire;
extern	sh_int	gsn_blindness;
extern	sh_int	gsn_charm_person;
extern	sh_int	gsn_curse;
extern	sh_int	gsn_invis;
extern	sh_int	gsn_mass_invis;
extern	sh_int	gsn_poison;
extern	sh_int	gsn_sleep;
extern	sh_int	gsn_paradox;
extern sh_int gsn_spew;
extern	sh_int	gsn_darkness;
extern      sh_int      gsn_multiplearms;

/* shadow attacks */
extern sh_int gsn_knifespin;
extern sh_int gsn_wakasashislice;
extern sh_int gsn_caltrops;
extern sh_int gsn_soulreaper;
extern sh_int gsn_moonstrike;
extern sh_int gsn_shadowthrust;
extern sh_int gsn_gutcutter;
extern sh_int gsn_dirtthrow;
extern sh_int gsn_soulseeker;

/* cyberdemon attacks  */
extern sh_int gsn_steam;
extern sh_int gsn_machinegun;
extern sh_int gsn_rocket;
extern sh_int gsn_pincers;

/*
 * Utility macros.
 */
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))
#define TOGGLE_BIT(var, bit)	((var) ^= (bit))
#define IS_NULLSTR(str)         ((str) == NULL || (str)[0] == '\0')
#define ENTRE(min,num,max)      ( ((min) < (num)) && ((num) < (max)) )

#define SET_AFF2(a,b) SET_BIT( (a)->affected_by2, (b) )

/* Ensure coord is on the map */
#define BOUNDARY(x, y) (((x) < 0) || ((y) < 0) || ((x) > MAPX) || ((y) > MAPY))

#define COLOUR(ch,color,bit)    (ADD_COLOUR((ch),(bit),(color));)


/*
 * Memory allocation macros.
 */
#define CREATE(result, type, number)					\
do											\
{											\
    if (!((result) = (type *) calloc ((number), sizeof(type))))	\
    {											\
	log_string(LOG_CRIT, "Calloc failure @ \n" ); \
	abort();									\
    }											\
} while(0)

#define RECREATE(result,type,number)					\
do											\
{											\
    if (!((result) = (type *) realloc ((result), sizeof(type) * (number))))\
    {											\
	log_string(LOG_CRIT, "Realloc failure \n" ); \
	abort();									\
    }											\
} while(0)

#define DISPOSE(point) 								\
do											\
{											\
   if((point))									\
  {											\
     free((point));								\
     (point) = NULL;								\
  }											\
} while(0)

#ifdef HASHSTR
#define STRALLOC(point)		str_dup((point))
#define QUICKLINK(point)	quick_link((point))
#define QUICKMATCH(p1, p2)	(int) (p1) == (int) (p2)
#define STRFREE(point)								\
do											\
{											\
   if((point))									\
  {											\
	if( str_free((point)) == -1 )						\
       log_string(LOG_CRIT, "STRFREEing bad pointer\n"); \
    (point) = NULL;									\
  }											\
} while(0)
#else
#define STRALLOC(point)		str_dup((point))
#define QUICKLINK(point)	str_dup((point))
#define QUICKMATCH(p1, p2)	strcmp((p1), (p2)) == 0
#define STRFREE(point)		DISPOSE((point))
#endif

/* double-linked list handling macros -Thoric */

#define LINK(link, first, last, next, prev)                     	\
do                                                              	\
{                                                               	\
   if ( !(first) )								\
   {                                           				\
      (first) = (link);				                       	\
      (last) = (link);							    	\
   }											\
   else                                                      	\
      (last)->next = (link);			                       	\
   (link)->next = NULL;			                         	\
   if ((first) == (link))							\
      (link)->prev = NULL;							\
   else										\
      (link)->prev = (last);			                       	\
   (last) = (link);				                       	\
} while(0)

#define INSERT(link, insert, first, next, prev)                 \
do                                                              \
{                                                               \
   (link)->prev = (insert)->prev;			                \
   if ( !(insert)->prev )                                       \
      (first) = (link);                                         \
   else                                                         \
      (insert)->prev->next = (link);                            \
   (insert)->prev = (link);                                     \
   (link)->next = (insert);                                     \
} while(0)

#define UNLINK(link, first, last, next, prev)                   	\
do                                                              	\
{                                                               	\
	if ( !(link)->prev )							\
	{			                                    	\
         (first) = (link)->next;			                 	\
	   if ((first))							 	\
	      (first)->prev = NULL;						\
	} 										\
	else										\
	{                                                 		\
         (link)->prev->next = (link)->next;                 	\
	}										\
	if ( !(link)->next ) 							\
	{				                                    \
         (last) = (link)->prev;                 			\
	   if ((last))								\
	      (last)->next = NULL;						\
	} 										\
	else										\
	{                                                    		\
         (link)->next->prev = (link)->prev;                 	\
	}										\
} while(0)

#define CHECK_LINKS(first, last, next, prev, type)		\
do {								\
  type *ptr, *pptr = NULL;					\
  if ( !(first) && !(last) )					\
    break;							\
  if ( !(first) )						\
  {								\
    bug( "CHECK_LINKS: last with NULL first!  %s.",		\
        __STRING(first) );					\
    for ( ptr = (last); ptr->prev; ptr = ptr->prev );		\
    (first) = ptr;						\
  }								\
  else if ( !(last) )						\
  {								\
    bug( "CHECK_LINKS: first with NULL last!  %s.",		\
        __STRING(first) );					\
    for ( ptr = (first); ptr->next; ptr = ptr->next );		\
    (last) = ptr;						\
  }								\
  if ( (first) )						\
  {								\
    for ( ptr = (first); ptr; ptr = ptr->next )			\
    {								\
      if ( ptr->prev != pptr )					\
      {								\
        bug( "CHECK_LINKS(%s): %p:->prev != %p.  Fixing.",	\
            __STRING(first), ptr, pptr );			\
        ptr->prev = pptr;					\
      }								\
      if ( ptr->prev && ptr->prev->next != ptr )		\
      {								\
        bug( "CHECK_LINKS(%s): %p:->prev->next != %p.  Fixing.",\
            __STRING(first), ptr, ptr );			\
        ptr->prev->next = ptr;					\
      }								\
      pptr = ptr;						\
    }								\
    pptr = NULL;						\
  }								\
  if ( (last) )							\
  {								\
    for ( ptr = (last); ptr; ptr = ptr->prev )			\
    {								\
      if ( ptr->next != pptr )					\
      {								\
        bug( "CHECK_LINKS (%s): %p:->next != %p.  Fixing.",	\
            __STRING(first), ptr, pptr );			\
        ptr->next = pptr;					\
      }								\
      if ( ptr->next && ptr->next->prev != ptr )		\
      {								\
        bug( "CHECK_LINKS(%s): %p:->next->prev != %p.  Fixing.",\
            __STRING(first), ptr, ptr );			\
        ptr->next->prev = ptr;					\
      }								\
      pptr = ptr;						\
    }								\
  }								\
} while(0)

#define ASSIGN_GSN(gsn, skill)					\
do								\
{								\
    if ( ((gsn) = skill_lookup((skill))) == -1 )		\
	fprintf( stderr, "ASSIGN_GSN: Skill %s not found.\n",	\
		(skill) );					\
} while(0)

#define CHECK_SUBRESTRICTED(ch)					\
do								\
{								\
    if ( (ch)->substate == SUB_RESTRICTED )			\
    {								\
	send_to_char( "You cannot use this command from within another command.\n\r", ch );	\
	return;							\
    }								\
} while(0)

/*
 * Character macros.
 */

#define MOUNTED(ch) \
		((!IS_NPC(ch) && ch->mount) ? ch->mount : NULL)
#define RIDDEN(ch) \
		((IS_NPC(ch) && ch->mount) ? ch->mount : NULL)
#define IS_COMB(ch, sn)		(IS_SET((ch)->monkcrap, (sn)))
#define IS_FS(ch, sn)		(IS_SET((ch)->monkstuff, (sn)))
#define IS_NEWFLAG(ch, sn)      (IS_SET((ch)->flag2, (sn)))
#define IS_CREATOR(ch)		(get_trust(ch) >= MAX_LEVEL)
#define GET_FORM(ch)            ((form_data[(ch)->cur_form].short_desc == NULL || form_data[(ch)->cur_form].short_desc[0] == '\0') ? form_data[(ch)->cur_form].name : "")
#define GET_PROPER_NAME(ch)     (IS_NPC((ch)) ? (ch)->short_descr : (ch)->pcdata->switchname)
#define GET_PC_NAME(ch)     	(IS_NPC((ch)) ? "<npc>" : (ch)->pcdata->switchname)

#define IS_NPC(ch)		(IS_SET((ch)->act, ACT_IS_NPC))
#define IS_JUDGE(ch)		(get_trust(ch) >= LEVEL_JUDGE)
#define IS_IMMORTAL(ch)		(get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_HERO(ch)		(get_trust(ch) >= LEVEL_HERO)

#define IS_DAY()	    	((weather_info.sunlight == SUN_RISE || weather_info.sunlight == SUN_LIGHT))
#define IS_NIGHT()	    	((!IS_DAY()))

#define CAN_PK(ch)		(get_trust(ch)>= 3 && get_trust(ch)<= 12)
#define IS_AFFECTED(ch, sn)	(IS_SET((ch)->affected_by, (sn)))
#define IS_AFFECTED2(ch, sn)	(IS_SET((ch)->affected_by2, (sn)))
#define IS_AFF2(ch, sn)		(IS_SET((ch)->affected_by2, (sn)))
#define IS_SPEAKING(ch, sn)	(IS_SET((ch)->pcdata->language[0], (sn)))
#define CAN_SPEAK(ch, sn)	(IS_SET((ch)->pcdata->language[1], (sn)))
#define IS_ITEMAFF(ch, sn)	(IS_SET((ch)->itemaffect, (sn)))
#define IS_IMMUNE(ch, sn)	(IS_SET((ch)->immune, (sn)))
#define IS_TRUSTED(ch,level)	(get_trust((ch)) >= (level))
#define IS_VAMPAFF(ch, sn)	(IS_SET((ch)->pcdata->stats[UNI_AFF], (sn)))
#define IS_VAMPPASS(ch, sn)	(IS_SET((ch)->pcdata->stats[UNI_CURRENT], (sn))) 
#define IS_FORM(ch, sn)		(IS_SET((ch)->form, (sn)))
#define IS_POLYAFF(ch, sn)	(IS_SET((ch)->polyaff, (sn)))
#define IS_EXTRA(ch, sn)	(IS_SET((ch)->extra, (sn)))
#define IS_MORE(ch, sn)		(IS_SET((ch)->more, (sn)))
#define IS_WILLPOWER(ch, sn) (IS_SET((ch)->pcdata->resist[0], (sn)))
#define IS_STANCE(ch, sn)	(ch->stance[0] == sn)
#define IS_DEMPOWER(ch, sn)	(IS_SET((ch)->pcdata->powers[DPOWER_FLAGS], (sn)))
#define IS_DEMAFF(ch, sn)	(IS_SET((ch)->pcdata->powers[DPOWER_CURRENT], (sn))) 
#define IS_CLASS(ch, CLASS)	(IS_SET((ch)->class, CLASS) && (ch->level >= LEVEL_AVATAR))
#define IS_HEAD(ch, sn)		(IS_SET((ch)->loc_hp[0], (sn)))
#define IS_BODY(ch, sn)		(IS_SET((ch)->loc_hp[1], (sn)))
#define IS_ARM_L(ch, sn)	(IS_SET((ch)->loc_hp[2], (sn)))
#define IS_ARM_R(ch, sn)	(IS_SET((ch)->loc_hp[3], (sn)))
//#define IS_ARM_T(ch, sn)	(IS_SET((ch)->loc_hp[7], (sn)))
//#define IS_ARM_F(ch, sn)	(IS_SET((ch)->loc_hp[8], (sn)))
#define IS_LEG_L(ch, sn)	(IS_SET((ch)->loc_hp[4], (sn)))
#define IS_LEG_R(ch, sn)	(IS_SET((ch)->loc_hp[5], (sn)))
#define IS_BLEEDING(ch, sn)	(IS_SET((ch)->loc_hp[6], (sn)))
#define STR(dat, field)         (( (dat)->field != NULL                    \
                                             ? (dat)->field                \
                                             : (dat)->pIndexData->field ))
#define IS_PLAYING( d )         (d->connected==CON_PLAYING)
#define PCF(ch,field)            ((ch)->pcdata->field)	/* OLC */
#define CH(descriptor)  ((descriptor)->original ? \
(descriptor)->original : (descriptor)->character)
#define IS_GOOD(ch)		(ch->alignment >= 1)
#define IS_EVIL(ch)		(ch->alignment <= -1)
#define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))

#define IS_AWAKE(ch)		(ch->position > POS_SLEEPING)
#define GET_AC(ch)		((ch)->armor				    \
				    + ( IS_AWAKE(ch)			    \
				    ? dex_app[get_curr_dex(ch)].defensive   \
				    : 0 ))
#define GET_HITROLL(ch)		((ch)->hitroll+str_app[get_curr_str(ch)].tohit)
#define GET_DAMROLL(ch)		((ch)->damroll+str_app[get_curr_str(ch)].todam)

#define IS_OUTSIDE(ch)		(!IS_SET(				    \
				    (ch)->in_room->room_flags,		    \
				    ROOM_INDOORS))

#define WAIT_STATE(ch, npulse)	\
if (!IS_IMMORTAL((ch))) \
((ch)->wait = UMAX((ch)->wait, (npulse)))

/*
 * Object Macros.
 */
#define CAN_WEAR(obj, part)	(IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(IS_SET((obj)->extra_flags, (stat)))
#define IS_OBJ_STAT2(obj, stat) (IS_SET((obj)->extra_flags2,(stat)))
#define IS_WEAP(obj, stat)      (IS_SET((obj)->weapflags,  (stat)))
#define IS_ARTIFACT( obj )      (IS_SET((obj)->quest, QUEST_ARTIFACT) )
/*
 * Description macros.
 */
//#define PERS(ch, looker)	( can_see( looker, (ch) ) ?		
//				( IS_NPC(ch) ? (ch)->short_descr	
//                                : ( IS_AFFECTED( (ch), AFF_POLYMORPH) ? 
//				(ch)->morph : (ch)->name ) )		
//				: "someone" )
char *PERS                     args( (CHAR_DATA *ch, CHAR_DATA *looker) );
#define NAME(ch)		( IS_NPC(ch) ? (ch)->short_descr : (ch)->name ) 
/*
 * J.O.P.E.
 */
struct jope_type
{
  char *const name;
  DO_FUN *do_fun;
  sh_int level;
};

/*
 * Structure for a command in the command lookup table.
 */
struct cmd_type
{
  char   * const name;
  DO_FUN * do_fun;
  sh_int   position;
  sh_int   level;
  sh_int   log;
    int                 race; /* 0 = all, other = specific race */
    sh_int		discipline; /* USE THE DISC_VAMP_???? etc.... */
    sh_int	       	disclevel;  /* level in disc the command is granted */
};

/*
 * Structure for a social in the socials table.
 */
struct	social_type
{
  char * name;
  char * char_no_arg;
  char * others_no_arg;
  char * char_found;
  char * others_found;
  char * vict_found;
  char * char_auto;
  char * others_auto;
};

struct wiznet_type
{
   char *name;
   long flag;
   int level;
};

/*
 * Structure for an X-social in the socials table.
 */
struct	xsocial_type
{
    char * const	name;
    char * const	char_no_arg;
    char * const	others_no_arg;
    char * const	char_found;
    char * const	others_found;
    char * const	vict_found;
    char * const	char_auto;
    char * const	others_auto;
    sh_int		gender;
    sh_int		stage;
    sh_int		position;
    sh_int		self;
    sh_int		other;
    sh_int              extra;
    bool		chance;
};



/*
 * Global constants.
 */

extern	const	struct	str_app_type	str_app		[36];
extern	const	struct	int_app_type	int_app		[36];
extern	const	struct	wis_app_type	wis_app		[36];
extern	const	struct	dex_app_type	dex_app		[36];
extern	const	struct	con_app_type	con_app		[36];

extern  const   struct  jope_type       jope_table      [];
extern  const   struct  cmd_type        cmd_table[];
extern	const	struct	liq_type		liq_table	[LIQ_MAX];
extern  const   struct  wiznet_type     wiznet_table    [];
extern	const	struct	skill_type		skill_table	[MAX_SKILL];
extern  const   char    *		discipline	[MAX_DISCIPLINES];
extern	const	struct	xsocial_type	xsocial_table	[];

struct item_type
{
    int         type;
    char *      name;
};

struct position_type
{
    char *name;
    char *short_name;
};

/* social table */
extern struct social_type *social_table;  
extern  const   struct  item_type       item_table      [];
extern  const   struct  position_type   position_table[];


/*
 * Global variables.
 */
extern		HELP_DATA	  *	help_first;
extern		HELP_DATA	  *	help_last;
extern		HELP_DATA	  *	first_help;
extern		HELP_DATA	  *	last_help;
extern		BAN_DATA	  *	ban_list;
extern   BAN_DATA          *  ban_free;
extern   QUEST_DATA        *  quest_free;
extern          HISTORY_DATA      *     history_free;
extern   BAN_DATA          *  newbieban_list;
extern   BAN_DATA          *  newbieban_free;
extern		CHAR_DATA	  *	char_list;
extern		DESCRIPTOR_DATA   *	descriptor_list;
extern		NOTE_DATA	  *	note_list;
extern		OBJ_DATA	  *	object_list;
extern          ROOM_INDEX_DATA   *     room_list;
extern		AFFECT_DATA	  *	affect_free;
extern		AFFECT2_DATA	  *	affect2_free;
extern          ALIAS_DATA        *     alias_free;
extern          MOB_TRIGGER       *     trigger_free;
extern          DUMMY_ARG         *     dummy_free;
extern          DUMMY_ARG         *     dummy_list;
extern          AREA_AFFECT       *     area_affect_free;
extern		CHAR_DATA	  *	char_free;
extern		DESCRIPTOR_DATA	  *	descriptor_free;
extern		EXTRA_DESCR_DATA  *	extra_descr_free;
extern		ROOMTEXT_DATA     *	roomtext_free;
extern		NOTE_DATA	  *	note_free;
extern		OBJ_DATA	  *	obj_free;
extern		PC_DATA		  *	pcdata_free;
extern		char			bug_buf		[];
extern		time_t			current_time;
extern		bool			fLogAll;
extern      bool 			wizlock;
extern		KILL_DATA		kill_table	[];
extern      FILE *			fpReserve;
extern		char			log_buf		[];
extern		TIME_INFO_DATA		time_info;
extern		WEATHER_DATA		weather_info;
extern      bool arena2;
extern      bool 			arena2death;
extern          bool                    tourney;
extern		bool			global_exp;
extern		bool			arena_open;
extern      bool            arena_base;
extern		bool			global_qp;
extern		bool			global_dt;
extern		bool			global_cp;
extern		bool			global_chaos;
extern		bool			extra_log;
extern          bool                    cmd_done;
extern		int			players_logged;
extern          int                     players_decap;
extern          int                     players_gstolen;
extern          int                     thread_count;
extern		int			iDelete;
extern		bool			arenaopen;
extern          bool                    ragnarok;
extern          int                     ragnarok_timer;
extern          int                     ragnarok_cost;
extern          int						pulse_arena;
extern          int                     total_output;
extern   		int 					mudinfo[MUDINFO_MAX];
extern   		int 					ccenter[CCENTER_MAX];
extern   		int 					top_playerid;
extern          int                     pulse_exp;
extern			int						artimkills;
extern          int                     pulse_dt;
extern          int                     pulse_cp;
extern          int                     exp_cost;
extern          int                     dt_cost;
extern          int                     cp_cost;
extern          pthread_mutex_t         memory_mutex;
extern         bool                    MOBtrigger;


/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if	defined(_AIX)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(apollo)
int	atoi		args( ( const char *string ) );
void *	calloc		args( ( unsigned nelem, size_t size ) );
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(hpux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(interactive)
#endif

#if	defined(linux)
char *crypt args((const char *key, const char *salt));
#endif

#if	defined(macintosh) || defined(WIN32)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(MIPS_OS)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(MSDOS)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(NeXT)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(sequent)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(sun)
char *crypt args((const char *key, const char *salt));
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );

#if 	defined(SYSV)
size_t fread args((void *ptr, size_t size, size_t n, FILE * stream));
#else
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
#endif
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(ultrix)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	defined(NOCRYPT)
#define crypt(s1, s2)	(s1)
#endif

/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */
#if defined(macintosh)
#define PLAYER_DIR	""		/* Player files			*/
#define NULL_FILE	"proto.are"	/* To reserve one stream	*/
#define BACKUP_DIR      "" /*reimburse shit -Infidel*/
#endif

#if defined(MSDOS)
#define PLAYER_DIR	""		/* Player files                 */
#define NULL_FILE	"nul"		/* To reserve one stream	*/
#endif

#if defined( WIN32 )
#define PLAYER_DIR       "..\\player\\"	/* Player files                 */
#define NULL_FILE        "nul"		/* To reserve one stream        */
#define BACKUP_DIR	"..\\backup\\"
#endif

#if defined(unix)
#define PLAYER_DIR	"../player/"	/* Player files			*/
#define BACKUP_DIR      "../player/backup/" /*reimb shit*/
#define SYSTEM_DIR		"../txt/"

#define NULL_FILE	"/dev/null"	/* To reserve one stream	*/
#endif

#define END_MARKER      "END"                      /* All sorts of load functions */
#define DISABLED_FILE   SYSTEM_DIR 	"disabled.txt"      /* disabled commands           */
#define AREA_LIST	SYSTEM_DIR	"area.lst"          /* List of areas               */
#define BAN_LIST	SYSTEM_DIR	"ban.txt"           /* ban                         */
#define NEWBIEBAN_LIST  SYSTEM_DIR	"newban.txt"        /* Newbie ban                  */
#define BUG_FILE	SYSTEM_DIR	"bugs.txt"          /* For 'bug' and bug( )		*/
#define SHUTDOWN_FILE	SYSTEM_DIR	"shutdown.txt"      /* For 'shutdown'              */
#define CRASH_TEMP_FILE SYSTEM_DIR	"crashtmp.txt"      /* Need it for crash-recover */
#define PROJECTS_FILE   SYSTEM_DIR	"projects.txt"
#define CHANGES_FILE    SYSTEM_DIR	"changes.dat"
#define VOTE_FILE       SYSTEM_DIR	"vote.txt"
#define WAR_FILE        SYSTEM_DIR	"war.txt"
#define WIZ_FILE	SYSTEM_DIR	"wizlist.txt"
#define CHAT_FILE	SYSTEM_DIR      "chat.data"
#define MUSIC_FILE	SYSTEM_DIR      "music.txt"
#define HINT_FILE	SYSTEM_DIR 	"hints.txt" /* for Hints code */
/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN
#define QF      QUEST_FUN
#define ED	EXIT_DATA
#define PC	PROG_CODE

/* act_comm.c */
char     *darktongue args((CHAR_DATA * ch, char *argument));
char     *oldelang args((CHAR_DATA * ch, char *argument));
char     *beasttalk args((CHAR_DATA * ch, char *argument));
char     *badlang args((CHAR_DATA * ch, char *argument));
void	add_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void  stop_follower     args (( CHAR_DATA * ch, bool isDead ));
void	die_follower	args( ( CHAR_DATA *ch ) );
bool	is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
void	room_text	args( ( CHAR_DATA *ch, char *argument ) );
char    *strlower       args( ( char * ip ) );
void	excessive_cpu	args( ( int blx ) );
bool	check_parse_name args( ( char *name ) );
void    ask_quest_question( CHAR_DATA *ch, char *argument );
void    room_message    args( ( ROOM_INDEX_DATA *room, char *message ) );

/* act_info.c */
bool    check_blind             args( ( CHAR_DATA *ch ) );
void	set_title	args( ( CHAR_DATA *ch, char *title ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
void    show_mlist_to_char      args( ( CHAR_DATA *list, CHAR_DATA *ch ) );				
int	char_hitroll	args( ( CHAR_DATA *ch ) );
int	char_damroll	args( ( CHAR_DATA *ch ) );
int	char_ac		args( ( CHAR_DATA *ch ) );

/* darkheart.c */

void    set_pc_name     args( ( CHAR_DATA *ch, char *title ) );

/* act_move.c */
void	move_char	args( ( CHAR_DATA *ch, int door ) );
void	open_lift	args( ( CHAR_DATA *ch ) );
void	close_lift	args( ( CHAR_DATA *ch ) );
void	move_lift	args( ( CHAR_DATA *ch, int to_room ) );
void	move_door	args( ( CHAR_DATA *ch ) );
void	thru_door	args( ( CHAR_DATA *ch, int doorexit ) );
void	open_door	args( ( CHAR_DATA *ch, bool be_open ) );
bool	is_open		args( ( CHAR_DATA *ch ) );
bool	same_floor	args( ( CHAR_DATA *ch, int cmp_room ) );
void	check_hunt	args( ( CHAR_DATA *ch ) );
int     figure_stat_cost args( ( const int mStat ) );

int   disc_points_needed args( (CHAR_DATA *ch) );
void  gain_disc_points  args( (CHAR_DATA *ch, int points ) );
RID  *get_random_room   args ( (CHAR_DATA *ch) );
RID  *get_rand_room   args (( void ));
RID      *get_treemeld_room args((void));
RID  *get_rand_room_by_sect   args ( (int sect ) );


/* act_obj.c */
bool	is_ok_to_wear	args( ( CHAR_DATA *ch, bool wolf_ok, char *argument ) );
void	quest_object	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	remove_obj	args( ( CHAR_DATA *ch, int iWear, bool fReplace ) );
void	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );
void    call_all         args (( CHAR_DATA * ch ));

/* act_wiz.c */
void   	get_reboot_string args( ( void ) );
void	bind_char	args( ( CHAR_DATA *ch ) );
void	logchan		args( ( char *argument ) );
void    do_ainfo    args( ( CHAR_DATA *ch, char *argument ) );

/* build.c */
ED *	get_exit	args( ( ROOM_INDEX_DATA *room, sh_int dir ) );
ED *	get_exit_to	args( ( ROOM_INDEX_DATA *room, sh_int dir,int vnum ) );
ED *	get_exit_num	args( ( ROOM_INDEX_DATA *room, sh_int count ) );
char *	copy_buffer	args( ( CHAR_DATA *ch ) );
void	edit_buffer	args( ( CHAR_DATA *ch, char *argument ) );
char *	strip_cr	args( ( char *str  ) );
void    start_editing   args( ( CHAR_DATA *ch, char *data ) );  
void    stop_editing    args( ( CHAR_DATA *ch ) );

/* clan.c */
void    do_clearcrap    args( ( CHAR_DATA *ch, char *argument ) );

/* wizlist.c */
void	update_wizlist	args( ( CHAR_DATA *ch, int level ) );

/* comm.c */
char *  mc_crypt		args( ( const char *pwd ) );
void    xlogf           args (( char *fmt, ... ));
void	close_socket	args( ( DESCRIPTOR_DATA *dclose ) );
void	close_socket2	args( ( DESCRIPTOR_DATA *dclose, bool kickoff ) );
void	write_to_buffer	args( ( DESCRIPTOR_DATA *d, const char *txt, int length ) );
void	send_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void    pager_to_char   args( ( const char *txt, CHAR_DATA * ch ) );
void	act		        args( ( const char *format, CHAR_DATA *ch, void *arg1, void *arg2, int type ) );
void	act2			args( ( const char *format, CHAR_DATA *ch, void *arg1, void *arg2, int type ) );
void    act_brief       args( ( const char *format, CHAR_DATA *ch, void *arg1, void *arg2, int type, int iBrief ) );
void	kavitem			args( ( const char *format, CHAR_DATA *ch, void *arg1, void *arg2, int type ) );
void  printf_to_char         args (( CHAR_DATA *ch, char *fmt, ... ));
bool  write_to_descriptor    args (( DESCRIPTOR_DATA *d, char *txt, int length ));
bool  write_to_descriptor_2  args (( int desc, char *txt, int length ));

void    banner_to_char  args( ( char *txt, CHAR_DATA *ch ) );
void    banner2_to_char args( ( char *txt, CHAR_DATA *ch ) );
void    divide_to_char  args( ( CHAR_DATA *ch ) );
void    divide2_to_char args( ( CHAR_DATA *ch ) );
void    divide3_to_char args( ( CHAR_DATA *ch ) );
void    divide4_to_char args( ( CHAR_DATA *ch ) );
void    divide5_to_char args( ( CHAR_DATA *ch ) );
void    divide6_to_char args( ( CHAR_DATA *ch ) );
void    stc	        args( ( const char *txt, CHAR_DATA *ch ) );
void    sta             args( ( const char *txt,AREA_DATA *area) );
void    cent_to_char    args( ( char *txt, CHAR_DATA *ch ) );
void    ch_printf       args( ( CHAR_DATA *ch, char *fmt, ... ) );



/* prototypes from db.c */
void  load_disabled   args( ( void ) );
void  save_disabled   args( ( void ) );
PC	*   get_prog_index	args( ( int vnum, int type ) );

/* prototypes from social-edit.c */
void  load_social_table   args (( void ));
void  save_social_table   args (( void ));

/* db.c */
void    randomize_object args( (int vnum) );
char *fread_string_eol args( ( FILE * fp ) );
void	boot_db		args( (bool fCopyOver ) );
void	area_update	args( ( void ) );
CD *	create_mobile	args( ( MOB_INDEX_DATA *pMobIndex ) );
OD *	create_object	args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
void	clear_char	args( ( CHAR_DATA *ch ) );
void	free_char	args( ( CHAR_DATA *ch ) );
char *	get_extra_descr	args( ( char *name, EXTRA_DESCR_DATA *ed ) );
char *	get_roomtext	args( ( const char *name, ROOMTEXT_DATA    *rt ) );
MID *	get_mob_index	args( ( int vnum ) );
OID *	get_obj_index	args( ( int vnum ) );
RID *	get_room_index	args( ( int vnum ) );
char	fread_letter	args( ( FILE *fp ) );
int	fread_number	args( ( FILE *fp ) );
char *	fread_string	args( ( FILE *fp ) );
void	fread_to_eol	args( ( FILE *fp ) );
char *	fread_word	args( ( FILE *fp ) );
void *	alloc_mem	args( ( int sMem ) );
void *	alloc_perm	args( ( int sMem ) );
void	free_mem	args( ( void *pMem, int sMem ) );
char *	str_dup		args( ( const char *str ) );
void	free_string	args( ( char *pstr ) );
int	number_fuzzy	args( ( int number ) );
int	number_range	args( ( int from, int to ) );
int	number_percent	args( ( void ) );
int	number_door	args( ( void ) );
int	number_bits	args( ( int width ) );
int	number_mm	args( ( void ) );
int	dice		args( ( int number, int size ) );
int	interpolate	args( ( int level, int value_00, int value_32 ) );
void	smash_tilde	args( ( char *str ) );
bool	str_cmp		args( ( const char *astr, const char *bstr ) );
bool	str_prefix	args( ( const char *astr, const char *bstr ) );
bool	str_infix	args( ( const char *astr, const char *bstr ) );
bool	str_suffix	args( ( const char *astr, const char *bstr ) );
char *	capitalize	args( ( const char *str ) );
void	append_file	args( ( CHAR_DATA *ch, char *file, char *str ) );
void	bug		args( ( const char *str, int param ) );
//void	log_string	args( ( const char *str ) );
void	tail_chain	args( ( void ) );
void	add_help	args( ( HELP_DATA *pHelp ) );
char *	strupper	args( ( const char *str ) );
void  bugf	args (( char *fmt, ... ));

void draconian_death     args (( CHAR_DATA *ch, CHAR_DATA *victim ));
/* arena.c */
void  close_arena        args (( void ));
void  open_arena         args (( void ));
void  update_bets        args (( CHAR_DATA * ch, CHAR_DATA * victim, bool payoff ));

/* wizutils.c */
void  aggr_test          args (( CHAR_DATA * ch ));
void  set_switchname     args (( CHAR_DATA * ch, char *title ));

/* religion.c */
void  check_faith               args (( CHAR_DATA *ch ));
bool  rel_oppose                args (( CHAR_DATA *ch, CHAR_DATA *victim ));

/* jobo_fight.c */
void  fortresskill       args (( CHAR_DATA * ch, CHAR_DATA * victim ));
void  ragnarokdecap      args (( CHAR_DATA * ch, CHAR_DATA * victim ));

/* desc_map.c */
void  draw_map           args (( CHAR_DATA * ch, char *desc ));
int   get_line           args (( char *desc, int max_len ));

/* jobo_comm.c */
void  delete_last_line_in_note     args (( CHAR_DATA *ch ));
void  init_tell_storage            args (( void ));
void  strip_history                args (( CHAR_DATA * ch ));
HISTORY_DATA *generate_history     args (( void ));
void  update_history               args (( CHAR_DATA * ch, CHAR_DATA * talker, char *buf, char *argument, bool tell ));
void  attach_history               args (( CHAR_DATA * ch, char *message ));
void  init_channel_storage      args (( void ));
void  update_channel_storage    args (( char *buf, int chan ));
void  showchannel               args (( CHAR_DATA *ch, int chan ));
void  talk_channel              args (( CHAR_DATA *ch, char *argument, int channel, int sub_channel, const char *verb ));

/* jobo_save.c */
void  load_newbiebans           args (( void ));
void  save_newbiebans           args (( void ));
void  load_topboard             args (( void ));
void  save_topboard             args (( void ));
void  load_leaderboard          args (( void ));
void  save_leaderboard          args (( void ));
void  load_religions            args (( void ));
void  save_religions            args (( void ));
void  load_mudinfo              args (( void ));
void  save_mudinfo              args (( void ));
void  load_ccenter              args (( void ));
void  save_ccenter              args (( void ));
void  write_mudinfo_database    args (( void ));
void  save_coreinfo             args (( void ));
void  load_coreinfo             args (( void ));
void  load_bans                 args (( void ));
void  save_bans                 args (( void ));

/* fae.c */
char *cut_after_locate          args (( char *str ));
char *cut_after_scry            args (( char *str ));
char *cut_after_tracking        args (( char *str ));

/* jobo_update.c */
void  update_trackers           args (( void ));
void  update_midi               args (( void ));
void  update_edge               args (( CHAR_DATA *ch ));

/* jobo_util.c */
char *get_exits                 args (( CHAR_DATA *ch ));
char *pathfind                  args (( CHAR_DATA *ch, CHAR_DATA *victim ));
void  strip_aggression          args (( CHAR_DATA *ch ));
void  update_aggressive_flags   args (( CHAR_DATA *ch, CHAR_DATA *victim ));
void  trigger_remove            args (( CHAR_DATA *ch, MOB_TRIGGER *mProg ));
void  mob_text                  args (( CHAR_DATA *ch, char *argument ));
bool  will_trigger              args (( char *say, char *trigger, int vnum ));
void  init_vt100                args (( DESCRIPTOR_DATA *d, char *xbuf ));
char *strip_ansi                args (( char *str ));
bool  in_arena                  args (( CHAR_DATA *ch ));
char *line_indent               args (( char *text, int wBegin, int wMax ));
void  ragnarok_stop             args (( void ));
void  dump_last_command         args (( void ));
void  update_mudinfo            args (( void ));
void  recycle_dummys            args (( void ));
bool  in_fortress               args (( CHAR_DATA * ch ));

/*
 * jobo_*.c
 */
int   get_ratio                 args (( CHAR_DATA *ch ));
void  win_prize                 args (( CHAR_DATA *ch ));
bool  is_upgrade                args (( CHAR_DATA *ch ));
bool  is_contained              args (( const char *astr, const char *bstr ));
bool  is_contained2             args (( const char *astr, const char *bstr ));
bool  is_webbed                 args (( CHAR_DATA *ch ));
void  increase_total_output     args (( int clenght ));
void  affect_to_area            args (( AREA_DATA *pArea, AREA_AFFECT *paf ));
void  area_affect_remove        args (( AREA_DATA *pArea, AREA_AFFECT *paf ));
bool  has_area_affect           args (( AREA_DATA *pArea, int affect, int religion ));
int   get_spend_qps             args (( CHAR_DATA *ch ));
void  log_string2               args (( const char *str ));
void  panic_shutdown            ( int iSignal ) __attribute__((noreturn));
void  check_help_soundex        args (( char *argument, CHAR_DATA * ch ));
void  special_decap_message     args (( CHAR_DATA *ch, CHAR_DATA *victim ));
int   strlen2                   args (( const char *b ));
int   calc_ratio                args (( int a, int b ));
void  logout_message            args (( CHAR_DATA *ch ));
void  login_message             args (( CHAR_DATA *ch ));
void  avatar_message            args (( CHAR_DATA *ch ));
void  tie_message               args (( CHAR_DATA * ch, CHAR_DATA * victim ));
void  powerdown                 args (( CHAR_DATA *ch ));
bool  multicheck                args (( CHAR_DATA *ch ));
bool  reachedDecapLimit         args (( CHAR_DATA *ch ));
void  enter_info                args (( char *str ));
void  leave_info                args (( char *str ));
void  death_info                args (( char *str ));
void  avatar_info               args (( char *str ));
int   getMight                  args (( CHAR_DATA *ch ));
void  forge_affect              args (( OBJ_DATA *obj, int value ));
void  update_revision           args (( CHAR_DATA *ch ));
void  recycle_descriptors       args (( void ));
void  update_ragnarok           args (( void ));
void  update_arena              args (( void ));
void  update_doubleexp          args (( void ));
void  update_doubleqps          args (( void ));
char *get_dystopia_banner       args (( char *title, int size ));
char *drunktalk					args (( char *argument ));
void  update_exp          		args (( void ));
void  update_dt          		args (( void ));
void  update_cp          		args (( void ));

/* jobo_quest.c */
void  quest_to_char             args (( CHAR_DATA * ch, QUEST_DATA * quest ));
void  quest_from_char           args (( CHAR_DATA * ch, QUEST_DATA * quest ));
bool  is_quest_target           args (( CHAR_DATA * ch, CHAR_DATA * victim ));
QF   *quest_lookup              args (( const char *name ));
char *quest_string              args (( QUEST_FUN * fun ));

/* auction.c */
void    auction_update  args( ( void ) );
void    auction_channel args( ( char *msg ) );

/* msp.c */
void  send_bluespell      args (( CHAR_DATA * ch ));
void  send_yellowspell    args (( CHAR_DATA * ch ));
void  send_redspell       args (( CHAR_DATA * ch ));
void  send_greenspell     args (( CHAR_DATA * ch ));
void  send_purplespell    args (( CHAR_DATA * ch ));
void  send_howl           args (( CHAR_DATA * ch ));
void  send_portal         args (( CHAR_DATA * ch ));
void  send_detonate       args (( CHAR_DATA * ch ));
void  send_losthead       args (( CHAR_DATA * ch ));
void  send_teleport       args (( CHAR_DATA * ch ));
void  send_decap          args (( CHAR_DATA * ch ));
void  send_thunder        args (( CHAR_DATA * ch ));
void  send_eagle          args (( CHAR_DATA * ch ));
void  update_music        args (( CHAR_DATA * ch ));

/* jope.c */
bool jope_load_char             args (( CHAR_DATA *ch, char *arg ));
void jope_free_char             args (( CHAR_DATA *ch ));
void jope_interp                args (( CHAR_DATA *ch, char *argument ));

/* jobo_shop.c */
SHOP_FUN *shop_lookup   args (( const char *name ));
char     *shop_string   args (( SHOP_FUN * fun ));

int    thread_status;
//char * last_command;

int      item_lookup    args( (const char *name) );
int      position_lookup    args( (const char *name) );
/* daemon.c */

ROOM_INDEX_DATA * locate_obj	args ( ( OBJ_DATA *obj ) );
void shock_effect  args( ( void *vo,int level, int dam, int target ) );
void cold_effect   args( ( void *vo,int level, int dam, int target ) );
void acid_effect   args( ( void *vo,int level, int dam, int target ) );
void fire_effect   args( ( void *vo,int level, int dam, int target ) );
void make_wall     args( ( ROOM_INDEX_DATA *room, int dir, int wall ) );

/* fight.c */
void  dropinvis            args (( CHAR_DATA * ch ));
int  cap_dam		args( ( CD *ch, CD *victim, int dam));
int   up_dam            args((CHAR_DATA *ch, CHAR_DATA *victim, int dam));
int  randomize_damage	args( ( CHAR_DATA *ch, int dam, int am ) );
void  update_damcap        args (( CHAR_DATA * ch, CHAR_DATA * victim ));


void	violence_update	args( ( void ) );
void	multi_hit	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	damage		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ) );
void	update_pos	args( ( CHAR_DATA *victim ) );
void	stop_fighting	args( ( CHAR_DATA *ch, bool fBoth ) );
void    stop_embrace    args( ( CHAR_DATA *ch, CHAR_DATA *victim));
bool	is_safe		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	hurt_person	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );
void    set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim, bool aggressive));
bool    has_timer	args( ( CHAR_DATA *ch ) );
bool    has_bad_chars   args( ( CHAR_DATA *ch, char *argument ) );
void    check_leaderboard  args( ( CHAR_DATA *ch ));
void    update_top_board   args( ( CHAR_DATA *ch ));
void    crashrecov      args( (int) );
void    iBacktrace      args( (int) );
void    retell_mccp   args( ( DESCRIPTOR_DATA *d ));
void    do_randomload      args ((CHAR_DATA *ch, char *argument));
/* handler.c */
int num_changes         args( ( void ) );
void	extract_exit	args( ( ROOM_INDEX_DATA *room, EXIT_DATA *pexit ) );
bool  char_exists             args (( bool backup, char *argument ));
int	get_trust	args( ( CHAR_DATA *ch ) );
int	get_age		args( ( CHAR_DATA *ch ) );
int	get_curr_str	args( ( CHAR_DATA *ch ) );
int	get_curr_int	args( ( CHAR_DATA *ch ) );
int	get_curr_wis	args( ( CHAR_DATA *ch ) );
int	get_curr_dex	args( ( CHAR_DATA *ch ) );
int	get_curr_con	args( ( CHAR_DATA *ch ) );
int	can_carry_n	args( ( CHAR_DATA *ch ) );
int	can_carry_w	args( ( CHAR_DATA *ch ) );
bool	is_name		args( ( char *str, char *namelist ) );
bool	is_full_name	args( ( const char *str, char *namelist ) );
void	affect_to_char	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect2_to_char	args( ( CHAR_DATA *ch, AFFECT2_DATA *paf ) );
void    affect_to_obj   args( ( OBJ_DATA *obj, AFFECT_DATA *paf ) );
void	affect_remove	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect2_remove	args( ( CHAR_DATA *ch, AFFECT2_DATA *paf ) );
void    alias_remove    args( ( CHAR_DATA *ch, ALIAS_DATA *ali ) );
void	affect_strip	args( ( CHAR_DATA *ch, int sn ) );
bool	is_affected	args( ( CHAR_DATA *ch, int sn ) );
bool	is_affected2	args( ( CHAR_DATA *ch, int sn ) );
void	affect_join	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect2_join	args( ( CHAR_DATA *ch, AFFECT2_DATA *paf ) );
void	char_from_room	args( ( CHAR_DATA *ch ) );
void	char_to_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void	obj_from_char	args( ( OBJ_DATA *obj ) );
int	apply_ac	args( ( OBJ_DATA *obj, int iWear ) );
OD *	get_eq_char	args( ( CHAR_DATA *ch, int iWear ) );
void	equip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void	unequip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
int	count_obj_list	args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
void	obj_from_room	args( ( OBJ_DATA *obj ) );
void	obj_to_room	args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_obj	args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void	obj_from_obj	args( ( OBJ_DATA *obj ) );
void	extract_obj	args( ( OBJ_DATA *obj ) );
void	extract_char	args( ( CHAR_DATA *ch, bool fPull ) );
CD *	get_char_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument ) );
CD *	get_char_room2	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument ) );
CD *	get_char_world	args( ( CHAR_DATA *ch, char *argument ) );
CD *    get_char_area   args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_type	args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *    get_obj_list    args( ( CHAR_DATA * ch, char *argument, OBJ_DATA * list ) );
OD *    get_wall_list   args( ( CHAR_DATA * ch, char *argument, OBJ_DATA * list ) );
OD *	get_obj_in_obj	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_carry   args( ( CHAR_DATA *ch, char *argument, CHAR_DATA *viewer ) );
OD *    get_obj_wear	args( ( CHAR_DATA *ch, char *argument, bool character ) );
OD *	get_obj_here	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument ) );
OD *	get_obj_room	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *    create_money    args( ( int amount ) );
int	get_obj_number	args( ( OBJ_DATA *obj ) );
int	get_obj_weight	args( ( OBJ_DATA *obj ) );
bool	room_is_dark	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	room_is_private	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	can_see		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_see_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	can_drop_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
char *	item_type_name	args( ( OBJ_DATA *obj ) );
char *	affect_loc_name	args( ( int location ) );
char *	affect_bit_name	args( ( int vector ) );
char *	quest_bit_name	args( ( int vector ) );
char *	affect2_bit_name args( ( int vector ) );
char *	extra_bit_name	args( ( int extra_flags ) );
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd, OBJ_DATA *obj ) );
bool    is_safe_room    args( ( ROOM_INDEX_DATA *pRoom ) );

void    set_learnable_disciplines args( ( CHAR_DATA *ch ) );
void    update_disc	args( ( CHAR_DATA *ch) );

/*mob_prog.c*/
void	program_flow	args( ( int vnum, char *source, CHAR_DATA *mob, 
				OBJ_DATA *obj, ROOM_INDEX_DATA *room,
				CHAR_DATA *ch, void *arg1,
				void *arg2, int numlines) );
PROG_CODE *get_mprog_by_vnum	args( (int vnum) );

void	p_act_trigger	args( ( char *argument, CHAR_DATA *mob, 
				OBJ_DATA *obj, ROOM_INDEX_DATA *room,
				CHAR_DATA *ch, void *arg1,
				void *arg2, int type ) );
bool	p_percent_trigger args( ( CHAR_DATA *mob, OBJ_DATA *obj,
				ROOM_INDEX_DATA *room, CHAR_DATA *ch, 
				void *arg1, void *arg2, int type ) );
void	p_bribe_trigger  args( ( CHAR_DATA *mob, CHAR_DATA *ch, int amount ) );
bool	p_exit_trigger   args( ( CHAR_DATA *ch, int dir, int type ) );
void	p_give_trigger   args( ( CHAR_DATA *mob, OBJ_DATA *obj, 
				ROOM_INDEX_DATA *room, CHAR_DATA *ch,
				OBJ_DATA *dropped, int type ) );
void 	p_greet_trigger  args( ( CHAR_DATA *ch, int type ) );
void	p_hprct_trigger  args( ( CHAR_DATA *mob, CHAR_DATA *ch ) );

/* mob_cmds.c */
void	mob_interpret	args( ( CHAR_DATA *ch, char *argument ) );
void	obj_interpret	args( ( OBJ_DATA *obj, char *argument ) );
void	room_interpret	args( ( ROOM_INDEX_DATA *room, char *argument ) );
void	mp_regex_trigger(char *argument, CHAR_DATA *mob, CHAR_DATA *ch, const void *arg1, const void *arg2);

/* mudmsg.c */
void    mud_message       args( ( CHAR_DATA *ch, char *arg ) );
void    mud_recv_message  args( ( void ) );

/* interp.c */
void	interpret	args( ( CHAR_DATA *ch, char *argument ) );
bool	is_number	args( ( char *arg ) );
int	number_argument	args( ( char *argument, char *arg ) );
char *	one_argument	args( ( char *argument, char *arg_first ) );
void	stage_update	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int stage, char *argument ) );
void    make_preg       args( ( CHAR_DATA *mother, CHAR_DATA *father ) );
void    do_rand_typo    args( ( CHAR_DATA *ch, char *argument ) );
void    save_war        args( ( void ) );
void    load_war        args( ( void ) );
/* magic.c */
int	skill_lookup	args( ( const char *name ) );
int	slot_lookup	args( ( int slot ) );
bool	saves_spell	args( ( int level, CHAR_DATA *victim ) );
void	obj_cast_spell	args( ( int sn, int level, CHAR_DATA *ch,
				    CHAR_DATA *victim, OBJ_DATA *obj ) );
void	enhance_stat	args( ( int sn, int level, CHAR_DATA *ch, 
				    CHAR_DATA *victim, int apply_bit,
				    int bonuses, int affect_bit ) );

/* save.c */
long	flag_convert	args( ( char letter) );
long 	fread_flag	args( ( FILE *fp ) );
void	save_char_obj		args( ( CHAR_DATA *ch ) );
bool	load_char_obj		args( ( DESCRIPTOR_DATA *d, char *name ) );
bool	load_char_short		args( ( DESCRIPTOR_DATA *d, char *name ) );

/* special.c */
SF *	spec_lookup	args( ( const char *name ) );

void centre_text            args( ( char *text, CHAR_DATA *ch ) );
  
/* mccp.c */
bool  compressStart      args (( DESCRIPTOR_DATA * desc ));
bool  compressEnd        args (( DESCRIPTOR_DATA * desc ));
bool  compressEnd2       args (( DESCRIPTOR_DATA * desc ));  // threadsafe version.
bool  processCompressed  args (( DESCRIPTOR_DATA * desc ));
bool  writeCompressed    args (( DESCRIPTOR_DATA * desc, char *txt, int length ));

#define HAS_TRIGGER_MOB(ch,trig)	(IS_SET((ch)->pIndexData->mprog_flags,(trig)))
#define HAS_TRIGGER_OBJ(obj,trig) (IS_SET((obj)->pIndexData->oprog_flags,(trig)))
#define HAS_TRIGGER_ROOM(room,trig) (IS_SET((room)->rprog_flags,(trig)))

#define IS_VALID(data)          ((data) != NULL && (data)->valid)
#define VALIDATE(data)          ((data)->valid = TRUE)
#define INVALIDATE(data)        ((data)->valid = FALSE)

/* update.c */
void	gain_exp	args( ( CHAR_DATA *ch, int gain ) );
void	gain_condition	args( ( CHAR_DATA *ch, int iCond, int value ) );
void	update_handler	args( ( void ) );
void    mobile_update   args( ( void ) );
void    weather_update  args( ( void ) );
void    char_update     args( ( void ) );
void    char_update2    args( ( void ) );
void    obj_update      args( ( void ) );
void    ww_update       args( ( void ) );
int     get_size        args( ( int val ) );
void    room_update	args( ( void ) );

/* kav_fight.c */
void	special_move	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/* kav_info.c */
void	birth_date	args( ( CHAR_DATA *ch, bool is_self ) );
void  other_age     args (( CHAR_DATA * ch, int extra, bool is_preg, char *argument ));
int	years_old	args( ( CHAR_DATA *ch ) );

/* kav_wiz.c */
void oset_affect args( ( CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest) );

/* clan.c */
void	reg_mend	args( ( CHAR_DATA *ch ) );
void	vamp_rage	args( ( CHAR_DATA *ch ) );
OD *	get_page	args( ( OBJ_DATA *book, int page_num ) );

/* update.c */
void sleep_update       args( (void) );

/* vic.c */
DECLARE_DO_FUN( do_relevel2 );
void reset_weapon   args( (CHAR_DATA *ch, int dtype) );
void reset_spell    args( (CHAR_DATA *ch, int dtype) );

/* zarius.c */
char *centerjob     args( (char *fill, const char *txt, int maxlen ) );
char *bash_color    args( ( const char *txt ) );
char *pcolor        args( ( CHAR_DATA * ch, const char *s, int tcolor ) );
#define xprintf(var,args...)   safe_printf (__FILE__, __FUNCTION__, __LINE__, sizeof(var), (var), ##args )
#define xprintf_2(var,args...) safe_printf (__FILE__, __FUNCTION__, __LINE__, MIL, (var), ##args)
void safe_strcatf( const char *file, const char *function, int line, int size, char *prev, char *next, ... );
# define xcatf(var, args...)    safe_strcatf(__FILE__,__FUNCTION__,__LINE__,sizeof(var), (var),##args )
# define xcatf_2(var, args...)    safe_strcatf(__FILE__,__FUNCTION__,__LINE__,MAS, (var),##args )

/* zarius_hint.c */
char *get_hint          args ( ( int level ) );

/* zarius_util.c */
void load_arti args( ( void ) );
char *MudBackTrace( void );
//size_t __mudstrlcpy( char *dst, const char *src, size_t siz, const char *dafile, const char *dafunction, int line );
//size_t __mudstrlcat( char *dst, const char *src, size_t siz, const char *dafile, const char *dafunction, int line );
//#define mudstrlcpy( dst, src, siz) __mudstrlcpy(dst, src, siz, __FILE__, __FUNCTION__, __LINE__)
//#define mudstrlcat( dst, src, siz) __mudstrlcat(dst, src, siz, __FILE__, __FUNCTION__, __LINE__)
size_t mudstrlcpy( char *dst, const char *src, size_t siz );
size_t mudstrlcat( char *dst, const char *src, size_t siz );
void log_string args( ( int type, const char *fmt, ... ) );
void wiznet_printf( CHAR_DATA * ch, OBJ_DATA * obj, long flag, long flag_skip, int min_level, char *format, ... );
void wiznet args( ( char *string, CHAR_DATA * ch, OBJ_DATA * obj, long flag, long flag_skip, int min_level ) );
long wiznet_lookup args( ( const char *name ) );
void pager_printf( CHAR_DATA * ch, char *fmt, ... );
void safe_printf args( ( const char *file, const char *function, int line, int size, char *str, char *fmt, ... ) );

/* zarius_quest.c */
void 	quest_inform		args( ( void ) );
void	quest_complete		args( ( CHAR_DATA *ch ) );
void    quest_cancel		args( ( void ) );
void    generate_auto_quest	args( ( void ) );

#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF
#undef  ED

/*****************************************************************************
 *                                    OLC                                    *
 *****************************************************************************/

/*
 * This structure is used in special.c to lookup spec funcs and
 * also in olc_act.c to display listings of spec funcs.
 */
struct spec_type
{
    char *	spec_name;
    SPEC_FUN *	spec_fun;
};

/*
 * Used in jobo_quest.c
 */
struct quest_type
{
  char *quest_name;
  QUEST_FUN *quest_fun;
};

/*
 * used in jobo_shop.c
 */
struct shop_type
{
  char *shop_name;
  SHOP_FUN *shop_fun;
};

/*
 * This structure is used in bit.c to lookup flags and stats.
 */
struct flag_type
{
    char * name;
    int  bit;
    bool settable;
};

/* tables for J.O.P.E. */
extern const struct flag_type extra_table[];
extern const struct flag_type act_table[];
extern const struct flag_type jflags_table[];
extern const struct flag_type newbits_table[];


/*
 * Object defined in limbo.are
 * Used in save.c to load objects that don't exist.
 */
#define OBJ_VNUM_DUMMY	1



/*
 * Area flags.
 */
#define         AREA_NONE       0
#define         AREA_CHANGED    1	/* Area has been modified. */
#define         AREA_ADDED      2	/* Area has been added to. */
#define         AREA_LOADING    4	/* Used for counting in db.c */
#define 	AREA_VERBOSE	8

/*
 * Area bits
 */
#define AREA_BIT_NONE         0
#define AREA_BIT_NO_TRAVEL    1 /* it is not possibly to use class travels to mobs in this area */
#define AREA_BIT_IREGEN       2 /* increases regen rate for the area           */
#define AREA_BIT_DREGEN       4 /* decreates regen rate for the area           */
#define AREA_BIT_NOMAGIC      8 /* no magic spells (player/object) can be cast */
#define AREA_BIT_NOHOME      16 /* it is not possible to home in this area     */
#define AREA_BIT_NOEDIT      32 /* This room cannot be edited by anyone        */
#define AREA_BIT_NOEDIT2     64 /* need it to actually work with noedit       */
#define AREA_WASTE          128
#define AREA_UPDATEWASTE    256

#define MAX_DIR	6
#define NO_FLAG -99	/* Must not be used in flags or stats. */

/*
 * Global Constants
 */
extern	char *	const	dir_name        [];
extern	const	sh_int	rev_dir         [];
extern	const	struct	spec_type	spec_table	[];
extern const struct quest_type quest_table[];
extern const struct shop_type shop_table[];

/*
 * Global variables
 */
extern char *last_command;
extern int mccp_mem_usage;
extern int mccp_mem_freed;
extern		int			nAllocPerm;
extern		int			nAllocString;
extern int sAllocPerm;
extern          AREA_DATA *             area_first;
extern          AREA_DATA *             area_last;
extern          int                     top_affect;
extern          int                     top_area;
extern          int                     top_ed;
extern          int                     top_exit;
extern          int                     top_help;
extern          int                     top_mob_index;
extern          int                     top_obj_index;
extern          int                     top_reset;
extern          int                     top_room;
extern          int                     top_vnum_mob;
extern          int                     top_vnum_obj;
extern          int                     top_vnum_room;
extern          char                    str_empty       [1];
extern  MOB_INDEX_DATA *        mob_index_hash  [MAX_KEY_HASH];
extern  OBJ_INDEX_DATA *        obj_index_hash  [MAX_KEY_HASH];
extern  ROOM_INDEX_DATA *       room_index_hash [MAX_KEY_HASH];
LEADER_BOARD        leader_board;
RELIGION_TABLE religion_table[MAX_RELIGIONS+1];
MAP_TYPE            map[MAPX + 1][MAPY + 1];
TOP_BOARD top_board[MAX_TOP_PLAYERS+1]; 
CHANNEL_STORAGE old_messages[STORE_MAX];

extern          PROG_CODE         *     mprog_list;
extern          PROG_CODE         *     rprog_list;
extern          PROG_CODE         *     oprog_list;

/* db.c */
void	reset_area      args( ( AREA_DATA * pArea ) );
void	reset_room	args( ( ROOM_INDEX_DATA *pRoom ) );

KINGDOM_DATA kingdom_table[MAX_KINGDOM+1];
WAR_DATA war_table[MAX_WAR];

/* string.c */
void	string_edit	args( ( CHAR_DATA *ch, char **pString ) );
void    string_append   args( ( CHAR_DATA *ch, char **pString ) );
char *	string_replace	args( ( char * orig, char * old, char * new ) );
void    string_add      args( ( CHAR_DATA *ch, char *argument ) );
char *  format_string   args( ( char *oldstring /*, bool fSpace */ ) );
char *  first_arg       args( ( char *argument, char *arg_first, bool fCase ) );
char *	string_unpad	args( ( char * argument ) );
char *	string_proper	args( ( char * argument ) );
char * all_capitalize	args( ( const char *str) );
char *  commanum        args( ( int number ) );

/* olc.c */
bool	run_olc_editor	args( ( DESCRIPTOR_DATA *d ) );
char	*olc_ed_name	args( ( CHAR_DATA *ch ) );
char	*olc_ed_vnum	args( ( CHAR_DATA *ch ) );

/* special.c */
char *	spec_string	args( ( SPEC_FUN *fun ) );	/* OLC */

int flag_lookup     args( (const char *name, const struct flag_type *flag_table) );

/* bit.c */
extern const struct flag_type 	area_flags[];
extern const struct flag_type 	area_bits[];
extern const struct flag_type   hobbit_dishes[];
extern const struct flag_type   monk_combos[];
extern const struct flag_type	sex_flags[];
extern const struct flag_type	exit_flags[];
extern const struct flag_type	door_resets[];
extern const struct flag_type	room_flags[];
extern const struct flag_type	sector_flags[];
extern const struct flag_type   furniture_flags[];
extern const struct flag_type	type_flags[];
extern const struct flag_type	extra_flags[];
extern const struct flag_type	wear_flags[];
extern const struct flag_type	act_flags[];
extern const struct flag_type	affect_flags[];
extern const struct flag_type	quest_flags[];
extern const struct flag_type	affect2_flags[];
extern const struct flag_type	apply_flags[];
extern const struct flag_type	wear_loc_strings[];
extern const struct flag_type	wear_loc_flags[];
extern const struct flag_type	weapon_flags[];
extern const struct flag_type	container_flags[];
extern const struct flag_type	liquid_flags[];
extern const struct flag_type	mprog_flags[];
extern const struct flag_type	oprog_flags[];
extern const struct flag_type	rprog_flags[];
extern const struct flag_type   position_flags[];


/* NOTE: This file only includes two examples! write your own! */

struct slay_type
{
  char *        owner;		/* only this player can use this option */
  char *        title;		/* each one must have a unique title!   */
  char *        char_msg;	/* is sent to the "slayer"		*/
  char *        vict_msg;	/* is sent to the "slayee" (poor sucker)*/
  char *        room_msg;	/* is sent to everyone else in the room */
};

/* I am lazy :) */
#define MSL MAX_STRING_LENGTH
#define MIL MAX_INPUT_LENGTH
char *CHAR2 (long iNum);
extern  bool    nodamcap;
void copyover_recover args((void));
char new_arg[MIL];
bool tctoggle;
    
#endif

