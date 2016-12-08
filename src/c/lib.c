#include <pebble.h>
#include "lib.h"

void a_graphics_fill_radial(GContext * ctx, GRect rect, GOvalScaleMode scale_mode, uint16_t inset_thickness, int32_t angle_start, int32_t angle_end) {
  while (angle_start < 0) { angle_start += TRIG_MAX_ANGLE; }
  while (angle_end < 0) { angle_end += TRIG_MAX_ANGLE; }
  while (angle_start > TRIG_MAX_ANGLE) { angle_start -= TRIG_MAX_ANGLE; }
  while (angle_end > TRIG_MAX_ANGLE) { angle_end -= TRIG_MAX_ANGLE; }

  if (angle_start > angle_end) {
    graphics_fill_radial(ctx, rect, scale_mode, inset_thickness, angle_start, TRIG_MAX_ANGLE);
    graphics_fill_radial(ctx, rect, scale_mode, inset_thickness, 0, angle_end);
  } else {
    graphics_fill_radial(ctx, rect, scale_mode, inset_thickness, angle_start, angle_end);
  }
}

float a_rand() {
  static long seed = 100;
  seed = (((seed * 214013L + 2531011L) >> 16) & 32767);

  return (seed % 1000) / 1000.;
}

void draw_hand(GContext *ctx, GPoint clockCentre, int32_t angle, int32_t length) {
  GPoint handEnd = {
    .x = (int16_t)(sin_lookup(angle) * length / TRIG_MAX_RATIO) + clockCentre.x,
    .y = (int16_t)(-cos_lookup(angle) * length / TRIG_MAX_RATIO) + clockCentre.y,
  };
  graphics_draw_line(ctx, handEnd, clockCentre);
}

void draw_simple_clock(GContext *ctx, time_t now, GPoint clockpoint, int8_t hourlength, int8_t minutelength) {
    struct tm *t = localtime(&now);
    int32_t minute_angle = TRIG_MAX_ANGLE * (0. + t->tm_min) / 60;
    int32_t hour_angle = TRIG_MAX_ANGLE * (t->tm_hour % 12) / 12 + minute_angle / 12;
    draw_hand(ctx, clockpoint, hour_angle, hourlength);
    draw_hand(ctx, clockpoint, minute_angle, minutelength);
}
