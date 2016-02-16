#include "global/global.h"
#include "nfs_dockerfile_manage.h"

GtkWidget *dialog11;
GtkWidget *text_view;
GtkWidget *text_view_save;
GtkTextIter start_iter;
GtkTextIter end_iter;
GtkWidget *window_space;
GtkTextTag *tag,*tag1,*tag2;
GtkWidget *button_quit;//查找退出
GtkWidget *entry_find;//输入查找字符
GtkWidget *button_next;//查找下一个
GtkWidget *status11;//显示行数与列数的状态栏

static char buf_build[1024]={0};
//char flags;
//static int i999=0;
static FILE *fd333=NULL;
static int file_chang=1;
//GCond *cond=NULL;
GCond cond;
GtkTextBuffer *textbuffer;
//GMutex *mutex=NULL;
GMutex mutex;
GtkTextIter iter;

void 		menuitem55_quit();

gpointer textbuffer_insert_fun(gpointer data)
{
		usleep(1000);
		//gdk_threads_enter ();
			if(fgets(buf_build,1024,fd333) ==NULL){
				printf("sssss");
			}
while(fgets(buf_build,1024,fd333) !=NULL){
		if(fgets(buf_build,1024,fd333))
		printf("%s\n",buf_build);
		sleep(1);
		gtk_text_buffer_insert (GTK_TEXT_BUFFER(textbuffer),&iter,buf_build,strlen(buf_build));
		}
	g_free(data);
	pclose(fd333);
	//g_cond_wait (&cond,&mutex);
	//g_cond_signal (&cond);
	//gdk_threads_leave ();
	return (void *)1;
}
void create_dockerbuild_textview(gchar *data)
{
	
		GtkWidget *window;
        GtkBuilder *builder;
		fd333=popen(data,"r");
		
		
        builder=gtk_builder_new();
        gtk_builder_add_from_file(builder,DOCKER_BUILDE_TEXTVIEW,NULL);
        window=GTK_WIDGET(gtk_builder_get_object(builder,DOCKEERBUILD_TEXTVIEW_WINDOW));
        text_view=GTK_WIDGET(gtk_builder_get_object(builder,"textview1"));

		


		
		textbuffer=gtk_text_view_get_buffer (GTK_TEXT_VIEW(text_view));
		gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER(textbuffer),&start_iter);
		gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER(textbuffer),&end_iter);
		gtk_text_buffer_delete (GTK_TEXT_BUFFER(textbuffer),&start_iter,&end_iter);
		gtk_text_buffer_get_iter_at_line_index (textbuffer,&iter,0,0);
		
		//g_thread_new ("popen",popen_func,NULL);
		
		g_thread_new ("texbuffer_insert",textbuffer_insert_fun,data);
		/*
		
		if(fgets(buf_build,1000,fd333) == NULL){
			//g_printf("555555\n");
		}
		while(fgets(buf_build,1024,fd333) !=NULL){
		//printf("%s\n",buf_build);
		gtk_text_buffer_insert (GTK_TEXT_BUFFER(textbuffer),&iter,buf_build,strlen(buf_build));
		}
		//printf("%s\n",data);
		//printf("%s\n",buff_dockerbuild);
		//memset(buf_build,'\0',sizeof(buf_build));
	//pclose(fd333);
*/
	gtk_widget_show_all (window);
	//g_cond_signal (&cond);
	//g_cond_wait (&cond,&mutex);
	//fclose (fd333);

}
void file_chooser_button22(GtkWidget *window,gpointer data)//选择Dockerfile文件确定按钮的信号
{

	const char  *buf_filechooser=NULL;
	static char *filename_chooser=NULL;
	static char *filename_chosser_file=NULL;
	static char *tmp=NULL;
	static gint num=0;
	static gint result_file=0;
	gchar *tmp11=NULL;
	GtkWidget *message_file;
	//message_file=gtk_message_dialog_new(dialog11,GTK_DIALOG_DESTROY_WITH_PARENT,
	   //                                 GTK_MESSAGE_INFO,GTK_BUTTONS_OK_CANCEL,"");
	buf_filechooser=gtk_entry_get_text (GTK_ENTRY(data));
	filename_chooser=gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER(dialog11));
	filename_chosser_file=gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog11));
	printf("1111%s\n",filename_chosser_file);
	tmp=(char *)malloc(sizeof(filename_chooser)*50);
	tmp11=(gchar *)malloc(sizeof(filename_chooser)*50);
	//printf("%s\n",buf_filechooser);
	//printf("%s\n",filename_chooser);
	for(num=0;num<strlen(buf_filechooser);num++){
	//if(ispunct(bu[num]) || isspace(bu[num])){
		if(isspace(buf_filechooser[num])){
	contains_message(GTK_MESSAGE_ERROR,_("The image name cannot contain spaces characters"));
		return;
	}
	}
	if(buf_filechooser[0] ==0){
		contains_message(GTK_MESSAGE_ERROR,_("The image name cannot is spaces"));
		return;
		//printf("ssss\n");

	}
	sprintf(tmp,"sudo docker build -t %s %s",buf_filechooser,filename_chooser);
	sprintf(tmp11,_("please sure Dockerfile path:%s"),filename_chosser_file);
	message_file=gtk_message_dialog_new(GTK_WINDOW(dialog11),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK_CANCEL,
	                                    tmp11,NULL);
		result_file=gtk_dialog_run (GTK_DIALOG(message_file));
		if(result_file==GTK_RESPONSE_OK){
				create_dockerbuild_textview(tmp);
				g_free(tmp11);
				gtk_widget_destroy (message_file);
				gtk_widget_destroy (dialog11);
		}
		else{
			g_free(tmp11);
			g_free(tmp);
			gtk_widget_destroy (message_file);

		}
	
	//printf("%s\n",tmp);

	//g_free (filename_chooser);

}
void file_chooser_button11(GtkWidget *window,gpointer data)//选择Dockerfile文件取消按钮的信号
{
	gtk_widget_destroy (data);
}
void select_dockerfile_textview(GtkWidget *window,gpointer data)//选择Dockerfile文件
{
	
	GtkBuilder *builder;
	GtkWidget *button11,*button22;
	GtkWidget *entry11;
    
	builder=gtk_builder_new ();
	gtk_builder_add_from_file (builder,FILE_CHOOSER,NULL);
	dialog11=GTK_WIDGET(gtk_builder_get_object (builder,FILE_CHOOSER_WINDOW));
	entry11=GTK_WIDGET(gtk_builder_get_object (builder,"entry1"));
	button11=GTK_WIDGET(gtk_builder_get_object (builder,"button1"));
	button22=GTK_WIDGET(gtk_builder_get_object (builder,"button2"));
	gtk_window_set_default_size(GTK_WINDOW(dialog11), 600, 400);
    
	g_signal_connect(G_OBJECT(button11),"clicked",G_CALLBACK(file_chooser_button11),dialog11);
	g_signal_connect(G_OBJECT(button22),"clicked",G_CALLBACK(file_chooser_button22),entry11);


	gtk_widget_show_all (dialog11);
	
}


/*  新建Dockerfile构建镜像的功能down */

gint  real_save(char *data,char *buf)
{
       static FILE *fp=NULL;
        fp=fopen(data,"w");
        if(fp==NULL)
        {
        printf("fopen is fail\n");
				return 3;
}
        //printf("%s\n",buf);
        fwrite(buf,sizeof(char),strlen(buf),fp);
        //fputs(buf,fp);
		gtk_window_set_title (GTK_WINDOW(window_space),data);
        fclose(fp);
	return 0;

}
void open_filechange_flag(GtkTextBuffer *window,gpointer data)//打开文件发生变化的改变状态
{
		file_chang=0;

		gtk_text_buffer_get_start_iter (window,&start_iter);
		gtk_text_buffer_get_end_iter (window,&end_iter);
		gtk_text_buffer_apply_tag(window,tag,&start_iter,&end_iter);
		gtk_text_buffer_apply_tag(window,tag1,&start_iter,&end_iter);
}
void menuitem22_open(GtkWidget *window,gpointer data_open)//打开文件
{
	GtkWidget *dialog;
	static gint result=0;
	static gchar *filename_open=NULL;
	static FILE *fb_open=NULL;
	static GtkTextBuffer *buffer_open;
	static gchar *buff=NULL;
	static gint fb_size=0;
	GtkTextIter start_open;
	GtkTextIter end_open;
    
	buff=(char *)malloc(sizeof(char)*1024);
	dialog=gtk_file_chooser_dialog_new(_("Please choose to open the file"),NULL,GTK_FILE_CHOOSER_ACTION_OPEN,("_取消"),GTK_RESPONSE_CANCEL,("_确定"),GTK_RESPONSE_OK,NULL);
	result=gtk_dialog_run(GTK_DIALOG(dialog));
    
	if(result==GTK_RESPONSE_OK){
		filename_open=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		fb_open=fopen(filename_open,"r");
			if(fb_open==NULL){
	contains_message (GTK_MESSAGE_ERROR,_("Failed to open file"));
					return;
			}
		else{
			printf("%s\n",filename_open);
			 fseek(fb_open,0L,SEEK_END); 
			fb_size=ftell(fb_open);
			fseek(fb_open,0L,SEEK_SET);
			gtk_widget_destroy (dialog);
			
		}
		buffer_open=gtk_text_view_get_buffer (GTK_TEXT_VIEW(data_open));
		gtk_text_buffer_get_start_iter (buffer_open,&start_open);
		gtk_text_buffer_get_end_iter (buffer_open,&end_open);
		//gtk_text_buffer_create_tag (buffer_open,"bg","background","red",NULL);
		//gtk_text_buffer_apply_tag_by_name (buffer_open,"bg",&start_open,&end_open);
		//buff=gtk_text_buffer_get_text (buffer_open,&start_open,&end_open,TRUE);
		//printf("%s\n",buff);
		gtk_text_buffer_delete (buffer_open,&start_open,&end_open);
		//fread(buff,sizeof(),sizeof(fb_open),fb_open);
		//while(fgets(buff,200,fb_open) !=NULL)
		//{
		//printf("%s\n",buff);
		//}
		
		printf("fb_size is %d\n",fb_size);
		buff=(char *)malloc(fb_size+1);
		if(fread(buff,fb_size,1,fb_open))
		//gtk_text_buffer_get_iter_at_line_index (buffer_open,&iter_open,1,0);
		gtk_text_buffer_get_iter_at_offset (buffer_open,&start_open,0);
		gtk_text_buffer_insert (buffer_open,&start_open,buff,-1);
		//printf("%s\n",buff);
		file_chang=1;
		//g_signal_connect(G_OBJECT(buffer_open),"changed",G_CALLBACK(open_filechange_flag),NULL);
		gtk_window_set_title (GTK_WINDOW(window_space),filename_open);
		printf("titile is %s\n",gtk_window_get_title (GTK_WINDOW(window_space)));
		g_free(buff);
		fclose (fb_open);
	//gtk_widget_destroy (dialog);
	}
	else{
		gtk_widget_destroy (dialog);

	}
	}

void menuitem22_open01(GtkWidget *window,gpointer data)//文件有改动当打开操作做的处理
{
		
GtkWidget *dialog_quit;
		static gint result_quit=0;
		if(file_chang==1){
			menuitem22_open(window_space,data);
			//g_printf("1111\n");
		}
	if(file_chang==0){
		//contains_message (GTK_MESSAGE_WARNING,"SSSSSS");
			dialog_quit=gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_YES_NO,
					                                   _("The contents of the change,not or do quit"));
			result_quit=gtk_dialog_run (GTK_DIALOG(dialog_quit));
			if(result_quit==GTK_RESPONSE_YES){
					gtk_widget_destroy (dialog_quit);
					menuitem22_open(window_space,data);
					//printf ("0000\n");
				}
				else{
					gtk_widget_destroy (dialog_quit);
					//gtk_widget_destroy (data);
					//gtk_widget_destroy (dialog_quit);
					//return;
				
				}
	}

}
void create_file_chooser(char *buf)
{
        GtkWidget *dialog;
        static gint result;
        static char *filenam_save=NULL;
        //FILE *fp;
        dialog=gtk_file_chooser_dialog_new(_("Please enter the name of the save file"),NULL,GTK_FILE_CHOOSER_ACTION_SAVE,("_取消"),GTK_RESPONSE_CANCEL,("_保存"),GTK_RESPONSE_OK,NULL);
        result=gtk_dialog_run(GTK_DIALOG(dialog));
        if(result==GTK_RESPONSE_OK){
                filenam_save=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
                //printf("%s\n",filenam); 
                if(real_save(filenam_save,buf)==0){
					contains_message (GTK_MESSAGE_INFO,_("Save success"));
				gtk_widget_destroy(dialog);
}
		}
        else{
                printf("NO\n");
              gtk_widget_destroy(dialog);

}
}





void menuitem4_sav(GtkWidget *window,gpointer data)//另保存
{

        static char *buf;
        GtkTextBuffer *buffer;
        GtkTextIter  start,end;
        buf=(char *)malloc(sizeof(char)*1024);
        buffer=gtk_text_view_get_buffer(data);
        gtk_text_buffer_get_start_iter(buffer,&start);
        gtk_text_buffer_get_end_iter(buffer,&end);
        buf=gtk_text_buffer_get_text(buffer,&start,&end,TRUE);
        create_file_chooser(buf);
		file_chang=1;
        printf("%s\n",buf);
//      g_free(buf);
        printf("1111\n");
}

void menuitem33_save(GtkWidget *window,gpointer data)//保存
{
	//printf("save\n");
		const char *buf_save=NULL;
		const char *path_name=NULL;
		GtkTextIter  start,end;
		GtkTextBuffer *buffer;
		buf_save=(char *)malloc(sizeof(char)*1024);
		//GtkTextBuffer *buffer33;
		buf_save=gtk_window_get_title (GTK_WINDOW(window_space));
        buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_save));
        gtk_text_buffer_get_start_iter(buffer,&start);
        gtk_text_buffer_get_end_iter(buffer,&end);
        buf_save=gtk_text_buffer_get_text(buffer,&start,&end,TRUE);
			
	if(gtk_window_get_title (GTK_WINDOW(window_space)) ==NULL)
	{
				//menuitem4_sav(window,text_view_save);	
				 create_file_chooser((char *)buf_save);
				file_chang=1;

	}
		if(gtk_window_get_title (GTK_WINDOW(window_space)) != NULL)
	{
			path_name=gtk_window_get_title (GTK_WINDOW(window_space));
			real_save((char *)path_name,(char *)buf_save);
			file_chang=1;
			printf("%s\n",buf_save);

	}

}


void menuitem55_quit(GtkWidget *window,gpointer data)//文件有改动时候退出键的处理
{
		GtkWidget *dialog_quit;
		static gint result_quit=0;
		if(file_chang==1){
			gtk_widget_destroy (data);
			//g_printf("1111\n");
		}
	if(file_chang==0){
		//contains_message (GTK_MESSAGE_WARNING,"SSSSSS");
			dialog_quit=gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_YES_NO,
					                                   _("The contents of the change,not or do quit"));
			result_quit=gtk_dialog_run (GTK_DIALOG(dialog_quit));
			if(result_quit==GTK_RESPONSE_YES){
					gtk_widget_destroy (dialog_quit);
					gtk_widget_destroy (data);
					//printf ("0000\n");
				}
				else{
					gtk_widget_destroy (dialog_quit);
					//return;
				
				}
	}

}
void curror_marr(GtkTextBuffer *buffer_mar,gpointer data)//光标的移动的信号函数
{
	static int row,low;
	static char *msg=NULL;
	msg=(char *)malloc(sizeof(msg)*100);
	//GtkTextIter iter;
	gtk_statusbar_pop (GTK_STATUSBAR(status11),0);
	gtk_text_buffer_get_iter_at_mark (buffer_mar,&iter,gtk_text_buffer_get_insert (buffer_mar));
	row=gtk_text_iter_get_line (&iter);
	low=gtk_text_iter_get_line_index (&iter);
	sprintf(msg,_("Number of rows: %10d    Number of column: %10d"),row,low);
	gtk_statusbar_push (GTK_STATUSBAR(status11),0,msg);

		g_free(msg);

}
void button_next_fun(GtkWidget *window,gpointer *data)//查找下一个的按钮信号函数
	{

		const char *buffer23=NULL;
		static	int jj=0;
		GtkTextBuffer *buffer_find;
		static int offset_seach;//当前距离开始的位置
		GtkTextIter start_iter_find,end_iter_find,start_iter_seach,end_iter_seach;
		//buffer_entry=(char *)malloc(sizeof(buffer_entry)*1024);
		
		buffer23=gtk_entry_get_text (GTK_ENTRY(entry_find));
		
		
		buffer_find=gtk_text_view_get_buffer (GTK_TEXT_VIEW(text_view_save));
		gtk_text_buffer_get_bounds (buffer_find,&start_iter_find,&end_iter_find);
		gtk_text_buffer_remove_tag (buffer_find,tag2,&start_iter_find,&end_iter_find);
		//buffer23=gtk_text_buffer_get_text (buffer_find,&start_iter_find,&end_iter_find,TRUE);
		while(gtk_text_iter_forward_search(&start_iter_find,buffer23,
GTK_TEXT_SEARCH_TEXT_ONLY | GTK_TEXT_SEARCH_VISIBLE_ONLY,
&start_iter_seach, &end_iter_seach, NULL)){
			gtk_text_buffer_apply_tag(buffer_find,tag2,
&start_iter_seach, &end_iter_seach);
			offset_seach=gtk_text_iter_get_offset (&end_iter_seach);
			gtk_text_buffer_get_iter_at_offset (buffer_find,&start_iter_find,offset_seach);
			printf("%d\n",jj);
			jj++;

		}
	
	//printf("%s\n",buffer23);

}
void menu_find_fun(GtkWidget *window,gpointer data)//查找按钮信号函数
{
	//printf("11111\n");
	gtk_widget_show (data);
	
}
void menu_find_fun_quit(GtkWidget *window,gpointer data)//查找退出隐藏窗口
{
	printf("101001011\n");
	//gtk_widget_show (data);
	GtkTextIter start_iter_quit,end_iter_quit;
		GtkTextBuffer *buffer_quit;
		buffer_quit=gtk_text_view_get_buffer (GTK_TEXT_VIEW(text_view_save));
		gtk_text_buffer_get_bounds (buffer_quit,&start_iter_quit,&end_iter_quit);
		gtk_text_buffer_remove_tag (buffer_quit,tag2,&start_iter_quit,&end_iter_quit);
	gtk_widget_hide (data);

}
void create_textview()//创建textview的主函数(新建Dockerfile基面)
{
        //GtkWidget *window;
        GtkBuilder *builder;
		GtkTextBuffer *buffer_new;
        GtkWidget *menuitem33;
		 GtkWidget *menuitem4;
		GtkWidget *menuitem22;
		GtkWidget *menuitem55;
		GtkWidget *menu_find;
        //GtkWidget *menubar;
		
		GtkWidget *box22;
		GtkWidget *scro;
		//GtkTextIter start_ss,end_ss,iter33;
		//GtkTextTag *tag;
		//GtkWidget *status11;
        //GtkWidget *text_view_save;
       // gtk_init(&argc,&argv);
        builder=gtk_builder_new();
        gtk_builder_add_from_file(builder,TEXT_VIEW,NULL);
        window_space=GTK_WIDGET(gtk_builder_get_object(builder,TEXT_VIEW_WINDOW));
	    gtk_widget_set_name(window_space, "nfs_dockerfile_manage");
       // menubar=GTK_WIDGET(gtk_builder_get_object(builder,"menubar1"));
        menuitem33=GTK_WIDGET(gtk_builder_get_object(builder,"imagemenuitem3"));
		menuitem22=GTK_WIDGET(gtk_builder_get_object(builder,"imagemenuitem2"));
		 menuitem4=GTK_WIDGET(gtk_builder_get_object(builder,"imagemenuitem4"));
		menuitem55=GTK_WIDGET(gtk_builder_get_object(builder,"imagemenuitem5"));
		scro=GTK_WIDGET(gtk_builder_get_object(builder,"scrolledwindow1"));
	//gtk_window_set_title(window_space,_("new_build_Docker_file"));
	    /*******查找字符*******/
		menu_find=GTK_WIDGET(gtk_builder_get_object(builder,"menuitem3"));
	
		box22=GTK_WIDGET(gtk_builder_get_object(builder,"box2"));
		button_quit=GTK_WIDGET(gtk_builder_get_object(builder,"button2"));
		entry_find=GTK_WIDGET(gtk_builder_get_object(builder,"entry1"));
		button_next=GTK_WIDGET(gtk_builder_get_object(builder,"button1"));
	/******** 查找字符*******/
	status11=GTK_WIDGET(gtk_builder_get_object(builder,"statusbar1"));

		text_view_save=gtk_source_view_new (); //源码编辑文本创建!!
		gtk_container_add(GTK_CONTAINER(scro),text_view_save);
		gtk_source_view_set_show_line_marks(GTK_SOURCE_VIEW(text_view_save),TRUE); //显示行号栏
		gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(text_view_save),TRUE);//行号栏里面显示数字
		gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(text_view_save),TRUE);//突显当>前行
		//GtkTextTagTable *table = gtk_text_tag_table_new();
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view_save),GTK_WRAP_CHAR); //设置自动换行的
		gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(text_view_save),TRUE); //启动自动缩进的文>

	
        //text_view_save=GTK_WIDGET(gtk_builder_get_object(builder,"textview1"));
			//text_view_save=GTK_WIDGET(gtk_builder_get_object(builder,"gtksourceview1"));
		buffer_new=gtk_text_view_get_buffer (GTK_TEXT_VIEW(text_view_save));
		file_chang=1;
		//gtk_text_buffer_get_iter_at_offset (buffer_new,&iter,10);
		tag=gtk_text_buffer_create_tag (buffer_new,"bg11","scale",PANGO_SCALE_X_LARGE,NULL);
		tag1=gtk_text_buffer_create_tag (buffer_new,"color11","foreground","gray",NULL);
		tag2=gtk_text_buffer_create_tag (buffer_new,"color22","foreground","red",NULL);

		//gtk_source_view_set_show_line_marks(text_view_save,TRUE); //显示行号栏
	
		//gtk_source_view_set_show_line_numbers(text_view_save,TRUE);
		//gtk_text_buffer_get_bounds (buffer_new,&start_ss,&end_ss);
		//gtk_text_view_forward_display_line (text_view_save,&start_ss);
		//printf ("%d\n",gtk_text_view_starts_display_line(text_view_save,&start_ss));
		g_signal_connect(G_OBJECT(menuitem4),"activate",G_CALLBACK(menuitem4_sav),text_view_save);

		g_signal_connect(G_OBJECT(menuitem22),"activate",G_CALLBACK(menuitem22_open01),text_view_save);
		g_signal_connect(G_OBJECT(menuitem55),"activate",G_CALLBACK(menuitem55_quit),window_space);
		g_signal_connect(G_OBJECT(menuitem33),"activate",G_CALLBACK(menuitem33_save),window_space);
		
		g_signal_connect(G_OBJECT(buffer_new),"changed",G_CALLBACK(open_filechange_flag),text_view_save);//检测新建内容由有无变化
		//g_signal_connect(G_OBJECT(window_space),"key-press-event",G_CALLBACK(key_pree_fun),NULL);
		//g_signal_connect(G_OBJECT(buffer_new),"changed",G_CALLBACK(menuitem22_open),text_view_save);
	g_signal_connect(G_OBJECT(window_space),"destroy",G_CALLBACK(gtk_main_quit),NULL);
		g_signal_connect(G_OBJECT(buffer_new),"mark_set",G_CALLBACK(curror_marr),buffer_new);//光标的移动的信号
		g_signal_connect(G_OBJECT(menu_find),"activate",G_CALLBACK(menu_find_fun),box22);//查找按钮的信号
		g_signal_connect(G_OBJECT(button_quit),"clicked",G_CALLBACK(menu_find_fun_quit),box22);//查找退出隐藏窗口
		g_signal_connect(G_OBJECT(button_next),"clicked",G_CALLBACK(button_next_fun),tag2);//查找下一个的按钮信号
		
        gtk_widget_show_all(window_space);
		gtk_widget_hide (box22);
		g_object_unref (builder);
       // gtk_main();




}
