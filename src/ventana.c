#include "ventana.h"

Window *esfera;
TextLayer *capa_hora, *capa_dia, *capa_data;

static Layer *capa_bateria;
static GFont fonte_hora, fonte_dia, fonte_data;
static bool mostrar_dia, mostrar_data, mostrar_carga, invertir_cores;

extern int porcentaxe_bateria;
static void actualizar_bateria(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  int width = (int)(float)(((float)porcentaxe_bateria * 144.0F) / 100.0F);
  if (invertir_cores) {
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);
    graphics_context_set_fill_color(ctx, GColorBlack);
  } else {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);
    graphics_context_set_fill_color(ctx, GColorWhite);
  }
  graphics_fill_rect(ctx, GRect(0, 0, width, bounds.size.h), 0, GCornerNone); 
}

static void saida(Window *window) {
  DEBUG("  [SAIDA] -> deinicializando recursos da interface");
  DEBUG("capas:           bateria[%p]", capa_bateria);
  layer_destroy(capa_bateria);
  DEBUG("capas de texto:     hora[%p]  data[%p]  dia[%p]", capa_hora, capa_data, capa_dia);
  text_layer_destroy(capa_dia);
  text_layer_destroy(capa_data);
  text_layer_destroy(capa_hora);
  DEBUG("pechando fontes:    hora[%p]  data[%p]  dia[%p]", 
         fonte_hora, fonte_data, fonte_dia);
  fonts_unload_custom_font(fonte_dia);
  fonts_unload_custom_font(fonte_data);
  fonts_unload_custom_font(fonte_hora);
  DEBUG("pechando ventana: esfera[%p]", esfera);
  window_destroy(esfera);
  DEBUG("  [SAIDA] -> deinicializacion OK!");
}

static void entrada(Window *window) {
  DEBUG("[ENTRADA] -> iniciando recursos da interface");
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  DEBUG("posicionando capas na ventana");
  // inicio da posición dinámica de capas ----->
  if ((!mostrar_dia) && (!mostrar_data)) {
    capa_hora = text_layer_create(GRect(0, 48, bounds.size.w, 70));
  } else {
    capa_hora = text_layer_create(GRect(0, 60, bounds.size.w, 70));    
  }
  text_layer_set_text_alignment(capa_hora, GTextAlignmentCenter); 

  if (mostrar_data) {
    capa_dia = text_layer_create(GRect(0, 12, 138, 34));
    text_layer_set_text_alignment(capa_dia, GTextAlignmentRight);  
  } else {
    capa_dia = text_layer_create(GRect(0, 36, bounds.size.w, 34));
    text_layer_set_text_alignment(capa_dia, GTextAlignmentCenter);   
  }

  if (mostrar_dia) {
    capa_data = text_layer_create(GRect(0, 40, 138, 28));    
    text_layer_set_text_alignment(capa_data, GTextAlignmentRight);
  } else {
    capa_data = text_layer_create(GRect(0, 44, bounds.size.w, 28));
    text_layer_set_text_alignment(capa_data, GTextAlignmentCenter);
  } // <---------------------- fin da posición dinámica de capas

  DEBUG("aplicando atributos das capas");
  // --- atributos das capas ---------------------------------
  fonte_hora = font_load(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_60);
  text_layer_set_font(capa_hora, fonte_hora);
  layer_add_child(window_layer, text_layer_get_layer(capa_hora));

  fonte_data = font_load(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_21);
  text_layer_set_font(capa_data, fonte_data); 
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(capa_data));  
  
  fonte_dia  = font_load(RESOURCE_ID_ROBOTO_BOLD_26);
  text_layer_set_font(capa_dia, fonte_dia); 
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(capa_dia));
  
  capa_bateria = layer_create(GRect(0, (bounds.size.h - 4), bounds.size.w, 4));
  layer_set_update_proc(capa_bateria, actualizar_bateria);
  layer_add_child(window_get_root_layer(window), capa_bateria);
  layer_mark_dirty(capa_bateria);


  if (invertir_cores) {
    DEBUG("invertindo cores da interface");
    text_layer_set_text_color(capa_hora, GColorBlack);
    text_layer_set_text_color(capa_dia, GColorBlack);
    text_layer_set_text_color(capa_data, GColorBlack);  
    text_layer_set_background_color(capa_hora, GColorWhite);
    text_layer_set_background_color(capa_dia, GColorWhite);
    text_layer_set_background_color(capa_data, GColorWhite);
  } else {
    DEBUG("empregando cores primarias");
    text_layer_set_text_color(capa_hora, GColorWhite);
    text_layer_set_text_color(capa_dia, GColorWhite);
    text_layer_set_text_color(capa_data, GColorWhite);
    text_layer_set_background_color(capa_hora, GColorBlack);
    text_layer_set_background_color(capa_dia, GColorBlack);
    text_layer_set_background_color(capa_data, GColorBlack);
  } // ----------------- fin dos atributos das capas

  LOG("aplicando configuración do reloxo");
  // mostramos as capas en base á configuración actual:
  if (mostrar_data) {
    layer_set_hidden(text_layer_get_layer(capa_data), false);
  } else {
    layer_set_hidden(text_layer_get_layer(capa_data), true);
  }
  
  if (mostrar_dia) {
    layer_set_hidden(text_layer_get_layer(capa_dia), false);
  } else {
    layer_set_hidden(text_layer_get_layer(capa_dia), true);
  }
  
  if (mostrar_carga) {
    layer_set_hidden(capa_bateria, false);
  } else {
    layer_set_hidden(capa_bateria, true);
  }
}

void iniciar_esfera() {
  esfera = window_create();
  // configuracion permanente
  DEBUG("-------------------------------");
  DEBUG("obtendo configuración do reloxo");
  if(!persist_exists(1024)) {
    DEBUG("configuración non atopada -> PRIMEIRO ARRANQUE");
    // se non existe gardamos os valores por defecto:
    persist_write_bool(1024, true); 
    persist_write_bool(DIA, true);
    persist_write_bool(DATA, true);
    persist_write_bool(CARGA, true);
    persist_write_bool(INVERTIR, false);
  }
  // obtemos a configuracion actual:
  mostrar_dia = persist_read_bool(DIA);
  mostrar_data = persist_read_bool(DATA);
  mostrar_carga = persist_read_bool(CARGA);
  invertir_cores = persist_read_bool(INVERTIR);

  DEBUG("Novos axustes --> dia: %d, data: %d, carga: %d, invertir: %d", mostrar_dia, mostrar_data,
        mostrar_carga, invertir_cores);

  if (invertir_cores) { 
    window_set_background_color(esfera, GColorWhite); 
  } else { 
    window_set_background_color(esfera, GColorBlack); 
  }
  window_set_window_handlers(esfera, (WindowHandlers) { .load = entrada, .unload = saida });
  window_stack_push(esfera, true);
  iniciar_eventos();
}