#include "global/global.h"
#include "nfs_docker-modules.h"

void  modules_menu_create(GtkWidget *window11,gpointer data)
{
	GtkWidget *menu_item11;
	GtkWidget *menu_item12;
	GtkWidget *menu_item22;
	GtkWidget *menu_item21;
	GtkWidget *menu_item31;
	GtkWidget *menu_item32;
	GtkWidget *menu_item41;
	GtkWidget *menu_item42;
	GtkWidget *menu_item55;
	GtkWidget *menu_item66;
	GtkWidget *menu_item77;
	GtkWidget *menu_item88;
	GtkWidget *imagemenuitem33;
	GtkWidget *imagemenuitem44;
	GtkBuilder *builder;
//	GtkWidget *button_docker11;
	GError* error = NULL;
	GtkWidget *window;
	GtkWidget *terminal_window;
	
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE11, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}
	window = GTK_WIDGET (gtk_builder_get_object (builder, TOP_WINDOW11));
	menu_item11=GTK_WIDGET(gtk_builder_get_object (builder,"menuitem1"));
	menu_item12=GTK_WIDGET(gtk_builder_get_object (builder,"imagemenuitem1"));
	menu_item22=GTK_WIDGET(gtk_builder_get_object (builder,"menuitem2"));
	menu_item21=GTK_WIDGET(gtk_builder_get_object (builder,"imagemenuitem2"));
	menu_item31=GTK_WIDGET(gtk_builder_get_object (builder,"menuitem3"));
	menu_item32=GTK_WIDGET(gtk_builder_get_object (builder,"imagemenuitem5"));
	menu_item41=GTK_WIDGET(gtk_builder_get_object (builder,"menuitem4"));
	menu_item42=GTK_WIDGET(gtk_builder_get_object (builder,"imagemenuitem6"));
	menu_item55=GTK_WIDGET(gtk_builder_get_object (builder,"menuitem5"));
	menu_item66=GTK_WIDGET(gtk_builder_get_object (builder,"menuitem6"));
	menu_item77=GTK_WIDGET(gtk_builder_get_object (builder,"menuitem7"));
	menu_item88=GTK_WIDGET(gtk_builder_get_object (builder,"menuitem8"));
	imagemenuitem33=GTK_WIDGET(gtk_builder_get_object (builder,"imagemenuitem3"));
	imagemenuitem44=GTK_WIDGET(gtk_builder_get_object (builder,"imagemenuitem4"));
	terminal_window = GTK_WIDGET(gtk_builder_get_object (builder, "scrolledwindow1"));
	
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item11),"命令信息");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item12),"进入界面");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item22),"容器管理");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item21),"容器管理信息");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item31),"镜像管理");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item32),"镜像管理信息");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item41),"数据管理");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item42),"数据管理信息");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item55),"配置文件管理");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item66),"网络管理");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item77),"选择Dockerfile构建镜像");
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item88),"新建Dockerfile构建镜像");
	gtk_menu_item_set_label(GTK_MENU_ITEM(imagemenuitem33),"容器互联方式");
	gtk_menu_item_set_label(GTK_MENU_ITEM(imagemenuitem44),"外部访问方式");
	gtk_window_set_title (GTK_WINDOW(window),"方德应用部署工具");
	gtk_widget_set_name(window, "window_modules_css");
	
	gtk_container_add(GTK_CONTAINER(terminal_window), GTK_WIDGET(data));

	g_signal_connect(G_OBJECT(menu_item12),"activate",G_CALLBACK(command_new_window),NULL);
	g_signal_connect(G_OBJECT(window11),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(G_OBJECT(menu_item32),"activate",G_CALLBACK(image_event),window);
	g_signal_connect(G_OBJECT(menu_item42),"activate",G_CALLBACK(volume_event),window);
	g_signal_connect(G_OBJECT(menu_item21),"activate",G_CALLBACK(create_view),NULL);
	g_signal_connect(G_OBJECT(menu_item88),"activate",G_CALLBACK(create_textview),NULL);
	g_signal_connect(G_OBJECT(menu_item77),"activate",G_CALLBACK(select_dockerfile_textview),NULL);
	g_signal_connect(G_OBJECT(imagemenuitem33),"activate",G_CALLBACK(create_link_contains),NULL);
	g_signal_connect(G_OBJECT(imagemenuitem44),"activate",G_CALLBACK(p_link_contains),NULL);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(main_quit),window11);
	gtk_widget_show_all (GTK_WIDGET (window));
}

void main_quit(GtkWidget *window1, GtkWidget *window2)
{
	gtk_widget_destroy (window1);
	gtk_widget_destroy (window2);
}
