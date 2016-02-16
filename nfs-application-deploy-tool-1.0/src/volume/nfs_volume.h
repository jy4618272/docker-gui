#ifndef _NFS_VOLUME_H_
#define _NFS_VOLUME_H_

#include "../global/global.h"

void volume_event(GtkWidget *widget, GtkWidget *window);
void volume_quit(GtkWidget *volume_window, GtkWidget *main_window);

void data_volume_create(GtkWidget *widget, GtkWidget *window);
void combo_box_add_image(GtkBuilder *builder);
void data_volume_create_cancel(GtkWidget *widget, GtkBuilder *builder);
void data_volume_create_ok(GtkWidget *widget, GtkBuilder *builder);
gchar *data_volume_create_get_data_name (gchar *str, const gchar *msg);
gint  data_volume_create_get_entry_text (GtkBuilder *builder, gchar *str);
void data_volume_create_response(GtkWidget *dialog, GtkBuilder *builder);

void container_look(GtkWidget *widget, GtkWidget *window1);
void data_volume_look(GtkWidget *widget, GtkWidget *window1);
void data_volume_look_add_column(GtkBuilder *builder);
void data_volume_look_add_data(GtkBuilder *builder, gint flag);
void data_volume_look_del(GtkWidget *widget, GtkBuilder *builder);
void data_volume_look_open(GtkWidget *widget, GtkBuilder *builder);
void select_all(GtkButton *button, GtkBuilder *builder);
void item_toggled (GtkCellRendererToggle *cell,gchar *path_str, gpointer data);

void container_create(GtkWidget *widget, GtkWidget *window);
void container_create_rebuild(GtkWidget *button, GtkWidget *window);
void container_create_ok(GtkWidget *button, GtkBuilder *builder);
gint  container_create_get_text (GtkBuilder *builder, gchar *str);
gint container_create_get_data (GtkBuilder *builder, gchar *str);
gboolean create_event(GtkBuilder *builder, gchar *str);

void data_volume_backup(GtkWidget *widget, GtkWidget *window);
void data_volume_backup_add_text_column(GtkTreeView *view);
void data_volume_backup_add_toggle_column(GtkTreeView *view, GtkCellRenderer *renderer);
void data_volume_backup_ok(GtkWidget *widget, GtkBuilder *builder);
gint data_volume_backup_get_data(GtkBuilder *builder, gchar *str, gchar *data_name);
void data_volume_backup_pick_up_data(gchar *str, gchar *data, gint flag);
gint data_volume_backup_get_image(GtkBuilder *builder, gchar *str);
gint data_volume_backup_get_name(GtkBuilder *builder, gchar *str);
void data_volume_backup_set_label(GtkBuilder *builder, gchar *data_name);
gint data_volume_backup_check_name(GtkBuilder *builder, const gchar *str);
void data_volume_backup_write_info_to_txt(GtkBuilder *builder, gchar *str, gchar *path);
void data_volume_backup_get_label(GtkCellRendererToggle *cell,gchar *path_str, GtkBuilder *builder);
void data_volume_backup_select_all(GtkWidget *widget, GtkBuilder *builder);
void data_volume_backup_button1_clicked(GtkWidget *widget, GtkLabel *label);
void data_volume_backup_button2_clicked(GtkWidget *widget, GtkLabel *label);

void data_volume_recovery(GtkWidget *widget, GtkWidget *window);
void data_volume_recovery_add_text_column(GtkTreeView *view);
void data_volume_recovery_add_toggle_column(GtkTreeView *view, GtkCellRenderer *renderer);
void data_volume_recovery_add_data(GtkBuilder *builder, gboolean flag);
void data_volume_recovery_renderer_get_label(GtkCellRendererToggle *cell,gchar *path_str, GtkBuilder *builder);
void data_volume_recovery_get_data(GtkBuilder *builder);
void data_volume_recovery_select_all(GtkButton *button, GtkBuilder *builder);
void data_volume_recovery_ok(GtkWidget *widget, GtkBuilder *builder);
gint  data_volume_recovery_get_container_name(GtkBuilder *builder, gchar *str);
gint data_volume_recovery_get_label(GtkBuilder *builder, gchar *str, gint flag);
void data_volume_recovery_get_tar_name(GtkBuilder *builder, gchar  *tar_name, gchar *num);
gboolean data_volume_recovery_data(gchar *str, gchar *tar_name, gchar *image);
gboolean data_volume_recovery_create_container(gchar *str);

void data_volume_transfer(GtkWidget *widget, GtkWidget *window);
void data_volume_transfer_ok(GtkWidget *widget, GtkBuilder *builder);
gboolean data_volume_transfer_create_directory(gchar *dir_name, gchar *tar_name);
gint data_volume_transfer_get_container_dir_name(GtkBuilder *builder, gchar *str);
gint data_volume_transfer_get_local_dir_name(GtkBuilder *builder, gchar *str);
void data_volume_transfer_button1_clicked(GtkWidget *widget, GtkBuilder *builder);
void data_volume_transfer_button2_clicked(GtkWidget *widget, GtkBuilder *builder);
void data_volume_transfer_button3_clicked(GtkWidget *widget, GtkBuilder *builder);
void data_volume_transfer_button3_response_ok(GtkBuilder *builder);
void strrev(char *s);

void data_volume_look_all(GtkWidget *widget, GtkWidget *window);

#endif


