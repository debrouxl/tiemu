/* A Bison parser, made by GNU Bison 1.875d.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     NULL_PTR = 259,
     CHARLIT = 260,
     FLOAT = 261,
     TYPENAME = 262,
     BLOCKNAME = 263,
     STRING = 264,
     NAME = 265,
     DOT_ID = 266,
     OBJECT_RENAMING = 267,
     DOT_ALL = 268,
     SPECIAL_VARIABLE = 269,
     ASSIGN = 270,
     ELSE = 271,
     THEN = 272,
     XOR = 273,
     OR = 274,
     _AND_ = 275,
     DOTDOT = 276,
     IN = 277,
     GEQ = 278,
     LEQ = 279,
     NOTEQUAL = 280,
     UNARY = 281,
     REM = 282,
     MOD = 283,
     NOT = 284,
     ABS = 285,
     STARSTAR = 286,
     TICK_LENGTH = 287,
     TICK_LAST = 288,
     TICK_FIRST = 289,
     TICK_ADDRESS = 290,
     TICK_ACCESS = 291,
     TICK_MODULUS = 292,
     TICK_MIN = 293,
     TICK_MAX = 294,
     TICK_VAL = 295,
     TICK_TAG = 296,
     TICK_SIZE = 297,
     TICK_RANGE = 298,
     TICK_POS = 299,
     ARROW = 300,
     NEW = 301
   };
#endif
#define INT 258
#define NULL_PTR 259
#define CHARLIT 260
#define FLOAT 261
#define TYPENAME 262
#define BLOCKNAME 263
#define STRING 264
#define NAME 265
#define DOT_ID 266
#define OBJECT_RENAMING 267
#define DOT_ALL 268
#define SPECIAL_VARIABLE 269
#define ASSIGN 270
#define ELSE 271
#define THEN 272
#define XOR 273
#define OR 274
#define _AND_ 275
#define DOTDOT 276
#define IN 277
#define GEQ 278
#define LEQ 279
#define NOTEQUAL 280
#define UNARY 281
#define REM 282
#define MOD 283
#define NOT 284
#define ABS 285
#define STARSTAR 286
#define TICK_LENGTH 287
#define TICK_LAST 288
#define TICK_FIRST 289
#define TICK_ADDRESS 290
#define TICK_ACCESS 291
#define TICK_MODULUS 292
#define TICK_MIN 293
#define TICK_MAX 294
#define TICK_VAL 295
#define TICK_TAG 296
#define TICK_SIZE 297
#define TICK_RANGE 298
#define TICK_POS 299
#define ARROW 300
#define NEW 301




/* Copy the first part of user declarations.  */
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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 148 "ada-exp.y"
typedef union YYSTYPE {
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

  } YYSTYPE;
/* Line 191 of yacc.c.  */
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC xmalloc
# endif

/* The parser invokes alloca or xmalloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
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
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  42
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1041

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  66
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  15
/* YYNRULES -- Number of rules. */
#define YYNRULES  96
/* YYNRULES -- Number of states. */
#define YYNSTATES  182

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    32,    61,
      55,    60,    34,    30,    62,    31,    54,    35,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    59,
      22,    21,    23,     2,    29,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    24,    25,    26,    27,
      28,    33,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      57,    58
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    13,    16,    19,    24,
      29,    30,    38,    39,    46,    50,    52,    54,    56,    60,
      63,    66,    69,    72,    73,    75,    79,    83,    89,    94,
      98,   102,   106,   110,   114,   118,   122,   126,   130,   134,
     138,   142,   148,   154,   158,   165,   172,   177,   181,   185,
     189,   193,   198,   202,   207,   211,   214,   217,   221,   225,
     229,   232,   235,   243,   251,   257,   261,   265,   269,   275,
     278,   279,   283,   285,   287,   288,   290,   292,   294,   296,
     298,   301,   303,   306,   308,   311,   313,   315,   317,   319,
     322,   324,   327,   330,   334,   337,   340
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      67,     0,    -1,    68,    -1,    80,    -1,    72,    -1,    68,
      59,    72,    -1,    69,    13,    -1,    69,    11,    -1,    69,
      55,    73,    60,    -1,    80,    55,    72,    60,    -1,    -1,
      80,    61,    71,    70,    55,    72,    60,    -1,    -1,    69,
      55,    72,    24,    72,    60,    -1,    55,    68,    60,    -1,
      77,    -1,    14,    -1,    69,    -1,    72,    15,    72,    -1,
      31,    72,    -1,    30,    72,    -1,    38,    72,    -1,    39,
      72,    -1,    -1,    72,    -1,    78,    57,    72,    -1,    73,
      62,    72,    -1,    73,    62,    78,    57,    72,    -1,    63,
      80,    64,    72,    -1,    72,    40,    72,    -1,    72,    34,
      72,    -1,    72,    35,    72,    -1,    72,    36,    72,    -1,
      72,    37,    72,    -1,    72,    29,    72,    -1,    72,    30,
      72,    -1,    72,    32,    72,    -1,    72,    31,    72,    -1,
      72,    21,    72,    -1,    72,    28,    72,    -1,    72,    27,
      72,    -1,    72,    25,    72,    24,    72,    -1,    72,    25,
      72,    52,    74,    -1,    72,    25,     7,    -1,    72,    38,
      25,    72,    24,    72,    -1,    72,    38,    25,    72,    52,
      74,    -1,    72,    38,    25,     7,    -1,    72,    26,    72,
      -1,    72,    22,    72,    -1,    72,    23,    72,    -1,    72,
      20,    72,    -1,    72,    20,    17,    72,    -1,    72,    19,
      72,    -1,    72,    19,    16,    72,    -1,    72,    18,    72,
      -1,    69,    45,    -1,    69,    44,    -1,    69,    43,    74,
      -1,    69,    42,    74,    -1,    69,    41,    74,    -1,    69,
      51,    -1,    69,    50,    -1,    76,    47,    55,    72,    62,
      72,    60,    -1,    76,    48,    55,    72,    62,    72,    60,
      -1,    76,    53,    55,    72,    60,    -1,    75,    43,    74,
      -1,    75,    42,    74,    -1,    75,    41,    74,    -1,    75,
      49,    55,    72,    60,    -1,    75,    46,    -1,    -1,    55,
       3,    60,    -1,     7,    -1,    75,    -1,    -1,     3,    -1,
       5,    -1,     6,    -1,     4,    -1,     9,    -1,    58,     7,
      -1,    10,    -1,    79,    10,    -1,    12,    -1,    79,    12,
      -1,    10,    -1,     7,    -1,    12,    -1,     8,    -1,    79,
       8,    -1,     7,    -1,    79,     7,    -1,     7,    45,    -1,
      79,     7,    45,    -1,    34,    72,    -1,    32,    72,    -1,
      72,    56,    72,    65,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   217,   217,   218,   224,   225,   230,   234,   241,   249,
     257,   257,   266,   270,   274,   277,   280,   284,   287,   291,
     295,   299,   303,   307,   310,   312,   314,   316,   320,   330,
     334,   338,   342,   346,   350,   354,   358,   362,   366,   370,
     374,   378,   380,   385,   390,   394,   400,   408,   412,   416,
     420,   424,   428,   432,   436,   440,   442,   448,   451,   454,
     457,   459,   461,   463,   465,   467,   470,   473,   476,   478,
     482,   484,   489,   496,   498,   504,   508,   515,   523,   527,
     535,   539,   540,   542,   545,   550,   551,   552,   555,   557,
     562,   563,   564,   566,   573,   575,   577
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "NULL_PTR", "CHARLIT", "FLOAT",
  "TYPENAME", "BLOCKNAME", "STRING", "NAME", "DOT_ID", "OBJECT_RENAMING",
  "DOT_ALL", "SPECIAL_VARIABLE", "ASSIGN", "ELSE", "THEN", "XOR", "OR",
  "_AND_", "'='", "'<'", "'>'", "DOTDOT", "IN", "GEQ", "LEQ", "NOTEQUAL",
  "'@'", "'+'", "'-'", "'&'", "UNARY", "'*'", "'/'", "REM", "MOD", "NOT",
  "ABS", "STARSTAR", "TICK_LENGTH", "TICK_LAST", "TICK_FIRST",
  "TICK_ADDRESS", "TICK_ACCESS", "TICK_MODULUS", "TICK_MIN", "TICK_MAX",
  "TICK_VAL", "TICK_TAG", "TICK_SIZE", "TICK_RANGE", "TICK_POS", "'.'",
  "'('", "'['", "ARROW", "NEW", "';'", "')'", "'''", "','", "'{'", "'}'",
  "']'", "$accept", "start", "exp1", "simple_exp", "@1", "save_qualifier",
  "exp", "arglist", "tick_arglist", "type_prefix", "opt_type_prefix",
  "variable", "any_name", "block", "type", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,    61,    60,    62,   276,   277,   278,   279,   280,    64,
      43,    45,    38,   281,    42,    47,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,    46,    40,    91,   300,   301,    59,
      41,    39,    44,   123,   125,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    66,    67,    67,    68,    68,    69,    69,    69,    69,
      70,    69,    71,    69,    69,    69,    69,    72,    72,    72,
      72,    72,    72,    73,    73,    73,    73,    73,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      74,    74,    75,    76,    76,    72,    72,    72,    72,    72,
      72,    77,    77,    77,    77,    78,    78,    78,    79,    79,
      80,    80,    80,    80,    72,    72,    72
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     1,     3,     2,     2,     4,     4,
       0,     7,     0,     6,     3,     1,     1,     1,     3,     2,
       2,     2,     2,     0,     1,     3,     3,     5,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     5,     3,     6,     6,     4,     3,     3,     3,
       3,     4,     3,     4,     3,     2,     2,     3,     3,     3,
       2,     2,     7,     7,     5,     3,     3,     3,     5,     2,
       0,     3,     1,     1,     0,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     2,     1,     1,     1,     1,     2,
       1,     2,     2,     3,     2,     2,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
      74,    75,    78,    76,    77,    72,    88,    79,    81,    83,
      16,    74,    74,    74,    74,    74,    74,    74,     0,     0,
       0,     2,    17,     4,    73,     0,    15,     0,     3,    92,
      20,     0,    19,    95,    94,    21,    22,     0,    80,    90,
       0,     0,     1,    74,     7,     6,    70,    70,    70,    56,
      55,    61,    60,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,     0,    74,    74,    70,    70,    70,    69,
       0,     0,     0,     0,    91,    89,    82,    84,    74,    12,
      14,    74,     5,     0,    59,    58,    57,    72,    81,    83,
      24,     0,     0,    18,    54,    74,    52,    74,    50,    38,
      48,    49,    43,     0,    47,    40,    39,    34,    35,    37,
      36,    30,    31,    32,    33,    74,    29,     0,    67,    66,
      65,    74,    74,    74,    74,    93,     0,    10,    28,     0,
      74,     8,    74,    74,    53,    51,    74,    70,    46,     0,
      96,     0,     0,     0,     0,     9,     0,    71,     0,    26,
       0,    25,    41,    42,    74,    70,    68,    74,    74,    64,
      74,    13,    74,    44,    45,     0,     0,     0,    27,    62,
      63,    11
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    20,    21,    22,   156,   137,    23,   101,    94,    24,
      25,    26,   102,    27,    31
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -52
static const short int yypact[] =
{
     233,   -52,   -52,   -52,   -52,    17,   -52,   -52,   -52,   -52,
     -52,   233,   233,   233,   233,   233,   233,   233,    23,    26,
      41,    15,   137,   923,    56,   -22,   -52,   118,   -32,   -52,
      33,   -32,    33,    19,    19,    33,    33,   -21,   -52,    31,
      58,    29,   -52,   233,   -52,   -52,    24,    24,    24,   -52,
     -52,   -52,   -52,    78,   233,   233,   159,   196,   233,   233,
     233,   270,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,    70,   233,   233,    24,    24,    24,   -52,
      45,    49,    60,    63,    66,   -52,   -52,   -52,   233,   -52,
     -52,   233,   923,   110,   -52,   -52,   -52,    46,    62,    77,
     891,   -51,    80,   955,   978,   233,   978,   233,   978,   -16,
     -16,   -16,   980,   813,   -16,   -16,   -16,   358,    33,    33,
      33,   -28,   -28,   -28,   -28,   307,   -28,   390,   -52,   -52,
     -52,   233,   233,   233,   233,   -52,   512,   -52,    19,    67,
     233,   -52,   344,   233,   978,   978,   233,    24,   980,   852,
     -52,   555,   422,   467,   598,   -52,    84,   -52,   641,   923,
      85,   923,   -16,   -52,   233,    24,   -52,   233,   233,   -52,
     233,   -52,   233,   -16,   -52,   684,   727,   770,   923,   -52,
     -52,   -52
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -52,   -52,   126,   -52,   -52,   -52,   -11,   -52,   -41,   -52,
     -52,   -52,     2,   127,     8
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -91
static const short int yytable[] =
{
      30,    32,    33,    34,    35,    36,    95,    96,    28,   141,
      73,   142,    74,    65,    66,    67,    68,   -90,    69,    70,
      71,    72,    73,    88,    74,    81,    82,    41,    75,    89,
      38,    83,    92,    39,     6,   128,   129,   130,    43,    90,
      75,    42,   100,   103,   104,   106,   108,   109,   110,   111,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    29,   126,   127,    84,    85,    69,    70,    71,
      72,    73,   -90,    74,    43,    75,    29,   136,   -90,    93,
     138,     1,     2,     3,     4,    97,     6,     7,    98,    75,
      99,    29,    10,    91,   144,   125,   145,    76,    77,    78,
     131,   -90,    79,   -86,   132,    80,   163,   -90,    11,    12,
      13,   135,    14,   139,   149,   133,    15,    16,   134,   -85,
     151,   152,   153,   154,   174,    84,    85,   157,    86,   158,
      87,   159,   161,    17,   -87,   162,    18,   143,   -23,   170,
     -23,    19,   172,    37,   160,     0,    40,     0,    44,     0,
      45,     0,     0,   173,     0,     0,   175,   176,     0,   177,
       0,   178,     1,     2,     3,     4,     5,     6,     7,     8,
       0,     9,     0,    10,     0,   105,     0,     0,    46,    47,
      48,    49,    50,     0,     0,     0,     0,    51,    52,    11,
      12,    13,    53,    14,     0,     0,     0,    15,    16,     1,
       2,     3,     4,     5,     6,     7,     8,     0,     9,     0,
      10,     0,     0,   107,    17,     0,     0,    18,     0,     0,
       0,     0,    19,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,     0,    15,    16,     1,     2,     3,     4,
       5,     6,     7,     8,     0,     9,     0,    10,     0,     0,
       0,    17,     0,     0,    18,     0,     0,     0,     0,    19,
       0,     0,     0,    11,    12,    13,     0,    14,     0,     0,
       0,    15,    16,     1,     2,     3,     4,   112,     6,     7,
       8,     0,     9,     0,    10,     0,     0,     0,    17,     0,
       0,    18,     0,     0,     0,     0,    19,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,     0,    15,    16,
       1,     2,     3,     4,   148,     6,     7,     8,     0,     9,
       0,    10,     0,     0,     0,    17,     0,     0,    18,     0,
       0,     0,     0,    19,     0,     0,     0,    11,    12,    13,
       0,    14,     0,     0,     0,    15,    16,     1,     2,     3,
       4,    97,     6,     7,    98,     0,    99,     0,    10,     0,
       0,     0,    17,     0,     0,    18,     0,     0,     0,     0,
      19,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,     0,    15,    16,     0,     0,     0,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,     0,    74,    17,
       0,     0,    18,     0,     0,    54,     0,    19,    55,    56,
      57,    58,    59,    60,    75,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,    71,    72,    73,     0,
      74,     0,     0,     0,     0,     0,     0,    54,     0,     0,
      55,    56,    57,    58,    59,    60,    75,    61,    62,    63,
      64,    65,    66,    67,    68,   150,    69,    70,    71,    72,
      73,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,     0,
       0,     0,    54,     0,   167,    55,    56,    57,    58,    59,
      60,     0,    61,    62,    63,    64,    65,    66,    67,    68,
       0,    69,    70,    71,    72,    73,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,     0,     0,     0,    54,     0,   168,
      55,    56,    57,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,    71,    72,
      73,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,     0,
      54,     0,   155,    55,    56,    57,    58,    59,    60,     0,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
      70,    71,    72,    73,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,     0,    54,     0,   166,    55,    56,    57,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,    71,    72,    73,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,     0,    54,     0,   169,    55,
      56,    57,    58,    59,    60,     0,    61,    62,    63,    64,
      65,    66,    67,    68,     0,    69,    70,    71,    72,    73,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,     0,    54,
       0,   171,    55,    56,    57,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
      71,    72,    73,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,     0,    54,     0,   179,    55,    56,    57,    58,    59,
      60,     0,    61,    62,    63,    64,    65,    66,    67,    68,
       0,    69,    70,    71,    72,    73,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,     0,    54,     0,   180,    55,    56,
      57,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,    71,    72,    73,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,     0,    54,     0,
     181,    55,    56,    57,    58,    59,    60,   146,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,    71,
      72,    73,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   147,     0,    54,     0,    75,
      55,    56,    57,    58,    59,    60,   164,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,    71,    72,
      73,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   165,     0,    54,     0,    75,    55,
      56,    57,    58,    59,    60,   140,    61,    62,    63,    64,
      65,    66,    67,    68,     0,    69,    70,    71,    72,    73,
       0,    74,     0,     0,     0,     0,     0,     0,    54,     0,
       0,    55,    56,    57,    58,    59,    60,    75,    61,    62,
      63,    64,    65,    66,    67,    68,     0,    69,    70,    71,
      72,    73,     0,    74,     0,     0,     0,     0,     0,     0,
     -91,     0,     0,    55,    56,    57,    58,    59,    60,    75,
      61,    62,    63,    64,    65,    66,    67,    68,     0,    69,
      70,    71,    72,    73,     0,    74,     0,     0,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,    75,    69,    70,    71,    72,    73,     0,    74,     0,
       0,   -72,   -72,   -72,     0,    29,   -72,   -72,   -72,   -72,
       0,     0,     0,   -72,    75,   -90,     0,     0,     0,     0,
       0,   -90
};

static const short int yycheck[] =
{
      11,    12,    13,    14,    15,    16,    47,    48,     0,    60,
      38,    62,    40,    29,    30,    31,    32,     0,    34,    35,
      36,    37,    38,    55,    40,    47,    48,    19,    56,    61,
       7,    53,    43,     7,     8,    76,    77,    78,    59,    60,
      56,     0,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    45,    74,    75,     7,     8,    34,    35,    36,
      37,    38,    55,    40,    59,    56,    45,    88,    61,    55,
      91,     3,     4,     5,     6,     7,     8,     9,    10,    56,
      12,    45,    14,    64,   105,    25,   107,    41,    42,    43,
      55,    55,    46,    57,    55,    49,   147,    61,    30,    31,
      32,    45,    34,     3,   125,    55,    38,    39,    55,    57,
     131,   132,   133,   134,   165,     7,     8,    60,    10,   140,
      12,   142,   143,    55,    57,   146,    58,    57,    60,    55,
      62,    63,    57,    17,   142,    -1,    19,    -1,    11,    -1,
      13,    -1,    -1,   164,    -1,    -1,   167,   168,    -1,   170,
      -1,   172,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    -1,    16,    -1,    -1,    41,    42,
      43,    44,    45,    -1,    -1,    -1,    -1,    50,    51,    30,
      31,    32,    55,    34,    -1,    -1,    -1,    38,    39,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    -1,    -1,    17,    55,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    30,    31,    32,    -1,
      34,    -1,    -1,    -1,    38,    39,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,
      -1,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    30,    31,    32,    -1,    34,    -1,    -1,
      -1,    38,    39,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    -1,    -1,    -1,    55,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      30,    31,    32,    -1,    34,    -1,    -1,    -1,    38,    39,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    12,
      -1,    14,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    30,    31,    32,
      -1,    34,    -1,    -1,    -1,    38,    39,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    -1,
      -1,    -1,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    30,    31,
      32,    -1,    34,    35,    36,    37,    38,    -1,    40,    55,
      -1,    -1,    58,    -1,    -1,    15,    -1,    63,    18,    19,
      20,    21,    22,    23,    56,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    34,    35,    36,    37,    38,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    20,    21,    22,    23,    56,    25,    26,    27,
      28,    29,    30,    31,    32,    65,    34,    35,    36,    37,
      38,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    15,    -1,    62,    18,    19,    20,    21,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    15,    -1,    62,
      18,    19,    20,    21,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      15,    -1,    60,    18,    19,    20,    21,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    15,    -1,    60,    18,    19,    20,    21,
      22,    23,    -1,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    34,    35,    36,    37,    38,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    15,    -1,    60,    18,
      19,    20,    21,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    15,
      -1,    60,    18,    19,    20,    21,    22,    23,    -1,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    34,    35,
      36,    37,    38,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    15,    -1,    60,    18,    19,    20,    21,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    15,    -1,    60,    18,    19,
      20,    21,    22,    23,    -1,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    34,    35,    36,    37,    38,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    15,    -1,
      60,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    15,    -1,    56,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    15,    -1,    56,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    34,    35,    36,    37,    38,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    56,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    20,    21,    22,    23,    56,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    -1,    40,    -1,    -1,    -1,    21,
      22,    23,    -1,    25,    26,    27,    28,    29,    30,    31,
      32,    56,    34,    35,    36,    37,    38,    -1,    40,    -1,
      -1,    41,    42,    43,    -1,    45,    46,    47,    48,    49,
      -1,    -1,    -1,    53,    56,    55,    -1,    -1,    -1,    -1,
      -1,    61
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    12,
      14,    30,    31,    32,    34,    38,    39,    55,    58,    63,
      67,    68,    69,    72,    75,    76,    77,    79,    80,    45,
      72,    80,    72,    72,    72,    72,    72,    68,     7,     7,
      79,    80,     0,    59,    11,    13,    41,    42,    43,    44,
      45,    50,    51,    55,    15,    18,    19,    20,    21,    22,
      23,    25,    26,    27,    28,    29,    30,    31,    32,    34,
      35,    36,    37,    38,    40,    56,    41,    42,    43,    46,
      49,    47,    48,    53,     7,     8,    10,    12,    55,    61,
      60,    64,    72,    55,    74,    74,    74,     7,    10,    12,
      72,    73,    78,    72,    72,    16,    72,    17,    72,    72,
      72,    72,     7,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    25,    72,    72,    74,    74,
      74,    55,    55,    55,    55,    45,    72,    71,    72,     3,
      24,    60,    62,    57,    72,    72,    24,    52,     7,    72,
      65,    72,    72,    72,    72,    60,    70,    60,    72,    72,
      78,    72,    72,    74,    24,    52,    60,    62,    62,    60,
      55,    60,    57,    72,    74,    72,    72,    72,    72,    60,
      60,    60
};

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
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


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
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

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

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
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

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

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

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to xreallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
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

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to xreallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


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

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 218 "ada-exp.y"
    { write_exp_elt_opcode (OP_TYPE);
			  write_exp_elt_type (yyvsp[0].tval);
 			  write_exp_elt_opcode (OP_TYPE); }
    break;

  case 5:
#line 226 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_COMMA); }
    break;

  case 6:
#line 231 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_IND); }
    break;

  case 7:
#line 235 "ada-exp.y"
    { write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string (yyvsp[0].ssym.stoken);
			  write_exp_elt_opcode (STRUCTOP_STRUCT);
			  }
    break;

  case 8:
#line 242 "ada-exp.y"
    {
			  write_exp_elt_opcode (OP_FUNCALL);
			  write_exp_elt_longcst (yyvsp[-1].lval);
			  write_exp_elt_opcode (OP_FUNCALL);
		        }
    break;

  case 9:
#line 250 "ada-exp.y"
    {
			  write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type (yyvsp[-3].tval);
			  write_exp_elt_opcode (UNOP_CAST);
			}
    break;

  case 10:
#line 257 "ada-exp.y"
    { type_qualifier = yyvsp[-2].tval; }
    break;

  case 11:
#line 258 "ada-exp.y"
    {
			  write_exp_elt_opcode (UNOP_QUAL);
			  write_exp_elt_type (yyvsp[-6].tval);
			  write_exp_elt_opcode (UNOP_QUAL);
			  type_qualifier = yyvsp[-4].tval;
			}
    break;

  case 12:
#line 266 "ada-exp.y"
    { yyval.tval = type_qualifier; }
    break;

  case 13:
#line 271 "ada-exp.y"
    { write_exp_elt_opcode (TERNOP_SLICE); }
    break;

  case 14:
#line 274 "ada-exp.y"
    { }
    break;

  case 16:
#line 281 "ada-exp.y"
    { write_dollar_variable (yyvsp[0].sval); }
    break;

  case 18:
#line 288 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_ASSIGN); }
    break;

  case 19:
#line 292 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_NEG); }
    break;

  case 20:
#line 296 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_PLUS); }
    break;

  case 21:
#line 300 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_LOGICAL_NOT); }
    break;

  case 22:
#line 304 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_ABS); }
    break;

  case 23:
#line 307 "ada-exp.y"
    { yyval.lval = 0; }
    break;

  case 24:
#line 311 "ada-exp.y"
    { yyval.lval = 1; }
    break;

  case 25:
#line 313 "ada-exp.y"
    { yyval.lval = 1; }
    break;

  case 26:
#line 315 "ada-exp.y"
    { yyval.lval = yyvsp[-2].lval + 1; }
    break;

  case 27:
#line 317 "ada-exp.y"
    { yyval.lval = yyvsp[-4].lval + 1; }
    break;

  case 28:
#line 322 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_MEMVAL);
			  write_exp_elt_type (yyvsp[-2].tval);
			  write_exp_elt_opcode (UNOP_MEMVAL);
			}
    break;

  case 29:
#line 331 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_EXP); }
    break;

  case 30:
#line 335 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_MUL); }
    break;

  case 31:
#line 339 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_DIV); }
    break;

  case 32:
#line 343 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_REM); }
    break;

  case 33:
#line 347 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_MOD); }
    break;

  case 34:
#line 351 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_REPEAT); }
    break;

  case 35:
#line 355 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_ADD); }
    break;

  case 36:
#line 359 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_CONCAT); }
    break;

  case 37:
#line 363 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_SUB); }
    break;

  case 38:
#line 367 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_EQUAL); }
    break;

  case 39:
#line 371 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_NOTEQUAL); }
    break;

  case 40:
#line 375 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_LEQ); }
    break;

  case 41:
#line 379 "ada-exp.y"
    { write_exp_elt_opcode (TERNOP_IN_RANGE); }
    break;

  case 42:
#line 381 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_IN_BOUNDS);
			  write_exp_elt_longcst ((LONGEST) yyvsp[0].lval);
			  write_exp_elt_opcode (BINOP_IN_BOUNDS);
			}
    break;

  case 43:
#line 386 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_IN_RANGE);
		          write_exp_elt_type (yyvsp[0].tval);
		          write_exp_elt_opcode (UNOP_IN_RANGE);
			}
    break;

  case 44:
#line 391 "ada-exp.y"
    { write_exp_elt_opcode (TERNOP_IN_RANGE);
		          write_exp_elt_opcode (UNOP_LOGICAL_NOT);
			}
    break;

  case 45:
#line 395 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_IN_BOUNDS);
			  write_exp_elt_longcst ((LONGEST) yyvsp[0].lval);
			  write_exp_elt_opcode (BINOP_IN_BOUNDS);
		          write_exp_elt_opcode (UNOP_LOGICAL_NOT);
			}
    break;

  case 46:
#line 401 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_IN_RANGE);
		          write_exp_elt_type (yyvsp[0].tval);
		          write_exp_elt_opcode (UNOP_IN_RANGE);
		          write_exp_elt_opcode (UNOP_LOGICAL_NOT);
			}
    break;

  case 47:
#line 409 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_GEQ); }
    break;

  case 48:
#line 413 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_LESS); }
    break;

  case 49:
#line 417 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_GTR); }
    break;

  case 50:
#line 421 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_AND); }
    break;

  case 51:
#line 425 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_LOGICAL_AND); }
    break;

  case 52:
#line 429 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_IOR); }
    break;

  case 53:
#line 433 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_LOGICAL_OR); }
    break;

  case 54:
#line 437 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_BITWISE_XOR); }
    break;

  case 55:
#line 441 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_ADDR); }
    break;

  case 56:
#line 443 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type (type_system_address ());
			  write_exp_elt_opcode (UNOP_CAST);
			}
    break;

  case 57:
#line 449 "ada-exp.y"
    { write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_FIRST); }
    break;

  case 58:
#line 452 "ada-exp.y"
    { write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_LAST); }
    break;

  case 59:
#line 455 "ada-exp.y"
    { write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_LENGTH); }
    break;

  case 60:
#line 458 "ada-exp.y"
    { write_exp_elt_opcode (OP_ATR_SIZE); }
    break;

  case 61:
#line 460 "ada-exp.y"
    { write_exp_elt_opcode (OP_ATR_TAG); }
    break;

  case 62:
#line 462 "ada-exp.y"
    { write_exp_elt_opcode (OP_ATR_MIN); }
    break;

  case 63:
#line 464 "ada-exp.y"
    { write_exp_elt_opcode (OP_ATR_MAX); }
    break;

  case 64:
#line 466 "ada-exp.y"
    { write_exp_elt_opcode (OP_ATR_POS); }
    break;

  case 65:
#line 468 "ada-exp.y"
    { write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_FIRST); }
    break;

  case 66:
#line 471 "ada-exp.y"
    { write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_LAST); }
    break;

  case 67:
#line 474 "ada-exp.y"
    { write_int (yyvsp[0].lval, type_int ());
			  write_exp_elt_opcode (OP_ATR_LENGTH); }
    break;

  case 68:
#line 477 "ada-exp.y"
    { write_exp_elt_opcode (OP_ATR_VAL); }
    break;

  case 69:
#line 479 "ada-exp.y"
    { write_exp_elt_opcode (OP_ATR_MODULUS); }
    break;

  case 70:
#line 483 "ada-exp.y"
    { yyval.lval = 1; }
    break;

  case 71:
#line 485 "ada-exp.y"
    { yyval.lval = yyvsp[-1].typed_val.val; }
    break;

  case 72:
#line 490 "ada-exp.y"
    { write_exp_elt_opcode (OP_TYPE);
			  write_exp_elt_type (yyvsp[0].tval);
			  write_exp_elt_opcode (OP_TYPE); }
    break;

  case 74:
#line 498 "ada-exp.y"
    { write_exp_elt_opcode (OP_TYPE);
			  write_exp_elt_type (builtin_type_void);
			  write_exp_elt_opcode (OP_TYPE); }
    break;

  case 75:
#line 505 "ada-exp.y"
    { write_int ((LONGEST) yyvsp[0].typed_val.val, yyvsp[0].typed_val.type); }
    break;

  case 76:
#line 509 "ada-exp.y"
    { write_int (convert_char_literal (type_qualifier, yyvsp[0].typed_val.val),
			       (type_qualifier == NULL) 
			       ? yyvsp[0].typed_val.type : type_qualifier);
		  }
    break;

  case 77:
#line 516 "ada-exp.y"
    { write_exp_elt_opcode (OP_DOUBLE);
			  write_exp_elt_type (yyvsp[0].typed_val_float.type);
			  write_exp_elt_dblcst (yyvsp[0].typed_val_float.dval);
			  write_exp_elt_opcode (OP_DOUBLE);
			}
    break;

  case 78:
#line 524 "ada-exp.y"
    { write_int (0, type_int ()); }
    break;

  case 79:
#line 528 "ada-exp.y"
    { 
			  write_exp_elt_opcode (OP_STRING);
			  write_exp_string (yyvsp[0].sval);
			  write_exp_elt_opcode (OP_STRING);
			}
    break;

  case 80:
#line 536 "ada-exp.y"
    { error ("NEW not implemented."); }
    break;

  case 81:
#line 539 "ada-exp.y"
    { write_var_from_name (NULL, yyvsp[0].ssym); }
    break;

  case 82:
#line 541 "ada-exp.y"
    { write_var_from_name (yyvsp[-1].bval, yyvsp[0].ssym); }
    break;

  case 83:
#line 543 "ada-exp.y"
    { write_object_renaming (NULL, yyvsp[0].ssym.sym, 
				             MAX_RENAMING_CHAIN_LENGTH); }
    break;

  case 84:
#line 546 "ada-exp.y"
    { write_object_renaming (yyvsp[-1].bval, yyvsp[0].ssym.sym, 
					     MAX_RENAMING_CHAIN_LENGTH); }
    break;

  case 85:
#line 550 "ada-exp.y"
    { }
    break;

  case 86:
#line 551 "ada-exp.y"
    { }
    break;

  case 87:
#line 552 "ada-exp.y"
    { }
    break;

  case 88:
#line 556 "ada-exp.y"
    { yyval.bval = yyvsp[0].bval; }
    break;

  case 89:
#line 558 "ada-exp.y"
    { yyval.bval = yyvsp[0].bval; }
    break;

  case 90:
#line 562 "ada-exp.y"
    { yyval.tval = yyvsp[0].tval; }
    break;

  case 91:
#line 563 "ada-exp.y"
    { yyval.tval = yyvsp[0].tval; }
    break;

  case 92:
#line 565 "ada-exp.y"
    { yyval.tval = lookup_pointer_type (yyvsp[-1].tval); }
    break;

  case 93:
#line 567 "ada-exp.y"
    { yyval.tval = lookup_pointer_type (yyvsp[-1].tval); }
    break;

  case 94:
#line 574 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_IND); }
    break;

  case 95:
#line 576 "ada-exp.y"
    { write_exp_elt_opcode (UNOP_ADDR); }
    break;

  case 96:
#line 578 "ada-exp.y"
    { write_exp_elt_opcode (BINOP_SUBSCRIPT); }
    break;


    }

/* Line 1010 of yacc.c.  */

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

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



