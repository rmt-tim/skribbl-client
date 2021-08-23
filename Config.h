#pragma once
#pragma once
#include <string>

#define DESIRED_FPS 60
#define MILISECONDS 1000
#define DESIRED_FRAME_TIME (MILISECONDS / DESIRED_FPS)

// screen config
static const std::string TITLE = "Game";
static const int SCREEN_WIDTH = 1080;
static const int SCREEN_HEIGHT = 720;

// color picker screen config
static const std::string COLOR_PICKER = "Color picker";
static const int COLOR_PICKER_WIDTH = 500;
static const int COLOR_PICKER_HEIGHT = 500;

// world config
static const int UNIT_WIDTH = 60;
static const int UNIT_HEIGHT = 60;

static const int VERTICAL_UNITS = SCREEN_WIDTH / UNIT_WIDTH;
static const int HORIZONTAL_UNITS = SCREEN_HEIGHT / UNIT_HEIGHT;

// =================== < Button IDs > =================== //
static const int PLUS_BUTTON_ID = 0;
static const int MINUS_BUTTON_ID = 1;

static const int PENCIL_BUTTON_ID = 2;
static const int BUCKET_BUTTON_ID = 3;
static const int RUBBER_BUTTON_ID = 4;
static const int TEXT_BUTTON_ID = 5;

// cursor config
static const int PENCIL_CURSOR_HOT_X = 2;
static const int PENCIL_CURSOR_HOT_Y = 29;

static const int BUCKET_CURSOR_HOT_X = 13;
static const int BUCKET_CURSOR_HOT_Y = 16;

static const int RUBBER_CURSOR_HOT_X = 16;
static const int RUBBER_CURSOR_HOT_Y = 16;

// rubber config
static const int RUBBER_WIDTH = 32;
static const int RUBBER_HEIGHT = 32;

// main panel config
static const int MAIN_PANEL_WIDTH = SCREEN_WIDTH;
static const int MAIN_PANEL_HEIGHT = 2 * UNIT_HEIGHT;

static const int MAIN_PANEL_START_X = 0;
static const int MAIN_PANEL_START_Y = 0;

// chat panel config
static const int CHAT_PANEL_WIDTH = 300;
static const int CHAT_PANEL_HEIGHT = SCREEN_HEIGHT - MAIN_PANEL_HEIGHT;

static const int CHAT_PANEL_START_X = SCREEN_WIDTH - CHAT_PANEL_WIDTH;
static const int CHAT_PANEL_START_Y = MAIN_PANEL_HEIGHT;

// text panel config
static const int TEXT_PANEL_WIDTH = 250;
static const int TEXT_PANEL_HEIGHT = 507;

static const int TEXT_PANEL_START_X = ((SCREEN_WIDTH + (SCREEN_WIDTH - CHAT_PANEL_WIDTH)) - TEXT_PANEL_WIDTH) / 2;
static const int TEXT_PANEL_START_Y = CHAT_PANEL_START_Y + 25;

// input panel config
static const int INPUT_PANEL_WIDTH = 250;
static const int INPUT_PANEL_HEIGHT = 40;

static const int INPUT_PANEL_START_X = ((SCREEN_WIDTH + (SCREEN_WIDTH - CHAT_PANEL_WIDTH)) - INPUT_PANEL_WIDTH) / 2;
static const int INPUT_PANEL_START_Y = (SCREEN_HEIGHT + (TEXT_PANEL_START_Y + TEXT_PANEL_HEIGHT) - INPUT_PANEL_HEIGHT) / 2;

// color button config
static const int COLOR_BUTTON_NUMBER = 21;

static const int COLOR_BUTTON_WIDTH = 20;
static const int COLOR_BUTTON_HEIGHT = 20;

static const int COLOR_BUTTON_START_X = 25;
static const int COLOR_BUTTON_START_Y = 25;

static const int COLOR_BUTTON_OFFSET = COLOR_BUTTON_WIDTH + 5;

// color panel
static const int COLOR_PANEL_WIDTH = (COLOR_BUTTON_NUMBER / 2) * COLOR_BUTTON_WIDTH + (COLOR_BUTTON_NUMBER / 2 + 1) * 5;
static const int COLOR_PANEL_HEIGHT = COLOR_BUTTON_NUMBER * 2 + 15;

static const int COLOR_PANEL_START_X = 20;
static const int COLOR_PANEL_START_Y = 20;

// current color panel
static const int CURRENT_COLOR_PANEL_X = COLOR_PANEL_WIDTH + COLOR_BUTTON_OFFSET;
static const int CURRENT_COLOR_PANEL_Y = COLOR_PANEL_START_Y;

static const int CURRENT_COLOR_PANEL_WIDTH = UNIT_WIDTH;
static const int CURRENT_COLOR_PANEL_HEIGHT = COLOR_PANEL_HEIGHT;

// current color button
static const int CURRENT_COLOR_BUTTON_X = CURRENT_COLOR_PANEL_X + 5;
static const int CURRENT_COLOR_BUTTON_Y = CURRENT_COLOR_PANEL_Y + 5;

static const int CURRENT_COLOR_BUTTON_WIDTH = CURRENT_COLOR_PANEL_WIDTH - 10;
static const int CURRENT_COLOR_BUTTON_HEIGHT = CURRENT_COLOR_PANEL_HEIGHT - 11;

// utility button config
static const int UTILITY_BUTTON_START_X = CURRENT_COLOR_BUTTON_X + CURRENT_COLOR_PANEL_WIDTH + 5;
static const int UTILITY_BUTTON_START_Y = CURRENT_COLOR_BUTTON_Y;

static const int UTILITY_BUTTON_WIDTH = COLOR_BUTTON_WIDTH;
static const int UTILITY_BUTTON_HEIGHT = COLOR_BUTTON_HEIGHT;

static const int UTILITY_BUTTON_HORIZONTAL_OFFSET = COLOR_BUTTON_OFFSET;
static const int UTILITY_BUTTON_VERTICAL_OFFSET = COLOR_BUTTON_OFFSET;

// utility panel
static const int UTILITY_PANEL_X = CURRENT_COLOR_PANEL_X + CURRENT_COLOR_PANEL_WIDTH + 5;
static const int UTILITY_PANEL_Y = CURRENT_COLOR_PANEL_Y;

static const int UTILITY_PANEL_WIDTH = UTILITY_BUTTON_WIDTH + 15;
static const int UTILITY_PANEL_HEIGHT = COLOR_PANEL_HEIGHT;

// brush button confit
static const int PANEL_BUTTON_WIDTH = 32;
static const int PANEL_BUTTON_HEIGHT = 32;

static const int BUTTON_START_X = COLOR_PANEL_WIDTH + 3 * UNIT_WIDTH;
static const int BUTTON_START_Y = 35;

static const int BUTTON_SAPWN_DIFF_X = 2 * UNIT_WIDTH;
static const int BUTTON_SAPWN_DIFF_Y = UNIT_HEIGHT;

// asset config
static const std::string MINUS_BUTTON_PATH = "Assets/Textures/minus_button.png";
static const std::string PLUS_BUTTON_PATH = "Assets/Textures/plus_button.png";

static const std::string PENCIL_BUTTON_PATH = "Assets/Textures/pencil.png";
static const std::string PENCIL_CURSOR_PATH = "Assets/Textures/pencil_cursor.png";

static const std::string RUBBER_BUTTON_PATH = "Assets/Textures/rubber.png";
static const std::string RUBBER_CURSOR_PATH = "Assets/Textures/rubber_cursor.png";

static const std::string TEXT_BUTTON_PATH = "Assets/Textures/text.png";

static const std::string FONT_PATH = "fonts/Roboto-Light.ttf";

// color picker config
static const int COLOR_PICKER_COLOR_PANEL_WIDTH = 400;
static const int COLOR_PICKER_COLOR_PANEL_HEIGHT = 150;

static const int COLOR_PICKER_COLOR_PANEL_START_X = 50;
static const int COLOR_PICKER_COLOR_PANEL_START_Y = 50;

// color picker interval config
static const int COLOR_INTERVAL_HORIZONTAL_OFFSET = 50;

static const int COLOR_INTERVAL_START_X = 50;
static const int COLOR_INTERVAL_START_Y = COLOR_PICKER_COLOR_PANEL_START_Y + COLOR_PICKER_COLOR_PANEL_HEIGHT + COLOR_INTERVAL_HORIZONTAL_OFFSET;

static const int COLOR_INTERVAL_END_X = COLOR_INTERVAL_START_X + COLOR_PICKER_COLOR_PANEL_WIDTH;
static const int COLOR_INTERVAL_END_Y = COLOR_INTERVAL_START_Y;

static const int COLOR_INTERVAL_WIDTH = COLOR_INTERVAL_END_X - COLOR_INTERVAL_START_X;
static const int COLOR_RGB_MAX = 255;

// toogle button config
static const int TOGGLE_BUTTON_WIDTH = 5;
static const int TOGGLE_BUTTON_HEIGHT = 10;

// red toggle button config
static const int RED_TOGGLE_BUTTON_START_X = COLOR_INTERVAL_START_X;
static const int RED_TOGGLE_BUTTON_START_Y = COLOR_INTERVAL_START_Y;

// green toggle button config
static const int GREEN_TOGGLE_BUTTON_START_X = COLOR_INTERVAL_START_X;
static const int GREEN_TOGGLE_BUTTON_START_Y = COLOR_INTERVAL_START_Y + COLOR_INTERVAL_HORIZONTAL_OFFSET;

// blue toggle button config
static const int BLUE_TOGGLE_BUTTON_START_X = COLOR_INTERVAL_START_X;
static const int BLUE_TOGGLE_BUTTON_START_Y = COLOR_INTERVAL_START_Y + 2 * COLOR_INTERVAL_HORIZONTAL_OFFSET;

// label config
static const int LABEL_PADDING = 20;
static const int LABEL_OFFSET = 50;

static const int LABEL_START_X = (COLOR_INTERVAL_START_X + COLOR_INTERVAL_END_X) / 2;
static const int LABEL_START_Y = COLOR_INTERVAL_START_Y - LABEL_PADDING;

// submit button config
static const std::string SUBMIT_BUTTON = "Submit";

static const int SUBMIT_BUTTON_WIDTH = 100 + 10;
static const int SUBMIT_BUTTON_HEIGHT = 40 + 10;

static const int SUBMIT_BUTTON_START_X = (COLOR_PICKER_WIDTH - SUBMIT_BUTTON_WIDTH) / 2 - 5;
static const int SUBMIT_BUTTON_START_Y = COLOR_PICKER_HEIGHT - SUBMIT_BUTTON_HEIGHT - 20;

// brush label config
static const int BRUSH_LABEL_X = BUTTON_START_X + PANEL_BUTTON_WIDTH + UNIT_WIDTH / 2 + 2;
static const int BRUSH_LABEL_Y = BUTTON_START_Y - 2;

// brush config
static const int BRUSH_INCREMENT = 1;

// messagePanel config
static const int MESSAGE_PANEL_WIDTH = 500;
static const int MESSAGE_PANEL_HEIGHT = 300;

static const int MESSAGE_PANEL_START_X = 50;
static const int MESSAGE_PANEL_START_Y = 50;

static const std::string MESSAGE_PANEL = "Notification!";

// textPanel config
static const std::string TEXTING_PANEL = "Hello!";

static const int TEXTING_PANEL_WIDTH = 500;
static const int TEXTING_PANEL_HEIGHT = 300;