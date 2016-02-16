#ifndef _NFS_PLINK_H_
#define _NFS_PLINK_H_

#include "global/global.h"

#ifdef DEBUG
#define  P_LINK UI_DIR_PATH_FOR_DEBUG"/nfs_plink.ui"
#else
#define  P_LINK UI_DIR_PATH_FOR_BUILD"/nfs_plink.ui"
#endif

#define  P_LINK_WINDOW "exter_access"
void p_link_contains();

#endif