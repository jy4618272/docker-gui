#include "global/global.h"
#include "nfs_volume.h"

#define BACKUP_TAR_LOCAL_PATH "$(pwd)/src/volume_tar"  /* 存放tar包(备份数据卷)的本地目录 */
#define BACKUP_TAR_DOCKER_PATH "/mytest"        /* 备份时,将tar包目录挂载到容器之中的目录 */
#define DOCKER_LOCAL_PATH "/var/lib/docker/containers"
#define BACKUP_VOLUME_FILE UI_DIR_PATH"/nfs_volume_backup.ui"
#define VOLUME_FILE UI_DIR_PATH"/nfs_volume.ui"
#define CREATE_VOLUMN_FILE UI_DIR_PATH"/nfs_volume_create.ui"
#define CREATE_CONTAINER_FILE UI_DIR_PATH"/nfs_volume_container_create.ui"
#define LOOK_VOLUME_FILE  UI_DIR_PATH"/nfs_volume_look.ui"
#define NO_CONTAINER_FLAG 1
#define NO_DATA_FLAG      2
#define NO_CONTAINER      3
#define NO_IMAGES         4
#define NO_CHOICE         5
#define NO_NAMES		  6
#define RETURN_OK         0
#define DATA_VOLUME_FLAG  1
#define CONTAINER_FLAG    2 
#define BACKUP_FLAG		  3
#define RECOVERY_FLAG     4
#define TRANSFER_FLAG     5
#define DATA1_FLAG        1
#define DATA2_FLAG        2
#define DATA3_FLAG        3

static gint volume_flag = 0;
static gboolean SELECT_ALL_FLAG=FALSE;
static gboolean SELECT_ALL_BACKUP_FLAG=FALSE;

typedef struct data
{
	gint  num;
	gchar name[24];
	gchar volumn_name[32];
	gchar mount_name[32];
	gchar image[32];
	gchar local[256];
}Datas;


void volume_event(GtkWidget *button, GtkWidget *window)
{	
	GtkBuilder *builder       = NULL;
	GError     *error         = NULL;
	GtkWidget  *volume_window = NULL;
	
	builder = gtk_builder_new ();
	
	if (!gtk_builder_add_from_file (builder, VOLUME_FILE, &error))
	{
		g_critical (_("Couldn't load builder file: %s"), error->message);
		g_error_free (error);
	}

	/* 如果不存在volume_tar文件夹就创建，用来存放备份，恢复和迁移数据卷的信息 */
	if(NULL == opendir("src/volume_tar"))
		mkdir("src/volume_tar", 0755);
	
//	gtk_widget_hide (window);
	volume_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	gtk_window_set_title (GTK_WINDOW(volume_window),_("data manage"));
	gtk_window_set_default_size (GTK_WINDOW(volume_window),700,500);
	gtk_widget_set_name (volume_window, "volume_window");
	
	GtkWidget *look_volume_button = GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	GtkWidget *create_volume_button = GTK_WIDGET(gtk_builder_get_object (builder,"button2"));
	GtkWidget *look_volume_container_button = GTK_WIDGET(gtk_builder_get_object (builder,"button3"));
	GtkWidget *create_volume_container_button = GTK_WIDGET(gtk_builder_get_object (builder,"button4"));
	GtkWidget *backup_button = GTK_WIDGET(gtk_builder_get_object (builder,"button5"));
	GtkWidget *recovery_button = GTK_WIDGET(gtk_builder_get_object (builder,"button6"));
	GtkWidget *transfer_button = GTK_WIDGET(gtk_builder_get_object (builder,"button7"));
	GtkWidget *look_all_button = GTK_WIDGET(gtk_builder_get_object (builder,"button8"));

	gtk_button_set_label (GTK_BUTTON(look_volume_button), _("look up datavolume"));
	gtk_button_set_label (GTK_BUTTON(create_volume_button), _("create datavolume"));
	gtk_button_set_label (GTK_BUTTON(look_volume_container_button), _("look up datavolume container"));
	gtk_button_set_label (GTK_BUTTON(create_volume_container_button), _("create datavolume container"));
	gtk_button_set_label (GTK_BUTTON(backup_button), _("backup datavolume"));
	gtk_button_set_label (GTK_BUTTON(recovery_button), _("recovrty datavolume"));
	gtk_button_set_label (GTK_BUTTON(transfer_button), _("transfer datavolume"));
	gtk_button_set_label (GTK_BUTTON(look_all_button), _("look all datavolume"));
	
	g_signal_connect(G_OBJECT(look_volume_button),"clicked",G_CALLBACK(data_volume_look),volume_window);
	g_signal_connect(G_OBJECT(create_volume_button),"clicked",G_CALLBACK(data_volume_create),volume_window);
	g_signal_connect(G_OBJECT(look_volume_container_button),"clicked",G_CALLBACK(container_look),volume_window);
	g_signal_connect(G_OBJECT(create_volume_container_button),"clicked",G_CALLBACK(container_create),volume_window);
	g_signal_connect(G_OBJECT(backup_button),"clicked",G_CALLBACK(data_volume_backup),volume_window);
	g_signal_connect(G_OBJECT(recovery_button),"clicked",G_CALLBACK(data_volume_recovery),volume_window);
	g_signal_connect(G_OBJECT(transfer_button),"clicked",G_CALLBACK(data_volume_transfer),volume_window);
	g_signal_connect(G_OBJECT(look_all_button),"clicked",G_CALLBACK(data_volume_look_all),volume_window);	
	g_signal_connect(G_OBJECT(volume_window),"destroy",G_CALLBACK(volume_quit),window);
	gtk_widget_show_all(volume_window);
}

void volume_quit(GtkWidget *volume_window, GtkWidget *main_window)
{
	gtk_widget_destroy (volume_window);
//	gtk_widget_show(main_window);
}

void data_volume_create(GtkWidget *widget, GtkWidget *window)
{
	GtkBuilder *builder;
	GtkWidget *data_volume_create_window;
	GtkWidget *button_ok;
	GtkWidget *button_cancel;
	GError* error = NULL;

	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, CREATE_VOLUMN_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	data_volume_create_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	gtk_window_set_title (GTK_WINDOW(data_volume_create_window), _("create datavolume"));
	gtk_window_set_default_size (GTK_WINDOW(data_volume_create_window),700,500);
	button_ok = GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	button_cancel = GTK_WIDGET(gtk_builder_get_object (builder,"button2"));
	
	GtkWidget *label1 = GTK_WIDGET(gtk_builder_get_object (builder,"label1"));
	GtkWidget *label2 = GTK_WIDGET(gtk_builder_get_object (builder,"label2"));
	GtkWidget *label3 = GTK_WIDGET(gtk_builder_get_object (builder,"label3"));
	GtkWidget *label4 = GTK_WIDGET(gtk_builder_get_object (builder,"label4"));
	GtkWidget *label5 = GTK_WIDGET(gtk_builder_get_object (builder,"label5"));
	GtkWidget *label6 = GTK_WIDGET(gtk_builder_get_object (builder,"label6"));
	GtkWidget *label7 = GTK_WIDGET(gtk_builder_get_object (builder,"label7"));
	
	gtk_label_set_text(GTK_LABEL(label1),_("the name of container:"));
	gtk_label_set_text(GTK_LABEL(label2),_("the name of datavolume:"));
	gtk_label_set_text(GTK_LABEL(label3),_("mount local directory as a datavolume:"));
	gtk_label_set_text(GTK_LABEL(label4),_("it could only include numbers,letters and \"-\",\"_\",\".\""));
	gtk_label_set_text(GTK_LABEL(label5),_("choose image"));
	gtk_label_set_text(GTK_LABEL(label6),_("could mount many or null, for exaple\n \
											/test:/mytest mount local directory to new \
											directory\n if directory not exists, create it(optional)\n"));
	gtk_label_set_text(GTK_LABEL(label7),_("datavolume must add absolute path, for example:/test\n \
											if add many datavolumes, separated by space"));
	
	combo_box_add_image(builder);
	g_signal_connect(G_OBJECT(button_cancel),"clicked",G_CALLBACK(data_volume_create_cancel),builder);
	g_signal_connect(G_OBJECT(button_ok),"clicked",G_CALLBACK(data_volume_create_ok),builder);
	gtk_widget_show_all(data_volume_create_window);
}

/* 给下拉列表添加镜像 */
void combo_box_add_image(GtkBuilder *builder)
{
	GtkComboBoxText *text = GTK_COMBO_BOX_TEXT(gtk_builder_get_object (builder,"comboboxtext1"));
	FILE *fp = NULL;
	gchar *str= NULL;
	gchar result[2048] = {0};
	gint total_image= 0;
	gint ix= 0;
	str = (gchar *)g_malloc(sizeof(gchar)*128);

	/* 获得镜像的个数 */
	sprintf(str,"sudo docker images | awk 'END{print NR}'");
	strcpy(result, popen_and_fread (str));
	total_image = atoi(result);

	memset(str, '\0', strlen(str));
	fp = fopen("tmp.txt","w");
	memset(result, '\0', sizeof(result));
	sprintf(str,"sudo docker images");
	strcpy(result,popen_and_fread (str));
	fwrite(result, 1, sizeof(result), fp);
	fclose(fp);

	/* 依次将镜像添加到列表 */
	for(ix=2; ix<=total_image; ix++)
	{
		sprintf(str,"cat tmp.txt | awk '{print $1\":\"$2}' | sed -n \"%d,1p\"",ix);
		strcpy(result, popen_and_fread (str));
		gtk_combo_box_text_append_text (text,result);
	}
	
	remove("tmp.txt");
	g_free(str);
	str = NULL;
}

void data_volume_create_cancel(GtkWidget *widget, GtkBuilder *builder)
{
	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	gtk_widget_destroy (GTK_WIDGET(window));
}

void data_volume_create_ok(GtkWidget *widget, GtkBuilder *builder)
{
	gchar str[1024] = {0}; 
	GtkComboBoxText *combo_box;
	gchar *combo_box_text = NULL;    
	
	if(RETURN_OK != data_volume_create_get_entry_text(builder, str))
	{
		return;
	}

	combo_box= GTK_COMBO_BOX_TEXT(gtk_builder_get_object (builder,"comboboxtext1"));
	combo_box_text  = gtk_combo_box_text_get_active_text(combo_box);
	if(!(combo_box_text && *combo_box_text))
	{
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("please choose image!!!"));
		return;
	}
	
	sprintf(str,"%s %s",str, combo_box_text);
	create_event(builder, str);
}

void data_volume_create_response(GtkWidget *dialog, GtkBuilder *builder)
{
	gtk_widget_destroy (dialog);
	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	gtk_widget_destroy (GTK_WIDGET(window));
}

/* 获得输入entry构件里的内容 */
gint data_volume_create_get_entry_text (GtkBuilder *builder, gchar *str)
{
	GtkWidget *entry1 = GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	GtkWidget *entry2 = GTK_WIDGET(gtk_builder_get_object (builder,"entry2"));
	GtkWidget *entry3 = GTK_WIDGET(gtk_builder_get_object (builder,"entry3"));
	const gchar* text1 = gtk_entry_get_text (GTK_ENTRY(entry1));
	const gchar* text2 = gtk_entry_get_text (GTK_ENTRY(entry2));
	const gchar* text3 = gtk_entry_get_text (GTK_ENTRY(entry3));
	
	if(!(text1 && *text1))
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, 
		                    GTK_BUTTONS_OK, _("please input container name!!!"));
		return NO_CONTAINER_FLAG;
	}

	/*
	char *p = text1;
	while(*p != '\0')
	{
		if(*p != ' ')
			p++;
		else
		{
			message_dialog_new (NULL, GTK_MESSAGE_WARNING, 
		            GTK_BUTTONS_OK, _("please input only one name!!!"));
			return NO_CONTAINER_FLAG;
		}
	}
	*/

	if(!(text2 && *text2))
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, 
		                    GTK_BUTTONS_OK, _("please imput datavolume name!!!"));
		return NO_DATA_FLAG;
	}

	gchar pVolume[1024] = {0}; /* 用来提取多个数据卷名称的中间变量 */
	strcpy(pVolume,text2);
	sprintf(str,"sudo docker run -itd --name %s",text1);
	
	strcpy(str, data_volume_create_get_data_name (str, pVolume)); /*用来提取多个数据卷名称*/

	if(text3 && *text3)  /*用来提取多个本地数据卷名称*/
	{
		memset(pVolume, '\0', sizeof(pVolume));
		strcpy(pVolume,text3);
		strcpy(str, data_volume_create_get_data_name (str, pVolume));
	}
	return RETURN_OK;
}

/* 用于提取多个数据卷名称 */
gchar * data_volume_create_get_data_name (gchar *str, const gchar *msg)
{
	gchar *pData   = NULL;
	gchar pVolume[1024] = {0};
	strcpy(pVolume,msg);
	pData = strtok(pVolume," ");

	if(pData)
	{
		sprintf(str,"%s -v %s",str,pData);
	}
		
	pData = strtok(NULL, " ");
	while(pData)
	{
		sprintf(str,"%s -v %s",str,pData);
		pData = strtok(NULL, " ");
	}

	return str;
}

void data_volume_look(GtkWidget *widget, GtkWidget *window1)
{
	GtkBuilder *builder = gtk_builder_new();
	GError* error = NULL;
	if (!gtk_builder_add_from_file (builder, LOOK_VOLUME_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	GtkWidget *look_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	GtkWidget *del_button    = GTK_WIDGET(gtk_builder_get_object (builder,"del_button"));
	GtkWidget *open_button  = GTK_WIDGET(gtk_builder_get_object (builder,"open_button"));
	GtkWidget *check_button = GTK_WIDGET(gtk_builder_get_object (builder,"checkbutton1"));
	GtkWidget *return_button = GTK_WIDGET(gtk_builder_get_object (builder,"return_button"));
	GtkWidget *label1        = GTK_WIDGET(gtk_builder_get_object (builder, "label1"));
	GtkWidget *label3        = GTK_WIDGET(gtk_builder_get_object (builder, "label3"));
	GtkWidget *label4        = GTK_WIDGET(gtk_builder_get_object (builder, "label4"));
	
	gtk_window_set_title (GTK_WINDOW(look_window),_("look up datavolume"));
	gtk_window_set_default_size (GTK_WINDOW(look_window),700,500);
	gtk_window_set_position (GTK_WINDOW(look_window),GTK_WIN_POS_CENTER);

	gtk_label_set_text (GTK_LABEL(label1), _("select one or more datavolumes"));
	gtk_label_set_text (GTK_LABEL(label3), _("remove the datavolume you select"));
	gtk_label_set_text (GTK_LABEL(label4), _("eopn the datavolume you select"));
	gtk_button_set_label (GTK_BUTTON(check_button), _("a key to select all"));
	
	volume_flag = DATA_VOLUME_FLAG;
	data_volume_look_add_column(builder);
	data_volume_look_add_data(builder,volume_flag);
	
	g_signal_connect(G_OBJECT(check_button),"clicked",G_CALLBACK(select_all),builder);
	g_signal_connect(G_OBJECT(del_button),"clicked",G_CALLBACK(data_volume_look_del),builder);
	g_signal_connect(G_OBJECT(open_button),"clicked",G_CALLBACK(data_volume_look_open),builder);
	g_signal_connect_swapped(G_OBJECT(return_button),"clicked",G_CALLBACK(gtk_widget_destroy), look_window);
	gtk_widget_show_all(look_window);
}

void data_volume_look_add_column(GtkBuilder *builder)
{
	GtkTreeView *view = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));

	tree_column_new (view, 0, _("Num"));
	tree_column_new (view, 1, _("Name"));
	tree_column_new (view, 2, _("Image"));
	tree_column_new (view, 3, _("create data"));
	tree_column_new (view, 4, _("mount data"));
	tree_column_new (view, 5, _("local data"));

	GtkCellRenderer *renderer;
	GtkTreeModel *model = gtk_tree_view_get_model (view);
	
	renderer = gtk_cell_renderer_toggle_new ();
	g_object_set (renderer, "xalign", 0.0, NULL);
	g_object_set_data (G_OBJECT (renderer), "column", (gint *)6);

	g_signal_connect (renderer, "toggled", G_CALLBACK (item_toggled), model);
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                          -1, _("Choice"),renderer,"active",6,NULL);
}

void item_toggled (GtkCellRendererToggle *cell,gchar *path_str, gpointer data)
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
  GtkTreeIter iter;
  gboolean toggle_item;

  gint *column;
  gchar *str = NULL;
  column = g_object_get_data (G_OBJECT (cell), "column");

  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, column, &toggle_item,1,&str, -1);

  /* do something with the value */
  toggle_item ^= 1;

  /* set new value */
  gtk_tree_store_set (GTK_TREE_STORE (model), &iter, column,
                      toggle_item, -1);
  gtk_tree_path_free (path);
}

void data_volume_look_add_data(GtkBuilder *builder, gint flag)
{
	GtkTreeStore *list = GTK_TREE_STORE(gtk_builder_get_object (builder,"treestore1"));
	GtkTreeIter iters;
	
	gchar str[1024] = {0};
	gchar str1[1024] = {0};
    gchar container_id[70] = {0};
    gchar find_path1[128] = {0};
    gchar name[128] = {0};
    gchar image[128] = {0};
    gchar volume[128] = {0};  /* 自己创建的数据卷 */
    gchar volume_container[1280] = {0} ; /* 数据卷容器里的数据卷 */
    gchar local[128] = {0};  /* 挂载本地的目录 */
    gchar *pData = NULL;
    gchar tmp[128] = {0};
    DIR            *pDir;   
    struct dirent  *ent;
	gint num = 0;
	
    pDir = opendir(DOCKER_LOCAL_PATH);
    if(pDir == NULL)
    {
        printf("path failed\n");
        return ;
    }

    while((ent=readdir(pDir))!=NULL)    /* 遍历该路径目录 */
    {
        if(ent->d_type == DT_DIR)
        {
            if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
                continue;
            
    //     设置寻找路径　
			memset(container_id, '\0', sizeof(container_id));
			memset(find_path1, '\0', sizeof(find_path1));
            sprintf(container_id,"%s",ent->d_name);
            sprintf(find_path1,"/var/lib/docker/containers/%s/config.json",container_id);

    //     查找镜像
			memset(str, '\0', sizeof(str));
			memset(image, '\0', sizeof(image));
  			sprintf(str,"cat %s | awk -F \"Image\" '{print $2}'" 
							   "| awk -F '\"' '{print $3}' ",find_path1);
			strcpy(image,popen_and_fread (str));

    //    查找名称　　　
            memset(str, '\0', sizeof(str));
			memset(name, '\0', sizeof(name));
            sprintf(str,"cat %s | awk -F \"Name\" '{print $2}' "
                			   "| awk -F '\"' '{print $3}' "
                			   "| awk -F \"/\" '{print $2}'",find_path1);
			strcpy(name,popen_and_fread (str));
			
    //   查找数据卷　　
            memset(str, '\0', sizeof(str));
			memset(volume, '\0', sizeof(volume));
            sprintf(str,"cat %s | awk -F Volumes '{print $2}' "
                			   "| awk -F WorkingDir '{print $1}' "
                			   "| awk -F : '{for(i=2;i<=NF;++i) printf $i\":\"}'"
                               "| awk -F , '{for(i=1;i<NF;i++) if(i==NF-1){printf $i} "
                			     "else{printf $i\",\"}}'",find_path1);
			strcpy(volume,popen_and_fread (str));

    //  查找挂载的目录以及数据卷容器    
			memset(volume_container, '\0', sizeof(volume_container));
			memset(local, '\0', sizeof(local));
            memset(str, '\0', sizeof(str));
            sprintf(str1,"cat %s | awk -F Volumes '{i=NF-1; printf $i}' "
                		        "| awk -F { '{printf $2}' "
                				"| awk -F } '{printf $1}'",find_path1);
			strcpy(str,popen_and_fread (str1));

            pData = strtok(str,",");
            if(pData)    
            {
                if(strstr(pData,"/var/lib/docker"))    /* 获取数据卷容器 */                                                                                                          
     	        {
                    sscanf(pData,"%[^:]",tmp);
                    strcat(volume_container,tmp);
                    strcat(volume_container,"  ");
                }
                else    /* 获取挂载的本地目录 */
                {
                    strcat(local,pData);
					strcat(local,"  ");
                } 
            }
			
            pData = strtok(NULL,",");
            while(pData)
            {
                if(strstr(pData,"/var/lib/docker"))   /* 获取数据卷容器 */
                {
                    sscanf(pData,"%[^:]",tmp);
                    strcat(volume_container,tmp);
                    strcat(volume_container,"  ");
                }
                else   /* 获取挂载的本地目录 */
                {
                    strcat(local,pData);
                    strcat(local,"  ");
                } 
                pData = strtok(NULL,",");
            }
			
			if(flag == DATA_VOLUME_FLAG)
			{
				if(volume_container[0]==0)
				{
					num++;
					gtk_tree_store_append (list, &iters,NULL);
					gtk_tree_store_set (list, &iters,
    		    		  		0, num,
       		        			1, name,
		                		2, image,
		                		3, volume,
		                		4, volume_container,
				        		5, local,
		                		6, SELECT_ALL_FLAG,
        		      			-1);
				}
			}

			if(flag == CONTAINER_FLAG)
			{
				if(volume_container[0]!=0)
				{
					num++;
					gtk_tree_store_append (list, &iters,NULL);
					gtk_tree_store_set (list, &iters,
    		    		  		0, num,
       		        			1, name,
		                		2, image,
		                		3, volume,
		                		4, volume_container,
				        		5, local,
		                		6, SELECT_ALL_FLAG,
        		      			-1);
				}
			}

			if(flag == BACKUP_FLAG)
			{
				num++;
				gtk_tree_store_append (list, &iters,NULL);
				gtk_tree_store_set (list, &iters,
    		    		  		0, num,
       		        			1, name,
		                		2, volume,
		                		3, volume_container,
				        		4, local,
		                		6, SELECT_ALL_FLAG,
        		      			-1);
		   }
        }
        else
            printf("%s\n",ent->d_name);
    }
}

void container_create(GtkWidget *widget, GtkWidget *window)
{
	GtkBuilder *builder = gtk_builder_new();
	GError* error = NULL;
	if (!gtk_builder_add_from_file (builder, CREATE_CONTAINER_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	GtkWidget *container_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	GtkWidget *button_ok        = GTK_WIDGET(gtk_builder_get_object (builder,"button_ok"));
	GtkWidget *button_cancel    = GTK_WIDGET(gtk_builder_get_object (builder,"button_cancel"));
	GtkWidget *button_rebuild   = GTK_WIDGET(gtk_builder_get_object (builder,"button_rebuild"));
	GtkWidget *entry            = GTK_WIDGET(gtk_builder_get_object(builder, "entry1")); 
	GtkWidget *label1        = GTK_WIDGET(gtk_builder_get_object (builder, "label1"));
	GtkWidget *label2        = GTK_WIDGET(gtk_builder_get_object (builder, "label2"));
	GtkWidget *label4        = GTK_WIDGET(gtk_builder_get_object (builder, "label4"));

	gtk_label_set_text (GTK_LABEL(label1), _("select one or more datavolumes"));
	gtk_label_set_text (GTK_LABEL(label2), _("the name of datavolume container"));
	gtk_label_set_text (GTK_LABEL(label4), _("select images"));
	gtk_button_set_label (GTK_BUTTON(button_rebuild), _("rebuild the datavolume"));
	
	gtk_window_set_default_size(GTK_WINDOW(container_window),700,500);
	gtk_window_set_title (GTK_WINDOW(container_window),_("create datavolume container"));
//	gtk_widget_set_tooltip_text(entry, "只能包含数字，字母以及\"-\",\"_\",\".\"");
	gtk_entry_set_placeholder_text (GTK_ENTRY(entry), _("it could only include number,letters and \"-\",\"_\",\".\""));
//	gtk_entry_set_visibility (GTK_ENTRY(entry), FALSE);
//	gtk_entry_set_invisible_char (GTK_ENTRY(entry), 'q');
	
	
	combo_box_add_image(builder);
	volume_flag = DATA_VOLUME_FLAG;
	data_volume_look_add_column(builder);
	data_volume_look_add_data(builder,volume_flag);

	g_signal_connect(G_OBJECT(button_ok),"clicked",G_CALLBACK(container_create_ok),builder);
	g_signal_connect_swapped(G_OBJECT(button_cancel),"clicked",G_CALLBACK(gtk_widget_destroy ),container_window);
	g_signal_connect(G_OBJECT(button_rebuild),"clicked",G_CALLBACK(container_create_rebuild),container_window);
	gtk_widget_show_all(container_window);
}

void container_create_rebuild(GtkWidget *button, GtkWidget *window)
{
	gtk_widget_destroy (window);
	data_volume_create(button,NULL);
}

void container_create_ok(GtkWidget *button, GtkBuilder *builder)
{
	gchar str[1024]    = {0};

	if(NO_CHOICE == container_create_get_data(builder, str))
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("please choose the datavolume first！"));
		return;
	}

	if(NO_DATA_FLAG == container_create_get_data(builder, str))
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, 
		                    _("there is no datavolume to choose, please create it first！"));
		GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
		gtk_widget_destroy (window);
		return;
	}
	
	if(RETURN_OK != container_create_get_text(builder, str))
	{
		printf("%s\n",str);
		return;
	}

	
	create_event (builder, str);
}

/* 获得输入entry构件里的内容 */
gint container_create_get_text (GtkBuilder *builder, gchar *str)
{
	GtkEntry    *entry  = NULL;
	const gchar *entry_text = NULL;
	GtkComboBoxText *combo_box = NULL;
	gchar *combo_text = NULL;

	entry = GTK_ENTRY(gtk_builder_get_object (builder,"entry1"));
	entry_text = gtk_entry_get_text (entry);
	
	if(!(entry_text && *entry_text))
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING ,GTK_BUTTONS_OK,
		                    _("please input the name of datavolume container!!!"));
		return NO_CONTAINER;
	}

/*
	char *p = entry_text;
	while(*p != '\0')
	{
		if(*p != ' ')
			p++;
		else
		{
			p++;
			if(*p == ' ')
				p++;
			else if(*p == '\0')
				break;
			else
			{
				message_dialog_new (NULL, GTK_MESSAGE_WARNING, 
		            GTK_BUTTONS_OK, _("please input only one name!!!"));
				return  NO_CONTAINER;
			}
		}
	}
*/	
	sprintf(str,"%s --name %s",str,entry_text);

	combo_box = GTK_COMBO_BOX_TEXT(gtk_builder_get_object (builder,"comboboxtext1"));
	combo_text = gtk_combo_box_text_get_active_text(combo_box);
	
	if(!(combo_text && *combo_text))
	{
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("please choose image!!!"));
		return NO_IMAGES;
	}
	
	sprintf(str,"%s %s",str,combo_text);
	return RETURN_OK;
}  

/* 获得数据卷名称，如果数据卷为空就返回 */
gint container_create_get_data (GtkBuilder *builder, gchar *str)
{
	GtkTreeView *view   = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkTreeModel *model = gtk_tree_view_get_model (view);
	GtkTreeIter iter;
	gboolean flg = FALSE;
	gboolean data_flg = FALSE;
	gchar *str1 = NULL;
	
	str1 = (gchar *)g_malloc(sizeof(gchar)*128);
	
	if(NULL == str1)
		return -1;
	
	sprintf(str,"sudo docker run -idt");

	//迭代器定位到第一行
	if(gtk_tree_model_get_iter_first(model,&iter))
	{		
		do
		{
			gtk_tree_model_get(model, &iter, 6, &flg, 1, &str1, -1);
			if(flg)
			{
				sprintf(str,"%s --volumes-from %s",str,str1);
				memset(str1, '\0', strlen(str1));
				data_flg = TRUE;
			}
		}while(gtk_tree_model_iter_next (model, &iter));

		if(!data_flg)  /* 没有选择数据卷 */
		{
			g_free(str1);
			return NO_CHOICE;
		}
	}
	else  /* 没有数据卷 */
	{
		return NO_DATA_FLAG;
	}
	
	g_free(str1);
	return RETURN_OK;
}

void container_look(GtkWidget *widget, GtkWidget *window1)
{
	GtkBuilder *builder = gtk_builder_new();
	GError* error = NULL;
	if (!gtk_builder_add_from_file (builder, LOOK_VOLUME_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	GtkWidget *container_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	GtkWidget *del_button    = GTK_WIDGET(gtk_builder_get_object (builder,"del_button"));
	GtkWidget *open_button  = GTK_WIDGET(gtk_builder_get_object (builder,"open_button"));
	GtkWidget *check_button = GTK_WIDGET(gtk_builder_get_object (builder,"checkbutton1"));
	GtkWidget *return_button = GTK_WIDGET(gtk_builder_get_object (builder,"return_button"));

	gtk_window_set_title (GTK_WINDOW(container_window),_("look up datavolume container"));
	gtk_window_set_default_size (GTK_WINDOW(container_window),700,500);
	gtk_window_set_position (GTK_WINDOW(container_window),GTK_WIN_POS_CENTER);
	
	volume_flag = CONTAINER_FLAG;
	data_volume_look_add_column(builder);
	data_volume_look_add_data(builder,volume_flag);
	
	g_signal_connect(G_OBJECT(check_button),"clicked",G_CALLBACK(select_all),builder);
	g_signal_connect(G_OBJECT(del_button),"clicked",G_CALLBACK(data_volume_look_del),builder);
	g_signal_connect(G_OBJECT(open_button),"clicked",G_CALLBACK(data_volume_look_open),builder);
	g_signal_connect_swapped(G_OBJECT(return_button),"clicked",G_CALLBACK(gtk_widget_destroy), container_window);
	gtk_widget_show_all(container_window);
}

void data_volume_look_del(GtkWidget *widget, GtkBuilder *builder)
{
	GtkTreeView *view = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkTreeModel *model = gtk_tree_view_get_model (view);
	GtkTreeIter iter;
	GtkTreeStore *list = GTK_TREE_STORE(gtk_builder_get_object (builder,"treestore1"));

	gboolean flag;
	gchar str[1024] = {0};
	gchar *str1 = NULL;
	str1 = (gchar *)g_malloc(sizeof(gchar)*128);
	if(NULL == str1)
		return;
	sprintf(str,"sudo docker rm -f -v");

	if(gtk_tree_model_get_iter_first(model,&iter))//迭代器定位到第一行
	{
		gboolean del_flag = FALSE;   /* 设置标志查看是否有选择删除项 */
		do
		{
			gtk_tree_model_get(model, &iter, 6, &flag, 1, &str1, -1);
			if(flag)
			{
				sprintf(str,"%s %s",str,str1);
				del_flag = TRUE;
			}
		}while(gtk_tree_model_iter_next (model, &iter));
		
		if(!del_flag)
		{
			message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("please choose the data you want to delete"));
			g_free(str1);
			return;
		}
	}
	else
	{	
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "please choose the data you want to delete");
		g_free(str1);
		return;
	}
	g_free(str1);
	
	FILE *fp = NULL;
	if( NULL == (fp = popen(str,"r")))
	{
		printf("popen failed!  %s  %d\n",__FILE__,__LINE__);
		return;
	}
	pclose(fp);

	gtk_tree_store_clear (GTK_TREE_STORE(list));
	data_volume_look_add_data(builder,volume_flag);
}

void data_volume_look_open(GtkWidget *widget, GtkBuilder *builder)
{
	GtkTreeView *view = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkTreeModel *model = gtk_tree_view_get_model (view);
	GtkTreeIter iter;
	
	gchar str[1024] = {0};
	gchar *str1 = NULL;
	str1 = (gchar *)g_malloc(sizeof(gchar)*128);
	gboolean flag;
	gint num = 0;
	
	if(gtk_tree_model_get_iter_first(model,&iter))//迭代器定位到第一行
	{
		gboolean open_flag = FALSE; 
		do
		{
			gtk_tree_model_get(model, &iter, 6, &flag, 1, &str1, -1);
		
			if(flag)
			{
				num++;
				if(num>=2)
					sprintf(str,"%s && sudo gnome-terminal -x bash -c \"sudo docker start -ai %s\"",str,str1);
				else
					sprintf(str,"sudo gnome-terminal -x bash -c \"sudo docker start -ai %s\"",str1);
				open_flag = TRUE;
			}
		}while(gtk_tree_model_iter_next (model, &iter));

		g_free(str1);
		if(!open_flag)
		{
			message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("please choose the data you want to open"));
			return;
		}
	}
	else
	{	
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("please choose the data you want to open"));
		g_free(str1);
		return;
	}

	FILE *fp = NULL;
	if( NULL == (fp = popen(str,"r")))
	{
		printf("popen failed!  %s  %d\n",__FILE__,__LINE__);
		return;
	}
	pclose(fp);
}

/*  一键全选和一键取消 */
void select_all(GtkButton *button, GtkBuilder *builder)
{	
	GtkTreeStore *list = GTK_TREE_STORE(gtk_builder_get_object (builder,"treestore1"));
	gtk_tree_store_clear (GTK_TREE_STORE(list));
	GtkLabel  *label   = GTK_LABEL(gtk_builder_get_object (builder, "label6"));
	SELECT_ALL_FLAG = TRUE;
	

	
	if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(button)))
	{
		gtk_button_set_label (button,_("a key to cancel all"));
		data_volume_look_add_data(builder,volume_flag);

		if(SELECT_ALL_BACKUP_FLAG)
			data_volume_backup_get_label(NULL, NULL, builder);
	}

	else
	{
		SELECT_ALL_FLAG = FALSE;
		gtk_button_set_label (button,_("a key to select all"));
		data_volume_look_add_data(builder,volume_flag);

		if(SELECT_ALL_BACKUP_FLAG)
			gtk_label_set_text (label, "");
	}

	SELECT_ALL_BACKUP_FLAG = FALSE;
	SELECT_ALL_FLAG = FALSE;
}

gboolean create_event(GtkBuilder *builder, gchar *str)
{
	gchar result[1024] = {0};
	const gchar *entry_text = NULL;
	gchar *pData = NULL;

	/* 将标准错误也打印到屏幕上，echo &?, 如果返回值为0则说明命令正确 */
	sprintf(str,"%s 2>&1; echo $?",str);
	strcpy(result,popen_and_fread (str));

	pData = &result[strlen(result)-1];
	
	if(*pData != '0')
	{
		result[strlen(result) - 1] = '\0';
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, result);
		return FALSE;
	}
	
	result[strlen(result) - 1] = '\0';  /* 将echo $? 的结果清除 */

	GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	entry_text = gtk_entry_get_text (GTK_ENTRY(entry));
	memset(str, '\0', strlen(str));
	sprintf(str,"sudo gnome-terminal -x bash -c \"sudo docker start -ia %s\"",entry_text);
	
	GtkWidget *dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL| 
	             		GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,
	                    GTK_BUTTONS_YES_NO, _("the datavolume creates successfully!!"));
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),_("whether to open datavolume？"));
	gint conditions = gtk_dialog_run(GTK_DIALOG(dialog));
		
	switch(conditions)
	{
		case GTK_RESPONSE_YES:  /* 进入到创建的新容器 */
			data_volume_create_response(dialog,builder);
			if(NULL == popen(str,"r"))
			{
				printf(_("popen failed!  %s   %d"),__FILE__,__LINE__);
			}
			break;
		
		case GTK_RESPONSE_NO : /* 直接返回 */
			data_volume_create_response(dialog,builder);
			break;
		
		default:
			break;
	} 

	return TRUE;
}


void data_volume_backup(GtkWidget *widget, GtkWidget *window)
{
	GtkBuilder *builder = gtk_builder_new();
	GError* error = NULL;
	if (!gtk_builder_add_from_file (builder, BACKUP_VOLUME_FILE, &error))
	{
		g_critical (_("Couldn't load builder file: %s"), error->message);
		g_error_free (error);
	}

	GtkWidget *backup_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	GtkWidget *button_ok     = GTK_WIDGET(gtk_builder_get_object (builder,"button_ok"));
	GtkWidget *button_cancel = GTK_WIDGET(gtk_builder_get_object (builder,"button_cancel"));
	GtkWidget *button_look   = GTK_WIDGET(gtk_builder_get_object (builder,"button"));
	GtkWidget *check_button  = GTK_WIDGET(gtk_builder_get_object (builder,"checkbutton1"));
	GtkTreeView     *view    = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkWidget *entry         = GTK_WIDGET(gtk_builder_get_object (builder, "entry2")); 
	GtkWidget *label1         = GTK_WIDGET(gtk_builder_get_object (builder, "label1")); 
	GtkWidget *label2         = GTK_WIDGET(gtk_builder_get_object (builder, "label2")); 
	GtkWidget *label3         = GTK_WIDGET(gtk_builder_get_object (builder, "label3")); 
	GtkWidget *label4         = GTK_WIDGET(gtk_builder_get_object (builder, "label4")); 
	GtkWidget *label5         = GTK_WIDGET(gtk_builder_get_object (builder, "label5")); 
	GtkCellRenderer *renderer = gtk_cell_renderer_toggle_new ();
	
	gtk_window_set_default_size (GTK_WINDOW(backup_window),700,500);
	gtk_window_set_position (GTK_WINDOW(backup_window),GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(backup_window),_("backup datavolume"));

	gtk_label_set_text (GTK_LABEL(label1), _("select one or more datavolume"));
	gtk_label_set_text (GTK_LABEL(label2), _("the datavolume you have chosen to backup"));
	gtk_label_set_text (GTK_LABEL(label3), _("select images"));
	gtk_label_set_text (GTK_LABEL(label4), _("input the name of new tar(no suffix)"));
	gtk_label_set_text (GTK_LABEL(label5), _("please input the name of new directory"));
	gtk_button_set_label (GTK_BUTTON(button_look), _("lookup the datavolume you have backuped"));
	gtk_button_set_label (GTK_BUTTON(check_button), _("a key ro select all"));
	
	volume_flag = BACKUP_FLAG;
	data_volume_backup_add_text_column(view);
	data_volume_backup_add_toggle_column(view, renderer);
	data_volume_look_add_data(builder, volume_flag);
	combo_box_add_image(builder);

	g_signal_connect(G_OBJECT(check_button),"clicked",G_CALLBACK(data_volume_backup_select_all),builder);
	g_signal_connect(G_OBJECT(button_ok), "clicked", G_CALLBACK(data_volume_backup_ok), builder);
	g_signal_connect_swapped(G_OBJECT(button_cancel), "clicked", G_CALLBACK(gtk_widget_destroy), backup_window);
	g_signal_connect (renderer, "toggled", G_CALLBACK (data_volume_backup_get_label), builder);
	
	gtk_widget_show_all (backup_window);
	gtk_widget_hide (label5);
	gtk_widget_hide (entry);
}

void data_volume_backup_select_all(GtkWidget *widget, GtkBuilder *builder)
{
	SELECT_ALL_BACKUP_FLAG = TRUE;
	select_all (GTK_BUTTON(widget),builder);
}

void data_volume_backup_get_label(GtkCellRendererToggle *cell,gchar *path_str, GtkBuilder *builder)
{
	gchar str[1024] = {0};
	gchar data_name[512] = {0};
	GtkLabel *label = GTK_LABEL(gtk_builder_get_object (builder, "label6"));
	
	memset(str, '\0', sizeof(str));
	memset(data_name, '\0', sizeof(data_name));

	if(NULL != cell)
	{
		GtkTreeView     *view     = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
		GtkTreeModel    *model    = gtk_tree_view_get_model (view);
		item_toggled(cell, path_str, model);
	}

	SELECT_ALL_BACKUP_FLAG = TRUE;
	data_volume_backup_get_data(builder, str, data_name);
	SELECT_ALL_BACKUP_FLAG = FALSE;
	
	gtk_label_set_text(label, data_name);
}

void data_volume_backup_add_text_column(GtkTreeView *view)
{
	tree_column_new (view, 0, _("Num"));
	tree_column_new (view, 1, _("Container Name"));
	tree_column_new (view, 2, _("Create DataVolume"));
	tree_column_new (view, 3, _("Mount DataVolume"));
	tree_column_new (view, 4, _("Local DataVolume"));
}

void data_volume_backup_add_toggle_column(GtkTreeView *view, GtkCellRenderer *renderer)
{
	g_object_set (renderer, "xalign", 0.0, NULL);
	g_object_set_data (G_OBJECT (renderer), "column", (gint *)6);
	
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                          -1, _("choice"),renderer,"active",6,NULL);
}

void data_volume_backup_ok(GtkWidget *widget, GtkBuilder *builder)
{
	gchar str[1024] = {0};
	gchar data_name[512] = {0};
	GtkWidget *window = NULL;
	gchar result[1024] = {0};
	gchar *pData = NULL;
	
	memset(str, '\0', sizeof(str));
	memset(data_name, '\0', sizeof(data_name));
	if (RETURN_OK != (data_volume_backup_get_data(builder, str, data_name)))
		return;
	
	data_volume_backup_set_label(builder, data_name);

	sprintf(str,"%s -v %s:%s",str, BACKUP_TAR_LOCAL_PATH, BACKUP_TAR_DOCKER_PATH);

	if(RETURN_OK != (data_volume_backup_get_image(builder,str)))
		return;

	if(RETURN_OK != (data_volume_backup_get_name(builder, str)))
		return;

	sprintf(str,"%s %s 2>&1; echo $?",str,data_name);
	strcpy(result, popen_and_fread (str));
	pData = &result[strlen(result)-1];
	
	if(*pData != '0')
	{
		result[strlen(result) - 1] = '\0';
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, result);
		return;
	}

	message_dialog_new (NULL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, _("backup successfully  ^_^"));

	data_volume_backup_write_info_to_txt(builder, data_name);
	
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	gtk_widget_destroy (window);
}

/* 获得选择备份的数据卷 */
gint data_volume_backup_get_data(GtkBuilder *builder, gchar *str, gchar *data_name)
{
	GtkTreeView *view = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkTreeModel *model = gtk_tree_view_get_model (view);
	GtkTreeIter iter;

	gboolean choose_flag = FALSE;  /* 用户是否选择至少一个数据卷,没选择就做出选择提示 */
	gboolean flag = FALSE;         /* 用户是否选择该行数据 */
	gchar *data1_tmp = NULL;       /* 该行数据的创建数据卷信息 */
	gchar *data2_tmp = NULL;	   /* 该行数据的挂载数据卷信息 */
	gchar *data3_tmp = NULL;       /* 该行数据的本地目录信息 */
	gchar *container_name      = NULL; /* 该行数据的容器名称 */
	gchar data1[256] = {0};			/* 创建数据卷信息的集合 */
	gchar data2[256] = {0};			/* 挂载数据卷信息的集合 */
	gchar data3[256] = {0};			/* 本地目录的集合 */
	gchar data_name_1[256] = {0};   /* 提取创建数据卷信息的集合中的数据卷名称 */
	gchar data_name_2[256] = {0};   /* 提取挂载数据卷信息的集合中的数据卷名称 */
	gchar data_name_3[256] = {0};   /* 提取本地目录的集合中的数据卷名称 */

	memset(data1, '\0', sizeof(data1));
	memset(data2, '\0', sizeof(data2));
	memset(data3, '\0', sizeof(data3));
	memset(data_name_1, '\0', sizeof(data_name_1));
	memset(data_name_2, '\0', sizeof(data_name_2));
	memset(data_name_3, '\0', sizeof(data_name_3));
	
	data1_tmp = (gchar *)g_malloc(sizeof(gchar)*256);
	data2_tmp = (gchar *)g_malloc(sizeof(gchar)*256);
	data3_tmp = (gchar *)g_malloc(sizeof(gchar)*256);
	container_name = (gchar *)g_malloc(sizeof(gchar)*256);
	
	if(data1_tmp == NULL || data2_tmp == NULL || data3_tmp == NULL)
	{
		printf("malloc error  %s  %d\n", __FILE__, __LINE__);
		return 0;
	}

	sprintf(str,"sudo docker run --rm");

	if(gtk_tree_model_get_iter_first(model,&iter))//迭代器定位到第一行
	{
		do   /* 循环提取想要的信息 */
		{
			gtk_tree_model_get(model, &iter, 6, &flag, 1, & container_name, 2, &data1_tmp, 3, &data2_tmp, 4, &data3_tmp, -1);
			
			if(flag)
			{
				choose_flag = TRUE;
				sprintf(data1,"%s %s", data1, data1_tmp);
				sprintf(data2,"%s %s", data2, data2_tmp);
				sprintf(data3,"%s %s", data3, data3_tmp);
				sprintf(str, "%s --volumes-from %s",str, container_name);
				memset(container_name, '\0', strlen(container_name));
				memset(data1_tmp, '\0', strlen(data1_tmp));
				memset(data2_tmp, '\0', strlen(data2_tmp));
				memset(data3_tmp, '\0', strlen(data3_tmp));
			}
		}while(gtk_tree_model_iter_next (model, &iter));

		/* 如果FLAG为假，则是点击了确定按钮，要执行该判断条件，给出提示
			如果FLAG为真，则是在TreeView选择了数据卷，不给提示 */
		if(!SELECT_ALL_BACKUP_FLAG)
		{	
			if(!choose_flag)  /* 用户没做出选择 */
			{
				message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("you have chosen none, please choose first!"));
				g_free(data1_tmp);
				g_free(data2_tmp);
				g_free(data3_tmp);
				g_free(container_name);
				return NO_CHOICE;
			}
		}
	}
	else  /* 没有任何可以选择的数据卷 */
	{
		if(!SELECT_ALL_BACKUP_FLAG)
		{
			message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("have none of datavolume, please create it first！"));
			g_free(data1_tmp);
			g_free(data2_tmp);
			g_free(data3_tmp);
			g_free(container_name);
			return NO_DATA_FLAG;
		}
	}

	data_volume_backup_pick_up_data(data_name_1, data1, DATA1_FLAG);
	data_volume_backup_pick_up_data(data_name_2, data2, DATA2_FLAG);
	data_volume_backup_pick_up_data(data_name_3, data3, DATA3_FLAG);
	sprintf(data_name,"%s %s %s %s",data_name,data_name_1,data_name_2,data_name_3);

	g_free(container_name);
	g_free(data1_tmp);
	g_free(data2_tmp);
	g_free(data3_tmp);
	return RETURN_OK;
}

/* 提取创建数据卷信息的集合中的数据卷名称 */
void data_volume_backup_pick_up_data(gchar *str, gchar *data, gint flag)
{
	gchar data_tmp[256] = {0};
	gchar command[1024]   = {0};
	
	memset(command, '\0', sizeof(data_tmp));
	memset(data_tmp, '\0', sizeof(data_tmp));
	strcpy(data_tmp, data);

	if(flag == DATA1_FLAG)
	{
		sprintf(command, "echo %s | awk -F : '{for(i=1;i<NF;i++) printf $i\" \"}'"
	  						     "| awk -F , '{for(i=1;i<=NF;i++) printf $i}'"
	    						 "| awk -F { '{for(i=1;i<=NF;i++) printf $i}'"
	    						 "| awk -F } '{for(i=1;i<=NF;i++) printf $i}'"
	    						 "| awk '{for(i=1;i<=NF;i++) if($i==\"null\"){continue}"
		    					   "else{printf $i\" \"}}'",data_tmp);
	}

	else if(flag == DATA2_FLAG)
	{
		sprintf(command, "echo %s",data_tmp);
	}
	
	else if(flag == DATA3_FLAG)
	{
		sprintf(command, "echo %s | awk -F : '{for(i=1;i<NF;i++) printf $i\" \"}'| awk '{for(i=1;i<=NF;i+2){printf $i\" \"}}'", data_tmp);
	}

	else
	{
		printf("flag error!  %s  %d",__FILE__,__LINE__);
		return;
	}
	memset(data_tmp, '\0', sizeof(data_tmp));
	strcpy(data_tmp,popen_and_fread (command));
	sprintf(str,"%s %s", str, data_tmp);
}

/* 提取镜像名称 */
gint data_volume_backup_get_image(GtkBuilder *builder, gchar *str)
{
	GtkComboBoxText *combo_box = NULL;
	const gchar *combo_box_text = NULL;  
	combo_box= GTK_COMBO_BOX_TEXT(gtk_builder_get_object (builder,"comboboxtext1"));
	combo_box_text  = gtk_combo_box_text_get_active_text(combo_box);
	
	if(!(combo_box_text && *combo_box_text))
	{
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("please choose image!!!"));
		return NO_IMAGES;
	}

	sprintf(str,"%s %s",str,combo_box_text);
	
	return RETURN_OK;
}

/* 提取tar包名称 */
gint data_volume_backup_get_name(GtkBuilder *builder, gchar *str)
{
	GtkEntry *entry = NULL;
	const gchar *entry_text = NULL;
	const gchar *pData1 = NULL;  
	const gchar *pData2 = NULL;
	
	entry = GTK_ENTRY(gtk_builder_get_object (builder, "entry1"));
	entry_text = gtk_entry_get_text (entry);

	if(!(entry_text && *entry_text))
	{
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("please input the name of tar!!!"));
		return NO_NAMES;
	}

	pData1 = entry_text;
	pData2 = entry_text;
	
	if(isalnum(*pData1) == 0)
	{
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("the acronym of name could only be number or letter"));
		return NO_NAMES;
	}
	
	while(*pData2 != '\0')
	{
		if( (isalnum(*pData2) != 0) || ((*pData2) == '_') || ((*pData2) == '-') )
			pData2++;

		else
		{
			message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("the name could only contains number,letter or underline"));	
			return NO_NAMES;
		}
	}
	
	if(RETURN_OK != (data_volume_backup_check_name (entry_text)))
		return NO_NAMES;
		
	sprintf(str,"%s tar cvf %s/%s.tar", str, BACKUP_TAR_DOCKER_PATH,entry_text);
	return RETURN_OK;
}

/* 检测tar包名称是否已存在 */
gint data_volume_backup_check_name(const gchar *str)
{
	gchar command[256] = {0};
	gchar result[256]  = {0};
	gchar str_tmp[256] = {0};
	gint num = 0;
	gint ix  = 0;

	memset(str_tmp, '\0', sizeof(str_tmp));
	sprintf(str_tmp,"%s.tar", str);
	
	memset(command, '\0', sizeof(command));
	sprintf(command, "find %s/*.tar | awk 'END{print NR}'",BACKUP_TAR_LOCAL_PATH);

	num = atoi(popen_and_fread (command));

	for(ix=1; ix<=num; ix++)
	{
		memset(command, '\0', sizeof(command));
		memset(result, '\0', sizeof(result));
		sprintf(command, "find %s/*.tar | sed -n \"%d,1p\" | awk -F / '{print $NF}'",BACKUP_TAR_LOCAL_PATH, ix);
		strcpy(result,popen_and_fread (command));

		if(strcmp(str_tmp,result) == 0)
		{
			message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("the name exists, please change a name"));
			return -1;
		}
	}
	return RETURN_OK;
}

/* 将已经选择的数据卷显示 */
void data_volume_backup_set_label(GtkBuilder *builder, gchar *data_name)
{
	GtkLabel *label = NULL;

	label = GTK_LABEL(gtk_builder_get_object (builder,"label6"));
	gtk_label_set_justify (label, GTK_JUSTIFY_LEFT);
	gtk_label_set_text (label, data_name);
}

/* 将生成tar包的信息写入到文件 */
void data_volume_backup_write_info_to_txt(GtkBuilder *builder, gchar *str)
{
	FILE *fp = NULL;
	GtkEntry *entry = NULL;
	const gchar *entry_text = NULL;
	gchar file_name[256] = {0};
	gchar file_name_tmp[256] = {0};
	gchar msg[256] = {0};
	
	entry = GTK_ENTRY(gtk_builder_get_object (builder, "entry1"));
	entry_text = gtk_entry_get_text (entry);

	memset(file_name, '\0', sizeof(file_name));
	memset(file_name_tmp, '\0', sizeof(file_name_tmp));
	memset(msg, '\0', sizeof(msg));
	sprintf(msg, "%s : %s\n", entry_text, str);
	
	strcpy(file_name_tmp, popen_and_fread ("pwd"));
	sprintf(file_name,"%s/src/volume_tar/info.txt", file_name_tmp);
	
	if(NULL == (fp = fopen(file_name, "a")))
	{
		printf("fopen failed  %s  %d\n", __func__, __LINE__);
		return;
	}

	fwrite(msg, 1, strlen(msg), fp);
	fclose(fp);
}


void data_volume_recovery(GtkWidget *widget, GtkWidget *window)
{
	GtkBuilder *builder = gtk_builder_new();
	GError* error = NULL;
	FILE *fp;
	
	if (!gtk_builder_add_from_file (builder, BACKUP_VOLUME_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	GtkWidget *recovery_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	GtkWidget *button_ok     = GTK_WIDGET(gtk_builder_get_object (builder,"button_ok"));
	GtkWidget *button_cancel = GTK_WIDGET(gtk_builder_get_object (builder,"button_cancel"));
	GtkWidget *check_button  = GTK_WIDGET(gtk_builder_get_object (builder,"checkbutton1"));
	GtkWidget *label1        = GTK_WIDGET(gtk_builder_get_object (builder, "label1"));
	GtkWidget *label2        = GTK_WIDGET(gtk_builder_get_object (builder, "label2"));
	GtkWidget *label4        = GTK_WIDGET(gtk_builder_get_object (builder, "label4"));
	GtkTreeView     *view     = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkWidget *entry         = GTK_WIDGET(gtk_builder_get_object (builder, "entry2")); 
	GtkWidget *button         = GTK_WIDGET(gtk_builder_get_object (builder, "button")); 
	GtkWidget *label         = GTK_WIDGET(gtk_builder_get_object (builder, "label5"));
	GtkCellRenderer *renderer = gtk_cell_renderer_toggle_new ();
	
	gtk_window_set_default_size (GTK_WINDOW(recovery_window),700,500);
	gtk_window_set_position (GTK_WINDOW(recovery_window),GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(recovery_window),_("recovery datavolume"));

	gtk_label_set_text(GTK_LABEL(label1), _("select one or more datavolume to recovery"));
	gtk_label_set_text(GTK_LABEL(label2), _("the recovery datavolume you have chosen"));
	gtk_label_set_text(GTK_LABEL(label4), _("input the name of new container"));
	gtk_button_set_label (GTK_BUTTON(button), _("look up the datavolume you have recoveried"));
	
	gboolean flg = FALSE;
	data_volume_recovery_add_text_column(view);
	data_volume_recovery_add_toggle_column(view, renderer);
	data_volume_recovery_add_data(builder, flg);
	combo_box_add_image(builder);
	
	g_signal_connect(G_OBJECT(check_button),"clicked",G_CALLBACK(data_volume_recovery_select_all),builder);
	g_signal_connect(G_OBJECT(button_ok), "clicked", G_CALLBACK(data_volume_recovery_ok), builder);
	g_signal_connect_swapped(G_OBJECT(button_cancel), "clicked", G_CALLBACK(gtk_widget_destroy), recovery_window);
	g_signal_connect (renderer, "toggled", G_CALLBACK (data_volume_recovery_renderer_get_label), builder);
	
	gtk_widget_show_all (recovery_window);
	gtk_widget_hide (label);
	gtk_widget_hide(entry);

	if(NULL == (fp = fopen("src/volume_tar/info.txt", "r")))
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("have none datavolume to recovery, please backup first"));
}

void data_volume_recovery_add_text_column(GtkTreeView *view)
{
	tree_column_new (view, 0, _("Num"));
	tree_column_new (view, 1, _("The name of tar"));
	tree_column_new (view, 2, _("DataVolume"));
}

void data_volume_recovery_add_toggle_column(GtkTreeView *view, GtkCellRenderer *renderer)
{
	g_object_set (renderer, "xalign", 0.0, NULL);
	g_object_set_data (G_OBJECT (renderer), "column", (gint *)6);
	
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                          -1, _("choice"),renderer,"active",6,NULL);
}

void data_volume_recovery_add_data(GtkBuilder *builder, gboolean flag)
{
	GtkTreeStore *list = GTK_TREE_STORE(gtk_builder_get_object (builder,"treestore1"));
	GtkTreeIter iters;
	
	gchar command[512] = {0};
	gchar result[512]  = {0};
	gchar str[512]     = {0};
	gchar tar_str[128] = {0};
	gchar data_str[128]= {0};
	gint  lines        = 0;
	gint  ix;

	memset(command, '\0', sizeof(command));
	memset(result , '\0', sizeof(result));

	strcpy(command, popen_and_fread ("pwd"));
	sprintf(str,"cat %s/src/volume_tar/info.txt | awk 'END{print NR}'", command);
	sprintf(command, "%s/src/volume_tar/info.txt",command);
	strcpy(result, popen_and_fread (str));
	lines  = atoi(result);

	for(ix=0; ix<lines; ix++)
	{
		memset(str, '\0', strlen(str));
		memset(result, '\0', strlen(result));
		memset(data_str, '\0', strlen(data_str));
		memset(tar_str, '\0', strlen(tar_str));
		
		sprintf(str, "cat %s | sed -n \"%d,1p\"", command, ix+1);
		strcpy(result, popen_and_fread (str));
		sscanf(result,"%[0-9a-z] : %[/0-9a-z ]",tar_str,data_str);

		gtk_tree_store_append (list, &iters,NULL);
		gtk_tree_store_set (list, &iters,
    		    		  		0, ix+1,
       		        			1, tar_str,
		                		2, data_str,
		                		6, flag,
        		      			-1);
	}
}

void data_volume_recovery_renderer_get_label(GtkCellRendererToggle *cell,gchar *path_str, GtkBuilder *builder)
{
	GtkTreeView     *view     = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkTreeModel    *model    = gtk_tree_view_get_model (view);
	item_toggled(cell, path_str, model);

	data_volume_recovery_get_data(builder);
}

void data_volume_recovery_get_data(GtkBuilder *builder)
{
	GtkLabel *label = GTK_LABEL(gtk_builder_get_object (builder, "label6"));
	GtkTreeView *view   = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkTreeModel *model = gtk_tree_view_get_model (view);
	GtkTreeIter iter;
	gchar data_name[512] = {0};
	gboolean flg = FALSE;
	gchar *str1 = NULL;

	memset(data_name, '\0', sizeof(data_name));
	
	str1 = (gchar *)g_malloc(sizeof(gchar)*128);
	if(NULL == str1)
		return ;
	
	if(gtk_tree_model_get_iter_first(model,&iter))
	{		
		do
		{
			gtk_tree_model_get(model, &iter, 6, &flg, 2, &str1, -1);
			if(flg)
			{
				sprintf(data_name,"%s %s", data_name, str1);
				memset(str1, '\0', strlen(str1));
			}
		}while(gtk_tree_model_iter_next (model, &iter));
	}
	
	g_free(str1);
	gtk_label_set_text(label, data_name);
}

void data_volume_recovery_select_all(GtkButton *button, GtkBuilder *builder)
{
	GtkTreeStore *list = GTK_TREE_STORE(gtk_builder_get_object (builder,"treestore1"));
	gtk_tree_store_clear (GTK_TREE_STORE(list));
	
	if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(button)))
	{
		gtk_button_set_label (button,_("a key to cancel all"));
		data_volume_recovery_add_data(builder, TRUE);
		data_volume_recovery_get_data(builder);
	}

	else
	{
		gtk_button_set_label (button,_("a key to select all"));
		data_volume_recovery_add_data(builder, FALSE);
		data_volume_recovery_get_data(builder);
	}
}

void data_volume_recovery_ok(GtkWidget *widget, GtkBuilder *builder)
{
	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
	
	gchar str[1024]  = {0};  /* 创建新容器 */
	gchar str1[1024] = {0};  /* 给新容器恢复数据卷 */
	gchar image_name[128] = {0};
	gchar entry_name[128] = {0};
	gchar tar_name[512] = {0};
	
	memset(str, '\0', sizeof(str));
	memset(str1, '\0', sizeof(str1));
	memset(image_name, '\0', sizeof(image_name));
	memset(entry_name, '\0', sizeof(entry_name));
	memset(tar_name, '\0', sizeof(tar_name));
	
	sprintf(str, "sudo docker run -idt");
	if(RETURN_OK != data_volume_recovery_get_label(builder, str, RECOVERY_FLAG))
		return;

	if(RETURN_OK != data_volume_backup_get_image (builder, image_name))
		return;

	if(RETURN_OK != data_volume_recovery_get_container_name(builder, entry_name))
		return;

	sprintf(str,"%s --name %s %s", str, entry_name, image_name);
	if(!data_volume_recovery_create_container(str))
		return;
	
	sprintf(str1, "sudo docker run --rm --volumes-from %s -v %s:%s %s tar xvf %s/",
	    									entry_name, BACKUP_TAR_LOCAL_PATH, 
	    									BACKUP_TAR_DOCKER_PATH, image_name,
	    									BACKUP_TAR_DOCKER_PATH);

	data_volume_recovery_get_tar_name(builder, tar_name);

	if(data_volume_recovery_data(str1, tar_name))
	{
		message_dialog_new (NULL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, _("datavolume recovery successfully ^_^"));
		gtk_widget_destroy (window);
	}
	else
	{
		memset(str, '\0', sizeof(str));
		sprintf(str, "sudo docker rm -v -f %s", entry_name);
		popen_and_fread (str);
	}
}

gint data_volume_recovery_get_label(GtkBuilder *builder, gchar *str, gint flag)
{
	GtkLabel *label = GTK_LABEL(gtk_builder_get_object (builder, "label6"));
	const gchar *label_text = NULL;

	label_text = gtk_label_get_text(label);
	
	if(!(label_text && *label_text))
	{
		if(flag == RECOVERY_FLAG)
			message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("please choose the recovery datavolume first!"));
		else if(flag == TRANSFER_FLAG)
			message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("please choose the transfer datavolume first!"));
		
		return NO_CHOICE;
	}

	strcpy(str, data_volume_create_get_data_name (str, label_text));

	return RETURN_OK; 
}

gint data_volume_recovery_get_container_name(GtkBuilder *builder, gchar *str)
{
	GtkEntry *entry = NULL;
	const gchar *entry_text = NULL;
	gchar *pData = NULL;
	gchar tmp[512] = {0};

	memset(tmp, '\0', sizeof(tmp));
	
	entry = GTK_ENTRY(gtk_builder_get_object (builder, "entry1"));
	entry_text = gtk_entry_get_text (entry);

	if(!(entry_text && *entry_text))
	{
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("please input the name of datavolume container first!!!"));
		return NO_NAMES;
	}

	strcpy(tmp,entry_text);
	pData = strtok(tmp, " ");
	pData = strtok(NULL, " ");
	if(pData)
	{
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("you could input at most one name"));
		return NO_NAMES;
	}
		
	strcpy(str, entry_text);
	return RETURN_OK;
}

void data_volume_recovery_get_tar_name(GtkBuilder *builder, gchar  *tar_name)
{
	GtkTreeView *view   = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkTreeModel *model = gtk_tree_view_get_model (view);
	GtkTreeIter iter;
	gboolean flg = FALSE;
	gchar *tar_name_tmp = NULL;

	tar_name_tmp = (gchar *)g_malloc(sizeof(gchar)*128);
	if(NULL == tar_name_tmp)
		return ;

	if(gtk_tree_model_get_iter_first(model,&iter))
	{		
		do
		{
			gtk_tree_model_get(model, &iter, 6, &flg, 1, &tar_name_tmp, -1);
			if(flg)
			{
				sprintf(tar_name,"%s %s",tar_name, tar_name_tmp);
				memset(tar_name_tmp, '\0', strlen(tar_name_tmp));
			}
		}while(gtk_tree_model_iter_next (model, &iter));
	}
	
	g_free(tar_name_tmp);
}

gboolean 	data_volume_recovery_data(gchar *str, gchar *tar_name)
{
	gchar *pData   = NULL;
	gchar tar[512] = {0};
	gchar str_tmp[1024] = {0};
	
	memset(str_tmp, '\0', sizeof(str_tmp));
	memset(tar, '\0', sizeof(tar));
	strcpy(str_tmp, str);
	strcpy(tar, tar_name);
	
	pData = strtok(tar," ");

	if(pData)
	{
		sprintf(str_tmp,"%s%s.tar",str,pData);
		printf("str:  %s\n",str_tmp);
		
		if(!data_volume_recovery_create_container(str_tmp))
			return FALSE;
	}
		
	pData = strtok(NULL, " ");
	while(pData)
	{
		memset(str_tmp, '\0', sizeof(str_tmp));	
		strcpy(str_tmp, str);
		
		sprintf(str_tmp,"%s%s.tar",str_tmp,pData);
		pData = strtok(NULL, " ");

		printf("str:  %s\n",str_tmp);
		if(!data_volume_recovery_create_container(str_tmp))
			return FALSE;
	}

	return TRUE;
}

gboolean data_volume_recovery_create_container(gchar *str)
{	
	gchar result[1024] = {0};
	gchar *pData = NULL;

	memset(result, '\0', sizeof(result));
	sprintf(str,"%s 2>&1; echo $?",str);
	strcpy(result,popen_and_fread (str));
	
	pData = &result[strlen(result)-1];
	
	if(*pData != '0')
	{
		result[strlen(result) - 1] = '\0';
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, result);
		return FALSE;
	}
	
	return TRUE;
}

void data_volume_transfer(GtkWidget *widget, GtkWidget *window)
{
	GtkBuilder *builder = gtk_builder_new();
	GError* error = NULL;
	FILE *fp = NULL;
	
	if (!gtk_builder_add_from_file (builder, BACKUP_VOLUME_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	GtkWidget *transfer_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	GtkWidget *button_ok     = GTK_WIDGET(gtk_builder_get_object (builder,"button_ok"));
	GtkWidget *button_cancel = GTK_WIDGET(gtk_builder_get_object (builder,"button_cancel"));
	GtkWidget *check_button  = GTK_WIDGET(gtk_builder_get_object (builder,"checkbutton1"));
	GtkWidget *label1        = GTK_WIDGET(gtk_builder_get_object (builder, "label1"));
	GtkWidget *label2        = GTK_WIDGET(gtk_builder_get_object (builder, "label2"));
	GtkWidget *label4        = GTK_WIDGET(gtk_builder_get_object (builder, "label4"));
	GtkWidget *button         = GTK_WIDGET(gtk_builder_get_object (builder, "button")); 
	GtkTreeView     *view     = GTK_TREE_VIEW(gtk_builder_get_object (builder,"treeview10"));
	GtkCellRenderer *renderer = gtk_cell_renderer_toggle_new ();
	
	gtk_window_set_default_size (GTK_WINDOW(transfer_window),700,500);
	gtk_window_set_position (GTK_WINDOW(transfer_window),GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW(transfer_window),_("transfer datavolume"));

	gtk_label_set_text(GTK_LABEL(label1), _("select one or more datavolume"));
	gtk_label_set_text(GTK_LABEL(label2), _("the datavolume you have chosen"));
	gtk_label_set_text(GTK_LABEL(label4), _("input the name of new container"));
	gtk_button_set_label (GTK_BUTTON(button), _("look up the datavolume that have transfered"));
	
	gboolean flg = FALSE;
	data_volume_recovery_add_text_column(view);
	data_volume_recovery_add_toggle_column(view, renderer);
	data_volume_recovery_add_data(builder, flg);
	combo_box_add_image(builder);

	g_signal_connect(G_OBJECT(check_button),"clicked",G_CALLBACK(data_volume_recovery_select_all),builder);
	g_signal_connect(G_OBJECT(button_ok), "clicked", G_CALLBACK(data_volume_transfer_ok), builder);
	g_signal_connect_swapped(G_OBJECT(button_cancel), "clicked", G_CALLBACK(gtk_widget_destroy), transfer_window);
	g_signal_connect (renderer, "toggled", G_CALLBACK (data_volume_recovery_renderer_get_label), builder);
	
	gtk_widget_show_all (transfer_window);
	if(NULL == (fp = fopen("src/volume_tar/info.txt", "r")))
		message_dialog_new (NULL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, _("have none datavolume to transfer, please backup first"));	
}

void data_volume_transfer_ok(GtkWidget *widget, GtkBuilder *builder)
{
	gchar str[1024]  = {0};  /* 创建新容器,迁移数据卷 */
	gchar str1[1024] = {0}; 
	gchar image_name[128] = {0};
	gchar entry_name[128] = {0};
	gchar dir_name[512] = {0};
	gchar tar_name[512] = {0};
	
	memset(str, '\0', sizeof(str));
	memset(str1, '\0', sizeof(str1));
	memset(image_name, '\0', sizeof(image_name));
	memset(entry_name, '\0', sizeof(entry_name));
	memset(tar_name, '\0', sizeof(tar_name));
	memset(dir_name, '\0', sizeof(dir_name));

	/* 判断是否选择了数据卷 */
	if(RETURN_OK != data_volume_recovery_get_label(builder, str, TRANSFER_FLAG))
		return;

	/* 判断是否选择了镜像 */
	if(RETURN_OK != data_volume_backup_get_image (builder, image_name))
		return;

	/* 判断是否输入新目录的名称 */
	if(RETURN_OK != data_volume_transfer_get_dir_name(builder, dir_name))
		return;

	/* 判断是否填入了新建容器名称 */
	if(RETURN_OK != data_volume_recovery_get_container_name(builder, entry_name))
		return;

	data_volume_recovery_get_tar_name(builder, tar_name);
	
	/*  根据目录名称新建目录 */
	if(!data_volume_transfer_create_directory(dir_name, tar_name))
		return;
	
	sprintf(str, "sudo docker run -idt --name %s -v %s/%s:/transfer %s ",
	    						entry_name, BACKUP_TAR_LOCAL_PATH, dir_name, image_name);

	/* 迁移数据卷，如果失败，则删除之前创建的目录 */
	if(!create_event (builder, str))
	{
		sprintf(str1, "sudo rm -rf $(pwd)/src/volume_tar/%s", tar_name);
		data_volume_recovery_create_container(str1);
	}
}


gint data_volume_transfer_get_dir_name(GtkBuilder *builder, gchar *str)
{
	GtkEntry *entry = NULL;
	const gchar *entry_text = NULL;
	
	entry = GTK_ENTRY(gtk_builder_get_object (builder, "entry2"));
	entry_text = gtk_entry_get_text (entry);

	if(!(entry_text && *entry_text))
	{
		message_dialog_new (NULL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("please input the name of new directory!!!"));
		return NO_CONTAINER;
	}

	strcpy(str, entry_text);
	return RETURN_OK;
}

gboolean data_volume_transfer_create_directory(gchar *dir_name, gchar *tar_name)
{
	gchar tmp[512] = {0};
	gchar str[512] = {0};  /*  获得多个tar包 */
	gchar str1[256] = {0};
	gchar *pData   = NULL;
	
	memset(str1, '\0', sizeof(str1));
	memset(str, '\0', sizeof(str));
	memset(tmp, '\0', sizeof(tmp));
	
	strcpy(tmp, tar_name);

	pData = strtok(tmp, " ");
	sprintf(str, "%s.tar", pData);
	
	pData = strtok(NULL, " ");

	while(pData)
	{
		sprintf(str, "%s %s.tar",str, pData);
		pData = strtok(NULL, " ");
	}

	/* 创建目录 */
	sprintf(str1,"cd $(pwd)/src/volume_tar && sudo mkdir %s", dir_name); 
	if(data_volume_recovery_create_container(str1))
	{
		memset(str1, '\0', sizeof(str1));
		
		/* 讲选择的相关tar包进行解压 */
		sprintf(str1,"cd $(pwd)/src/volume_tar && sudo cp %s %s"
	    			     "&& cd %s && sudo tar xvf *.tar && sudo rm -rf *.tar",
	    			     	str, dir_name, dir_name);
		
		if(data_volume_recovery_create_container(str1))
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}


void data_volume_look_all(GtkWidget *widget, GtkWidget *window)
{
	GtkBuilder *builder = gtk_builder_new();
	GError* error = NULL;
	if (!gtk_builder_add_from_file (builder, LOOK_VOLUME_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	GtkWidget *look_all_window = GTK_WIDGET(gtk_builder_get_object (builder,"window"));
	GtkWidget *del_button    = GTK_WIDGET(gtk_builder_get_object (builder,"del_button"));
	GtkWidget *open_button  = GTK_WIDGET(gtk_builder_get_object (builder,"open_button"));
	GtkWidget *check_button = GTK_WIDGET(gtk_builder_get_object (builder,"checkbutton1"));
	GtkWidget *return_button = GTK_WIDGET(gtk_builder_get_object (builder,"return_button"));

	gtk_window_set_title (GTK_WINDOW(look_all_window),_("look up all datavolumes"));
	gtk_window_set_default_size (GTK_WINDOW(look_all_window),700,500);
	gtk_window_set_position (GTK_WINDOW(look_all_window),GTK_WIN_POS_CENTER_ALWAYS);
	
	volume_flag = BACKUP_FLAG;
	data_volume_look_add_column(builder);
	data_volume_look_add_data(builder,volume_flag);
	
	g_signal_connect(G_OBJECT(check_button),"clicked",G_CALLBACK(select_all),builder);
	g_signal_connect(G_OBJECT(del_button),"clicked",G_CALLBACK(data_volume_look_del),builder);
	g_signal_connect(G_OBJECT(open_button),"clicked",G_CALLBACK(data_volume_look_open),builder);
	g_signal_connect_swapped(G_OBJECT(return_button),"clicked",G_CALLBACK(gtk_widget_destroy), look_all_window);
	gtk_widget_show_all(look_all_window);
}




