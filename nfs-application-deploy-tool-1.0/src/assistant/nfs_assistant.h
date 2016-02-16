#ifndef _ASSISTANT_H
#define _ASSISTANT_H

#include "../global/global.h"

void creat_page1(GtkAssistant *assistant);
void creat_page2(GtkAssistant *assistant);
void creat_page3(GtkAssistant *assistant);
void creat_page4(GtkAssistant *assistant);
void on_button_change(GtkWidget *widget, gpointer data);
void on_assistant_close_cancel (GtkWidget *widget, gpointer data);
gint on_assistant_apply_changes(GtkWidget *widget, gpointer data);
void on_assistant_apply (GtkWidget *widget, gpointer data);
void on_assistant_prepare (GtkWidget *widget, GtkWidget *page, gpointer data);
void docker_start();
void terminal_create(GtkWidget *window);

#endif
