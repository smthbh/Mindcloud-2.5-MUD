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

#define IS_PRIPOWER(ch, sn)     (IS_SET((ch)->pcdata->powers[PRIPOWER_FLAGS], (sn)))
#define IS_PRIAFF(ch, sn)       (IS_SET((ch)->pcdata->powers[PRIPOWER_CURRENT], (sn)))

#define PRIEST_CURRENT                 (DC)
#define PRIEST_TOTAL                   (DT)
#define PRIEST_POWER                   (DB)

#define FAITH_PTS			9

#define PRIPOWER_FLAGS                  1
#define PRIPOWER_CURRENT                2

/* Powers */

#define PRI_MIHE                     1
#define PRI_MAHE                     2
#define PRI_GRHE                     4
#define PRI_STRM		     8
#define PRI_MSTM                    16
#define PRI_GSTM                    32
#define PRI_SIGHT                   64
#define PRI_GBLS                   128
#define PRI_GODSTRIKE              256
#define PRI_GDHE                   512
#define PRI_TURN                  1024
#define PRI_PFORM                 2048
#define PRI_HAUR                  4096
#define PRI_GPEC                  8192
#define PRI_SCRY                 16384
#define PRI_READ                 32768
#define PRI_SHIELD               65536
