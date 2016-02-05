/*
01234567890123456789012345678901234567890123456789012345678901234567890123456789
Game Code v2 for ROM based muds. Robert Schultz, Sembiance  -  bert@ncinter.net
Snippets of mine can be found at http://www.ncinter.net/~bert/mud/
This file (games.c) contains all the game functions.
*/
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
#include "merc.h"
#include "recycle.h"
#include "interp.h"

void do_game( CHAR_DATA *ch, char *argument )
{
  char                arg1[MAX_INPUT_LENGTH];
  int whichGame;
  
  argument = one_argument(argument, arg1);
  
  if ( arg1[0] == '\0' )
    {
      send_to_char("Type 'help games' for more information on games.\n\r", ch);
      return;
    }
  
  if(IS_NPC(ch))
    {
      send_to_char("Sorry, only player characters may play games.\n\r", ch);
      return;
    }
  
  if ( !strcmp(arg1, "slot") || !strcmp(arg1, "slots"))
    whichGame = GAME_SLOTS;
  else if( !strcmp(arg1, "highdice"))
    whichGame = GAME_HIGH_DICE;
  else
    whichGame = GAME_NONE;
  
  switch(whichGame)
    {
    case GAME_SLOTS:
      do_slots(ch, argument);
      break;
    case GAME_HIGH_DICE:
      do_high_dice(ch, argument);
      break;
    default:
      send_to_char("Thats not a game. Type 'help games' for a list.\n\r", ch);
      break;
    }
	      

  return;
}

void  do_slots(CHAR_DATA *ch, char *argument )
{
  OBJ_DATA            *slotMachine;
  char                buf[MAX_STRING_LENGTH];
  char                arg[MAX_INPUT_LENGTH];
  int counter, winArray[11];
  int cost, jackpot, bars, winnings, numberMatched;
  int bar1, bar2, bar3, bar4, bar5;
  bool partial, won, wonJackpot, foundSlot;

  char *bar_messages[] = {
    "<------------>",
    "#YGold Coin#0",               /* 1 */
    "#RLock Pick#0",
    "#P-Zarius- #0",               /* 3 */
    "#cCityguard#0",
    "#CElf Sword#0",               /* 5 */
    "#yAn Orange#0",
    "#rFly Spell#0",
    "#GElemental#0",
    "#WPizzaman #0",
    "#BMudSchool#0",              /* 10 */
  };

  argument = one_argument(argument, arg);
  
  if(arg[0] == '\0')
    {
      send_to_char("Syntax: game slots <which machine>\n\r", ch);
      return;
    }

  foundSlot = FALSE; 

  for (slotMachine = ch->in_room->contents; slotMachine != NULL; slotMachine = slotMachine->next_content)
    {
      if ( (slotMachine->item_type == ITEM_SLOT_MACHINE) && (can_see_obj(ch,slotMachine)))
	{
	  if(is_name(arg, slotMachine->name))
	    {
	      foundSlot = TRUE;
	      break;
	    }
	  else
	    {
	      foundSlot = FALSE;
	    }
	}
    }
  
  if(foundSlot == FALSE)
    {
      send_to_char("That slot machine is not here.\n\r", ch);
      return;
    }

  cost = slotMachine->value[0];
  if(cost <= 0)
    {
      send_to_char("This slot machine seems to be broken.\n\r", ch);
      return;
    }

  if(cost > ch->bones)
    {
      xprintf(buf, "This slot machine costs %d bones to play.\n\r", cost);
      send_to_char(buf, ch);
      return;
    }

  ch->bones -= cost;

  jackpot = slotMachine->value[1];
  bars = slotMachine->value[2];

  if(slotMachine->value[3] == 1)
    partial = TRUE;
  else
    partial = FALSE;


  bar1 = number_range( 1, 10 );
  bar2 = number_range( 1, 10 );
  bar3 = number_range( 1, 10 );
  if(bars > 3)
    {
      bar4 = number_range( 1, 10 );
      bar5 = number_range( 1, 10 );
    }
  else
    {
      bar4 = 0;
      bar5 = 0;
    }

  if(bars == 3)
    {
      send_to_char("#0////------------#PSlot Machine#0------------\\\\\\\\#n\n\r", ch);
      xprintf(buf, "#0|#c{{}#0|#n  %s  %s  %s    |#c{{}#0|#n\n\r", bar_messages[bar1],
	      bar_messages[bar2], bar_messages[bar3]);
      send_to_char(buf, ch);
      send_to_char("#0\\\\\\\\------------------------------------////#n\n\r", ch);
    }
  else
    {
      send_to_char("#0////-----------------------#PSlot Machine#0----------------------\\\\\\\\#n\n\r", ch);
      xprintf(buf, "#0|#c{{}#0|#n  %s  %s  %s  %s  %s  #0|#c{{}#0|#n\n\r", bar_messages[bar1],
	      bar_messages[bar2], bar_messages[bar3], bar_messages[bar4], bar_messages[bar5]);
      send_to_char(buf, ch);
      send_to_char("#0\\\\\\\\---------------------------------------------------------////#n\n\r",
		   ch);
    }

  wonJackpot = FALSE;
  winnings = 0;
  won = FALSE;
  numberMatched = 0;

  if(bars == 3)
    {
      if( (bar1 == bar2) && (bar2 == bar3) )
        {
          winnings = jackpot;  /* they won the jackpot, make it */
          won = TRUE;          /* worth their while!            */
          slotMachine->value[1] = cost*10;   /* put it back to something */
          wonJackpot = TRUE;
        }
      else
        {
            slotMachine->value[1] += cost;
        }
    }
  else if(bars == 5)
    {
      if( (bar1 == bar2) && (bar2 == bar3) && (bar3 == bar4) && (bar4 == bar5) )
        {
          winnings = jackpot;  /* if no partial, better have a  */
          won = TRUE;          /* kick butt jackpot for them    */
          slotMachine->value[1] = cost*25;
          wonJackpot = TRUE;
        }
      else
        {
            slotMachine->value[1] += cost;
        }
    }
  else
    {
      send_to_char("This is a bad slot machine. Contact casino administration.\n\r", ch);
      return;
    }
    if(slotMachine->value[1] >= 15000000)
      slotMachine->value[1] = 15000000;

  for(counter = 0; counter <= 12; counter++)
    {
      winArray[counter] = 0;
    }


  if(!won && partial)
    {
      if(bars == 3)
        {
          if(bar1 == bar2)
            {
              winnings += cost/2;
              won = TRUE;
              numberMatched++;
            }
          if(bar1 == bar3)
            {
              numberMatched++;
              if(won)
                winnings += cost;
              else
                {
                  winnings += cost/2;
                  won = TRUE;
                }
            }
          if(bar2 == bar3)
            {
              numberMatched++;
              if(won)
                winnings += cost;
              else
                {
                  winnings += cost/2;
                  won = TRUE;
                }
            }
              if(!won)
                slotMachine->value[1] += cost;
              else
                slotMachine->value[1] -= winnings;
         
        }
      if( bars == 5)
        {
          winArray[bar1]++;
          winArray[bar2]++;
          winArray[bar3]++;
          winArray[bar4]++;
          winArray[bar5]++;

          for(counter = 0; counter <= 12; counter++)
            {
              if(winArray[counter] > 1)
                numberMatched += winArray[counter];
            }

          if(numberMatched == 5)
            {
                slotMachine->value[1] -= (cost*7)/2;
              winnings += cost*7;
            }
          if(numberMatched == 4)
            {
                slotMachine->value[1] -= (cost*5)/2;
              winnings += cost*5;
            }
          if(numberMatched == 3)
            {
              winnings += cost/2;
                slotMachine->value[1] += cost/2;
            }
          if(numberMatched == 2)
            {
                slotMachine->value[1] += cost-1;
              winnings = 1;
            }
          if(numberMatched == 0)
            {
              winnings = 0;
                slotMachine->value[1] += cost;
            }
          if(winnings > 0)
            won = TRUE;
        }
    }

  ch->bones += winnings;

  if(won && wonJackpot)
    {
      xprintf(buf, "You won the jackpot worth %d bones!! The jackpot now stands at %d bones.\n\r",
	      winnings, slotMachine->value[1]);
      send_to_char(buf, ch);
      xprintf(buf, "%s won a jackpot of %d bones from the casino!!!", ch->pcdata->switchname, winnings);
      do_info(ch, buf);
    }
  if(won && !wonJackpot)
    {
      xprintf(buf, "You matched %d bars and won %d bones! The jackpot is now worth %d bones.\n\r",
	      numberMatched, winnings, slotMachine->value[1]);
      send_to_char(buf, ch);
    }
  if(!won)
    {
      xprintf(buf, "Sorry you didn't win anything. The jackpot is now worth %d bones.\n\r",
	      slotMachine->value[1]);
      send_to_char(buf, ch);
    }

  if(slotMachine->value[1] > 15000000)
    slotMachine->value[1] = 15000000;

  return;
}

void do_high_dice( CHAR_DATA *ch, char *argument)
{
  char                buf[MAX_STRING_LENGTH];
  char                arg[MAX_INPUT_LENGTH];
  CHAR_DATA           *dealer;
  int die, dealerDice, playerDice;
  int bet;
  
  argument = one_argument(argument, arg);

  if(arg[0] == '\0' || !is_number(arg))
    {
      send_to_char("Syntax is: game highdice <bet>\n\r", ch);
      return;
    }

  bet = atoi(arg);

  if(bet < 10)
    {
      send_to_char("Minimum bet is 10 bones coins.\n\r", ch);
      return;
    }

  if(bet > 1000)
    {
      send_to_char("Maximum bet is 1,000 bones coins.\n\r", ch);
      return;
    }

  for ( dealer = ch->in_room->people; dealer; dealer = dealer->next_in_room )
    {
      if ( IS_NPC(dealer) && IS_SET(dealer->act, ACT_DEALER) && can_see(ch,dealer))
	break;
    }
  
  if ( dealer == NULL )
    {
      send_to_char( "You do not see any dice dealer here.\n\r", ch );
      return;
    }

  if(bet > ch->bones)
    {
      send_to_char("You can not afford to bet that much!\n\r", ch);
      return;
    }

  dealerDice = 0;
  playerDice = 0;


  die = number_range(1, 6);
  dealerDice += die;
  die = number_range(1, 6);
  dealerDice += die;

  die = number_range(1, 6);
  playerDice += die;
  die = number_range(1, 6);
  playerDice += die;

  xprintf(buf, "#c%s#w rolled two dice with a total of #Y%d!#n\n\r", dealer->short_descr, 
	  dealerDice);
  send_to_char(buf, ch);
  xprintf(buf, "#wYou rolled two dice with a total of #Y%d!#n\n\r", playerDice);
  send_to_char(buf, ch);

  if(dealerDice > playerDice)
    {
      xprintf(buf, "#RYou lost! #c%s#0 takes your bet of #Y%d bones#0.#n\n\r",
	      dealer->short_descr, bet);
      send_to_char(buf, ch);
      ch->bones -= bet;
    }

  if(dealerDice < playerDice)
    {
      xprintf(buf, "#RYou won! #c%s #0gives you your winnings of #Y%d bones#0.#n\n\r",
	      dealer->short_descr, bet);
      send_to_char(buf, ch);
      ch->bones += bet;
    }

  if(dealerDice == playerDice)
    {
      xprintf(buf, "#RYou lost! #0The dealer always wins in a tie. You lose #Y%d bones#0.#n\n\r",
	      bet);
      send_to_char(buf, ch);
      ch->bones -= bet;
    }

  return;
}


