#include "../global/global.h"

int pts_id = 0;
GtkWidget *terminal_window = NULL;

struct TerminalPlugin{
	GPid child_pid;
	GtkWidget *frame;
	GtkWidget *shell, *term;
	GtkWidget *shell_box, *term_box;
	gboolean pty_flags;
		
};
typedef struct TerminalPlugin TP;

static void init_shell(TP *tp, const char *path)
{
	gchar *shell[2] = {0, 0};
	static gboolean first_time = TRUE;
	VteTerminal *term = VTE_TERMINAL(tp -> shell);

	shell[0] = vte_get_user_shell();
	
	if(shell[0] == NULL)
		shell[0] = g_strdup("/bin/sh");
	
	if(!first_time)
		vte_terminal_reset (term, FALSE, TRUE);
	else
		first_time = FALSE;

//	vte_terminal_set_font_from_string (term, "19");
	vte_terminal_fork_command_full (term, VTE_PTY_DEFAULT, path, shell, NULL,
	                                0, NULL, NULL, NULL, NULL);

	g_free(shell[0]);
}

static void init_term(TP *tp, const char *path)
{
	gchar *shell[2] = {0, 0};
	VteTerminal *term = VTE_TERMINAL(tp ->term);

	shell[0] = vte_get_user_shell();
	
	if(shell[0] == NULL)
		shell[0] = g_strdup("/bin/sh");

	vte_terminal_fork_command_full (term, VTE_PTY_DEFAULT, NULL, shell, NULL,
	                                G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, NULL);

	g_free(shell[0]);
}

static gboolean terminal_focus_cb(GtkWidget *widget, GdkEventKey *event, TP *tp)
{
	gtk_widget_grab_focus (widget);
	return FALSE;
}

static gboolean terminal_keypress_cb(GtkWidget *widget, GdkEventKey *event, TP *tp)
{
	if(event->type != GDK_KEY_PRESS)
		return FALSE;

	/* ctrl-d */
	if((event->keyval == GDK_KEY_d || event->keyval == GDK_KEY_D) &&
	   (event->state & GDK_CONTROL_MASK))
	{
		if(tp->child_pid)
		{
			kill(tp->child_pid, SIGINT);
			tp->child_pid = 0;
		}
		return TRUE;
	}
	return FALSE;
}

/*
static gboolean terminal_click_cb(GtkWidget *widget, GdkEventButton *event, TP *tp)
{
	static gboolean flg = TRUE;
	if(event->button == 3)
	{
		if(flg)
		{
			gtk_container_remove (GTK_CONTAINER(tp->frame), tp->shell_box);
			gtk_container_add(GTK_CONTAINER(tp->frame), tp->term_box);
			gtk_widget_show_all (tp->term_box);
			flg = FALSE;
		}
		else
		{
			gtk_container_remove (GTK_CONTAINER(tp->frame), tp->term_box);
			gtk_container_add(GTK_CONTAINER(tp->frame), tp->shell_box);
			gtk_widget_show_all (tp->shell_box);
			flg = TRUE;
		}
	}

	return FALSE;
}
*/

static void terminal_child_exited_cb(VteTerminal *term, gpointer data)
{
	int status = vte_terminal_get_child_exit_status (term);
	TP *tp = (TP *)data;
	GPid  pid = tp->child_pid;

	if(tp->child_pid)
	{
		gboolean focus;

		focus = gtk_widget_is_focus (tp->term);

		gtk_container_remove (GTK_CONTAINER(tp->frame), tp->term_box);
		gtk_container_add(GTK_CONTAINER(tp->frame), tp->shell_box);
		gtk_widget_show_all (tp->shell_box);
		if(focus)
			gtk_widget_grab_focus (tp->shell);

		
		tp->child_pid = 0;
	}

	g_signal_emit_by_name (tp, "child-exited", pid, status);
}

static GtkWidget *create_terminal(TP *tp)
{
	GtkWidget *term;
	
	term = vte_terminal_new();
	gtk_widget_set_size_request (term, 10, 10);
	vte_terminal_set_size(VTE_TERMINAL(term), 50, 1);

	g_signal_connect(G_OBJECT(term), "focus-in-event", G_CALLBACK(terminal_focus_cb), tp);
//	g_signal_connect(G_OBJECT(term), "button-press-event", G_CALLBACK(terminal_click_cb), tp);
	g_signal_connect(G_OBJECT(term), "child-exited", G_CALLBACK(terminal_child_exited_cb), tp);
	return term;
}


static GtkWidget *create_box(GtkWidget *term)
{
	GtkWidget *sb, *hbox;

	sb = gtk_scrollbar_new (GTK_ORIENTATION_VERTICAL, gtk_scrollable_get_vadjustment (GTK_SCROLLABLE(term)));
	gtk_widget_set_can_focus (sb, FALSE);

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(hbox), term, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), sb, FALSE, TRUE, 0);

	g_object_ref_sink (hbox);

	return hbox;
}

void terminal_create(GtkWidget *window)
{
//	g_return_if_fail(window != NULL);

	TP *tp = NULL;
	GtkWidget *book;
	GtkWidget *frame;
	GtkWidget *label;
	gchar result[512] = {0};
	gchar *p = NULL;
	
	tp = (TP *)g_malloc(sizeof(TP));
	tp->child_pid = 0;

	/* create terminal */
	tp->shell     = create_terminal (tp);
	tp->shell_box = create_box (tp->shell);
	
	tp->term     = create_terminal (tp);
	tp->term_box = create_box(tp->term);

	/* key-press handler for ctrl-d "kill" */
	g_signal_connect(G_OBJECT(tp->term), "key-press-event", 
	                 G_CALLBACK(terminal_keypress_cb), tp);

	book = gtk_notebook_new();
	gtk_container_set_border_width (GTK_CONTAINER(book), 3);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (book), GTK_POS_TOP);

	/* 添加用户终端 */
	label = gtk_label_new(_("Terminal"));	
	frame = gtk_frame_new("");
	gtk_container_set_border_width (GTK_CONTAINER(frame), 2);
	gtk_frame_set_shadow_type (GTK_FRAME(frame), GTK_SHADOW_IN);

	gtk_container_add(GTK_CONTAINER(frame), tp->shell_box);	
	gtk_notebook_prepend_page (GTK_NOTEBOOK(book), frame, label);

#if 0	
	/* 添加Textview */
	GtkWidget *sw;
	GtkWidget *textview;
	
	label = gtk_label_new("TextView");
	frame = gtk_frame_new("TextView");
	gtk_frame_set_shadow_type (GTK_FRAME(frame), GTK_SHADOW_IN);
	gtk_notebook_prepend_page (GTK_NOTEBOOK(book), frame, label);
	
	sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	textview = gtk_text_view_new();
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
	
	gtk_container_add(GTK_CONTAINER(frame), sw);
	gtk_container_add(GTK_CONTAINER(sw), textview);
#endif

	
	/* 添加显示结果终端 */
	label = gtk_label_new(_("Executing"));	
	frame = gtk_frame_new("");

	gtk_container_set_border_width (GTK_CONTAINER(frame), 2);
	gtk_container_add(GTK_CONTAINER(frame), tp->term_box);
	
	gtk_frame_set_shadow_type (GTK_FRAME(frame), GTK_SHADOW_IN);
	gtk_notebook_prepend_page (GTK_NOTEBOOK(book), frame, label);

//	gtk_container_add(GTK_CONTAINER(window), book);

	/* 初始化终端 */
	init_shell(tp, NULL);
	init_term(tp, NULL);

	terminal_window = book;
	
	/*  得出最新创建终端的tty号 */
	strcpy(result, popen_and_fread ("ls -U /dev/pts"));
	p  = strtok(result, "\n");

	pts_id = atoi(p);
	printf("%d\n", pts_id);
}














