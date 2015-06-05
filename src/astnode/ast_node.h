/*
 * ast_node.h
 *  Created on: May 21, 2015 4:10:35 PM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef AST_NODE_H_    // NOLINT
#define AST_NODE_H_
#include <vector>

enum AstNodeType {
    AST_NODE,
    AST_STMT_LIST,

    AST_SELECT_LIST,
    AST_SELECT_EXPR,
    AST_FROM_LIST,
    AST_TABLE,
    AST_SUBQUERY,
    AST_JOIN_CONDITION,
    AST_JOIN,
    AST_WHERE_CLAUSE,
    AST_GROUPBY_LIST,
    AST_GROUPBY_CLAUSE,
    AST_ORDERBY_LIST,
    AST_ORDERBY_CLAUSE,
    AST_HAVING_CLAUSE,
    AST_LIMIT_CLAUSE,
    AST_SELECT_INTO_CLAUSE,
    AST_COLUMN,
    AST_SELECT_STMT,

    AST_EXPR_CONST,
    AST_EXPR_CONST_STRING,
    AST_EXPR_CONST_DOUBLE,
    AST_EXPR_CONST_INT,
    AST_EXPR_CONST_BOOL,

    AST_EXPR_UNARY,
    AST_EXPR_BINARY,
    AST_EXPR_TERNARY,

    AST_EXPR_ADD,
    AST_EXPR_SUB,
    AST_EXPR_MUL,
    AST_EXPR_DIV,
    AST_EXPR_MOD,
    AST_EXPR_MOD_SIGN,
    AST_EXPR_POSITIVE,
    AST_EXPR_NEGATIVE,

    AST_EXPR_AND,
    AST_EXPR_OR,
    AST_EXPR_XOR,

    AST_EXPR_EQUAL,
    AST_EXPR_LESS,
    AST_EXPR_MORE,
    AST_EXPR_LESSEQU,
    AST_EXPR_MOREEQU,
    AST_EXPR_INEQUAL,
    AST_EXPR_UNKOWN,

    AST_EXPR_EQUAL_SUB,
    AST_EXPR_LESS_SUB,
    AST_EXPR_MORE_SUB,
    AST_EXPR_LESSEQU_SUB,
    AST_EXPR_MOREEQU_SUB,
    AST_EXPR_INEQUAL_SUB,
    AST_EXPR_UNKOWN_SUB,

    AST_EXPR_EQUAL_ALL,
    AST_EXPR_LESS_ALL,
    AST_EXPR_MORE_ALL,
    AST_EXPR_LESSEQU_ALL,
    AST_EXPR_MOREEQU_ALL,
    AST_EXPR_INEQUAL_ALL,
    AST_EXPR_UNKOWN_ALL,

    AST_EXPR_EQUAL_ANY,
    AST_EXPR_LESS_ANY,
    AST_EXPR_MORE_ANY,
    AST_EXPR_LESSEQU_ANY,
    AST_EXPR_MOREEQU_ANY,
    AST_EXPR_INEQUAL_ANY,
    AST_EXPR_UNKOWN_ANY,

    AST_EXPR_EQUAL_SOME,
    AST_EXPR_LESS_SOME,
    AST_EXPR_MORE_SOME,
    AST_EXPR_LESSEQU_SOME,
    AST_EXPR_MOREEQU_SOME,
    AST_EXPR_INEQUAL_SOME,
    AST_EXPR_UNKOWN_SOME,

    AST_EXPR_BIT_AND,
    AST_EXPR_BIT_OR,
    AST_EXPR_BIT_XOR,
    AST_EXPR_LSHIFT,
    AST_EXPR_RSHIFT,
    AST_EXPR_NOR,

    AST_EXPR_ISNULL,
    AST_EXPR_NOTNULL,
    AST_EXPR_ISBOOL,
    AST_EXPR_ISNOTBOOL,
    AST_EXPR_UNKNOWN,


    AST_COUNT_ALL,
    AST_COUNT_EXPR,
    AST_SUM_EXPR,
    AST_AVG_EXPR,
    AST_MIN_EXPR,
    AST_MAX_EXPR,

    AST_CMP_NULL,
    AST_CMP_SUBQUERY,
    AST_CMP_ANY,
    AST_CMP_SOME,
    AST_CMP_ALL,

    AST_EXPR_BETWEEN,


    AST_EXPR_LIST,
    AST_EXPR_IN_LIST,
    AST_EXPR_LIST_IN_LIST,
    AST_EXPR_NOTIN_LIST,
    AST_EXPR_LIST_NOTIN_SEL,
    AST_EXPR_LIST_NOTIN_LIST,
    AST_EXPR_IN_SEL,
    AST_EXPR_LIST_IN_SEL,
    AST_EXPR_NOTIN_SEL,
    AST_EXPR_EXSIST_SEL,
    AST_EXPR_NOTEXSIST_SEL,

    AST_SUBSTRING_EXPR_EXPR,
    AST_SUBSTRING_EXPR_FROM_EXPR,
    AST_SUBSTRING_EXPR_EXPR_EXPR,
    AST_SUBSTRING_EXPR_FROM_EXPR_FOR_EXPR,

    AST_UPPER,


    AST_TRIM_EXPR,
    AST_TRIM_UPPER,
    AST_TRIM_LTB_EXPR,
    AST_CAST,
    AST_COALESCE,
    AST_DATE_INT,
    AST_DATE_STRING,
    AST_DATE_DOUBLE,
    AST_DATE_FLOAT,
    AST_DATE_CHAR,
    AST_DATE_ADD,
    AST_DTAE_SUB,
    AST_INTERVAL_EXPR_DAY_HOUR,
    AST_INTERVAL_EXPR_DAY_MS,
    AST_INTERVAL_EXPR_DAY_MIN,
    AST_INTERVAL_EXPR_DAY_SEC,
    AST_INTERVAL_EXPR_DAY,
    AST_INTERVAL_EXPR_YEAR_MON,
    AST_INTERVAL_EXPR_YEAR,
    AST_INTERVAL_EXPR_WEEK,
    AST_INTERVAL_EXPR_QUARTER,
    AST_CASE_EXPR_LIST,
    AST_CASE_EXPR_LIST_ELSE,
    AST_CASE_LIST,
    AST_CASE_LIST_ELSE_EXPR,
    AST_WHEN_EXPR_THEN,
    AST_LIST_WHEN_EXPR_THEN,
    AST_EXPR_LIKE,
    AST_EXPR_NOTLIKE,
    AST_EXPR_REGEXP,
    AST_EXPR_NOTREGEXP,
};

class AstNode {
 public:
    explicit AstNode(AstNodeType ast_node_type);
    virtual ~AstNode();
    virtual void Print(int level = 0) const;
    AstNodeType ast_node_type();
    AstNodeType ast_node_type_;
};
struct ParseResult {
    void * yyscan_info_;
    AstNode * ast;
    const char * sql_clause;
    int error_number;
// std::vector<AstNode *>*node_pointer;
};

/*
 * used to link every statement in one sql contains multiple statement
 * for example: select a from tb;select max(a) from tb2;
 */
class AstStmtList : public AstNode {
 public:
    AstStmtList(AstNodeType ast_node_type, AstNode* stmt, AstNode* next);
    ~AstStmtList();
    void Print(int level = 0) const;
    AstNode* stmt_;
    AstNode* next_;
};

#endif  // AST_NODE_H__    //  NOLINT
