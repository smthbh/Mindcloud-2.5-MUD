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

PROJECT_DATA 	* get_project_by_number args( ( int pnum ) );
NOTE_DATA 	* get_log_by_number  	args( ( PROJECT_DATA *pproject, int pnum ) );

PROJECT_DATA *		first_project;
PROJECT_DATA *		last_project;
PROJECT_DATA * read_project args( ( FILE *fp ) );

void   note_attach         args( ( CHAR_DATA *ch ) );
void   write_projects      args( ( void ) );
void   delete_project      args( ( PROJECT_DATA *proj ) );

void load_projects( void )
{
    FILE *fp;
    PROJECT_DATA *proj;

    first_project = NULL;
    last_project = NULL;

    if( ( fp = fopen(PROJECTS_FILE, "r" ) ) == NULL )
        return;

    while( (proj = read_project( fp ) ) )
    {
      LINK( proj, first_project, last_project, next, prev );
    }

    return;
}

NOTE_DATA * read_log( FILE *fp )
{
    NOTE_DATA * log;
    char * word;

    log = alloc_mem( sizeof( NOTE_DATA ) );
    
    for ( ; ; )
    {
        word = fread_word( fp );
        if ( !str_cmp( word, "Sender" ) )
            log->sender   = fread_string( fp );
        else if ( !str_cmp( word, "Date" ) )
            log->date     = fread_string( fp );
        else if ( !str_cmp( word, "Subject" ) )
            log->subject  = fread_string( fp );
        else if ( !str_cmp( word, "Text" ) )
            log->text     = fread_string( fp );
        else if ( !str_cmp( word, "Endlog" ) )
        {
            fread_to_eol( fp );
            log->next             = NULL;
            return log;
        }
        else
        {
            free_mem( log, sizeof( NOTE_DATA ) );
            bug( "read_log: bad key word.", 0 );
            return NULL;
        }
    }
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )    if ( !str_cmp( word, literal ) ){field  = value;fMatch = TRUE;break;}
PROJECT_DATA * read_project( FILE *fp )
{
    PROJECT_DATA *project;
    NOTE_DATA *log, *tlog;
    char *word;
    char buf [MSL];
    bool fMatch;
    char letter;

    do
    {
        letter = getc( fp );
        if( feof(fp) )
        {
            fclose( fp );
            return NULL;
        }
    }

    while( isspace(letter) )
        ;
    ungetc( letter, fp );

    project = alloc_mem( sizeof( PROJECT_DATA ) );

    project->first_log = NULL;
    project->next       = NULL;
    project->coder = NULL;
    project->description = str_dup("");
    project->name = str_dup( "" );
    project->owner = str_dup( "" );
    project->date = str_dup( "Not Set?!");
    project->status = str_dup( "No update." );

    for ( ; ; )
    {
        word   = feof( fp ) ? "End" : fread_word( fp );
        fMatch = FALSE;

        switch ( UPPER(word[0]) )
        {
        case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;
	case 'C':
		KEY("Coder",	project->coder,fread_string( fp )  );
		break;
	case 'D':
		if (!str_cmp(word, "Date") )
			free_string(project->date);
		else if ( !str_cmp(word, "Description") )
			free_string(project->description);
		KEY("Date",		project->date,fread_string( fp ));		
	   	KEY("Description",project->description,fread_string(fp));
		break;
        case 'E':
            if ( !str_cmp( word, "End" ) )
            {   
                if ( !project->description )
                    project->description = str_dup( "" );
                if ( !project->name )
                    project->name = str_dup( "" );
                if ( !project->owner )
                    project->owner = str_dup( "" );
                if ( !project->date )
	        project->date = str_dup( "Not Set?!" );
                if ( !project->status )
                    project->status = str_dup( "No update." );
                if ( str_cmp(project->owner, "None") )
                    project->taken = TRUE;
                return project;
            }
            break;
	case 'L':
	    if( !str_cmp( word, "Log" ) )
	    {
		fread_to_eol( fp );
		log = read_log( fp );
		if( !log )
		{
		   xprintf( buf, "read_project: couldn't read log,aborting");
		   bug( buf, 0 );
		   exit( 1 );
	        }
		if(!log->sender)
		  log->sender = str_dup( "" );
		if(!log->date)
		  log->date   = str_dup( "" );
		if(!log->subject)
		  log->subject = str_dup( "None" );
		  log->to_list = str_dup( "" );
                  if( !project->first_log )
                      project->first_log = log;
                  else
                  {
                      log->next = project->first_log;
                      project->first_log = log;
                  }

		fMatch = TRUE;
		break;
	    }
		break;
	case 'N':
	    KEY( "Name", 		project->name,fread_string( fp )  );
		break;
	case 'O':
	    KEY( "Owner", 		project->owner,fread_string( fp ) );
		break;
	case 'S':
	    KEY( "Status", 		project->status,fread_string( fp ) );
		break;
        }
        if ( !fMatch ) 
        {
            xprintf( buf, "read_project: no match: %s", word );
            bug( buf, 0 );
        }
    }


    for( log = project->first_log; log; log = tlog )
    {
          tlog = log->next;
          free_string( log->text    );
          free_string( log->subject );
          free_string( log->to_list );
          free_string( log->date    );
          free_string( log->sender  );
          log->next = note_free;
          note_free   = log;
    }
    project->first_log =NULL;
    if ( project->coder )
        free_string( project->coder );
    if ( project->description )
        free_string( project->description );
    if ( project->name )
        free_string( project->name );
    if ( project->owner )
        free_string( project->owner );
    if ( project->date )
        free_string( project->date );
    if ( project->status )
        free_string( project->status );

    free_mem( project, sizeof( PROJECT_DATA ) );
    return project;
}

void do_project( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MSL];
   int pcount;
   int pnum;
   PROJECT_DATA *pproject;

   if( IS_NPC( ch ) )
      return;

   if( !ch->desc )
   {
      bug( "do_project: no descriptor", 0 );
      return;
   }

   argument = one_argument( argument, arg );
   smash_tilde( argument );

   if( !str_cmp( arg, "save" ) )
   {
      write_projects();
      send_to_char ("Projects saved.\n\r",ch);
      return;
   }

   if ( !str_cmp( arg, "code" ) )
   {
      pcount = 0;
	send_to_char( 
" ## | Owner       | Project              |\n\r",ch);
	send_to_char(
"---|-------------|----------------------|--------------------------|-----------\n\r",ch);
      for( pproject = first_project; pproject; pproject = pproject->next )
      {
	 pcount++;
 	if ( (pproject->status && str_cmp(pproject->status, "approved"))
		 || pproject->coder != NULL)  
		continue;
	xprintf(buf, "%2d | %-11s | %-20s |\n\r", 
	    pcount, 
	    pproject->owner ? pproject->owner : "(None)", 
 	    pproject->name);
        send_to_char(buf, ch);
      }
	return;
   }
   if ( !str_cmp( arg, "more" ) || !str_cmp( arg, "mine" ) )
   {
        NOTE_DATA *log;
	bool MINE = FALSE;
	int num_logs=0;
	pcount = 0;
	
	if ( !str_cmp( arg, "mine" ) )
		MINE = TRUE;

	send_to_char("\n\r",ch);
	send_to_char( 
" ## | Owner       | Project              | Coder         | Status     | # of Logs\n\r",ch);
	send_to_char(
"---|-------------|----------------------|---------------|------------|----------\n\r", ch);
      for( pproject = first_project; pproject; pproject = pproject->next )
      {
	   pcount++;
	   if ( MINE &&(!pproject->owner||str_cmp( ch->name,pproject->owner)) 
		&& (!pproject->coder || str_cmp( ch->name, pproject->coder)) )
		continue;
	   else if (!MINE && pproject->status && 
			!str_cmp("Done", pproject->status ) )
		continue;
	   num_logs = 0;
	   for ( log = pproject->first_log; log; log=log->next )
		num_logs++;
	   xprintf(buf, "%2d | %-11s | %-20s | %-13s | %-10s | %3d\n\r", 
	    pcount, 
	    pproject->owner ? pproject->owner : "(None)", 
 	    pproject->name,
	    pproject->coder ? pproject->coder : "(None)",
	    pproject->status ? pproject->status : "(None)",
	     num_logs);
	   send_to_char(buf, ch);
       }
	return;
   }
   if( arg[0] == '\0' || !str_cmp( arg, "list" ) )
   {
      bool aflag, projects_available;
      aflag = FALSE;
      projects_available = FALSE;
      if( !str_cmp( argument, "available" ) )
      	aflag = TRUE;

      send_to_char("\n\r",ch);
      if( !aflag )
      {
	send_to_char( 
" ## | Owner       | Project              | Date                     | Status\n\r",ch);
	send_to_char(
"---|-------------|----------------------|--------------------------|-----------\n\r",ch);
      }
      else
      {
	send_to_char(" ## | Project              | Date\n\r",ch);
	send_to_char(
"---|----------------------|--------------------------\n\r",ch);
      }
      pcount = 0;
      for( pproject = first_project; pproject; pproject = pproject->next )
      {
	 pcount++;
	 if (pproject->status && !str_cmp("Done", pproject->status ) )
		continue;
 	 if(!aflag)
	   xprintf(buf, "%2d | %-11s | %-20s | %-24s | %-10s\n\r", 
	    pcount, 
	    pproject->owner ? pproject->owner : "(None)", 
 	    pproject->name,
	    pproject->date,
	    pproject->status ? pproject->status : "(None)");
	 else
	   if( !pproject->taken )
	   {
	      if( !projects_available )
		projects_available = TRUE;
	      xprintf(buf, "%2d | %-20s | %s\n\r",
	      pcount,
	      pproject->name,
	      pproject->date);
	   }
	   send_to_char(buf, ch);
      }
      if(pcount == 0)
         send_to_char("No projects exist.\n\r",ch);
      else if( aflag && !projects_available )
	 send_to_char("No projects available.\n\r",ch); 
      return;
   }

   if( !str_cmp( arg, "add" ) )
   {
      char *strtime;
      PROJECT_DATA *new_project; /* Just to be safe */

      if( get_trust(ch) < LEVEL_JUDGE)
      {
	send_to_char("You are not powerfull enough to add a new project.\n\r",ch);
	return;
      }

      new_project = alloc_mem( sizeof( PROJECT_DATA));	
      LINK( new_project, first_project, last_project, next, prev );
      new_project->name = str_dup(argument);
      new_project->coder = NULL;
      new_project->taken = FALSE;
      new_project->owner = NULL;
      new_project->status = NULL;
      new_project->description = str_dup( "" );
      new_project->first_log = NULL;
      new_project->last_log = NULL;
      strtime                         = ctime( &current_time );
      strtime[strlen(strtime)-1]      = '\0'; 
      new_project->date = str_dup( strtime );
      write_projects();
      send_to_char("Ok.\n\r",ch );
      return;
   }

   if( !is_number( arg ) )   
   {
      send_to_char("Invalid project.\n\r",ch);
      return;
   }
      
   pnum = atoi( arg );
   pproject = get_project_by_number( pnum );
   if(!pproject)
   {
      send_to_char("No such project.\n\r",ch);
      return;
   }

   argument = one_argument( argument, arg1 );

   if( !str_cmp( arg1, "desc" ) )
   {
      if ( pproject->description == NULL )
	pproject->description = str_dup("");
      string_append( ch, &pproject->description );
      return;
   }
   if( !str_cmp( arg1, "delete" ) )
   {
      NOTE_DATA *log, *tlog;
      if( get_trust(ch) < LEVEL_HIGHJUDGE)
      {
	send_to_char("You are not high enough level to delete a project.\n\r",
		ch );
        return;
      }

      tlog = NULL;
      for( log = pproject->first_log; log; tlog = log, log = log->next )
      {
          if ( tlog == NULL )
               pproject->first_log   = log->next;
          else
               tlog->next = log->next;
          free_note( log );
      }
      pproject->last_log = NULL;
      UNLINK( pproject, first_project, last_project, next, prev );

      free_string( pproject->name );
      if ( pproject->coder )
	free_string( pproject->coder );
      if( pproject->owner )
	free_string( pproject->owner );
      if( pproject->description )
        free_string( pproject->description );
      if ( pproject->date )
	free_string( pproject->date );
      if ( pproject->status )
	free_string( pproject->status );

      free_mem( pproject, sizeof( *pproject) );
      write_projects();
      send_to_char("Ok.\n\r",ch );
      return;
   }

   if( !str_cmp( arg1, "take" ) )
   {
      if( pproject->taken  && pproject->owner &&
		!str_cmp(pproject->owner, ch->name ) )
      {
	pproject->taken = FALSE;
	free_string( pproject->owner );
	pproject->owner = NULL;
	send_to_char("You removed yourself as the owner.\n\r", ch );
        write_projects();
	return;
      }
      else if ( pproject->taken )
      {
      	send_to_char("This project is already taken.\n\r" ,ch);
	return;
      }


      if( pproject->owner )
	free_string( pproject->owner );
      pproject->owner = str_dup( ch->name );
      pproject->taken = TRUE;
      write_projects();
      send_to_char("Ok.\n\r" ,ch);
      return;
   }
   if( !str_cmp( arg1, "coder" ) )
   {
	if ( pproject->coder && !str_cmp(ch->name, pproject->coder))
	{
	  free_string( pproject->coder );
	  pproject->coder = NULL;
	  send_to_char("You removed yourself as the coder.\n\r", ch );
      	  write_projects();
	  return;
	}
	else if ( pproject->coder )
	{
	  send_to_char("This project already has a coder.\n\r",ch );
	  return;
	}
       	pproject->coder = str_dup( ch->name );
	write_projects();
	send_to_char("Ok.\n\r" ,ch);
	return;
   }
   if( !str_cmp( arg1, "status" ) )
   {
      if( pproject->owner && str_cmp( pproject->owner, ch->name ) 
	&& get_trust(ch) < LEVEL_JUDGE
	&& pproject->coder 
	&& str_cmp(pproject->coder, ch->name) )
      {  
	send_to_char("This is not your project!\n\r",ch );
	return;
      }
      if(pproject->status)
	free_string(pproject->status);
      pproject->status = str_dup(argument);
      write_projects();
      send_to_char("Done.\n\r", ch );
      return;
   } 
   if( !str_cmp( arg1, "show" ) )
   {
	if ( pproject->description )
	  send_to_char ( pproject->description, ch );
	else
	  send_to_char ("That project does not have a description.\n\r",ch);
	return;
   }
   if( !str_cmp( arg1, "log" ) )
   {
      NOTE_DATA *plog,*tlog;
      if( !str_cmp( argument, "write" ) )
      {
	 string_append( ch, &ch->pnote->text);
	 return;
      }

      argument = one_argument( argument, arg2 );

      if( !str_cmp( arg2, "subject" ) )
      {
	note_attach(ch);
	free_string(ch->pnote->subject);
	ch->pnote->subject = str_dup( argument );
	send_to_char("Ok.\n\r",ch);
	return;
      }

      if( !str_cmp( arg2, "post" ) )
      {
        char *strtime;

        if( pproject->owner && str_cmp(ch->name, pproject->owner) &&
	    pproject->coder && str_cmp(ch->name, pproject->coder) && 
	    get_trust(ch) < LEVEL_JUDGE )
	{
	   send_to_char("This is not your project!\n\r",ch );
	   return;
	}

        if ( !ch->pnote )
        {   
            send_to_char("You have no log in progress.\n\r" ,ch);
            return;
        }

        if ( !ch->pnote->subject )
	{
	    send_to_char( "Your log has no subject.\n\r",ch );
	    return;
	}
        
        strtime                         = ctime( &current_time );
        strtime[strlen(strtime)-1]      = '\0'; 
        ch->pnote->date                 = str_dup( strtime );
	ch->pnote->sender		= ch->name;
        
        plog           = ch->pnote;
        ch->pnote       = NULL;
	if( !pproject->first_log )       
	    pproject->first_log = plog;
	else
	{
	    for( tlog = pproject->first_log; tlog->next; tlog = tlog->next )
	    	;
	    tlog->next = plog;
	}

	plog->next = NULL;
	pproject->last_log = plog;
	write_projects();
        send_to_char("Ok.\n\r",ch);
	return;
      }
      
      if( !str_cmp( arg2, "list" ) )
      {
        if( pproject->owner && pproject->coder 
	    && str_cmp(ch->name, pproject->owner) 
	    && get_trust(ch) < LEVEL_BUILDER
	    && str_cmp(ch->name, pproject->coder) )
	{
	   send_to_char("This is not your project!\n\r",ch );
	   return;
	}

	pcount = 0;
        xprintf(buf, "Project: %-12s: %s\n\r",
         pproject->owner ? pproject->owner : "(None)",
         pproject->name);   
         send_to_char(buf,ch);   	

       	for( plog = pproject->first_log; plog; plog = plog->next )
      	{
           pcount++;
           xprintf( buf, "%2d) %-12s: %s\n\r",
                        pcount,
                        plog->sender,
                        plog->subject );
           send_to_char(buf,ch);
   	}
	if( pcount == 0 )
	   send_to_char("No logs available.\n\r",ch );
	return;
      }
      
      if( !is_number( arg2 ))
      {
	send_to_char("Invalid log.\n\r",ch );
        return;
      }
      
      pnum = atoi( arg2 );

      plog = get_log_by_number( pproject, pnum );	
      if( !plog )
      {
         send_to_char("Invalid log.\n\r",ch );
         return;
      }
	

      if( !str_cmp( argument, "delete" ) )
      {
        if( pproject->owner 
	 && str_cmp(ch->name, pproject->owner) 
	 && get_trust(ch) < LEVEL_JUDGE
         && pproject->coder && str_cmp(ch->name, pproject->coder ) )
	{
	   send_to_char("This is not your project!\n\r",ch );
	   return;
	}
	for( tlog = pproject->first_log; tlog->next; tlog = tlog->next )
		if( tlog->next == plog )
			break;
	tlog->next = plog->next;	
	free_note( plog );
	write_projects();
        send_to_char("Ok.\n\r",ch );
	return;
      }

      if( !str_cmp( argument, "read" ) )
      {
        if( pproject->owner && pproject->coder 
	    && str_cmp(ch->name, pproject->owner) 
	    && get_trust(ch) < LEVEL_BUILDER
	    && str_cmp(ch->name, pproject->coder ) )
	{
	   send_to_char("This is not your project!\n\r" ,ch);
	   return;
	}

	xprintf( buf, "[%3d] %s: %s\n\r%s\n\r%s",
        pnum,
        plog->sender,
        plog->subject,
        plog->date,
        plog->text );
        send_to_char(buf,ch);
	return;
      }
    }
   send_to_char("Unknown syntax see help 'PROJECT'.\n\r", ch );
   return;
}

PROJECT_DATA *get_project_by_number(int pnum)
{
   int pcount;
   PROJECT_DATA *pproject;
   pcount = 0;
   for( pproject = first_project; pproject; pproject = pproject->next )
   {
      pcount++;
      if(pcount == pnum)
         return pproject;
   }
   return NULL;
}

NOTE_DATA *get_log_by_number(PROJECT_DATA *pproject, int pnum)
{
   int pcount;
   NOTE_DATA *plog;
   pcount = 0;
   for( plog = pproject->first_log; plog; plog = plog->next )
   {
      pcount++;
      if(pcount == pnum)
         return plog;
   }
   return NULL;
}

void note_attach( ch ) CHAR_DATA *ch;
{
    NOTE_DATA *pnote;

    if ( ch->pnote != NULL )
	return;

    if ( note_free == NULL )
    {
	pnote     = alloc_perm( sizeof(*ch->pnote) );
    }
    else
    {
	pnote     = note_free;
	note_free = note_free->next;
    }

    pnote->next         = NULL;
    pnote->sender       = str_dup( ch->name );
    pnote->date         = str_dup( "" );
    pnote->to_list      = str_dup( "" );
    pnote->subject      = str_dup( "" );
    pnote->text         = str_dup( "" );
    ch->pnote           = pnote;
    return;
}
void write_projects()
{
    PROJECT_DATA *project;
    NOTE_DATA *log;
    FILE *fpout;
  
    fpout = fopen( PROJECTS_FILE, "w" );
    if ( !fpout )
    {
        bug( "FATAL: cannot open projects.txt for writing!\n\r", 0 );
        return;
    }
    for ( project = first_project; project; project = project->next )
    {
	fprintf( fpout, "Name		   %s~\n",  project->name );
       	fprintf( fpout, "Owner		   %s~\n",  (project->owner) ?
			project->owner : "None" );
	if ( project->coder )
	 fprintf( fpout, "Coder		    %s~\n", project->coder );
	fprintf( fpout, "Status		   %s~\n",  (project->status) ?
			project->status : "No update." );
 	fprintf( fpout, "Date		   %s~\n",  (project->date) ? 
			project->date : "Not Set?!?" );
	if ( project->description )
	  fprintf( fpout, "Description         %s~\n", 
				strip_cr(project->description) );
	if( project->first_log )
		for( log = project->first_log; log; log = log->next )
		  fprintf( fpout, "Log\n"
				  "Sender %s~\n"
				  "Date %s~\n"
				  "Subject %s~\n"
			          "Text %s~\n"
				  "Endlog\n",
	                log->sender,
	                log->date,
	                log->subject,
	                log->text
	                );
    
        fprintf( fpout, "End\n" );
    }
    fclose( fpout );
}


