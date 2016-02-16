## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). \
	$(gtk+-3.0_CFLAGS)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(NFS_APPLICATION_DEPLOY_TOOL_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = nfs_application_deploy_tool

nfs_application_deploy_tool_SOURCES = \
	global/global.c \
	global/global.h \
	manage/nfs-application-deploy-tool.c \
	manage/nfs-application-deploy-tool.h \
	module/nfs_command_information_modules.c \
	module/nfs_command_information_modules.h \
	module/nfs_docker-modules.c \
	module/nfs_docker-modules.h \
	container/nfs_container_manage.c \
	container/nfs_container_manage.h \
	dockerfile/nfs_dockerfile_manage.c \
	dockerfile/nfs_dockerfile_manage.h \
	link/nfs_link.c \
	link/nfs_link.h \
	link/nfs_plink.c \
	link/nfs_plink.h \
	assistant/nfs_assistant.c \
	assistant/nfs_assistant.h \
	image/nfs_image.c \
	image/nfs_image.h \
	volume/nfs_volume.c \
	volume/nfs_volume.h \
	terminal/nfs_terminal.c \
	theme/theme.css	


nfs_application_deploy_tool_LDFLAGS = \
	-Wl,--export-dynamic

nfs_application_deploy_tool_LDADD = $(NFS_APPLICATION_DEPLOY_TOOL_LIBS) \
	$(gtk+-3.0_LIBS)




EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

