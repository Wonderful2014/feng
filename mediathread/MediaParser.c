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
 *	- Federico Ridolfo	<federico.ridolfo@polito.it>
 *	- Marco Penno		<marco.penno@polito.it>
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

#include <string.h>

#include <fenice/MediaParser.h>
#include <fenice/utils.h>

int register_media_type(MediaParserType * parser_type, MediaParser * p)
{
	p->parser_type=parser_type; 
	return ERR_NOERROR;
}

void free_parser(MediaParser *p)
{
	if(p->parser_type!=NULL) {
		p->parser_type->calculate_timestamp=NULL;
		p->parser_type->close();
		p->parser_type->properties=NULL;
		free(p->parser_type);
		p->parser_type=NULL;
	}
	if(p!=NULL) {
		free(p);	
		p=NULL;
	}
}

MediaParser * add_media_parser(void) 
{
	MediaParserType * parser_type;
	MediaParser *p;
	if((parser_type=(MediaParserType *)malloc(sizeof(MediaParserType)))==NULL)
		return NULL;
	
	if((p=(MediaParser *)malloc(sizeof(MediaParser)))==NULL) {
		free(parser_type);
		return NULL;
	}
	p->parser_type=parser_type;
	return p;
}

int set_media_entity(MediaParserType *pt, char *encoding_name)
{

	if ((strcmp(encoding_name,"H26L")!=0) || (strcmp(encoding_name,"MPV")!=0) || \
	    (strcmp(encoding_name,"MP2T")!=0) || (strcmp(encoding_name,"MP4V-ES")!=0) ) {
		strcpy(pt->media_entity,"video");
	}
		
	else if ((strcmp(encoding_name,"MPA")!=0) || (strcmp(encoding_name,"PCM")!=0) || (strcmp(encoding_name,"GSM")!=0)) {
		strcpy(pt->media_entity,"audio");
	}
	else
		return ERR_GENERIC; /*unknown*/
	/*TODO: text*/
	
	return ERR_NOERROR;
}