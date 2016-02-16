#ifndef _NFS_IMAGE_H_
#define _NFS_IMAGE_H_

#include "../global/global.h"

typedef struct
{
	GtkLabel *label;
	GtkTreeSelection *selection;
	GtkEntry *entry;
	GtkBuilder *builder;
	gchar *tags;
}Labels;

void image_event(GtkWidget *widget, GtkWidget *window);
void image_window_destroy(GtkWidget *window, GtkWidget *window1);

void message_dialog_new(GtkWindow *parrent, GtkMessageType type,  
                          GtkButtonsType buttons, const gchar *message_format);
void tree_column_new(GtkTreeView *view, gint column, const gchar* column_name);
gchar *popen_and_fread(gchar *command);

void image_pull_event(GtkWidget *button, GtkWidget *window);	
void pull_treeview_set(GtkTreeView *view,GtkBuilder *builder);
void attach_widgets (GtkBuilder *builder, gint current_page, gint total_page);
void pull_get_lines();
void pull_switch_to_nextpage(GtkButton *button, GtkBuilder *builder);
void pull_switch_to_prevpage(GtkButton *button, GtkBuilder *builder);
void pull_switch_to_homepage(GtkButton *button, GtkBuilder *builder);
void pull_treeview_get_search_image(GtkTreeView *view, GtkBuilder *builder);
void pull_set_page(GtkListStore *list, GtkTreeIter iter, gint x);
void pull_ok_clicked_create_thread(GtkWidget *button, GtkBuilder *builder);
void pull_ok_clicked(gpointer data);
void pull_cancel_clicked(GtkWidget *button, GtkWidget *window);
void pull_search_clicked(GtkWidget *button, GtkBuilder *builder);
gboolean message_dialog_create(gpointer data);
gboolean set_window_false(gpointer data);
gboolean set_window_true(gpointer data);

void image_image_event(GtkWidget *button, GtkWidget *window);
void image_treeview_set(GtkBuilder *builder);
void image_return(GtkButton *button, GtkWidget *window);

void image_rmi_event(GtkWidget *button, GtkWidget *window);
void rmi_treeview_set(GtkBuilder *builder);
void rmi_label_set(GtkTreeView *treeview, GtkBuilder *builder);
void rmi_ok_clicked(GtkWidget *widget, GtkBuilder *builder);
void rmi_cancel_clicked(GtkWidget *widget, GtkBuilder *builder);

void image_load_event(GtkWidget *button, GtkWidget *window);
void load_treeview_set(GtkBuilder *builder);
void load_add_tar_item(GtkBuilder *builder);
void load_tar_choose(GtkTreeView *treeview, GtkBuilder *builder);
void load_ok_clicked(GtkWidget *button, GtkBuilder *builder);
void load_cancel_clicked(GtkWidget *button, GtkWidget *window);
void load_file_button_select(GtkButton *button, GtkLabel *label);

void image_save_event(GtkWidget *button, GtkWidget *window);
Labels * save_treeview_set(GtkBuilder *builder, Labels *labels);
void save_image_choose(GtkWidget *widget, Labels *labels);
void save_ok_clicked(GtkWidget *button, Labels *labels);
void save_cancel_clicked(GtkWidget *button, GtkWidget *window);
void save_radio1_clicked(GtkWidget *radiobutton, GtkWidget *button);
void save_radio2_clicked(GtkWidget *radiobutton, GtkWidget *button);

void get_item_row_line(gint x, gint y, gchar **dest, gint flag);
gint get_item_n_row();
void add_tags_item(GtkBuilder *builder, gint flag);

void image_tag_event(GtkWidget *button, GtkWidget *window);
void tag_ok_clicked(GtkWidget *widget, GtkBuilder *builder);
void tag_cancel_clicked(GtkWidget *widget, GtkBuilder *builder);

void image_commit_event(GtkWidget *button, GtkWidget *window);
void commit_treeview_set(GtkBuilder *builder);
void commit_ok_clicked(GtkWidget *widget, GtkBuilder *builder);
void commit_cancel_clicked(GtkWidget *widget, GtkBuilder *builder);

gboolean create_spinner(gpointer data);
gboolean destroy_spinner(gpointer data);
#endif
