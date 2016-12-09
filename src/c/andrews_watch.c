#include "andrews_watch.h"

#include "pebble.h"

#include "lib.h"
#include "logo.h"
#include "notificons.h"

static Window *s_window;
static Layer *s_simple_bg_layer, *s_simple_stats_layer, *s_date_layer, *s_hands_layer;
static TextLayer *s_day_label, *s_num_label;

static char s_num_buffer[4], s_day_buffer[6];
static int s_tickwidth = 3; //12

//Settings
static bool s_show_seconds = false;
static bool s_backwards = false;
static bool s_utc = false;
static bool s_stats = false;

/* Stats
 * max_left = 0, max_right = 0: Use max of left/right for both max values
 * max_left > 0, max_right = 0: Use max_left for both max values
 * max_left = 0, max_right > 0: Use only max_right and right values
 * max_left > 0, max_right > 0: Use max_left for left and max_right for right
 */
static uint32_t s_stat_left = 0;
static uint32_t s_stat_right = 0;
static uint32_t s_stat_max_left = 0;
static uint32_t s_stat_max_right = 0;
static HealthValue s_max_steps = 100;

static void bg_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint centre = grect_center_point(&bounds);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, GColorWhite);
  const int16_t inner_length = (bounds.size.w / 2) - PBL_IF_ROUND_ELSE(s_tickwidth, 1);
  const int16_t outer_length = bounds.size.h / 2;
  for (int i = 0; i < 12; ++i) {
    int32_t tick_angle = TRIG_MAX_ANGLE * i / 12;
    GPoint tick_inner = {
      .x = (int16_t)(sin_lookup(tick_angle) * (int32_t)inner_length / TRIG_MAX_RATIO) + centre.x,
      .y = (int16_t)(-cos_lookup(tick_angle) * (int32_t)inner_length / TRIG_MAX_RATIO) + centre.y,
    };
    GPoint tick_outer = {
      .x = (int16_t)(sin_lookup(tick_angle) * (int32_t)outer_length / TRIG_MAX_RATIO) + centre.x,
      .y = (int16_t)(-cos_lookup(tick_angle) * (int32_t)outer_length / TRIG_MAX_RATIO) + centre.y,
    };
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_stroke_width(ctx, 1);
    graphics_draw_line(ctx, tick_inner, tick_outer);
  }

  draw_logo(layer, ctx);
  draw_icon(layer, ctx);
}

static void stats_update_proc(Layer *layer, GContext *ctx) {
  if (!s_stats) return;
  GRect bounds = layer_get_bounds(layer);
  
  HealthValue steps = health_service_sum_today(HealthMetricStepCount);
  
  // Red/Blue stream: http://www.sandraandwoo.com/gaia/2012/04/06/shadowdancers-006/
  int32_t start_angle = TRIG_MAX_ANGLE * ((9.0 - 0.1 + 0.2 * a_rand()) / 12.);
  int32_t end_angle   = TRIG_MAX_ANGLE * ((5.5 - 0.1 + 0.2 * a_rand()) / 12.);

  if (s_stat_left > 0 || s_stat_right > 0) {
    float maxl = s_stat_left > s_stat_right ? s_stat_left : s_stat_right;
    if (s_stat_max_left > 0) maxl = s_stat_max_left;
    float maxr = maxl;
    if (s_stat_max_right > 0) maxr = s_stat_max_right;
    start_angle = TRIG_MAX_ANGLE * (1.0 - (s_stat_left / maxl) / 2);
    end_angle = TRIG_MAX_ANGLE * ((s_stat_right / maxr) / 2);
  }
  if (s_stat_max_left == 0 && s_stat_max_right > 0) {
    end_angle *= 2;
    start_angle = TRIG_MAX_ANGLE;
  }
  if (s_backwards) {
    int32_t t = start_angle;
    start_angle = TRIG_MAX_ANGLE - end_angle;
    end_angle = TRIG_MAX_ANGLE - t;
  }
  GRect frame = grect_inset(bounds, GEdgeInsets(PBL_IF_ROUND_ELSE(0, -2)));
  graphics_context_set_fill_color(ctx, GColorRed);
  a_graphics_fill_radial(ctx, frame, GOvalScaleModeFitCircle, 5, start_angle, TRIG_MAX_ANGLE);
  graphics_context_set_fill_color(ctx, GColorPictonBlue);
  a_graphics_fill_radial(ctx, frame, GOvalScaleModeFitCircle, 5, 0, end_angle);
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint centre = grect_center_point(&bounds);

  graphics_context_set_antialiased(ctx, 1);

  const int16_t second_hand_length = (bounds.size.w / 2) - (PBL_IF_ROUND_ELSE(s_tickwidth, 1) + 1);
  const int16_t minute_hand_length = second_hand_length;
  const int16_t hour_hand_length = minute_hand_length * 0.8;
  const int16_t utc_hour_hand_length = hour_hand_length;

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  struct tm *ut = gmtime(&now);
  int32_t second_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
  int32_t minute_angle = TRIG_MAX_ANGLE * (0. + t->tm_min) / 60 + second_angle / 60;
  int32_t utc_minute_angle = TRIG_MAX_ANGLE * (0. + ut->tm_min) / 60 + second_angle / 60;
  int32_t hour_angle = TRIG_MAX_ANGLE * (t->tm_hour % 12) / 12 + minute_angle / 12;
  int32_t utc_hour_angle = TRIG_MAX_ANGLE * ((ut->tm_hour) % 12) / 12 + utc_minute_angle / 12;
  if (s_backwards) {
    hour_angle *= -1;
    utc_hour_angle *= -1;
    minute_angle *= -1;
    second_angle *= -1;
  }

  // minute/hour hand
  GColor hands_color = GColorWhite;
  GColor uhands_color = GColorLightGray;
//  if (t->tm_hour < 6 || t->tm_hour >= 18) hands_color = GColorLightGray;
//  if (ut->tm_hour > 6 || ut->tm_hour >= 18) uhands_color = GColorDarkGray;

  if(s_utc) {
    graphics_context_set_stroke_color(ctx, uhands_color);
    graphics_context_set_stroke_width(ctx, 3);
    draw_hand(ctx, centre, utc_hour_angle, utc_hour_hand_length);
    
    graphics_context_set_fill_color(ctx, uhands_color);
    GRect frame = grect_inset(bounds, GEdgeInsets((bounds.size.w / 2) - 11));
    bool ahead = (ut->tm_year < t->tm_year) || (ut->tm_mon < t->tm_mon) || (ut->tm_mday < t->tm_mday) || (ut->tm_hour < t->tm_hour) || (ut->tm_min < t->tm_min);
    if (ahead) {
      a_graphics_fill_radial(ctx, frame, GOvalScaleModeFitCircle, 2, utc_hour_angle, hour_angle);
    } else {
      a_graphics_fill_radial(ctx, frame, GOvalScaleModeFitCircle, 2, hour_angle, utc_hour_angle);
    }
  }

  graphics_context_set_stroke_color(ctx, hands_color);
  graphics_context_set_stroke_width(ctx, 3);
  draw_hand(ctx, centre, hour_angle, hour_hand_length);
  draw_hand(ctx, centre, minute_angle, minute_hand_length);
  graphics_context_set_fill_color(ctx, hands_color);
  graphics_fill_circle(ctx, centre, 3);

  // second hand
  if (s_show_seconds) {
    graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
    graphics_context_set_stroke_width(ctx, 1);
    draw_hand(ctx, centre, second_angle, second_hand_length);
  
    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
    graphics_fill_circle(ctx, centre, 2);
  }
  
  // dot in the middle
  graphics_context_set_fill_color(ctx, GColorLightGray);
  graphics_fill_circle(ctx, centre, 1);
}

static void date_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(s_day_buffer, sizeof(s_day_buffer), "%a", t);
  text_layer_set_text(s_day_label, s_day_buffer);

  strftime(s_num_buffer, sizeof(s_num_buffer), "%d", t);
  text_layer_set_text(s_num_label, s_num_buffer);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  if (s_show_seconds || tick_time->tm_sec % 15 == 0 || tick_time->tm_sec % 15 == 7) {
    //layer_mark_dirty(window_get_root_layer(s_window));
    layer_mark_dirty(s_hands_layer);
  }
  if (tick_time->tm_sec % 30 == 0) {
    //Notification updates
    layer_mark_dirty(s_simple_stats_layer);
  }

  //Ask for weather update. Should be removed when the app does this.
  if(tick_time->tm_hour % 6 == 0 && tick_time->tm_min == 30 && tick_time->tm_sec == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
  
    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
  
    // Send the message!
    app_message_outbox_send();
  }
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *seconds_tick_t = dict_find(iter, MESSAGE_KEY_ShowSeconds);
  if(seconds_tick_t) {
    s_show_seconds = seconds_tick_t->value->int32 == 1;
  }
  Tuple *back_tick_t = dict_find(iter, MESSAGE_KEY_ShowBackwards);
  if(back_tick_t) {
    s_backwards = back_tick_t->value->int32 == 1;
  }
  Tuple *utc_tick_t = dict_find(iter, MESSAGE_KEY_ShowUTC);
  if(utc_tick_t) {
    s_utc = utc_tick_t->value->int32 == 1;
  }
  Tuple *stats_tick_t = dict_find(iter, MESSAGE_KEY_ShowStreams);
  if(stats_tick_t) {
    s_stats = stats_tick_t->value->int32 == 1;
  }
  for (int i = 0; i < 4; i++) {
    Tuple *stats_vals_t = dict_find(iter, MESSAGE_KEY_Stats + i);
    if(stats_vals_t) {
      if (i == 0) {
        s_stat_left = stats_vals_t->value->uint32;
      } else if (i == 1) {
        s_stat_right = stats_vals_t->value->uint32;
      } else if (i == 2) {
        s_stat_max_left = stats_vals_t->value->uint32;
      } else {
        s_stat_max_right = stats_vals_t->value->uint32;
      }
    }
  }

  icon_inbox(iter, context);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint centre = grect_center_point(&bounds);

  s_simple_bg_layer = layer_create(bounds);
  layer_set_update_proc(s_simple_bg_layer, bg_update_proc);
  layer_add_child(window_layer, s_simple_bg_layer);

  s_simple_stats_layer = layer_create(bounds);
  layer_set_update_proc(s_simple_stats_layer, stats_update_proc);
  layer_add_child(window_layer, s_simple_stats_layer);

  s_date_layer = layer_create(bounds);
  layer_set_update_proc(s_date_layer, date_update_proc);
  layer_add_child(window_layer, s_date_layer);
  
  s_day_label = text_layer_create(GRect(centre.x - 26, centre.y + 30, 27, 20));
  text_layer_set_text(s_day_label, s_day_buffer);
  text_layer_set_background_color(s_day_label, GColorBlack);
  text_layer_set_text_color(s_day_label, GColorWhite);
  text_layer_set_font(s_day_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  
  layer_add_child(s_date_layer, text_layer_get_layer(s_day_label));
  
  s_num_label = text_layer_create(GRect(centre.x + 1, centre.y + 30, 27, 20));
  text_layer_set_text(s_num_label, s_num_buffer);
  text_layer_set_background_color(s_num_label, GColorBlack);
  text_layer_set_text_color(s_num_label, GColorWhite);
  text_layer_set_font(s_num_label, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  layer_add_child(s_date_layer, text_layer_get_layer(s_num_label));

  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);
}

static void window_unload(Window *window) {
  layer_destroy(s_simple_bg_layer);
  layer_destroy(s_date_layer);
  
  text_layer_destroy(s_day_label);
  text_layer_destroy(s_num_label);

  layer_destroy(s_hands_layer);
}

static void init() {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);

  // Open AppMessage connection
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(128, 128);

  s_day_buffer[0] = '\0';
  s_num_buffer[0] = '\0';

  Layer *window_layer = window_get_root_layer(s_window);
  logo_init(window_layer);
  icon_init(window_layer);
  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

static void deinit() {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
