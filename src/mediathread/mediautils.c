/* * 
 * This file is part of Feng
 *
 * Copyright (C) 2009 by LScube team <team@lscube.org>
 * See AUTHORS for more details
 * 
 * feng is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * feng is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with feng; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA 
 *
 * */

#include <stdlib.h>
#ifdef ENABLE_DUMA
#include <duma.h>
#endif
#include <string.h>
#include <glib.h>

#include "mediautils.h"

void *MObject_calloc(size_t size)
{
    MObject *new_obj;
    
    new_obj = g_malloc0(size);
    new_obj->refs=1;
    MObject_destructor(new_obj, g_free);

    return new_obj;
}

void *MObject_dup(void *obj, size_t size)
{
    MObject *new_obj = g_malloc(size);
    memcpy(new_obj, obj, size);

    new_obj->refs=1;

    return new_obj;
}

void MObject_init(MObject *obj)
{
    obj->refs = 1;
    obj->destructor = NULL;
}

void MObject_zero(MObject *obj, size_t size)
{
    size_t obj_hdr_size;

    // set to zero object data part
    obj_hdr_size = obj->data - (char *)obj;
    memset(obj->data, 0, size-obj_hdr_size);
}

void MObject_unref(MObject *mobject)
{
    if ( mobject && !--mobject->refs && mobject->destructor)
        mobject->destructor(mobject);
}

// Ripped from ffmpeg, see sdp.c

static void digit_to_char(gchar *dst, guint8 src)
{
    if (src < 10) {
        *dst = '0' + src;
    } else {
        *dst = 'A' + src - 10;
    }
}

static gchar *data_to_hex(gchar *buff, const guint8 *src, gint s)
{
    gint i;

    for(i = 0; i < s; i++) {
        digit_to_char(buff + 2 * i, src[i] >> 4);
        digit_to_char(buff + 2 * i + 1, src[i] & 0xF);
    }

    return buff;
}

gchar *extradata2config(const guint8 *extradata, gint extradata_size)
{
    gchar *config = g_malloc(extradata_size * 2 + 1);

    if (config == NULL) {
        return NULL;
    }

    data_to_hex(config, extradata, extradata_size);

    config[extradata_size * 2] = '\0';

    return config;
}