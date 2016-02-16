#include "global/global.h"
#include "nfs_command_information_modules.h"

void
button22_fun(GtkWidget *button,gpointer *data)//输入命令的功能
{
		static gchar *command11;
		GtkWidget *dialog11;
		static FILE *fd;
		static gint pp;
		static gchar buf[1024];
		static gchar *message_info;
		GtkMessageType type11;
		gint ss;
		command11=gtk_entry_get_text(GTK_ENTRY(data));
		memset(buf,'\0',sizeof(buf));
		fd=popen(command11,"r");
		pp=fread(buf,sizeof(char),sizeof(buf),fd);
		//printf("buf is %s\n",buf);
		if(pp == NULL){
			message_info=_("it is not a command or start failed");
				
			type11=GTK_MESSAGE_ERROR;
				
		}
		else{
		message_info=buf;
		type11=GTK_MESSAGE_INFO;
		}
		dialog11=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,type11,GTK_BUTTONS_OK,message_info);
		ss=gtk_dialog_run(GTK_DIALOG(dialog11));
		gtk_widget_destroy (dialog11);
		pclose (fd);
}


void
button33_fun(GtkWidget *button,gpointer *data)//"关于Docker查看容器 查看版本"按钮的功能
{
	GtkWidget *dialog;
	gchar *message_formart;
	GtkMessageType type11;
	static char buf_fd[1024]={0};
	static gchar buf2[1024]={0};
	static gint pp=0;
	static FILE *fd=NULL;
	static FILE *fd1=NULL;
	static char error_fd[1024]={0};
	memset(buf_fd,'\0',sizeof(buf_fd));
	memset(buf2,'\0',sizeof(buf2));
	memset(error_fd,'\0',sizeof(error_fd));
	sprintf(error_fd,"%s 2>&1",(char*)data);
	fd=popen((char*)data,"r");
	
	pp=fread(buf_fd,sizeof(char),sizeof(buf_fd),fd);
	
	if(pp == NULL){
		if(data!=NULL){
			fd1=popen(error_fd,"r");
				fread(buf2,sizeof(char),sizeof(buf2),fd1);
				message_formart=buf2;
				pclose(fd1);
			}
			if(data==NULL)
		{
				message_formart=_("please start up Docker first");
			}
		type11=GTK_MESSAGE_ERROR;
	}
	else{
	message_formart=buf_fd;
		type11=GTK_MESSAGE_INFO;
	}
	dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,type11,GTK_BUTTONS_OK,message_formart);
		gint ss=gtk_dialog_run (GTK_DIALOG(dialog));	
		//if(ss==GTK_RESPONSE_OK){
			gtk_widget_destroy (dialog);
		//}
		pclose (fd);
		memset(buf_fd,'\0',sizeof(buf_fd));


}
void
command_new_window (GtkWidget *command,gpointer data)//命令信息-->进入界面主函数
{
	GtkWidget *window;
	GtkWidget *button11;
	GtkWidget *button22;
	GtkWidget *button33;
	GtkWidget *button44;
	GtkWidget *button55;
	GtkWidget *label11;
	GtkWidget  *entry11;
	GtkWidget *line11;
	GtkBuilder *builder;
	GError* error = NULL;
	
	
	/* Load UI from file */
	builder = gtk_builder_new ();
	
	if (!gtk_builder_add_from_file (builder, COMMAND_INFORMATION_MODULES_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	/* Auto-connect signal handlers */

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (builder,COMMAND_INFORMATION_MODULES_WINDOW));
        if (!window)
        {
		g_critical ("Widget \"%s\" is missing in file %s.",
				TOP_WINDOW,
				UI_FILE);
        }

	button11=GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	button44=GTK_WIDGET(gtk_builder_get_object (builder,"button4"));
	button33=GTK_WIDGET(gtk_builder_get_object (builder,"button3"));
	button22=GTK_WIDGET(gtk_builder_get_object (builder,"button2"));
	button55=GTK_WIDGET(gtk_builder_get_object (builder,"button5"));
	label11=GTK_WIDGET(gtk_builder_get_object(builder,"label1"));
	entry11=GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	gtk_button_set_label (GTK_BUTTON(button11),_("look up image"));
	gtk_button_set_label (GTK_BUTTON(button33),_("Docker info"));
	gtk_button_set_label (GTK_BUTTON(button44),_("look up container"));
	gtk_button_set_label (GTK_BUTTON(button22),_("executing"));
	gtk_button_set_label (GTK_BUTTON(button55),_("look up version"));
	gtk_label_set_text (GTK_LABEL(label11),_("input command"));
	//gtk_entry_set_text (GTK_ENTRY(entry11),"2222");
	gtk_window_set_title (GTK_WINDOW(window),_("command info"));
	gtk_widget_set_name(window, "window_command_modules_css");
	
	g_signal_connect(G_OBJECT(button33),"clicked",G_CALLBACK(button33_fun),"docker info");
	/* ANJUTA: Widgets initialization for command.ui - DO NOT REMOVE */
	g_signal_connect(G_OBJECT(button11),"clicked",G_CALLBACK(button33_fun),"docker images");
	g_signal_connect(G_OBJECT(button44),"clicked",G_CALLBACK(button33_fun),"docker ps -a");
	g_signal_connect(G_OBJECT(button55),"clicked",G_CALLBACK(button33_fun),"docker -v");
	g_signal_connect(G_OBJECT(button22),"clicked",G_CALLBACK(button22_fun),entry11);
	g_object_unref (builder);
	
	gtk_widget_show_all (GTK_WIDGET (window));
}
