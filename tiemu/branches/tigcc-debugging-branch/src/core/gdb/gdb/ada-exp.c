/* A Bison parser, made from ada-exp.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	INT	257
# define	NULL_PTR	258
# define	CHARLIT	259
# define	FLOAT	260
# define	TYPENAME	261
# define	BLOCKNAME	262
# define	STRING	263
# define	NAME	264
# define	DOT_ID	265
# define	OBJECT_RENAMING	266
# define	DOT_ALL	267
# define	SPECIAL_VARIABLE	268
# define	ASSIGN	269
# define	_AND_	270
# define	OR	271
# define	XOR	272
# define	THEN	273
# define	ELSE	274
# define	NOTEQUAL	275
# define	LEQ	276
# define	GEQ	277
# define	IN	278
# define	DOTDOT	279
# define	UNARY	280
# define	MOD	281
# define	REM	282
# define	STARSTAR	283
# define	ABS	284
# define	NOT	285
# define	TICK_ACCESS	286
# define	TICK_ADDRESS	287
# define	TICK_FIRST	288
# define	TICK_LAST	289
# define	TICK_LENGTH	290
# define	TICK_MAX	291
# define	TICK_MIN	292
# define	TICK_MODULUS	293
# define	TICK_POS	294
# define	TICK_RANGE	295
# define	TICK_SIZE	296
# define	TICK_TAG	297
# define	TICK_VAL	298
# define	ARROW	299
# define	NEW	300

#line 38 "ada-exp.y"


#include "defs.h"
#include "gdb_string.h"
#include <ctype.h>
#include "expression.h"
#include "value.h"
#include "parser-defs.h"
#include "language.h"
#include "ada-lang.h"
#include "bfd.h" /* Required by objfiles.h.  */
#include "symfile.h" /* Required by objfiles.h.  */
#include "objfiles.h" /* For have_full_symbols and have_partial_symbols */
#include "frame.h"
#include "block.h"

/* Remap normal yacc parser interface names (yyparse, yylex, yyerror, etc),
   as well as gratuitiously global symbol names, so we can have multiple
   yacc generated parsers in gdb.  These are only the variables
   produced by yacc.  If other parser generators (bison, byacc, etc) produce
   additional global names that conflict at link time, then those parser
   generators need to be fixed instead of adding those names to this list.  */

/* NOTE: This is clumsy, especially since BISON and FLEX provide --prefix
   options.  I presume we are maintaining it to accommodate systems
   without BISON?  (PNH) */

#define	yymaxdepth ada_maxdepth
#define	yyparse	_ada_parse	/* ada_parse calls this after  initialization */
#define	yylex	ada_lex
#define	yyerror	ada_error
#define	yylval	ada_lval
#define	yychar	ada_char
#define	yydebug	ada_debug
#define	yypact	ada_pact
#define	yyr1	ada_r1
#define	yyr2	ada_r2
#define	yydef	ada_def
#define	yychk	ada_chk
#define	yypgo	ada_pgo
#define	yyact	ada_act
#define	yyexca	ada_exca
#define yyerrflag ada_errflag
#define yynerrs	ada_nerrs
#define	yyps	ada_ps
#define	yypv	ada_pv
#define	yys	ada_s
#define	yy_yys	ada_yys
#define	yystate	ada_state
#define	yytmp	ada_tmp
#define	yyv	ada_v
#define	yy_yyv	ada_yyv
#define	yyval	ada_val
#define	yylloc	ada_lloc
#define yyreds	ada_reds		/* With YYDEBUG defined */
#define yytoks	ada_toks		/* With YYDEBUG defined */
#define yyname	ada_name		/* With YYDEBUG defined */
#define yyrule	ada_rule		/* With YYDEBUG defined */

#ifndef YYDEBUG
#define	YYDEBUG	1		/* Default to yydebug support */
#endif

#define YYFPRINTF parser_fprintf

struct name_info {
  struct symbol *sym;
  struct minimal_symbol *msym;
  struct block *block;
  struct stoken stoken;
};

/* If expression is in the context of TYPE'(...), then TYPE, else
 * NULL.  */
static struct type *type_qualifier;

int yyparse (void);

static int yylex (void);

void yyerror (char *);

static struct stoken string_to_operator (struct stoken);

static void write_int (LONGEST, struct type *);

static void write_object_renaming (struct block *, struct symbol *, int);

static void write_var_from_name (struct block *, struct name_info);

static LONGEST convert_char_literal (struct type *, LONGEST);

static struct type *type_int (void);

static struct type *type_long (void);

static struct type *type_long_long (void);

static struct type *type_float (void);

static struct type *type_double (void);

static struct type *type_long_double (void);

static struct type *type_char (void);

static struct type *type_system_address (void);

#line 147 "ada-exp.y"
#ifndef YYSTYPE
typedef union
  {
    LONGEST lval;
    struct {
      LONGEST val;
      struct type *type;
    } typed_val;
    struct {
      DOUBLEST dval;
      struct type *type;
    } typed_val_float;
    struct type *tval;
    struct stoken sval;
    struct name_info ssym;
    int voidval;
    struct block *bval;
    struct internalvar *ivar;

  } yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		182
#define	YYFLAG		-32768
#define	YYNTBASE	66

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 300 ? yytranslate[x] : 80)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    32,    61,
      55,    60,    34,    30,    62,    31,    54,    35,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    59,
      23,    21,    24,     2,    29,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    56,     2,    65,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    63,     2,    64,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    22,    25,    26,    27,    28,
      33,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    57,
      58
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     2,     4,     6,    10,    13,    16,    21,    26,
      27,    35,    36,    43,    47,    49,    51,    53,    57,    60,
      63,    66,    69,    70,    72,    76,    80,    86,    91,    95,
      99,   103,   107,   111,   115,   119,   123,   127,   131,   135,
     139,   145,   151,   155,   162,   169,   174,   178,   182,   186,
     190,   195,   199,   204,   208,   211,   214,   218,   222,   226,
     229,   232,   240,   248,   254,   258,   262,   266,   272,   275,
     276,   280,   282,   284,   285,   287,   289,   291,   293,   295,
     298,   300,   303,   305,   308,   310,   312,   314,   316,   319,
     321,   324,   327,   331,   334,   337
};
static const short yyrhs[] =
{
      67,     0,    79,     0,    71,     0,    67,    59,    71,     0,
      68,    13,     0,    68,    11,     0,    68,    55,    72,    60,
       0,    79,    55,    71,    60,     0,     0,    79,    61,    70,
      69,    55,    71,    60,     0,     0,    68,    55,    71,    28,
      71,    60,     0,    55,    67,    60,     0,    76,     0,    14,
       0,    68,     0,    71,    15,    71,     0,    31,    71,     0,
      30,    71,     0,    40,    71,     0,    39,    71,     0,     0,
      71,     0,    77,    57,    71,     0,    72,    62,    71,     0,
      72,    62,    77,    57,    71,     0,    63,    79,    64,    71,
       0,    71,    38,    71,     0,    71,    34,    71,     0,    71,
      35,    71,     0,    71,    37,    71,     0,    71,    36,    71,
       0,    71,    29,    71,     0,    71,    30,    71,     0,    71,
      32,    71,     0,    71,    31,    71,     0,    71,    21,    71,
       0,    71,    22,    71,     0,    71,    25,    71,     0,    71,
      27,    71,    28,    71,     0,    71,    27,    71,    50,    73,
       0,    71,    27,     7,     0,    71,    40,    27,    71,    28,
      71,     0,    71,    40,    27,    71,    50,    73,     0,    71,
      40,    27,     7,     0,    71,    26,    71,     0,    71,    23,
      71,     0,    71,    24,    71,     0,    71,    16,    71,     0,
      71,    16,    19,    71,     0,    71,    17,    71,     0,    71,
      17,    20,    71,     0,    71,    18,    71,     0,    68,    41,
       0,    68,    42,     0,    68,    43,    73,     0,    68,    44,
      73,     0,    68,    45,    73,     0,    68,    51,     0,    68,
      52,     0,    75,    47,    55,    71,    62,    71,    60,     0,
      75,    46,    55,    71,    62,    71,    60,     0,    75,    49,
      55,    71,    60,     0,    74,    43,    73,     0,    74,    44,
      73,     0,    74,    45,    73,     0,    74,    53,    55,    71,
      60,     0,    74,    48,     0,     0,    55,     3,    60,     0,
       7,     0,    74,     0,     0,     3,     0,     5,     0,     6,
       0,     4,     0,     9,     0,    58,     7,     0,    10,     0,
      78,    10,     0,    12,     0,    78,    12,     0,    10,     0,
       7,     0,    12,     0,     8,     0,    78,     8,     0,     7,
       0,    78,     7,     0,     7,    41,     0,    78,     7,    41,
       0,    34,    71,     0,    32,    71,     0,    71,    56,    71,
      65,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   217,   218,   224,   225,   230,   234,   241,   249,   257,
     257,   266,   269,   274,   277,   280,   284,   287,   291,   295,
     299,   303,   307,   310,   312,   314,   316,   320,   330,   334,
     338,   342,   346,   350,   354,   358,   362,   366,   370,   374,
     378,   380,   385,   390,   394,   400,   408,   412,   416,   420,
     424,   428,   432,   436,   440,   442,   448,   451,   454,   457,
     459,   461,   463,   465,   467,   470,   473,   476,   478,   482,
     484,   488,   495,   497,   504,   508,   515,   523,   527,   535,
     539,   540,   542,   545,   550,   551,   552,   555,   557,   562,
     563,   564,   566,   573,   575,   577
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "INT", "NULL_PTR", "CHARLIT", "FLOAT", 
  "TYPENAME", "BLOCKNAME", "STRING", "NAME", "DOT_ID", "OBJECT_RENAMING", 
  "DOT_ALL", "SPECIAL_VARIABLE", "ASSIGN", "_AND_", "OR", "XOR", "THEN", 
  "ELSE", "'='", "NOTEQUAL", "'<'", "'>'", "LEQ", "GEQ", "IN", "DOTDOT", 
  "'@'", "'+'", "'-'", "'&'", "UNARY", "'*'", "'/'", "MOD", "REM", 
  "STARSTAR", "ABS", "NOT", "TICK_ACCESS", "TICK_ADDRESS", "TICK_FIRST", 
  "TICK_LAST", "TICK_LENGTH", "TICK_MAX", "TICK_MIN", "TICK_MODULUS", 
  "TICK_POS", "TICK_RANGE", "TICK_SIZE", "TICK_TAG", "TICK_VAL", "'.'", 
  "'('", "'['", "ARROW", "NEW", "';'", "')'", "'\\''", "','", "'{'", 
  "'}'", "']'", "start", "exp1", "simple_exp", "@1", "save_qualifier", 
  "exp", "arglist", "tick_arglist", "type_prefix", "opt_type_prefix", 
  "variable", "any_name", "block", "type", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    66,    66,    67,    67,    68,    68,    68,    68,    69,
      68,    70,    68,    68,    68,    68,    71,    71,    71,    71,
      71,    71,    72,    72,    72,    72,    72,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    73,
      73,    74,    75,    75,    71,    71,    71,    71,    71,    71,
      76,    76,    76,    76,    77,    77,    77,    78,    78,    79,
      79,    79,    79,    71,    71,    71
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     1,     1,     3,     2,     2,     4,     4,     0,
       7,     0,     6,     3,     1,     1,     1,     3,     2,     2,
       2,     2,     0,     1,     3,     3,     5,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       5,     5,     3,     6,     6,     4,     3,     3,     3,     3,
       4,     3,     4,     3,     2,     2,     3,     3,     3,     2,
       2,     7,     7,     5,     3,     3,     3,     5,     2,     0,
       3,     1,     1,     0,     1,     1,     1,     1,     1,     2,
       1,     2,     1,     2,     1,     1,     1,     1,     2,     1,
       2,     2,     3,     2,     2,     4
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
      73,    74,    77,    75,    76,    71,    87,    78,    80,    82,
      15,    73,    73,    73,    73,    73,    73,    73,     0,     0,
       1,    16,     3,    72,     0,    14,     0,     2,    91,    19,
       0,    18,    94,    93,    21,    20,     0,    79,    89,     0,
       0,    73,     6,     5,    54,    55,    69,    69,    69,    59,
      60,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,     0,    73,    69,    69,    69,    68,     0,     0,
       0,     0,    90,    88,    81,    83,    73,    11,    13,    73,
       4,     0,    56,    57,    58,    71,    80,    82,    23,     0,
       0,    17,    73,    49,    73,    51,    53,    37,    38,    47,
      48,    39,    46,    42,     0,    33,    34,    36,    35,    29,
      30,    32,    31,    28,    73,     0,    64,    65,    66,    73,
      73,    73,    73,    92,     0,     9,    27,     0,    73,     7,
      73,    73,    50,    52,    73,    69,    45,     0,    95,     0,
       0,     0,     0,     8,     0,    70,     0,    25,     0,    24,
      40,    41,    73,    69,    67,    73,    73,    63,    73,    12,
      73,    43,    44,     0,     0,     0,    26,    62,    61,    10,
       0,     0,     0
};

static const short yydefgoto[] =
{
     180,    20,    21,   154,   135,    22,    99,    92,    23,    24,
      25,   100,    26,    30
};

static const short yypact[] =
{
     246,-32768,-32768,-32768,-32768,    22,-32768,-32768,-32768,-32768,
  -32768,   246,   246,   246,   246,   246,   246,   246,     6,    24,
     -35,    54,   947,   105,   -21,-32768,    77,   -32,-32768,    34,
     -32,    34,     5,     5,    34,    34,    41,-32768,    23,    31,
       9,   246,-32768,-32768,-32768,-32768,    21,    21,    21,-32768,
  -32768,   131,   246,   169,   208,   246,   246,   246,   246,   246,
     246,   246,   284,   246,   246,   246,   246,   246,   246,   246,
     246,   246,    61,   246,    21,    21,    21,-32768,    37,    39,
      47,    48,    66,-32768,-32768,-32768,   246,-32768,-32768,   246,
     947,   107,-32768,-32768,-32768,    25,    51,    55,   919,    19,
      57,   975,   246,   998,   246,   998,   998,   -20,   -20,   -20,
     -20,   -20,   -20,   523,   847,   376,    34,    34,    34,   -19,
     -19,   -19,   -19,   -19,   322,   404,-32768,-32768,-32768,   246,
     246,   246,   246,-32768,   525,-32768,     5,    56,   246,-32768,
     360,   246,   998,   998,   246,    21,   523,   883,-32768,   571,
     441,   483,   617,-32768,    60,-32768,   663,   947,    67,   947,
     -20,-32768,   246,    21,-32768,   246,   246,-32768,   246,-32768,
     246,   -20,-32768,   709,   755,   801,   947,-32768,-32768,-32768,
     111,   117,-32768
};

static const short yypgoto[] =
{
  -32768,   106,-32768,-32768,-32768,   -11,-32768,   -41,-32768,-32768,
  -32768,   -15,   109,     8
};


#define	YYLAST		1054


static const short yytable[] =
{
      29,    31,    32,    33,    34,    35,    93,    94,    27,    63,
      64,    65,    66,    37,    67,    68,    69,    70,    71,    71,
      72,    72,   -89,    86,    41,    79,    80,    40,    81,    87,
      90,    38,     6,   126,   127,   128,    73,    73,    82,    83,
      98,   101,   103,   105,   106,   107,   108,   109,   110,   111,
     112,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    73,   125,    28,    28,    42,    28,    43,    67,    68,
      69,    70,    71,    89,    72,   134,    91,   -89,   136,   139,
     -89,   140,   -85,   -89,    82,    83,   -89,    84,   124,    85,
      73,   142,   129,   143,   130,    44,    45,    46,    47,    48,
      41,    88,   131,   132,   161,    49,    50,   133,   -84,    51,
     137,   181,   -86,   147,   141,   168,   155,   182,   149,   150,
     151,   152,   172,    36,   170,   158,     0,   156,    39,   157,
     159,     0,     0,   160,     1,     2,     3,     4,    95,     6,
       7,    96,     0,    97,     0,    10,     0,     0,    74,    75,
      76,   171,     0,    77,   173,   174,     0,   175,    78,   176,
       0,    11,    12,    13,     0,    14,     0,     0,     0,     0,
      15,    16,     1,     2,     3,     4,     5,     6,     7,     8,
       0,     9,     0,    10,     0,     0,    17,     0,   102,    18,
       0,   -22,     0,   -22,    19,     0,     0,     0,     0,    11,
      12,    13,     0,    14,     0,     0,     0,     0,    15,    16,
       0,     1,     2,     3,     4,     5,     6,     7,     8,     0,
       9,     0,    10,     0,    17,     0,     0,    18,   104,     0,
       0,     0,    19,     0,     0,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,     0,     0,    15,    16,     1,
       2,     3,     4,     5,     6,     7,     8,     0,     9,     0,
      10,     0,     0,    17,     0,     0,    18,     0,     0,     0,
       0,    19,     0,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,     0,     0,    15,    16,     1,     2,     3,
       4,   113,     6,     7,     8,     0,     9,     0,    10,     0,
       0,    17,     0,     0,    18,     0,     0,     0,     0,    19,
       0,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,     0,     0,    15,    16,     1,     2,     3,     4,   146,
       6,     7,     8,     0,     9,     0,    10,     0,     0,    17,
       0,     0,    18,     0,     0,     0,     0,    19,     0,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,     0,
       0,    15,    16,     1,     2,     3,     4,    95,     6,     7,
      96,     0,    97,     0,    10,     0,     0,    17,     0,     0,
      18,     0,     0,     0,     0,    19,     0,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,     0,     0,    15,
      16,     0,     0,     0,     0,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    17,    72,     0,    18,    52,
      53,    54,    55,    19,     0,    56,    57,    58,    59,    60,
      61,    62,    73,    63,    64,    65,    66,     0,    67,    68,
      69,    70,    71,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    53,    54,    55,
      73,     0,    56,    57,    58,    59,    60,    61,    62,   148,
      63,    64,    65,    66,     0,    67,    68,    69,    70,    71,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    73,    52,    53,
      54,    55,     0,   165,    56,    57,    58,    59,    60,    61,
      62,     0,    63,    64,    65,    66,     0,    67,    68,    69,
      70,    71,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    73,
      52,    53,    54,    55,     0,   166,    56,    57,    58,    59,
      60,    61,    62,     0,    63,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    28,    72,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,     0,     0,     0,   -71,     0,   -89,     0,
       0,    73,     0,     0,   -89,   153,    52,    53,    54,    55,
       0,     0,    56,    57,    58,    59,    60,    61,    62,     0,
      63,    64,    65,    66,     0,    67,    68,    69,    70,    71,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,     0,
       0,   164,    52,    53,    54,    55,     0,     0,    56,    57,
      58,    59,    60,    61,    62,     0,    63,    64,    65,    66,
       0,    67,    68,    69,    70,    71,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,     0,     0,   167,    52,    53,
      54,    55,     0,     0,    56,    57,    58,    59,    60,    61,
      62,     0,    63,    64,    65,    66,     0,    67,    68,    69,
      70,    71,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    73,
       0,     0,     0,   169,    52,    53,    54,    55,     0,     0,
      56,    57,    58,    59,    60,    61,    62,     0,    63,    64,
      65,    66,     0,    67,    68,    69,    70,    71,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    73,     0,     0,     0,   177,
      52,    53,    54,    55,     0,     0,    56,    57,    58,    59,
      60,    61,    62,     0,    63,    64,    65,    66,     0,    67,
      68,    69,    70,    71,     0,    72,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,     0,     0,   178,    52,    53,    54,    55,
       0,     0,    56,    57,    58,    59,    60,    61,    62,     0,
      63,    64,    65,    66,     0,    67,    68,    69,    70,    71,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,     0,
       0,   179,    52,    53,    54,    55,     0,     0,    56,    57,
      58,    59,    60,    61,    62,   144,    63,    64,    65,    66,
       0,    67,    68,    69,    70,    71,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,    52,    53,
      54,    55,     0,    73,    56,    57,    58,    59,    60,    61,
      62,   162,    63,    64,    65,    66,     0,    67,    68,    69,
      70,    71,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   163,    52,    53,    54,    55,     0,    73,
      56,    57,    58,    59,    60,    61,    62,   138,    63,    64,
      65,    66,     0,    67,    68,    69,    70,    71,     0,    72,
       0,     0,    52,    53,    54,    55,     0,     0,    56,    57,
      58,    59,    60,    61,    62,    73,    63,    64,    65,    66,
       0,    67,    68,    69,    70,    71,     0,    72,     0,     0,
  -32768,    53,    54,    55,     0,     0,    56,    57,    58,    59,
      60,    61,    62,    73,    63,    64,    65,    66,     0,    67,
      68,    69,    70,    71,     0,    72,     0,     0,     0,    56,
      57,    58,    59,    60,    61,    62,     0,    63,    64,    65,
      66,    73,    67,    68,    69,    70,    71,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    73
};

static const short yycheck[] =
{
      11,    12,    13,    14,    15,    16,    47,    48,     0,    29,
      30,    31,    32,     7,    34,    35,    36,    37,    38,    38,
      40,    40,     0,    55,    59,    46,    47,    19,    49,    61,
      41,     7,     8,    74,    75,    76,    56,    56,     7,     8,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    56,    73,    41,    41,    11,    41,    13,    34,    35,
      36,    37,    38,    64,    40,    86,    55,    55,    89,    60,
      55,    62,    57,    61,     7,     8,    61,    10,    27,    12,
      56,   102,    55,   104,    55,    41,    42,    43,    44,    45,
      59,    60,    55,    55,   145,    51,    52,    41,    57,    55,
       3,     0,    57,   124,    57,    55,    60,     0,   129,   130,
     131,   132,   163,    17,    57,   140,    -1,   138,    19,   140,
     141,    -1,    -1,   144,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    -1,    -1,    43,    44,
      45,   162,    -1,    48,   165,   166,    -1,   168,    53,   170,
      -1,    30,    31,    32,    -1,    34,    -1,    -1,    -1,    -1,
      39,    40,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    -1,    -1,    55,    -1,    19,    58,
      -1,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    -1,    55,    -1,    -1,    58,    20,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    -1,    -1,    55,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    30,    31,    32,    -1,
      34,    -1,    -1,    -1,    -1,    39,    40,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    -1,
      -1,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,    55,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    30,    31,    32,    -1,    34,    -1,    -1,    -1,
      -1,    39,    40,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    -1,    -1,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      30,    31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    55,    40,    -1,    58,    15,
      16,    17,    18,    63,    -1,    21,    22,    23,    24,    25,
      26,    27,    56,    29,    30,    31,    32,    -1,    34,    35,
      36,    37,    38,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    17,    18,
      56,    -1,    21,    22,    23,    24,    25,    26,    27,    65,
      29,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    15,    16,
      17,    18,    -1,    62,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      15,    16,    17,    18,    -1,    62,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    41,    40,    43,    44,    45,    46,
      47,    48,    49,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    56,    -1,    -1,    61,    60,    15,    16,    17,    18,
      -1,    -1,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    60,    15,    16,    17,    18,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    60,    15,    16,
      17,    18,    -1,    -1,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    60,    15,    16,    17,    18,    -1,    -1,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      31,    32,    -1,    34,    35,    36,    37,    38,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    60,
      15,    16,    17,    18,    -1,    -1,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    60,    15,    16,    17,    18,
      -1,    -1,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    60,    15,    16,    17,    18,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    15,    16,
      17,    18,    -1,    56,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    15,    16,    17,    18,    -1,    56,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    34,    35,    36,    37,    38,    -1,    40,
      -1,    -1,    15,    16,    17,    18,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    56,    29,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,    -1,    40,    -1,    -1,
      15,    16,    17,    18,    -1,    -1,    21,    22,    23,    24,
      25,    26,    27,    56,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    -1,    40,    -1,    -1,    -1,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    31,
      32,    56,    34,    35,    36,    37,    38,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "e:\\mingw32\\bin\\bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or xmalloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC xmalloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 315 "e:\\mingw32\\bin\\bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to xreallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to xreallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 2:
#line 218 "ada-exp.y"
{ write_exp_elt_opcode (OP_TYPE);
			  write_exp_elt_type (yyvsp[0].tval);
 			  write_exp_elt_opcode (OP_TYPE); }
    break;
case 4:
#line 226 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_COMMA); }
    break;
case 5:
#line 231 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_IND); }
    break;
case 6:
#line 235 "ada-exp.y"
{ write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string (yyvsp[0].ssym.stoken);
			  write_exp_elt_opcode (STRUCTOP_STRUCT);
			  }
    break;
case 7:
#line 242 "ada-exp.y"
{
			  write_exp_elt_opcode (OP_FUNCALL);
			  write_exp_elt_longcst (yyvsp[-1].lval);
			  write_exp_elt_opcode (OP_FUNCALL);
		        }
    break;
case 8:
#line 250 "ada-exp.y"
{
			  write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type (yyvsp[-3].tval);
			  write_exp_elt_opcode (UNOP_CAST);
			}
    break;
case 9:
#line 257 "ada-exp.y"
{ type_qualifier = yyvsp[-2].tval; }
    break;
case 10:
#line 258 "ada-exp.y"
{
			  write_exp_elt_opcode (UNOP_QUAL);
			  write_exp_elt_type (yyvsp[-6].tval);
			  write_exp_elt_opcode (UNOP_QUAL);
			  type_qualifier = yyvsp[-4].tval;
			}
    break;
case 11:
#line 266 "ada-exp.y"
{ yyval.tval = type_qualifier; }
    break;
case 12:
#line 271 "ada-exp.y"
{ write_exp_elt_opcode (TERNOP_SLICE); }
    break;
case 13:
#line 274 "ada-exp.y"
{ }
    break;
case 15:
#line 281 "ada-exp.y"
{ write_dollar_variable (yyvsp[0].sval); }
    break;
case 17:
#line 288 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_ASSIGN); }
    break;
case 18:
#line 292 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_NEG); }
    break;
case 19:
#line 296 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_PLUS); }
    break;
case 20:
#line 300 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_LOGICAL_NOT); }
    break;
case 21:
#line 304 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_ABS); }
    break;
case 22:
#line 307 "ada-exp.y"
{ yyval.lval = 0; }
    break;
case 23:
#line 311 "ada-exp.y"
{ yyval.lval = 1; }
    break;
case 24:
#line 313 "ada-exp.y"
{ yyval.lval = 1; }
    break;
case 25:
#line 315 "ada-exp.y"
{ yyval.lval = yyvsp[-2].lval + 1; }
    break;
case 26:
#line 317 "ada-exp.y"
{ yyval.lval = yyvsp[-4].lval + 1; }
    break;
case 27:
#line 322 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_MEMVAL);
			  write_exp_elt_type (yyvsp[-2].tval);
			  write_exp_elt_opcode (UNOP_MEMVAL);
			}
    break;
case 28:
#line 331 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_EXP); }
    break;
case 29:
#line 335 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_MUL); }
    break;
case 30:
#line 339 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_DIV); }
    break;
case 31:
#line 343 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_REM); }
    break;
case 32:
#line 347 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_MOD); }
    break;
case 33:
#line 351 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_REPEAT); }
    break;
case 34:
#line 355 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_ADD); }
    break;
case 35:
#line 359 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_CONCAT); }
    break;
case 36:
#line 363 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_SUB); }
    break;
case 37:
#line 367 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_EQUAL); }
    break;
case 38:
#line 371 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_NOTEQUAL); }
    break;
case 39:
#line 375 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_LEQ); }
    break;
case 40:
#line 379 "ada-exp.y"
{ write_exp_elt_opcode (TERNOP_IN_RANGE); }
    break;
case 41:
#line 381 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_IN_BOUNDS);
			  write_exp_elt_longcst ((LONGEST) yyvsp[0].lval);
			  write_exp_elt_opcode (BINOP_IN_BOUNDS);
			}
    break;
case 42:
#line 386 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_IN_RANGE);
		          write_exp_elt_type (yyvsp[0].tval);
		          write_exp_elt_opcode (UNOP_IN_RANGE);
			}
    break;
case 43:
#line 391 "ada-exp.y"
{ write_exp_elt_opcode (TERNOP_IN_RANGE);
		          write_exp_elt_opcode (UNOP_LOGICAL_NOT);
			}
    break;
case 44:
#line 395 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_IN_BOUNDS);
			  write_exp_elt_longcst ((LONGEST) yyvsp[0].lval);
			  write_exp_elt_opcode (BINOP_IN_BOUNDS);
		          write_exp_elt_opcode (UNOP_LOGICAL_NOT);
			}
    break;
case 45:
#line 401 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_IN_RANGE);
		          write_exp_elt_type (yyvsp[0].tval);
		          write_exp_elt_opcode (UNOP_IN_RANGE);
		          write_exp_elt_opcode (UNOP_LOGICAL_NOT);
			}
    break;
case 46:
#line 409 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_GEQ); }
    break;
case 47:
#line 413 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_LESS); }
    break;
case 48:
#line 417 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_GTR); }
    break;
case 49:
#line 421 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_BITWISE_AND); }
    break;
case 50:
#line 425 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_LOGICAL_AND); }
    break;
case 51:
#line 429 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_BITWISE_IOR); }
    break;
case 52:
#line 433 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_LOGICAL_OR); }
    break;
case 53:
#line 437 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_BITWISE_XOR); }
    break;
case 54:
#line 441 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_ADDR); }
    break;
case 55:
#line 443 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type (type_system_address ());
			  write_exp_elt_opcode (UNOP_CAST);
			}
    break;
case 56:
#line 449 "ada-exp.y"
{ write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_FIRST); }
    break;
case 57:
#line 452 "ada-exp.y"
{ write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_LAST); }
    break;
case 58:
#line 455 "ada-exp.y"
{ write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_LENGTH); }
    break;
case 59:
#line 458 "ada-exp.y"
{ write_exp_elt_opcode (OP_ATR_SIZE); }
    break;
case 60:
#line 460 "ada-exp.y"
{ write_exp_elt_opcode (OP_ATR_TAG); }
    break;
case 61:
#line 462 "ada-exp.y"
{ write_exp_elt_opcode (OP_ATR_MIN); }
    break;
case 62:
#line 464 "ada-exp.y"
{ write_exp_elt_opcode (OP_ATR_MAX); }
    break;
case 63:
#line 466 "ada-exp.y"
{ write_exp_elt_opcode (OP_ATR_POS); }
    break;
case 64:
#line 468 "ada-exp.y"
{ write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_FIRST); }
    break;
case 65:
#line 471 "ada-exp.y"
{ write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_LAST); }
    break;
case 66:
#line 474 "ada-exp.y"
{ write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_LENGTH); }
    break;
case 67:
#line 477 "ada-exp.y"
{ write_exp_elt_opcode (OP_ATR_VAL); }
    break;
case 68:
#line 479 "ada-exp.y"
{ write_exp_elt_opcode (OP_ATR_MODULUS); }
    break;
case 69:
#line 483 "ada-exp.y"
{ yyval.lval = 1; }
    break;
case 70:
#line 485 "ada-exp.y"
{ yyval.lval = yyvsp[-1].typed_val.val; }
    break;
case 71:
#line 490 "ada-exp.y"
{ write_exp_elt_opcode (OP_TYPE);
			  write_exp_elt_type (yyvsp[0].tval);
			  write_exp_elt_opcode (OP_TYPE); }
    break;
case 73:
#line 498 "ada-exp.y"
{ write_exp_elt_opcode (OP_TYPE);
			  write_exp_elt_type (builtin_type_void);
			  write_exp_elt_opcode (OP_TYPE); }
    break;
case 74:
#line 505 "ada-exp.y"
{ write_int ((LONGEST) yyvsp[0].typed_val.val, yyvsp[0].typed_val.type); }
    break;
case 75:
#line 509 "ada-exp.y"
{ write_int (convert_char_literal (type_qualifier, yyvsp[0].typed_val.val),
			       (type_qualifier == NULL) 
			       ? yyvsp[0].typed_val.type : type_qualifier);
		  }
    break;
case 76:
#line 516 "ada-exp.y"
{ write_exp_elt_opcode (OP_DOUBLE);
			  write_exp_elt_type (yyvsp[0].typed_val_float.type);
			  write_exp_elt_dblcst (yyvsp[0].typed_val_float.dval);
			  write_exp_elt_opcode (OP_DOUBLE);
			}
    break;
case 77:
#line 524 "ada-exp.y"
{ write_int (0, type_int ()); }
    break;
case 78:
#line 528 "ada-exp.y"
{ 
			  write_exp_elt_opcode (OP_STRING);
			  write_exp_string (yyvsp[0].sval);
			  write_exp_elt_opcode (OP_STRING);
			}
    break;
case 79:
#line 536 "ada-exp.y"
{ error ("NEW not implemented."); }
    break;
case 80:
#line 539 "ada-exp.y"
{ write_var_from_name (NULL, yyvsp[0].ssym); }
    break;
case 81:
#line 541 "ada-exp.y"
{ write_var_from_name (yyvsp[-1].bval, yyvsp[0].ssym); }
    break;
case 82:
#line 543 "ada-exp.y"
{ write_object_renaming (NULL, yyvsp[0].ssym.sym, 
				             MAX_RENAMING_CHAIN_LENGTH); }
    break;
case 83:
#line 546 "ada-exp.y"
{ write_object_renaming (yyvsp[-1].bval, yyvsp[0].ssym.sym, 
					     MAX_RENAMING_CHAIN_LENGTH); }
    break;
case 84:
#line 550 "ada-exp.y"
{ }
    break;
case 85:
#line 551 "ada-exp.y"
{ }
    break;
case 86:
#line 552 "ada-exp.y"
{ }
    break;
case 87:
#line 556 "ada-exp.y"
{ yyval.bval = yyvsp[0].bval; }
    break;
case 88:
#line 558 "ada-exp.y"
{ yyval.bval = yyvsp[0].bval; }
    break;
case 89:
#line 562 "ada-exp.y"
{ yyval.tval = yyvsp[0].tval; }
    break;
case 90:
#line 563 "ada-exp.y"
{ yyval.tval = yyvsp[0].tval; }
    break;
case 91:
#line 565 "ada-exp.y"
{ yyval.tval = lookup_pointer_type (yyvsp[-1].tval); }
    break;
case 92:
#line 567 "ada-exp.y"
{ yyval.tval = lookup_pointer_type (yyvsp[-1].tval); }
    break;
case 93:
#line 574 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_IND); }
    break;
case 94:
#line 576 "ada-exp.y"
{ write_exp_elt_opcode (UNOP_ADDR); }
    break;
case 95:
#line 578 "ada-exp.y"
{ write_exp_elt_opcode (BINOP_SUBSCRIPT); }
    break;
}

#line 705 "e:\\mingw32\\bin\\bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 581 "ada-exp.y"


/* yylex defined in ada-lex.c: Reads one token, getting characters */
/* through lexptr.  */

/* Remap normal flex interface names (yylex) as well as gratuitiously */
/* global symbol names, so we can have multiple flex-generated parsers */
/* in gdb.  */

/* (See note above on previous definitions for YACC.) */

#define yy_create_buffer ada_yy_create_buffer
#define yy_delete_buffer ada_yy_delete_buffer
#define yy_init_buffer ada_yy_init_buffer
#define yy_load_buffer_state ada_yy_load_buffer_state
#define yy_switch_to_buffer ada_yy_switch_to_buffer
#define yyrestart ada_yyrestart
#define yytext ada_yytext
#define yywrap ada_yywrap

static struct obstack temp_parse_space;

/* The following kludge was found necessary to prevent conflicts between */
/* defs.h and non-standard stdlib.h files.  */
#define qsort __qsort__dummy
#include "ada-lex.c"

int
ada_parse (void)
{
  lexer_init (yyin);		/* (Re-)initialize lexer.  */
  left_block_context = NULL;
  type_qualifier = NULL;
  obstack_free (&temp_parse_space, NULL);
  obstack_init (&temp_parse_space);

  return _ada_parse ();
}

void
yyerror (char *msg)
{
  error ("A %s in expression, near `%s'.", (msg ? msg : "error"), lexptr);
}

/* The operator name corresponding to operator symbol STRING (adds
   quotes and maps to lower-case).  Destroys the previous contents of
   the array pointed to by STRING.ptr.  Error if STRING does not match
   a valid Ada operator.  Assumes that STRING.ptr points to a
   null-terminated string and that, if STRING is a valid operator
   symbol, the array pointed to by STRING.ptr contains at least
   STRING.length+3 characters.  */

static struct stoken
string_to_operator (struct stoken string)
{
  int i;

  for (i = 0; ada_opname_table[i].encoded != NULL; i += 1)
    {
      if (string.length == strlen (ada_opname_table[i].decoded)-2
	  && strncasecmp (string.ptr, ada_opname_table[i].decoded+1,
			  string.length) == 0)
	{
	  strncpy (string.ptr, ada_opname_table[i].decoded,
		   string.length+2);
	  string.length += 2;
	  return string;
	}
    }
  error ("Invalid operator symbol `%s'", string.ptr);
}

/* Emit expression to access an instance of SYM, in block BLOCK (if
 * non-NULL), and with :: qualification ORIG_LEFT_CONTEXT.  */
static void
write_var_from_sym (struct block *orig_left_context,
		    struct block *block,
		    struct symbol *sym)
{
  if (orig_left_context == NULL && symbol_read_needs_frame (sym))
    {
      if (innermost_block == 0
	  || contained_in (block, innermost_block))
	innermost_block = block;
    }

  write_exp_elt_opcode (OP_VAR_VALUE);
  write_exp_elt_block (block);
  write_exp_elt_sym (sym);
  write_exp_elt_opcode (OP_VAR_VALUE);
}

/* Emit expression to access an instance of NAME in :: context
 * ORIG_LEFT_CONTEXT.  If no unique symbol for NAME has been found,
 * output a dummy symbol (good to the next call of ada_parse) for NAME
 * in the UNDEF_DOMAIN, for later resolution by ada_resolve.  */
static void
write_var_from_name (struct block *orig_left_context,
		     struct name_info name)
{
  if (name.msym != NULL)
    {
      write_exp_msymbol (name.msym,
			 lookup_function_type (type_int ()),
			 type_int ());
    }
  else if (name.sym == NULL)
    {
      /* Multiple matches: record name and starting block for later
         resolution by ada_resolve.  */
      char *encoded_name = ada_encode (name.stoken.ptr);
      struct symbol *sym =
	obstack_alloc (&temp_parse_space, sizeof (struct symbol));
      memset (sym, 0, sizeof (struct symbol));
      SYMBOL_DOMAIN (sym) = UNDEF_DOMAIN;
      SYMBOL_LINKAGE_NAME (sym)
	= obsavestring (encoded_name, strlen (encoded_name), &temp_parse_space);
      SYMBOL_LANGUAGE (sym) = language_ada;

      write_exp_elt_opcode (OP_VAR_VALUE);
      write_exp_elt_block (name.block);
      write_exp_elt_sym (sym);
      write_exp_elt_opcode (OP_VAR_VALUE);
    }
  else
    write_var_from_sym (orig_left_context, name.block, name.sym);
}

/* Write integer constant ARG of type TYPE.  */

static void
write_int (LONGEST arg, struct type *type)
{
  write_exp_elt_opcode (OP_LONG);
  write_exp_elt_type (type);
  write_exp_elt_longcst (arg);
  write_exp_elt_opcode (OP_LONG);
}

/* Emit expression corresponding to the renamed object designated by
 * the type RENAMING, which must be the referent of an object renaming
 * type, in the context of ORIG_LEFT_CONTEXT.  MAX_DEPTH is the maximum
 * number of cascaded renamings to allow.  */
static void
write_object_renaming (struct block *orig_left_context, 
		       struct symbol *renaming, int max_depth)
{
  const char *qualification = SYMBOL_LINKAGE_NAME (renaming);
  const char *simple_tail;
  const char *expr = TYPE_FIELD_NAME (SYMBOL_TYPE (renaming), 0);
  const char *suffix;
  char *name;
  struct symbol *sym;
  enum { SIMPLE_INDEX, LOWER_BOUND, UPPER_BOUND } slice_state;

  if (max_depth <= 0)
    error ("Could not find renamed symbol");

  /* if orig_left_context is null, then use the currently selected
     block; otherwise we might fail our symbol lookup below.  */
  if (orig_left_context == NULL)
    orig_left_context = get_selected_block (NULL);

  for (simple_tail = qualification + strlen (qualification);
       simple_tail != qualification; simple_tail -= 1)
    {
      if (*simple_tail == '.')
	{
	  simple_tail += 1;
	  break;
	}
      else if (strncmp (simple_tail, "__", 2) == 0)
	{
	  simple_tail += 2;
	  break;
	}
    }

  suffix = strstr (expr, "___XE");
  if (suffix == NULL)
    goto BadEncoding;

  name = (char *) obstack_alloc (&temp_parse_space, suffix - expr + 1);
  strncpy (name, expr, suffix-expr);
  name[suffix-expr] = '\000';
  sym = lookup_symbol (name, orig_left_context, VAR_DOMAIN, 0, NULL);
  if (sym == NULL)
    error ("Could not find renamed variable: %s", ada_decode (name));
  if (ada_is_object_renaming (sym))
    write_object_renaming (orig_left_context, sym, max_depth-1);
  else
    write_var_from_sym (orig_left_context, block_found, sym);

  suffix += 5;
  slice_state = SIMPLE_INDEX;
  while (*suffix == 'X')
    {
      suffix += 1;

      switch (*suffix) {
      case 'A':
        suffix += 1;
        write_exp_elt_opcode (UNOP_IND);
        break;
      case 'L':
	slice_state = LOWER_BOUND;
      case 'S':
	suffix += 1;
	if (isdigit (*suffix))
	  {
	    char *next;
	    long val = strtol (suffix, &next, 10);
	    if (next == suffix)
	      goto BadEncoding;
	    suffix = next;
	    write_exp_elt_opcode (OP_LONG);
	    write_exp_elt_type (type_int ());
	    write_exp_elt_longcst ((LONGEST) val);
	    write_exp_elt_opcode (OP_LONG);
	  }
	else
	  {
	    const char *end;
	    char *index_name;
	    int index_len;
	    struct symbol *index_sym;

	    end = strchr (suffix, 'X');
	    if (end == NULL)
	      end = suffix + strlen (suffix);

	    index_len = simple_tail - qualification + 2 + (suffix - end) + 1;
	    index_name
	      = (char *) obstack_alloc (&temp_parse_space, index_len);
	    memset (index_name, '\000', index_len);
	    strncpy (index_name, qualification, simple_tail - qualification);
	    index_name[simple_tail - qualification] = '\000';
	    strncat (index_name, suffix, suffix-end);
	    suffix = end;

	    index_sym =
	      lookup_symbol (index_name, NULL, VAR_DOMAIN, 0, NULL);
	    if (index_sym == NULL)
	      error ("Could not find %s", index_name);
	    write_var_from_sym (NULL, block_found, sym);
	  }
	if (slice_state == SIMPLE_INDEX)
	  {
	    write_exp_elt_opcode (OP_FUNCALL);
	    write_exp_elt_longcst ((LONGEST) 1);
	    write_exp_elt_opcode (OP_FUNCALL);
	  }
	else if (slice_state == LOWER_BOUND)
	  slice_state = UPPER_BOUND;
	else if (slice_state == UPPER_BOUND)
	  {
	    write_exp_elt_opcode (TERNOP_SLICE);
	    slice_state = SIMPLE_INDEX;
	  }
	break;

      case 'R':
	{
	  struct stoken field_name;
	  const char *end;
	  suffix += 1;

	  if (slice_state != SIMPLE_INDEX)
	    goto BadEncoding;
	  end = strchr (suffix, 'X');
	  if (end == NULL)
	    end = suffix + strlen (suffix);
	  field_name.length = end - suffix;
	  field_name.ptr = xmalloc (end - suffix + 1);
	  strncpy (field_name.ptr, suffix, end - suffix);
	  field_name.ptr[end - suffix] = '\000';
	  suffix = end;
	  write_exp_elt_opcode (STRUCTOP_STRUCT);
	  write_exp_string (field_name);
	  write_exp_elt_opcode (STRUCTOP_STRUCT);
	  break;
	}

      default:
	goto BadEncoding;
      }
    }
  if (slice_state == SIMPLE_INDEX)
    return;

 BadEncoding:
  error ("Internal error in encoding of renaming declaration: %s",
	 SYMBOL_LINKAGE_NAME (renaming));
}

/* Convert the character literal whose ASCII value would be VAL to the
   appropriate value of type TYPE, if there is a translation.
   Otherwise return VAL.  Hence, in an enumeration type ('A', 'B'),
   the literal 'A' (VAL == 65), returns 0.  */
static LONGEST
convert_char_literal (struct type *type, LONGEST val)
{
  char name[7];
  int f;

  if (type == NULL || TYPE_CODE (type) != TYPE_CODE_ENUM)
    return val;
  sprintf (name, "QU%02x", (int) val);
  for (f = 0; f < TYPE_NFIELDS (type); f += 1)
    {
      if (strcmp (name, TYPE_FIELD_NAME (type, f)) == 0)
	return TYPE_FIELD_BITPOS (type, f);
    }
  return val;
}

static struct type *
type_int (void)
{
  return builtin_type (current_gdbarch)->builtin_int;
}

static struct type *
type_long (void)
{
  return builtin_type (current_gdbarch)->builtin_long;
}

static struct type *
type_long_long (void)
{
  return builtin_type (current_gdbarch)->builtin_long_long;
}

static struct type *
type_float (void)
{
  return builtin_type (current_gdbarch)->builtin_float;
}

static struct type *
type_double (void)
{
  return builtin_type (current_gdbarch)->builtin_double;
}

static struct type *
type_long_double (void)
{
  return builtin_type (current_gdbarch)->builtin_long_double;
}

static struct type *
type_char (void)
{
  return language_string_char_type (current_language, current_gdbarch);
}

static struct type *
type_system_address (void)
{
  struct type *type 
    = language_lookup_primitive_type_by_name (current_language,
					      current_gdbarch, 
					      "system__address");
  return  type != NULL ? type : lookup_pointer_type (builtin_type_void);
}

void
_initialize_ada_exp (void)
{
  obstack_init (&temp_parse_space);
}

/* FIXME: hilfingr/2004-10-05: Hack to remove warning.  The function
   string_to_operator is supposed to be used for cases where one
   calls an operator function with prefix notation, as in 
   "+" (a, b), but at some point, this code seems to have gone
   missing. */

struct stoken (*dummy_string_to_ada_operator) (struct stoken) 
     = string_to_operator;

