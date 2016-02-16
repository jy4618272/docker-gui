## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

dist_doc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS \
	src/ui/nfs_assistant.ui \
	src/ui/nfs_command_information_modules.ui \
	src/ui/nfs_container_manage.ui \
	src/ui/nfs_container_manage_createcontains.ui \
	src/ui/nfs_dockerbuild_textview.ui \
	src/ui/nfs_docker_modules.ui \
	src/ui/nfs_image.ui \
	src/ui/nfs_image_pull.ui \
	src/ui/nfs_image_save_load_rmi.ui \
	src/ui/nfs_link.ui \
	src/ui/nfs_new_dockerfile.ui \
	src/ui/nfs_plink.ui \
	src/ui/nfs_select_dockerfile.ui \
	src/ui/nfs_volume.ui \
	src/ui/nfs_volume_container_create.ui \
	src/ui/nfs_volume_create.ui \
	src/ui/nfs_volume_backup.ui \
	src/ui/nfs_volume_look.ui \
	src/ui/nfs_image_save.ui \
	src/ui/nfs_volume_transfer.ui


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)
