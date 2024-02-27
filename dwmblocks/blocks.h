//Modify this file to change what commands output to your statusbar, and recompile using the make command.
//
//pkill dwmblocks -RTMIN+n
static Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	//{"", "dwmblk-todo.sh",						20,		8},
	{"", "dwmblk-sys-monitor.sh",				5,		6},
	{"", "dwmblk-music.sh",						5,		5},

	{"", "dwmblk-battery.sh",					20,		4},
	{"", "dwmblk-internet.sh",					5,		3},
	{"", "dwmblk-volumn.sh",					60,		9},
	{"", "dwmblk-freemem.sh",					2,		2},
	{"", "dwmblk-time.sh",						20,		1},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char *delim = "|";
