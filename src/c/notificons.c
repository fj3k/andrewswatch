#include <pebble.h>
#include "notificons.h"
#include "lib.h"

//State
static time_t s_last_update;
//Seven day forcast plus today (if checked late in the day)
static int s_forecast[8] = {-2, -2, -2, -2, -2, -2, -2, -2};
static int s_temperature[8] = {-40, -40, -40, -40, -40, -40, -40, -40};
static int s_min_temperature[8] = {-40, -40, -40, -40, -40, -40, -40, -40};

static int s_notificons[9] = {0, 0, 0, 0, 0, 0, 0, 0};

static int s_priority = -1;

static int s_phone_battery = 100;
static bool s_phone_charging = false;
static bool s_no_phone = false;

//Paths
static GPath *s_call_path_ptr = NULL;
static GPath *s_sms_path_ptr = NULL;
static GPath *s_mail_path_ptr = NULL;
static GPath *s_chat_path_ptr = NULL;
static GPath *s_alarm_path_ptr = NULL;
static GPath *s_calendar_path_ptr = NULL;
static char s_calnum_buffer[4];

static GPath *s_sun_1_path_ptr = NULL;
static GPath *s_sun_2_path_ptr = NULL;
static GPath *s_sun_3_path_ptr = NULL;
static GPath *s_sun_4_path_ptr = NULL;
static GPath *s_sun_5_path_ptr = NULL;
static GPath *s_sun_6_path_ptr = NULL;
static GPath *s_sun_7_path_ptr = NULL;
static GPath *s_sun_8_path_ptr = NULL;
static GPath *s_sun_9_path_ptr = NULL;

static GPath *s_overcast_path_ptr = NULL;

static GPath *s_rain_1_path_ptr = NULL;
static GPath *s_rain_2_path_ptr = NULL;
static GPath *s_rain_3_path_ptr = NULL;
static GPath *s_rain_4_path_ptr = NULL;
static GPath *s_rain_5_path_ptr = NULL;

static GPath *s_snow_2_path_ptr = NULL;
static GPath *s_snow_3_path_ptr = NULL;
static GPath *s_snow_4_path_ptr = NULL;
static GPath *s_snow_5_path_ptr = NULL;
static GPath *s_snow_6_path_ptr = NULL;
static GPath *s_snow_7_path_ptr = NULL;

static GPath *s_storm_2_path_ptr = NULL;

static GPath *s_wind_1_path_ptr = NULL;
static GPath *s_wind_2_path_ptr = NULL;
static GPath *s_wind_3_path_ptr = NULL;

static GPath *s_battery_path_ptr = NULL;
static GPath *s_phone_path_ptr = NULL;

static GPath *s_heart_path_ptr = NULL;
static GPath *s_home_path_ptr = NULL;
static GPath *s_friend_path_ptr = NULL;
static GPath *s_work_path_ptr = NULL;
static GPath *s_err_1_path_ptr = NULL;
static GPath *s_err_2_path_ptr = NULL;

static GPath *s_dotdotdot_1_path_ptr = NULL;
static GPath *s_dotdotdot_2_path_ptr = NULL;
static GPath *s_dotdotdot_3_path_ptr = NULL;


//Positions
static GPoint s_weather_pos[3];
static GPoint s_noticon_pos[7];
static GPoint s_priority_pos;
static GPoint s_battery_pos;
static GPoint s_phone_pos;
static GPoint s_dotdotdot_pos;


void init_sun() {
  s_sun_1_path_ptr = gpath_create(&SUN_1_PATH);
  s_sun_2_path_ptr = gpath_create(&SUN_2_PATH);
  s_sun_3_path_ptr = gpath_create(&SUN_3_PATH);
  s_sun_4_path_ptr = gpath_create(&SUN_4_PATH);
  s_sun_5_path_ptr = gpath_create(&SUN_5_PATH);
  s_sun_6_path_ptr = gpath_create(&SUN_6_PATH);
  s_sun_7_path_ptr = gpath_create(&SUN_7_PATH);
  s_sun_8_path_ptr = gpath_create(&SUN_8_PATH);
  s_sun_9_path_ptr = gpath_create(&SUN_9_PATH);
}
void draw_sun(GContext *ctx, GPoint weatherpos, bool half) {
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
  gpath_move_to(s_sun_1_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_sun_1_path_ptr);
  gpath_move_to(s_sun_2_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_sun_2_path_ptr);
  gpath_move_to(s_sun_3_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_sun_3_path_ptr);
  gpath_move_to(s_sun_4_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_sun_4_path_ptr);
  gpath_move_to(s_sun_5_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_sun_5_path_ptr);
  if (!half) {
    gpath_move_to(s_sun_6_path_ptr, weatherpos);
    gpath_draw_outline(ctx, s_sun_6_path_ptr);
    gpath_move_to(s_sun_7_path_ptr, weatherpos);
    gpath_draw_outline(ctx, s_sun_7_path_ptr);
    gpath_move_to(s_sun_8_path_ptr, weatherpos);
    gpath_draw_outline(ctx, s_sun_8_path_ptr);
  }
  gpath_move_to(s_sun_9_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_sun_9_path_ptr);
}
void init_cloud() {
  s_overcast_path_ptr = gpath_create(&OVERCAST_PATH);
  s_rain_1_path_ptr = gpath_create(&RAIN_1_PATH);
}
void draw_cloud(GContext *ctx, GPoint weatherpos, bool open) {
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite));
  if (open) {
    gpath_move_to(s_rain_1_path_ptr, weatherpos);
    gpath_draw_outline_open(ctx, s_rain_1_path_ptr);
  } else {
    gpath_move_to(s_overcast_path_ptr, weatherpos);
    gpath_draw_outline(ctx, s_overcast_path_ptr);
  }
}
void erase_cloud(GContext *ctx, GPoint weatherpos) {
  graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorClear, GColorBlack));
  gpath_move_to(s_overcast_path_ptr, weatherpos);
  gpath_draw_filled(ctx, s_overcast_path_ptr);
}
void init_rain() {
  s_rain_2_path_ptr = gpath_create(&RAIN_2_PATH);
  s_rain_3_path_ptr = gpath_create(&RAIN_3_PATH);
  s_rain_4_path_ptr = gpath_create(&RAIN_4_PATH);
  s_rain_5_path_ptr = gpath_create(&RAIN_5_PATH);
}
void draw_rain(GContext *ctx, GPoint weatherpos) {
  draw_cloud(ctx, weatherpos, true);
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorPictonBlue, GColorWhite));
  gpath_move_to(s_rain_2_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_rain_2_path_ptr);
  gpath_move_to(s_rain_3_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_rain_3_path_ptr);
  gpath_move_to(s_rain_4_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_rain_4_path_ptr);
  gpath_move_to(s_rain_5_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_rain_5_path_ptr);
}
void init_snow() {
  s_snow_2_path_ptr = gpath_create(&SNOW_2_PATH);
  s_snow_3_path_ptr = gpath_create(&SNOW_3_PATH);
  s_snow_4_path_ptr = gpath_create(&SNOW_4_PATH);
  s_snow_5_path_ptr = gpath_create(&SNOW_5_PATH);
  s_snow_6_path_ptr = gpath_create(&SNOW_6_PATH);
  s_snow_7_path_ptr = gpath_create(&SNOW_7_PATH);
}
void draw_snow(GContext *ctx, GPoint weatherpos) {
  draw_cloud(ctx, weatherpos, true);
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorElectricBlue, GColorWhite));
  gpath_move_to(s_snow_2_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_snow_2_path_ptr);
  gpath_move_to(s_snow_3_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_snow_3_path_ptr);
  gpath_move_to(s_snow_4_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_snow_4_path_ptr);
  gpath_move_to(s_snow_5_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_snow_5_path_ptr);
  gpath_move_to(s_snow_6_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_snow_6_path_ptr);
  gpath_move_to(s_snow_7_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_snow_7_path_ptr);
}
void init_storm() {
  s_storm_2_path_ptr = gpath_create(&STORM_2_PATH);
}
void draw_storm(GContext *ctx, GPoint weatherpos) {
  draw_cloud(ctx, weatherpos, true);
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
  gpath_move_to(s_storm_2_path_ptr, weatherpos);
  gpath_draw_outline(ctx, s_storm_2_path_ptr);
}
void init_wind() {
  s_wind_1_path_ptr = gpath_create(&WIND_1_PATH);
  s_wind_2_path_ptr = gpath_create(&WIND_2_PATH);
  s_wind_3_path_ptr = gpath_create(&WIND_3_PATH);
}
void draw_wind(GContext *ctx, GPoint weatherpos, bool dust) {
  graphics_context_set_stroke_color(ctx, !dust ? PBL_IF_COLOR_ELSE(GColorElectricBlue, GColorWhite) : PBL_IF_COLOR_ELSE(GColorRajah, GColorWhite));
  gpath_move_to(s_wind_1_path_ptr, weatherpos);
  gpath_draw_outline_open(ctx, s_wind_1_path_ptr);
  gpath_move_to(s_wind_2_path_ptr, weatherpos);
  gpath_draw_outline_open(ctx, s_wind_2_path_ptr);
  gpath_move_to(s_wind_3_path_ptr, weatherpos);
  gpath_draw_outline_open(ctx, s_wind_3_path_ptr);
}

/**
 * icon_init
 */
void icon_init(Layer *window_layer) {
  GRect bounds = layer_get_bounds(window_layer);
  GPoint centre = grect_center_point(&bounds);
  GPoint icontre = GPoint(centre.x - 8, centre.y - 8);
  
  //Positions
  int32_t hand_length = (int32_t)centre.x - PBL_IF_ROUND_ELSE(2, 1) - 15;
  for (int i = 15; i < 22; ++i) {
    int32_t tick_angle = TRIG_MAX_ANGLE * i / 24;
    s_noticon_pos[21 - i] = GPoint((int16_t)(sin_lookup(tick_angle) * hand_length / TRIG_MAX_RATIO) + icontre.x, (int16_t)(-cos_lookup(tick_angle) * hand_length / TRIG_MAX_RATIO) + icontre.y);
  }
  int32_t dotdotdot_angle = TRIG_MAX_ANGLE * 14 / 24;
  s_dotdotdot_pos = GPoint((int16_t)(sin_lookup(dotdotdot_angle) * hand_length / TRIG_MAX_RATIO) + icontre.x, (int16_t)(-cos_lookup(dotdotdot_angle) * hand_length / TRIG_MAX_RATIO) + icontre.y);
  for (int i = 2; i <= 4; ++i) {
    int32_t tick_angle = TRIG_MAX_ANGLE * i / 12;
    s_weather_pos[i - 2] = GPoint((int16_t)(sin_lookup(tick_angle) * hand_length / TRIG_MAX_RATIO) + icontre.x, (int16_t)(-cos_lookup(tick_angle) * hand_length / TRIG_MAX_RATIO) + icontre.y);
  }
  s_priority_pos = GPoint(icontre.x - (hand_length - 25), icontre.y);
  s_battery_pos = GPoint(icontre.x + (hand_length - 45), icontre.y);
  s_phone_pos = GPoint(icontre.x + (hand_length - 25), icontre.y);

  //Notifications
  s_call_path_ptr = gpath_create(&CALL_PATH);
  s_sms_path_ptr = gpath_create(&SMS_PATH);
  s_mail_path_ptr = gpath_create(&MAIL_PATH);
  s_chat_path_ptr = gpath_create(&CHAT_PATH);
  s_alarm_path_ptr = gpath_create(&ALARM_PATH);
  s_calendar_path_ptr = gpath_create(&CALENDAR_PATH);
  s_calnum_buffer[0] = '\0';

  //Weather
  init_sun();
  init_cloud();
  init_rain();
  init_snow();
  init_storm();
  init_wind();

  //Priority
  s_battery_path_ptr = gpath_create(&BATTERY_PATH);
  s_phone_path_ptr = gpath_create(&PHONE_PATH);

  //Priority
  s_heart_path_ptr = gpath_create(&HEART_PATH);
  s_home_path_ptr = gpath_create(&HOME_PATH);
  s_friend_path_ptr = gpath_create(&FRIEND_PATH);
  s_work_path_ptr = gpath_create(&WORK_PATH);
  s_err_1_path_ptr = gpath_create(&ERR_1_PATH);
  s_err_2_path_ptr = gpath_create(&ERR_2_PATH);

  //Dot dot dot
  s_dotdotdot_1_path_ptr = gpath_create(&DOTDOTDOT_1_PATH);
  gpath_move_to(s_dotdotdot_1_path_ptr, s_dotdotdot_pos);
  s_dotdotdot_2_path_ptr = gpath_create(&DOTDOTDOT_2_PATH);
  gpath_move_to(s_dotdotdot_2_path_ptr, s_dotdotdot_pos);
  s_dotdotdot_3_path_ptr = gpath_create(&DOTDOTDOT_3_PATH);
  gpath_move_to(s_dotdotdot_3_path_ptr, s_dotdotdot_pos);
}

/**
 * draw_icon
 */
void draw_icon(Layer *layer, GContext *ctx) {
  GColor call_colour = PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
  GColor msg_colour = PBL_IF_COLOR_ELSE(GColorGreen, GColorWhite);
  GColor mail_colour = PBL_IF_COLOR_ELSE(GColorElectricBlue, GColorWhite);
  GColor alrt_colour = PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite);

  GColor err_colour = PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite);
  GColor heart_colour = PBL_IF_COLOR_ELSE(GColorFolly, GColorWhite);
  GColor home_colour = PBL_IF_COLOR_ELSE(GColorGreen, GColorWhite);
  GColor friend_colour = PBL_IF_COLOR_ELSE(GColorElectricBlue, GColorWhite);
  GColor work_colour = PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite);

  graphics_context_set_stroke_width(ctx, 1);
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  //Weather
  int weatherstart = 0;
  if (s_last_update) {
    struct tm *wt = localtime(&s_last_update);
    //Find midnight at the start of the last update day.
    time_t dayOfUpdate = s_last_update - wt->tm_hour * 3600 + wt->tm_min * 60 + wt->tm_sec;
    weatherstart = (now - dayOfUpdate) / 86400;
  }
  for (int i = weatherstart; i < weatherstart + 3; i++) {
    GPoint weatherpos = s_weather_pos[i - weatherstart];
    int weatherforecast = i < 8 ? s_forecast[i] : WEATHER_NONE;
    int temp = i < 8 ? s_temperature[i] : -40;
    if (
      weatherforecast == WEATHER_SUN
      || weatherforecast == WEATHER_PARTCLOUD
      || weatherforecast == WEATHER_SHOWERS
      || weatherforecast == WEATHER_FOG
      || weatherforecast == WEATHER_HAZE
    ) {
      draw_sun(ctx, weatherpos, weatherforecast != WEATHER_SUN);
    }
    if (weatherforecast == WEATHER_PARTCLOUD || weatherforecast == WEATHER_SHOWERS) {
      erase_cloud(ctx, weatherpos);
    }
    if (weatherforecast == WEATHER_CLOUD || weatherforecast == WEATHER_PARTCLOUD) {
      draw_cloud(ctx, weatherpos, false);
    } else if (weatherforecast == WEATHER_RAIN || weatherforecast == WEATHER_SHOWERS) {
      draw_rain(ctx, weatherpos);
    } else if (weatherforecast == WEATHER_SNOW) {
      draw_snow(ctx, weatherpos);
    } else if (weatherforecast == WEATHER_FOG || weatherforecast == WEATHER_HAZE) {
      graphics_context_set_stroke_color(ctx, weatherforecast == WEATHER_FOG ? PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite) : PBL_IF_COLOR_ELSE(GColorRajah, GColorWhite));
      for (int i = 9; i < 16; i += 2) {
        graphics_draw_line(ctx, GPoint(weatherpos.x + 0, weatherpos.y + i), GPoint(weatherpos.x + 16, weatherpos.y + i));
      }
    } else if (weatherforecast == WEATHER_STORM) {
      draw_storm(ctx, weatherpos);
    } else if (weatherforecast == WEATHER_WINDY || weatherforecast == WEATHER_DUSTSTORM) {
      draw_wind(ctx, weatherpos, weatherforecast == WEATHER_DUSTSTORM);
    }

    if (i == 0 && temp == -40 && s_min_temperature[1] != -40) {
      graphics_context_set_text_color(ctx, PBL_IF_COLOR_ELSE(GColorElectricBlue, GColorWhite));
      temp = s_min_temperature[1];
    } else {
      graphics_context_set_text_color(ctx, GColorWhite);
    }

    if (temp == 0) {
      char temp_buffer[2] = "0\0";
      graphics_draw_text(ctx, temp_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(weatherpos.x - 16, weatherpos.y + 6, 20, 14), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
    } else if (temp != -40) {
      char temp_buffer[4] = "   \0";
      bool neg = (temp < 0);
      if (neg) temp *= -1;
      for (int j = 2; j >= 0; j--) {
        if (temp == 0) {
          if (neg) temp_buffer[j] = '-';
          break;
        }
        temp_buffer[j] = (temp % 10) + '0';
        temp = temp / 10;
      }
      graphics_draw_text(ctx, temp_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(weatherpos.x - 16, weatherpos.y + 6, 20, 14), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
    }
  }

  //Notifications
  int icons = 0, origCount = 0;
  int draw_notificons[9];
  for (int i = 0; i < 8; i++) {
    draw_notificons[i] = s_notificons[i];
    if (s_notificons[i] > 0) icons++;
  }
  origCount = icons;
  //Thin down if we have too many
  if (icons > 4 && draw_notificons[NOTIF_SMS] > 0 && draw_notificons[NOTIF_CHAT] > 0) {
    draw_notificons[NOTIF_CHAT] = 0;
    icons--;
  }
  if (icons > 4 && draw_notificons[NOTIF_ALARM] > 0 && draw_notificons[NOTIF_CALENDAR] > 0) {
    draw_notificons[NOTIF_CALENDAR] = 0;
    icons--;
  }
  if (origCount > 4) {
    graphics_context_set_stroke_color(ctx, GColorWhite);
    gpath_draw_outline(ctx, s_dotdotdot_1_path_ptr);
    gpath_draw_outline(ctx, s_dotdotdot_2_path_ptr);
    gpath_draw_outline(ctx, s_dotdotdot_3_path_ptr);
  }
  int iconIndex = 4 - icons;

  graphics_context_set_stroke_color(ctx, call_colour);
  if (draw_notificons[NOTIF_CALL] > 0) {
    gpath_move_to(s_call_path_ptr, s_noticon_pos[iconIndex]);
    iconIndex += 2;
    gpath_draw_outline(ctx, s_call_path_ptr);
  }

  graphics_context_set_stroke_color(ctx, msg_colour);
  if (draw_notificons[NOTIF_SMS] > 0) {
    gpath_move_to(s_sms_path_ptr, s_noticon_pos[iconIndex]);
    iconIndex += 2;
    gpath_draw_outline(ctx, s_sms_path_ptr);
  }
  if (draw_notificons[NOTIF_CHAT] > 0) {
    gpath_move_to(s_chat_path_ptr, s_noticon_pos[iconIndex]);
    iconIndex += 2;
    gpath_draw_outline(ctx, s_chat_path_ptr);
  }

  graphics_context_set_stroke_color(ctx, mail_colour);
  if (draw_notificons[NOTIF_MAIL] > 0) {
    gpath_move_to(s_mail_path_ptr, s_noticon_pos[iconIndex]);
    iconIndex += 2;
    gpath_draw_outline_open(ctx, s_mail_path_ptr);
  }

  graphics_context_set_stroke_color(ctx, alrt_colour);
  graphics_context_set_text_color(ctx, alrt_colour);
  if (draw_notificons[NOTIF_ALARM] > 0) {
    GPoint posi = s_noticon_pos[iconIndex];
    iconIndex += 2;
    gpath_move_to(s_alarm_path_ptr, posi);
    gpath_draw_outline(ctx, s_alarm_path_ptr);

    GPoint clockpoint = GPoint(posi.x + 8, posi.y + 8);
    draw_simple_clock(ctx, now, clockpoint, 6, 6);
  }
  if (draw_notificons[NOTIF_CALENDAR] > 0) {
    strftime(s_calnum_buffer, sizeof(s_calnum_buffer), "%d", t);
    graphics_draw_text(ctx, s_calnum_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(s_noticon_pos[iconIndex].x, s_noticon_pos[iconIndex].y + 1, 16, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

    gpath_move_to(s_calendar_path_ptr, s_noticon_pos[iconIndex]);
    iconIndex += 2;
    gpath_draw_outline(ctx, s_calendar_path_ptr);
  }

  //Priorities
  if (s_priority == PRIORITY_ERROR) {
    graphics_context_set_stroke_color(ctx, err_colour);
    gpath_move_to(s_err_1_path_ptr, s_priority_pos);
    gpath_draw_outline(ctx, s_err_1_path_ptr);
    gpath_move_to(s_err_2_path_ptr, s_priority_pos);
    gpath_draw_outline(ctx, s_err_2_path_ptr);
  } else if (s_priority == PRIORITY_HEART) {
    graphics_context_set_stroke_color(ctx, heart_colour);
    gpath_move_to(s_heart_path_ptr, s_priority_pos);
    gpath_draw_outline(ctx, s_heart_path_ptr);
  } else if (s_priority == PRIORITY_HOME) {
    graphics_context_set_stroke_color(ctx, home_colour);
    gpath_move_to(s_home_path_ptr, s_priority_pos);
    gpath_draw_outline(ctx, s_home_path_ptr);
  } else if (s_priority == PRIORITY_FRIEND) {
    graphics_context_set_stroke_color(ctx, friend_colour);
    gpath_move_to(s_friend_path_ptr, s_priority_pos);
    gpath_draw_outline_open(ctx, s_friend_path_ptr);
  } else if (s_priority == PRIORITY_WORK) {
    graphics_context_set_stroke_color(ctx, work_colour);
    gpath_move_to(s_work_path_ptr, s_priority_pos);
    gpath_draw_outline(ctx, s_work_path_ptr);
  }

  BatteryChargeState charge_state = battery_state_service_peek();
  if (charge_state.charge_percent < 20 || charge_state.is_charging) {
    if (charge_state.charge_percent < 10) {
      graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
    } else if (charge_state.charge_percent < 20) {
      graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
    } else {
      graphics_context_set_stroke_color(ctx, GColorWhite);
    }
    gpath_move_to(s_battery_path_ptr, s_battery_pos);
    gpath_draw_outline(ctx, s_battery_path_ptr);
    int chargeOffset = 14 - (charge_state.charge_percent / 100. * 12);
    graphics_draw_line(ctx, GPoint(s_battery_pos.x + 5, s_battery_pos.y + chargeOffset), GPoint(s_battery_pos.x + 11, s_battery_pos.y + chargeOffset));
    if (charge_state.is_charging) {
      graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorElectricBlue, GColorWhite));
      gpath_move_to(s_storm_2_path_ptr, GPoint(s_battery_pos.x, s_battery_pos.y - 4));
      gpath_draw_outline(ctx, s_storm_2_path_ptr);
    }
  }
  if (s_phone_battery < 20 || s_phone_charging || s_no_phone) {
    graphics_context_set_stroke_color(ctx, GColorWhite);
    gpath_move_to(s_phone_path_ptr, s_phone_pos);
    gpath_draw_outline(ctx, s_phone_path_ptr);
    if (s_phone_battery < 20 || s_phone_charging) {
      if (s_phone_battery < 10) {
        graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
      } else if (s_phone_battery < 20) {
        graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
      }
      gpath_move_to(s_battery_path_ptr, s_phone_pos);
      gpath_draw_outline(ctx, s_battery_path_ptr);
      int chargeOffset = 14 - (s_phone_battery / 100. * 12);
      graphics_draw_line(ctx, GPoint(s_phone_pos.x + 5, s_phone_pos.y + chargeOffset), GPoint(s_phone_pos.x + 11, s_phone_pos.y + chargeOffset));
      if (s_phone_charging) {
        graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorElectricBlue, GColorWhite));
        gpath_move_to(s_storm_2_path_ptr, GPoint(s_phone_pos.x, s_phone_pos.y - 4));
        gpath_draw_outline(ctx, s_storm_2_path_ptr);
      }
    }
    if (s_no_phone) {
      graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorBlack, GColorBlack));
      graphics_context_set_stroke_width(ctx, 5);
      graphics_draw_line(ctx, GPoint(s_phone_pos.x, s_phone_pos.y + 12), GPoint(s_phone_pos.x + 16, s_phone_pos.y + 4));
      graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
      graphics_context_set_stroke_width(ctx, 1);
      graphics_draw_line(ctx, GPoint(s_phone_pos.x, s_phone_pos.y + 12), GPoint(s_phone_pos.x + 16, s_phone_pos.y + 4));
    }
  }
}

void icon_inbox(DictionaryIterator *iter, void *context) {
  Tuple *notif_call_t = dict_find(iter, MESSAGE_KEY_NotifCall);
  if (notif_call_t) {
    s_notificons[NOTIF_CALL] = notif_call_t->value->int32;
  }
  Tuple *notif_sms_t = dict_find(iter, MESSAGE_KEY_NotifSMS);
  if (notif_sms_t) {
    s_notificons[NOTIF_SMS] = notif_sms_t->value->int32;
  }
  Tuple *notif_mail_t = dict_find(iter, MESSAGE_KEY_NotifMail);
  if (notif_mail_t) {
    s_notificons[NOTIF_MAIL] = notif_mail_t->value->int32;
  }
  Tuple *notif_chat_t = dict_find(iter, MESSAGE_KEY_NotifChat);
  if (notif_chat_t) {
    s_notificons[NOTIF_CHAT] = notif_chat_t->value->int32;
  }
  Tuple *notif_alarm_t = dict_find(iter, MESSAGE_KEY_NotifAlarm);
  if (notif_alarm_t) {
    s_notificons[NOTIF_ALARM] = notif_alarm_t->value->int32;
  }
  Tuple *notif_calendar_t = dict_find(iter, MESSAGE_KEY_NotifCalendar);
  if (notif_calendar_t) {
    s_notificons[NOTIF_CALENDAR] = notif_calendar_t->value->int32;
  }
  for (int i = 0; i < 8; i++) {
    Tuple *notif_forecast_t = dict_find(iter, MESSAGE_KEY_Forecast + i);
    if (notif_forecast_t) {
      switch (notif_forecast_t->value->int32) {
        case 1:  //Sunny
        case 2:  //Clear
          s_forecast[i] = WEATHER_SUN;
          break;
        case 3:  //Partly cloudy
          s_forecast[i] = WEATHER_PARTCLOUD;
          break;
        case 4:  //Cloudy
          s_forecast[i] = WEATHER_CLOUD;
          break;
        case 10: //Fog
          s_forecast[i] = WEATHER_FOG;
          break;
        case 6:  //Hazy
          s_forecast[i] = WEATHER_HAZE;
          break;
        case 8:  //Light rain
        case 12: //Rain
          s_forecast[i] = WEATHER_RAIN;
          break;
        case 11: //Shower
        case 17: //Light shower
          s_forecast[i] = WEATHER_SHOWERS;
          break;
        case 16: //Storm
          s_forecast[i] = WEATHER_STORM;
          break;
        case 9:  //Windy
          s_forecast[i] = WEATHER_WINDY;
          break;
        case 13: //Dusty
          s_forecast[i] = WEATHER_DUSTSTORM;
          break;
        case 14: //Frost
        case 15: //Snow
          s_forecast[i] = WEATHER_SNOW;
          break;
        default:
          s_forecast[i] = WEATHER_NONE;
      }
      s_last_update = time(NULL);
    }
    Tuple *notif_temperature_t = dict_find(iter, MESSAGE_KEY_Temperature + i);
    if (notif_temperature_t) {
      s_temperature[i] = notif_temperature_t->value->int32;
    }
    Tuple *notif_min_temperature_t = dict_find(iter, MESSAGE_KEY_MinTemp + i);
    if (notif_min_temperature_t) {
      s_min_temperature[i] = notif_min_temperature_t->value->int32;
    }
  }
  Tuple *notif_priority_t = dict_find(iter, MESSAGE_KEY_Priority);
  if (notif_priority_t) {
    s_priority = notif_priority_t->value->int32;
  }
  Tuple *notif_battery_t = dict_find(iter, MESSAGE_KEY_PhoneBattery);
  if (notif_battery_t) {
    s_phone_battery = notif_battery_t->value->int32;
  }
  Tuple *notif_charging_t = dict_find(iter, MESSAGE_KEY_PhoneCharging);
  if (notif_charging_t) {
    s_phone_charging = notif_charging_t->value->int32;
  }
}

void icon_connex(bool connected) {
  s_no_phone = !connected;
}