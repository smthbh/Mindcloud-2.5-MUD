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
#include <time.h>
#include "merc.h"
#include "interp.h"


AREA_AFFECT * area_affect_free = NULL;

bool gFound;

#define RID ROOM_INDEX_DATA

bool  examine_room         args (( RID *pRoom, RID *tRoom, AREA_DATA *pArea, int steps ));
void examine_steps         args (( RID *pRoom, RID *tRoom, AREA_DATA *pArea, int steps ));
void  dijkstra             args (( RID *chRoom, RID *victRoom ));
RID  *heap_getMinElement   args (( HEAP *heap ));
HEAP *init_heap            args (( RID *root ));

/*
 * Is astr contained within bstr ?
 *
 */
bool is_contained( const char *astr, const char *bstr )
{
  int i, alen, blen, count;
   
  alen = strlen(astr);
  blen = strlen(bstr);
  if (alen > blen) return FALSE;
  for (i = 0; i <= (blen - alen); i++)
  {
    count = 0;
    while (count < alen && UPPER(astr[count]) == UPPER(bstr[i+count])) count++;
    if (count == alen) return TRUE;
  }
  return FALSE;
}

/*
 * A case sensitive version
 */
bool is_contained2(const char *astr, const char *bstr)
{
  int i, alen, blen, count;

  alen = strlen(astr);
  blen = strlen(bstr);
  if (alen > blen) return FALSE;
  for (i = 0; i <= (blen - alen); i++)
  {
    count = 0;
    while (count < alen && astr[count] == bstr[i+count]) count++;
    if (count == alen) return TRUE;
      }
  return FALSE;
    }

int strlen2(const char *s)
{
  int i, b, count=0;

  if (s[0] == '\0') return 0;
  b = strlen(s);
  for (i = 0; i < b; i++)
  {
    if (s[i] == '#') count++;
  }
  return (b + 7 * count);
}



void win_prize( CHAR_DATA *ch )
{
  int i,vnum;
  OBJ_DATA *obj;
  OBJ_INDEX_DATA *pIndex;
  
  if (IS_NPC(ch)) return;
  i = number_range(1,100);
  if (i < 50) vnum = OBJ_VNUM_PROTOPLASM;
  else if (i < 65) vnum = 33851;
  else if (i < 75) vnum = 33852;
  else if (i < 80) vnum = 33853;
  else if (i < 85) vnum = 33854;
  else if (i < 90) vnum = 33855;
  else if (i < 95) vnum = 33856;
  else vnum = 33857;
  if ((pIndex = get_obj_index(vnum)) == NULL)
  {
    bug("BAD PRIZE!!",0);
    return;
  }
  obj = create_object(pIndex, 50);
  if (vnum == OBJ_VNUM_PROTOPLASM)
  {
    obj->level = 1;
    free_string(obj->short_descr);
    free_string(obj->name);
    free_string(obj->description);
    obj->short_descr = str_dup("A prize token");
    obj->description = str_dup("A token lies on the floor");
    obj->name = str_dup("prize token");
    obj->value[0] = number_range(100,300);
    obj->item_type = ITEM_QUEST;
  }
  obj_to_char(obj,ch);
  return;
}

void do_clearstats2( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
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
    if (IS_SET(ch->newbits, NEW_DFORM)) REMOVE_BIT(ch->newbits, NEW_DFORM);
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    if (IS_SET(ch->newbits2, NEW2_EAGLES)) REMOVE_BIT(ch->newbits2, NEW2_EAGLES);
    if (IS_SET(ch->newbits2, NEW2_VVIGOR)) REMOVE_BIT(ch->newbits2, NEW2_VVIGOR);
    if (IS_SET(ch->newbits, NEW_CLOUDBLESS)) REMOVE_BIT(ch->newbits, NEW_CLOUDBLESS);
    if (IS_SET(ch->newbits2, NEW2_HAWKEYES)) REMOVE_BIT(ch->newbits2, NEW2_HAWKEYES);
    save_char_obj( ch );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",ch);
    return;
}

/*
 *  If an item have a bad char in it's short/long/name desc, the mud
 *  will freeze when the char with that item tries to log on, so use
 *  this check on any renaming of items (glamour/reshape/etc).
 */
bool has_bad_chars(CHAR_DATA *ch, char *argument)
{
  int i;
  
  if ( argument[0] == '\0' ) return FALSE;
  for (i = 0; argument[i] != '\0' ; i++)  
  {
    if (argument[i] == '~') return TRUE;
  }
  return FALSE;
}

bool is_webbed(CHAR_DATA *ch)
{
  if (IS_AFFECTED(ch, AFF_WEBBED))
  {
    send_to_char("Not while webbed.\n\r", ch);
    return TRUE;
  }
  if (IS_AFFECTED2(ch, AFF2_THORNS))
  {
    send_to_char("Not while rooted by brambles.\n\r", ch);
    return TRUE;
  }
  return FALSE;
}

void ragnarok_stop()
{
  DESCRIPTOR_DATA *d;
  
  ragnarok = FALSE;
  do_info(NULL,"#CPeace has been restored in the realms, the time of ragnarok is no more#n");
  for (d = descriptor_list; d != NULL; d = d->next)
  {
    if (d->character && d->connected == CON_PLAYING)
    {
      d->character->fight_timer = 0;
      d->character->pcdata->safe_counter = 5;
      do_call(d->character,"all");
      do_restore(d->character,"self");
    }
  }
  return;
}


void powerdown(CHAR_DATA *ch)
{
  if (IS_NPC(ch)) return;

  if (IS_CLASS(ch, CLASS_DEMON))
  {
    if (IS_POLYAFF(ch, POLY_ZULOFORM))  /* demonform */
    {
      REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      free_string( ch->morph );
      ch->morph = str_dup( "" );
    }
  }
  else if (IS_CLASS(ch, CLASS_MONK)) /* spiritpower and monkflame */
  {
    if (IS_SET(ch->newbits, NEW_MONKFLAME)) REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
    if (IS_SET(ch->newbits, NEW_POWER))     REMOVE_BIT(ch->newbits, NEW_POWER);
  }
  else if (IS_CLASS(ch, CLASS_WEREWOLF))
  {
    if (IS_SET(ch->special, SPC_WOLFMAN))
      do_unwerewolf(ch, "");
  }
  else if (IS_CLASS(ch, CLASS_DROW))
  {
    if (IS_SET(ch->newbits,NEW_DFORM)) /* spiderform */
    {
      free_string(ch->morph);
      ch->morph = str_dup("");
      REMOVE_BIT(ch->newbits,NEW_DFORM);
      REMOVE_BIT(ch->newbits,THIRD_HAND);
      REMOVE_BIT(ch->newbits,FOURTH_HAND);
    }
  }
  else if (IS_CLASS(ch, CLASS_VAMPIRE))
  {
    if (IS_EXTRA(ch, EXTRA_DRAGON)) /* dragonform */
    {
      REMOVE_BIT(ch->extra,EXTRA_DRAGON);
      free_string( ch->morph );
      ch->morph = str_dup( "" );
    }
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) /* zuloform */
    {
      REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      free_string( ch->morph );
      ch->morph = str_dup( "" );
    }
  }
  else if (IS_CLASS(ch, CLASS_GIANT))
  {
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) /* dawnstrength */
    {
      REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      free_string(ch->morph);
      ch->morph=str_dup("");
    }
  }
  else if (IS_CLASS(ch, CLASS_TANARRI))
  {
    if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)  /* fury */
      ch->pcdata->powers[TANARRI_FURY_ON] = 0;
  }
  else if (IS_CLASS(ch, CLASS_DRONE)) /* nightmare aura */
  {
    if (ch->pcdata->powers[DRONE_NIGHTMARE] == 1)
      ch->pcdata->powers[DRONE_NIGHTMARE] = 0;
  }
  else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    if (IS_SET(ch->affected_by, AFF_POLYMORPH)) /* reset form */
    {
      ch->pcdata->powers[SHAPE_FORM]=0;
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      free_string(ch->morph);
      ch->morph=str_dup("");
    }
  }
  else if (IS_CLASS(ch, CLASS_CYBORG))
  {
    if (IS_SET(ch->newbits, NEW_CUBEFORM))  /* avatar of Lloth */
    {
      REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);  
      free_string(ch->morph);
      ch->morph=str_dup("");
    }
  }
  else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
  {
    if (IS_SET(ch->pcdata->powers[AURAS], MIGHT_AURA))  /* aura of might */
      REMOVE_BIT(ch->pcdata->powers[AURAS], MIGHT_AURA);
  }
  else if (IS_CLASS(ch, CLASS_ANGEL))
  {
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) /* godly favor */
    {
      REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      free_string(ch->morph);
      ch->morph=str_dup("");
    }
  }
  else if (IS_CLASS(ch, CLASS_FAE))
  {
    if (IS_SET(ch->newbits, NEW_CUBEFORM))  /* elemental form */
    {
      REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      if (IS_SET(ch->affected_by, AFF_PASS_DOOR)) REMOVE_BIT(ch->affected_by, AFF_PASS_DOOR);
      free_string(ch->morph);
      ch->morph=str_dup("");
    }
  }

  /* and a little bit of everything */
  REMOVE_BIT(ch->pcdata->jflags, JFLAG_DRONE_HORRID);
  return;
}

int get_ratio(CHAR_DATA *ch)
{
  long ratio;

  if (IS_NPC(ch)) return 0;
  if ((ch->pkill + ch->pdeath) == 0) ratio = 0; // to avoid divide by zero.
  else if (ch->pkill > ch->pdeath)
    ratio = ch->pkill * 100 * ((ch->pkill * ch->pkill) - (ch->pdeath * ch->pdeath))/((ch->pkill + ch->pdeath) * (ch->pkill + ch->pdeath));
  else if (ch->pkill > 0)
    ratio = (-100) * (ch->pdeath - ch->pkill) / ch->pkill;
  else
    ratio = (-100) * ch->pdeath;
  return (int) ratio;
}

bool multicheck(CHAR_DATA *ch)
{
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];

  if (ch->level > 6) return FALSE;

  for (gch = char_list; gch; gch = gch->next)
  {
    if (IS_NPC(gch)) continue;
    if (gch == ch) continue;
    if (gch->level > 6) continue;
    if (strlen(gch->lasthost) > 2)
    {
      if (gch->desc)
      {
        if (!str_cmp(gch->desc->host, ch->desc->host))
        {
          xprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
          log_string(LOG_CONNECT, buf);
          //do_info(ch, buf);
          return TRUE;
        }
      }
      else if (!str_cmp(gch->lasthost, ch->desc->host))
      {
        xprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
        log_string(LOG_CONNECT, buf);
        return TRUE;
      }
    }
  }
  return FALSE;
}



void death_info(char *str)
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];

  if (str[0] == '\0') return;

  xprintf(buf, "#0/\\/\\ #RDe#Ya#Rth #0/\\/\\#n %s\n\r", str);

  for (d = descriptor_list; d != NULL; d = d->next)
  {
    if (d->connected == CON_PLAYING && d->character != NULL)
      send_to_char(buf, d->character);
  }
  return;
}

void avatar_info(char *str)
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];

  if (str[0] == '\0') return;

  xprintf(buf, "#p-=#P[ #WAvatar #P]#p=-#n %s\n\r", str);

  for (d = descriptor_list; d != NULL; d = d->next) 
  {  
    if (d->connected == CON_PLAYING && d->character != NULL)                                              
      send_to_char(buf, d->character);
  }  
  return;
}

void leave_info(char *str)
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];

  if (str[0] == '\0') return;
  
  xprintf(buf, "#G+#g-#G+ #0Leaves #G+#g-#G+#n %s\n\r", str);

  for (d = descriptor_list; d != NULL; d = d->next) 
  {  
    if (d->connected == CON_PLAYING && d->character != NULL)                                              
      send_to_char(buf, d->character);
  }  
  return;
}

void enter_info(char *str)
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];

  if (str[0] == '\0') return;

  xprintf(buf, "#0=#w-#0= #WEnters #0=#w-#0= %s\n\r", str);

  for (d = descriptor_list; d != NULL; d = d->next) 
  {  
    if (d->connected == CON_PLAYING && d->character != NULL)                                              
      send_to_char(buf, d->character);
  }  
  return;
}


void forge_affect(OBJ_DATA *obj, int value)
{
  AFFECT_DATA paf;

  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_HITROLL;
  paf.modifier       = value;   
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);
    
  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_DAMROLL;
  paf.modifier       = value;
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);
}

void dump_last_command()
{
  FILE *fp;
  char buf[MAX_STRING_LENGTH];

  fp = fopen("../txt/crash.txt","a");
  if (cmd_done)
    fprintf (fp,"Last command typed : %s  (thread count : %d) (command executed without flaws)\n",last_command, thread_count);
  else
    fprintf (fp,"Last command typed : %s  (thread count : %d) (crash happended during this command)\n",last_command, thread_count);
  fflush(fp);
  fclose(fp);

  /*
   * creates a note to the immortals
   */
  xprintf(buf, "It seems we have crashed, the last command typed was\n\r\n\r");
  strcat(buf, last_command);
  strcat(buf, "\n\r\n\rPlease remember that this doesn't mean that this caused the crash.\n\r\n\rRegards,\n\r\n\rThe Crash Code");
  make_note("Immortal", "Crash Code", "imm", "We Crashed", 7, buf);
} 

void update_revision(CHAR_DATA *ch)
{
  if (IS_NPC(ch)) return;
  if (ch->pcdata->revision == CURRENT_REVISION) return;

  /*
   * We don't end cases with break, since we want the player to be fully updated.
   */
  switch (ch->pcdata->revision)
  {
    case 1:
      ch->pcdata->revision++;
    default:
      break;
  }
  return;
}

bool in_fortress(CHAR_DATA *ch)
{
  if (!ch->in_room) return FALSE;
  if (ch->in_room->vnum >= 151 && ch->in_room->vnum <= 170) return TRUE;
  return FALSE;
}

bool in_arena(CHAR_DATA *ch)
{
  if (!ch->in_room) return FALSE;
  if (ch->in_room->vnum >= 50 && ch->in_room->vnum <= 67) return TRUE;
  return FALSE;
}

void increase_total_output(int clenght)
{
  total_output += clenght;
}

void logout_message(CHAR_DATA *ch)
{
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };
 
  DESCRIPTOR_DATA *d;
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;
  int size;
  
  size = strlen2(ch->pcdata->logoutmessage);
  if (size > 380)
  {
    bug("Bad logoutmessage.",0);
    return;
  }
    
  ptr2  = "#C<- #RLeaves #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->logoutmessage;
  
  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;
   
  while (*dmess != '\0')
  {
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(1, ch->sex, 2)]; break;
      case 'm': i = him_her [URANGE(1, ch->sex, 2)]; break;
      case 's': i = his_her [URANGE(1, ch->sex, 2)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r';
  
  for (d = descriptor_list; d; d = d->next)   
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
}
  void update_mudinfo()
{
  DESCRIPTOR_DATA *d;
  int i, pcount = 0;

  /*
   * Each week, the data is stored to a file, and
   * the variable cleared.
   */
  if (mudinfo[MUDINFO_UPDATED] > 20160)
  {
    write_mudinfo_database();
    for (i = 0; i < (MUDINFO_MAX - 2); i++)
    {
      mudinfo[i] = 0;
    }
    log_string(LOG_GAME, "Mudinfo database updated.");
  }

  /* Increase update count */
  mudinfo[MUDINFO_UPDATED]++;

  /* Outdate the output data */

  if (total_output > mudinfo[MUDINFO_DATA_PEAK]) mudinfo[MUDINFO_DATA_PEAK] = total_output;

  /* The stored data */
  if (mudinfo[MUDINFO_BYTE] > 1048576) // 1 megabyte
  {
    mudinfo[MUDINFO_MBYTE]++;
    mudinfo[MUDINFO_BYTE] -= 1048576;
  }
  mudinfo[MUDINFO_BYTE] += total_output;
  
  /* The temp data */
  if (mudinfo[MUDINFO_BYTE_S] > 1048576) // 1 megabyte
  {
    mudinfo[MUDINFO_MBYTE_S]++;
    mudinfo[MUDINFO_BYTE_S] -= 1048576;
  }
  mudinfo[MUDINFO_BYTE_S] += total_output;
 
  /* We clear the counter */
  total_output = 0;

  for (d = descriptor_list; d; d = d->next)
  {
    if (d->connected == CON_PLAYING && d->lookup_status == STATUS_DONE) 
    {
      if (d->character)
      {
        if (d->character->level < 7)
        {
          pcount++;
          if (d->out_compress) mudinfo[MUDINFO_MCCP_USERS]++;
          else mudinfo[MUDINFO_OTHER_USERS]++;
          if (IS_SET(d->character->act, PLR_SOUND)) mudinfo[MUDINFO_MSP_USERS]++;
        }
      }
    }
  }  

  if (pcount > mudinfo[MUDINFO_PEAK_USERS]) mudinfo[MUDINFO_PEAK_USERS] = pcount;

  save_mudinfo();
}

void recycle_descriptors()
{
  DESCRIPTOR_DATA *dclose;
  DESCRIPTOR_DATA *dclose_next;

  for (dclose = descriptor_list; dclose; dclose = dclose_next)
  {
    dclose_next = dclose->next;
    if (dclose->lookup_status != STATUS_CLOSED) continue;

    /*
     * First let's get it out of the descriptor list.
     */
    if ( dclose == descriptor_list )
    {
      descriptor_list = descriptor_list->next;
    }
    else   
    { 
      DESCRIPTOR_DATA *d;
       
      for (d = descriptor_list; d && d->next != dclose; d = d->next)
        ;
      if (d != NULL)
        d->next = dclose->next;
      else
      {
        bug( "Recycle_descriptors: dclose not found.", 0 );
        continue;
      }
    }

    /*
     * Clear out that memory
     */
    free_string( dclose->host );
    free_mem( dclose->outbuf, dclose->outsize );
 
    /*
     * Mccp
     */
    if (dclose->out_compress)
    {
      deflateEnd(dclose->out_compress);
      free_mem(dclose->out_compress_buf, COMPRESS_BUF_SIZE);
      free_mem(dclose->out_compress, sizeof(z_stream));
    }

    /*     
     * Bye bye mr. Descriptor.
     */
    close( dclose->descriptor );

    /*   
     * And then we recycle
     */
    dclose->next        = descriptor_free;
    descriptor_free     = dclose;
  }
}

void affect_to_area(AREA_DATA *pArea, AREA_AFFECT *paf)
{
  AREA_AFFECT *paf_new;

  if (area_affect_free == NULL)
  {
    paf_new = alloc_perm( sizeof(*paf_new) );
  }
  else
  {
    paf_new          = area_affect_free;
    area_affect_free = area_affect_free->next;
  }

  *paf_new        = *paf;
  paf_new->next   = pArea->affects;
  pArea->affects  = paf_new;
  
  return;
}

void area_affect_remove(AREA_DATA *pArea, AREA_AFFECT *paf)
{
  if (pArea->affects == NULL)
  {
    bug( "Area_affect_remove: no affect.", 0 );
    return;
  }
  if (paf == pArea->affects)
  {
    pArea->affects = paf->next;
  }
  else
  {
    AREA_AFFECT *prev;

    for (prev = pArea->affects; prev; prev = prev->next)
    {
      if (prev->next == paf)
      {
        prev->next = paf->next;
        break;
      }
    }
    if (!prev)
    {
      bug( "Area_affect_remove: cannot find paf.", 0);
      return;
    }
  }

  paf->next        = area_affect_free;
  area_affect_free = paf;

  return;
} 

/*
 * A religion of 0 will result in any religion is true,
 * but a religion != 0 will only result in true if the spell
 * has that given religion.
 */
bool has_area_affect(AREA_DATA *pArea, int affect, int religion)
{
  AREA_AFFECT *paf;

  for (paf = pArea->affects; paf; paf = paf->next)
  {
    if (paf->type == affect)
    {
      if (religion != 0 && paf->religion != religion) continue;
      else return TRUE;
    }
  }
  return FALSE;
}

int get_next_playerid()
{
  top_playerid++;
  save_coreinfo();
  return top_playerid;
}

/*
 * Writes a string straight to stderr
 */

void log_string2(const char *str)
{
  char *strtime;

  strtime = ctime(&current_time);
  strtime[strlen(strtime)-1] = '\0';
  fprintf(stderr, "%s :: %s\n", strtime, str);

  return;
}

void tie_message(CHAR_DATA *ch, CHAR_DATA *victim)
{
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };
  DESCRIPTOR_DATA *d;
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;
  int size;
   
  size = strlen2(ch->pcdata->tiemessage);
  if (size > 380)
  {
    bug("Bad tiemessage.",0);
    return;
  }
  
  ptr2  = "#C<- #RTie #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->tiemessage;
   
  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;
   
  while (*dmess != '\0')
  {
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(1, ch->sex, 2)]; break;
      case 'm': i = him_her [URANGE(1, ch->sex, 2)]; break;
      case 's': i = his_her [URANGE(1, ch->sex, 2)]; break;
      case 'N': i = victim->name; break;
      case 'S': i = his_her [URANGE(1, victim->sex, 2)]; break;
      case 'M': i = him_her [URANGE(1, victim->sex, 2)]; break;
      case 'E': i = he_she  [URANGE(1, victim->sex, 2)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */   
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r';
  
  for (d = descriptor_list; d; d = d->next)
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
}

void login_message(CHAR_DATA *ch)
{
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };
   
  DESCRIPTOR_DATA *d;
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;
  int size;
  
  size = strlen2(ch->pcdata->loginmessage);
  if (size > 380)
  {
    bug("Bad loginmessage.",0); 
    return;
  }

  ptr2  = "#C<- #REnters #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->loginmessage;
      
  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;

  while (*dmess != '\0')
  {  
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(1, ch->sex, 2)]; break;
      case 'm': i = him_her [URANGE(1, ch->sex, 2)]; break;
      case 's': i = his_her [URANGE(1, ch->sex, 2)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r';
    
  for (d = descriptor_list; d; d = d->next)
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
}
void check_help_soundex(char *argument, CHAR_DATA *ch)
{
  HELP_DATA *pHelp;
  char buf[MAX_STRING_LENGTH];
  char tbuf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  char keyword[MAX_INPUT_LENGTH];
  char *str;
  bool found = FALSE;

  one_argument(argument, arg);

  if (arg[0] == '\0') return;

  xprintf(buf, "\n\r[Perhaps:");
  for (pHelp = first_help; pHelp; pHelp = pHelp->next)
  {
    if (pHelp->level > ch->level) continue;

    str = pHelp->keyword;
    str = one_argument(str, keyword);
    while (keyword[0] != '\0')
    {
      if (SoundexMatch(GetSoundexKey(arg), GetSoundexKey(keyword)) > 75)
      {
        found = TRUE;
        xprintf(tbuf, " %s", keyword);
        strcat(buf, tbuf);
      }
      str = one_argument(str, keyword);
    }
  }
  strcat(buf, "]\n\r");
  if (found) send_to_char(buf, ch);
}

/*
 * New system to replace status, called fair fight, it measures the
 * difference between two players, giving them points for their
 * stances, powers, and stats. If they are within each others range,
 * the call will return TRUE, if not FALSE. Call for fair_fight when
 * you need to see if a fight is fair (ie. decapping).
 */
bool fair_fight(CHAR_DATA *ch, CHAR_DATA *victim)
{

  return TRUE;
}

void special_decap_message(CHAR_DATA *ch, CHAR_DATA *victim)
{  
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };
  DESCRIPTOR_DATA *d;
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;
  int size;
   
  size = strlen2(ch->pcdata->decapmessage);
  if (size > 380)
  {
    bug("Bad decapmessage.",0);
    return;
  }

  ptr2  = "#C<- #RDeath #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->decapmessage;

  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;

  while (*dmess != '\0')
  {
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(1, ch->sex, 2)]; break;
      case 'm': i = him_her [URANGE(1, ch->sex, 2)]; break;
      case 's': i = his_her [URANGE(1, ch->sex, 2)]; break;
      case 'N': i = victim->name; break;
      case 'S': i = his_her [URANGE(1, victim->sex, 2)]; break;
      case 'M': i = him_her [URANGE(1, victim->sex, 2)]; break;
      case 'E': i = he_she  [URANGE(1, victim->sex, 2)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r';
  
  for (d = descriptor_list; d; d = d->next)
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
}






/*
 * This function have always been broken, perhaps
 * one day in the future I'll make a functionel
 * piece of code, but untill then, it's more
 * guessing the right size than anything else.
 */
char *get_dystopia_banner(char *title, int size)
{
  int tSize = strlen(title);
  int patternsize, bufferspaces = 0, blockcount, i;
  static char buf[200];

  /* just so we can use strcat */
  buf[0] = '\0';

  /* calculating the amount of patterns on each side of the title */
  patternsize = (size - tSize)/2;
  if (!(patternsize % 4))
    patternsize -= 4;
  else
    while(patternsize % 4) patternsize--;
  patternsize /= 4;

  /* calculating buffer spaces */
  while ((tSize + bufferspaces) % 4) bufferspaces++;
  blockcount = bufferspaces/2;

  if (patternsize < 1)
  {
    strcat(buf, "#0<>== #G");
    strcat(buf, title);
    strcat(buf, " #0==<>#n");  
  }
  else
  {
    /* first add patterns */
    strcat(buf, "#0<>==");
    for (i = 1; i < patternsize; i++)
      strcat(buf, "<>==");
    /* add the title */
    if (tSize)
    {
      strcat(buf, "#G ");
      while (bufferspaces > blockcount)
      {
        bufferspaces--;
        strcat(buf, " ");
      }
    }
    else strcat(buf, "<>");
    strcat(buf, title);
    if (tSize)
    {
      strcat(buf, "#0 ");
      while (bufferspaces > 0)
      {
        bufferspaces--;
        strcat(buf, " ");
      }
    }
    /* add secondary patterns */
    for (i = 0; i < patternsize; i++)
      strcat(buf, "==<>");
    strcat(buf, "#n");
  }

  return buf;
}

int calc_ratio(int a, int b)
{
  int ratio;

  if (b == 0 && a > 0) ratio = 100;
  else if ((a + b) != 0) ratio = (100*a)/(a+b);
  else ratio = 0;
  return ratio;
}
void avatar_message(CHAR_DATA *ch)
{  
  static char * const he_she  [] = { "XX", "he",  "she" };
  static char * const him_her [] = { "XX", "him", "her" };
  static char * const his_her [] = { "XX", "his", "her" };

  DESCRIPTOR_DATA *d; 
  char buf[400]; // that should be plenty.
  const char *dmess;
  const char *i;
  char *ptr2;
  char *ptr;  
  int size;

  size = strlen2(ch->pcdata->avatarmessage);
  if (size > 380)
  {
    bug("Bad avatarmessage.",0);
    return;
  }

  ptr2  = "#C<- #RAvatar #C->#n ";
  ptr   = buf;
  dmess = ch->pcdata->avatarmessage;
  
  while ((*ptr = *ptr2) != '\0')
    ++ptr, ++ptr2;
   
  while (*dmess != '\0')
  {
    if ( *dmess != '$' )
    {
      *ptr++ = *dmess++;
      continue;
    }
    ++dmess;
    switch (*dmess)
    {
      default:  i = ""; break;
      case 'n': i = ch->name; break;
      case 'e': i = he_she  [URANGE(0, ch->sex, 1)]; break;
      case 'm': i = him_her [URANGE(0, ch->sex, 1)]; break;
      case 's': i = his_her [URANGE(0, ch->sex, 1)]; break;
    }
    ++dmess;
    /* copying the data into the pointer */
    while ((*ptr = *i) != '\0')
      ++ptr, ++i;
  }
  *ptr++ = '\n';
  *ptr++ = '\r'; 
   
  for (d = descriptor_list; d; d = d->next)
  {
    if (d->lookup_status != STATUS_DONE) continue;
    if (d->connected != CON_PLAYING ) continue;
    write_to_buffer( d, buf, ptr - buf );
  }
  return;
} 
char *strip_ansi(char *str)
{
  static char buf[MAX_STRING_LENGTH];
  char *ptr;

  buf[0] = '\0';
  ptr = buf;

  while (*str != '\0')
  {
    if (*str != '#') *ptr++ = *str++;
    else if (*(++str) != '\0') str++;
  }
  *ptr = '\0';
  return buf;
}
char *line_indent(char *text, int wBegin, int wMax)
{
  static char buf[MAX_STRING_LENGTH];
  char *ptr;
  char *ptr2;
  int count = 0;
  bool stop = FALSE;
  int wEnd = 0;

  buf[0] = '\0';
  ptr = text;
  ptr2 = buf;

  while (!stop)
  {
    if (count == 0)
    {
      if (*ptr == '\0') wEnd = wMax - wBegin;
      else if (strlen(ptr) < (wMax - wBegin)) wEnd = wMax - wBegin;
      else
      {
        int x = 0;

        while (*(ptr + (wMax - wBegin - x)) != ' ') x++;
        wEnd = wMax - wBegin - (x - 1);
        if (wEnd < 1) wEnd = wMax - wBegin;
      }
    }
    if (count == 0 && *ptr == ' ') ptr++;
    else if (++count != wEnd)
    {
      if ((*ptr2++ = *ptr++) == '\0') stop = TRUE;
    }
    else if (*ptr == '\0')
    {
      stop = TRUE;
      *ptr2 = '\0';
    }
    else
    {
      int k;

      count = 0;
      *ptr2++ = '\n';
      *ptr2++ = '\r';
      for (k = 0; k < wBegin; k++)
        *ptr2++ = ' ';
    }
  }
  return buf;
}
char *get_exits(CHAR_DATA *ch)
{
  //extern char *const dir_name[];
  static char buf[MAX_STRING_LENGTH];
  EXIT_DATA *pexit;
  bool found;
  int door;

  buf[0] = '\0';

  if (!check_blind(ch)) return "";
 
  xprintf(buf, "#0[#GExits#w:#C");

  found = FALSE;
  for (door = 0; door <= 5; door++)
  {
    if ((pexit = ch->in_room->exit[door]) != NULL && pexit->to_room != NULL)
    {
      found = TRUE;
      if (IS_SET(pexit->exit_info, EX_CLOSED))
      {
        strcat(buf, " #0(#C");
        strcat(buf, dir_name[door]);
        strcat(buf, "#0)#C");
      }
      else
      {  
        strcat(buf, " ");
        strcat(buf, dir_name[door]);
      }
    }
  }
  if (!found)
    strcat(buf, " none");
  strcat(buf, "#0]#n\n\r");

  return buf;
}
void init_vt100(DESCRIPTOR_DATA *d, char *xbuf)
{
  CHAR_DATA *ch;
  char buf[MAX_STRING_LENGTH];
  int i;

  if ((ch = d->character) == NULL)
  {
    bug("Init_vt100: No character", 0);
    return;
  }
  if ((i = atoi(xbuf)) < 10)
  {
    send_to_char("VT100 Failed.\n\r", ch);
    return;
  }
  ch->pcdata->vt100_size = i;
  SET_BIT(ch->pcdata->tempflag, TEMP_VT100);
  xprintf(buf, "\e[%d;1H%s%s\e[1;1H%s%s\e[1;%dr",
    i, VT_CLEAR_LINE, VT_SAVECURSOR, VT_SETWIN_CLEAR, VT_CLEAR_SCREEN, i - 2);
  send_to_char(buf, ch);
  send_to_char("VT100 Initialized.\n\r", ch);
  return;
}

void recycle_dummys()
{
  DUMMY_ARG *dummy;
  DUMMY_ARG *dummy_next;
        
  for (dummy = dummy_list; dummy; dummy = dummy_next)
  {
    dummy_next = dummy->next;
    if (dummy->status == 1) continue;  // being used

    if (dummy == dummy_list)
    {
      dummy_list = dummy_list->next;
	  dummy->next = dummy_free;
      dummy_free = dummy;
    } 
    else
    {
      DUMMY_ARG *prev;
        
      /* we find the prev dummy arg */
      for (prev = dummy_list; prev && prev->next != dummy; prev = prev->next)
        ;
      if (prev != NULL)
        prev->next = dummy->next;
      else
      {
        bug( "Recycle_dymmys: dummy not found.", 0 );  
        continue;
      }
      
      /* recycle */  
      dummy->next = dummy_free;
      dummy_free = dummy;
    }
  }     
}

HEAP *init_heap(RID *root)
{
  AREA_DATA *pArea;
  RID *pRoom;
  HEAP * heap;
  int i, size, vnum;

  if ((pArea = root->area) == NULL) return NULL;
  size = pArea->uvnum - pArea->lvnum;

  if (size >= MAX_KNUDE)
  {
    bug("Init_heap: Size %d exceeds MAX_KNUDE", size);
    return NULL;
  }

  heap = calloc(1, sizeof(*heap));

  /* we want the root at the beginning */
  heap->knude[1]             = root;
  heap->knude[1]->steps      = 0;
  heap->knude[1]->heap_index = 1;

  /* initializing the rest of the heap */
  for (i = 2, vnum = pArea->lvnum; vnum < pArea->uvnum; vnum++)
  {
    if ((pRoom = get_room_index(vnum)))
    {
      if (pRoom == root) continue;
      heap->knude[i]             = pRoom;
      heap->knude[i]->steps      = 2 * MAX_KNUDE;
      heap->knude[i]->heap_index = i;
      i++;
    }
  }

  heap->iVertice = i-1;

  /* setting the rest to NULL */
  for (; i < MAX_KNUDE; i++)
    heap->knude[i] = NULL;

  return heap;
}

/*
 * Finds the smallest element and returns it after
 * making sure the heap is in perfect order after
 * the removal of the vertice with the smallest
 * element.
 */
RID *heap_getMinElement(HEAP *heap)
{
  RID *tRoom;
  RID *pRoom;
  bool done = FALSE;
  int i = 1;

  /* this is the element we wish to return */
  pRoom = heap->knude[1];

  if (pRoom->steps == 2 * MAX_KNUDE)
    bug("Removing room with max steps : %d", pRoom->vnum);

  /* We move the last vertice to the front */
  heap->knude[1] = heap->knude[heap->iVertice];
  heap->knude[1]->heap_index = 1;

  /* Decrease the size of the heap and remove the last entry */
  heap->knude[heap->iVertice] = NULL;
  heap->iVertice--;

  /* Swap places till it fits */
  while(!done)
  {
    if (heap->knude[i] == NULL)
      done = TRUE;
    else if (heap->knude[2*i] == NULL)
      done = TRUE;
    else if (heap->knude[2*i+1] == NULL)
    {
      if (heap->knude[i]->steps > heap->knude[2*i]->steps)
      {
        tRoom                        = heap->knude[i];
        heap->knude[i]               = heap->knude[2*i];
        heap->knude[i]->heap_index   = i;
        heap->knude[2*i]             = tRoom;
        heap->knude[2*i]->heap_index = 2*i;
        i = 2*i;
      }
      done = TRUE;
    }
    else if (heap->knude[i]->steps <= heap->knude[2*i]->steps &&
        heap->knude[i]->steps <= heap->knude[2*i+1]->steps)
      done = TRUE;
    else if (heap->knude[2*i]->steps <= heap->knude[2*i+1]->steps)
    {
      tRoom                        = heap->knude[i];
      heap->knude[i]               = heap->knude[2*i];
      heap->knude[i]->heap_index   = i;
      heap->knude[2*i]             = tRoom;
      heap->knude[2*i]->heap_index = 2*i;
      i = 2*i;
    }
    else
    {
      tRoom                          = heap->knude[i];
      heap->knude[i]                 = heap->knude[2*i+1];
      heap->knude[i]->heap_index     = i;
      heap->knude[2*i+1]             = tRoom;
      heap->knude[2*i+1]->heap_index = 2*i+1;
      i = 2*i+1;
    }
  }

  /* return the element */
  return pRoom;
}

void dijkstra(RID *chRoom, RID *victRoom)
{
  RID *pRoom;
  RID *tRoom;
  RID *xRoom;
  HEAP *heap;
  int door, x;
  bool stop;

  /* allocate a new heap */
  heap = init_heap(chRoom);

  /* find shortest amounts of steps to each room */
  while (heap->iVertice)
  {
    if ((pRoom = heap_getMinElement(heap)) == NULL)
    {
      bug("Dijstra: Getting NULL room", 0);
      return;
    }
    if (pRoom == victRoom)
      gFound = TRUE;

    /* update all exits */
    for (door = 0; door < 6; door++)
    {
      if (pRoom->exit[door] == NULL) continue;   
      if (pRoom->exit[door]->to_room == NULL) continue;

      /* update step count, and swap in the heap */
      if (pRoom->exit[door]->to_room->steps > pRoom->steps + 1)
      {
        xRoom = pRoom->exit[door]->to_room;
        xRoom->steps = pRoom->steps + 1;
        stop = FALSE;
        while ((x = xRoom->heap_index) != 1 && !stop)
        {
          if (heap->knude[x/2]->steps > xRoom->steps)
          {
            tRoom                       = heap->knude[x/2];
            heap->knude[x/2]            = xRoom;
            xRoom->heap_index           = xRoom->heap_index/2;
            heap->knude[x]              = tRoom;
            heap->knude[x]->heap_index  = x;
          }
          else stop = TRUE;
        }
      }
    }
  }

  /* free the heap */
  free(heap);
}

/*
 * The mobs vnum is included for bug-reports.
 */
bool will_trigger(char *say, char *trigger, int vnum)
{
  char buf[50];
  char *ptr;
  bool found;

  buf[0] = '\0';
  ptr = buf;

  while (*trigger != '\0')
  {
    switch(*trigger)
    {
      default:
        bug("Will_trigger: Bad keyword on mob %d.", vnum);
        return FALSE;
      case '&':  // all keywords must be included
        trigger++;
        while (*trigger != '\0' && *trigger != '&' && *trigger != '!' && *trigger != '|')
        {
          if (*trigger != '*')
            *ptr++ = *trigger;
          else
          {
            *ptr = '\0';
            if (!is_contained(buf, say)) return FALSE;
            else
            {
              buf[0] = '\0';
              ptr = buf;
            }
          }
          trigger++;
        }
        break;
      case '|':  // at least one must be included
        trigger++; found = FALSE;
        while (*trigger != '\0' && *trigger != '&' && *trigger != '!' && *trigger != '|')
        {
          if (*trigger != '*')   
            *ptr++ = *trigger;
          else
          { 
            *ptr = '\0';
            if (is_contained(buf, say)) found = TRUE;
            buf[0] = '\0';
            ptr = buf;
          }   
          trigger++;
        }
        if (!found) return FALSE;
        break;
      case '!':  // none of these may be included
        trigger++;
        while (*trigger != '\0' && *trigger != '&' && *trigger != '!' && *trigger != '|')
        {
          if (*trigger != '*')   
            *ptr++ = *trigger;
          else
          {
            *ptr = '\0';
            if (is_contained(buf, say)) return FALSE;
            else
            {
              buf[0] = '\0';
              ptr = buf;  
            }
          }
          trigger++;  
        }
        break;
    }
  }
  return TRUE;
}

void mob_text(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *victim;
  MOB_TRIGGER *mProg;
  char *arg;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  for (vch = ch->in_room->people; vch; vch = vch_next)
  {
    int runs = 0;

    vch_next = vch->next_in_room;
    if (!IS_NPC(vch)) continue;

    while (runs < 2)
    {
      if (runs == 0) mProg = vch->pIndexData->triggers;
      else mProg = vch->triggers;

      for ( ; mProg; mProg = mProg->next)
      {
        if (will_trigger(argument, mProg->keywords, vch->pIndexData->vnum))
        {
          switch(mProg->type)
          {
            default:
              bug("Mob_text: Bad type on mob %d.", vch->pIndexData->vnum);
              break;
            case MTRIG_SAY:
              do_say(vch, mProg->roomOutput);
              break;
            case MTRIG_LOCATE:
              if ((arg = cut_after_locate(argument)) == NULL)
              {
                do_say(vch, mProg->chOutput);
                return;
              }
              else if ((victim = get_char_area(ch, arg)) == NULL)
              {
                do_say(vch, mProg->chOutput);
                return;
              }
              else
              {
                if ((arg = pathfind(ch, victim)) == NULL)
                {
                  do_say(vch, mProg->chOutput);
                  return;
                }
                else
                {
                  xprintf(buf, mProg->roomOutput, arg);
                  do_say(vch, buf);
                  return;
                }
              }
              break;
            case MTRIG_HUNTER:
              if ((arg = cut_after_tracking(argument)) == NULL)
              {
                do_say(vch, mProg->chOutput);
                return;
              }
              else if ((victim = get_char_area(ch, arg)) == NULL)
              {
                do_say(vch, mProg->chOutput);
                return;
              }
              if (IS_NPC(victim))
              {
                do_say(vch, mProg->chOutput);
                return;
              }
              else  
              {
                if ((arg = pathfind(ch, victim)) == NULL)
                {
                  do_say(vch, mProg->chOutput);
                  return;
                }
                else
                {
                  free_string(vch->hunting);
                  vch->hunting = str_dup(arg);
                  vch->tracking = TRUE;
                  vch->hunt_pointer = 0;
                  vch->hunt_playerid = victim->pcdata->playerid;
                  do_say(vch, mProg->roomOutput);
                  return;
                }
              }
              break;
            case MTRIG_SCRY:
              break;
          }
          return;
        }
      }
      runs++;
    }
  }
}

void trigger_remove(CHAR_DATA *ch, MOB_TRIGGER *mProg)
{
  if (ch->triggers == NULL)
  {
    bug("Trigger_remove: no trigger.", 0);
    return;
  }
  if (mProg == ch->triggers)
  {
    ch->triggers = ch->triggers->next;
  }
  else
  {
    MOB_TRIGGER *prev;

    for (prev = ch->triggers; prev; prev = prev->next)
    {
      if (prev->next == mProg)
      {
        prev->next = mProg->next;
        break;
      }
    }
    if (!prev)
    {
      bug("Trigger_remove: cannot find mProg.", 0);
      return;
    }
  }
  mProg->next   = trigger_free;
  trigger_free  = mProg;

  return;
}

void update_aggressive_flags(CHAR_DATA *ch, CHAR_DATA *victim)
{
  int iDef, iAggr;

  /* first we check if we even care */
  if (IS_NPC(ch) || IS_NPC(victim)) return;
  if (ch->level > 6) return;
  if (victim->pcdata->aggress_towards == ch->pcdata->playerid) return;
  if (ch->pcdata->aggress_from == victim->pcdata->playerid) return;
  if (!str_cmp(victim->name, ch->pcdata->retaliation)) return;
  if (in_arena(ch) || in_fortress(ch)) return;
  if (IS_SET(ch->pcdata->tempflag, TEMP_AGGRESSIVE)) return;
  if (ch->class == victim->class && ch->generation >= victim->generation) return;
  if (ragnarok) return;

  iDef = victim->race;
  iAggr = ch->race;

  /* If the player aggresses someone other than
   * the person who attacked them, they don't deserve
   * the gang protection, since they are asking for it.
   */
  ch->pcdata->aggress_from = 0;

  /* This will take care of bully players 
  if (iDef < iAggr + 5)
  {
    victim->pcdata->aggress_from = ch->pcdata->playerid;
    ch->pcdata->bully_points += 50;
    if (ch->pcdata->bully_points > 400)
    {
      SET_BIT(ch->pcdata->jflags, JFLAG_BULLY);
      ch->pcdata->bully_counter += 240;  // roughly 2 hours
      ch->pcdata->bully_points = 0;
      send_to_char("#PTake that, bully!#n\n\r", ch);
    }
  }
*/
  /* this will take care of really small players attacking large players */
  if (iAggr < iDef + 5)
    ch->pcdata->aggress_towards = victim->pcdata->playerid;

  /* aggro counter up a little, untill I can fix it */
  ch->pcdata->agg_counter = UMAX(2, ch->pcdata->agg_counter);
  victim->pcdata->agg_counter = UMAX(2, victim->pcdata->agg_counter);
}

void strip_aggression(CHAR_DATA *ch)
{
  if (IS_NPC(ch)) return;

  ch->pcdata->agg_counter     = 0;
  ch->pcdata->aggress_towards = 0;
  ch->pcdata->aggress_from    = 0;
}

char *pathfind(CHAR_DATA *ch, CHAR_DATA *victim)
{
  int const exit_names [] = { 'n', 'e', 's', 'w', 'u', 'd' };
  RID *pRoom;
  AREA_DATA *pArea;
  static char path[MAX_STRING_LENGTH]; // should be plenty.
  int iPath = 0, vnum, door;
  bool found;

  if (!ch->in_room || !victim->in_room) return NULL;
  if (ch->in_room == victim->in_room) return NULL;
  if ((pArea = ch->in_room->area) != victim->in_room->area) return NULL;

  /* initialize all rooms in the area */
  for (vnum = pArea->lvnum; vnum < pArea->uvnum; vnum++)
  {
    if ((pRoom = get_room_index(vnum)))
    {
      pRoom->visited = FALSE;
      pRoom->steps = 999;
      for (door = 0; door < 6; door++)
      {
        if (pRoom->exit[door] == NULL) continue;
        pRoom->exit[door]->color = FALSE;
      }
    }
  }

  /* initialize variables */
  pRoom = ch->in_room;
  gFound = FALSE;

  /* In the first run, we only count steps, no coloring is done */
  examine_steps(pRoom, victim->in_room, pArea, 0);

  /* If the target room was never found, we return NULL */
  if (!gFound) return NULL;

  /* in the second run, we color the shortest path using the step counts */
  if (!examine_room(pRoom, victim->in_room, pArea, 0))
    return NULL;

  /* then we follow the trace */
  while (pRoom != victim->in_room)
  {
    found = FALSE;
    for (door = 0; door < 6 && !found; door++)  
    {
      if (pRoom->exit[door] == NULL) continue;
      if (pRoom->exit[door]->to_room == NULL) continue;
      if (!pRoom->exit[door]->color) continue;

      pRoom->exit[door]->color = FALSE;
      found = TRUE;
      path[iPath] = exit_names[door];
      iPath++;
      pRoom = pRoom->exit[door]->to_room;
    }
    if (!found)
    {
      bug("Pathfind: Fatal Error in %d.", pRoom->vnum); 
      return NULL;
    }
  }  

  path[iPath] = '\0';
  return path;
}

bool examine_room(ROOM_INDEX_DATA *pRoom, ROOM_INDEX_DATA *tRoom, AREA_DATA *pArea, int steps)
{
  int door;
      
  /* been here before, out of the area or can we get here faster */
  if (pRoom->area != pArea) return FALSE;
  if (pRoom->visited) return FALSE;
  if (pRoom->steps < steps) return FALSE;
   
  /* Have we found the room we are searching for */
  if (pRoom == tRoom)
    return TRUE;
    
  /* mark the room so we know we have been here */
  pRoom->visited = TRUE;
      
  /* Depth first traversel of all exits */
  for (door = 0; door < 6; door++)
  {   
    if (pRoom->exit[door] == NULL) continue;
    if (pRoom->exit[door]->to_room == NULL) continue;
     
    /* assume we are walking the right way */
    pRoom->exit[door]->color = TRUE;
 
    /* recursive return */
    if (examine_room(pRoom->exit[door]->to_room, tRoom, pArea, steps + 1))
      return TRUE;
  
    /* it seems we did not walk the right way */
    pRoom->exit[door]->color = FALSE;
  }
  return FALSE;
}
    
void examine_steps(ROOM_INDEX_DATA *pRoom, ROOM_INDEX_DATA *tRoom, AREA_DATA *pArea, int steps)
{
  int door;
      
  /* Have we found the room we are searching for */
  if (pRoom == tRoom)
  {   
    if (pRoom->steps > steps) pRoom->steps = steps;
    gFound = TRUE;
    return;
  }
    
  /* been here before, or out of the area */
  if (pRoom->area != pArea) return;
  if (pRoom->steps != 999)
  {
    if (pRoom->steps > steps) pRoom->steps = steps;
    return;
  }
   
  /* update the rooms step count*/
  pRoom->steps = steps;
    
  /* Depth first traversel of all exits */
  for (door = 0; door < 6; door++)
  {
    if (pRoom->exit[door] == NULL) continue;
    if (pRoom->exit[door]->to_room == NULL) continue;
  
    examine_steps(pRoom->exit[door]->to_room, tRoom, pArea, steps + 1);
  }
    
  return;  
}
