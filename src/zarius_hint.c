#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* Hint Module for Mindcloud 4 */

HINT_DATA *           first_hint;
HINT_DATA *           last_hint;

HINT_DATA *read_hint args( ( char *filename, FILE *fp));
void do_hint 	args (( CHAR_DATA *ch, char *argument ));
void hintswap 	args (( HINT_DATA *pHint1, HINT_DATA *pHint2 ));
void write_rank args (( void ));

void do_nohint( CHAR_DATA *ch, char * argument)
{
	if (IS_NPC(ch))
		return;

    if (IS_SET(ch->pcdata->jflags, JFLAG_NOHINT))
    {
      send_to_char("You will now see hints.\n\r",ch);
      REMOVE_BIT(ch->pcdata->jflags, JFLAG_NOHINT);
    }
   else
   {
     send_to_char("From now on, you will not see any hints.\n\r",ch);
     SET_BIT(ch->pcdata->jflags, JFLAG_NOHINT);
   }
}

void hintswap(HINT_DATA *pHint1, HINT_DATA *pHint2)
{
	if (pHint1->prev)
    {
    	pHint1->prev->next = pHint2;
    }
    else  
    {
    	first_hint = pHint2;
    }
          
    pHint2->prev = pHint1->prev;    
    pHint1->next = pHint2->next;
                 
    if (pHint1->next)
    {
	    pHint1->next->prev = pHint1;
    }
                                
    pHint2->next = pHint1;   
    pHint1->prev = pHint2;
}

char *get_hint(int level)
{
   HINT_DATA *hintData;
   bool found=FALSE;
   char buf[MSL];
   int count,which;

   count = 0;
   if (level < 0)
   {
	xprintf(buf,"Hint Level error, Level was %d",level);
	 return str_dup(buf);
   }
   else
   {
	found = FALSE;
	hintData = first_hint;
	for (hintData = first_hint;hintData;hintData = hintData->next)
	{
	    if (level >= hintData->low && level <= hintData->high)
		count++;
	}
	if (count > 1)
 	{
	    which = number_range(1,count);
	    count = 0;
	    for (hintData = first_hint;hintData;hintData = hintData->next)
	    {
		if (level >= hintData->low && level <= hintData->high)
		    count++;
		if (count == which)
		    return str_dup(hintData->text);
	    }
	}		
	else if (count == 1)
	{
	    for (hintData = first_hint;hintData;hintData = hintData->next)
	    {
		if (level >= hintData->low && level <= hintData->high)
		    return str_dup(hintData->text);
	    }
	}
	else return str_dup(" ");	    
    }
    return str_dup(" ");
}


void write_hint( void)
{
    HINT_DATA *hintData;
    FILE *fp;
    char filename[256];

    xprintf(filename,"%s",HINT_FILE);
    if (( fp = fopen(filename,"w")) == NULL)
    {
	bug("Save hint: fopen",0);
	perror(filename);
	return;
    }
    else
    {
    	for (hintData = first_hint;hintData;hintData=hintData->next)
    	{
		fprintf(fp,"Text     %s~\n",hintData->text);
	       	fprintf(fp,"Low        %d\n",hintData->low);
		fprintf(fp,"High       %d\n",hintData->high);
		fprintf(fp,"End\n");
	}
	fclose(fp);
	return;
    }
}

void do_hint(CHAR_DATA *ch, char *argument)
{
    char arg[MSL];
    char arg2[MSL];
    char arg3[MSL];
    int  i=0;
    HINT_DATA *hintData;
    int no=0;
    int ano=0;
    bool found=FALSE;


    if (IS_NPC(ch))
	return;
    if (!IS_IMMORTAL(ch)) return;
    else 
    if (argument[0] == '\0' && IS_IMMORTAL(ch))
    {
	send_to_char("#YHint what (Hint Help for Syntax) ?#n\n\r",ch);
	return;
    }
    argument = one_argument(argument,arg);
    argument = one_argument(argument,arg2);
    argument = one_argument(argument,arg3);
    if (!str_cmp(arg,"help") || arg[0]=='\0')
    {
	send_to_char("#RHint Help#n\n\r",ch);
	send_to_char("#YSyntax   : #Yhint <command>#n\n\r\n\r",ch);
	send_to_char("#CCommands : #Ylist                   - will list the hints\n\r",ch);
	send_to_char("         : #Yadd <low> <high>       - will add a hint\n\r",ch);
	send_to_char("         : #Yremove <number>        - will remove the hint at <number>\n\r",ch);
	send_to_char("         : #YSort                   - will sort the hints\n\r",ch);
	send_to_char("         : #Ysave                   - will save the hints\n\r",ch);
	send_to_char("         : #YEdit <number> <option> - will edit hint <number>\n\r",ch);
	send_to_char("           #Y      options = <text/low/high>\n\r",ch);
     	return;
    }
    if (!str_cmp(arg,"sort"))
    {
	HINT_DATA *pCurrentHint;
	
        pCurrentHint = first_hint;  
 
        while (pCurrentHint->next)
        {
                if (pCurrentHint->next->low < pCurrentHint->low)
                {
                        while (pCurrentHint->next->low < pCurrentHint->low)
                        {
                                hintswap(pCurrentHint, pCurrentHint->next);
 
                                if(pCurrentHint->prev->prev)
                                {
                                        pCurrentHint = pCurrentHint->prev->prev;
                                }
                                else
                                {
                                        pCurrentHint = first_hint;
                                }
                        }
                }
                else
                {
                        pCurrentHint = pCurrentHint->next;
                }
        }
	send_to_char("Hints sorted!\n\r",ch);
	return;
    }

    if(!str_cmp(arg,"list"))
    {
        if (first_hint)
        {
    	    pager_printf(ch,"No | Low | High |            Text             \n\r");
    	    pager_printf(ch,"---|-----|------|--------------------------------------------------\n\r");
    	    i=0;
    	    for (hintData=first_hint;hintData;hintData = hintData->next)
    	    {
    	    	i++;
    	    	pager_printf(ch,"%2d | %3d | %4d | %-30s\n\r",i,hintData->low,hintData->high,hintData->text);
     	    }
 	    pager_printf(ch,"\n\r%d hints found",i);
 	}
 	else
 	   ch_printf(ch,"No Hints formed yet!\n\r");
 	return;
    }

    else if (!str_cmp(arg,"remove"))
    {
	no = 0;
	if (!is_number(arg2))
	{
	    send_to_char("#RRemove which hint ?#n\n\r",ch);
	    return;
	}
	ano = atoi(arg2);
	found = FALSE;
	for (hintData=first_hint;hintData;hintData=hintData->next)
	{
	    no++;
	    if (no==ano)
	    {
		    ch_printf(ch,"#wHint Number #W%d #Rremoved#n\n\r",ano);
		    UNLINK(hintData,first_hint,last_hint,next,prev);
	   	    if (hintData->text) STRFREE(hintData->text);
		    DISPOSE(hintData);
		    found = TRUE;
		    break;
	    }
	}
	if (!found)
	{
	    send_to_char("Hint not found\n\r",ch);
	    return;
	}
	return;
    }
    else if (!str_cmp(arg,"add"))
    {
	if (arg2=='\0')
	{
	    send_to_char("What level is the low limit for this hint ?\n\r",ch);
	    return;
	}
	if (arg3=='\0')	
	{
	    send_to_char("What level is the high limit for this hint?\n\r",ch);
	    return;
	}
	if (atoi(arg2) > atoi(arg3))
 	{
	    ch_printf(ch,"High is less than Low, aborting!\n\r");
	    return;
	}
	CREATE(hintData, HINT_DATA,1);
	hintData->low = atoi(arg2);
	hintData->high = atoi(arg3);
	hintData->text = STRALLOC(argument);
	LINK(hintData,first_hint,last_hint,next,prev);
	send_to_char("Ok. Hint created\n\r",ch);
	return;
    }
    else if (!str_cmp(arg,"edit"))
    {
    	if (arg2[0]== '\0')
    	{
    	    ch_printf(ch,"Edit what number?\n\r");
    	    return;
    	}
    	else no = atoi(arg2);
    	if (arg3[0]=='\0')
    	{
    	    ch_printf(ch,"Edit which of Hint %d's fields (low/high/text)?\n\r",no);
    	    return;
    	}
    	if (argument[0]=='\0')
    	{
    	    ch_printf(ch,"Change hint %d's field %s to what ?\n\r",no,arg3);
    	    return;
    	}
    	for (hintData=first_hint;hintData;hintData = hintData->next)
    	{
    	    i++;
    	    if (i==no)
    	    {
    	    	found = TRUE;
    	    	break;
    	    }
    	}
    	if (!found)
    	{
    	    ch_printf(ch,"Hint %d not found!\n\r",no);
    	    return;
    	}
    	else
    	{
    	    if (!str_cmp(arg3,"text"))
    	    {
    	    	if (hintData->text) STRFREE(hintData->text);
    	    	hintData->text = STRALLOC(argument);
    	    	ch_printf(ch,"Hint text changed!\n\r");
    	    	return;
    	    }
    	    else if (!str_cmp(arg3,"low"))
    	    {
		if (atoi(argument) > hintData->high)
		{
		    ch_printf(ch,"Low cannot be higher than High, aborting!\n\r");
		    return;
		}
		hintData->low = atoi(argument);
    	    	ch_printf(ch,"Low level for hint changed!\n\r");
    	    	return;
    	    }
	    else if (!str_cmp(arg3,"high"))
	    {
		if (atoi(argument) < hintData->low)
		{
		    ch_printf(ch,"High cannot be lower than Low, aborting!\n\r");
		    return;
		}
		hintData->high = atoi(argument);
		ch_printf(ch,"High level for hint changed!\n\r");
		return;
	    }
    	    else
    	    {
    	    	ch_printf(ch,"Valid fields are : low/high/text\n\r",ch);
    	    	return;
    	    }
    	}
    }
    else if (!str_cmp(arg,"save"))
    {
    	write_hint();	    
	ch_printf(ch,"Hint saved\n\r");
    }
    else
    {
	send_to_char("Syntax : hint (list/add/remove/edit/save/sort\n\r",ch);
	return;
    }	
}


void load_hint( void )
{
   char filename[MAX_INPUT_LENGTH];
   FILE *fp;
   HINT_DATA *hintData;

   first_hint = NULL;
   last_hint = NULL;
   xprintf( filename, "%s",HINT_FILE );
   if( ( fp = fopen( filename, "r" ) ) == NULL )
      return;

   while( ( hintData = read_hint( filename, fp ) ) != NULL )
      LINK( hintData, first_hint, last_hint, next, prev );

   return;
}
HINT_DATA *read_hint( char *filename, FILE *fp )
{
   HINT_DATA *hintData;
   char *word;
   char buf[MAX_STRING_LENGTH];
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
   while( isspace(letter) );
   ungetc( letter, fp );

   CREATE( hintData, HINT_DATA, 1);
#ifdef KEY
#undef KEY
#endif
#define KEY( literal, field, value )                                    \
                                if ( !str_cmp( word, literal ) )        \
                                {                                       \
                                    field  = value;                     \
                                    fMatch = TRUE;                      \
                                    break;                              \
                                }
   hintData->next       = NULL;
   hintData->prev       = NULL;
   hintData->text	   = STRALLOC("");
   hintData->low	   = 0;
   hintData->high       = 0;
    for ( ; ; )
    {
        word   = feof( fp ) ? "End" : fread_word( fp );
        fMatch = FALSE;

        switch ( UPPER(word[0]) )
        {
        case 'T':
                if (!str_cmp(word, "Text") )
                        STRFREE(hintData->text);
                KEY("Text",           hintData->text,         fread_string( fp ));
                break;
        case 'E':
            if ( !str_cmp( word, "End" ) )
            {
              if ( !hintData->text )
                hintData->text = STRALLOC("");
              return hintData;
            }
                break;
        case 'H':
                KEY("High",           hintData->high, fread_number(fp));
                break;
        case 'L':
                KEY("Low",           hintData->low, fread_number(fp));
                break;
        }
        if ( !fMatch )
        {
            xprintf( buf, "read_hint: no match: %s", word );
            bug( buf, 0 );
        }
    }
  if ( hintData->text)
        STRFREE( hintData->text);
  DISPOSE( hintData);
  return hintData;
}

