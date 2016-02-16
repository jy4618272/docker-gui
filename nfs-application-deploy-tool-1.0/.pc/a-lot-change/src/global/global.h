#ifndef _GLOBAL_
#define _GLOBAL_

#include <libintl.h>
#include <config.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <vte/vte.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <gtksourceview/gtksource.h>

#define UI_DIR_PATH "/usr/share/nfs_application_deploy_tool"

#include "module/nfs_docker-modules.h"             //创建菜单条来显示各个模块菜单
#include "assistant/nfs_assistant.h"
#include "manage/nfs-application-deploy-tool.h"
#include "module/nfs_command_information_modules.h"//命令信息模块界面
#include "image/nfs_image.h"
#include "volume/nfs_volume.h"
#include "container/nfs_container_manage.h"        //容器管理模块界面
#include "dockerfile/nfs_dockerfile_manage.h"      //配置文件模块
#include "link/nfs_link.h"                         //容器link的模块
#include "link/nfs_plink.h"

#endif