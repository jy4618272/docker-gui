#include "global/global.h"

#ifdef DEBUG
#define FILE_CHOOSER UI_DIR_PATH_FOR_DEBUG"/nfs_select_dockerfile.ui"
#define TEXT_VIEW UI_DIR_PATH_FOR_DEBUG"/nfs_new_dockerfile.ui"
#define  DOCKER_BUILDE_TEXTVIEW UI_DIR_PATH_FOR_DEBUG"/nfs_dockerbuild_textview.ui"
#else
#define FILE_CHOOSER UI_DIR_PATH_FOR_BUILD"/nfs_select_dockerfile.ui"
#define TEXT_VIEW UI_DIR_PATH_FOR_BUILD"/nfs_new_dockerfile.ui"
#define  DOCKER_BUILDE_TEXTVIEW UI_DIR_PATH_FOR_BUILD"/nfs_dockerbuild_textview.ui"
#endif

#define  FILE_CHOOSER_WINDOW "filechooserdialog2"
#define  TEXT_VIEW_WINDOW "window1"
#define  DOCKEERBUILD_TEXTVIEW_WINDOW "window1"

void create_textview();//创建textview的主函数
void select_dockerfile_textview(GtkWidget *window,gpointer data);//选择/创建dockerfile的主函数