module.exports = [
  {
    "type": "heading",
    "defaultValue": "Watch Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Choose settings"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Basic"
      },
      {
        "type": "toggle",
        "messageKey": "ShowSeconds",
        "defaultValue": false,
        "label": "Show seconds"
      },
      {
        "type": "toggle",
        "messageKey": "ShowBackwards",
        "defaultValue": false,
        "label": "Show backwards"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "More Settings"
      },
      {
        "type": "toggle",
        "messageKey": "ShowUTC",
        "defaultValue": false,
        "label": "Show UTC"
      },
      {
        "type": "toggle",
        "messageKey": "ShowStreams",
        "defaultValue": false,
        "label": "Show red/blue stream"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Fake notifications"
      },
      {
        "type": "toggle",
        "messageKey": "NotifCall",
        "defaultValue": false,
        "label": "Call"
      },
      {
        "type": "toggle",
        "messageKey": "NotifSMS",
        "defaultValue": false,
        "label": "SMS"
      },
      {
        "type": "toggle",
        "messageKey": "NotifMail",
        "defaultValue": false,
        "label": "email"
      },
      {
        "type": "toggle",
        "messageKey": "NotifChat",
        "defaultValue": false,
        "label": "Chat"
      },
      {
        "type": "toggle",
        "messageKey": "NotifAlarm",
        "defaultValue": false,
        "label": "Alarm"
      },
      {
        "type": "toggle",
        "messageKey": "NotifCalendar",
        "defaultValue": false,
        "label": "Calendar"
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];