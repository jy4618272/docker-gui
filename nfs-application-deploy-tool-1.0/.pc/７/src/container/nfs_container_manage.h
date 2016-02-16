#include "global/global.h"


#define CONTAINER_MANAGE_FILE "src/ui/nfs_container_manage.ui"
#define  CONTAINER_MANAGE_WINDOW  "window1"
#define CREATE_CONTAINS_FILE "src/ui/nfs_container_manage_createcontains.ui"
#define  CONTAINS_MANAGE_CREATECONTAINS_WINDOW "window22"

#define CONTAINER_MANAGE_GUARD_FILE "src/ui/nfs_container_manage_guard.ui"
#define  CONTAINER_MANAGE_GUARD_WINDOW  "window_guard"

#define CONTAINER_MANAGE_LSGUARD_FILE "src/ui/nfs_container_manage_lsguard.ui"
#define  CONTAINER_MANAGE_LSGUARD_WINDOW  "window_lsgurd"
#define PATH "/var/lib/docker/containers/"
gint contains_num;//容器的数量
struct sss{
        char id[50];
        char image[50];
        char name[50];
};//容器的信息
struct sss a[1024];
void create_view(GtkWidget *container,gpointer data);//容器管理的主界面

/*..........................导入功能.................*/
 void import_create_file_veiw_fun(GtkWidget *window,gpointer data);
 void import_create_file_view(gchar *buf);//导入功能填写界面
 void import_print_file_name(GtkWidget *window);
 void import_selcet_file(GtkWidget *window,gpointer data);
/*............................导入功能.......................*/


void export_selcet_file(GtkWidget *window,gpointer data);//导出功能
void create_contians_view();//创建容器填入参数界面
void button11_create_contians_view_fun(GtkWidget *window,gpointer data);
gpointer defin_myname_contians_fun(gpointer data);
void button22_treeview_fun(GtkWidget *window,gpointer data);
void button33_treeview_fun(GtkWidget *window,gpointer *data);
void button44_treeview_fun(GtkWidget *window,gpointer *data);
gpointer docker_tree_view_clean(gpointer data);
gpointer docker_contains_create_func(gpointer data);
void button66_treeview_fun(GtkWidget *window,gpointer *data);
void button66_treeview_contains_fun(gpointer data);
void button99_treeview_fun(GtkWidget *window,gpointer *data);
void button88_treeview_fun(GtkButton *button,gpointer data);
void button11_treeview_fun(GtkWidget *window,gpointer data);
void docker_contain_start(gpointer data);
void clicked_row11(GtkWidget *window,gpointer data);
void contains_message(GtkMessageType type,gpointer buf);//全局的一个函数:执行成功某件事后弹出一个界面提示
void docker_contain();
void combox_images_data(GtkWidget *combox_all);