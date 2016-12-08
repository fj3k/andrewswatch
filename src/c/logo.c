#include <pebble.h>
#include "logo.h"
#include "lib.h"

static GPath *s_logo_path_ptr = NULL;
static GPath *s_logo_hole_path_ptr = NULL;

void logo_init(Layer *window_layer) {
  GRect bounds = layer_get_bounds(window_layer);
  GPoint centre = grect_center_point(&bounds);

  s_logo_path_ptr = gpath_create(&LOGO_PATH);
  s_logo_hole_path_ptr = gpath_create(&LOGO_HOLE_PATH);
  gpath_move_to(s_logo_path_ptr, GPoint(centre.x - 24, centre.y - 64));
  gpath_move_to(s_logo_hole_path_ptr, GPoint(centre.x - 24, centre.y - 64));
}

void draw_logo(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint centre = grect_center_point(&bounds);
  GPoint clockpoint = GPoint(centre.x + 34.729 - 24, centre.y - 64 + 31.592);
  GColor logoColour = PBL_IF_COLOR_ELSE(GColorBlueMoon, GColorWhite);

  graphics_context_set_fill_color(ctx, logoColour);
  graphics_context_set_stroke_color(ctx, logoColour);
  graphics_context_set_stroke_width(ctx, 1);
  gpath_draw_outline(ctx, s_logo_path_ptr);
  gpath_draw_filled(ctx, s_logo_path_ptr);
  //gpath_draw_outline(ctx, s_logo_hole_path_ptr);

  time_t now = time(NULL);
  graphics_context_set_stroke_width(ctx, 2);
  draw_simple_clock(ctx, now, clockpoint, 8, 12);

  graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorClear, GColorBlack));
  gpath_draw_filled(ctx, s_logo_hole_path_ptr);
}
