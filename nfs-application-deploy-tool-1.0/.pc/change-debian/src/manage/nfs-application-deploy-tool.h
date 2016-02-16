/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * nfs-application-deploy-tool.h
 * Copyright (C) 2015 zkfd <zkfd@nfschina.com>
 * 
 * nfs-application-deploy-tool is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * nfs-application-deploy-tool is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _NFS_APPLICATION_DEPLOY_TOOL_
#define _NFS_APPLICATION_DEPLOY_TOOL_

#include "global/global.h"

G_BEGIN_DECLS

#define NFS_APPLICATION_DEPLOY_TOOL_TYPE_APPLICATION             (nfs_application_deploy_tool_get_type ())
#define NFS_APPLICATION_DEPLOY_TOOL_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), NFS_APPLICATION_DEPLOY_TOOL_TYPE_APPLICATION, Nfsapplicationdeploytool))
#define NFS_APPLICATION_DEPLOY_TOOL_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), NFS_APPLICATION_DEPLOY_TOOL_TYPE_APPLICATION, NfsapplicationdeploytoolClass))
#define NFS_APPLICATION_DEPLOY_TOOL_IS_APPLICATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NFS_APPLICATION_DEPLOY_TOOL_TYPE_APPLICATION))
#define NFS_APPLICATION_DEPLOY_TOOL_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), NFS_APPLICATION_DEPLOY_TOOL_TYPE_APPLICATION))
#define NFS_APPLICATION_DEPLOY_TOOL_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), NFS_APPLICATION_DEPLOY_TOOL_TYPE_APPLICATION, NfsapplicationdeploytoolClass))

#define UI_FILE UI_DIR_PATH"/nfs_assistant.ui" 
#define TOP_WINDOW "window"

typedef struct _NfsapplicationdeploytoolClass NfsapplicationdeploytoolClass;
typedef struct _Nfsapplicationdeploytool Nfsapplicationdeploytool;
typedef struct _NfsapplicationdeploytoolPrivate NfsapplicationdeploytoolPrivate;

struct _NfsapplicationdeploytoolClass
{
	GtkApplicationClass parent_class;
};

struct _Nfsapplicationdeploytool
{
	GtkApplication parent_instance;

	NfsapplicationdeploytoolPrivate *priv;

};

GType nfs_application_deploy_tool_get_type (void) G_GNUC_CONST;
Nfsapplicationdeploytool *nfs_application_deploy_tool_new (void);
void assistant_new(GtkBuilder *builder);

/* Callbacks */

G_END_DECLS

#endif /* _APPLICATION_H_ */

