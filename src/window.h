static void load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  day_layer = text_layer_create(GRect(0, 12, 138, 34));
  date_layer = text_layer_create(GRect(0, 38, 138, 28));
  time_layer = text_layer_create(GRect(0, 64, bounds.size.w, 70));
  battery_layer = layer_create(GRect(0, (bounds.size.h - 4), bounds.size.w, 4));

  day_font  = font_load(RESOURCE_ID_ROBOTO_BOLD_26);
  date_font = font_load(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_21);
  time_font = font_load(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_60);
  text_layer_set_font(day_layer, day_font);  
  text_layer_set_font(date_layer, date_font);
  text_layer_set_font(time_layer, time_font);    
  text_layer_set_text_alignment(day_layer, GTextAlignmentRight);
  text_layer_set_text_alignment(date_layer, GTextAlignmentRight);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

  GColor8 negro = GColorBlack;
  GColor8 branco = GColorWhite;

  if (color == 0) {
    text_layer_set_text_color(time_layer, branco);
    text_layer_set_text_color(day_layer, branco);
    text_layer_set_text_color(date_layer, branco);
    text_layer_set_background_color(time_layer, negro);
    text_layer_set_background_color(day_layer, negro);
    text_layer_set_background_color(date_layer, negro);
  } else {
    text_layer_set_text_color(time_layer, GColorBlack);
    text_layer_set_text_color(day_layer, GColorBlack);
    text_layer_set_text_color(date_layer, GColorBlack);  
    text_layer_set_background_color(time_layer, GColorWhite);
    text_layer_set_background_color(day_layer, GColorWhite);
    text_layer_set_background_color(date_layer, GColorWhite);
  } 

  layer_set_update_proc(battery_layer, battery_update_proc);
  layer_add_child(window_layer, text_layer_get_layer(time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  layer_add_child(window_get_root_layer(window), battery_layer);
  layer_mark_dirty(battery_layer);
}

static void unload(Window *window) {
  text_layer_destroy(time_layer); fonts_unload_custom_font(time_font);
  text_layer_destroy(date_layer); fonts_unload_custom_font(date_font);
  text_layer_destroy(day_layer); fonts_unload_custom_font(day_font);
  window_destroy(watchface);
}