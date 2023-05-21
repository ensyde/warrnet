#ifndef CHATCMDS_H
#define CHATCMDS_H

const char *ChatCommands[] = {
	"/m", "/w", "/msg", "/whisper",		// Whisper user
	"/whois", "/whereis", "/where",		// Find a user
	"/emote", "/me",							// Emote
	"/help", "/?",								// Help menu (with no args)
	"/rejoin", "/resign",					// Give up operator status
	"/unsquelch", "/unignore",				// Un-Ignore a user
	"/squelch", "/ignore",					// Ignore a user
	"/channel", "/join",						// Join a channel
	"/friends", "/f",							// Friends list command
	"/away",										// Set status to away
	"/dnd",										// Do not disturb mode, users may not whisper you
	"/options", "/o",							// Set chat options
	"/ban",										// Ban user from channel
	"/kick",										// Kick user from channel
	"/beep",										// Send a beep (bots only?)
	"/designate",								// Designate next operator once you leave/resign
	"/nobeep",									// Disable beeps
	"/stats",									// Get game record for a user
	"/time",										// Get server time
	"/unban",									// Unban a user from the channel
	"/users",									// Get user statistics for server
	"/whoami",									// Echos back your user info
	"/who",
	NULL
};

const char *ChatCmdHelp[] = {

/************************************/
/* Cmd#| Command descripton         */
/************************************/
/*  0  */"/m [aliases: /whisper, /w, /msg]: Type \"/whisper Username Message\" to send someone a private message.",
/*  0  */"/w [aliases: /m, /whisper, /msg]: Type \"/whisper Username Message\" to send someone a private message.",
/*  0  */"/msg [aliases: /m, /w, /whisper]: Type \"/whisper Username Message\" to send someone a private message.",
/*  0  */"/whisper [aliases: /m, /w, /msg]: Type \"/whisper Username Message\" to send someone a private message.",

/*  1  */"/whois [aliases: /whereis, /where]: Type \"/where Username\" to locate the user and find out their client type.",
/*  1  */"/whereis [aliases: /whois, /where]: Type \"/where Username\" to locate the user and find out their client type.",
/*  1  */"/where [aliases: /whereis, /whois]: Type \"/where Username\" to locate the user and find out their client type.",

/*	 2	 */"/emote [aliases: /me]: Type \"/emote cries\" to do an emotion from the first person perspective.",
/*	 2	 */"/me [aliases: /emote]: Type \"/emote cries\" to do an emotion from the first person perspective.",

/*	 3	 */"/help [aliases: /?]: Type \"/help\" to display this help menu.",
/*	 3	 */"/? [aliases: /help]: Type \"/help\" to display this help menu.",

/*	 4	 */"/rejoin [aliases: /resign]: Type \"/rejoin\" to give up your operator status to someone else.",
/*	 4	 */"/resign [aliases: /rejoin]: Type \"/rejoin\" to give up your operator status to someone else.",

/*	 5	 */"/unsquelch [aliases: /unignore]: Type \"/unignore Username\" to take Username off your ignore list.",
/*	 5	 */"/unignore [aliases: /unsquelch]: Type \"/unignore Username\" to take Username off your ignore list.",

/*	 6	 */"/squelch [aliases: /ignore]: Type \"/ignore Username\" to add someone to your ignore list (you will no longer hear them speak).",
/*	 6	 */"/ignore [aliases: /squelch]: Type \"/ignore Username\" to add someone to your ignore list (you will no longer hear them speak).",

/*	 7	 */"/channel [aliases: /join]: Type \"/channel ChannelName\" to join a channel.",
/*	 7	 */"/join [aliases: /channel]: Type \"/channel ChannelName\" to join a channel.",

/* -1  */"/friends [aliases: /f]: Not implemented.",
/* -1  */"/f [aliases: /friends]: Not implemented.",

/*	 8	 */"/away [aliases: none]: Type \"/away Message\" to set your status to away, and to respond automatically to any whispers.",
/*	 9	 */"/dnd [aliases: none]: Type \"/dnd Message\" this will block all incoming whispers and display the Message to the whispering user.",

/*	 10 */"/options [aliases: /o]: There are no plans to implement this feature in War.net in the near future.",
/*	 10 */"/o [aliases: /options]: There are no plans to implement this feature in War.net in the near future.",

/*	 11 */"/ban [aliases: none]: Type \"/ban Username\" to ban Username from the channel (operators only).",
/*	 12 */"/kick [aliases: none]: Type \"/kick Username\" to kick Username from the channel (operators only).",
/*	 13 */"/beep [aliases: none]: Type \"/beep\" to send other chat clients a beep message.",
/*	 14 */"/designate [aliases: none]: Type \"/designate Username\" to designate the next operator after you leave (operators only).",
/*	 15 */"/nobeep [aliases: none]: Type \"/nobeep\" to disable your client from receiving /beep commands.",
/*  -1 */"/stats [aliases: none]: Not implemented.",
/*	 16 */"/time [aliases: none]: Type \"/time\" to get the current time.",
/*	 17 */"/unban [aliases: none]: Type \"/unban Username\" to unban Username from the channel (operators only).",
/*	 18 */"/users [aliases: none]: Type \"/users\" to get server statistics.",
/*	 19 */"/whoami [aliases: none]: Type \"/whoami\" to get your personal info.",

	NULL
};

#endif
