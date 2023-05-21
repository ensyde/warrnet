# warrnet
If you are having problems w/ the password not working, try and get rid of
the optimizations in the Makefile.

======================

[20:12:31] Moonshine: what's the server called
[20:12:42] c0ol: WaRRNeT
[20:12:44] c0ol: or something
[20:12:47] Moonshine: lol k
[20:13:02] Moonshine: warnet isnt bad actually

Startup:
	preload 4096/256 threads (16)

Main:
	accept connection
		check if they have not connected 8 times
	loop through all the threads and give
		new connection to one of them

Child (Thread)
	wait for events
		handle em :)

Client (Each and every bot)
	wait for out handle event to get called
		handle whatever :)

======================
For Us Coders, cvs 101

	1) Add to .bashrc or .bash_login or just type it each time:
	
		export CVS_RSH=ssh
		export CVSROOT=:ext:YOURUSERNAMEHERE@cvs.war-net.sourceforge.net:/cvsroot/war-net

	2) Check it out
	
		cvs co warnet
		
	3) Before editing the source, remember to update it each time
	
		cvs update
		
	4) Edit the source file, make sure your not working on a file someone else
	   is or we will start locking the files, which is a pain and we don't
	   want to bother with that :)
	
	5) Commit your changes to that file, leave a nice description
	
		cvs commit

	Note: Only steps 3 through 5 will typically be needed.
//hacked
