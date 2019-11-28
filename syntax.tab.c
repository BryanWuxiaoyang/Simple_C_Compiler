/* A Bison parser, made by GNU Bison 3.4.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "syntax.y"

    #include <stdio.h>
    #include <string.h>
    #include "lex.yy.c"
    #include "SemanticAnalysis.h"
   #include "IntermediateLanguage.h"
    #include "LinearOptimize.h"

    void yyerror(char* msg);
    Node nodeBuffer[8];
    void errorTypeB(int lineno,int errorId){
        ok=0;
        if(lex_err == lineno){
            //lex_err=0;
            return;
        }
        printf("Error type B at Line %d: ",lineno);
        switch(errorId){
            case 1:printf("Error difine list\n");break;
            case 2:printf("Statement missing \";\"\n");break;
            case 3:printf("Statement missing \";\"\n");break;
            case 4:printf("Additional \"else\"\n");break;
            case 5:printf("Error use of \"[\"\n");break;
            case 6:printf("Statement missing \";\"\n");break;
            case 7:printf("Statement missing \";\"\n");break;
            case 20:printf("expression error\n");break;
            case 21:printf("brackets not matching\n"); break;
            default: printf("Syntax Error\n"); break;
        }
        lex_err = lineno;
    }

#line 103 "syntax.tab.c"

# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_SYNTAX_TAB_H_INCLUDED
# define YY_YY_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    FLOAT = 259,
    ID = 260,
    SEMI = 261,
    COMMA = 262,
    ASSIGNOP = 263,
    RELOP = 264,
    PLUS = 265,
    MINUS = 266,
    STAR = 267,
    DIV = 268,
    AND = 269,
    OR = 270,
    DOT = 271,
    NOT = 272,
    TYPE_INT = 273,
    TYPE_FLOAT = 274,
    LP = 275,
    RP = 276,
    LB = 277,
    RB = 278,
    LC = 279,
    RC = 280,
    STRUCT = 281,
    RETURN = 282,
    IF = 283,
    ELSE = 284,
    WHILE = 285,
    NEG = 286,
    ERROR = 287
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef struct _Node_* YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SYNTAX_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   300

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  137

#define YYUNDEFTOK  2
#define YYMAXUTOK   287

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    50,    50,    55,    59,    62,    67,    71,    76,    81,
      83,    85,    87,    89,    92,    95,   101,   104,   107,   111,
     118,   123,   126,   129,   133,   136,   142,   146,   152,   157,
     159,   162,   167,   171,   176,   182,   188,   192,   195,   199,
     202,   207,   214,   223,   230,   232,   234,   236,   243,   247,
     250,   255,   257,   260,   263,   269,   272,   278,   283,   288,
     293,   298,   303,   308,   313,   318,   323,   327,   331,   337,
     342,   348,   353,   356,   359,   362,   365,   367,   370,   375
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT", "ID", "SEMI", "COMMA",
  "ASSIGNOP", "RELOP", "PLUS", "MINUS", "STAR", "DIV", "AND", "OR", "DOT",
  "NOT", "TYPE_INT", "TYPE_FLOAT", "LP", "RP", "LB", "RB", "LC", "RC",
  "STRUCT", "RETURN", "IF", "ELSE", "WHILE", "NEG", "ERROR", "$accept",
  "Program", "ExtDefList", "ExtDef", "ExtDecList", "Specifier",
  "StructSpecifier", "OptTag", "Tag", "VarDec", "FunDec", "VarList",
  "ParamDec", "CompSt", "StmtList", "Stmt", "DefList", "Def", "DecList",
  "Dec", "Exp", "Args", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287
};
# endif

#define YYPACT_NINF -78

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-78)))

#define YYTABLE_NINF -50

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      47,    34,   -78,   -78,     3,    12,   -78,    47,    91,   -78,
     -78,   -78,   -78,    -8,    17,   -78,   -78,   -78,   -78,    23,
     -78,    69,    45,    14,    80,   123,   -78,   -78,    44,    21,
      55,   -78,    59,   -78,    95,    44,    83,    59,    88,   -78,
      44,   101,   136,   -78,   -78,   109,   -78,   -78,    90,   -78,
      66,   146,   138,   -78,   -78,   -78,   129,   -78,   -12,   -78,
     125,   -78,   -78,   133,   108,   108,   108,   144,   145,   108,
     -78,   143,    90,   165,   108,   -78,   -78,    44,   -78,   -78,
      99,    51,    -3,   213,   182,   108,   108,    -3,   -78,   -78,
     -78,   -78,   108,   108,   108,   108,   108,   108,   108,   108,
     177,    22,   258,   -78,   -78,   -78,   198,   163,   -78,   -78,
     -78,   228,   243,   258,   273,   278,   278,    -3,    -3,   266,
     124,   -78,   162,   -78,   147,   108,   -78,    99,    99,   -78,
     -78,   -78,   -78,   157,   -78,    99,   -78
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    17,    16,    22,     0,     2,     0,     0,    18,
      12,    13,     9,    23,     0,    20,     1,     3,    11,    24,
       6,     0,    14,     0,     0,     0,    10,     5,     0,     0,
       0,     8,     0,     7,     0,     0,     0,     0,    29,    28,
       0,     0,    32,    24,    15,     0,    26,    35,     0,    51,
      55,     0,    53,    19,    48,    30,    33,    27,     0,    25,
       0,    73,    74,    72,     0,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,    52,    50,     0,    31,    47,
       0,     0,    67,     0,     0,     0,     0,    66,    34,    36,
      44,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,    54,    46,    69,    79,     0,    65,    45,
      40,     0,     0,    57,    60,    61,    62,    63,    64,    58,
      59,    71,     0,    75,     0,     0,    68,     0,     0,    76,
      77,    70,    78,    41,    43,     0,    42
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -78,   -78,   192,   -78,   161,    11,   -78,   -78,   -78,   -31,
     -78,   142,   -78,   178,   130,   -77,   135,   -78,   126,   -78,
     -64,    92
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     5,     6,     7,    21,    35,     9,    14,    15,    22,
      23,    41,    42,    70,    71,    72,    36,    37,    51,    52,
      73,   107
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      82,    83,    84,   104,    50,    87,     2,     3,    13,    56,
     102,     8,    16,   100,     4,    30,   -21,   106,     8,   101,
      31,   111,   112,   122,    45,    61,    62,    63,   113,   114,
     115,   116,   117,   118,   119,   120,    40,   124,    32,    64,
      10,    24,    65,    25,    46,   123,    50,    -4,     1,    43,
     133,   134,    28,    69,    61,    62,    63,    11,   136,    12,
      34,   106,   -49,   -49,   -49,     2,     3,    29,    64,    40,
      26,    65,   105,     4,    74,    27,   -49,     2,     3,   -49,
      47,    34,    69,   -49,   -49,     4,   -49,   -49,    29,   -49,
     -49,    60,    18,    61,    62,    63,    19,    20,     2,     3,
      60,    49,    61,    62,    63,   -49,     4,    64,    53,    55,
      65,    61,    62,    63,    32,   -37,    64,    66,    67,    65,
      68,    69,    57,    32,    38,    64,    66,    67,    65,    68,
      69,    79,    59,    93,    94,    95,    96,    97,    98,    69,
     100,     2,     3,    58,    39,    77,   101,    75,   130,     4,
      47,    29,    76,    81,    80,    92,    93,    94,    95,    96,
      97,    98,    99,   100,    85,    86,    90,    48,    88,   101,
     131,    91,    54,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   121,   109,   126,   129,   135,   101,   110,    44,
      92,    93,    94,    95,    96,    97,    98,    99,   100,    17,
      78,    33,    89,   103,   101,   125,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,   132,     0,     0,
     101,    92,    93,    94,    95,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   108,   101,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   127,
     101,    92,    93,    94,    95,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   128,   101,    92,    93,    94,    95,
      96,    97,    98,    99,   100,    93,    94,    95,    96,    97,
     101,     0,   100,    94,    95,    96,    97,     0,   101,   100,
      96,    97,     0,     0,   100,   101,     0,     0,     0,     0,
     101
};

static const yytype_int16 yycheck[] =
{
      64,    65,    66,    80,    35,    69,    18,    19,     5,    40,
      74,     0,     0,    16,    26,     1,    24,    81,     7,    22,
       6,    85,    86,     1,     3,     3,     4,     5,    92,    93,
      94,    95,    96,    97,    98,    99,    25,   101,    24,    17,
       6,    24,    20,    20,    23,    23,    77,     0,     1,     5,
     127,   128,     7,    31,     3,     4,     5,    23,   135,    25,
       1,   125,     3,     4,     5,    18,    19,    22,    17,    58,
       1,    20,    21,    26,     8,     6,    17,    18,    19,    20,
      25,     1,    31,    24,    25,    26,    27,    28,    22,    30,
      31,     1,     1,     3,     4,     5,     5,     6,    18,    19,
       1,     6,     3,     4,     5,    25,    26,    17,    25,    21,
      20,     3,     4,     5,    24,    25,    17,    27,    28,    20,
      30,    31,    21,    24,     1,    17,    27,    28,    20,    30,
      31,     6,    23,     9,    10,    11,    12,    13,    14,    31,
      16,    18,    19,     7,    21,     7,    22,     1,     1,    26,
      25,    22,     6,    20,    29,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    20,    20,     1,    32,    25,    22,
      23,     6,    37,     8,     9,    10,    11,    12,    13,    14,
      15,    16,     5,     1,    21,    23,    29,    22,     6,    28,
       8,     9,    10,    11,    12,    13,    14,    15,    16,     7,
      58,    23,    72,    77,    22,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    -1,   125,    -1,    -1,
      22,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    21,    22,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,
      22,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    21,    22,     8,     9,    10,    11,
      12,    13,    14,    15,    16,     9,    10,    11,    12,    13,
      22,    -1,    16,    10,    11,    12,    13,    -1,    22,    16,
      12,    13,    -1,    -1,    16,    22,    -1,    -1,    -1,    -1,
      22
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    18,    19,    26,    34,    35,    36,    38,    39,
       6,    23,    25,     5,    40,    41,     0,    35,     1,     5,
       6,    37,    42,    43,    24,    20,     1,     6,     7,    22,
       1,     6,    24,    46,     1,    38,    49,    50,     1,    21,
      38,    44,    45,     5,    37,     3,    23,    25,    49,     6,
      42,    51,    52,    25,    49,    21,    42,    21,     7,    23,
       1,     3,     4,     5,    17,    20,    27,    28,    30,    31,
      46,    47,    48,    53,     8,     1,     6,     7,    44,     6,
      29,    20,    53,    53,    53,    20,    20,    53,    25,    47,
       1,     6,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    22,    53,    51,    48,    21,    53,    54,    21,     1,
       6,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,     5,     1,    23,    53,     7,    21,    21,    21,    23,
       1,    23,    54,    48,    48,    29,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    35,    35,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    37,    37,    38,    38,    38,    39,
      39,    40,    40,    41,    42,    42,    42,    43,    43,    43,
      43,    44,    44,    45,    46,    46,    47,    47,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    49,    49,
      50,    50,    50,    51,    51,    52,    52,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    54,    54
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     3,     2,
       3,     2,     2,     2,     1,     3,     1,     1,     1,     5,
       2,     1,     0,     1,     1,     4,     3,     4,     3,     3,
       4,     3,     1,     2,     4,     2,     2,     0,     2,     1,
       3,     5,     7,     5,     2,     3,     3,     2,     2,     0,
       3,     2,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     4,     3,
       4,     3,     1,     1,     1,     3,     4,     4,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 50 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0];  
        yyval=createNode(1,nodeBuffer,1,Program,(yyloc).first_line);     
        nodeBuffer[0]=yyval; 
    }
#line 1506 "syntax.tab.c"
    break;

  case 3:
#line 55 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1];   
        nodeBuffer[1]=yyvsp[0];  
        yyval=createNode(2,nodeBuffer,1,ExtDefList,(yyloc).first_line);
    }
#line 1516 "syntax.tab.c"
    break;

  case 4:
#line 59 "syntax.y"
    {
        yyval=createNode(0,NULL,2,ExtDefList,(yyloc).first_line); 
    }
#line 1524 "syntax.tab.c"
    break;

  case 5:
#line 62 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,1,ExtDef,(yyloc).first_line); 
    }
#line 1535 "syntax.tab.c"
    break;

  case 6:
#line 67 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,2,ExtDef,(yyloc).first_line); 
    }
#line 1545 "syntax.tab.c"
    break;

  case 7:
#line 71 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,3,ExtDef,(yyloc).first_line);
    }
#line 1556 "syntax.tab.c"
    break;

  case 8:
#line 76 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2];
        nodeBuffer[1]=yyvsp[-1];
        nodeBuffer[2]=yyvsp[0];
        yyval=createNode(3,nodeBuffer,4,ExtDef,(yyloc).first_line);
    }
#line 1567 "syntax.tab.c"
    break;

  case 9:
#line 81 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 12);
    }
#line 1575 "syntax.tab.c"
    break;

  case 10:
#line 83 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 2);
    }
#line 1583 "syntax.tab.c"
    break;

  case 11:
#line 85 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 2);
    }
#line 1591 "syntax.tab.c"
    break;

  case 12:
#line 87 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 1);
    }
#line 1599 "syntax.tab.c"
    break;

  case 13:
#line 89 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 1);
    }
#line 1607 "syntax.tab.c"
    break;

  case 14:
#line 92 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,1,ExtDecList,(yyloc).first_line); 
    }
#line 1616 "syntax.tab.c"
    break;

  case 15:
#line 95 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,2,ExtDecList,(yyloc).first_line);
    }
#line 1627 "syntax.tab.c"
    break;

  case 16:
#line 101 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,1,Specifier,(yyloc).first_line); 
    }
#line 1636 "syntax.tab.c"
    break;

  case 17:
#line 104 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,2,Specifier,(yyloc).first_line); 
    }
#line 1645 "syntax.tab.c"
    break;

  case 18:
#line 107 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,3,Specifier,(yyloc).first_line); 
    }
#line 1654 "syntax.tab.c"
    break;

  case 19:
#line 111 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-4]; 
        nodeBuffer[1]=yyvsp[-3]; 
        nodeBuffer[2]=yyvsp[-2]; 
        nodeBuffer[3]=yyvsp[-1]; 
        nodeBuffer[4]=yyvsp[0]; 
        yyval=createNode(5,nodeBuffer,1,StructSpecifier,(yyloc).first_line); 
    }
#line 1667 "syntax.tab.c"
    break;

  case 20:
#line 118 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,2,StructSpecifier,(yyloc).first_line); 
    }
#line 1677 "syntax.tab.c"
    break;

  case 21:
#line 123 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,1,OptTag,(yyloc).first_line);
    }
#line 1686 "syntax.tab.c"
    break;

  case 22:
#line 126 "syntax.y"
    {
        yyval=createNode(0,NULL,2,OptTag,(yyloc).first_line);
    }
#line 1694 "syntax.tab.c"
    break;

  case 23:
#line 129 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,1,Tag,(yyloc).first_line); 
    }
#line 1703 "syntax.tab.c"
    break;

  case 24:
#line 133 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,1,VarDec,(yyloc).first_line); 
    }
#line 1712 "syntax.tab.c"
    break;

  case 25:
#line 136 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-3]; 
        nodeBuffer[1]=yyvsp[-2]; 
        nodeBuffer[2]=yyvsp[-1]; 
        nodeBuffer[3]=yyvsp[0]; 
        yyval=createNode(4,nodeBuffer,2,VarDec,(yyloc).first_line); 
    }
#line 1724 "syntax.tab.c"
    break;

  case 26:
#line 142 "syntax.y"
    {
        yyval = createNode(0, NULL,3,VarDec,(yyloc).first_line); 
        errorTypeB((yyloc).first_line, 5);
    }
#line 1733 "syntax.tab.c"
    break;

  case 27:
#line 146 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-3]; 
        nodeBuffer[1]=yyvsp[-2]; 
        nodeBuffer[2]=yyvsp[-1]; 
        nodeBuffer[3]=yyvsp[0]; 
        yyval=createNode(4,nodeBuffer,1,FunDec,(yyloc).first_line); 
    }
#line 1745 "syntax.tab.c"
    break;

  case 28:
#line 152 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,2,FunDec,(yyloc).first_line); 
    }
#line 1756 "syntax.tab.c"
    break;

  case 29:
#line 157 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 21);
    }
#line 1764 "syntax.tab.c"
    break;

  case 30:
#line 159 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 21);
    }
#line 1772 "syntax.tab.c"
    break;

  case 31:
#line 162 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,1,VarList,(yyloc).first_line); 
    }
#line 1783 "syntax.tab.c"
    break;

  case 32:
#line 167 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,2,VarList,(yyloc).first_line); 
    }
#line 1792 "syntax.tab.c"
    break;

  case 33:
#line 171 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,1,ParamDec,(yyloc).first_line); 
    }
#line 1802 "syntax.tab.c"
    break;

  case 34:
#line 176 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-3]; 
        nodeBuffer[1]=yyvsp[-2]; 
        nodeBuffer[2]=yyvsp[-1]; 
        nodeBuffer[3]=yyvsp[0]; 
        yyval=createNode(4,nodeBuffer,1,CompSt,(yyloc).first_line); 
    }
#line 1814 "syntax.tab.c"
    break;

  case 35:
#line 182 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,2,CompSt,(yyloc).first_line); 
        errorTypeB((yyloc).first_line, 0);
    }
#line 1825 "syntax.tab.c"
    break;

  case 36:
#line 188 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,1,StmtList,(yyloc).first_line); 
    }
#line 1835 "syntax.tab.c"
    break;

  case 37:
#line 192 "syntax.y"
    {
        yyval=createNode(0,NULL,2,StmtList,(yyloc).first_line);
    }
#line 1843 "syntax.tab.c"
    break;

  case 38:
#line 195 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,1,Stmt,(yyloc).first_line); 
    }
#line 1853 "syntax.tab.c"
    break;

  case 39:
#line 199 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,2,Stmt,(yyloc).first_line); 
    }
#line 1862 "syntax.tab.c"
    break;

  case 40:
#line 202 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,3,Stmt,(yyloc).first_line); 
    }
#line 1873 "syntax.tab.c"
    break;

  case 41:
#line 207 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-4]; 
        nodeBuffer[1]=yyvsp[-3]; 
        nodeBuffer[2]=yyvsp[-2]; 
        nodeBuffer[3]=yyvsp[-1]; 
        nodeBuffer[4]=yyvsp[0]; 
        yyval=createNode(5,nodeBuffer,4,Stmt,(yyloc).first_line); 
    }
#line 1886 "syntax.tab.c"
    break;

  case 42:
#line 214 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-6]; 
        nodeBuffer[1]=yyvsp[-5]; 
        nodeBuffer[2]=yyvsp[-4]; 
        nodeBuffer[3]=yyvsp[-3]; 
        nodeBuffer[4]=yyvsp[-2]; 
        nodeBuffer[5]=yyvsp[-1]; 
        nodeBuffer[6]=yyvsp[0]; 
        yyval=createNode(7,nodeBuffer,5,Stmt,(yyloc).first_line);
    }
#line 1901 "syntax.tab.c"
    break;

  case 43:
#line 223 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-4]; 
        nodeBuffer[1]=yyvsp[-3]; 
        nodeBuffer[2]=yyvsp[-2]; 
        nodeBuffer[3]=yyvsp[-1]; 
        nodeBuffer[4]=yyvsp[0]; 
        yyval=createNode(5,nodeBuffer,6,Stmt,(yyloc).first_line); 
    }
#line 1914 "syntax.tab.c"
    break;

  case 44:
#line 230 "syntax.y"
    { 
        errorTypeB((yyloc).first_line, 2);
    }
#line 1922 "syntax.tab.c"
    break;

  case 45:
#line 232 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 3);
    }
#line 1930 "syntax.tab.c"
    break;

  case 46:
#line 234 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 4);
    }
#line 1938 "syntax.tab.c"
    break;

  case 47:
#line 236 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,7,Stmt,(yyloc).first_line); 
        errorTypeB((yyloc).first_line, 0);
        yyerrok;
    }
#line 1950 "syntax.tab.c"
    break;

  case 48:
#line 243 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,1,DefList,(yyloc).first_line); 
    }
#line 1960 "syntax.tab.c"
    break;

  case 49:
#line 247 "syntax.y"
    {
        yyval=createNode(0,NULL,2,DefList,(yyloc).first_line); 
    }
#line 1968 "syntax.tab.c"
    break;

  case 50:
#line 250 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,1,Def,(yyloc).first_line); 
    }
#line 1979 "syntax.tab.c"
    break;

  case 51:
#line 255 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 1);
    }
#line 1987 "syntax.tab.c"
    break;

  case 52:
#line 257 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 9);
    }
#line 1995 "syntax.tab.c"
    break;

  case 53:
#line 260 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,1,DecList,(yyloc).first_line); 
    }
#line 2004 "syntax.tab.c"
    break;

  case 54:
#line 263 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,2,DecList,(yyloc).first_line); 
    }
#line 2015 "syntax.tab.c"
    break;

  case 55:
#line 269 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,1,Dec,(yyloc).first_line); 
    }
#line 2024 "syntax.tab.c"
    break;

  case 56:
#line 272 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,2,Dec,(yyloc).first_line); 
    }
#line 2035 "syntax.tab.c"
    break;

  case 57:
#line 278 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,1,Exp,(yyloc).first_line); 
    }
#line 2046 "syntax.tab.c"
    break;

  case 58:
#line 283 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,2,Exp,(yyloc).first_line); 
    }
#line 2057 "syntax.tab.c"
    break;

  case 59:
#line 288 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,3,Exp,(yyloc).first_line); 
    }
#line 2068 "syntax.tab.c"
    break;

  case 60:
#line 293 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,4,Exp,(yyloc).first_line); 
    }
#line 2079 "syntax.tab.c"
    break;

  case 61:
#line 298 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,5,Exp,(yyloc).first_line); 
    }
#line 2090 "syntax.tab.c"
    break;

  case 62:
#line 303 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,6,Exp,(yyloc).first_line); 
    }
#line 2101 "syntax.tab.c"
    break;

  case 63:
#line 308 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,7,Exp,(yyloc).first_line); 
    }
#line 2112 "syntax.tab.c"
    break;

  case 64:
#line 313 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,8,Exp,(yyloc).first_line); 
    }
#line 2123 "syntax.tab.c"
    break;

  case 65:
#line 318 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,9,Exp,(yyloc).first_line);
    }
#line 2134 "syntax.tab.c"
    break;

  case 66:
#line 323 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,10,Exp,(yyloc).first_line); 
    }
#line 2144 "syntax.tab.c"
    break;

  case 67:
#line 327 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-1]; 
        nodeBuffer[1]=yyvsp[0]; 
        yyval=createNode(2,nodeBuffer,11,Exp,(yyloc).first_line); 
    }
#line 2154 "syntax.tab.c"
    break;

  case 68:
#line 331 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-3]; 
        nodeBuffer[1]=yyvsp[-2]; 
        nodeBuffer[2]=yyvsp[-1]; 
        nodeBuffer[3]=yyvsp[0]; 
        yyval=createNode(4,nodeBuffer,12,Exp,(yyloc).first_line); 
    }
#line 2166 "syntax.tab.c"
    break;

  case 69:
#line 337 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,13,Exp,(yyloc).first_line); 
    }
#line 2177 "syntax.tab.c"
    break;

  case 70:
#line 342 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-3]; 
        nodeBuffer[1]=yyvsp[-2]; 
        nodeBuffer[2]=yyvsp[-1]; 
        nodeBuffer[3]=yyvsp[0]; 
        yyval=createNode(4,nodeBuffer,14,Exp,(yyloc).first_line); 
    }
#line 2189 "syntax.tab.c"
    break;

  case 71:
#line 348 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,15,Exp,(yyloc).first_line); 
    }
#line 2200 "syntax.tab.c"
    break;

  case 72:
#line 353 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,16,Exp,(yyloc).first_line); 
    }
#line 2209 "syntax.tab.c"
    break;

  case 73:
#line 356 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,17,Exp,(yyloc).first_line); 
    }
#line 2218 "syntax.tab.c"
    break;

  case 74:
#line 359 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,18,Exp,(yyloc).first_line); 
    }
#line 2227 "syntax.tab.c"
    break;

  case 75:
#line 362 "syntax.y"
    {
        yyval = createNode(0, nodeBuffer,19,Exp,(yyloc).first_line); 
        errorTypeB((yyloc).first_line, 5);
    }
#line 2236 "syntax.tab.c"
    break;

  case 76:
#line 365 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 21);
    }
#line 2244 "syntax.tab.c"
    break;

  case 77:
#line 367 "syntax.y"
    {
        errorTypeB((yyloc).first_line, 21);
    }
#line 2252 "syntax.tab.c"
    break;

  case 78:
#line 370 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[-2]; 
        nodeBuffer[1]=yyvsp[-1]; 
        nodeBuffer[2]=yyvsp[0]; 
        yyval=createNode(3,nodeBuffer,1,Args,(yyloc).first_line); 
    }
#line 2263 "syntax.tab.c"
    break;

  case 79:
#line 375 "syntax.y"
    {
        nodeBuffer[0]=yyvsp[0]; 
        yyval=createNode(1,nodeBuffer,2,Args,(yyloc).first_line); 
    }
#line 2272 "syntax.tab.c"
    break;


#line 2276 "syntax.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 379 "syntax.y"


void yyerror(char* msg){
        ok=0;
}

int main(int argc,char** argv){
    if(argc<=1)
        return 1;
    FILE* f=fopen(argv[1],"r");
    if(!f)
    {
        printf("Error Open File!\n");
        return 1;
    }
    yyrestart(f);
    yyparse();
    FILE* file = fopen("temp.txt","w");
    if(ok){
        putSyntaxTreeToFile(file, nodeBuffer[0]);
        fclose(file);
        semAnalysis(nodeBuffer[0]);
        printInterCodeList(NULL,NULL);
        optimizeInterCodeLinear();
        printInterCodeList(NULL, NULL);
    }
    return 0;
}
