
#include "global/global.h"
#include "nfs_plink.h"

GtkWidget *p_combox;

static struct p_link_image
{
	char p_name[100];
	char p_tag[100];
};

static struct p_link_image image_list[1024]={0,};
static char p_link_combo_data[1024]={0,};

GtkWidget *p_entry = NULL;
GtkWidget *p_entry_con = NULL;

static gint p_num=0;

void add_data_combox(GtkWidget *p_combox)//往combox添加镜像的数据
{
	GtkListStore *p_store;
	GtkTreeIter iter;
	static FILE *p_fd=NULL;
	static char *p_buf=NULL;
	p_buf=(char *)malloc(sizeof(char)*1024);
	memset(p_buf,'\0',sizeof(p_buf));
	p_store=gtk_list_store_new (2,G_TYPE_STRING,G_TYPE_STRING);
	GtkCellRenderer *p_cell,*p_cell2;
	p_cell=gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (p_combox,p_cell,TRUE);
	gtk_cell_layout_set_attributes(p_combox,p_cell,"text",0,NULL);
	p_cell2=gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (p_combox,p_cell2,TRUE);
	gtk_cell_layout_set_attributes(p_combox,p_cell2,"text",1);
	
	p_fd=popen("docker images","r");
	
	if(p_fd==NULL)
	{
		contains_message(GTK_MESSAGE_ERROR,_("you must start docker firsr to continue"));
		

	}
	
	fgets(p_buf,1024,p_fd);
	while(fgets(p_buf,1024,p_fd) !=NULL){
	sscanf(p_buf,"%s %s",image_list[p_num].p_name,image_list[p_num].p_tag);
	gtk_list_store_append (p_store,&iter);
	gtk_list_store_set (p_store,&iter,0,image_list[p_num].p_name,1,image_list[p_num].p_tag,-1);
	p_num++;
	}
	
	gtk_combo_box_set_model (p_combox,p_store);
	pclose(p_fd);
	g_free (p_buf);

}
void p_combx_changed(GtkWidget *window,gpointer data)//获取combox每次选择的数据
{
	GtkTreeModel *model;
	GtkTreeIter iter_p;
	memset(p_link_combo_data,'\0',sizeof(p_link_combo_data));
	static char *p_row_name=NULL;
	static  char *p_row_tag=NULL;
	model=gtk_combo_box_get_model (window);
	gtk_combo_box_get_active_iter (window,&iter_p);
	gtk_tree_model_get (model,&iter_p,0,&p_row_name,1,&p_row_tag,-1);
	//gtk_tree_model_get (model,&iter_p,0,&p_row_name,-1);
	//printf("%s %s\n",p_row_name,p_row_tag);
	sprintf(p_link_combo_data,"%s:%s",p_row_name,p_row_tag);

	//printf("%s\n",p_link_combo_data);
}
void p_link_fun(gpointer data)//线程函数
{
	gdk_threads_enter ();
printf("%s\n",data);
	static FILE *fd_fun=NULL;
	//gtk_entry_set_text (p_entry_con,"");
	//gtk_entry_reset_im_context(p_entry);
	fd_fun=popen(data,"r");
	
	gdk_threads_leave ();
	pclose (fd_fun);
	g_free(data);
}
void p_button11_clicked(GtkWidget *window,gpointer data)//按钮信号函数
{
		static char *p_link_s=NULL;
		static char *p_link_entry_data=NULL;
		static char *p_link_contains_data=NULL;
		//p_link_entry_data=(char *)malloc(sizeof(char)*1024);
		//	p_link_contains_data=(char *)malloc(sizeof(char)*1024);
			p_link_s=(char *)malloc(sizeof(p_link_s)*1024);
		//memset(p_link_entry_data,'\0',sizeof(char)*1024);
		//memset(p_link_contains_data,'\0',sizeof(char)*1024);
		memset(p_link_s,'\0',sizeof(p_link_s)*1024);
		p_link_entry_data=gtk_entry_get_text (p_entry);
		p_link_contains_data=gtk_entry_get_text (p_entry_con);						
		
		if(*p_link_entry_data==NULL){
			contains_message(GTK_MESSAGE_ERROR,_("Port number can not be empty"));
			
				return;
		}
		if(*p_link_contains_data==NULL){
		contains_message(GTK_MESSAGE_ERROR,_("Custom container name can not be empty"));
			
		return;
		}
		if(p_link_combo_data[0]=='\0')
	{
		contains_message(GTK_MESSAGE_ERROR,_("image not is empty"));
		return;
			
	}
		sprintf(p_link_s,"sudo gnome-terminal -x bash -c \"sudo docker run -ti --rm --name %s %s %s /bin/bash\"",
		        p_link_contains_data,p_link_entry_data,p_link_combo_data);
			//printf("%s\n",p_link_s);
			//g_free (p_link_entry_data);
			//g_free (p_link_contains_data);
			g_thread_new ("p_link",p_link_fun,p_link_s);
	printf("%s %s %s\n",p_link_entry_data,p_link_contains_data,p_link_combo_data);
		//g_free (p_link_entry_data);
		//g_free (p_link_contains_data);


}

void p_link_contains()
{
	
         GtkWidget *window;
	GtkBuilder *builder;
	GtkWidget *button11,
                            *button22,
                            *button33;
    
	GtkWidget *label11;
	GtkWidget *label22;
	GtkWidget *label33;
	GtkWidget *label44;
	GtkWidget *p_combox;
    
	builder=gtk_builder_new ();
    
	gtk_builder_add_from_file (builder,P_LINK,NULL);
	window=GTK_WIDGET(gtk_builder_get_object (builder, P_LINK_WINDOW));
	label11=GTK_WIDGET(gtk_builder_get_object (builder,"label1"));
	label22=GTK_WIDGET(gtk_builder_get_object (builder,"label2"));
	label33=GTK_WIDGET(gtk_builder_get_object (builder,"label3"));
	label44=GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	p_combox=GTK_WIDGET(gtk_builder_get_object (builder,"combobox1"));
	p_entry=GTK_WIDGET(gtk_builder_get_object (builder,"entry2"));
	p_entry_con=GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	//create_contians_view_entry22=GTK_WIDGET(gtk_builder_get_object (builder,"entry2"));
	button11=GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	button22=GTK_WIDGET(gtk_builder_get_object (builder,"button2"));
	button33=GTK_WIDGET(gtk_builder_get_object (builder,"button3"));
	gtk_window_set_title (window,_("External access"));
	gtk_label_set_text (GTK_LABEL(label11),_("please input image"));
	//gtk_label_set_selectable(label11,TRUE);
	gtk_label_set_text (GTK_LABEL(label33),_("please input yourself contains name"));
	gtk_label_set_text (GTK_LABEL(label22),_("Please enter the mapping port number"));
	gtk_label_set_text (GTK_LABEL(label44),_("image                             tag"));
	gtk_button_set_label (button22,_("Writing format:"));
	gtk_button_set_label (button33,_("Port changes or additions:"));
	
	gtk_button_set_label (GTK_BUTTON(button11),_("sure"));
	add_data_combox(p_combox);
	gtk_widget_set_tooltip_markup (button22,_("<span foreground=\"#000000\">-P(Random mapping port).-p 500:22 (put host 50 port to contain 22).</span>"));
	gtk_widget_set_tooltip_markup (button33,_("<span foreground=\"#000000\">Example:put A contain export,then import A produce a image</span>"));
	//gtk_widget_set_tooltip_text(button11,"sadasdasd");
	
        g_signal_connect(G_OBJECT(p_combox),"changed",G_CALLBACK(p_combx_changed),NULL);
        g_signal_connect(G_OBJECT(button11),"clicked",G_CALLBACK(p_button11_clicked),NULL);
        //g_signal_connect(G_OBJECT(button22),"enter",G_CALLBACK(p_button22_clicked),label11);
        //g_signal_connect(G_OBJECT(button22),"leave",G_CALLBACK(p_button22_clicked_leave),label11);
        g_object_unref (builder);
        gtk_widget_show_all (window);
        
        //gtk_widget_hide (label11);
        //g_printf("ppppp\n");

}

