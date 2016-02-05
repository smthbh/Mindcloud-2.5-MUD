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
/*
 * Original code by Xkilla
 * Cleaned up by Dreimas
 */
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include "merc.h"
#include "changes.h"

/*
 * Globals
 */
char *current_date args( ( void ) );

/*
 * Local Functions
 */

int maxChanges;
#define  NULLSTR( str )  ( str == NULL || str[0] == '\0' )
CHANGE_DATA *changes_table;
void do_echo( CHAR_DATA * ch, char *argument );

void load_changes( void )
{
   FILE *fp;
   int i;

   if( !( fp = fopen( CHANGES_FILE, "r" ) ) )
   {
      bug( "Could not open changes.dat file for reading.", 0 );
      return;
   }

   fscanf( fp, "%d\n", &maxChanges );

   /*
    * Use malloc so we can realloc later on 
    */
   changes_table = ( CHANGE_DATA * ) malloc( sizeof( CHANGE_DATA ) * ( maxChanges + 1 ) );

   for( i = 0; i < maxChanges; i++ )
   {
      changes_table[i].change = fread_string( fp );
      changes_table[i].coder = fread_string( fp );
      changes_table[i].date = fread_string( fp );
      changes_table[i].type = fread_string( fp );
      changes_table[i].mudtime = fread_number( fp );
   }
   changes_table[maxChanges].coder = str_dup( "" );
   fclose( fp );
   return;  /* just return */
}

char *current_date(  )
{
   static char buf[128];
   struct tm *datetime;

   datetime = localtime( &current_time );
   strftime( buf, sizeof( buf ), "%m/%d/%Y", datetime );
   return buf;
}

void save_changes( void )
{
   FILE *fp;
   int i;

   if( !( fp = fopen( CHANGES_FILE, "w" ) ) )
   {
      perror( CHANGES_FILE );
      return;
   }

   fprintf( fp, "%d\n", maxChanges );
   for( i = 0; i < maxChanges; i++ )
   {
      fprintf( fp, "%s~\n", changes_table[i].change );
      fprintf( fp, "%s~\n", changes_table[i].coder );
      fprintf( fp, "%s~\n", changes_table[i].date );
      fprintf( fp, "%s~\n", changes_table[i].type );
      fprintf( fp, "%ld\n", changes_table[i].mudtime );
      fprintf( fp, "\n" );
   }

   fclose( fp );
   return;
}

void delete_change( int iChange )
{
   int i, j;
   CHANGE_DATA *new_table;

   new_table = ( CHANGE_DATA * ) malloc( sizeof( CHANGE_DATA ) * maxChanges );

   if( !new_table )
   {
      return;
   }

   for( i = 0, j = 0; i < maxChanges + 1; i++ )
   {
      if( i != iChange )
      {
         new_table[j] = changes_table[i];
         j++;
      }
   }

   free( changes_table );
   changes_table = new_table;

   maxChanges--;

   return;
}

void do_addchange( CHAR_DATA * ch, char *argument )
{
   CHANGE_DATA *new_table;
   char arg1[MAX_INPUT_LENGTH];
   char buf[MSL];

   argument = one_argument( argument, arg1 );

   if( IS_NPC( ch ) )
      return;

   if( argument[0] == '\0' )
   {
      send_to_char( "Syntax: Addchange <type> <string>\n\r", ch );
      send_to_char( "#wTypes are: code, area, help, rule, typo.#n\n\r", ch );
      send_to_char( "#wType '#Rchanges#w' to view the list.#n\n\r", ch );
      return;
   }

   /*
    * Addchange must have an argument now - Zarius
    */
   if( str_cmp( arg1, "code" ) && str_cmp( arg1, "area" ) && str_cmp( arg1, "help" ) && str_cmp( arg1, "rule" )
       && str_cmp( arg1, "typo" ) )
   {
      send_to_char( "Incorrect Type!  Must be code, area, help, rule or typo ONLY\n\r", ch );
      return;
   }

   if( strlen( argument ) < 10 )
   {
      send_to_char( "The change description must be longer than 10 chars.\n\r", ch );
      return;
   }

   maxChanges++;
   new_table = ( CHANGE_DATA * ) realloc( changes_table, sizeof( CHANGE_DATA ) * ( maxChanges + 1 ) );

   if( !new_table )  /* realloc failed */
   {
      send_to_char( "Memory allocation failed. Brace for impact.\n\r", ch );
      return;
   }

   changes_table = new_table;

   changes_table[maxChanges - 1].change = str_dup( argument );
   changes_table[maxChanges - 1].coder = str_dup( ch->name );
   changes_table[maxChanges - 1].date = str_dup( current_date(  ) );
   changes_table[maxChanges - 1].type = str_dup( capitalize( arg1 ) );
   changes_table[maxChanges - 1].mudtime = current_time;

   send_to_char( "Changes Created.\n\r", ch );
   send_to_char( "Type 'changes' to see the changes.\n\r", ch );
   xprintf( buf, "#D<#CCHANGE#D> #w%s change #R##%d #wadded, type '#Rchanges#w' to see the details.#D\n\r",
            capitalize( arg1 ), maxChanges );
   do_echo( ch, buf );
   save_changes(  );
   return;
}

void do_delchange( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char buf[MSL];
   int num;

   argument = one_argument( argument, arg1 );

   if( IS_NPC( ch ) )
      return;

   if( !ch->desc || NULLSTR( arg1 ) || !is_number( arg1 ) )
   {
      send_to_char( "#wFor delchange you must provide a change number.#D\n\r", ch );
      send_to_char( "Syntax: delchange (change number)\n\r", ch );
      return;
   }

   num = atoi( arg1 );
   if( num < 0 || num > maxChanges )
   {
      xprintf( buf, "Valid changes are from 0 to %d.\n\r", maxChanges );
      send_to_char( buf, ch );
      return;
   }
   delete_change( num );
   send_to_char( "Change deleted.\n\r", ch );
   return;
}

/*
 * The following format code has been adapted from KaViR's justify
 * snippet -- Dreimas
 */

static void AddSpaces( char **ppszText, int iNumber )
{
   int iLoop;

   for( iLoop = 0; iLoop < iNumber; iLoop++ )
   {
      *( *ppszText )++ = ' ';
   }
}

char *change_justify( char *pszText, int iAlignment )
{
   static char s_szResult[4096];
   char *pszResult = &s_szResult[0];
   char szStore[4096];
   int iMax;
   int iLength = iAlignment - 1;
   int iLoop = 0;

   if( strlen( pszText ) < 10 )
   {
      strcpy( s_szResult, "BUG: Justified string cannot be less than 10 characters long." );
      return ( &s_szResult[0] );
   }

   while( *pszText == ' ' )
      pszText++;

   szStore[iLoop++] = *pszText++;

   if( szStore[iLoop - 1] >= 'a' && szStore[iLoop - 1] <= 'z' )
      szStore[iLoop - 1] = UPPER( szStore[iLoop - 1] );

   /*
    * The first loop goes through the string, copying it into szStore. The
    * * string is formatted to remove all newlines, capitalise new sentences,
    * * remove excess white spaces and ensure that full stops, commas and
    * * exclaimation marks are all followed by two white spaces.
    */
   while( *pszText )
   {
      switch ( *pszText )
      {
         default:
            szStore[iLoop++] = *pszText++;
            break;
         case ' ':
            if( *( pszText + 1 ) != ' ' )
            {
               /*
                * Store the character 
                */
               szStore[iLoop++] = *pszText;
            }
            pszText++;
            break;
         case '.':
         case '?':
         case '!':
            szStore[iLoop++] = *pszText++;
            switch ( *pszText )
            {
               default:
                  szStore[iLoop++] = ' ';
                  szStore[iLoop++] = ' ';
                  /*
                   * Discard all leading spaces 
                   */
                  while( *pszText == ' ' )
                     pszText++;
                  /*
                   * Store the character 
                   */
                  szStore[iLoop++] = *pszText++;
                  if( szStore[iLoop - 1] >= 'a' && szStore[iLoop - 1] <= 'z' )
                     szStore[iLoop - 1] &= ~32;
                  break;
               case '.':
               case '?':
               case '!':
                  break;
            }
            break;
         case ',':
            /*
             * Store the character 
             */
            szStore[iLoop++] = *pszText++;
            /*
             * Discard all leading spaces 
             */
            while( *pszText == ' ' )
               pszText++;
            /*
             * Commas shall be followed by one space 
             */
            szStore[iLoop++] = ' ';
            break;
         case '$':
            szStore[iLoop++] = *pszText++;
            while( *pszText == ' ' )
               pszText++;
            break;
         case '\n':
         case '\r':
            pszText++;
            break;
      }
   }

   szStore[iLoop] = '\0';

   /*
    * Initialise iMax to the size of szStore 
    */
   iMax = strlen( szStore );

   /*
    * The second loop goes through the string, inserting newlines at every
    * * appropriate point.
    */
   while( iLength < iMax )
   {
      /*
       * Go backwards through the current line searching for a space 
       */
      while( szStore[iLength] != ' ' && iLength > 1 )
         iLength--;

      if( szStore[iLength] == ' ' )
      {
         szStore[iLength] = '\n';

         iLength += iAlignment;
      }
      else
         break;
   }

   /*
    * Reset the counter 
    */
   iLoop = 0;

   /*
    * The third and final loop goes through the string, making sure that there
    * * is a \r (return to beginning of line) following every newline, with no
    * * white spaces at the beginning of a particular line of text.
    */
   while( iLoop < iMax )
   {
      /*
       * Store the character 
       */
      *pszResult++ = szStore[iLoop];
      switch ( szStore[iLoop] )
      {
         default:
            break;
         case '\n':
            *pszResult++ = '\r';
            while( szStore[iLoop + 1] == ' ' )
               iLoop++;
            /*
             * Add spaces to the front of the line as appropriate 
             */
            AddSpaces( &pszResult, 25 );
            break;
      }
      iLoop++;
   }

   *pszResult++ = '\0';

   return ( &s_szResult[0] );
}

int num_changes( void )
{
   char *test;
   int today;
   int i;

   i = 0;
   test = current_date(  );
   today = 0;

   for( i = 0; i < maxChanges; i++ )
      if( !str_cmp( test, changes_table[i].date ) )
         today++;

   return today;
}

void do_changes( CHAR_DATA * ch, char *argument )
{
   char arg[MIL];
   char buf[MSL];
   char *test;
   int today;
   int i;
   bool fAll;
   int totalpages = 1;

   one_argument( argument, arg );

   if( IS_NPC( ch ) )
      return;

   if( str_cmp( arg, "code" ) && str_cmp( arg, "area" ) && str_cmp( arg, "help" ) && str_cmp( arg, "rule" )
       && str_cmp( arg, "typo" ) && str_cmp( arg, "all" ) && !NULLSTR( arg ) )
   {
      send_to_char( "Incorrect Type!  Must be code, area, help, rule, type or ALL\n\r", ch );
      return;
   }

   if( maxChanges < 1 )
      return;

   i = 0;
   test = current_date(  );
   today = 0;

   for( i = 0; i < maxChanges; i++ )
      if( !str_cmp( test, changes_table[i].date ) )
         today++;

   if( NULLSTR( arg ) )
      fAll = FALSE;
   else
      fAll = TRUE;

   send_to_char( "\n\r", ch );
   xprintf( buf, "#R-=[ #W%s Changelog #R]=-", MUDNAME );
   cent_to_char( buf, ch );
   pager_to_char( "#R--------------------------------------------------------------------------------#n\n\r", ch );
   pager_to_char( "#wNo.  Coder        Date        Type    Change#n\n\r", ch );
   pager_to_char( "#R--------------------------------------------------------------------------------#n\n\r", ch );

   if( is_number( arg ) )
   {
      int page = atoi( arg );
      int number;

      number = page * 10;

      if( page < 0 || page > totalpages )
      {
         ch_printf( ch, "#RPage must be between 1 and %d!!!\n\r", totalpages );
         return;
      }

      for( i = ( number - 10 ); ( i < number && i < maxChanges ); i++ )
      {
         pager_printf( ch, "#0[#R%3d#0] %-11s #c*%-6s #P%-5s #w%-45s#n\n\r", ( i + 1 ), changes_table[i].coder,
                    changes_table[i].date, changes_table[i].type, change_justify( changes_table[i].change, 45 ) );
      }
   }
   else {

   for( i = 0; i < maxChanges; i++ )
   {
      if( !fAll && changes_table[i].mudtime + ( 7 * 24L * 3600L ) < current_time )
         continue;

      if( !str_cmp( arg, "code" ) && str_cmp( changes_table[i].type, "code" ) )
         continue;
      if( !str_cmp( arg, "area" ) && str_cmp( changes_table[i].type, "area" ) )
         continue;
      if( !str_cmp( arg, "help" ) && str_cmp( changes_table[i].type, "help" ) )
         continue;
      if( !str_cmp( arg, "rule" ) && str_cmp( changes_table[i].type, "rule" ) )
         continue;
      if( !str_cmp( arg, "typo" ) && str_cmp( changes_table[i].type, "typo" ) )
         continue;

      pager_printf( ch, "#0[#R%3d#0] %-11s #c*%-6s #P%-5s #w%-55s#D\n\r", ( i + 1 ), changes_table[i].coder,
               changes_table[i].date, changes_table[i].type, change_justify( changes_table[i].change, 55 ) );
   }
   }
   pager_to_char( "#R--------------------------------------------------------------------------------#n\n\r", ch );
   pager_printf( ch, "#w    There are #D[ #Y%d#D ] #wchanges in the database, #Y%d #wof them were added today.#n\n\r",
            maxChanges, today );
   pager_to_char( "#0       Also see: '#Cchanges all#0' for a list of all the changes.#0\n\r", ch );
   pager_to_char( "#0             Or: '#Cchanges <type>#0' for a filtered list of all.#n\n\r", ch );
   if( totalpages > 1 )
   {
      pager_printf( ch, "#0             Or: '#Cchanges <1 to %d>#0' for individual pages.#n\n\r", totalpages );
   }
   pager_to_char( "#R--------------------------------------------------------------------------------#n\n\r", ch );
   return;
}

