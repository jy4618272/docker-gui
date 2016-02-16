#include "global/global.h"

#define FILE_CHOOSER UI_DIR_PATH"/nfs_select_dockerfile.ui"
#define  FILE_CHOOSER_WINDOW "filechooserdialog2"
#define TEXT_VIEW UI_DIR_PATH"/nfs_new_dockerfile.ui"
#define  TEXT_VIEW_WINDOW "window1"
#define  DOCKER_BUILDE_TEXTVIEW UI_DIR_PATH"/nfs_dockerbuild_textview.ui"
#define  DOCKEERBUILD_TEXTVIEW_WINDOW "window1"

void create_textview();//创建textview的主函数
void select_dockerfile_textview(GtkWidget *window,gpointer data);//选择/创建dockerfile的主函数