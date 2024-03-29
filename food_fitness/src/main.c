#include <pebble.h>

#include "windows/radio_button_exercise_window.h"
#include "windows/pin_window.h"
#include "windows/dialog_config_window.h"
#include "windows/radio_button_food_window.h"
#include "windows/radio_button_class_window.h"
#include "windows/checkbox_window.h"

#define NUM_WINDOWS 10

static Window *s_main_window;
static MenuLayer *s_menu_layer;

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return NUM_WINDOWS;
}

static void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "Exercise", NULL, NULL);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "Food", NULL, NULL);
     break;
    case 2:
          menu_cell_basic_draw(ctx, cell_layer, "Class", NULL, NULL);
      break;
    default:
      break;
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return CHECKBOX_WINDOW_CELL_HEIGHT;
}

static void pin_complete_callback(PIN pin, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Pin was %d %d %d", pin.digits[0], pin.digits[1], pin.digits[2]);
  pin_window_pop((PinWindow*)context, true);
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      radio_button_window_exercise_push();
      break;
    case 1:
      radio_button_window_food_push();
      break;
    case 2:
          radio_button_class_window_push();
        break;
    default:
      break;
  }
}

static void draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *context) {
  menu_cell_basic_header_draw(ctx, cell_layer, "Wellbeing");
}

static int16_t get_header_height_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static uint16_t get_num_sections_callback(struct MenuLayer *menu_layer, void *context) {
  return 1;
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)get_num_rows_callback,
      .draw_row = (MenuLayerDrawRowCallback)draw_row_callback,
      .get_cell_height = (MenuLayerGetCellHeightCallback)get_cell_height_callback,
      .select_click = (MenuLayerSelectCallback)select_callback,
      .draw_header = (MenuLayerDrawHeaderCallback)draw_header_callback,
      .get_header_height = (MenuLayerGetHeaderHeightCallback)get_header_height_callback,
      .get_num_sections = (MenuLayerGetNumberOfSectionsCallback)get_num_sections_callback,
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
