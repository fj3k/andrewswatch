#include <pebble.h>
#include "stats.h"
#include "lib.h"

static bool s_stats = false;
static bool s_steps = false;

static const int32_t NOST = -2147483648;
//RIGHT OUTER, RIGHT INNER, LEFT OUTER, LEFT INNER
static int s_data[4] = {-2147483648, -2147483648, -2147483648, -2147483648};
static int s_max[4] = {-2147483648, -2147483648, -2147483648, -2147483648};
static int s_tick[4] = {-2147483648, -2147483648, -2147483648, -2147483648};
static bool s_neg[4] = {false, false, false, false};


static void drawBar(Layer *layer, GContext *ctx, int32_t val, int32_t max, bool neg, int32_t tick, bool right, bool full, bool inner) {
  GRect bounds = layer_get_bounds(layer);
  GPoint centre = grect_center_point(&bounds);

  int inset = PBL_IF_ROUND_ELSE(0, -2);
  int width = 4;
  if (inner) {
    inset = inset + 5;
    width = 2;
  }
  GRect frame = grect_inset(bounds, GEdgeInsets(inset));

  int32_t angle = TRIG_MAX_ANGLE * ((val * 1.) / (max * 1.));
  int32_t start_angle = 0;
  int32_t end_angle = 0;
  if (neg) {
    start_angle += TRIG_MAX_ANGLE / 2;
    angle = angle / 2;
    angle += TRIG_MAX_ANGLE / 2;
  }
  if (!full) {
    start_angle = start_angle / 2;
    angle = angle / 2;
  }
  if (neg && val < 0) {
    end_angle = start_angle;
    start_angle = angle;
  } else {
    end_angle = angle;
  }

  if (!right) {
    angle = TRIG_MAX_ANGLE - end_angle;
    end_angle = TRIG_MAX_ANGLE - start_angle;
    start_angle = angle;
  }

  a_graphics_fill_radial(ctx, frame, GOvalScaleModeFitCircle, width, start_angle, end_angle);

  //Draw ticks only if we have them and there aren't too many.
  if (tick != NOST && (max / tick) < (full ? 60 : 30)) {
    graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorClear, GColorBlack));
    graphics_context_set_stroke_width(ctx, 1);
    const int16_t inner_length = (frame.size.w / 2) - width - 1;
    const int16_t outer_length = frame.size.h / 2 + 1;
    int32_t start = val < 0 ? 0 - (((-1 * val) / tick) * tick) : tick; //Integer division of the positive value so it rounds down and steps through zero, not -1.
    int32_t end = val < 0 ? 0 : val;
    for (int32_t t = start; t < end; t += tick) {
      if (t == 0) continue;
      int32_t tick_angle = TRIG_MAX_ANGLE * ((t * 1.) / (max * 1.));
      if (neg) {
        tick_angle = tick_angle / 2;
        tick_angle += TRIG_MAX_ANGLE / 2;
      }
      if (!full) tick_angle = tick_angle / 2;
      if (!right) tick_angle = TRIG_MAX_ANGLE - tick_angle;

      GPoint tick_inner = {
        .x = (int16_t)(sin_lookup(tick_angle) * (int32_t)inner_length / TRIG_MAX_RATIO) + centre.x,
        .y = (int16_t)(-cos_lookup(tick_angle) * (int32_t)inner_length / TRIG_MAX_RATIO) + centre.y,
      };
      GPoint tick_outer = {
        .x = (int16_t)(sin_lookup(tick_angle) * (int32_t)outer_length / TRIG_MAX_RATIO) + centre.x,
        .y = (int16_t)(-cos_lookup(tick_angle) * (int32_t)outer_length / TRIG_MAX_RATIO) + centre.y,
      };
      graphics_draw_line(ctx, tick_inner, tick_outer);
    }
  }
}

void stat_init(Layer *layer) {
  
}

void draw_stat(Layer *layer, GContext *ctx) {
  if (!s_stats) return;
  GRect bounds = layer_get_bounds(layer);
  GColor colour[4] = {GColorPictonBlue, GColorElectricBlue, GColorRed, GColorSunsetOrange};

  // Process self first so max[i] >= data[i] (saves comparing data later
  int dCount = 0;
  for (int i = 0; i < 4; i++) {
    if (s_data[i] == NOST) continue; //No data.
    if (s_data[i] < 0) s_neg[i] = true;
    if (s_max[i] != NOST) { 
      if (s_data[i] > s_max[i]) s_max[i] = s_data[i];
      if (-1 * s_data[i] > s_max[i]) s_max[i] = s_data[i] * -1;
    }
  }

  // Match pairs
  for (int i = 0; i < 4; i++) {
    if (s_data[i] == NOST) continue; //No data.
    if (s_max[i] == NOST && s_max[(i+2)%4] != NOST) {
      if (s_data[i] > s_max[(i+2)%4]) s_max[(i+2)%4] = s_data[i];
      if (-1 * s_data[i] > s_max[(i+2)%4]) s_max[(i+2)%4] = s_data[i] * -1;
      s_max[i] = s_max[(i+2)%4];
      s_neg[i] = s_neg[(i+2)%4] = (s_neg[i] || s_neg[(i+2)%4]);
    }
    if (s_tick[i] == NOST && s_tick[(i+2)%4] != NOST) {
      s_tick[i] = s_tick[(i+2)%4];
    }
  }

  for (int i = 0; i < 2; i++) {
    if (s_max[i] == NOST && s_max[(i+2)%4] == NOST) {
      s_max[i] = s_max[(i+2)%4] = (s_data[i] > s_data[(i+2)%4]) ? s_data[i] : s_data[(i+2)%4];
      s_neg[i] = s_neg[(i+2)%4] = (s_neg[i] || s_neg[(i+2)%4]);
    }
  }

  //If neither outer nor inner overlap, they both can be full.
  bool full = !((s_data[0] != NOST) && (s_data[2] != NOST)) && !((s_data[1] != NOST) && (s_data[3] != NOST));
  bool innerOnly = (s_data[0] == NOST) && (s_data[2] == NOST);
  //Draw stats
  for (int i = 0; i < 4; i++) {
    if (s_data[i] != NOST) {
      graphics_context_set_fill_color(ctx, colour[i]);
      drawBar(layer, ctx, s_data[i], s_max[i], s_neg[i], s_tick[i], i < 2, full, i % 2 == 1 && !innerOnly);
    }
  }
}

static void updateSteps() {
  HealthValue steps = health_service_sum_today(HealthMetricStepCount);
  //Send them to app.
}

void stat_inbox(DictionaryIterator *iter, void *context) {
  Tuple *stats_tick_t = dict_find(iter, MESSAGE_KEY_ShowStreams);
  if(stats_tick_t) {
    s_stats = stats_tick_t->value->int32 == 1;
  }
  if (!s_stats) return;

  for (int i = 0; i < 4; i++) {
    Tuple *stat_data_t = dict_find(iter, MESSAGE_KEY_StatsData + i);
    if (stat_data_t) {
      s_data[i] = stat_data_t->value->int32;
    }
    Tuple *stat_max_t = dict_find(iter, MESSAGE_KEY_StatsMax + i);
    if (stat_max_t) {
      s_max[i] = stat_max_t->value->int32;
    }
    Tuple *stat_tick_t = dict_find(iter, MESSAGE_KEY_StatsTick + i);
    if (stat_tick_t) {
      s_tick[i] = stat_tick_t->value->int32;
    }
    Tuple *stat_neg_t = dict_find(iter, MESSAGE_KEY_StatsNeg + i);
    if (stat_neg_t) {
      s_neg[i] = stat_neg_t->value->int32 == 1;
    }
  }
}
