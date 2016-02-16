#include "../global/global.h"
#include "nfs_assistant.h"

#ifdef DEBUG
#define CSS_PATH UI_DIR_PATH_FOR_DEBUG"/../theme/theme.css"
#else
#define CSS_PATH UI_DIR_PATH_FOR_BUILD"/../theme/theme.css"
#endif

GtkAssistant *assistant=NULL;
GtkWidget *bar = NULL;
extern GtkWidget *terminal_window;
extern int pts_id;

void assistant_new(GtkBuilder *builder)
{
	assistant = GTK_ASSISTANT(gtk_builder_get_object (builder,"window"));
	gtk_window_set_default_size (GTK_WINDOW(assistant),600,450);
	gtk_container_set_border_width (GTK_CONTAINER(assistant),10);
	gtk_window_set_position (GTK_WINDOW(assistant),GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(assistant),"docker_assistant");

	GtkCssProvider *provider = gtk_css_provider_new ();
	gtk_css_provider_load_from_path (provider, CSS_PATH, NULL);
	GdkDisplay *display = gdk_display_get_default ();
	GdkScreen  *screen  = gdk_display_get_default_screen (display);
	gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(provider),
	                                           GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	
	creat_page1(assistant);
	creat_page2(assistant);
	creat_page3(assistant);
	creat_page4(assistant);

    g_signal_connect (G_OBJECT (assistant), "cancel",
                G_CALLBACK (on_assistant_close_cancel), &assistant);
    g_signal_connect (G_OBJECT (assistant), "close",
                G_CALLBACK (on_assistant_close_cancel), &assistant);
    g_signal_connect (G_OBJECT (assistant), "prepare",
                G_CALLBACK (on_assistant_prepare), NULL);
    g_signal_connect (G_OBJECT (assistant), "apply",
                G_CALLBACK (on_assistant_apply), NULL);
}

void docker_start()
{
	int return_value;
	if((return_value = system("sudo docker daemon&")) == -1)
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, 
		                    _("start docker daemon failed, please restart the program!"));
		exit(-1);
	}
	
}

void on_assistant_close_cancel (GtkWidget *widget, gpointer data)
{
    GtkWidget **assistant = (GtkWidget **) data;
	
    gtk_widget_destroy (*assistant);
    *assistant = NULL;
}

/*
 * Start a timer to simulate changes taking a few seconds to apply. 
 */
void on_assistant_apply (GtkWidget *widget, gpointer data)
{
	g_timeout_add (100, ( GSourceFunc)on_assistant_apply_changes, NULL);
}

gint on_assistant_apply_changes(GtkWidget *widget, gpointer data)
{
	gdouble fraction = 0.0;
	
	fraction = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR(bar));
	fraction += 0.1;

	if(fraction<1)
	{
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(bar),fraction);
		return G_SOURCE_CONTINUE;
	}
	else
	{	
#if 1
		/* 创建伪终端 */
		terminal_create(NULL);
		int fd;
		char str[128] = {0};
		sprintf(str, "/dev/pts/%d", pts_id);

		/* 输出重定向，将标准错误和标准输出定向到指定的终端 */
		printf("%s\n", str);
		if((fd = open(str, O_RDWR | O_CREAT, 0777)) == -1)
		{
			printf("open tty failed\n");
			exit(0);
		}
		else
		{
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
			close(fd);
			printf(_("EXECUTING\n"));
			printf("----------------------------------------------------------------------\n\n");
		}
#endif
		/* 后台模式启动docker daemon */
		docker_start ();
		gtk_widget_hide (GTK_WIDGET(assistant));

		modules_menu_create(GTK_WIDGET(assistant),terminal_window);
		return G_SOURCE_REMOVE;
	}	
}

/*
 *  clicked button to agree to start docker daemon and then you could choose to next page
 */
void on_button_change(GtkWidget *widget, gpointer data)
{
	GtkAssistant *assistant = GTK_ASSISTANT(data);
	GtkWidget *current_page;
	gint page_number;

	page_number = gtk_assistant_get_current_page(assistant);
	current_page = gtk_assistant_get_nth_page (assistant,page_number);

	if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
		gtk_assistant_set_page_complete(assistant,current_page,TRUE);
	else
		gtk_assistant_set_page_complete(assistant,current_page,FALSE);
}

/*
 *  set assistant titles  
 */
void on_assistant_prepare (GtkWidget *widget, GtkWidget *page, gpointer data)
{
	gint current_page, n_pages;
	gchar *title;

	current_page = gtk_assistant_get_current_page (GTK_ASSISTANT (widget));
	n_pages = gtk_assistant_get_n_pages (GTK_ASSISTANT (widget));

	title = g_strdup_printf (_("Start Wizard (%d of %d)"), current_page + 1, n_pages);
	gtk_window_set_title (GTK_WINDOW (widget), title);
	g_free (title);

	if (current_page == 3)
		gtk_assistant_commit (GTK_ASSISTANT (widget));
}


void creat_page1 (GtkAssistant *assistant)
{
	GtkWidget *label;

	label = gtk_label_new(_("this is a project that use gtk3+ to make package of docker commands "));
	gtk_widget_show(label);
	
	gtk_assistant_append_page (assistant,label);
	gtk_assistant_set_page_complete (assistant,label,TRUE);
	gtk_assistant_set_page_title (assistant,label,_("The page of introduction"));
	gtk_assistant_set_page_type (assistant,label,GTK_ASSISTANT_PAGE_INTRO);
}


void creat_page2 (GtkAssistant *assistant)
{
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *box;

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	label = gtk_label_new(_("you must start docker firsr to continue"));
	button = gtk_check_button_new_with_label (_("agree to start"));

	gtk_container_set_border_width (GTK_CONTAINER(box),60);
	gtk_box_pack_start (GTK_BOX(box),label,TRUE,FALSE,0);
	gtk_box_pack_start (GTK_BOX(box),button,TRUE,FALSE,0);
	
	gtk_widget_set_halign (label,GTK_ALIGN_CENTER);
	gtk_widget_set_valign (label,GTK_ALIGN_END);
	gtk_widget_set_halign (button,GTK_ALIGN_CENTER);
	gtk_widget_set_valign (button,GTK_ALIGN_START);
	g_signal_connect (G_OBJECT (button), "clicked",
	                   G_CALLBACK (on_button_change), assistant);

	gtk_widget_show_all(box);
	gtk_assistant_append_page (assistant,box);	
	gtk_assistant_set_page_title (assistant,box,_("The page of selection"));
	gtk_assistant_set_page_type(assistant,box,GTK_ASSISTANT_PAGE_CONTENT);
}


void creat_page3 (GtkAssistant *assistant)
{
	GtkWidget *label;

	label = gtk_label_new(_("OK,now you can have a try!"));
	gtk_widget_show(label);
	
	gtk_assistant_append_page (assistant,label);
	gtk_assistant_set_page_title(assistant,label,_("the page of confirmation"));
	gtk_assistant_set_page_type (assistant,label,GTK_ASSISTANT_PAGE_CONFIRM);
	gtk_assistant_set_page_complete(assistant,label,TRUE);
}


void creat_page4(GtkAssistant *assistant)
{
	bar = gtk_progress_bar_new();
	gtk_widget_set_halign (bar, GTK_ALIGN_CENTER);
	gtk_widget_set_valign (bar, GTK_ALIGN_CENTER);

	gtk_assistant_append_page (assistant,bar);
	gtk_assistant_set_page_type (assistant,bar,GTK_ASSISTANT_PAGE_PROGRESS);
	gtk_assistant_set_page_title(assistant,bar,_("The page of applying changes"));
	gtk_assistant_set_page_complete(assistant,bar,FALSE);
}
