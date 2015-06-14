#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_first_city_name;
static TextLayer *s_second_city_name;
static TextLayer *s_second_time_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void update_second_time() {
  // Get a tm structure
  time_t temp = time(NULL) + 25200; // hardcoded timezone difference. To be changed.
  struct tm *second_tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char second_buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(second_buffer, sizeof("00:00"), "%H:%M", second_tick_time);
  } else {
    //Use 12 hour format
    strftime(second_buffer, sizeof("00:00"), "%I:%M", second_tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_second_time_layer, second_buffer);
}

static void main_window_load(Window *window) {
  // Get the root layer
  Layer *window_layer = window_get_root_layer(window);
  // Get the bounds of the window for sizing the text layer
  GRect bounds = layer_get_bounds(window_layer);
  
  // Create first city TextLayer
  s_first_city_name = text_layer_create(GRect(0, 2, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(s_first_city_name, GColorClear);
  text_layer_set_text_color(s_first_city_name, GColorBlack);
  text_layer_set_text(s_first_city_name, "Nice");
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_first_city_name, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_first_city_name, GTextAlignmentCenter);
  
  // Create first city time TextLayer
  s_time_layer = text_layer_create(GRect(0, 24, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Create second city TextLayer
  s_second_city_name = text_layer_create(GRect(0, 80, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(s_second_city_name, GColorBlack);
  text_layer_set_text_color(s_second_city_name, GColorClear);
  text_layer_set_text(s_second_city_name, "Tokyo");
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_second_city_name, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_second_city_name, GTextAlignmentCenter);
  
    // Create second city time TextLayer
  s_second_time_layer = text_layer_create(GRect(0, 109, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(s_second_time_layer, GColorBlack);
  text_layer_set_text_color(s_second_time_layer, GColorClear);
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_second_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_second_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_first_city_name));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_second_city_name));
  layer_add_child(window_layer, text_layer_get_layer(s_second_time_layer));
 
  // Make sure the time is displayed from the start
  update_time();
  update_second_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_first_city_name);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_second_city_name);
  text_layer_destroy(s_second_time_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  update_second_time();
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
