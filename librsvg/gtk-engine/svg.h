/* GTK+ Rsvg Engine
 * Copyright (C) 1998-2000 Red Hat, Inc.
 * Copyright (C) 2002 Dom Lachowicz
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Written by Owen Taylor <otaylor@redhat.com>, based on code by
 * Carsten Haitzler <raster@rasterman.com>
 */

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <rsvg.h>

/* internals */

typedef struct _ThemeData ThemeData;
typedef struct _ThemeImage ThemeImage;
typedef struct _ThemeMatchData ThemeMatchData;
typedef struct _ThemePixbuf ThemePixbuf;

enum
{
  TOKEN_IMAGE = G_TOKEN_LAST + 1,
  TOKEN_FUNCTION,
  TOKEN_FILE,
  TOKEN_STRETCH,
  TOKEN_RECOLORABLE,
  TOKEN_BORDER,
  TOKEN_DETAIL,
  TOKEN_STATE,
  TOKEN_SHADOW,
  TOKEN_GAP_SIDE,
  TOKEN_GAP_FILE,
  TOKEN_GAP_BORDER,
  TOKEN_GAP_START_FILE,
  TOKEN_GAP_START_BORDER,
  TOKEN_GAP_END_FILE,
  TOKEN_GAP_END_BORDER,
  TOKEN_OVERLAY_FILE,
  TOKEN_OVERLAY_BORDER,
  TOKEN_OVERLAY_STRETCH,
  TOKEN_ARROW_DIRECTION,
  TOKEN_D_HLINE,
  TOKEN_D_VLINE,
  TOKEN_D_SHADOW,
  TOKEN_D_POLYGON,
  TOKEN_D_ARROW,
  TOKEN_D_DIAMOND,
  TOKEN_D_OVAL,
  TOKEN_D_STRING,
  TOKEN_D_BOX,
  TOKEN_D_FLAT_BOX,
  TOKEN_D_CHECK,
  TOKEN_D_OPTION,
  TOKEN_D_CROSS,
  TOKEN_D_RAMP,
  TOKEN_D_TAB,
  TOKEN_D_SHADOW_GAP,
  TOKEN_D_BOX_GAP,
  TOKEN_D_EXTENSION,
  TOKEN_D_FOCUS,
  TOKEN_D_SLIDER,
  TOKEN_D_ENTRY,
  TOKEN_D_HANDLE,
  TOKEN_D_STEPPER,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_TOP,
  TOKEN_UP,
  TOKEN_BOTTOM,
  TOKEN_DOWN,
  TOKEN_LEFT,
  TOKEN_RIGHT,
  TOKEN_NORMAL,
  TOKEN_ACTIVE,
  TOKEN_PRELIGHT,
  TOKEN_SELECTED,
  TOKEN_INSENSITIVE,
  TOKEN_NONE,
  TOKEN_IN,
  TOKEN_OUT,
  TOKEN_ETCHED_IN,
  TOKEN_ETCHED_OUT,
  TOKEN_ORIENTATION,
  TOKEN_HORIZONTAL,
  TOKEN_VERTICAL
};

typedef enum
{
  COMPONENT_NORTH_WEST = 1 << 0,
  COMPONENT_NORTH      = 1 << 1,
  COMPONENT_NORTH_EAST = 1 << 2, 
  COMPONENT_WEST       = 1 << 3,
  COMPONENT_CENTER     = 1 << 4,
  COMPONENT_EAST       = 1 << 5, 
  COMPONENT_SOUTH_EAST = 1 << 6,
  COMPONENT_SOUTH      = 1 << 7,
  COMPONENT_SOUTH_WEST = 1 << 8,
  COMPONENT_ALL 	  = 1 << 9
} ThemePixbufComponent;

typedef enum {
  THEME_MATCH_GAP_SIDE        = 1 << 0,
  THEME_MATCH_ORIENTATION     = 1 << 1,
  THEME_MATCH_STATE           = 1 << 2,
  THEME_MATCH_SHADOW          = 1 << 3,
  THEME_MATCH_ARROW_DIRECTION = 1 << 4
} ThemeMatchFlags;

typedef enum {
  THEME_CONSTANT_ROWS = 1 << 0,
  THEME_CONSTANT_COLS = 1 << 1,
  THEME_MISSING = 1 << 2
} ThemeRenderHints;

struct _ThemePixbuf
{
  gchar      *filename;
  RsvgHandle *handle;
  gboolean    stretch;
  gint        border_left;
  gint        border_right;
  gint        border_bottom;
  gint        border_top;
  guint       hints[3][3];
};

struct _ThemeMatchData
{
  guint           function;	/* Mandatory */
  gchar          *detail;

  ThemeMatchFlags flags;

  GtkPositionType gap_side;
  GtkOrientation  orientation;
  GtkStateType    state;
  GtkShadowType   shadow;
  GtkArrowType    arrow_direction;
};

struct _ThemeImage
{
  guint           refcount;

  ThemePixbuf    *background;
  ThemePixbuf    *overlay;
  ThemePixbuf    *gap_start;
  ThemePixbuf    *gap;
  ThemePixbuf    *gap_end;
  
  gchar           recolorable;

  ThemeMatchData  match_data;
};


ThemePixbuf *theme_pixbuf_new          (void);
void         theme_pixbuf_destroy      (ThemePixbuf  *theme_pb);
void         theme_pixbuf_set_filename (ThemePixbuf  *theme_pb,
					const char   *filename);
GdkPixbuf *  theme_pixbuf_get_pixbuf   (ThemePixbuf  *theme_pb, gint width, gint height);
void         theme_pixbuf_set_border   (ThemePixbuf  *theme_pb,
					gint          left,
					gint          right,
					gint          top,
					gint          bottom);
void         theme_pixbuf_set_stretch  (ThemePixbuf  *theme_pb,
					gboolean      stretch);
void         theme_pixbuf_render       (ThemePixbuf  *theme_pb,
					GdkWindow    *window,
					GdkBitmap    *mask,
					GdkRectangle *clip_rect,
					guint         component_mask,
					gboolean      center,
					gint          dest_x,
					gint          dest_y,
					gint          dest_width,
					gint          dest_height);
