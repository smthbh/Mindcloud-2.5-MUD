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
#include <stdarg.h>
#include <time.h>
#include "merc.h"
#include "interp.h"


/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 *
 * Renamed so it can play itself system independent.
 * Samson 10-12-03
 */
size_t mudstrlcpy( char *dst, const char *src, size_t siz )
{
   register char *d = dst;
   register const char *s = src;
   register size_t n = siz;

   /*
    * Copy as many bytes as will fit 
    */
   if( n != 0 && --n != 0 )
   {
      do
      {
         if( ( *d++ = *s++ ) == 0 )
            break;
      }
      while( --n != 0 );
   }

   /*
    * Not enough room in dst, add NUL and traverse rest of src 
    */
   if( n == 0 )
   {
      if( siz != 0 )
         *d = '\0';  /* NUL-terminate dst */
      while( *s++ )
         ;
   }
   return ( s - src - 1 ); /* count does not include NUL */
}

/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(initial dst) + strlen(src); if retval >= siz,
 * truncation occurred.
 *
 * Renamed so it can play itself system independent.
 * Samson 10-12-03
 */
size_t mudstrlcat( char *dst, const char *src, size_t siz )
{
   register char *d = dst;
   register const char *s = src;
   register size_t n = siz;
   size_t dlen;

   /*
    * Find the end of dst and adjust bytes left but don't go past end 
    */
   while( n-- != 0 && *d != '\0' )
      d++;
   dlen = d - dst;
   n = siz - dlen;

   if( n == 0 )
      return ( dlen + strlen( s ) );
   while( *s != '\0' )
   {
      if( n != 1 )
      {
         *d++ = *s;
         n--;
      }
      s++;
   }
   *d = '\0';
   return ( dlen + ( s - src ) );   /* count does not include NUL */
}

/*Originaly, i heard on gamedev that someone was looking for a way to stop xprintf
 *from overflowing, ofcourse, people fail to think of functions like strlcpy, which
 *is essentaily a safe xprintf, atleast as it has been described to me.  I however
 *chose to write my own system.  This allows me to capture the function, and line
 *of the overflowing string in question.  Thus allowing me to essentaily find out
 *where the mud overflows, and stop it from happening.  Great little tool.
 *All i ask is that you leave this header in place.  -- Darien of Sandstorm:Mages Sanctuary
 */
void safe_printf( const char *file, const char *function, int line, int size, char *str, char *fmt, ... )
{
   char buf[MAS];
   va_list args;
   va_start( args, fmt );
   vsprintf( buf, fmt, args );
   va_end( args );

   /*
    * Max Alloc Size is allot!
    */
   if( size > MAS )
   {
      log_string( LOG_BUG, "xprintf size greater then MAS!!!!\n\r" );
      log_string( LOG_BUG, "Warning: Overflow has been caught by xprintf.\n\r" );
      log_string( LOG_BUG, "Memcheck: xprintf:File %s, Function %s, Line %d.\n\r", file, function, line );
      wiznet_printf( NULL, NULL, WIZ_MEMCHECK, 0, 0, "Memcheck: System memory corrupted by overflow through xprintf: File: %s Function: %s Line: %d", file, function, line  );
      return;
   }

   if( ( unsigned )size < strlen( buf ) + 1 )
   {
      log_string( LOG_BUG, "XPRINTF error: fmt %s.\n\r", fmt );
      log_string( LOG_BUG, "Warning: Overflow has been caught by xprintf.\n\r" );
      log_string( LOG_BUG, "Memcheck: xprintf: File %s, Function %s, Line %d.\n\r", file, function, line );
      wiznet_printf( NULL, NULL, WIZ_MEMCHECK, 0, 0, "Memcheck: System memory corrupted by overflow through xprintf: File: %s Function: %s Line: %d", file, function, line  );
   }
   else
   {
      mudstrlcpy( str, buf, size );

      /*
       * Just double checking.
       */
      if( strlen( str ) > ( unsigned )size - 1 )
      {
         char egbug[MSL];
         log_string( LOG_BUG, "XPRINTF error: fmt %s.\n\r", fmt );
         log_string( LOG_BUG, "Warning: Overflow has been caught by xprintf.\n\r" );
         log_string( LOG_BUG, "Memcheck: Xprintf: File %s, Function %s, Line %d.\n\r", file, function, line );

         /*
          * Yes, this is a potential loop bug if infact the xprintf does collapse in on itself..
          */
         xprintf( egbug, "Memcheck: System memory corrupted by overflow through xprintf: File: %s Function: %s Line: %d",
                  file, function, line );
         wiznet( egbug, NULL, NULL, WIZ_MEMCHECK, 0, 0 );
      }
   }
}

/* xcatf a formated string */
void safe_strcatf( const char *file, const char *function, int line, int size, char *prev, char *next, ... )
{
   char buf[MAS];
   va_list args;

   va_start( args, next );
   vsnprintf( buf, MAS, next, args );
   va_end( args );

   /*
    * Max Alloc Size is allot!
    */
   if( size > MAS )
   {
      char egbug[MSL];
      log_string( LOG_BUG, "xcatf size greater then MAS!!!!\n\r" );
      log_string( LOG_BUG, "ERROR: System Memory Corrupted by Overflow, through xcatf.\n\r" );
      log_string( LOG_BUG, "Memcheck: xcatf:File %s, Function %s, Line %d.\n\r", file, function, line );
      xprintf( egbug, "Memcheck: System memory corrupted by overflow through xcatf: File: %s Function: %s Line: %d", file,
               function, line );
      wiznet( egbug, NULL, NULL, WIZ_MEMCHECK, 0, 0 );
      return;
   }

   if( ( unsigned )size < strlen( buf ) + 1 )
   {
      char egbug[MSL];
      log_string( LOG_BUG, "XCATF error: fmt %s.\n\r", next );
      log_string( LOG_BUG, "ERROR: System Memory Corrupted by Overflow, through xcatf.\n\r" );
      log_string( LOG_BUG, "Memcheck: xcatf: File %s, Function %s, Line %d.\n\r", file, function, line );
      /*
       * Yes, this is a potential loop bug if infact the xcatf does collapse in on itself..
       */
      xprintf( egbug, "Memcheck: System memory corrupted by overflow through xcatf: File: %s Function: %s Line: %d", file,
               function, line );
      wiznet( egbug, NULL, NULL, WIZ_MEMCHECK, 0, 0 );
      return;
   }

   mudstrlcat( prev, buf, size );

   /*
    * Just double checking.
    */
   if( strlen( prev ) > ( unsigned )size - 1 )
   {
      char egbug[MSL];
      log_string( LOG_BUG, "XCATF error: fmt %s.\n\r", next );
      log_string( LOG_BUG, "ERROR: System Memory Corrupted by Overflow, through xcatf.\n\r" );
      log_string( LOG_BUG, "Memcheck: Xcatf: File %s, Function %s, Line %d.\n\r", file, function, line );
      /*
       * Yes, this is a potential loop bug if infact the xcatf does collapse in on itself..
       */
      xprintf( egbug, "Memcheck: System memory corrupted by overflow through xcatf: File: %s Function: %s Line: %d", file,
               function, line );
      wiznet( egbug, NULL, NULL, WIZ_MEMCHECK, 0, 0 );
   }
}


char *get_curdate( void )
{
   static char buf[128];
   struct tm *datetime;

   datetime = localtime( &current_time );
   strftime( buf, sizeof( buf ), "%m-%d-%Y", datetime );
   return buf;
}

void log_string( int type, const char *fmt, ... )
{
   DESCRIPTOR_DATA *d;
   va_list args;
   char *strtime;
   char buf[45];
   char bufew[2 * MSL];
   char bufee[2 * MSL];
   FILE *log_file;
   buf[0] = '\0';

   log_file = NULL;

   // Get the wanted text
   va_start( args, fmt );
   vsprintf( bufew, fmt, args );
   va_end( args );

   if( type & LOG_CRIT )
   {
      xprintf( buf, "../log/%s.critical", get_curdate(  ) );
      log_file = fopen( buf, "a" );
      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      fprintf( log_file, "%s :: %s\n", strtime, bash_color( bufew ) );
      fflush( log_file );
      fclose( log_file );
      for( d = descriptor_list; d != NULL; d = d->next )
         if( d->connected == CON_PLAYING && IS_IMMORTAL( d->character ) )
            printf_to_char( d->character, "Critical: %s\n\r", bufew );
      xprintf( bufee, "Critical: %s", bufew );
   }
   if( type & LOG_ERR )
   {
      xprintf( buf, "../log/%s.error", get_curdate(  ) );
      log_file = fopen( buf, "a" );
      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      fprintf( log_file, "%s :: %s\n", strtime, bash_color( bufew ) );
      fprintf( stderr, "%s :: %s\n", strtime, bash_color( bufew ) );
      fflush( log_file );
      fclose( log_file );
      xprintf( bufee, "Error: %s", bufew );
   }
   if( type & LOG_BUG )
   {
      xprintf( buf, "../log/%s.bug", get_curdate(  ) );
      log_file = fopen( buf, "a" );
      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      fprintf( log_file, "%s :: %s\n", strtime, bash_color( bufew ) );
      fflush( log_file );
      fclose( log_file );
      xprintf( bufee, "Bug: %s", bufew );
      log_string2( bufee );
   }
   if( type & LOG_SECURITY )
   {
      xprintf( buf, "../log/%s.security", get_curdate(  ) );
      log_file = fopen( buf, "a" );
      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      fprintf( log_file, "%s :: %s\n", strtime, bash_color( bufew ) );
      fflush( log_file );
      fclose( log_file );
      xprintf( bufee, "Security: %s", bufew );
   }
   if( type & LOG_CONNECT )
   {
      xprintf( buf, "../log/%s.connect", get_curdate(  ) );
      log_file = fopen( buf, "a" );
      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      fprintf( log_file, "%s :: %s\n", strtime, bash_color( bufew ) );
      fflush( log_file );
      fclose( log_file );
      xprintf( bufee, "Connect: %s", bufew );
   }
   if( type & LOG_GAME )
   {
      xprintf( buf, "../log/%s.game", get_curdate(  ) );
      log_file = fopen( buf, "a" );
      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      fprintf( log_file, "%s :: %s\n", strtime, bash_color( bufew ) );
      fprintf( stderr, "%s :: %s\n", strtime, bash_color( bufew ) );
      fflush( log_file );
      fclose( log_file );
      xprintf( bufee, "Game: %s", bufew );
   }
   if( type & LOG_COMMAND )
   {
      xprintf( buf, "../log/%s.comm", get_curdate(  ) );
      log_file = fopen( buf, "a" );
      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';
      fprintf( log_file, "%s :: %s\n", strtime, bash_color( bufew ) );
      fflush( log_file );
      fclose( log_file );
      xprintf( bufee, "Command: %s", bufew );
   }
   wiznet(bufee,NULL,NULL, WIZ_DEBUG, 0, 7);
}

void do_wiznet( CHAR_DATA * ch, char *argument )
{
   int flag;
   int col = 0;
   char buf[MAX_STRING_LENGTH];

   if( argument[0] == '\0' )
/* Show wiznet options - just like channel command */
   {
      send_to_char( "\n\r#WWELCOME TO WIZNET!!!\n\r", ch );
      send_to_char( "#0Option      Status       Option       Status\n\r", ch );
      send_to_char( "#B---------------------------------------------#n\n\r", ch );
      /*
       * list of all wiznet options 
       */
      buf[0] = '\0';

      for( flag = 0; wiznet_table[flag].name != NULL; flag++ )
      {
         if( wiznet_table[flag].level <= get_trust( ch ) )
         {
            xprintf( buf, "#P%-14s#n %s\t", wiznet_table[flag].name,
                     IS_SET( ch->wiznet, wiznet_table[flag].flag ) ? "#GON#n" : "#ROFF#n" );
            send_to_char( buf, ch );
            col++;
            if( col == 2 )
            {
               send_to_char( "\n\r", ch );
               col = 0;
            }
         }
      }
/* To avoid color bleeding */
      send_to_char( "\n\r#n", ch );
      return;
   }

   if( !str_prefix( argument, "on" ) )
   {
      send_to_char( "#GWelcome to Wiznet!#n\n\r", ch );
      SET_BIT( ch->wiznet, WIZ_ON );
      return;
   }

   if( !str_prefix( argument, "off" ) )
   {
      send_to_char( "#GSigning off of Wiznet.#n\n\r", ch );
      REMOVE_BIT( ch->wiznet, WIZ_ON );
      return;
   }

   flag = wiznet_lookup( argument );

   if( flag == -1 || get_trust( ch ) < wiznet_table[flag].level )
   {
      send_to_char( "#wNo such option.#n\n\r", ch );
      return;
   }

   if( IS_SET( ch->wiznet, wiznet_table[flag].flag ) )
   {
      xprintf( buf, "#BYou will no longer see %s on wiznet.#n\n\r", wiznet_table[flag].name );
      send_to_char( buf, ch );
      REMOVE_BIT( ch->wiznet, wiznet_table[flag].flag );
      return;
   }
   else
   {
      xprintf( buf, "#BYou will now see %s on wiznet.#n\n\r", wiznet_table[flag].name );
      send_to_char( buf, ch );
      SET_BIT( ch->wiznet, wiznet_table[flag].flag );
      return;
   }
}

void wiznet( char *string, CHAR_DATA * ch, OBJ_DATA * obj, long flag, long flag_skip, int min_level )
{
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   xprintf( buf, "#W%s#n", string );
   for( d = descriptor_list; d != NULL; d = d->next )
   {
	  if(!d->character) return;
      if( d->connected == CON_PLAYING && ( IS_HERO( d->character ) ) && IS_SET( d->character->wiznet, WIZ_ON )
          && ( !flag || IS_SET( d->character->wiznet, flag ) ) && ( !flag_skip
                                                                    || !IS_SET( d->character->wiznet, flag_skip ) )
          && get_trust( d->character ) >= min_level && d->character != ch )
      {
         if( IS_SET( d->character->wiznet, WIZ_PREFIX ) )
            send_to_char( "#Y-->#n ", d->character );
         act( buf, d->character, obj, ch, TO_CHAR );
      }
   }

   return;
}

void wiznet_printf( CHAR_DATA * ch, OBJ_DATA * obj, long flag, long flag_skip, int min_level, char *format, ... )
{
   va_list ap;
   char buf[MSL], buf2[MSL], Newtime[30];
   char *strtime = ( char * )ctime( &current_time );
   DESCRIPTOR_DATA *d;
   int pos = 0, i = 1;

   do
   {
      if( i > 11 )
         buf[pos++] = *strtime;
   }
   while( *strtime++ && i++ && pos < 8 );
   buf[pos] = '\0';
   mudstrlcpy( Newtime, buf, 30 );
   xprintf( buf, "[WiZNET] %s: ", Newtime );

   va_start( ap, format );

   if( !descriptor_list )
      return;

   for( d = descriptor_list; d != NULL; d = d->next )
   {
      char immnetbuffer[MSL];

      if( !d->character )
         continue;

      if( d->connected == CON_PLAYING && IS_IMMORTAL( d->character ) && IS_SET( d->character->wiznet, WIZ_ON )
          && ( !flag || IS_SET( d->character->wiznet, flag ) ) && ( !flag_skip
                                                                    || !IS_SET( d->character->wiznet, flag_skip ) )
          && get_trust( d->character ) >= min_level && d->character != ch )
      {
         vsnprintf( buf2, sizeof( buf2 ), format, ap );
         mudstrlcat( buf, buf2, MSL );
         xprintf( immnetbuffer, "%s", buf );
         act( immnetbuffer, d->character, obj, ch, TO_CHAR );
      }
   }

   va_end( ap );
   return;
}

/* returns a flag for wiznet */
long wiznet_lookup( const char *name )
{
   int flag;

   for( flag = 0; wiznet_table[flag].name != NULL; flag++ )
   {
      if( LOWER( name[0] ) == LOWER( wiznet_table[flag].name[0] ) && !str_prefix( name, wiznet_table[flag].name ) )
         return flag;
   }

   return -1;
}

void pager_printf( CHAR_DATA * ch, char *fmt, ... )
{
   char buf[MSL * 2];
   va_list args;

   va_start( args, fmt );
   vsnprintf( buf, MAX_STRING_LENGTH * 2, fmt, args );
   va_end( args );

   pager_to_char( buf, ch );
}

void load_arti()
{
	char buf[MSL];
	//Right now, this will just load an arti randomly to the mud
    //But if you want you can make it load by the person who gets the last mkill or whatever
    //Zarius
	randomize_object(number_range(33960, 33976));
	xprintf_2(buf, "#7The Gods smile down and drop an #0Artifact#n into the world!\n\r" );
	do_info( NULL, buf );
}
