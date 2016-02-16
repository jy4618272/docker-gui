#ifndef _NFS_COMMAND_H_
#define _NFS_COMMAND_H_

#ifdef DEBUG
#define COMMAND_INFORMATION_MODULES_FILE UI_DIR_PATH_FOR_DEBUG"/nfs_command_information_modules.ui"
#else
#define COMMAND_INFORMATION_MODULES_FILE UI_DIR_PATH_FOR_BUILD"/nfs_command_information_modules.ui"
#endif

#define COMMAND_INFORMATION_MODULES_WINDOW "window"

void command_new_window (GtkWidget *command,gpointer data);//命令信息-->进入界面主函数
void button33_fun(GtkWidget *button,gpointer *data);//进入界面,"关于Docker查看容器 查看版本"按钮的功能
void button22_fun(GtkWidget *button,gpointer *data);//输入命令的功能

#endif