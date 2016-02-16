#ifndef _DOCKER_MODULES_H_
#define _DOCKER_MODULES_H_

#define UI_FILE11 "UI_DIR_PATH/nfs_docker_modules.ui"
#define TOP_WINDOW11 "window11"

void modules_menu_create(GtkWidget *window11,gpointer data);//创建菜单条来显示各个模块菜单
void main_quit(GtkWidget *window1, GtkWidget *window2);
void terminal_create(GtkWidget *window);
#endif