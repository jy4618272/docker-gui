#
# Regular cron jobs for the nfs-application-deploy-tool package
#
0 4	* * *	root	[ -x /usr/bin/nfs-application-deploy-tool_maintenance ] && /usr/bin/nfs-application-deploy-tool_maintenance
