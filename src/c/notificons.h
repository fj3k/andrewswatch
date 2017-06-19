#pragma once

void icon_init(Layer*);
void draw_icon(Layer*, GContext*);
void icon_inbox(DictionaryIterator*, void*);
void icon_connex(bool connected);

static const int NOTIF_NONE = -1;
static const int NOTIF_CALL = 0;
static const int NOTIF_SMS = 1;
static const int NOTIF_MAIL = 2;
static const int NOTIF_CHAT = 3;
static const int NOTIF_ALARM = 4;
static const int NOTIF_CALENDAR = 5;
static const int NOTIF_NEWSFEED = 6;
static const int NOTIF_SOCIALMEDIA = 7;

static const int WEATHER_NONE = -2;
static const int WEATHER_SNOW = -1;
static const int WEATHER_RAIN = 0;
static const int WEATHER_CLOUD = 1;
static const int WEATHER_SUN = 2;
static const int WEATHER_PARTCLOUD = 3;
static const int WEATHER_SHOWERS = 4;
static const int WEATHER_FOG = 5;
static const int WEATHER_HAZE = 6;
static const int WEATHER_STORM = 7;
static const int WEATHER_WINDY = 8;
static const int WEATHER_DUSTSTORM = 9;

static const int PRIORITY_NONE = -1;
static const int PRIORITY_ERROR = 99;
static const int PRIORITY_HEART = 3;
static const int PRIORITY_HOME = 2;
static const int PRIORITY_FRIEND = 1;
static const int PRIORITY_WORK = 0;


//Call
static const GPathInfo CALL_PATH = {
  .num_points = 13,
  .points = (GPoint []) {{0, 14}, {0, 8}, {8, 0}, {14, 0}, {16, 2}, {16, 4}, {14, 6}, {10, 6}, {6, 10}, {6, 14}, {4, 16}, {2, 16}, {0, 14}}
};

//SMS
static const GPathInfo SMS_PATH = {
  .num_points = 12,
  .points = (GPoint []) {{2, 14}, {0, 12}, {0, 4}, {2, 2}, {14, 2}, {16, 4}, {16, 12}, {14, 14}, {8, 14}, {6, 16}, {4, 14}, {2, 14}}
};

//Mail
static const GPathInfo MAIL_PATH = {
  .num_points = 11,
  .points = (GPoint []) {{0, 4}, {2, 2}, {14, 2}, {16, 4}, {16, 12}, {14, 14}, {2, 14}, {0, 12}, {0, 4}, {8, 8}, {16, 4}}
};

//Chat
static const GPathInfo CHAT_PATH = {
  .num_points = 24,
  .points = (GPoint []) {{6, 16}, {4, 14}, {2, 14}, {0, 12}, {0, 11}, {2, 9}, {10, 9}, {8, 7}, {2, 7}, {0, 5}, {0, 4}, {2, 2}, {14, 2}, {16, 4}, {16, 5}, {14, 7}, {12, 7}, {10, 9}, {14, 9}, {16, 11}, {16, 12}, {14, 14}, {8, 14}, {6, 16}}
};

//Alarm
static const GPathInfo ALARM_PATH = {
  .num_points = 9,
  .points = (GPoint []) {{0, 4}, {4, 0}, {12, 0}, {16, 4}, {16, 12}, {12, 16}, {4, 16}, {0, 12}, {0, 4}}
};

//Calendar
static const GPathInfo CALENDAR_PATH = {
  .num_points = 12,
  .points = (GPoint []) {{0, 4}, {16, 4}, {16, 14}, {14, 16}, {2, 16}, {0, 14}, {0, 2}, {2, 0}, {14, 0}, {16, 2}, {16, 4}, {0, 4}}
};

//Sun
static const GPathInfo SUN_1_PATH = {
  .num_points = 9,
  .points = (GPoint []) {{6, 7}, {7, 6}, {9, 6}, {10, 7}, {10, 9}, {9, 10}, {7, 10}, {6, 9}, {6, 7}}
};
static const GPathInfo SUN_2_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{4, 4}, {2, 2}}
};
static const GPathInfo SUN_3_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{8, 3}, {8, 0}}
};
static const GPathInfo SUN_4_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{12, 4}, {14, 2}}
};
static const GPathInfo SUN_5_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{13, 8}, {16, 8}}
};
static const GPathInfo SUN_6_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{12, 12}, {14, 14}}
};
static const GPathInfo SUN_7_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{8, 13}, {8, 16}}
};
static const GPathInfo SUN_8_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{4, 12}, {2, 14}}
};
static const GPathInfo SUN_9_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{3, 8}, {0, 8}}
};

//Overcast
static const GPathInfo OVERCAST_PATH = {
  .num_points = 13,
  .points = (GPoint []) {{2, 14}, {0, 12}, {0, 10}, {2, 8}, {4, 8}, {6, 6}, {8, 6}, {10, 4}, {14, 4}, {16, 6}, {16, 10}, {12, 14}, {2, 14}}
};

//Rain
static const GPathInfo RAIN_1_PATH = {
  .num_points = 12,
  .points = (GPoint []) {{2, 14}, {0, 12}, {0, 10}, {2, 8}, {4, 8}, {6, 6}, {8, 6}, {10, 4}, {14, 4}, {16, 6}, {16, 12}, {14, 14}}
};
static const GPathInfo RAIN_2_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{6, 16}, {6, 14}}
};
static const GPathInfo RAIN_3_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{6, 12}, {6, 10}}
};
static const GPathInfo RAIN_4_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{10, 14}, {10, 12}}
};
static const GPathInfo RAIN_5_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{10, 10}, {10, 8}}
};

//Snow (uses RAIN_1)
static const GPathInfo SNOW_2_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{13, 11}, {9, 9}}
};
static const GPathInfo SNOW_3_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{9, 11}, {13, 9}}
};
static const GPathInfo SNOW_4_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{11, 12}, {11, 8}}
};
static const GPathInfo SNOW_5_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{4, 15}, {8, 13}}
};
static const GPathInfo SNOW_6_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{4, 13}, {8, 15}}
};
static const GPathInfo SNOW_7_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{6, 12}, {6, 16}}
};

//Storm (uses RAIN_1)
static const GPathInfo STORM_2_PATH = {
  .num_points = 4,
  .points = (GPoint []) {{8, 8}, {7, 12}, {9, 12}, {8, 16}}
};

//Wind
static const GPathInfo WIND_1_PATH = {
  .num_points = 6,
  .points = (GPoint []) {{0, 8}, {14, 8}, {16, 10}, {16, 12}, {14, 14}, {12, 14}}
};
static const GPathInfo WIND_2_PATH = {
  .num_points = 6,
  .points = (GPoint []) {{0, 6}, {10, 6}, {12, 4}, {12, 2}, {10, 0}, {8, 0}, {6, 2}}
};
static const GPathInfo WIND_3_PATH = {
  .num_points = 6,
  .points = (GPoint []) {{0, 10}, {8, 10}, {10, 12}, {10, 14}, {8, 16}, {6, 16}, {4, 14}}
};


//Battery warning
static const GPathInfo BATTERY_PATH = {
  .num_points = 9,
  .points = (GPoint []) {{11, 14}, {5, 14}, {5, 3}, {7, 3}, {7, 2}, {9, 2}, {9, 3}, {11, 3}, {11, 14}}
};
static const GPathInfo PHONE_PATH = {
  .num_points = 10,
  .points = (GPoint []) {{13, 16}, {12, 17}, {4, 17}, {3, 16}, {3, 1}, {4, 0}, {12, 0}, {13, 1}, {13, 16}, {3, 16}}
};


//Heart
static const GPathInfo HEART_PATH = {
  .num_points = 11,
  .points = (GPoint []) {{0, 4}, {4, 0}, {6, 0}, {8, 2}, {10, 0}, {12, 0}, {16, 4}, {16, 8}, {8, 16}, {0, 8}, {0, 4}}
};

//Home
static const GPathInfo HOME_PATH = {
  .num_points = 6,
  .points = (GPoint []) {{0, 16}, {0, 8}, {8, 0}, {16, 8}, {16, 16}, {0, 16}}
};

//Friend
static const GPathInfo FRIEND_PATH = {
  .num_points = 14,
  .points = (GPoint []) {{4, 6}, {4, 2}, {6, 0}, {10, 0}, {12, 2}, {12, 6}, {16, 10}, {16, 16}, {0, 16}, {0, 10}, {4, 6}, {6, 8}, {10, 8}, {12, 6}}
};

//Work
static const GPathInfo WORK_PATH = {
  .num_points = 8,
  .points = (GPoint []) {{10, 6}, {16, 6}, {16, 16}, {10, 16}, {10, 0}, {0, 0}, {0, 16}, {10, 16}}
};

//Error
static const GPathInfo ERR_1_PATH = {
  .num_points = 4,
  .points = (GPoint []) {{8, 0}, {0, 16}, {16, 16}, {8, 0}}
};
static const GPathInfo ERR_2_PATH = {
  .num_points = 2,
  .points = (GPoint []) {{8, 4}, {8, 12}}
};


//Dot dot dot
static const GPathInfo DOTDOTDOT_1_PATH = {
  .num_points = 4,
  .points = (GPoint []) {{3, 11}, {5, 11}, {5, 13}, {3, 13}}
};
static const GPathInfo DOTDOTDOT_2_PATH = {
  .num_points = 4,
  .points = (GPoint []) {{7, 11}, {9, 11}, {9, 13}, {7, 13}}
};
static const GPathInfo DOTDOTDOT_3_PATH = {
  .num_points = 4,
  .points = (GPoint []) {{11, 11}, {13, 11}, {13, 13}, {11, 13}}
};

