/* * 
 *  $Id$
 *  
 *  This file is part of Fenice
 *
 *  Fenice -- Open Media Server
 *
 *  Copyright (C) 2004 by
 *  	
 *	- Giampaolo Mancini	<giampaolo.mancini@polito.it>
 *	- Francesco Varano	<francesco.varano@polito.it>
 *	- Marco Penno		<marco.penno@polito.it>
 *	- Federico Ridolfo	<federico.ridolfo@polito.it>
 *	- Eugenio Menegatti 	<m.eu@libero.it>
 *	- Stefano Cau
 *	- Giuliano Emma
 *	- Stefano Oldrini
 * 
 *  Fenice is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Fenice is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Fenice; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 * */

#include <stdio.h>

#include <fenice/bufferpool.h>

/* ! move current read position
 *
 * \return 1 on error, 0 otherwise
 * */
int32 OMSbuff_gotreader(OMSConsumer *cons)
{
	OMSSlot *last_read = cons->last_read_pos;
	// OMSSlot *next = cons->read_pos->next;
	OMSSlot *next = cons->read_pos;

	if ( !next->refs || (next->slot_seq < cons->last_seq) ) {
		// added some slots?
		if ( last_read && last_read->next->refs && (last_read->next->slot_seq > cons->last_seq) )
			next = last_read->next;
		else
			return 1; // NULL;
	} else if (last_read && ( last_read->next->slot_seq < next->slot_seq ) )
			next = last_read->next;

	next->refs--;

	cons->last_seq = next->slot_seq;
	// cons->read_pos = next;

	cons->last_read_pos = next;
	cons->read_pos = next->next;

	// cons->read_pos->refs--;
	
	// return cons->read_pos;
	return 0; // next;
}

