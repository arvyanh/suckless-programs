/* See LICENSE file for copyright and license details. */

/* Constants */
#include <X11/X.h>
//primary terminal
#define TERMINAL   "terminator"
#define TERMCLASS  "terminator"
//backup
#define TERMINAL2  "alacritty"
//#define TERMINAL2  "rxvt-unicode"


/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 5;       /* horiz inner gap between windows */
static unsigned int gappiv    = 5;       /* vert inner gap between windows */
static unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 5;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "mono:pixelsize=13:antialias=true:autohint=true", "WenQuanYi Zen Hei Mono", "JoyPixels:pixelsize=12:antialias=true:autohint=true", "FreeMono:style=Bold Oblique"};
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#0099ff";
static char selbgcolor[]            = "#005577";

/* commands */
//static const char *volupcmd[]  = { "mpc", "volume", "+1", NULL };
//static const char *voldowncmd[]  = { "mpc", "volume", "-1", NULL };

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
    const char *name;
    const void *cmd;
} Sp;

const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };

static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm",      spcmd1},
    {"spranger",    spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
    */
    /* class    instance      title          tags mask    isfloating   isterminal  noswallow  monitor */
    { "Gimp",     NULL,       NULL,             1 << 8,       0,           0,         0,        -1 },
    { TERMCLASS,   NULL,       NULL,            0,            0,           1,         0,        -1 },
    { NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
    { NULL,      "spterm",    NULL,             SPTAG(0),     1,           1,         0,        -1 },
    { NULL,      "spcalc",    NULL,             SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",    tile },         /* Default: Master on left, slaves on right */
    { "TTT",    bstack },       /* Master on top, slaves on bottom */

    { "[@]",    spiral },       /* Fibonacci spiral */
    { "[\\]",   dwindle },      /* Decreasing in size right and leftward */

    { "H[]",    deck },         /* Master on left, slaves in monocle-like mode on right */
    { "[M]",    monocle },      /* All windows on top of eachother */

    { "|M|",    centeredmaster },       /* Master in middle, slaves on sides */
    { ">M>",    centeredfloatingmaster },   /* Same but master floats */

    { "><>",    NULL },         /* no layout function means floating behavior */
    { NULL,     NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
    { MOD,  XK_j,   ACTION##stack,  {.i = INC(+1) } }, \
    { MOD,  XK_k,   ACTION##stack,  {.i = INC(-1) } }, \
    { MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
    /* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
    /* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
    /* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
    /* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };
static const char *termcmd2[] = { TERMINAL2, NULL };
static const char *termcmd3[] = { TERMINAL, "-e", "tmux", "new-session", "-A", "-s", "dwm_default_tmux", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
        { "dwm.color0",     STRING, &normbordercolor },
        { "dwm.color8",     STRING, &selbordercolor },
        { "dwm.color0",     STRING, &normbgcolor },
        { "dwm.color4",     STRING, &normfgcolor },
        { "dwm.color0",     STRING, &selfgcolor },
        { "dwm.color4",     STRING, &selbgcolor },
        { "borderpx",       INTEGER, &borderpx },
        { "snap",       INTEGER, &snap },
        { "showbar",        INTEGER, &showbar },
        { "topbar",     INTEGER, &topbar },
        { "nmaster",        INTEGER, &nmaster },
        { "resizehints",    INTEGER, &resizehints },
        { "mfact",      FLOAT,  &mfact },
        { "gappih",     INTEGER, &gappih },
        { "gappiv",     INTEGER, &gappiv },
        { "gappoh",     INTEGER, &gappoh },
        { "gappov",     INTEGER, &gappov },
        { "swallowfloating",    INTEGER, &swallowfloating },
        { "smartgaps",      INTEGER, &smartgaps },
};


/*
Mask        | Value | Key
------------+-------+------------
ShiftMask   |     1 | Shift
LockMask    |     2 | Caps Lock
ControlMask |     4 | Ctrl
Mod1Mask    |     8 | Alt
Mod2Mask    |    16 | Num Lock
Mod3Mask    |    32 | Scroll Lock
Mod4Mask    |    64 | Windows
*/


#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
    /* modifier                     key        function        argument */
    STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)
    /* { MODKEY|ShiftMask,      XK_Escape,  spawn,  SHCMD("") }, */
    //{ MODKEY,         XK_grave,   spawn,  SHCMD("dmenuunicode") },
    /* { MODKEY|ShiftMask,      XK_grave,   togglescratch,  SHCMD("") }, */
    TAGKEYS(            XK_1,       0)
    TAGKEYS(            XK_2,       1)
    TAGKEYS(            XK_3,       2)
    TAGKEYS(            XK_4,       3)
    TAGKEYS(            XK_5,       4)
    TAGKEYS(            XK_6,       5)
    TAGKEYS(            XK_7,       6)
    TAGKEYS(            XK_8,       7)
    TAGKEYS(            XK_9,       8)
    { MODKEY,           XK_0,       view,       {.ui = ~0 } },
    { MODKEY|ShiftMask, XK_0,       tag,        {.ui = ~0 } },      /*sends to 0 (i.e. every tag)*/
    /*{ MODKEY,         XK_minus,   spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,       XK_minus,   spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_equal,   spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,       XK_equal,   spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_BackSpace,   spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,       XK_BackSpace,   spawn,      SHCMD("") },*/
    
    { MODKEY,           XK_Tab,     view,       {0} },
    /*{ XK_Alt_L,           XK_Tab,     zoom,       {1} },*/    /*alt tab switch tags*/
    { MODKEY,           XK_space,   zoom,       {1} },  /* to top of stack */
    { MODKEY|ShiftMask,     XK_space,   togglefloating, {0} },
    /* { MODKEY|ShiftMask,      XK_Tab,     spawn,      SHCMD("") }, */
    { MODKEY,           XK_s,       togglesticky,   {0} },          /*shows up in every window*/
    { MODKEY|ShiftMask,         XK_s,       spawn,  SHCMD("scrot -b '%Y:%m:%d:%H:%M:%S.png' -e 'mv $f ~'") },           /*shows up in every window*/

    { MODKEY,                           XK_Print,       spawn,  SHCMD("gnome-screenshot") },            
    { MODKEY|ShiftMask,                 XK_Print,       spawn,  SHCMD("gnome-screenshot -a") },         
    { MODKEY|ShiftMask|ControlMask,     XK_Print,       spawn,  SHCMD("flameshot gui") },           

    /*system*/
    { MODKEY,           XK_q,       killclient, {0} },
    { MODKEY|ShiftMask|ControlMask,     XK_q,       quit,       {0} },
    //syspend to RAM (save program state in ram and sleep)
    { MODKEY|ShiftMask,     XK_q,       spawn,      SHCMD("systemctl suspend") },

    /*{ MODKEY,         XK_w,       spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,       XK_w,       spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_e,       spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,       XK_e,       spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_r,       spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,       XK_r,       spawn,      SHCMD("") },*/

    /* multimedia section */

    //  { 0,                            XF86XK_AudioLowerVolume, spawn,         SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -10%")},
    //  { 0,                            XF86XK_AudioRaiseVolume, spawn,         SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +10%") },
    //  { 0,                            XF86XK_AudioMute,        spawn,         SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },
    //  { MODKEY,                            XK_F3,      spawn,         SHCMD("cmus-remote -n; mocp -f") },     //next
    //  { MODKEY,                            XK_F2,      spawn,         SHCMD("cmus-remote -r; mocp -r") },     //prev
    //  { MODKEY,                            XK_F1,              spawn,         SHCMD("cmus-remote -u; mocp -G") },     //toggle stop
    //  { 0,                            XF86XK_MonBrightnessUp,      spawn,         SHCMD("ybacklight -inc 5") },
    //  { 0,                            XF86XK_MonBrightnessDown,        spawn,         SHCMD("ybacklight -dec 5") },
    //// not testest if failed, try static const char *brupcmd[] = { "brightnessctl", "set", "10%+", NULL };
    //// and { 0, XF86XK_MonBrightnessUp,  spawn,          {.v = brupcmd} },
    //{ 0, XF86XK_MonBrightnessUp,  spawn,              SHCMD("brightnessctl set 10%+")},
    //{ 0, XF86XK_MonBrightnessDown, spawn,             SHCMD("brightnessctl set 10%-")},
    { MODKEY,           XK_t,       setlayout,  {.v = &layouts[0]} }, /* tile */
    { MODKEY|ShiftMask,     XK_t,       setlayout,  {.v = &layouts[1]} }, /* bstack */
    { MODKEY,           XK_y,       setlayout,  {.v = &layouts[2]} }, /* spiral */
    { MODKEY|ShiftMask,     XK_y,       setlayout,  {.v = &layouts[3]} }, /* dwindle */
    { MODKEY,           XK_u,       setlayout,  {.v = &layouts[4]} }, /* deck */
    { MODKEY|ShiftMask,     XK_u,       setlayout,  {.v = &layouts[5]} }, /* monocle */
    { MODKEY,           XK_i,       setlayout,  {.v = &layouts[6]} }, /* centeredmaster */
    { MODKEY|ShiftMask,     XK_i,       setlayout,  {.v = &layouts[7]} }, /* centeredfloatingmaster */
    /*{ MODKEY,         XK_o,       setlayout,  {.v = &layouts[6]} },*/ /* centeredmaster */
    /*{ MODKEY|ShiftMask,       XK_o,       setlayout,  {.v = &layouts[7]} },*/ /* centeredfloatingmaster */
    { MODKEY,           XK_m,       incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask,     XK_m,       incnmaster,     {.i = -1 } },
    /*{ MODKEY,         XK_p,           spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,       XK_p,           spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_bracketleft,     spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,       XK_bracketleft,     spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_bracketright,    spawn,      SHCMD("") },*/
    /*{ MODKEY|ShiftMask,       XK_bracketright,    spawn,      SHCMD("") },*/
    { MODKEY,           XK_backslash,       view,       {0} },
    /* { MODKEY|ShiftMask,      XK_backslash,       spawn,      SHCMD("") }, */

    { MODKEY,           XK_a,       togglegaps, {0} },
    { MODKEY|ShiftMask,     XK_a,       defaultgaps,    {0} },
    { MODKEY,           XK_d,       spawn,          SHCMD("dmenu_run > /tmp/dmenu_out") },
    /* { MODKEY,            XK_d,       spawn,      SHCMD("") } }, */
    { MODKEY,           XK_f,       togglefullscr,  {0} },
    { MODKEY|ShiftMask,     XK_f,       setlayout,  {.v = &layouts[8]} },  /*floating mode*/
    { MODKEY,           XK_g,       shiftview,  { .i = -1 } },
    { MODKEY|ShiftMask,     XK_g,       shifttag,   { .i = -1 } },
    { MODKEY,           XK_h,       setmfact,   {.f = -0.05} },
    /* J and K are automatically bound above in STACKEYS */
    { MODKEY,           XK_l,       setmfact,       {.f = +0.05} },
    { MODKEY,           XK_semicolon,   shiftview,  { .i = 1 } },
    { MODKEY|ShiftMask,     XK_semicolon,   shifttag,   { .i = 1 } },
    { MODKEY,           XK_apostrophe,  togglescratch,  {.ui = 1} },
    /* { MODKEY|ShiftMask,      XK_apostrophe,  spawn,      SHCMD("") }, */
    { MODKEY,                           XK_Return,  spawn,          {.v = termcmd } },
    { MODKEY|ShiftMask,                 XK_Return,  spawn,          {.v = termcmd2} },
    { MODKEY|ControlMask|ShiftMask,     XK_Return,  spawn,          {.v = termcmd3} },

    { MODKEY,           XK_z,       incrgaps,   {.i = +5 } },
    { MODKEY,           XK_x,       incrgaps,   {.i = -5 } },
    /* { MODKEY,            XK_c,       spawn,      SHCMD("") }, */
    /* { MODKEY|ShiftMask,      XK_c,       spawn,      SHCMD("") }, */
    /* V is automatically bound above in STACKKEYS */
    /*{ MODKEY,         XK_b,       togglebar,  {0} },*/
    /* { MODKEY|ShiftMask,      XK_b,       spawn,      SHCMD("") }, */
    /*{ MODKEY,         XK_n,       spawn,      SHCMD(TERMINAL "") },*/
    /*{ MODKEY|ShiftMask,       XK_n,       spawn,      SHCMD(TERMINAL "") },*/
    /*{ MODKEY,         XK_m,       spawn,      SHCMD(TERMINAL "") },*/
    /*{ MODKEY|ShiftMask,       XK_m,       spawn,      SHCMD("") },*/
    //{ MODKEY,         XK_comma,   spawn,          SHCMD("") },
    //{ MODKEY|ShiftMask,       XK_comma,   spawn,      SHCMD("") },
    //{ MODKEY,         XK_period,  spawn,          SHCMD("") },
    //{ MODKEY|ShiftMask,       XK_period,  spawn,      SHCMD("") },

    //change focused monitor
    { MODKEY,               XK_Left,    focusmon,   {.i = -1 } },
    { MODKEY|ShiftMask,     XK_Left,    tagmon,     {.i = -1 } },
    { MODKEY,               XK_Right,   focusmon,   {.i = +1 } },
    { MODKEY|ShiftMask,     XK_Right,   tagmon,     {.i = +1 } },


    /*multiple work space?*/
    //{ MODKEY,             XK_Up,  focusmon,   {.i = +1 } },
    //{ MODKEY|ShiftMask,       XK_Up,  tagmon,     {.i = +1 } },
    //{ MODKEY,             XK_Down,    focusmon,   {.i = +1 } },
    //{ MODKEY|ShiftMask,       XK_Down,    tagmon,     {.i = +1 } },


    { MODKEY,               XK_Page_Up, shiftview,  { .i = -1 } },
    { MODKEY|ShiftMask,     XK_Page_Up, shifttag,   { .i = -1 } },
    { MODKEY,               XK_Page_Down,   shiftview,  { .i = +1 } },
    { MODKEY|ShiftMask,     XK_Page_Down,   shifttag,   { .i = +1 } },
    { MODKEY,               XK_Insert,  spawn,      SHCMD("") },
    { MODKEY|ShiftMask,     XK_Insert,  spawn,      SHCMD("") },

    /*{ MODKEY,         XK_F1,      spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_F2,      spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_F3,      spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_F4,      spawn,      SHCMD(TERMINAL "") },*/
    /* { MODKEY,            XK_F5,      xrdb,       {.v = NULL } }, */
    /*{ MODKEY,         XK_F6,      spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_F7,      spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_F8,      spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_F9,      spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_F10,     spawn,      SHCMD("") },*/
    /*{ MODKEY,         XK_F11,     spawn,      SHCMD("") },*/
    /* { MODKEY,            XK_F12,     xrdb,       {.v = NULL } }, */
    /* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
    /* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
    /* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
    /* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
    /* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
    /* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
    /* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
    /* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
    /* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
    /* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
    /* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
    /* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
    /* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
    /* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
    /* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
#ifndef __OpenBSD__
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
    { ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
    { ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
    { ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
    { ClkStatusText,        ShiftMask,      Button3,        sigdwmblocks,   {.i = 7} },
#endif
    { ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,         MODKEY,     Button4,    incrgaps,   {.i = +1} },
    { ClkClientWin,         MODKEY,     Button5,    incrgaps,   {.i = -1} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTagBar,        0,      Button4,    shiftview,  {.i = -1} },
    { ClkTagBar,        0,      Button5,    shiftview,  {.i = 1} },
    { ClkRootWin,       0,      Button2,    togglebar,  {0} },
};

