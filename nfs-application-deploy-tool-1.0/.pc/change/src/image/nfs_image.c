#include "../global/global.h"
#include "nfs_image.h"

#define IMAGE_FILE "src/ui/nfs_image.ui"
#define IMAGE_IMAGE_FILE "src/ui/nfs_image_image.ui"
#define SAVE_FILE "src/ui/nfs_image_save_load_rmi.ui"
#define LOAD_FILE "src/ui/nfs_image_save_load_rmi.ui"
#define RMI_FILE "src/ui/nfs_image_save_load_rmi.ui"
#define TAG_FILE "src/ui/nfs_image_save_load_rmi.ui"
#define PULL_FILE "src/ui/nfs_image_pull.ui"
#define COMMIT_FILE "src/ui/nfs_image_commit.ui"
#define LOAD_AND_SAVE_PATH  "src/image_tar"

#define SEARCH_FILE "search.txt"

#define RMI_FLAG 1
#define SAVE_FLAG 2
#define MAX_SIZE 1024
#define PERROR(s) {printf("\t%s\n\tfunction: %s   line: %d\n",s,__FUNCTION__,__LINE__);}

static gint max_page = 0;
static gint current_page = 0;
extern gint contains_num;

typedef struct
{
  gint   number;
  gchar *repository;
  gchar *tags;
  gchar *image_id;
  gchar *created;
  gchar *size;
}Item;

struct array
{
	gint line1;
	gint line2;
	gint line3;
	gint line4;
	gint line5;
};
struct array arr;
//extern struct sss a[];

void image_event(GtkWidget *widget, GtkWidget *window)
{
	GtkBuilder *builder;
	GtkWidget  *window_image;
	GError     *error = NULL;
	GtkWidget  *image_button;
	GtkWidget  *load_button;
	GtkWidget  *save_button;
	GtkWidget  *pull_button;
	GtkWidget  *rmi_button;
	GtkWidget  *tag_button;
	GtkWidget  *commit_button;
	GtkWidget  *build_button;
	
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, IMAGE_FILE, &error))
	{
		g_critical ("Couldn't load builder: %s", error->message);
		g_error_free (error);
	}

	window_image   = GTK_WIDGET(gtk_builder_get_object (builder,"image_window"));
	image_button   = GTK_WIDGET(gtk_builder_get_object (builder,"image_button"));
	load_button    = GTK_WIDGET(gtk_builder_get_object (builder,"load_button"));
	save_button    = GTK_WIDGET(gtk_builder_get_object (builder,"save_button"));
	pull_button    = GTK_WIDGET(gtk_builder_get_object (builder,"pull_button"));
	rmi_button     = GTK_WIDGET(gtk_builder_get_object (builder,"rmi_button"));
	tag_button     = GTK_WIDGET(gtk_builder_get_object (builder,"tag_button"));
	commit_button  = GTK_WIDGET(gtk_builder_get_object (builder,"commit_button"));
	build_button   = GTK_WIDGET(gtk_builder_get_object (builder,"build_button"));

	gtk_widget_set_name (window_image, "image_window");
	gtk_window_set_default_size (GTK_WINDOW(window_image),700,500);
	gtk_window_set_position (GTK_WINDOW(window_image),GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(window_image),_("image manage"));

	gtk_button_set_label (GTK_BUTTON(image_button), _("look up image(image)"));
	gtk_button_set_label (GTK_BUTTON(load_button), _("load image(load)"));
	gtk_button_set_label (GTK_BUTTON(save_button), _("save image(save)"));
	gtk_button_set_label (GTK_BUTTON(pull_button), _("pull image(pull)"));
	gtk_button_set_label (GTK_BUTTON(rmi_button), _("remove image(rmi)"));
	gtk_button_set_label (GTK_BUTTON(tag_button), _("tag image(tag)"));
	gtk_button_set_label (GTK_BUTTON(commit_button), _("commit image(commit)"));
	gtk_button_set_label (GTK_BUTTON(build_button), _("build image(build)"));
	
	g_signal_connect(G_OBJECT(window_image), "destroy", G_CALLBACK(image_window_destroy), window);
	g_signal_connect(G_OBJECT(image_button), "clicked", G_CALLBACK(image_image_event), window_image);
	g_signal_connect(G_OBJECT(save_button),  "clicked", G_CALLBACK(image_save_event), window_image);
	g_signal_connect(G_OBJECT(load_button),  "clicked", G_CALLBACK(image_load_event), window_image);
	g_signal_connect(G_OBJECT(rmi_button),   "clicked", G_CALLBACK(image_rmi_event),  window_image);
	g_signal_connect(G_OBJECT(pull_button),  "clicked", G_CALLBACK(image_pull_event), window_image);
	g_signal_connect(G_OBJECT(tag_button),   "clicked", G_CALLBACK(image_tag_event),  window_image);
	g_signal_connect(G_OBJECT(commit_button),"clicked", G_CALLBACK(image_commit_event),window_image);

	//gtk_widget_hide (window);
	gtk_widget_show_all (window_image);
}

void image_window_destroy(GtkWidget *window, GtkWidget *window1)
{
	gtk_widget_destroy (window);
//	gtk_widget_show_all(window1);
}

void message_dialog_new(GtkWindow *parrent, GtkMessageType type,  
                        GtkButtonsType buttons, const gchar *message)
{
	GtkWidget *dialog;
	
	dialog = gtk_message_dialog_new(parrent, GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,
	               						type, buttons, message, NULL);
	gtk_window_set_position (GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy (dialog);
}

void tree_column_new(GtkTreeView *view, gint column, const gchar* column_name)
{
	GtkCellRenderer   *renderer;
	GtkTreeViewColumn *columns;

	renderer = gtk_cell_renderer_text_new ();
	columns  = gtk_tree_view_column_new_with_attributes (column_name, renderer,
                                               			"text", column, NULL);
	gtk_tree_view_column_set_resizable (columns, TRUE);
	gtk_tree_view_column_set_clickable (columns,TRUE);
	gtk_tree_view_append_column (view, columns);
	gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(columns),TRUE); 
}

gchar *popen_and_fread(gchar *command)
{
	gchar str[4096] = {0};
	FILE  *fp       = NULL;
	gchar *p        = NULL;
	
	fp = popen(command,"r");
	if(NULL == fp)
	{
		PERROR(_("popen error!"));
		return NULL;
	}

	if(fread(str, 1, sizeof(str), fp) == 0);
			
	if(feof(fp) == 0)
	{
		PERROR(_("fread error!"));
		return NULL;
	}

	if('\n' == str[strlen(str)-1])
		str[strlen(str)-1]='\0';

	pclose (fp);
	p = str;
	
	return p;
}

void image_pull_event(GtkWidget *button, GtkWidget *window)
{
	GtkBuilder  *builder;
	GError      *error = NULL;
	GtkWidget   *pull_window;
	GtkTreeView *view;
	GtkWidget   *label1;
	GtkWidget   *label2;
	GtkWidget   *ok_button;
	GtkWidget   *cancel_button ;
	GtkWidget   *search_button ;
	GtkWidget   *button1;
	GtkWidget   *button2;
	GtkWidget   *button3;
	
	builder = gtk_builder_new();

	if(!gtk_builder_add_from_file (builder,PULL_FILE,&error))
	{
		g_critical(_("the pull_file opened failed %s\n"),error->message);
		g_error_free (error);
	}
	
	pull_window   = GTK_WIDGET(gtk_builder_get_object (builder,"pull_window"));
	view          = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	label1        = GTK_WIDGET(gtk_builder_get_object(builder,"label1"));
	label2        = GTK_WIDGET(gtk_builder_get_object(builder,"label2"));
	ok_button     = GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	cancel_button = GTK_WIDGET(gtk_builder_get_object (builder,"button2"));
	search_button = GTK_WIDGET(gtk_builder_get_object (builder,"button3"));
	button1       = GTK_WIDGET(gtk_builder_get_object(builder,"button4"));
	button2		  = GTK_WIDGET(gtk_builder_get_object(builder,"button5"));
	button3		  = GTK_WIDGET(gtk_builder_get_object(builder,"button6"));
	
	gtk_window_set_position (GTK_WINDOW(pull_window),GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(pull_window),"pull_window");
	gtk_window_set_default_size (GTK_WINDOW(pull_window),700,500);
	gtk_window_set_title (GTK_WINDOW(pull_window),_("download image"));
	
	gtk_label_set_text (GTK_LABEL(label1),_("please input the name of image you want to download"));
	gtk_label_set_text (GTK_LABEL(label2),_("please input the name of image you want to search"));
	gtk_button_set_label (GTK_BUTTON(button1), _("home page"));
	gtk_button_set_label (GTK_BUTTON(button2), _("previous page"));
	gtk_button_set_label (GTK_BUTTON(button3), _("next page"));
	
	pull_treeview_set(view,builder);
	
	g_signal_connect(G_OBJECT(pull_window),"destroy",G_CALLBACK(pull_cancel_clicked),pull_window);
	g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(pull_ok_clicked_create_thread),builder);
	g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(pull_cancel_clicked),pull_window);
	g_signal_connect(G_OBJECT(search_button),"clicked",G_CALLBACK(pull_search_clicked),builder);
	g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(pull_switch_to_homepage),builder);
	g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(pull_switch_to_prevpage),builder);
	g_signal_connect(G_OBJECT(button3),"clicked",G_CALLBACK(pull_switch_to_nextpage),builder);
//	g_signal_connect_swapped(G_OBJECT(button2),"clicked",G_CALLBACK(switch_to_prev_page),book);
//	g_signal_connect_swapped(G_OBJECT(button3),"clicked",G_CALLBACK(switch_to_next_page),book);
	g_signal_connect(G_OBJECT(view),"cursor_changed",G_CALLBACK(pull_treeview_get_search_image),builder);
	
	gtk_widget_show_all(pull_window);
}

void pull_treeview_set(GtkTreeView *view,GtkBuilder *builder)
{
	GtkListStore *list;
	
	list = GTK_LIST_STORE(gtk_builder_get_object (builder,"liststore"));
	gtk_list_store_clear (GTK_LIST_STORE(list));
	
	tree_column_new(view, 0, _("NUM"));
	tree_column_new(view, 1, _("NAME"));
	tree_column_new(view, 2, _("DESCRIPTION"));
	tree_column_new(view, 3, _("STARS"));
	tree_column_new(view, 4, _("OFFICIAL"));
	tree_column_new(view, 5, _("AUTOMATED"));
}

void pull_switch_to_homepage(GtkButton *button, GtkBuilder *builder)
{
	if(0 != access(SEARCH_FILE,F_OK))
		return;
	
	current_page = 0;
	attach_widgets (builder, current_page, max_page);
}

void pull_switch_to_prevpage(GtkButton *button, GtkBuilder *builder)
{
	if(0 != access(SEARCH_FILE,F_OK))
		return;
	
	current_page--;
	if(current_page < 0)
	{
		current_page = 0;
		return;
	}
	
	attach_widgets (builder, current_page, max_page);
}

void pull_switch_to_nextpage(GtkButton *button, GtkBuilder *builder)
{
	if(0 != access(SEARCH_FILE,F_OK))
		return;
	
	current_page++;
	if(current_page >= max_page)
	{
		current_page --;
		return;
	}
	
	attach_widgets (builder, current_page, max_page);
}

void pull_treeview_get_search_image(GtkTreeView *view, GtkBuilder *builder)
{
	gchar            *tags = NULL;
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;
	
	selection = gtk_tree_view_get_selection (view);
	model     = gtk_tree_view_get_model (view);
	gtk_tree_selection_get_selected (selection,&model,&iter);
	tags      = g_malloc(sizeof(gchar)*128);

	if(!gtk_tree_model_get_iter(model,&iter,gtk_tree_model_get_path (model,&iter)))
	{
		g_free(tags);
		tags = NULL;
		return;
	}
	
	gtk_tree_model_get (model,&iter,1,&tags,-1);
	printf("-%s--\n",tags);
	
	GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	gtk_entry_set_text (GTK_ENTRY(entry),tags);
	g_free(tags);
	tags = NULL;
}

void pull_ok_clicked_create_thread(GtkWidget *button, GtkBuilder *builder)
{	
	GtkWidget    *entry = NULL;
	const gchar* text  = NULL;

	entry = GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	text = gtk_entry_get_text (GTK_ENTRY(entry));
	
	if(!(text && *text))
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING , GTK_BUTTONS_OK,
               _("please input the name of image you want to download"));
		return ;
	}
	
	g_thread_new("ok_clicked", (GThreadFunc)pull_ok_clicked, builder);
}

gboolean message_dialog_create(gpointer data)
{
	gchar *str = (gchar *)data;
	message_dialog_new (NULL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, str);
	
	return FALSE;
}

gboolean set_window_false(gpointer data)
{
	GtkWidget *window1;

	window1 = (GtkWidget *)data;
	gtk_widget_set_sensitive (window1, FALSE);
	gtk_widget_realize (window1);
//	gdk_window_set_cursor (gdk_get_default_root_window (), gdk_cursor_new(GDK_WATCH));
	
	return FALSE;
}

gboolean set_window_true(gpointer data)
{
	GtkWidget *window = (GtkWidget *)data;
	gtk_widget_set_sensitive (window, TRUE);
//	gdk_window_set_cursor (gdk_get_default_root_window (), gdk_cursor_new(GDK_ARROW));
	return FALSE;
}

void pull_ok_clicked(gpointer data)
{
	GtkWidget     *pull_window = NULL;
	GtkBuilder    *builder     = NULL;
	GtkWidget     *entry       = NULL;
	GtkWidget     *window      = NULL;
	const         gchar *text = NULL;
	FILE          *fp          = NULL;
	gchar         *str         = NULL;
	gchar         *p           = NULL;  
	gchar search_result[4096] = {0};

	usleep(1000);
//	gdk_threads_enter ();

	builder     = (GtkBuilder  *)data;
	pull_window = GTK_WIDGET(gtk_builder_get_object (builder,"pull_window"));
	entry       = GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	text        = gtk_entry_get_text (GTK_ENTRY(entry));
	window      = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	g_idle_add(set_window_false, pull_window);
	g_idle_add(create_spinner , window);
	
	str = g_malloc(sizeof(gchar)*1000);
	sprintf(str,"sudo docker pull %s 2>&1; echo $?",text);

	if((fp = popen(str,"r"))==NULL)
	{
		g_critical("popen failed in %s in %d\n",__func__,__LINE__);
		g_idle_add (message_dialog_create, _("download failed，please try again!"));
		pclose(fp);
		g_free(str);
		g_idle_add(destroy_spinner , window);
		g_idle_add(set_window_true, pull_window);
//		gdk_threads_leave ();
		return;
	}

	while(fread(search_result, 1, sizeof(search_result), fp) != 0)
	{
		if(fread(search_result, 1, sizeof(search_result), fp) == 0)
			break;
	}
	pclose(fp);
	
	search_result[strlen(search_result) - 1] = '\0';
	p = &search_result[strlen(search_result)-1];
	
	if(*p != '0')
	{
		search_result[strlen(search_result) - 1] = '\0';
		g_idle_add (message_dialog_create, search_result);
		g_free(str);
		str = NULL;
		g_idle_add(destroy_spinner , window);
		g_idle_add(set_window_true, pull_window);
//		gdk_threads_leave ();
		return;
	}
	
	g_idle_add (message_dialog_create, _("downloag successfully，you could look for it outside"));
	g_idle_add(destroy_spinner , window);
	g_idle_add(set_window_true, pull_window);
	
//	gdk_threads_leave ();
}

void pull_cancel_clicked(GtkWidget *button, GtkWidget *window)
{
	gtk_widget_destroy (window);
	if(0 == access(SEARCH_FILE,F_OK))
		remove(SEARCH_FILE);
}

void pull_search_clicked(GtkWidget *button, GtkBuilder *builder)
{
	GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object (builder,"entry3"));
	const gchar* text = gtk_entry_get_text (GTK_ENTRY(entry));
	
	if(!(text && *text))
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, 
		                      _("please input the name of image you want to search first!"));
		return ;
	}
	
	FILE  *fp;
	gchar  search_result[4096] = {0};
	gchar *str                 = NULL;
	str = g_malloc(sizeof(gchar)*100);
	sprintf(str,"sudo docker search %s 2>&1;echo $?",text);

	if(NULL == (fp = popen(str,"r")))
	{
		PERROR("popen failed");
		g_free(str);
		str = NULL;
		return;
	}
	
	if(fread(search_result, 1, sizeof(search_result), fp) == 0)
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("fread failed, please try again!"));
		pclose(fp);
		return;
	}
	pclose(fp);
	
	search_result[strlen(search_result) - 1] = '\0';
	gchar *p = &search_result[strlen(search_result)-1];
	
	if(*p != '0')
	{
		search_result[strlen(search_result) - 1] = '\0';
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, search_result);
		g_free(str);
		str = NULL;
		return;
	}

	search_result[strlen(search_result) - 1] = '\0';
	FILE *search = NULL;
	search = fopen(SEARCH_FILE,"w");
	fwrite(search_result, 1, strlen(search_result), search);
	fclose(search);

	memset(str, 0, strlen(str));
	gchar line_result[64] = {0};
	sprintf(str,"cat search.txt | awk 'END{print NR}'");
	strcpy(line_result,popen_and_fread (str));
	gint total_line = atoi(line_result);
	
	g_free(str);
	str = NULL;

	gint x,y;
	x = (total_line-2)/9;
	y = (total_line-2)%9;
	if(y!=0)
		x++;

	current_page = 0;
	max_page = x;
	attach_widgets (builder, 0, x);
}

void attach_widgets (GtkBuilder *builder, gint show_page, gint total_page)
{
	gint x;
	pull_get_lines ();
	GtkTreeView  *view;
	GtkTreeModel *list;
	GtkLabel     *label;
	GtkTreeIter   iter;
	gchar *str = (gchar *)g_malloc(sizeof(gchar)*64);
	sprintf(str,"page %d/%d",show_page+1,total_page);
	printf("%d  %d\n",show_page,total_page);
	
	label = GTK_LABEL(gtk_builder_get_object (builder,"label6"));
	gtk_label_set_text (GTK_LABEL(label),str);
	view  = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	list  = GTK_TREE_MODEL(gtk_tree_view_get_model (view));
	gtk_list_store_clear (GTK_LIST_STORE(list));
	
	for(x=2; x<11; x++)
	{
		pull_set_page (GTK_LIST_STORE(list), iter, 9*show_page+x); 
	}
}

void pull_get_lines()
{
	gchar str[128]   = {0};
	gchar strnum[32] = {0};

	sprintf(str,"cat %s | sed -n \"1,1p\" | wc -L",SEARCH_FILE);
	strcpy(strnum,popen_and_fread (str));
	arr.line5 = atoi(strnum);
	
	memset(str,'\0',sizeof(str));
	sprintf(str,"cat %s | sed -n \"1,1p\" | awk -F \"D\" '{print $1}' | wc -L",SEARCH_FILE);
	memset(strnum, '\0', sizeof(strnum));
	strcpy(strnum,popen_and_fread (str));
	arr.line1 = atoi(strnum);

	memset(str,'\0',sizeof(str));
	sprintf(str,"cat %s | sed -n \"1,1p\" | awk -F \"S\" '{print $1,$2}' | wc -L",SEARCH_FILE);
	memset(strnum, '\0', sizeof(strnum));
	strcpy(strnum,popen_and_fread (str));
	arr.line2 = atoi(strnum);

	memset(str,'\0',sizeof(str));
	sprintf(str,"cat %s | sed -n \"1,1p\" | awk -F \"O\" '{print $1,$2}' | wc -L",SEARCH_FILE);
	memset(strnum, '\0', sizeof(strnum));
	strcpy(strnum,popen_and_fread (str));
	arr.line3 = atoi(strnum);
	
	memset(str,'\0',sizeof(str));
	sprintf(str,"cat %s | sed -n \"1,1p\" | awk -F \"A\" '{print $1,$2,$3,$4}' | wc -L",SEARCH_FILE);
	memset(strnum, '\0', sizeof(strnum));
	strcpy(strnum,popen_and_fread (str));
	arr.line4 = atoi(strnum);

	printf("%d %d %d %d %d\n",arr.line1,arr.line2,arr.line3,arr.line4,arr.line5);
}

void pull_set_page(GtkListStore *list, GtkTreeIter iter, gint x)
{
	gchar str[128]  = {0};
	gchar str1[128] = {0};
	gchar str2[128] = {0};	
	gchar str3[32]  = {0};
	gchar str4[32]  = {0};
	gchar str5[32]  = {0};
	gint  number    = x-1;
	
	memset(str,'\0',sizeof(str));
	sprintf(str, "cat %s | awk '{print $1}' | sed -n \"%d,1p\"",SEARCH_FILE,x);
	strcpy(str1,popen_and_fread (str));
	
	memset(str,'\0',sizeof(str));
	sprintf(str, "cat %s | sed -n \"%d,1p\"| cut -b %d-%d",SEARCH_FILE,x,arr.line1+1,arr.line2);  
	//注意：print和printf是有区别的，print自在换行
	strcpy(str2,popen_and_fread (str));
	
	memset(str,'\0',sizeof(str));
	sprintf(str, "cat %s | sed -n \"%d,1p\"| cut -b %d-%d",SEARCH_FILE,x,arr.line2+1,arr.line3); 
	strcpy(str3,popen_and_fread (str));
	
	memset(str,'\0',sizeof(str));
	sprintf(str, "cat %s | sed -n \"%d,1p\"| cut -b %d-%d",SEARCH_FILE,x,arr.line3+1,arr.line4); 
	strcpy(str4,popen_and_fread (str));
	
	memset(str,'\0',sizeof(str));
	sprintf(str, "cat %s | sed -n \"%d,1p\"| cut -b %d-%d",SEARCH_FILE,x,arr.line4+1,arr.line5); 
	strcpy(str5,popen_and_fread (str));

	gtk_list_store_append(list,&iter);
	gtk_list_store_set(list,&iter,0,number,1,str1,2,str2,3,str3,4,str4,5,str5,-1);
}

void image_image_event(GtkWidget *button, GtkWidget *window)
{
	GtkBuilder *builder;
	GtkWidget  *image_window;
	GtkWidget  *close_button ;
	GError     *error = NULL;

	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, IMAGE_IMAGE_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	image_window    = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	close_button    = GTK_WIDGET(gtk_builder_get_object (builder,"button"));

	gtk_window_set_position (GTK_WINDOW(image_window),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size (GTK_WINDOW(image_window),700,500);
	gtk_window_set_title (GTK_WINDOW(image_window),_("look up images"));
	image_treeview_set(builder);
	
	g_signal_connect(G_OBJECT(close_button),"clicked",G_CALLBACK(image_return),image_window);
	gtk_widget_show_all (image_window);
}

void image_return(GtkButton *button, GtkWidget *window)
{
	gtk_widget_destroy (window);
}

void image_treeview_set(GtkBuilder *builder)
{
	GtkTreeView *treeview;
	treeview   = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	
	tree_column_new(treeview, 0, _("NUMBER"));
	tree_column_new(treeview, 1, _("REPOSTORY"));
	tree_column_new(treeview, 2, _("TAGS"));
	tree_column_new(treeview, 3, _("IMAGE_ID"));
	tree_column_new(treeview, 4, _("CREATED"));
	tree_column_new(treeview, 5, _("SIZE"));
	
	add_tags_item(builder, SAVE_FLAG);
}

void image_rmi_event(GtkWidget *button, GtkWidget *window)
{   
	GtkBuilder *builder;
	GError     *error = NULL;

	builder = gtk_builder_new();
	if (!gtk_builder_add_from_file (builder, RMI_FILE, &error))
	{
		g_critical (_("Couldn't open remove builder file: %s"), error->message);
		g_error_free (error);
	}

	GtkWidget *rmi_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	GtkTreeView *treeview = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	GtkWidget *label1     = GTK_WIDGET(gtk_builder_get_object (builder,"label1"));
	GtkWidget *label3     = GTK_WIDGET(gtk_builder_get_object (builder,"label3"));
	GtkWidget *buttonbox  = GTK_WIDGET(gtk_builder_get_object (builder,"buttonbox2"));

	rmi_treeview_set(builder);
	g_signal_connect(G_OBJECT(treeview),"cursor-changed",G_CALLBACK(rmi_label_set),builder);
	gtk_label_set_text (GTK_LABEL(label1),_("please choose the image you want to delete"));
	gtk_label_set_text (GTK_LABEL(label3),_("the image you will delete:"));
	
	GtkWidget *ok_button = GTK_WIDGET(gtk_builder_get_object (builder,"button_ok"));
	GtkWidget *cancel_button = GTK_WIDGET(gtk_builder_get_object (builder,"button_cancel"));
	g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(rmi_ok_clicked),builder);
	g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(rmi_cancel_clicked),builder);

	gtk_window_set_position (GTK_WINDOW(rmi_window),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size (GTK_WINDOW(rmi_window),700,500);
	gtk_window_set_title (GTK_WINDOW(rmi_window), _("remove image"));
	gtk_widget_show_all(rmi_window);
	gtk_widget_hide(buttonbox);
}

void rmi_treeview_set(GtkBuilder *builder)
{		
	GtkTreeView *view = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	
	tree_column_new(view, 0, _("NUM"));
	tree_column_new (view, 1, _("IMAGE"));
	tree_column_new (view, 2, _("IMAGE ID"));

	add_tags_item(builder,RMI_FLAG);
}

void rmi_label_set(GtkTreeView *treeview, GtkBuilder *builder)
{
	GtkTreeIter       iter;
	GtkTreeModel     *model;
	GtkTreeSelection *selection;
	GtkWidget        *label;
	gchar            *image = NULL;
	
	gtk_tree_view_set_grid_lines(treeview,GTK_TREE_VIEW_GRID_LINES_BOTH);
	selection = GTK_TREE_SELECTION(gtk_tree_view_get_selection (treeview));
	label     = GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	image     = g_malloc(sizeof(gchar)*100);
	model     = gtk_tree_view_get_model (treeview);
	gtk_tree_selection_get_selected (selection,&model,&iter);

	if(!gtk_tree_model_get_iter(model,&iter,gtk_tree_model_get_path (model,&iter)))
		return;
	
	gtk_tree_model_get (model, &iter, 1, &image, -1);

	if(strcmp(image, "<none>:<none>") == 0)
	{
		memset(image, '\0', strlen(image));
		gtk_tree_model_get (model, &iter, 2, &image, -1);
	}

	gtk_label_set_text(GTK_LABEL(label),image);

	g_free(image);
}

void rmi_ok_clicked(GtkWidget *widget, GtkBuilder *builder)
{
	GtkWidget    *label = GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	const gchar *str   = gtk_label_get_text (GTK_LABEL(label));
	
	if(!(str && *str))
	{
		message_dialog_new(NULL, GTK_MESSAGE_ERROR,  
                          GTK_BUTTONS_OK, _("you have chosen none of image!"));
		return;
	}

	FILE *fp        = NULL;
    gchar str1[100] = {0};
	
    sprintf(str1,"sudo docker rmi -f %s",str);
    if(NULL == (fp = popen(str1,"r")))
	{
		PERROR("popen failed");
		pclose(fp);
		return;
	}

	message_dialog_new(NULL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
	                     _("the image you have chosen removes successfully ^_^"));

	pclose(fp);
	add_tags_item(builder,RMI_FLAG);
}

void rmi_cancel_clicked(GtkWidget *widget, GtkBuilder *builder)
{
	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	gtk_widget_destroy (GTK_WIDGET(window));
}

void image_save_event(GtkWidget *button, GtkWidget *window)
{
	GtkBuilder  *builder;
	GtkWidget   *save_window;
	GtkTreeView *treeview;
	GtkWidget   *ok_button ;
	GtkWidget   *cancel_button;
	GtkWidget   *label1;
	GtkWidget   *label2;
	GtkWidget   *label3;
	GtkWidget   *label4;
	GError      *error = NULL;

	Labels *labels = (Labels *)g_malloc(sizeof(Labels));
    labels->tags   = (gchar*)g_malloc(sizeof(gchar));

	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, SAVE_FILE, &error))
	{
		g_critical (_("Couldn't load builder file: %s"), error->message);
		g_error_free (error);
	}

	label1        = GTK_WIDGET(gtk_builder_get_object (builder,"label1"));
	label2        = GTK_WIDGET(gtk_builder_get_object (builder,"label2"));
	label3        = GTK_WIDGET(gtk_builder_get_object (builder,"label3"));
	label4        = GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	labels        = save_treeview_set(builder,labels);
	save_window   = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	ok_button     = GTK_WIDGET(gtk_builder_get_object (labels->builder,"button_ok"));
	treeview      = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	cancel_button = GTK_WIDGET(gtk_builder_get_object (labels->builder,"button_cancel"));

	gtk_label_set_text (GTK_LABEL(label1),_("please choose the image you want to save"));
	gtk_label_set_text (GTK_LABEL(label2),".tar");
	gtk_label_set_text (GTK_LABEL(label3),_("please input the name of image you want to save"));
	gtk_label_set_justify (GTK_LABEL(label3),GTK_JUSTIFY_CENTER);
	gtk_window_set_position (GTK_WINDOW(save_window),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size (GTK_WINDOW(save_window),700,500);
	gtk_window_set_title (GTK_WINDOW(save_window),_("save image"));

	g_signal_connect(G_OBJECT(treeview),"cursor-changed",G_CALLBACK(save_image_choose),labels);
	g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(save_ok_clicked),labels);
	g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(save_cancel_clicked),save_window);

	gtk_widget_show_all (save_window);
	gtk_widget_hide(label4);
}

Labels * save_treeview_set(GtkBuilder *builder, Labels *labels)
{
	GtkTreeView *treeview = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	add_tags_item(builder, SAVE_FLAG);

	labels->builder   = builder;
	labels->entry     = GTK_ENTRY(gtk_builder_get_object (builder,"entry1"));
	labels->selection = GTK_TREE_SELECTION(gtk_tree_view_get_selection (treeview));
	labels->label     = GTK_LABEL(gtk_builder_get_object (builder,"choose_image_label"));
	
	if(labels->tags != NULL)
    {
       g_free(labels->tags);
       labels->tags = NULL;
	}
	
	tree_column_new(treeview, 0, _("NUMBER"));
	tree_column_new(treeview, 1, _("REPOSTORY"));
	tree_column_new(treeview, 2, _("TAGS"));
	tree_column_new(treeview, 3, _("IMAGE_ID"));
	tree_column_new(treeview, 4, _("CREATED"));
	tree_column_new(treeview, 5, _("SIZE"));
    
	return labels;
}

void save_image_choose(GtkWidget *widget, Labels *labels)
{
	GtkTreeView  *treeview = GTK_TREE_VIEW(widget);
	GtkTreeIter   iter;
	GtkTreeModel *model;
	gchar        *repository = NULL;
	gchar        *tags = NULL;
	gchar        *image_name = NULL;
	
	image_name   = (gchar *)g_malloc(sizeof(gchar)*100);
	labels->tags = (gchar *)g_malloc(sizeof(gchar)*100);
	model        = gtk_tree_view_get_model (treeview);
	
	gtk_tree_selection_get_selected (GTK_TREE_SELECTION(labels->selection),&model,&iter);
	gtk_tree_model_get (model,&iter,1,&repository,2,&tags,-1);

	sprintf(image_name,"%s:%s",repository,tags);
	sprintf(labels -> tags, "%s", image_name); 
	image_name = g_strdup_printf (_("the image you have chosen:<b>%s</b>"), image_name);
	gtk_label_set_markup (GTK_LABEL (labels->label),image_name);
	
	g_free(image_name);
}

void save_ok_clicked(GtkWidget *button, Labels *labels)
{	
	const gchar *text     = NULL;
	gchar        *tags     = labels->tags;
	gchar         str[100] = {0};
	
	text = gtk_entry_get_text (GTK_ENTRY(labels->entry));
	if(!(text && *text))
	{
		message_dialog_new(NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, 
		                     _("please input the name of image you want to save first"));
		return;
	}

	if(!(tags && *tags))
	{
		message_dialog_new(NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, 
		                     _("you have chosen none of image!"));
		return;
	}

	/* 将镜像保存到指定目录，如果不存在就创建该目录 */
	if(NULL == opendir("src/image_tar"))
		mkdir("src/image_tar", 0755);
	
	else
	{
		GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (labels->builder,"window"));

		FILE *fp = NULL;
		sprintf(str,"sudo docker save -o %s/%s.tar %s",LOAD_AND_SAVE_PATH,text,tags);
		if(NULL == (fp = popen(str,"r")))
		{
			PERROR("popen failed");
			pclose(fp);
			return;
		}

		pclose(fp);
		message_dialog_new(NULL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
		                     _("save successfully ^_^"));
		gtk_widget_destroy (GTK_WIDGET(window));
	}
}

void save_cancel_clicked(GtkWidget *button, GtkWidget *window)
{	
	gtk_widget_destroy (window);
}


void get_item_row_line(gint x, gint y, gchar **dest, gint flag)
{
	gchar str[100]    = {0}; 
	gchar result[100] = {0};
	memset(result, '\0', sizeof(result));
	*dest = (gchar *)g_malloc(sizeof(gchar)*100);

	if(y==4)
		sprintf(str,"cat tmp.txt | sed -n '%d,1p' | awk '{print$4,$5,$6}'",x);
	else if(y==5)
		sprintf(str,"cat tmp.txt | sed -n '%d,1p' | awk '{print$7,$8}'",x);
	else if(flag == SAVE_FLAG)
		sprintf(str,"cat tmp.txt | sed -n '%d,1p' | awk '{print$%d}'",x,y);
	else if(y==3)
		sprintf(str,"cat tmp.txt | sed -n '%d,1p' | awk '{print$%d}'",x,y);
	else
		sprintf(str,"cat tmp.txt | sed -n '%d,1p' | awk '{print$1\":\"$2}'",x);

	strcpy(result,popen_and_fread (str));
	strcpy(*dest,result);
}

gint get_item_n_row()
{  
    gchar str[100]   = {0};
	gchar result[32] = {0};
	
    memset(str,'\0',sizeof(str));
    sprintf(str,"sudo docker images | awk 'END{print NR}'");

	strcpy(result,popen_and_fread(str));
    gint num = atoi(result);

	return num;
}

void add_tags_item(GtkBuilder *builder, gint flag)
{
	GtkTreeIter   iters;
	GtkListStore *list;
	Item          item;
	gint          row;
	gint          total_row;	
	FILE         *fp_write;
	gchar         txt[2048]={0};
	gchar         str[100] ={0}; 
	
	list      = GTK_LIST_STORE(gtk_builder_get_object (builder,"liststore"));
	gtk_list_store_clear (list);
	total_row = get_item_n_row ();
	
	sprintf(str,"sudo docker images");
	fp_write = fopen("tmp.txt","w");
	
	memset(txt,'\0',sizeof(txt));
	strcpy(txt,popen_and_fread (str));
	fwrite(txt,1,sizeof(txt),fp_write);
	fclose(fp_write);

	if(flag == SAVE_FLAG)
	{
		for(row=2; row<=total_row; row++)
		{
			item.number = row - 1; 
			get_item_row_line(row,1,&item.repository,flag);
			get_item_row_line(row,2,&item.tags,flag);
			get_item_row_line(row,3,&item.image_id,flag);
			get_item_row_line(row,4,&item.created,flag);
			get_item_row_line(row,5,&item.size,flag);
	
			gtk_list_store_append (list, &iters);
			gtk_list_store_set (GTK_LIST_STORE (list), &iters,
    		          		0, item.number,
       		        		1, item.repository,
		                    2, item.tags,
		                    3, item.image_id,
		                    4, item.created,
		                    5, item.size,
        		      		-1);
		}
	}

	if(flag == RMI_FLAG)
	{
		for(row=2; row<=total_row; row++)
		{	
			item.number = row - 1; 
			get_item_row_line(row,2,&item.tags,flag);
			get_item_row_line(row,3,&item.image_id,flag);
		
			gtk_list_store_append (list, &iters);
			gtk_list_store_set (GTK_LIST_STORE (list), &iters,
    		          		0, item.number,
       		        		1, item.tags,
			                2, item.image_id,
        		      		-1);
		}
	}
	remove("tmp.txt");
}

void image_load_event(GtkWidget *button, GtkWidget *window)
{
	gchar        str[64] = {0};
	GtkBuilder  *builder;
	GtkWidget   *load_window;
	GError      *error = NULL;
	GtkWidget   *box;
	GtkWidget   *label1;
	GtkWidget   *label3;
	GtkWidget   *label4;
	GtkTreeView *view;
	GtkWidget   *ok_button;
	GtkWidget   *cancel_button;
	
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, LOAD_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	load_window   = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	label1        = GTK_WIDGET(gtk_builder_get_object (builder,"label1"));
	label3        = GTK_WIDGET(gtk_builder_get_object (builder,"label3"));
	label4        = GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	view          = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	ok_button     = GTK_WIDGET(gtk_builder_get_object (builder,"button_ok"));
	cancel_button = GTK_WIDGET(gtk_builder_get_object (builder,"button_cancel"));
	box           = GTK_WIDGET(gtk_builder_get_object (builder,"buttonbox2"));
	
	gtk_window_set_position (GTK_WINDOW(load_window),GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(load_window),_("load image"));
	gtk_window_set_default_size (GTK_WINDOW(load_window),700,500);
	
	gtk_label_set_text (GTK_LABEL(label1),_("please choose the image you want to load"));
	gtk_label_set_text (GTK_LABEL(label3),_("the image you have chosen: "));
	gtk_label_set_justify (GTK_LABEL(label3),GTK_JUSTIFY_CENTER);

	g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(load_ok_clicked),builder);
	g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(load_cancel_clicked),load_window);
	g_signal_connect(G_OBJECT(view),"cursor-changed",G_CALLBACK(load_tar_choose),builder);
	
	load_treeview_set(builder);
	
	gtk_widget_show_all (load_window);
	gtk_widget_hide (label4);
	gtk_widget_hide (box);

	sprintf(str,"find src/image_tar -name \"*.tar\" | awk 'END{print NR}'");
	if(NULL == opendir("src/image_tar") || 0 == strcmp("0", popen_and_fread (str)))
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("have no tar to load，please create it first！"));
}

void load_treeview_set(GtkBuilder *builder)
{   
	GtkTreeView *view;
	view = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	
	tree_column_new(view, 0, _("Num"));
	tree_column_new (view, 1, _("Tar"));
	
	load_add_tar_item(builder);
}

void load_tar_choose(GtkTreeView *treeview, GtkBuilder *builder)
{
	GtkWidget        *label;
	GtkTreeIter       iter;
	GtkTreeModel     *model;
	GtkTreeSelection *selection;
	gchar            *tar  = NULL;
	gchar            *text = NULL;
	
	text = (gchar *)g_malloc(sizeof(gchar)*100);
	tar = (gchar *)g_malloc(sizeof(gchar)*100);

	selection = GTK_TREE_SELECTION(gtk_builder_get_object (builder,"treeview-selection1"));
	model     = gtk_tree_view_get_model (treeview);
	gtk_tree_selection_get_selected (selection,&model,&iter);
	gtk_tree_model_get (model,&iter,1,&tar,-1);

	sprintf(text,_("the tar you have chosen :<b>%s</b>"),tar);
//image_name = g_strdup_printf ("the image you have chosen :<b>%s</b>", image_name);
	
	label = GTK_WIDGET(gtk_builder_get_object (builder,"label3"));
	gtk_label_set_markup (GTK_LABEL (label),text);
}   


void load_ok_clicked(GtkWidget *button, GtkBuilder *builder)
{
	GtkWidget   *label;
	gchar        tar[50]  = {0};
	const gchar *text    = NULL;
	FILE         *fp      = NULL;
	gchar        str[100] = {0};
	GtkWidget   *load_window;

	label = GTK_WIDGET(gtk_builder_get_object (builder,"label3"));
	text = gtk_label_get_text (GTK_LABEL(label));
	sscanf(text,"%*[^:]:%s",tar);  

	if(strlen(tar) == 0)
	{
		message_dialog_new (NULL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
		                      _("you have no tar to choose!"));
		return;
	}

	memset(str,'\0',sizeof(str));
	sprintf(str,"sudo docker load --input %s/%s",LOAD_AND_SAVE_PATH,tar);
	
	if(NULL == (fp=popen(str,"r")))
	{
		pclose(fp);
		PERROR("popen failed");
		return;
	}

	pclose(fp);
	message_dialog_new (NULL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
                 	    _("load succussfully ^_^"));

	load_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	gtk_widget_destroy (load_window);
}


void load_cancel_clicked(GtkWidget *button, GtkWidget *window)
{
	gtk_widget_destroy (window);
}

void load_add_tar_item(GtkBuilder *builder)
{
	gchar         result[100] = {0};
	gchar         str[100]    = {0};
	GtkTreeIter   iters;
	GtkListStore *list;
	Item         *item;
	gint          num;
	gint          x;

	list = GTK_LIST_STORE(gtk_builder_get_object (builder,"liststore"));
	item = (Item *)malloc(sizeof(Item));
	
	sprintf(str,"find %s -name \"*.tar\" | awk 'END{print NR}' ",LOAD_AND_SAVE_PATH); 
	strcpy(result,popen_and_fread (str));
	num = atoi(result);

	for(x=0; x<num; x++)
	{
		memset(str,'\0',sizeof(str));
		sprintf(str,"find %s -name \"*.tar\" | awk -F \"/\" '{print $3}' | sed -n \"%d,1p\" ",LOAD_AND_SAVE_PATH,x+1); 
		memset(result,'\0',sizeof(result));
		strcpy(result,popen_and_fread (str));
		
		item->number = x+1;
		item->tags = result; 
		
		gtk_list_store_append (list, &iters);
		gtk_list_store_set (GTK_LIST_STORE (list), &iters,
    		          		0, item->number,
       		        		1, item->tags,
        		      		-1);
	}
	free(item);
}

void image_tag_event(GtkWidget *button, GtkWidget *window)
{
	GtkBuilder  *builder;
	GError      *error = NULL;
	GtkWidget   *tag_window;
	GtkTreeView *view;
	GtkWidget   *label1;
	GtkWidget   *label2;
	GtkWidget   *label3;
	GtkWidget   *label_tip;
	GtkWidget   *ok_button;
	GtkWidget   *cancel_button;
		
	builder = gtk_builder_new();
	
	if(!gtk_builder_add_from_file (builder,TAG_FILE,&error))
	{
		g_critical("the pull_file opened failed %s\n",error->message);
		g_error_free (error);
	}
	
	tag_window    = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	label1        = GTK_WIDGET(gtk_builder_get_object(builder,"label1"));
	label2        = GTK_WIDGET(gtk_builder_get_object(builder,"label2"));
	label3        = GTK_WIDGET(gtk_builder_get_object(builder,"label3"));
	label_tip     = GTK_WIDGET(gtk_builder_get_object(builder,"choose_image_label"));
	view          = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	ok_button     = GTK_WIDGET(gtk_builder_get_object (builder,"button_ok"));
	cancel_button = GTK_WIDGET(gtk_builder_get_object (builder,"button_cancel"));

	gtk_widget_set_name (tag_window, "tag_window");
	gtk_window_set_position (GTK_WINDOW(tag_window),GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(tag_window),_("tag image"));
	gtk_window_set_default_size (GTK_WINDOW(tag_window),700,500);
	gtk_label_set_text (GTK_LABEL(label1),_("please tag the image you want to choose"));
	gtk_label_set_text (GTK_LABEL(label2),_("input the name of new image when you have chosen the image, separated by a colon(name:tag)"));
	gtk_label_set_text (GTK_LABEL(label3),_("the image you have chosen: "));
	gtk_label_set_text (GTK_LABEL(label_tip), _("Note: if you have taged successfully, it will not cover the original image, but create a new image you have named "));
	
	rmi_treeview_set(builder);
	
	g_signal_connect(G_OBJECT(view),"cursor-changed",G_CALLBACK(rmi_label_set),builder);
	g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(tag_ok_clicked),builder);
	g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(tag_cancel_clicked),builder);
	
	gtk_widget_show_all(tag_window);
}

void tag_ok_clicked(GtkWidget *widget, GtkBuilder *builder)
{
	GtkWidget    *label;
	const gchar *label_text = NULL;
	GtkWidget    *entry;
	const gchar *entry_text = NULL;
	FILE         *fp = NULL;
    gchar         str[100] = {0};
	gchar         result[1024] = {0};

	label      = GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	label_text = gtk_label_get_text (GTK_LABEL(label));
	
	if(!(label_text && *label_text))
	{
		message_dialog_new(NULL, GTK_MESSAGE_ERROR,  
                          GTK_BUTTONS_OK, _("you have chosen none of image!"));
		return;
	}

	entry = GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	entry_text = gtk_entry_get_text (GTK_ENTRY(entry));
	
	if(!(entry_text && *entry_text))
	{
		message_dialog_new(NULL, GTK_MESSAGE_ERROR,  
                          GTK_BUTTONS_OK, _("pleage input the name of new image!"));
		return;
	}

    sprintf(str,"sudo docker tag %s %s 2>&1;echo $?",label_text,entry_text);
	printf("--%s---\n",str);
	
    if(NULL == (fp = popen(str,"r")))
	{
		PERROR("popen failed");
		pclose(fp);
		return;
	}

	if(fread(result, 1, sizeof(result), fp) == 0)
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("fread failed, please try again!"));
		pclose(fp);
		return;
	}
	pclose(fp);
	result[strlen(result) - 1] = '\0';
	gchar *p = &result[strlen(result)-1];
	
	if(*p != '0')
	{
		result[strlen(result) - 1] = '\0';
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, result);
		return;
	}

	message_dialog_new(NULL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
	                     _("tag successfully ^_^"));

	tag_cancel_clicked (NULL,builder);
}

void tag_cancel_clicked(GtkWidget *widget, GtkBuilder *builder)
{
	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	gtk_widget_destroy (GTK_WIDGET(window));
}

void image_commit_event(GtkWidget *button, GtkWidget *window)
{
	GtkBuilder  *builder;
	GError      *error = NULL;
	GtkWidget   *commit_window;
	GtkTreeView *view;
	GtkWidget   *label1;
	GtkWidget   *label2;
	GtkWidget   *label3;
	GtkWidget   *label4;
	GtkWidget   *label5;
	GtkWidget   *ok_button;
	GtkWidget   *cancel_button;

	builder = gtk_builder_new();

	if(!gtk_builder_add_from_file (builder,COMMIT_FILE,&error))
	{
		g_critical(_("the pull_file opened failed %s\n"),error->message);
		g_error_free (error);
	}
	
	commit_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	view          = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	label1        = GTK_WIDGET(gtk_builder_get_object(builder,"label1"));
	label2		  = GTK_WIDGET(gtk_builder_get_object(builder,"label2"));
	label3	      = GTK_WIDGET(gtk_builder_get_object(builder,"label3"));
	label4		  = GTK_WIDGET(gtk_builder_get_object(builder,"label6"));
	label5		  = GTK_WIDGET(gtk_builder_get_object(builder,"label5"));
	ok_button     = GTK_WIDGET(gtk_builder_get_object (builder,"button_ok"));
	cancel_button = GTK_WIDGET(gtk_builder_get_object (builder,"button_cancel"));


	gtk_window_set_position (GTK_WINDOW(commit_window),GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(commit_window), _("commit image"));
	gtk_window_set_default_size (GTK_WINDOW(commit_window),700,500);
	gtk_label_set_text (GTK_LABEL(label1), _("please choose the container that in order to commit new image"));
	gtk_label_set_text (GTK_LABEL(label2), _("please input the author(optonal)"));
	gtk_label_set_text (GTK_LABEL(label3), _("please input the message(optional)"));
	gtk_label_set_text (GTK_LABEL(label4), _("please input the name of new image, separated by colon(name:tag)"));
	gtk_label_set_text (GTK_LABEL(label5), _("the container you have chosen"));
	gtk_label_set_justify (GTK_LABEL(label4), GTK_JUSTIFY_CENTER);
	
	commit_treeview_set(builder);

	g_signal_connect(G_OBJECT(view),"cursor-changed",G_CALLBACK(rmi_label_set),builder);
	g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(commit_ok_clicked),builder);
	g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(commit_cancel_clicked),builder);
	
	gtk_widget_show_all(commit_window);
}

void commit_treeview_set(GtkBuilder *builder)
{
	GtkTreeView  *view;
	GtkTreeIter   iter;
	GtkListStore *list;
	gint          j;
	
	view = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview"));
	tree_column_new(view,  0, _("Num"));
	tree_column_new (view, 1, _("Name"));
	tree_column_new (view, 2, _("ID"));
	tree_column_new (view, 3, _("Image"));
	docker_contain ();

	list = gtk_list_store_new (4, G_TYPE_INT,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING);
	gtk_tree_view_set_model (view,GTK_TREE_MODEL(list));
	
	for(j=0; j<contains_num; j++)
	{
		gtk_list_store_append (list,&iter);
		gtk_list_store_set (list, &iter, 0, j+1, -1);
		gtk_list_store_set (list, &iter, 1, &a[j].name, -1);
		gtk_list_store_set (list, &iter, 2, &a[j].id, -1);
		gtk_list_store_set (list, &iter, 3, &a[j].image, -1);
	}
}

void commit_ok_clicked(GtkWidget *widget, GtkBuilder *builder)
{
	const gchar *author_text    = NULL;   
	const gchar *commit_text    = NULL;    
	const gchar *new_image_text = NULL;  
	const gchar *image_text     = NULL;   
	gchar         str[1024]     = {0};
	GtkWidget    *author_entry; 
	GtkWidget    *commit_entry;   
	GtkWidget    *new_image_entry; 
	GtkWidget    *image_label;     


	author_entry    = GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	commit_entry    = GTK_WIDGET(gtk_builder_get_object (builder,"entry2"));
	new_image_entry = GTK_WIDGET(gtk_builder_get_object (builder,"entry3"));
	image_label     = GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	author_text     = gtk_entry_get_text (GTK_ENTRY(author_entry));
	commit_text     = gtk_entry_get_text (GTK_ENTRY(commit_entry));
	new_image_text  = gtk_entry_get_text (GTK_ENTRY(new_image_entry));
	image_text      = gtk_label_get_text (GTK_LABEL(image_label));
	
	sprintf(str,"sudo docker commit");
	
	if(author_text && *author_text)
		sprintf(str,"%s -a %s",str,author_text);

	if(commit_text && *commit_text)
		sprintf(str,"%s -m %s",str,commit_text);

	if(!(image_text && *image_text))
	{
		message_dialog_new(NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, 
		                     _("please choose the container you want to commit!"));
		return;
	}
	sprintf(str,"%s %s",str,image_text);
	
	if(!(new_image_text && *new_image_text))
	{
		message_dialog_new(NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, 
		                     _("please input the name of new image, separated by colon(name:tag)"));
		return;
	}
	sprintf(str,"%s %s",str,new_image_text);
	printf("%s\n",str);

	FILE *fp = NULL;
	gchar result[1024] = {0};
	sprintf(str,"%s 2>&1; echo $?",str);
    if(NULL == (fp = popen(str,"r")))
	{
		PERROR("popen failed");
		pclose(fp);
		return;
	}

	if(fread(result, 1, sizeof(result), fp) == 0)
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("fread failed, please try again!"));
		pclose(fp);
		return;
	}
	pclose(fp);
	result[strlen(result) - 1] = '\0';
	gchar *p = &result[strlen(result)-1];
	
	if(*p != '0')
	{
		result[strlen(result) - 1] = '\0';
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, result);
		return;
	}

	message_dialog_new(NULL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
	                     _("commit succussfully ^_^\n"));
	commit_cancel_clicked(NULL,builder);
}

void commit_cancel_clicked(GtkWidget *widget, GtkBuilder *builder)
{
	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	gtk_widget_destroy (GTK_WIDGET(window));
}

gboolean create_spinner(gpointer data)
{
	GtkWidget *window; 
	GtkSpinner *spinner;

	window = (GtkWidget *)data; 
	
	gtk_window_set_default_size(GTK_WINDOW(window),200,200);
	gtk_window_set_position (GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_decorated (GTK_WINDOW(window), FALSE);
	gtk_widget_set_opacity (window, 1.0);
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);
	
	spinner = GTK_SPINNER(gtk_spinner_new());
	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(spinner));
	gtk_spinner_start (spinner);
	gtk_widget_show_all(window);
	
	return FALSE;
}

gboolean destroy_spinner(gpointer data)
{
	GtkWidget *window = (GtkWidget *)data;
	gtk_widget_destroy (window);
	return FALSE;
}

