#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "merc.h"
#include "version.h"
#include "build.h"

const char *versionToString( int version )
{
   static char buf[64];

   xprintf( buf, "%d.%02d.%02d", version / 10000, ( version - ( ( 10000 * ( version / 10000 ) ) ) ) / 100, version % 100 );

   return buf;
}

void do_version( CHAR_DATA * ch, char *argument )
{
   char buf[MSL];

   cent_to_char( "#0--------------------------------------------------------------------------------#n", ch );
   cent_to_char( "#WM#windcloud: #Y2.5#w", ch );
   xprintf( buf, "#wThis Version #r%s#w. Build: #r%d #wCompiled: #r%s#n.", versionToString( VERSION ), LINKCOUNT, COMPILE_TIME );
   cent_to_char( buf, ch );
   cent_to_char( "#wby Zarius(jeff@mindcloud.com)#n", ch );
   cent_to_char( "#0--------------------------------------------------------------------------------#n", ch );
   return;
}


