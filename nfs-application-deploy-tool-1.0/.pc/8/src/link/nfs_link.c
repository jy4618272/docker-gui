#include "global/global.h"
#include "nfs_link.h"

static gint link_nm=0;
static gint link_contains_nm=0;

GtkWidget *contains_link_entry11,
          *contains_link_entry22,
          *contains_link_entry33;

GtkWidget *combox33,*combox22;

//GtkWidget *entry_combo;
static struct docker_images{
	char name[100];
	char tag[100];
};

static char link_image_tag[100]={0};//存镜像的的信息
static char link_contains_id[100]={0};//存容器的id
static char my_contains_name[100]={0};//存储自定义容器的名字
static char my_link_contains_name[100]={0};//存储连接容器的别名
static char my_link_contains_do[100]={0};//链接容器的动作参数

static struct docker_images view_link[1024]={0};
static struct docker_contains_link{
		char name[100];
		char  ID[100];
};

static struct docker_contains_link contians_link[1024]={0};
void combox22_change_fun(GtkWidget *window,gpointer data)
{
	char *row_image,*row_tags;
	GtkTreeModel *model_link;
	GtkTreeIter iter_link;
	memset(link_image_tag,'\0',sizeof(link_image_tag));
	model_link=gtk_combo_box_get_model (window);
	gtk_combo_box_get_active_iter(window, &iter_link);
	gtk_tree_model_get (model_link,&iter_link,0,&row_image,-1);
	gtk_tree_model_get (model_link,&iter_link,1,&row_tags,-1);
	sprintf(link_image_tag,"%s:%s",row_image,row_tags);
	//printf("%s %s %s\n",row_image,row_tags,link_image_tag);
	//printf("combox22\n");

}
void combox33_change_fun(GtkWidget *window,gpointer data)
{
		memset(link_contains_id,'\0',sizeof(link_contains_id));
		static char *row_name=NULL;
		static char *row_id=NULL;
		GtkTreeIter iter_c;
		GtkTreeModel *model_c;
		model_c=gtk_combo_box_get_model (window);
		gtk_combo_box_get_active_iter (window,&iter_c);
		gtk_tree_model_get(model_c,&iter_c,0,&row_name,-1);
		gtk_tree_model_get(model_c,&iter_c,1,&row_id,-1);
		sprintf(link_contains_id,"%s",row_id);
		//printf("%s\n",link_contains_id);
	//printf("333\n");
}
void link_contains_fun(gpointer data)
{
	static  char *tmp_link=NULL;
	static char buf[1024]={0};
	pid_t pid;
	static FILE *fd_link=NULL;
	tmp_link=(char *)malloc(sizeof(char)*1024);
	gdk_threads_enter ();
	
sprintf(tmp_link,"sudo gnome-terminal -x  bash -c \"docker start %s && docker run -ti --rm --name %s --link %s:%s %s %s 2>&1\"",link_contains_id,my_contains_name,link_contains_id,
	    												my_link_contains_name,link_image_tag,my_link_contains_do);

	pid=system(tmp_link);
	if(WIFEXITED(pid))
	{
		printf("111\n");

	}
	/*fd_link=popen(tmp_link,"r");
	while(fgets(buf,1024,fd_link)!=NULL)
	{
	printf("bbb\n");
	printf("%s\n",buf);
	}
	if(fd_link ==NULL){
		printf("fail\n");
	}
	
	printf("%s\n",tmp_link);
	
	//VV=vte_terminal_new();
	*/
	gdk_threads_leave ();
	g_free (tmp_link);
	//pclose (fd_link);
	printf("fff\n");
}
void button_link_fun(GtkWidget *window,gpointer data)
{
	static char *p=NULL;
	static char *tmp=NULL;
	static char *tmp1=NULL;
	if(link_image_tag[0]=='\0'){
		contains_message(GTK_MESSAGE_ERROR,_("image not is empty"));
		return;
		
	}
	if(link_contains_id[0]=='\0'){
		contains_message(GTK_MESSAGE_ERROR,_("contain is not empty"));
		return;
		
	}
	p=gtk_entry_get_text (contains_link_entry11);
	tmp=gtk_entry_get_text (contains_link_entry22);
	tmp1=gtk_entry_get_text (contains_link_entry33);
	sprintf(my_contains_name,"%s",p);
	sprintf(my_link_contains_name,"%s",tmp);
	sprintf(my_link_contains_do,"%s",tmp1);
	printf("%s %s %s\n",my_contains_name,my_link_contains_name,my_link_contains_do);
	//printf("%s\n",gtk_entry_get_text (contains_link_entry11));
	if(my_contains_name[0]=='\0')
	{
		contains_message(GTK_MESSAGE_ERROR,_("Custom container name can not be empty"));
			return;

	}
	if(my_link_contains_name[0]=='\0')
	{
		contains_message(GTK_MESSAGE_ERROR,_("link contains is not empoty"));
		return;

	}
	
	//printf("%s %s\n",link_image_tag,link_contains_id);
		//printf("%s\n",tmp11);
	g_thread_new ("c_link",link_contains_fun,tmp1);
		
}
void create_link_contains()
{
	GtkWidget *window;
	GtkWidget *combox;
	
	GtkBuilder *builder;
	builder=gtk_builder_new ();
	GtkWidget *label11;
	GtkWidget *label22;
	GtkWidget *label33;
	GtkWidget *label44;
	GtkWidget *button_link;
	FILE      *fd_link = NULL,
	          *fd_contains_link = NULL;
	GtkCellRenderer   *renderer,
                   *renderer11;
	GtkTreeIter iter;
	static char buf_link[1024]={0};
	link_nm=0;
	memset(link_image_tag,'\0',sizeof(link_image_tag));
	memset(link_contains_id,'\0',sizeof(link_contains_id));
	memset(my_contains_name,'\0',sizeof(my_contains_name));
	memset(my_link_contains_name,'\0',sizeof(my_link_contains_name));
	memset(my_link_contains_do,'\0',sizeof(my_link_contains_do));
	//buf_link=(char *)malloc(sizeof (char)*1024);
	gtk_builder_add_from_file (builder,DOCKER_LINK_CONTAINS,NULL);
	window=GTK_WIDGET(gtk_builder_get_object (builder,DOCKER_LINK_WINDOW ));
	label11=GTK_WIDGET(gtk_builder_get_object (builder,"label1"));
	label22=GTK_WIDGET(gtk_builder_get_object (builder,"label2"));
	label33=GTK_WIDGET(gtk_builder_get_object (builder,"label3"));
	label44=GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	gtk_window_set_title (window,_("contain connect"));
	contains_link_entry11=GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	//gtk_entry_set_text (contains_link_entry11,"sadsadasd");
	contains_link_entry22=GTK_WIDGET(gtk_builder_get_object (builder,"entry2"));
	contains_link_entry33=GTK_WIDGET(gtk_builder_get_object (builder,"entry3"));
	//entry_combo=GTK_WIDGET(gtk_builder_get_object (builder,"combobox-entry1"));
	//gtk_entry_set_text (entry_combo,"33333");
	button_link=GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	combox33=GTK_WIDGET(gtk_builder_get_object (builder,"combobox3"));
	combox22=GTK_WIDGET(gtk_builder_get_object (builder,"combobox2"));
	gtk_label_set_text (GTK_LABEL(label11),_("please select image name"));
	gtk_label_set_text (GTK_LABEL(label22),_("please input yourself contains name"));
	gtk_label_set_text (GTK_LABEL(label33),_("please select link contain name"));
	gtk_label_set_text (GTK_LABEL(label44),_("please select link contain alias name"));
	gtk_button_set_label (GTK_BUTTON(button_link),_("sure"));
	/* ........................................................................*/
	fd_link=popen("docker images","r");
	      
        if(fgets(buf_link,1000,fd_link) == NULL)
        {
		button33_fun (NULL,NULL);
		
        }
	GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
		//GtkListStore *store22 = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(combox22, renderer,FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combox22), renderer,
                             "text",0,
                               NULL);
	renderer11 = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(combox22, renderer11,TRUE);
	gtk_cell_layout_set_attributes(combox22, renderer11,
                             "text",1,
                               NULL);
	
	while(fgets(buf_link,1024,fd_link))
{
		//printf("%s\n",buf_link);
		if(sscanf(buf_link,"%s  %s",view_link[link_nm].name,view_link[link_nm].tag) < 2)
		{
        printf("docker Containerization init fale\n");

		}
		//printf("%s %s\n",view_link[link_nm].name,view_link[link_nm].tag);
		//gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combox22),"view_link[link_nm].name	    qqq" );
		//gtk_combo_box_text_prepend (GTK_COMBO_BOX_TEXT(combox22),"400","sssss");
		gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,0, view_link[link_nm].name,1,view_link[link_nm].tag,-1);
		link_nm++;
}
		gtk_combo_box_set_model(combox22,store);
/*........................................................................................*/
	//gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combox),view_link[].name);
		//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combox), "caterpillar2");
		//gtk_combo_box_text_append_text(GTK_COMBO_BOX(combox22), "caterpillar3");
	//g_signal_connect (G_OBJECT(create_contians_view_entry11), "insert-text", G_CALLBACK (on_entry_insert_text), NULL);
	fd_contains_link=popen("docker ps -a","r");
	char *buf_contains_link;
	buf_contains_link=(char *)malloc(sizeof(buf_contains_link)*1024);
	if(fgets(buf_contains_link,1024,fd_contains_link)==NULL){

		button33_fun(NULL,NULL);
	}
	
	GtkListStore *list_contains;
	list_contains=gtk_list_store_new (2,G_TYPE_STRING,G_TYPE_STRING);
	GtkCellRenderer *cell_contains=gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (combox33,cell_contains,TRUE);
	gtk_cell_layout_set_attributes(combox33,cell_contains,
                             "text",0,
                               NULL);
        GtkCellRenderer *cell_contains11=gtk_cell_renderer_text_new ();
        gtk_cell_layout_pack_start (combox33,cell_contains11,TRUE);
	gtk_cell_layout_set_attributes(combox33,cell_contains11,
                             "text",1,
                               NULL);
//gtk_combo_box_set_model (combox33,list_contains);


			while(fgets(buf_contains_link,1024,fd_contains_link)!=NULL){
				sscanf(buf_contains_link,"%s",contians_link[link_contains_nm].ID);
				sscanf(&buf_contains_link[strlen(buf_contains_link)-21],"%s",contians_link[link_contains_nm].name);
				gtk_list_store_append (list_contains,&iter);
				gtk_list_store_set(list_contains, &iter,1, contians_link[link_contains_nm].ID,0,contians_link[link_contains_nm].name,-1);
				//printf("3333%s   %s\n",contians_link[link_contains_nm].name,contians_link[link_contains_nm].ID);
			link_contains_nm++;
	}
		gtk_combo_box_set_model (combox33,list_contains);
		//int i;
	/*for(i=0;i<4;i++){
	gtk_list_store_append(list_contains, &iter);
	gtk_list_store_set(list_contains, &iter,0, "name",1,"tag",-1);
	}
*/
	g_signal_connect(G_OBJECT(combox22),"changed",G_CALLBACK(combox22_change_fun),NULL);
	g_signal_connect(G_OBJECT(button_link),"clicked",G_CALLBACK(button_link_fun),NULL);
	g_signal_connect(G_OBJECT(combox33),"changed",G_CALLBACK(combox33_change_fun),NULL);
	pclose (fd_link);
	g_object_unref (builder);
	gtk_widget_show_all (window);



}
