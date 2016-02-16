#include "global/global.h"

gint contains_num=0;
gint images_nu=0;

struct container_images{
	char name[100];
	char tag[100];
};//镜像的信息

struct contains_state
{
        char attachstdin_state[1024];
        char hostname[1024];
        char attachstdin[1024];
        char name[1024];
        char image[1024];
};//守护容器的信息
struct contains_state state[1024];//
char contains_name_tag[1024]={0};
char contains_name_import_tag[1024]={0};
GtkWidget *tree_view;
GtkWidget *gurd_entry1;
GtkWidget *gurd_entry2;
char gurd_entry1_data[1024]={0};
char gurd_entry2_data[1024]={0};
char gurd_comb_data[1024]={0};
GtkWidget *create_contians_view_entry22;
GtkWidget *window_import;//导入功能填写的界面
void docker_contain()//获取容器数据,并把数据赋值给结构体
{
	
	static char buf[1024]={0};
	static gint leng_buf=0;
	static char *p_buf=NULL;
		
      static FILE *fd=NULL;
        fd=popen("docker ps -a","r");
		contains_num=0;
		
        
        if(fgets(buf,1000,fd) == NULL)
{
		button33_fun (NULL,NULL);
}
        while(fgets(buf,1000,fd) !=NULL)
{
		p_buf=buf;
		if(sscanf(buf,"%s  %s",a[contains_num].id,a[contains_num].image) < 2)
		{
        printf("docker Containerization init fale\n");

		}
		   
			leng_buf=0;
		    leng_buf = strlen(buf) - 1;
			sscanf(p_buf+(leng_buf-20),"%s",a[contains_num].name);
   			 contains_num++;		
}
	
	//memset(buf,'\0',sizeof(buf));
	pclose(fd);

}
void gurd_fun(GtkListStore *list,GtkTreeIter iter)
{
	char buf[1024]={0};
        char config_path[1024]={0};
        DIR *fd=NULL;
        FILE *po=NULL;
        int gurd_nu=0;
        int leng=0;
        struct dirent *p;
        char *str=NULL;
        FILE *fv=NULL;
		str=(char *)g_malloc(4096*4);
        if((fd=opendir(PATH))==NULL)
        {
        //printf("opendir is error\n");
			contains_message(GTK_MESSAGE_ERROR,_("No permission to open the file"));
			return;

}
        while((p=readdir(fd))!=NULL)
        {
                gurd_nu=0;
                sprintf(buf,"%s",p->d_name);
                if(strcmp(buf,".")==0||strcmp(buf,"..")==0){
                        continue;
}
                        sprintf(config_path,"%s%s/config.json",PATH,buf);
                        fv=fopen(config_path,"r");
                        fseek(fv,0,SEEK_END);
                        leng=ftell(fv);
                        printf("leng is %d\n",leng);
                        fseek(fv,0,SEEK_SET);
                        memset(str,'\0',1*sizeof(str));
sprintf(str,"cat %s | awk -F \"AttachStdout\" '{print $2}' " "| awk -F ',' '{print $2}'",config_path);
                        po=popen(str,"r");
                        memset(str,'\0',sizeof(char));
                        while(fgets(str,leng,po)){

                        sscanf(str,"%[^:]%*[:]%s",state[gurd_nu].attachstdin,state[gurd_nu].attachstdin_state);
                       // printf("1 is %s\n",state[gurd_nu].attachstdin);
                       // printf("2 is %s\n",state[gurd_nu].attachstdin_state);
                                }
//___________________________________镜像__________________//                   
                                memset(str,'\0',1*sizeof(str));
sprintf(str,"cat %s | awk -F \"Image\" '{print $2}'" "| awk -F '\"' '{print $3}'",config_path);
                        po=popen(str,"r");
                        memset(str,'\0',1*sizeof(str));
                        while(fgets(str,1024,po)){

                        sscanf(str,"%s",state[gurd_nu].image);
                       // printf("image is %s\n",state[gurd_nu].image);
                                }
//___________________________________镜像__________________//
//__________________________________名称_______________//

							  memset(str,'\0',1*sizeof(str));
sprintf(str,"cat %s | awk -F \"Name\" '{print $2}' " "| awk -F '\"' '{print $3}' " "| awk -F \"/\" '{print $2}'",config_path);
                        po=popen(str,"r");
                        memset(str,'\0',1*sizeof(str));
                        while(fgets(str,1024,po)){

                        sscanf(str,"%s",state[gurd_nu].name);
                        //printf("name is %s\n",state[gurd_nu].name);
                                }
//__________________________________名称_______________//

//__________________________________ID______________//

						memset(str,'\0',1*sizeof(str));
                        sprintf(str,"cat %s | awk -F \"Hostname\" '{print $2}'" "| awk -F '\"' '{print $3}'",config_path);
                        po=popen(str,"r");
                        memset(str,'\0',1*sizeof(str));
                        while(fgets(str,1024,po)){

                        sscanf(str,"%s",state[gurd_nu].hostname);
                       // printf("ID is %s\n",state[gurd_nu].hostname);
}
                        if(strcmp(state[gurd_nu].attachstdin_state,"false")==0){
                         gtk_list_store_append (list,&iter);
				gtk_list_store_set (list,&iter,0,state[gurd_nu].image,1,state[gurd_nu].hostname,2,state[gurd_nu].name);
						
}
                        gurd_nu++;
                        fclose(fv);
                        
}
g_free(str);

}
void close_window_lsgurd22(GtkWidget *window,gpointer data)
{
	gtk_widget_destroy (data);
}
void show_window(GtkWidget *window,gpointer data)
{
gtk_widget_show (data);
//gtk_widget_destroy (data);
}
void button77_lsgurd_fun(GtkWidget *window,gpointer data)//从文件读取守护容器的信息
{
	gtk_widget_hide (data);
	//GtkTreeViewColumn *image_nam;
	GtkWidget *image_nam;
	//GtkTreeViewColumn *id;
	//GtkTreeViewColumn *nam;
	GtkWidget *id;
	GtkWidget *nam;
	
	GtkBuilder *builder;
	GtkTreeIter iter_gurd;
	GtkCellRenderer *cell_image;
	GtkListStore *list_gurd;
	GtkTreeView *tree_view_lsgurd;
	GtkWidget *window_lsgurd22;
	GtkWidget *button_close;
	builder=gtk_builder_new ();
	gint gurd_ls_nu=0;
	gtk_builder_add_from_file (builder,CONTAINER_MANAGE_LSGUARD_FILE,NULL);
	window_lsgurd22=GTK_WIDGET(gtk_builder_get_object (builder,CONTAINER_MANAGE_LSGUARD_WINDOW));
	button_close=GTK_WIDGET(gtk_builder_get_object (builder,"button2"));
	image_nam=GTK_WIDGET(gtk_builder_get_object (builder,"treeviewcolumn2"));
	id=GTK_WIDGET(gtk_builder_get_object (builder,"treeviewcolumn3"));
	nam=GTK_WIDGET(gtk_builder_get_object (builder,"treeviewcolumn4"));
	gtk_tree_view_column_set_title (image_nam,_("please select image name"));
	//gtk_tree_view_column_set_title (id,_("asdasd"));
	gtk_tree_view_column_set_title (id,_("gurd contain ID"));
	gtk_tree_view_column_set_title (nam,_("gurd contain name"));
	
	cell_image=gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (image_nam,cell_image,TRUE);
	gtk_tree_view_column_set_attributes(image_nam,cell_image,"text",0,NULL);

	cell_image=gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (id,cell_image,TRUE);
	gtk_tree_view_column_set_attributes(id,cell_image,"text",1,NULL);

	cell_image=gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (nam,cell_image,TRUE);
	gtk_tree_view_column_set_attributes(nam,cell_image,"text",2,NULL);
	
	list_gurd=gtk_list_store_new (3,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
		
	tree_view_lsgurd=GTK_WIDGET(gtk_builder_get_object (builder,"treeview_lsurd"));
	gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view_lsgurd),GTK_TREE_MODEL(list_gurd));

		gurd_fun(list_gurd,iter_gurd);
	
	gtk_window_set_title (window_lsgurd22,_("ls gurd contain"));
	g_signal_connect(G_OBJECT(button_close),"clicked",G_CALLBACK(close_window_lsgurd22),window_lsgurd22);
	g_signal_connect(G_OBJECT(window_lsgurd22),"destroy",G_CALLBACK(show_window),data);
	g_object_unref (builder);
	gtk_widget_show_all (window_lsgurd22);

}
void contains_message(GtkMessageType type,char *buf)//全局的一个函数:执行成功某件事后弹出一个界面提示
{
	GtkWidget *dialog;
	//dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,type,GTK_BUTTONS_OK,buf);
		gtk_dialog_run (GTK_DIALOG(dialog));
		gtk_widget_destroy (dialog);


}
void clicked_row11(GtkWidget *window,gpointer data)//获取treeview每行的数据
{
	GtkTreeIter iter11;
	static char *value11=NULL;
	value11=(char *)malloc(sizeof(char)*100);
	GtkTreeModel *mod;
	mod=gtk_tree_view_get_model (GTK_TREE_VIEW(window));
	//gtk_tree_selection_get_selected (GTK_TREE_SELECTION(data),&mod,&iter11);
	//g_printf("%s\n",);
	//gtk_tree_selection_get_selected(GTK_TREE_SELECTION(data), &mod, &iter11);
	//gtk_tree_model_get (GTK_TREE_MODEL(mod),&iter11,2,&value,-1);
	if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(data), &mod, &iter11))
	{
	gtk_tree_model_get (GTK_TREE_MODEL(mod),&iter11,2,&value11,-1);
	//printf("%s\n",value11);
	g_free(value11);
	//g_printf("111111111111111111\n");
	}
}
void docker_contain_start(gpointer data)//启动并进入容器
{
	
	gdk_threads_enter ();
	g_printf("%s\n",data);
	system(data);
	g_free(data);
	gdk_threads_leave ();
	printf("llll\n");
}
void button11_treeview_fun(GtkWidget *window,gpointer data)//启动并进入容器
{
	GtkTreeIter iter22;
	GtkTreeModel *mod22;
	
	GtkMessageType type;
	
	mod22=gtk_tree_view_get_model (GTK_TREE_VIEW(window));
	static char *ss=NULL;
	static char *tmp=NULL;
	ss=(char *)malloc(sizeof(char)*100);
	tmp=(char *)malloc(sizeof(char)*100);
	//gtk_tree_selection_get_selected (GTK_TREE_SELECTION(data),&mod22,&iter22);
	//gtk_tree_model_get(GTK_TREE_MODEL(mod22),&iter22,1,&ss,-1);
	

	if(gtk_tree_selection_get_selected (GTK_TREE_SELECTION(data),&mod22,&iter22))
	{
	//system("ls");
		gtk_tree_model_get(GTK_TREE_MODEL(mod22),&iter22,1,&ss,-1);
		sprintf(tmp,"sudo gnome-terminal -x bash -c \"sudo docker start -i %s\"",ss);
		g_thread_new("dockr_start",docker_contain_start,tmp);
			
		
		printf("%s\n",ss);
		printf("%s\n",tmp);
		g_free(ss);
}
	else{
		contains_message(GTK_MESSAGE_ERROR,_("Choose a line of content"));
		
	}
	
}
void button88_treeview_fun(GtkButton *button,gpointer data)//删除单个容器
{
	
	GtkTreeModel *model;
	GtkListStore *list22;
	GtkTreeIter iter;
	static char *buf=NULL;
	static char *tmp=NULL;
	static int status=0;
	buf=(char *)malloc(sizeof(char)*100);
	tmp=(char *)malloc(sizeof(char)*100);
	model=gtk_tree_view_get_model (GTK_TREE_VIEW(tree_view));
	list22=GTK_LIST_STORE(model);
	if(gtk_tree_selection_get_selected (GTK_TREE_SELECTION(data),&model,&iter))
	{
		
		gtk_tree_model_get (model,&iter,1,&buf,-1);
		printf("%s\n",buf);
		sprintf(tmp,"sudo docker rm %s",buf);
		status=system(tmp);
		
			if(status==0){
			gtk_list_store_remove (list22, &iter);
			contains_message(GTK_MESSAGE_INFO,_("Delete success"));	
		}
			else
		{
		contains_message(GTK_MESSAGE_ERROR,_("Can't delete the running container"));
			return;
		}
		g_free(buf);
		g_free(tmp);
	}
	else
	{
	contains_message(GTK_MESSAGE_ERROR,_("Choose a line of content"));
		return;
	}


}
void button99_treeview_fun(GtkWidget *window,gpointer *data)//删除所有容器
{
	GtkWidget *dialog;
	static gint gtk_run=0;
	static gint j=0;
	GtkListStore *list11;
	GtkTreeModel *model;
	GtkTreeIter iter;
	pid_t pid;
	model=gtk_tree_view_get_model (GTK_TREE_VIEW(tree_view));
	list11=GTK_LIST_STORE(model);
	dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_YES_NO,"是否删除所有容器");
		gtk_run=gtk_dialog_run (GTK_DIALOG(dialog));
		if(gtk_run==GTK_RESPONSE_YES){
			//printf("yyyy\n");
			pid=system("docker rm `docker ps -a -q`");
			//printf("pid is %d\n",pid);
			//printf(" WIFEXITED is %d\n",WIFEXITED(pid));
			//printf(" WEXITSTATUS is %d\n",WEXITSTATUS(pid));
			
			if(pid==0)
			{
				contains_message(GTK_MESSAGE_INFO,_("Delete success"));
				docker_contain();
				gtk_list_store_clear (list11);
				for(j=0;j<contains_num;j++){
	gtk_list_store_append (list11,&iter);
	gtk_list_store_set (list11, &iter, 0,&a[j].image,-1);
	gtk_list_store_set (list11, &iter, 1,&a[j].id,-1);
	gtk_list_store_set (list11, &iter, 2,&a[j].name,-1);
					}
			}
			else
			{
			//contains_message(GTK_MESSAGE_ERROR,"不能删除正在运行及不存在的容器");
				docker_contain();
				gtk_list_store_clear (list11);
				for(j=0;j<contains_num;j++){
	gtk_list_store_append (list11,&iter);
	gtk_list_store_set (list11, &iter, 0,&a[j].image,-1);
	gtk_list_store_set (list11, &iter, 1,&a[j].id,-1);
	gtk_list_store_set (list11, &iter, 2,&a[j].name,-1);
		}
		contains_message(GTK_MESSAGE_ERROR,_("Can't delete the running container"));
			}
			gtk_widget_destroy (dialog);
		}
		else
	
		gtk_widget_destroy (dialog);


}

void docker_contains_create_func(gpointer *data)//创建系统默认名字容器的功能
{	
	gdk_threads_enter ();
	system("sudo gnome-terminal -x bash -c \"sudo docker run -ti  ubuntu /bin/bash\"");	
	gdk_threads_leave ();
}
void docker_tree_view_clean(gpointer data)//创建容器后刷新界面
{
	
	gdk_threads_enter ();
	sleep(3);
	static gint j=0;
	GtkListStore *list11;
	GtkTreeModel *model;
	GtkTreeIter *iter;
	static char buf[1024]={0};
		static gint leng_buf=0;
		static char *p_buf=NULL;
        static FILE *fd=NULL;
        fd=popen("docker ps -a","r");
		//i=0;
    model=gtk_tree_view_get_model (GTK_TREE_VIEW(tree_view));
	list11=GTK_LIST_STORE(model);
	
	docker_contain();
	gtk_list_store_clear (list11);
	for(j=0;j<contains_num;j++){
	gtk_list_store_append (list11,&iter);
	gtk_list_store_set (list11, &iter, 0,&a[j].image,-1);
	gtk_list_store_set (list11, &iter, 1,&a[j].id,-1);
	gtk_list_store_set (list11, &iter, 2,&a[j].name,-1);
		}
	gdk_threads_leave ();
}
void button44_treeview_fun(GtkWidget *window,gpointer *data)//创建容器的选择界面
{

	GtkWidget *dialog;
	static gint gtk_run=0;
	dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_YES_NO,"是否使用系统默认的名字");
		gtk_run=gtk_dialog_run (GTK_DIALOG(dialog));
		if(gtk_run==GTK_RESPONSE_YES){
			//system("sudo gnome-terminal -x bash -c \"sudo docker run -ti --name chen ubuntu /bin/bash\"");
			g_thread_new("docker_contains_create",docker_contains_create_func,dialog);
			g_thread_new ("clean",docker_tree_view_clean,dialog);
			gtk_widget_destroy (dialog);
		}
		else if(gtk_run==GTK_RESPONSE_NO)
	{
		printf("nnnn\n");
		create_contians_view();//选择不是系统默认创建容器的界面
		gtk_widget_destroy (dialog);

	}
		else{
			gtk_widget_destroy (dialog);
			
		}

}
void button33_create_gurd_contians_fun(GtkWidget *window,gpointer data)//守护界面按钮信号
{
	
	gint num=0;
	gint num22=0;
	gint num33=0;
	gchar *bf1=NULL;
	gchar *bf2=NULL;
	gchar *tmp11=NULL;
	tmp11=(char *)malloc(sizeof(char)*1024);
	bf1=gtk_entry_get_text (gurd_entry1);
	bf2=gtk_entry_get_text (gurd_entry2);
	for(num=0;num<strlen(gurd_comb_data);num++){
	//if(ispunct(bu[num]) || isspace(bu[num])){
		if(isspace(gurd_comb_data[num])){
	contains_message(GTK_MESSAGE_ERROR,_("The image name cannot contain spaces characters"));
		return;
	}
	}
	if(gurd_comb_data[0]==NULL){
	contains_message(GTK_MESSAGE_ERROR,_("The image name cannot is spaces"));
		return;
	}
	for(num22=0;num22<strlen(bf1);num22++){
	if(ispunct(bf1[num22]) || isspace(bf1[num22])){
	contains_message(GTK_MESSAGE_ERROR,_("contains name not is Spaces and special characters"));
		return;
	}
	}
	if(bf1[0]==NULL){
	contains_message(GTK_MESSAGE_ERROR,_("contains name is not empty"));
		return;
	}
	
	if(bf2[0]==NULL){
	contains_message(GTK_MESSAGE_ERROR,_("gurd contains dosomsing is not empty"));
		return;
	}
	sprintf(tmp11,"docker run -d --name %s %s /bin/bash -c \"%s\"",bf1,gurd_comb_data,bf2);
	printf("%s\n",tmp11);
	g_thread_new ("defin_myname_contians22",defin_myname_contians_fun,tmp11);
	g_thread_new ("clean22",docker_tree_view_clean,NULL);
	gtk_widget_destroy (data);
}
void combox_gurd_change(GtkWidget *window,gpointer data)//守护comb信号
{
	char *combo_name=NULL;
	char *combo_tag=NULL;
	GtkTreeModel *model_combox;
	GtkTreeIter iter_combo;
	model_combox=gtk_combo_box_get_model (window);
	gtk_combo_box_get_active_iter (window,&iter_combo);
	gtk_tree_model_get(model_combox,&iter_combo,0,&combo_name,1,&combo_tag,-1);
	sprintf(gurd_comb_data,"%s:%s",combo_name,combo_tag);
	printf("%s\n",gurd_comb_data);


}
void button33_treeview_fun(GtkWidget *window,gpointer *data)//守护运行
{
	GtkBuilder *builder;
	builder=gtk_builder_new ();
	GtkWidget *button11;
	GtkWidget *label11;
	GtkWidget *label22;
	GtkWidget *label33;
	GtkWidget *combox_images;
	gurd_comb_data[0]=0;
	gtk_builder_add_from_file (builder,CONTAINER_MANAGE_GUARD_FILE,NULL);
	window=GTK_WIDGET(gtk_builder_get_object (builder,CONTAINER_MANAGE_GUARD_WINDOW));
	label11=GTK_WIDGET(gtk_builder_get_object (builder,"label1"));
	label22=GTK_WIDGET(gtk_builder_get_object (builder,"label2"));
	label33=GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	gurd_entry1=GTK_WIDGET(gtk_builder_get_object (builder,"entry2"));
	gurd_entry2=GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	//create_contians_view_entry22=GTK_WIDGET(gtk_builder_get_object (builder,"entry2"));
	button11=GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	gtk_label_set_text (GTK_LABEL(label11),_("please input image"));
	gtk_label_set_text (GTK_LABEL(label22),_("please input yourself contains name"));
	gtk_label_set_text (GTK_LABEL(label33),_("please write gurd will what do somthing"));
	gtk_button_set_label (GTK_BUTTON(button11),_("sure"));
	combox_images=GTK_WIDGET(gtk_builder_get_object (builder,"combobox1"));
	combox_images_data(combox_images);
	g_signal_connect(G_OBJECT(button11),"clicked",G_CALLBACK(button33_create_gurd_contians_fun),window);
	g_signal_connect(G_OBJECT(combox_images),"changed",G_CALLBACK(combox_gurd_change),combox_images);
	g_object_unref (builder);
	gtk_widget_show_all (window);
}
void 
button22_treeview_fun(GtkWidget *window,gpointer data)//终止容器
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	GtkListStore *list11;
	static char *buf=NULL;
	static char *buf11=NULL;
	buf11=(char *)malloc(sizeof(char)*100);
	buf=(char *)malloc(sizeof(char)*100);
	model=gtk_tree_view_get_model (GTK_TREE_VIEW(tree_view));
	list11=GTK_LIST_STORE(model);
	if(gtk_tree_selection_get_selected (data,&model,&iter)){
			gtk_tree_model_get (model,&iter,1,&buf,-1);
			sprintf(buf11,"docker stop %s",buf);
			if(system(buf11)==0){
				
			contains_message(GTK_MESSAGE_INFO,_("already stop contains"));
			}
	}
	else
	{
	contains_message(GTK_MESSAGE_ERROR,_("Choose a line of content"));

	}
}
void defin_myname_contians_fun(gpointer data)
{
	pid_t st;
	st=system(data);
	g_free (data);
}
void button11_create_contians_view_fun(GtkWidget *window,gpointer data)//判断创建容器的主界面填写的内容及创建容器
{
	gint num=0;
	gint num22=0;
	//gchar *bu=NULL;
	gchar *bf=NULL;
	gchar *tmp11=NULL;
	tmp11=(char *)malloc(sizeof(char)*100);
	//bu=gtk_entry_get_text (create_contians_view_entry11);
	bf=gtk_entry_get_text (create_contians_view_entry22);
	for(num=0;num<strlen(contains_name_tag);num++){
	//if(ispunct(bu[num]) || isspace(bu[num])){
		if(isspace(contains_name_tag[num])){
	contains_message(GTK_MESSAGE_ERROR,_("The image name cannot contain spaces characters"));
		return;
	}
	}
	if(contains_name_tag[0]==NULL){
	contains_message(GTK_MESSAGE_ERROR,_("The image name cannot is spaces"));
		return;
	}
	for(num22=0;num22<strlen(bf);num22++){
	if(ispunct(bf[num22]) || isspace(bf[num22])){
	contains_message(GTK_MESSAGE_ERROR,_("contains name not is Spaces and special characters"));
		return;
	}
	}
	if(bf[0]==NULL){
	contains_message(GTK_MESSAGE_ERROR,_("contains name is not empty"));
		return;
	}
	sprintf(tmp11,"sudo gnome-terminal -x bash -c \"docker run -ti --name %s %s /bin/bash\"",bf,contains_name_tag);
		g_printf("%s\n",tmp11);
		g_thread_new ("defin_myname_contians",defin_myname_contians_fun,tmp11);
		g_thread_new ("clean",docker_tree_view_clean,NULL);
}
void combox_images_change(GtkWidget *window,gpointer data)
{
	char *combo_name=NULL;
	char *combo_tag=NULL;
	GtkTreeModel *model_combox;
	GtkTreeIter iter_combo;
	model_combox=gtk_combo_box_get_model (window);
	gtk_combo_box_get_active_iter (window,&iter_combo);
	gtk_tree_model_get(model_combox,&iter_combo,0,&combo_name,1,&combo_tag,-1);
	//printf("%s %s\n",combo_name,combo_tag);
	sprintf(contains_name_tag,"%s:%s",combo_name,combo_tag);
	//printf("%s\n",contains_name_tag);
}
void combox_images_import_change(GtkWidget *window,gpointer data)
{
	char *combo_name_import=NULL;
	char *combo_tag_import=NULL;
	GtkTreeModel *model_combox;
	GtkTreeIter iter_combo;
	model_combox=gtk_combo_box_get_model (window);
	gtk_combo_box_get_active_iter (window,&iter_combo);
	gtk_tree_model_get(model_combox,&iter_combo,0,&combo_name_import,1,&combo_tag_import,-1);
	//printf("%s %s\n",combo_name,combo_tag);
	sprintf(contains_name_import_tag,"%s",combo_name_import);
	//printf("%s\n",contains_name_tag);
}
void combox_images_data(GtkWidget *combox_all)//所有combox获取镜像的信息
{
	GtkTreeIter iter_images;
	FILE *fd_images=NULL;
	char *images_buf=NULL;
	struct container_images images_infomation[1024];
	images_buf=(char *)g_malloc(sizeof(images_buf)*1024);
	memset(images_buf,'\0',sizeof(images_buf));
	GtkListStore *list_images=gtk_list_store_new (2,G_TYPE_STRING,G_TYPE_STRING);
	GtkCellRenderer *cell_images=gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (combox_all,cell_images,TRUE);
	gtk_cell_layout_set_attributes(combox_all,cell_images,"text",0,NULL);
	GtkCellRenderer *cell_images22=gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (combox_all,cell_images22,FALSE);
	gtk_cell_layout_set_attributes(combox_all,cell_images22,"text",1,NULL);
	gtk_combo_box_set_model (combox_all,list_images);

	fd_images=popen("docker images","r");
	 images_nu=0;
	if(fgets(images_buf,1024,fd_images) ==NULL)
	{
		button33_fun (NULL,NULL);
		return;
	}
	while(fgets(images_buf,1024,fd_images))
	{
		if(sscanf(images_buf,"%s %s",images_infomation[images_nu].name,images_infomation[images_nu].tag)<2)
		{
			printf("镜像数据赋值失败\n");
		}
		gtk_list_store_append (list_images,&iter_images);
		gtk_list_store_set (list_images,&iter_images,0,images_infomation[images_nu].name,1,images_infomation[images_nu].tag,-1);
		images_nu++;
	}
		g_free(images_buf);
		pclose (fd_images);
}
void create_contians_view()//选择不是系统默认创建容器的界面
{
	GtkWidget *window;
	GtkBuilder *builder;
	builder=gtk_builder_new ();
	GtkWidget *button11;
	GtkWidget *label11;
	GtkWidget *label22;
	GtkWidget *combox_images;
	contains_name_tag[0]=0;
	gtk_builder_add_from_file (builder,CREATE_CONTAINS_FILE,NULL);
	window=GTK_WIDGET(gtk_builder_get_object (builder,CONTAINS_MANAGE_CREATECONTAINS_WINDOW));
	label11=GTK_WIDGET(gtk_builder_get_object (builder,"label1"));
	label22=GTK_WIDGET(gtk_builder_get_object (builder,"label2"));
	create_contians_view_entry22=GTK_WIDGET(gtk_builder_get_object (builder,"entry2"));
	button11=GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	gtk_label_set_text (GTK_LABEL(label11),_("please input image"));
	gtk_label_set_text (GTK_LABEL(label22),_("please input yourself contains name"));
	gtk_button_set_label (GTK_BUTTON(button11),_("sure"));
	combox_images=GTK_WIDGET(gtk_builder_get_object (builder,"combobox1"));
	combox_images_data(combox_images);
	g_signal_connect(G_OBJECT(button11),"clicked",G_CALLBACK(button11_create_contians_view_fun),window);
	g_signal_connect(G_OBJECT(combox_images),"changed",G_CALLBACK(combox_images_change),combox_images);
	g_object_unref (builder);
	gtk_widget_show_all (window);
}
/*---------------------------------导出down-----------------------------*/
	void export_selcet_file(GtkWidget *window,gpointer data)
{
	GtkWidget *dialog;
	static gchar *filename55=NULL;
	GtkTreeModel *model55;
	static gchar *buf55=NULL;
	static gchar *tmp55=NULL;
	GtkTreeIter iter55;
	
	model55=gtk_tree_view_get_model (GTK_TREE_VIEW(tree_view));
	
	if(gtk_tree_selection_get_selected (GTK_TREE_SELECTION(data),&model55,&iter55)){
		buf55=(char *)malloc(sizeof(char)*20);
		tmp55=(char *)malloc(sizeof(char)*200);
		gtk_tree_model_get (model55,&iter55,1,&buf55,-1);
	dialog=gtk_file_chooser_dialog_new(_("Please rename the exported file"),NULL,GTK_FILE_CHOOSER_ACTION_SAVE,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_SAVE,
	                                   GTK_RESPONSE_OK,NULL);
	if(gtk_dialog_run (dialog)==GTK_RESPONSE_OK){
		//import_print_file_name (dialog);
		filename55=gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog));
		//g_printf("yyyy%s\n",buf55);
		//g_printf("%s\n",filename55);
		sprintf(tmp55,"docker export -o %s %s",filename55,buf55);
		system(tmp55);
		g_printf("%s\n",tmp55);
		g_free(buf55);
		g_free(tmp55);
		gtk_widget_destroy (dialog);
	}
	else{
		gtk_widget_destroy (dialog);
	}
	}
	else{
	contains_message (GTK_MESSAGE_ERROR,_("Choose a line of content"));
		return;
	}
}





/*-------------------------------导出up--------------------------------*/
/*-------------------------导入功能down-----------------------------------------------------*/
void import_create_file_veiw_fun(GtkWidget *window,gpointer data)
{
	static gint num;
	static gint num22;
	//gchar buf[];
	//buf=(char *)malloc(sizeof(char)*20);
	//static gchar *bu=NULL;
	static gchar *bf=NULL;
	static gchar *tmp11=NULL;
	tmp11=(char *)malloc(sizeof(char)*100);
	//bu=gtk_entry_get_text (GTK_WIDGET(create_contians_view_entry11));
	bf=gtk_entry_get_text (create_contians_view_entry22);
	for(num=0;num<strlen(contains_name_import_tag);num++){
	//if(ispunct(bu[num]) || isspace(bu[num])){
		if(isspace(contains_name_import_tag[num])){
	contains_message(GTK_MESSAGE_ERROR,_("The image name cannot contain spaces characters"));
		return;
	}
	}
	if(contains_name_import_tag[0]==NULL){
	contains_message(GTK_MESSAGE_ERROR,_("The image name cannot is spaces"));
		return;
	}
	for(num22=0;num22<strlen(bf);num22++){
	//if(ispunct(bf[num22]) || isspace(bf[num22])){
		if(isspace(bf[num22])){
	contains_message(GTK_MESSAGE_ERROR,_("label name is not spacechar"));
		return;
	}
	}
	if(bf[0]==NULL){
	contains_message(GTK_MESSAGE_ERROR,_("label name is not empty"));
		return;
	}
	sprintf(tmp11,"cat %s | sudo docker import - %s:%s",data,contains_name_import_tag,bf);
		//g_printf("%s\n",contains_name_import_tag);
		g_printf("%s\n",tmp11);
		if(system(tmp11)==0)
	{
			contains_message(GTK_MESSAGE_INFO,_("import successfull"));
	}
		g_free(tmp11);
		gtk_widget_destroy (window_import);
}

void import_create_file_view(gchar *buf)//导入功能填写界面
{
	
	
	GtkBuilder *builder;
	builder=gtk_builder_new ();
	GtkWidget *button11;
	GtkWidget *label11;
	GtkWidget *label22;
	GtkWidget *combox_images_import;
	FILE *fd_images_import;
	
	gtk_builder_add_from_file (builder,CREATE_CONTAINS_FILE,NULL);
	window_import=GTK_WIDGET(gtk_builder_get_object (builder,CONTAINS_MANAGE_CREATECONTAINS_WINDOW));
	label11=GTK_WIDGET(gtk_builder_get_object (builder,"label1"));
	label22=GTK_WIDGET(gtk_builder_get_object (builder,"label2"));
	combox_images_import=GTK_WIDGET(gtk_builder_get_object (builder,"combobox1"));
	create_contians_view_entry22=GTK_WIDGET(gtk_builder_get_object (builder,"entry2"));
	button11=GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	gtk_label_set_text (GTK_LABEL(label11),_("please write import new image name"));
	gtk_label_set_text (GTK_LABEL(label22),_("please write import new image label_name"));
	gtk_button_set_label (GTK_BUTTON(button11),"sure");
	combox_images_data(combox_images_import);
	g_signal_connect(G_OBJECT(button11),"clicked",G_CALLBACK(import_create_file_veiw_fun),buf);//buf文件控件获取的路径名字
	g_signal_connect(G_OBJECT(combox_images_import),"changed",G_CALLBACK(combox_images_import_change),combox_images_import);
	g_object_unref (builder);
	gtk_widget_show_all (window_import);
}

void import_print_file_name(GtkWidget *window)//获取导入功能选择文件的名字
{
	gchar *import_file_name;
	import_file_name=gtk_file_chooser_get_filename (window);
	import_create_file_view(import_file_name);
	//system();
	g_printf("%s\n",import_file_name);
}
void import_selcet_file(GtkWidget *window,gpointer data)//创建导入文件的选择界面
{
	GtkWidget *dialog;
	
	dialog=gtk_file_chooser_dialog_new(_("please select import files"),window,GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,
	                                   GTK_RESPONSE_OK,NULL);
	if(gtk_dialog_run (dialog)==GTK_RESPONSE_OK){
		//g_printf("oooooo\n");
		import_print_file_name (dialog);
		gtk_widget_destroy (dialog);
	}
	else{
	//g_printf("NNNNNN\n");
		gtk_widget_destroy (dialog);
	}

}
/*----------导入功能up----------------------------------------------------------------*/
void create_view(GtkWidget *container,gpointer data)//容器管理的主界面
{
	GtkTreeViewColumn *column22;
	GtkTreeViewColumn *column33;
	GtkTreeViewColumn *column44;
	GtkWidget *window;
	//GtkWidget *combox_images;
	GtkBuilder *builder;
	GtkTreeIter iter;
	GtkWidget *button11;
	GtkWidget *button22;
	GtkWidget *button33;
	GtkWidget *button44;
	GtkWidget *button55;
	GtkWidget *button66;
	GtkWidget *button77;
	GtkWidget *button88;
	GtkWidget *button99;
	GtkWidget *button110;
	
	GtkCellRenderer *cell11;
	GtkListStore *list11;
	
	GtkCellRenderer *cell;
	GtkTreeSelection *selection;
	static int j;
	docker_contain();
	builder=gtk_builder_new ();
	gtk_builder_add_from_file (builder,CONTAINER_MANAGE_FILE,NULL);
	window=GTK_WIDGET(gtk_builder_get_object (builder,CONTAINER_MANAGE_WINDOW));
	gtk_window_set_title (window,_("contians manage"));
	gtk_widget_set_name(window, "nfs_container_mange_window");
	button11=GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	button22=GTK_WIDGET(gtk_builder_get_object (builder,"button2"));
	button33=GTK_WIDGET(gtk_builder_get_object (builder,"button3"));
	button44=GTK_WIDGET(gtk_builder_get_object (builder,"button4"));
	button55=GTK_WIDGET(gtk_builder_get_object (builder,"button5"));
	button66=GTK_WIDGET(gtk_builder_get_object (builder,"button6"));
	button77=GTK_WIDGET(gtk_builder_get_object (builder,"button7"));
	button88=GTK_WIDGET(gtk_builder_get_object (builder,"button8"));
	button99=GTK_WIDGET(gtk_builder_get_object (builder,"button9"));
	button110=GTK_WIDGET(gtk_builder_get_object (builder,"button10"));
	
	column22=GTK_WIDGET(gtk_builder_get_object (builder,"treeviewcolumn1"));
	column33=GTK_WIDGET(gtk_builder_get_object (builder,"treeviewcolumn2"));
	column44=GTK_WIDGET(gtk_builder_get_object (builder,"treeviewcolumn3"));
	gtk_tree_view_column_set_title (column22,_("Mirror image"));
	cell=gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column22, cell,FALSE);
	gtk_tree_view_column_set_attributes(column22,cell,"text",0,NULL);
	
	gtk_tree_view_column_set_title (column33,"ID");
	cell=gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column33,cell,FALSE);
	gtk_tree_view_column_set_attributes(column33,cell,"text",1,NULL);
	
	gtk_tree_view_column_set_title (column44,_("name_colum"));
	cell=gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column44,cell,FALSE);
	gtk_tree_view_column_set_attributes(column44,cell,"text",2,NULL);
	
	gtk_button_set_label (GTK_BUTTON(button11),_("starting contain and enter"));
	
	gtk_button_set_label (GTK_BUTTON(button22),_("stop contian"));
         gtk_button_set_label (GTK_BUTTON(button33),_("create gurd contian"));
         gtk_button_set_label (GTK_BUTTON(button44),_("create contian"));
         gtk_button_set_label (GTK_BUTTON(button55),_("export"));
	
	gtk_button_set_label (GTK_BUTTON(button66),_("ls starting contain"));
	gtk_button_set_label (GTK_BUTTON(button77),_("ls gurd contain"));
	gtk_button_set_label (GTK_BUTTON(button88),_("dele on contian"));
	gtk_button_set_label (GTK_BUTTON(button99),_("dele mul contians"));
	gtk_button_set_label (GTK_BUTTON(button110),_("import"));
	
	tree_view=GTK_WIDGET(gtk_builder_get_object (builder,"treeview1"));
	list11=gtk_list_store_new (4, G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING);
	gtk_tree_view_set_model (GTK_TREE_VIEW(tree_view),GTK_TREE_MODEL(list11));
	selection=gtk_tree_view_get_selection (GTK_TREE_VIEW(tree_view));

	

	
	//mod=gtk_tree_view_get_model (GTK_TREE_VIEW(tree_view));
	g_signal_connect(G_OBJECT(tree_view),"cursor-changed",G_CALLBACK(clicked_row11),selection);
	//g_signal_connect(G_OBJECT(selection),"changed",G_CALLBACK(clicked_row11),selection);
	g_signal_connect(G_OBJECT(button11),"clicked",G_CALLBACK(button11_treeview_fun),selection);
	g_signal_connect(G_OBJECT(button88),"clicked",G_CALLBACK(button88_treeview_fun),selection);
	g_signal_connect(G_OBJECT(button44),"clicked",G_CALLBACK(button44_treeview_fun),selection);
	g_signal_connect(G_OBJECT(button33),"clicked",G_CALLBACK(button33_treeview_fun),selection);
	g_signal_connect(G_OBJECT(button99),"clicked",G_CALLBACK(button99_treeview_fun),selection);
	g_signal_connect(G_OBJECT(button66),"clicked",G_CALLBACK(button33_fun),"docker ps");
	g_signal_connect(G_OBJECT(button22),"clicked",G_CALLBACK(button22_treeview_fun),selection);
	g_signal_connect(G_OBJECT(button110),"clicked",G_CALLBACK(import_selcet_file),window);
	g_signal_connect(G_OBJECT(button55),"clicked",G_CALLBACK(export_selcet_file),selection);
	g_signal_connect(G_OBJECT(button77),"clicked",G_CALLBACK(button77_lsgurd_fun),window);
	
		for(j=0;j<contains_num;j++){
	gtk_list_store_append (list11,&iter);
	gtk_list_store_set (list11, &iter, 0,&a[j].image,-1);
	gtk_list_store_set (list11, &iter, 1,&a[j].id,-1);
	gtk_list_store_set (list11, &iter, 2,&a[j].name,-1);
		}
	
	g_object_unref (builder);
	
	gtk_widget_show_all (window);
	//return tree_view;

}

