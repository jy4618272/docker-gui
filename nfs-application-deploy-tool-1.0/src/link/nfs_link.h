#ifndef _NFS_LINK_H_
#define _NFS_LINK_H_

#include "global/global.h"

#ifdef DEBUG
#define  DOCKER_LINK_CONTAINS UI_DIR_PATH_FOR_DEBUG"/nfs_link.ui"
#else
#define  DOCKER_LINK_CONTAINS UI_DIR_PATH_FOR_BUILD"/nfs_link.ui"
#endif

#define  DOCKER_LINK_WINDOW "container_inter"
void create_link_contains();

#endif