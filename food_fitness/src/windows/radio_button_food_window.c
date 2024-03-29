/**
 * Example implementation of the radio button list UI pattern.
 */

#include "radio_button_food_window.h"

static Window *s_main_window;
static MenuLayer *s_menu_layer;
DictionaryIterator *iterator;


static int s_current_selection = 0;

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return RADIO_BUTTON_WINDOW_NUM_ROWS + 1;
}

static void sendMessage(int x)
{
    app_message_open(APP_MESSAGE_INBOX_SIZE_MINIMUM,APP_MESSAGE_OUTBOX_SIZE_MINIMUM);
    app_message_outbox_begin(&iterator);
    int key = 78;
    int value = x;
    dict_write_int(iterator, key, &value, sizeof(int), true);
    const uint32_t final_size = dict_write_end(iterator);
    app_message_outbox_send();
}


static void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *context) {
  if(cell_index->row == RADIO_BUTTON_WINDOW_NUM_ROWS) {
    // This is the submit item
    menu_cell_basic_draw(ctx, cell_layer, "Submit", NULL, NULL);
  } else {
    // This is a choice item
    
    static char s_buff[16];
    switch(cell_index->row) {
    case 0:
      snprintf(s_buff, sizeof(s_buff), "No meals");
      break;
    case 1:
      snprintf(s_buff, sizeof(s_buff), "1 meal");
      break;
    case 2:
      snprintf(s_buff, sizeof(s_buff), "2 meals");
      break;
    case 3:
      snprintf(s_buff, sizeof(s_buff), "3 meals");
      break;
    default:
      break;
  }
    
    menu_cell_basic_draw(ctx, cell_layer, s_buff, NULL, NULL);

    GRect bounds = layer_get_bounds(cell_layer);
    GPoint p = GPoint(bounds.size.w - (3 * RADIO_BUTTON_WINDOW_RADIO_RADIUS), (bounds.size.h / 2));

    // Selected?
    if(menu_cell_layer_is_highlighted(cell_layer)) {
      graphics_context_set_stroke_color(ctx, GColorWhite);
      graphics_context_set_fill_color(ctx, GColorWhite);
    } else {
      graphics_context_set_fill_color(ctx, GColorBlack);
    }

    // Draw radio filled/empty
    graphics_draw_circle(ctx, p, RADIO_BUTTON_WINDOW_RADIO_RADIUS);
    if(cell_index->row == s_current_selection) {
      // This is the selection
      graphics_fill_circle(ctx, p, RADIO_BUTTON_WINDOW_RADIO_RADIUS - 3);
    }
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  return RADIO_BUTTON_WINDOW_CELL_HEIGHT;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  if(cell_index->row == RADIO_BUTTON_WINDOW_NUM_ROWS) {
    // Do something with user choice
      sendMessage(s_current_selection);
  } else {
    // Change selection
    s_current_selection = cell_index->row;
    menu_layer_reload_data(menu_layer);
  }
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
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);

  window_destroy(window);
  s_main_window = NULL;
}

void radio_button_window_food_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}

