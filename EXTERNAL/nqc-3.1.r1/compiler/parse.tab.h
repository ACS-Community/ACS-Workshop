//
typedef union {
        int			fInt;
        bool		fBool;
        Resource*	fResource;
        Fragment*	fFragment;
        Stmt*		fStmt;
        BlockStmt*	fBlock;
        Symbol*		fSymbol;
        char*		fString;
        Expr*		fExpr;
        FunctionDef*	fFunction;
        CallStmt*	fCall;
        AsmStmt*	fAsmStmt;
        Field*		fField;
        CaseStmt*	fCaseStmt;
        DeclareStmt*	fDeclareStmt;
        LocationNode*	fLocation;
} YYSTYPE;
#define	OR	258
#define	AND	259
#define	REL_EQ	260
#define	REL_NE	261
#define	REL_LE	262
#define	REL_GE	263
#define	LEFT	264
#define	RIGHT	265
#define	UMINUS	266
#define	INDIRECT	267
#define	INCDEC	268
#define	ABS	269
#define	SIGN	270
#define	TYPE	271
#define	EVENT_SRC	272
#define	LOWER_THAN_ELSE	273
#define	ELSE	274
#define	LOWER_THAN_EXPR_SHIFT	275
#define	ID	276
#define	NUMBER	277
#define	ASSIGN	278
#define	ASSIGN2	279
#define	TASKOP	280
#define	JUMP	281
#define	TASK	282
#define	SUB	283
#define	STRING	284
#define	PP_DEFINE	285
#define	PP_INCLUDE	286
#define	NL	287
#define	WS	288
#define	PP_ARG	289
#define	PP_UNKNOWN	290
#define	PP_IFDEF	291
#define	PP_IF	292
#define	PP_ELSE	293
#define	PP_ELIF	294
#define	PP_ENDIF	295
#define	PP_UNDEF	296
#define	PP_PRAGMA	297
#define	PP_GLOM	298
#define	PP_ERROR	299
#define	PP_WARNING	300
#define	IF	301
#define	WHILE	302
#define	DO	303
#define	FOR	304
#define	REPEAT	305
#define	SWITCH	306
#define	CASE	307
#define	DEFAULT	308
#define	MONITOR	309
#define	CATCH	310
#define	ACQUIRE	311
#define	GOTO	312
#define	ASM	313
#define	INT	314
#define	T_VOID	315
#define	T_CONST	316
#define	SENSOR	317
#define	TASKID	318
#define	NOLIST	319
#define	RES	320
#define	CTRUE	321
#define	CFALSE	322


extern YYSTYPE yylval;
