#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "lgfx/v1_autodetect/LGFX_AutoDetect_sdl.hpp"
#include "user_code.hpp"

LGFX lcd ( 320, 240 );

int32_t target_x = 160 * 256;
int32_t target_y = 120 * 256;
int32_t current_x = 0;
int32_t current_y = 0;
int32_t add_x = 0;
int32_t add_y = 0;

void setup()
{
  lcd.init();
}

void loop()
{
  static int i;
  ++i;
  lcd.fillCircle(current_x >> 8, current_y >> 8, 5, i);
  current_x += add_x;
  current_y += add_y;
  add_x += (current_x < target_x) ? 1 : -1;
  add_y += (current_y < target_y) ? 1 : -1;
  lgfx::touch_point_t new_tp;
  if (lcd.getTouch(&new_tp))
  {
	target_x = new_tp.x * 256;
	target_y = new_tp.y * 256;
	lcd.drawCircle(new_tp.x, new_tp.y, 5, TFT_WHITE);
  }
  lgfx::delay(1);
}