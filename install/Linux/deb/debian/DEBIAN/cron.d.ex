#
# Regular cron jobs for the embeddedlite package
#
0 4	* * *	root	[ -x /usr/bin/embeddedlite_maintenance ] && /usr/bin/embeddedlite_maintenance
