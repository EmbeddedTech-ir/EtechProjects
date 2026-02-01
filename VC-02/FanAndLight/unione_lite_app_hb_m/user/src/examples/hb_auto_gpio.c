#include "user_config.h"

#include "user_event.h"

#include "user_gpio.h"

#include "user_player.h"

#include "user_pwm.h"

#include "user_timer.h"

#include "user_uart.h"

#define TAG "auto_gpio"

#define UART_SEND_MAX 16

typedef struct {
  char data[UART_SEND_MAX];
  int len;
}
uart_data_t;

const uart_data_t g_uart_buf[] = {};

int current_fan_speed = 0;

static void _custom_setting_cb(USER_EVENT_TYPE event,
  user_event_context_t * context) {
  event_custom_setting_t * setting = NULL;
  if (context) {
    setting = & context -> custom_setting;
    LOGT(TAG, "user command: %s", setting -> cmd);


    if (0 == uni_strcmp(setting -> cmd, "FanOn")) {
      current_fan_speed = 100;
      user_pwm_change_duty(PWM_NUM_1_B2, current_fan_speed);

    }

    if (0 == uni_strcmp(setting -> cmd, "FanOff")) {
      current_fan_speed = 0;
      user_pwm_change_duty(PWM_NUM_1_B2, current_fan_speed);


    }

    if (0 == uni_strcmp(setting -> cmd, "DecreaseFan")) {
      current_fan_speed -= 30;
      if(current_fan_speed<0){
        current_fan_speed=0;
      }
      user_pwm_change_duty(PWM_NUM_1_B2, current_fan_speed);

    }
    if (0 == uni_strcmp(setting -> cmd, "IncreaseFan")) {
      current_fan_speed += 30;
      if(current_fan_speed>100){
        current_fan_speed=100;
      }
      user_pwm_change_duty(PWM_NUM_1_B2, current_fan_speed);

    }

    if (0 == uni_strcmp(setting -> cmd, "TurnOn")) {
      user_gpio_set_value(GPIO_NUM_B3, 1);
    }
    if (0 == uni_strcmp(setting -> cmd, "OffLight")) {
      user_gpio_set_value(GPIO_NUM_B3, 0);
    }

    user_player_reply_list_random(setting -> reply_files);
  }
}

static void _register_event_callback(void) {
  user_event_subscribe_event(USER_CUSTOM_SETTING, _custom_setting_cb);
}

int hb_auto_gpio(void) {
  user_gpio_init();
  user_gpio_set_mode(GPIO_NUM_A25, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A25, 0);
  user_gpio_set_mode(GPIO_NUM_A26, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A26, 0);
  user_gpio_set_mode(GPIO_NUM_A27, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A27, 0);
  user_gpio_set_mode(GPIO_NUM_A28, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_A28, 0);
  user_gpio_set_mode(GPIO_NUM_B6, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_B6, 0);
  user_gpio_set_mode(GPIO_NUM_B7, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_B7, 0);
  user_pwm_init(PWM_NUM_1_B2, 22000, true);
  user_pwm_start(PWM_NUM_1_B2, 0);
  user_gpio_set_mode(GPIO_NUM_B3, GPIO_MODE_OUT);
  user_gpio_set_value(GPIO_NUM_B3, 0);
  _register_event_callback();
  return 0;
}