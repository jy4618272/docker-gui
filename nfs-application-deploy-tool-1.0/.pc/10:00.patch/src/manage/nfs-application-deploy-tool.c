/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * nfs-application-deploy-tool.c
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


#include "global/global.h"
#include "manage/nfs-application-deploy-tool.h"

/* For testing propose use the local (not installed) ui file */



G_DEFINE_TYPE (Nfsapplicationdeploytool, nfs_application_deploy_tool, GTK_TYPE_APPLICATION);

/* ANJUTA: Macro NFS_APPLICATION_DEPLOY_TOOL_APPLICATION gets Nfsapplicationdeploytool - DO NOT REMOVE */
struct _NfsapplicationdeploytoolPrivate
{
	/* ANJUTA: Widgets declaration for nfs_application_deploy_tool.ui - DO NOT REMOVE */
};

/* Create a new window loading a file */
static void
nfs_application_deploy_tool_new_window (GApplication *app,
                           GFile        *file)
{
	/* 不是超级权限打开的话就退出 */
	if(getuid() != 0)
	{
		GtkWidget *dialog = gtk_message_dialog_new(NULL,
	           				GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,
	               			GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
		                    _("please usr root mode to execute it!"), NULL);
		gtk_window_set_position (GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy (dialog);
		exit(0);
	} 

	GtkWidget *window = NULL;

	GtkBuilder *builder = NULL;
	GError* error = NULL;	

	/* Load UI from file */
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (builder, app);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (builder, TOP_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
                		TOP_WINDOW,
                		UI_FILE);
        }

	assistant_new(builder);
    
	//g_signal_connect(G_OBJECT(),"clicked",G_CALLBACK( modules_menu_create),NULL);

	/* ANJUTA: Widgets initialization for nfs_application_deploy_tool.ui - DO NOT REMOVE */

	g_object_unref (builder);
	
	
	gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));
	if (file != NULL)
	{
		/* TODO: Add code here to open the file in the new window */
	}

	
	
	
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_widget_show_all (GTK_WIDGET (window));
    
}


/* GApplication implementation */
static void
nfs_application_deploy_tool_activate (GApplication *application)
{
	nfs_application_deploy_tool_new_window (application, NULL);
}

static void
nfs_application_deploy_tool_open (GApplication  *application,
                     GFile        **files,
                     gint           n_files,
                     const gchar   *hint)
{
	gint i;

	for (i = 0; i < n_files; i++)
		nfs_application_deploy_tool_new_window (application, files[i]);
}

static void
nfs_application_deploy_tool_init (Nfsapplicationdeploytool *object)
{
	object->priv = G_TYPE_INSTANCE_GET_PRIVATE (object, NFS_APPLICATION_DEPLOY_TOOL_TYPE_APPLICATION, NfsapplicationdeploytoolPrivate);
}

static void
nfs_application_deploy_tool_finalize (GObject *object)
{
	G_OBJECT_CLASS (nfs_application_deploy_tool_parent_class)->finalize (object);
}

static void
nfs_application_deploy_tool_class_init (NfsapplicationdeploytoolClass *klass)
{
	G_APPLICATION_CLASS (klass)->activate = nfs_application_deploy_tool_activate;
	G_APPLICATION_CLASS (klass)->open = nfs_application_deploy_tool_open;

	g_type_class_add_private (klass, sizeof (NfsapplicationdeploytoolPrivate));

	G_OBJECT_CLASS (klass)->finalize = nfs_application_deploy_tool_finalize;
}

Nfsapplicationdeploytool *
nfs_application_deploy_tool_new (void)
{
	return g_object_new (nfs_application_deploy_tool_get_type (),
	                     "application-id", "org.gnome.nfs_application_deploy_tool",
	                     "flags", G_APPLICATION_HANDLES_OPEN,
	                     NULL);
}

