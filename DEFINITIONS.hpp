#pragma once
#include <string>

constexpr int WINDOW_WIDTH{ 852 };
constexpr int WINDOW_HEIGHT{ 480 };

constexpr int FINAL_NUMBER_OF_SHADOWS{ 20 };

constexpr int IN_GAME_FONT_SIZE{ 12 };
constexpr int MENU_FONT_SIZE{ 18 };
constexpr int SCORE_TABLE_FONT_SIZE{ 14 };

constexpr unsigned LVL1_ENEMIES_COUNT{ 52 };
constexpr unsigned LVL1_POINTS_FOR_ENEMY{ 30 };
constexpr unsigned LVL1_ENEMIES_CHARGING_AT_ONCE{ 3 };
constexpr unsigned LVL1_POINTS{ 14'400 };
constexpr float LVL1_ENEMIES_MOVEMENT_SPEED{ 1.2f };
constexpr float LVL1_ENEMIES_MOVE_FORWARD_TIME{ 10.f };
constexpr float LVL1_ENEMIES_MOVE_FORWARD_DURATION{ 0.8f };
constexpr float LVL1_ENEMIES_BULLETS_SPEED{ 4.f };
constexpr float LVL1_ENEMIES_CHANCE_TO_SHOOT{ 50.f };
constexpr float LVL1_ENEMIES_START_SHOOT_DELAY{ 5.f };
constexpr float LVL1_ENEMIES_CHARGING_SPEED{ 2.2f };
constexpr float LVL1_SPACE_BETWEEN_ENEMIES{ 10.f };

constexpr unsigned LVL2_ENEMIES_COUNT{ 40 };
constexpr unsigned LVL2_POINTS_FOR_ENEMY{ 45 };
constexpr unsigned LVL2_ENEMIES_CHARGING_AT_ONCE{ 4 };
constexpr unsigned LVL2_POINTS{ 20'400 };
constexpr float LVL2_ENEMIES_MOVEMENT_SPEED{ 1.0f };
constexpr float LVL2_ENEMIES_BULLETS_SPEED{ 5.f };
constexpr float LVL2_ENEMIES_CHANCE_TO_SHOOT{ 75.f };
constexpr float LVL2_ENEMIES_CHARGING_SPEED{ 2.5f };
constexpr float LVL2_SPACE_BETWEEN_ENEMIES{ 25.f };

constexpr unsigned MAX_POINTS{ LVL1_ENEMIES_COUNT*LVL1_POINTS_FOR_ENEMY + LVL1_POINTS + LVL2_ENEMIES_COUNT*LVL2_POINTS_FOR_ENEMY + LVL2_POINTS };
constexpr int SCORE_TABLE_MAX_SIZE{ 10 };

constexpr float SHADOW_FRAME_TIME{ 0.05f };
constexpr float BUTTON_ANIMATION_FRAME_TIME{ 0.7f };
constexpr float BACKGROUND_ANIMATION_FRAME_TIME{ 0.5f };
constexpr float ENEMY_ANIMATION_FRAME_TIME{ 0.15f };
constexpr float BOOM_ANIMATION_FRAME_TIME{ 0.1f };

constexpr float ENEMIES_SHOT_DELAY{ 0.8f };
constexpr float ENEMIES_WIDTH{ 32.f };
constexpr float ENEMIES_HEIGHT{ 32.f };
constexpr float ENEMIES_TOP_MARGIN{ 50.f };
constexpr float ENEMIES_SIDE_MARGIN{ 150.f };

constexpr unsigned PLAYER_LIVES{ 3 };
constexpr float PLAYER_START_SPEED{ 2.5f };
constexpr float PLAYER_START_BULLET_SPEED{ 5.f };
constexpr float PLAYER_BLURRING_FRAME_TIME{ 0.2f };
constexpr int PLAYER_BLURRING_FRAMES{ 15 };
constexpr int PLAYER_NICK_MAX_LENGTH{ 12 };

const std::string SPLASH_STATE_BACKGROUND_FILEPATH{ "Resources/splash_screen_background.png" };

const std::string START_BUTTON_FILEPATH{ "Resources/start_button.png" };
const std::string START_BUTTON_PRESSED_FILEPATH{ "Resources/start_button_pressed.png" };

const std::string BLACK_SHADOW_FILEPATH{ "Resources/black10.png" }; 

const std::string PIXEL_FONT_FILEPATH{ "Resources/pixel_font.ttf" };

const std::string LEVEL_BACKGROUND1_FILEPATH{ "Resources/Level_background1.png" };
const std::string LEVEL_BACKGROUND2_FILEPATH{ "Resources/Level_background2.png" };
const std::string LEVEL_BACKGROUND3_FILEPATH{ "Resources/Level_background3.png" };

const std::string PLAYER_SHIP_FILEPATH{ "Resources/player_ship.png" };
const std::string HIT_PLAYER_SHIP_FILEPATH{ "Resources/hit_player_ship.png" };
const std::string PLAYER_BULLET_FILEPATH{ "Resources/player_bullet.png" };
const std::string ENEMY_BULLET_FILEPATH{ "Resources/enemy_bullet.png" };

const std::string RED_ENEMY1_FILEPATH{ "Resources/red_enemy1.png" };
const std::string RED_ENEMY2_FILEPATH{ "Resources/red_enemy2.png" };
const std::string RED_ENEMY3_FILEPATH{ "Resources/red_enemy3.png" };

const std::string ENEMY_DST1_FILEPATH{ "Resources/enemy_destroyed1.png" };
const std::string ENEMY_DST2_FILEPATH{ "Resources/enemy_destroyed2.png" };
const std::string ENEMY_DST3_FILEPATH{ "Resources/enemy_destroyed3.png" };
const std::string ENEMY_DST4_FILEPATH{ "Resources/enemy_destroyed4.png" };

const std::string LIVES_POWERUP1_FILEPATH{ "Resources/lives_powerup1.png" };
const std::string LIVES_POWERUP2_FILEPATH{ "Resources/lives_powerup2.png" };
const std::string LIVES_POWERUP3_FILEPATH{ "Resources/lives_powerup3.png" };

const std::string SPEED_POWERUP1_FILEPATH{ "Resources/speed_powerup1.png" };
const std::string SPEED_POWERUP2_FILEPATH{ "Resources/speed_powerup2.png" };
const std::string SPEED_POWERUP3_FILEPATH{ "Resources/speed_powerup3.png" };

const std::string BULLETS_POWERUP1_FILEPATH{ "Resources/bullets_powerup1.png" };
const std::string BULLETS_POWERUP2_FILEPATH{ "Resources/bullets_powerup2.png" };
const std::string BULLETS_POWERUP3_FILEPATH{ "Resources/bullets_powerup3.png" };

const std::string SCORE_STATE_BACKGROUND{ "Resources/score_state_background.png" };
const std::string SCORES_FILEPATH{ "User_data/high_score.txt" };
