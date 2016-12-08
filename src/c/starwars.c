#include <pebble.h>
#include "starwars.h"

static GPath *s_leia_path_ptr = NULL;
static GPath *s_leia_hole_path_ptr = NULL;
static GPath *s_han_path_ptr = NULL;
static GPath *s_han_hole_path_ptr = NULL;

void sw_init(Layer *window_layer) {
  GRect bounds = layer_get_bounds(window_layer);
  GPoint centre = grect_center_point(&bounds);

  s_leia_path_ptr = gpath_create(&LEIA_PATH);
  s_leia_hole_path_ptr = gpath_create(&LEIA_HOLE_PATH);
  gpath_move_to(s_leia_path_ptr, GPoint(centre.x - 75, centre.y - 60));
  gpath_move_to(s_leia_hole_path_ptr, GPoint(centre.x - 75, centre.y - 60));
  s_han_path_ptr = gpath_create(&HAN_PATH);
  s_han_hole_path_ptr = gpath_create(&HAN_HOLE_PATH);
  gpath_move_to(s_han_path_ptr, GPoint(centre.x - 75, centre.y - 60));
  gpath_move_to(s_han_hole_path_ptr, GPoint(centre.x - 75, centre.y - 60));
}

void draw_sw(Layer *layer, GContext *ctx) {
  GColor swColour = PBL_IF_COLOR_ELSE(GColorBlueMoon, GColorWhite);

  graphics_context_set_fill_color(ctx, swColour);
  graphics_context_set_stroke_color(ctx, swColour);
  gpath_draw_filled(ctx, s_leia_path_ptr);
  gpath_draw_filled(ctx, s_han_path_ptr);

  graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorClear, GColorBlack));
  gpath_draw_filled(ctx, s_leia_hole_path_ptr);
  gpath_draw_filled(ctx, s_han_hole_path_ptr);
}
