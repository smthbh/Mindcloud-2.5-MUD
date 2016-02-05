/***************************************************************************
 *  This file contains auction code developed by Brian Babey, and any      *
 *  communication regarding it should be sent to [bbabey@iname.com]        *
 *  Web Address: http://www.erols.com/bribe/                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*       ROM 2.4 is copyright 1993-1995 Russ Taylor                         *
*       ROM has been brought to you by the ROM consortium                  *
*           Russ Taylor (rtaylor@pacinfo.com)                              *
*           Gabrielle Taylor (gtaylor@pacinfo.com)                         *
*           Brian Moore (rom@rom.efn.org)                                  *
*       By using this code, you have agreed to follow the terms of the     *
*       ROM license, in the file Rom24/doc/rom.license                     *
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
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "merc.h"

DECLARE_DO_FUN( do_auction );

void show_obj_stats( CHAR_DATA *ch, OBJ_DATA *obj );
char * flag_string args( ( const struct flag_type *table, int bit ) );


void do_auchn( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
        
    if ( argument[0] == '\0' )
    {
        return;
    }
            
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected == CON_PLAYING &&
                !IS_SET(d->character->deaf, CHANNEL_AUCTION) )
        {  
            send_to_char( "#w[#0ø#w]#W Auction: ",d->character );
            send_to_char( argument, d->character );
            send_to_char( "#n\n\r", d->character );
        }
    }
    
    return;
}

void do_auction( CHAR_DATA *ch, char * argument )
{
    long cp=0;
    OBJ_DATA *	obj;
    char arg1[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( ch == NULL || IS_NPC(ch) )
	return;

    if ( arg1[0] == '\0')
    {
        do_auction(ch,"info");
	return;
    }


    if ( !str_cmp( arg1, "stop" ) && IS_IMMORTAL(ch))
    {
       if (auction_info.item == NULL) {stc("Nothing on auction.\n\r",ch); return; }

            xprintf(buf, "#R%s stops the auction. %s is confiscated.#n\n\r", ch->pcdata->switchname,
                auction_info.item->short_descr);
        do_auchn( auction_info.high_bidder,buf );
            obj_to_char( auction_info.item, ch );
    
        xprintf(buf, "%s appears in your hands..\n\r",
                capitalize(auction_info.item->short_descr) );
        send_to_char( buf, ch );
        
            auction_info.item           = NULL;
            auction_info.owner          = NULL;
            auction_info.current_bid    = 0;
            auction_info.status         = 0;
            
            return;
        }
    if ( !str_cmp( arg1, "info" ) )
    {
	obj = auction_info.item;

	if ( !obj )
	{
	    send_to_char("There is nothing up for auction right now.\n\r",ch);
	    return;
	}

	if ( auction_info.owner == ch )
	{
	    xprintf( buf, "\n\rYou are currently auctioning %s.\n\r",
		obj->short_descr );
	    send_to_char( buf, ch );
	    return;
	}

	else 
	    show_obj_stats( ch, obj );

	return;
    }


    if ( !str_cmp( arg1, "bid" ) )
    {
	long bid;
        int bah;
        obj = auction_info.item;

        if( !obj )
	{
	     stc( "There is no auction up for sale.\n\r", ch);
	     return;
        }
      	if ( IS_SET( obj->quest, QUEST_ARTIFACT) )
	{
	      int count;
	      OBJ_DATA * pObj;
	      if( ch->level < 3 )
	      {
			stc ( "Only avatars may bid on artifacts.\n\r",ch);
			return;
              }
	      
 	      count = 0;
	      for( pObj = ch->carrying; pObj; pObj = pObj->next )
		   if( IS_ARTIFACT( pObj ) ) count++;

	      if( count > 2 )
	      {
		stc( "You may only carry 2 artifacts.\n\r", ch);
	        return;
	      }
	}
		    
        if ( !obj )
        {
            send_to_char("There is nothing up for auction right now.\n\r",ch);
            return;
        }

	if ( arg2[0] == '\0' )
	{
	    send_to_char("You must enter an amount to bid.\n\r",ch);
	    return;
	}

	bid = atol( arg2 );
        bah = bid + 500;

        if ( bah < bid ) {stc("You need to bid at least 500 more than the current bid.\n\r",ch); return; }

	if ( bid <= auction_info.current_bid )
	{
	    xprintf(buf, "You must bid above the current bid of %ld Bones.\n\r",
		auction_info.current_bid );
	    return;
	}

        if ( bid < auction_info.current_bid + 500)
        { stc("You need to bid 500 more than the current bid.\n\r",ch); return; } 
	if ( bid < auction_info.minbid )
	{
	    xprintf( buf, "That's below the minimum bid.\r");
	    send_to_char(buf,ch);
	    return;
	}

	if (ch->bones < bid )
	{
	    send_to_char("You can't cover that bid.\n\r",ch);
	    return;
	}

	xprintf(buf, "#B%s #wjust bid #Y%ld #wbones for %s.#n\n\r", ch->pcdata->switchname,
	    bid, auction_info.item->short_descr);
        do_auchn( ch,buf);
	if ( auction_info.high_bidder != NULL )
	{
	    auction_info.high_bidder->bones += auction_info.bones_held;
	}

	cp = UMIN( ch->bones, bid );

	if ( cp < bid )
	{
	    cp = bid;
	}

	ch->bones -= cp;

	auction_info.bones_held	= cp;
	auction_info.high_bidder	= ch;
	auction_info.current_bid	= bid;
	auction_info.status	 	= 0;

	return;	
    }

    if ( arg2[0] == '\0')
    { stc("Syntax: auction <object> <min bid>",ch); return; }
    
    if ( auction_info.item != NULL )
    {
        send_to_char("There is already another item up for bid.\n\r",ch);
        return;
    }
    
    if ( (obj = get_obj_carry( ch, arg1,ch)) == NULL )
    {
        send_to_char("You aren't carrying that item.\n\r",ch);
        return;
    }
    
/*    if ( obj->questowner != NULL && strlen(obj->questowner) > 1 )
    {    
       send_to_char("You cannot  claimed items.\n\r",ch);
       return;
    }*/
    
    if((obj->ownerid != ch->pcdata->playerid) && (obj->ownerid != 0))
    {
    	send_to_char("That item is claimed by someone else.\n\r", ch);
    	return;
    }
    if (IS_SET(obj->quest, QUEST_ARTIFACT))
    {
    	stc("You can't auction artifacts\n\r",ch);
    	return;
    }
    if ( IS_OBJ_STAT( obj, ITEM_NODROP ) )
    {   
        send_to_char("You can't let go of that item.\n\r",ch);
        return;
    }
    auction_info.owner          = ch;
    auction_info.item           = obj;
    auction_info.current_bid    = 0;
    auction_info.status         = 0;
    auction_info.minbid         = atoi(arg2);
    obj->questowner = NULL;

    xprintf(buf,"%s has put up %s for auction. [Minimum Bid: %ld bones]\n\r", ch->pcdata->switchname, obj->short_descr, auction_info.minbid);
        do_auchn( ch,buf );
     
    obj_from_char( obj );

    return;

}

void auction_update( )
{
    char buf[MAX_STRING_LENGTH], temp[MAX_STRING_LENGTH];

    if ( auction_info.item == NULL )
	return;

    auction_info.status++;

    if ( auction_info.status == AUCTION_LENGTH )
    {
	xprintf(buf,"#w%s #R#fSOLD#n #wto %s for #Y%ld #0Bones.#n\n\r",
	    capitalize(auction_info.item->short_descr),
	    auction_info.high_bidder->name,
	    auction_info.current_bid );
        do_auchn( auction_info.high_bidder,buf );

	auction_info.owner->bones += auction_info.bones_held;
	auction_info.item->ownerid = 0;

	xprintf(temp,  "%ld bones ", auction_info.bones_held );
	xprintf(buf, "You receive %ld.\n\r",
		auction_info.bones_held);
	send_to_char( buf, auction_info.owner );
		
	obj_to_char( auction_info.item, auction_info.high_bidder );

	xprintf(buf, "%s appears in your hands.\n\r",
		capitalize(auction_info.item->short_descr) );
	send_to_char( buf, auction_info.high_bidder );
	
	auction_info.item		= NULL;
	auction_info.owner		= NULL;
	auction_info.high_bidder	= NULL;
	auction_info.current_bid	= 0;
	auction_info.status		= 0;
	auction_info.bones_held	= 0;

	return;
    }

    if ( auction_info.status == AUCTION_LENGTH - 1 )
    {
	xprintf(buf, "%s - going twice at %ld bones.\n\r",
		capitalize(auction_info.item->short_descr),
		auction_info.current_bid );
        do_auchn( auction_info.high_bidder,buf );
	return;
    }

    if ( auction_info.status == AUCTION_LENGTH - 2 )
    {
	if ( auction_info.current_bid == 0 )
	{
	    xprintf(buf, "No bids on %s - item sent back to Auctioner.\n\r",
		auction_info.item->short_descr);
            do_auchn( auction_info.high_bidder,buf );
	    obj_to_char( auction_info.item, auction_info.owner );

	xprintf(buf, "%s is returned to you.\n\r",
		capitalize(auction_info.item->short_descr) );
	send_to_char( buf, auction_info.owner );
	
	    auction_info.item           = NULL;
	    auction_info.owner          = NULL;
	    auction_info.current_bid    = 0;
	    auction_info.status         = 0;

	    return;
	}

        xprintf(buf, "%s - going once at %ld bones.\n\r",
                capitalize(auction_info.item->short_descr),
                auction_info.current_bid );
        do_auchn( auction_info.high_bidder,buf );
        return;
    }

    return;

}

/*
 * Show_obj_stats: code taken from stock identify spell (-Brian)
 */
void show_obj_stats( CHAR_DATA *ch, OBJ_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int itemtype;

    xprintf( buf, "Object '%s' is type %s.\n\r",
        obj->name,
        flag_string( type_flags, obj->item_type) );
    send_to_char(buf,ch);
    xprintf( buf, "Extra Flags: %s.\n\r",
        flag_string( extra_flags, obj->extra_flags ) );
    send_to_char(buf,ch);
    xprintf( buf, "Weight is %d, value at %d.\n\r",
        obj->weight,
        obj->cost );
    send_to_char(buf,ch);

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1 &&
        obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
        xprintf( buf, "This object was created by %s, and is owned by %s.\n\r", obj->questmaker,obj->questowner );
        send_to_char( buf, ch );
    }
    else if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
        xprintf( buf, "This object was created by %s.\n\r", obj->questmaker );
        send_to_char( buf, ch );
    }
    else if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
        xprintf( buf, "This object is owned by %s.\n\r", obj->questowner );
        send_to_char( buf, ch );
    }
        
    switch ( obj->item_type )   
    {
    case ITEM_PILL:
    case ITEM_SCROLL:
    case ITEM_POTION:
        xprintf( buf, "Level %d spells of:", obj->value[0] );
        send_to_char( buf, ch );
    
        if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
        {
            send_to_char( " '", ch );
            send_to_char( skill_table[obj->value[1]].name, ch );
            send_to_char( "'", ch );
        }
        
        if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
        {
            send_to_char( " '", ch );
            send_to_char( skill_table[obj->value[2]].name, ch );
            send_to_char( "'", ch );
        }
        
        if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
        {
            send_to_char( " '", ch );
            send_to_char( skill_table[obj->value[3]].name, ch );
            send_to_char( "'", ch );
        }
        
        send_to_char( ".\n\r", ch );
        break;
         
    case ITEM_WAND:
    case ITEM_STAFF:
        xprintf( buf, "Has %d(%d) charges of level %d",
            obj->value[1], obj->value[2], obj->value[0] );
        send_to_char( buf, ch );
            
        if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
        {
            send_to_char( " '", ch );
            send_to_char( skill_table[obj->value[3]].name, ch );
            send_to_char( "'", ch );
        }
        
        send_to_char( ".\n\r", ch );
        break;
        
    case ITEM_PIECE:
		xprintf( buf, "This is a piece of another object.\n\r" );
		send_to_char( buf, ch );
		break;
        
    case ITEM_WEAPON:   
        {
    
           xprintf( buf, "Damage is %d to %d (average %d).\n\r",
                    obj->value[1], obj->value[2],
                    ( obj->value[1] + obj->value[2] ) / 2 );
           send_to_char( buf, ch );
        
    xprintf( buf, capitalize(obj->short_descr));
            
    if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] - ((obj->value[0] / 1000) * 1000);
	else
	    itemtype = obj->value[0];
            
    if (itemtype > 0)
	{
	    if (obj->level < 10)
		xprintf(buf,"%s is a minor spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level < 20)
		xprintf(buf,"%s is a lesser spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level < 30)
		xprintf(buf,"%s is an average spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level < 40)
		xprintf(buf,"%s is a greater spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level < 50)
		xprintf(buf,"%s is a major spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level > 50)
		xprintf(buf,"%s is an ultimate spell weapon.\n\r",capitalize(obj->short_descr));
	    else
		xprintf(buf,"%s is a supreme spell weapon.\n\r",capitalize(obj->short_descr));
	    send_to_char(buf,ch);
	}
    
    if (IS_SET(obj->spectype, SITEM_COPPER))
        send_to_char("This item is forged with copper.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_IRON))
        send_to_char("This item is forged with iron.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_STEEL))
        send_to_char("This item is forged with steel.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_ADAMANTITE))
        send_to_char("This item is forged with adamantite.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_MITHRIL))
        send_to_char("This item is forged with mithril.\n\r",ch);
    if (IS_SET(obj->quest, QUEST_BLOODA))
        send_to_char( "This weapon is dripping with kindred blood.\n\r",ch);
    if (IS_SET(obj->quest, QUEST_ENCHANTED))
	send_to_char( "This item has been enchanted.\n\r", ch );
    if (IS_SET(obj->quest, QUEST_SPELLPROOF))
	send_to_char( "This item is resistant to offensive spells.\n\r", ch );
    if( IS_SET(obj->extra_flags, ITEM_MENCHANT))
	stc( "This weapon is indeed perfect.\n\r", ch);
    if (IS_SET(obj->spectype, SITEM_DEMONIC))
	send_to_char( "This item is crafted from demonsteel.\n\r", ch );
    else if (IS_SET(obj->spectype, SITEM_SILVER))
	send_to_char( "This item is crafted from gleaming silver.\n\r", ch );
                     
   if (itemtype == 1)
	    xprintf (buf, "This weapon is dripping with corrosive acid.\n\r");
	else if (itemtype == 4)
	    xprintf (buf, "This weapon radiates an aura of darkness.\n\r");
	else if (itemtype == 30)
	    xprintf (buf, "This ancient relic is the bane of all evil.\n\r");
	else if (itemtype == 34)
	    xprintf (buf, "This vampiric weapon drinks the souls of its victims.\n\r");
	else if (itemtype == 37)
	    xprintf (buf, "This weapon has been tempered in hellfire.\n\r");
	else if (itemtype == 48)
	    xprintf (buf, "This weapon crackles with sparks of lightning.\n\r");
	else if (itemtype == 53)
	    xprintf (buf, "This weapon is dripping with a dark poison.\n\r");
	else if (itemtype > 0)
	    xprintf (buf, "This weapon has been imbued with the power of %s.\n\r",skill_table[itemtype].name);
	if (itemtype > 0)
	    send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] / 1000;
	else
	    break;

	if (itemtype == 4)
	    xprintf (buf, "This weapon radiates an aura of darkness.\n\r");
	else if (itemtype == 27 || itemtype == 2)
	    xprintf (buf, "This weapon allows the wielder to see invisible things.\n\r");
	else if (itemtype == 39 || itemtype == 3)
	    xprintf (buf, "This weapon grants the power of flight.\n\r");
	else if (itemtype == 45 || itemtype == 1)
	    xprintf (buf, "This weapon allows the wielder to see in the dark.\n\r");
	else if (itemtype == 46 || itemtype == 5)
	    xprintf (buf, "This weapon renders the wielder invisible to the human eye.\n\r");
	else if (itemtype == 52 || itemtype == 6)
	    xprintf (buf, "This weapon allows the wielder to walk through solid doors.\n\r");
	else if (itemtype == 54 || itemtype == 7)
	    xprintf (buf, "This holy weapon protects the wielder from evil.\n\r");
        else if (itemtype == 139)
            xprintf (buf, "This unholy weapon protects the wielder from good.\n\r");
	else if (itemtype == 57 || itemtype == 8)
	    xprintf (buf, "This ancient weapon protects the wielder in combat.\n\r");
	else if (itemtype == 9)
	    xprintf (buf, "This crafty weapon allows the wielder to walk in complete silence.\n\r");
	else if (itemtype == 10)
	    xprintf (buf, "This powerful weapon surrounds its wielder with a shield of lightning.\n\r");
	else if (itemtype == 11)
	    xprintf (buf, "This powerful weapon surrounds its wielder with a shield of fire.\n\r");
	else if (itemtype == 12)
	    xprintf (buf, "This powerful weapon surrounds its wielder with a shield of ice.\n\r");
	else if (itemtype == 13)
	    xprintf (buf, "This powerful weapon surrounds its wielder with a shield of acid.\n\r");
	else if (itemtype == 14)
	    xprintf (buf, "This weapon protects its wielder from clan DarkBlade guardians.\n\r");
	else if (itemtype == 15)
	    xprintf (buf, "This ancient weapon surrounds its wielder with a shield of chaos.\n\r");
	else if (itemtype == 16)
	    xprintf (buf, "This ancient weapon regenerates the wounds of its wielder.\n\r");
	else if (itemtype == 26)
	    xprintf (buf, "This ancient weapon gives the power of mantis 3.\n\r");
	else if (itemtype == 17)
	    xprintf (buf, "This ancient weapon allows its wielder to move at supernatural speed.\n\r");
	else if (itemtype == 18)
	    xprintf (buf, "This razor sharp weapon can slice through armour without difficulty.\n\r");
	else if (itemtype == 19)
	    xprintf (buf, "This ancient weapon protects its wearer from player attacks.\n\r");
	else if (itemtype == 20)
	    xprintf (buf, "This ancient weapon surrounds its wielder with a shield of darkness.\n\r");
	else if (itemtype == 21)
	    xprintf (buf, "This ancient weapon grants superior protection to its wielder.\n\r");
	else if (itemtype == 22)
	    xprintf (buf, "This ancient weapon grants its wielder supernatural vision.\n\r");
	else if (itemtype == 23)
	    xprintf (buf, "This ancient weapon makes its wielder fleet-footed.\n\r");
	else if (itemtype == 24)
	    xprintf (buf, "This ancient weapon conceals its wielder from sight.\n\r");
	else if (itemtype == 25)
	    xprintf (buf, "This ancient weapon invokes the power of the beast.\n\r");
	else if (itemtype == 28)
	    xprintf (buf, "This ancient weapon increases the level of your spells by 20.\n\r");
	else
	    xprintf (buf, "This item is bugged...please report it.\n\r");
	if (itemtype > 0)
	    send_to_char( buf, ch );
        break;
    }
    case ITEM_ARMOR:
        xprintf( buf, "Armor class is %d.\n\r", obj->value[0] );
        send_to_char( buf, ch );
        if (obj->value[3] < 1)
	    break;
	if (obj->value[3] == 4)
	    xprintf (buf, "This object radiates an aura of darkness.\n\r");
	else if (obj->value[3] == 27 || obj->value[3] == 2)
	    xprintf (buf, "This item allows the wearer to see invisible things.\n\r");
	else if (obj->value[3] == 39 || obj->value[3] == 3)
	    xprintf (buf, "This object grants the power of flight.\n\r");
	else if (obj->value[3] == 45 || obj->value[3] == 1)
	    xprintf (buf, "This item allows the wearer to see in the dark.\n\r");
	else if (obj->value[3] == 46 || obj->value[3] == 5)
	    xprintf (buf, "This object renders the wearer invisible to the human eye.\n\r");
	else if (obj->value[3] == 52 || obj->value[3] == 6)
	    xprintf (buf, "This object allows the wearer to walk through solid doors.\n\r");
	else if (obj->value[3] == 54 || obj->value[3] == 7)
	    xprintf (buf, "This holy relic protects the wearer from evil.\n\r");
        else if (obj->value[3] == 139)
            xprintf (buf, "This unholy relic protects the wearer from good.\n\r");
	else if (obj->value[3] == 57 || obj->value[3] == 8)
	    xprintf (buf, "This ancient relic protects the wearer in combat.\n\r");
	else if (obj->value[3] == 9)
	    xprintf (buf, "This crafty item allows the wearer to walk in complete silence.\n\r");
	else if (obj->value[3] == 10)
	    xprintf (buf, "This powerful item surrounds its wearer with a shield of lightning.\n\r");
	else if (obj->value[3] == 11)
	    xprintf (buf, "This powerful item surrounds its wearer with a shield of fire.\n\r");
	else if (obj->value[3] == 12)
	    xprintf (buf, "This powerful item surrounds its wearer with a shield of ice.\n\r");
	else if (obj->value[3] == 13)
	    xprintf (buf, "This powerful item surrounds its wearer with a shield of acid.\n\r");
	else if (obj->value[3] == 14)
	    xprintf (buf, "This object channels the power of god into its wearer.\n\r");
	else if (obj->value[3] == 15)
	    xprintf (buf, "This ancient item surrounds its wearer with a shield of chaos.\n\r");
	else if (obj->value[3] == 16)
	    xprintf (buf, "This ancient item regenerates the wounds of its wearer.\n\r");
	else if (obj->value[3] == 17)
	    xprintf (buf, "This ancient item allows its wearer to move at supernatural speed.\n\r");
	else if (obj->value[3] == 18)
	    xprintf (buf, "This powerful item allows its wearer to shear through armour without difficulty.\n\r");
	else if (obj->value[3] == 19)
	    xprintf (buf, "This powerful item protects its wearer from player attacks.\n\r");
	else if (obj->value[3] == 20)
	    xprintf (buf, "This ancient item surrounds its wearer with a shield of darkness.\n\r");
	else if (obj->value[3] == 21)
	    xprintf (buf, "This ancient item grants superior protection to its wearer.\n\r");
	else if (obj->value[3] == 22)
	    xprintf (buf, "This ancient item grants its wearer supernatural vision.\n\r");
	else if (obj->value[3] == 23)
	    xprintf (buf, "This ancient item makes its wearer fleet-footed.\n\r");
	else if (obj->value[3] == 24)
	    xprintf (buf, "This ancient item conceals its wearer from sight.\n\r");
	else if (obj->value[3] == 25)
	    xprintf (buf, "This ancient item invokes the power of the beast.\n\r");
        else if (obj->value[3] == 28)
            xprintf (buf, "This ancient item invokes the power of ancient magic.\n\r");
	else if (obj->value[3] == 29)
	    xprintf (buf, "This ancient artifact grants its wearer the power of the third eye.\n\r"); 
        else if (obj->value[3] == 30)
            xprintf (buf, "This great bird grants the power of talons.\n\r");
	else
	    xprintf (buf, "This item is bugged...please report it.\n\r");
	if (obj->value[3] > 0)
	    send_to_char( buf, ch );
        break;
    }
        
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
        if ( paf->location != APPLY_NONE && paf->modifier != 0 )
        {
            xprintf( buf, "Affects %s by %d.\n\r",
                affect_loc_name( paf->location ), paf->modifier );
            send_to_char( buf, ch );
        }
    }
        
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
        if ( paf->location != APPLY_NONE && paf->modifier != 0 )
        {
            xprintf( buf, "Affects %s by %d.\n\r",
                affect_loc_name( paf->location ), paf->modifier );
            send_to_char( buf, ch );
        }
    }
    return;
}

