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
/*
 * Communication storage system. Ye Jobo.
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "merc.h"

HISTORY_DATA * history_free;
HISTORY_DATA * stored_tells;

void init_channel_storage()
{
  int i, j;

  for (i = 1; i < STORE_MAX; i++)
  {
    for (j = 1; j <= MAX_STORAGE; j++)
    {
      old_messages[i].last[j] = str_dup("");
    }
  }
}

void update_channel_storage(char *buf, int chan)
{
  char xbuf[MAX_STRING_LENGTH];
  char tbuf[20];
  int i;

  if (chan != STORE_TELL)
  {
    xprintf(tbuf, "%-18.18s", buf);
    xprintf(xbuf, "%s %s", tbuf, line_indent(&buf[19], 15, 85));
  }
  else xprintf(xbuf, buf);

  if (chan >= STORE_MAX)
  {
    bug("update_channel_storage : Bad channel", 0);
    return;
  }

  for (i = MAX_STORAGE; i > 1; i--)
  {
    free_string(old_messages[chan].last[i]);
    old_messages[chan].last[i] = str_dup(old_messages[chan].last[i-1]);
  }
  free_string(old_messages[chan].last[1]);
  old_messages[chan].last[1] = str_dup(xbuf);
}

void do_lastmessage(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];

  one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (!str_cmp(arg, "chat")) showchannel(ch, STORE_CHAT);
  else if (!str_cmp(arg, "newbie")) showchannel(ch, STORE_NEWBIE);
  else if (!str_cmp(arg, "flame")) showchannel(ch, STORE_FLAME);
  else if (!str_cmp(arg, "roleplay")) showchannel(ch, STORE_ROLEPLAY);
  else if (!str_cmp(arg, "tell") && ch->level > 6) showchannel(ch, STORE_TELL);
  else if (ch->level < 7) send_to_char("Show the last chat/newbie/flame/roleplay messages ?\n\r", ch);
  else send_to_char("Show the last chat/newbie/flame/roleplay/tell messages ?\n\r", ch);
  return;
}

void showchannel(CHAR_DATA *ch, int chan)
{
  char buf[MAX_STRING_LENGTH];
  int i;

  for (i = MAX_STORAGE; i > 0; i--)
  {
    if (strlen(old_messages[chan].last[i]) < 1) continue;
    xprintf(buf, "%s\n\r", old_messages[chan].last[i]);
    send_to_char(buf, ch);
  }
}

void init_tell_storage()
{
  HISTORY_DATA *history_new;
  HISTORY_DATA *history_prev = NULL;
  int i;

  for (i = 0; i < MAX_STORAGE; i++)
  {
    if (history_free == NULL)
    {
      history_new = alloc_perm(sizeof(*history_new));
    }
    else
    {
      history_new  = history_free;
      history_free = history_free->next;
    }
    history_new->message = str_dup("");
    history_new->next = history_prev;
    history_prev = history_new;
  }
  stored_tells = history_new;
}

/*
char *makedrunk ( char *string, CHAR_DATA * ch )
{
    struct struckdrunk drunk[] = {
        {3, 10,
         {"a", "a", "a", "A", "aa", "ah", "Ah", "ao", "aw", "oa", "ahhhh"}},
        {8, 5,
         {"b", "b", "b", "B", "B", "vb"}},
        {3, 5,
         {"c", "c", "C", "cj", "sj", "zj"}},
        {5, 2,
         {"d", "d", "D"}},
        {3, 3,
         {"e", "e", "eh", "E"}},
        {4, 5,
         {"f", "f", "ff", "fff", "fFf", "F"}},
        {8, 2,
         {"g", "g", "G"}},
        {9, 6,
         {"h", "h", "hh", "hhh", "Hhh", "HhH", "H"}},
        {7, 6,
         {"i", "i", "Iii", "ii", "iI", "Ii", "I"}},
        {9, 5,
         {"j", "j", "jj", "Jj", "jJ", "J"}},
        {7, 2,
         {"k", "k", "K"}},
        {3, 2,
         {"l", "l", "L"}},
        {5, 8,
         {"m", "m", "mm", "mmm", "mmmm", "mmmmm", "MmM", "mM", "M"}},
        {6, 6,
         {"n", "n", "nn", "Nn", "nnn", "nNn", "N"}},
        {3, 6,
         {"o", "o", "ooo", "ao", "aOoo", "Ooo", "ooOo"}},
        {3, 2,
         {"p", "p", "P"}},
        {5, 5,
         {"q", "q", "Q", "ku", "ququ", "kukeleku"}},
        {4, 2,
         {"r", "r", "R"}},
        {2, 5,
         {"s", "ss", "zzZzssZ", "ZSssS", "sSzzsss", "sSss"}},
        {5, 2,
         {"t", "t", "T"}},
        {3, 6,
         {"u", "u", "uh", "Uh", "Uhuhhuh", "uhU", "uhhu"}},
        {4, 2,
         {"v", "v", "V"}},
        {4, 2,
         {"w", "w", "W"}},
        {5, 6,
         {"x", "x", "X", "ks", "iks", "kz", "xz"}},
        {3, 2,
         {"y", "y", "Y"}},
        {2, 9,
         {"z", "z", "ZzzZz", "Zzz", "Zsszzsz", "szz", "sZZz", "ZSz", "zZ", "Z"}}
    };

    char buf[MSL];
    char temp;
    int pos = 0;
    int drunklevel;
    int randomnum;

    if ( ch == NULL )
        drunklevel = 5;
    else
    if ( IS_NPC ( ch ) )
        drunklevel = 0;
    else
        drunklevel = ch->pcdata->condition[COND_DRUNK];

    if ( drunklevel > 0 )
    {
        do
        {
            temp = toupper ( *string );
            if ( ( temp >= 'A' ) && ( temp <= 'Z' ) )
            {
                if ( drunklevel > drunk[temp - 'A'].min_drunk_level )
                {
                    randomnum =
                        number_range ( 0, drunk[temp - 'A'].number_of_rep );
                    strcpy ( &buf[pos],
                             drunk[temp - 'A'].replacement[randomnum] );
                    pos += strlen ( drunk[temp - 'A'].replacement[randomnum] );
                }
                else
                    buf[pos++] = *string;
            }
            else
            {
                if ( ( temp >= '0' ) && ( temp <= '9' ) )
                {
                    temp = '0' + number_range ( 0, 9 );
                    buf[pos++] = temp;
                }
                else
                    buf[pos++] = *string;
            }
        }
        while ( *string++ );
        buf[pos] = '\0';       
        strcpy ( string, buf );
        return ( string );
    }
    return ( string );
}
*/

char *drunktalk(char *argument)
{
  static char buf[MAX_STRING_LENGTH];
  char *ptr;
  int i, length;

  struct spk_type
  {
    char * old;
    char * new;
  };

  static const struct spk_type spk_table[] =
  {
    { " ",  " "   },
    { "is", "ish" },
    { "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
    { "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
    { "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
    { "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
    { "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
    { "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
    { "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
    { ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
    { ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
    { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
    { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
    { "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
  };

  buf[0] = '\0';

  for (ptr = argument; *ptr != '\0'; ptr += length)
  {
    for (i = 0; spk_table[i].old[0] != '\0'; i++)
    {
      if (!str_prefix(spk_table[i].old, ptr))
	 {
        strcat(buf, spk_table[i].new);

        if (number_range(1, 5) == 1 && str_cmp(spk_table[i].new, " "))
          strcat(buf, spk_table[i].new);
        else if (!str_cmp(spk_table[i].new, " "))
        {
          if (number_range(1, 5) == 1 && strlen(buf) < MAX_INPUT_LENGTH)
            strcat(buf, "*hic* ");
        }
        break;
      }
    }

    length = UMAX(1, strlen(spk_table[i].old));
  }

  buf[0] = UPPER(buf[0]);

  for (i = 1; buf[i] != '\0'; i++)
  {
    if (number_range(1, 3) == 1)
      buf[i] = UPPER(buf[i]);
  }

  return buf;
}


void talk_channel( CHAR_DATA *ch, char *argument, int channel, int sub_channel, const char *verb )
{
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;

  if (RTIMER(ch->in_room, RTIMER_SILENCE) != 0)
  {
    send_to_char("Something prevents you from speaking in this room.\n\r", ch);
    return;
  }
  if ( argument[0] == '\0' )
  {
    xprintf( buf, "%s what?\n\r", verb );
    buf[0] = UPPER(buf[0]);
    send_to_char(buf,ch);
    return;
  }
  if ( IS_HEAD(ch, LOST_TONGUE) )
  {
    xprintf( buf, "You can't %s without a tongue!\n\r", verb );
    send_to_char( buf, ch );
    return;
  }
  if ( IS_EXTRA(ch, GAGGED) )
  {
    xprintf( buf, "You can't %s with a gag on!\n\r", verb );
    send_to_char( buf, ch );
    return;
  }
  
  argument = drunktalk ( argument );

  REMOVE_BIT(ch->deaf, channel);
  switch ( channel )
  {
    default:
      if (ch->flag4 == 1)
      {
        xprintf( buf, "You whine '#1%s#n'.\n\r",  argument );
        send_to_char( buf, ch );
        xprintf( buf, "%s #Pwhines#n '#1$t#n'.",ch->name );
      }
      else if (channel == CHANNEL_FLAME)
      {
        xprintf( buf, "You %s '#G%s#n'.\n\r", verb, argument );
        send_to_char( buf, ch );
        xprintf( buf, "%s %ses '#G$t#n'.",ch->name, verb );
      }
      else if (channel == CHANNEL_CHAT)
      {
      	argument = pcolor ( ch, argument, 0 );
        xprintf( buf, "You %s '#W%s#n'.\n\r", verb, argument );
        send_to_char( buf, ch );
        if (ch->trust > 6)
          xprintf( buf, "#Y(#G*#Y)#C%s#Y(#G*#Y)#n '#1$t#n'.", ch->name);
        else if (IS_NPC(ch))
          xprintf( buf, "%s chats '#W$t#n'.", ch->short_descr);
        else if (IS_CLASS(ch, CLASS_WEREWOLF))
          xprintf( buf, "%s barks '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_MAGE))
          xprintf( buf, "%s chants '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
          xprintf( buf, "%s whispers '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_VAMPIRE))
          xprintf( buf, "%s snarls '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_DRONE))
          xprintf( buf, "%s drools '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_ANGEL))
          xprintf( buf, "%s preaches '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_TANARRI))
          xprintf( buf, "%s booms '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_LICH))
          xprintf( buf, "%s squicks '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_DEMON))
          xprintf( buf, "%s growls '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_CYBORG))
          xprintf( buf, "%s chitters '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_GIANT))
          xprintf( buf, "%s rumbles '#W$t#n'.",ch->name);
        else if (IS_CLASS(ch, CLASS_SKYBLADE))
          xprintf( buf, "%s hollars '#W$t#n'.",ch->name);
        else  
          xprintf( buf, "%s %ss '#W$t#n'.", ch->name, verb );
      }
      else
      {
        xprintf( buf, "You %s '#W%s#n'.\n\r", verb, argument );
        send_to_char( buf, ch );
        xprintf( buf, "%s %ss '#W$t#n'.",ch->name,     verb );
      }
      break;
    case CHANNEL_IMMTALK:
      xprintf( buf, "#Y.:#P%s#Y:.#C $t.#n",ch->name);
      act( buf, ch, argument, NULL, TO_CHAR );
      break;
    case CHANNEL_CLASS:
      switch(sub_channel)
      {
        case CC_ANGEL:
          xprintf( buf, "#0[#7%s#0]#C '$t'.#n",ch->name );
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_DEMON:
          xprintf( buf, "#0[#R%s#0]#C '$t'.#n",ch->name );
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_MAGE:
          xprintf( buf, "#n{{#0%s#n}}#C '$t'.#n",ch->name );
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_SHIFTER:
          xprintf( buf, "#G*#C(>#R%s#C<)#G* #C'$t'.#n",ch->name );
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_CYBORG:
          xprintf( buf, "#p{#0-#p}#0%s#p{#0-#p} #C'$t'.#n",ch->name);
          act( buf,ch,argument,NULL,TO_CHAR);
          break;
        case CC_WW:
          xprintf( buf, "#Y((#L%s#Y))#C '$t'.#n",ch->name );
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_NINJA:
          xprintf(buf, "#Y-*(#0%s#Y)*-#C '$t'.#n",ch->name );
          act(buf, ch, argument, NULL, TO_CHAR);
          break;
        case CC_SAMURAI:
          xprintf( buf, "#C-=#R%s#C=-  '$t'.#n",ch->name );
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_KNIGHT:
          xprintf( buf, "#0.x.#7%s#0.x.#C '$t'.#n",ch->name);
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_FAE:
          xprintf( buf, "#G>>#R(#Y%s#R)#G<<#C '$t'.#n",ch->name);
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_GIANT:
          xprintf( buf, "#G<:>#o%s#G<:>#C '$t'.#n",ch->name);
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_TANARRI:
          xprintf( buf, "#Y{#R%s#Y}#C '$t'.#n",ch->name);
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_LICH:
          xprintf( buf, "#G>*<#7%s#G>*<#C '$t'.#n",ch->name);
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_DRONE:
          xprintf( buf, "#G<#0=#Y{#0%s#Y}#0=#G> #C'$t'.#n", ch->name);
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_SKYBLADE:
          xprintf( buf, "#B=#R*#w>#0%s#w<#R*#B= #W'$t#W'.#n", ch->name);
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_DROW:
          xprintf( buf, "#P.o0#0%s#P0o.#C '$t'.#n",ch->name);
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_MONK:
          xprintf( buf, "#0.x[#l%s#0]x. #C '$t'.#n",ch->name );
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
        case CC_VAMPIRE:
          xprintf( buf, "#R<<#0%s#R>>#C $t.#n" , ch->name);
          act( buf, ch, argument, NULL, TO_CHAR );
          break;
      }
      break;      
    case CHANNEL_ROLEPLAY:
      xprintf( buf, "#o(#R%s#o) #n'#Y$t#n'.", ch->name);
      act( buf, ch, argument, NULL, TO_CHAR ); 
      break;
    case CHANNEL_RELIGION:
      xprintf( buf, religion_table[ch->pcdata->religion].channel, ch->name );
      act( buf, ch, argument, NULL, TO_CHAR );
      break;
  }

  /* silenced, and they don't know it :) */
  if (!IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE)) return;

  /* anti spamming */
  //if (!IS_NPC(ch) && (channel == CHANNEL_CHAT || channel == CHANNEL_NEWBIE))
  //{
  //  if (!str_cmp(ch->pcdata->last_global, argument)) return;
   // free_string(ch->pcdata->last_global);
   // ch->pcdata->last_global = str_dup(argument);
  //}

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    CHAR_DATA *gch;

    if (d->connected != CON_PLAYING) continue;
    if ((gch = d->character) == NULL) continue;
    if (gch == ch) continue;
    if (IS_SET(gch->deaf, channel)) continue;

    if (channel == CHANNEL_IMMTALK && !IS_IMMORTAL(gch)) continue;
    if (channel == CHANNEL_CLASS && !IS_IMMORTAL(gch) && (gch->class != ch->class || gch->level < 3))
      continue;
    if (channel == CHANNEL_RELIGION && !IS_IMMORTAL(gch) &&
      gch->pcdata->religion != ch->pcdata->religion) continue;
    if (channel == CHANNEL_YELL && gch->in_room && gch->in_room->area != ch->in_room->area)
      continue;

    if (!IS_NPC(gch) && !IS_NPC(ch))
    {
      bool ignore = FALSE;
      int i;
 
      for (i = 0; i < MAX_IGNORE; i++)
        if (gch->pcdata->ignore[i] == ch->pcdata->playerid) ignore = TRUE;
  
      if (ignore) continue;
    }

    act(buf, ch, argument, gch, TO_VICT);
  }
  return;  
}


HISTORY_DATA *generate_history()
{
  HISTORY_DATA *history_new;
  HISTORY_DATA *history_prev = NULL;
  int i;

  for (i = 0; i < MAX_HISTORY; i++)
  {
    if (history_free == NULL)
    {
      history_new = alloc_perm(sizeof(*history_new));
    }
    else
    {
      history_new  = history_free;
      history_free = history_free->next;
    }
    history_new->message = STRALLOC("");
    history_new->next = history_prev;
    history_prev = history_new;
  }
  return history_new;
}

void strip_history(CHAR_DATA *ch)
{
  HISTORY_DATA *pHistory;
  HISTORY_DATA *pHistory_next;

  for (pHistory = ch->pcdata->history; pHistory; pHistory = pHistory_next)
  {
    pHistory_next = pHistory->next;

    free_string(pHistory->message);
    pHistory->next = history_free;
    history_free = pHistory;    
  }
}

void do_history(CHAR_DATA *ch, char *argument)
{
  HISTORY_DATA *history;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  for (history = ch->pcdata->history; history; history = history->next)
  {
    if (strlen(history->message) > 3)
    {
      xprintf(buf, " %s\n\r", history->message);
      send_to_char(buf, ch);
    }
  }
}

void update_history(CHAR_DATA *ch, CHAR_DATA *talker, char *buf, char *argument, bool tell)
{
  char message[MAX_STRING_LENGTH];
  char tbuf[MAX_STRING_LENGTH];
  char xbuf[MAX_STRING_LENGTH];
  int i = 0;

  if (IS_NPC(ch)) return;

  xprintf(xbuf, "%s", line_indent(strip_ansi(argument), 22, 80));

  if (!tell)
  {
    xprintf(tbuf, strip_ansi(buf));
    while (tbuf[i] != '\0' && tbuf[i] != ' ') i++;
    tbuf[i] = '\0';
    xprintf(message, "#C%-18s #0: #R%-s#n", tbuf, xbuf);
  }
  else
  {
    xprintf(message, "#0[#GT#0] #C%-14s #0: #R%-s#n", talker->name, xbuf);
  }

  attach_history(ch, message);
  return;
}

void attach_history(CHAR_DATA *ch, char *message)
{
  HISTORY_DATA *history;
  HISTORY_DATA *history_prev1 = NULL;
  HISTORY_DATA *history_prev2 = NULL;

  if (history_free == NULL)
  {
    history = alloc_perm(sizeof(*history));
  }
  else
  {
    history      = history_free;
    history_free = history_free->next;
  }
  history->message = str_dup(message);
  history->next = ch->pcdata->history;
  ch->pcdata->history = history;

  for (history = ch->pcdata->history; history; history = history->next)
  {
    if (history_prev1) history_prev2 = history_prev1;
    history_prev1 = history;
  }
  history_prev2->next = NULL;
  free_string(history_prev1->message);
  history_prev1->next = history_free;
  history_free = history_prev1;
}

/*
 * Will remove the last line in the current note,
 * if there is a note, and it has a line to remove.
 */
void delete_last_line_in_note(CHAR_DATA *ch)
{
  char buf[4 * MAX_STRING_LENGTH];
  char *ptr;
  bool found = FALSE;
  int nCount = 0;

  buf[0] = '\0';

  if (IS_NPC(ch)) return;
  if (ch->pcdata->in_progress->text == NULL)
  {
    send_to_char("No note to delete lines in.\n\r", ch);
    return;
  }
  if (strlen(ch->pcdata->in_progress->text) < 1)
  {
    send_to_char("Empty note, nothing to delete.\n\r", ch);
    return;
  }
  strcpy(buf, ch->pcdata->in_progress->text);
  ptr = buf;
  while (*ptr != '\0')
  {
    if (*ptr == '\n') nCount++;
    ptr++;
  }
  if (nCount == 1)
  {
    free_string(ch->pcdata->in_progress->text);
    ch->pcdata->in_progress->text = NULL;
    send_to_char("Entire note deleted.\n\r", ch);
    return;
  }
  else
  {
    while (*ptr != '\n' || !found)
    {
      if (*ptr == '\n') found = TRUE;
      ptr--;
    }
  }
  ptr++;
  *ptr = '\0';
  free_string(ch->pcdata->in_progress->text);
  ch->pcdata->in_progress->text = str_dup(buf);
  send_to_char("Line deleted.\n\r", ch);
}
