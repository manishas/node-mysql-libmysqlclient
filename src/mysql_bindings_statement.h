/*!
 * Copyright by Oleg Efimov and node-mysql-libmysqlclient contributors
 * See contributors list in README
 *
 * See license text in LICENSE file
 */

#ifndef SRC_MYSQL_BINDINGS_STATEMENT_H_
#define SRC_MYSQL_BINDINGS_STATEMENT_H_

#include <mysql.h>

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>

#include "./mysql_bindings.h"

#define MYSQLSTMT_MUSTBE_INITIALIZED \
    if (stmt->state < STMT_INITIALIZED) { \
        return THREXC("Statement not initialized"); \
    }

#define MYSQLSTMT_MUSTBE_PREPARED \
    if (stmt->state < STMT_PREPARED) { \
        return THREXC("Statement not prepared"); \
    }

#define MYSQLSTMT_MUSTBE_EXECUTED \
    if (stmt->state < STMT_EXECUTED) { \
        return THREXC("Statement not executed"); \
    }

#define MYSQLSTMT_MUSTBE_STORED \
    if (stmt->state < STMT_STORED_RESULT) { \
        return THREXC("Statement result not stored"); \
    }

/** section: Classes
 * class MysqlStatement
 *
 * MySQL prepared statement class
 **/
class MysqlStatement : public node::ObjectWrap {
  public:
    static Persistent<FunctionTemplate> constructor_template;

    static void Init(Handle<Object> target);

  protected:
    MYSQL_STMT *_stmt;

    MYSQL_BIND *binds;
    unsigned long param_count;

    enum MysqlStatementState {
        STMT_CLOSED,
        STMT_INITIALIZED,
        STMT_PREPARED,
        STMT_BINDED_PARAMS,
        STMT_EXECUTED,
        STMT_BINDED_BUFFERS,
        STMT_STORED_RESULT,
    };
    MysqlStatementState state;

    MysqlStatement(MYSQL_STMT *my_stmt);

    ~MysqlStatement();

    // Constructor

    static Handle<Value> New(const Arguments& args);

    // Properties

    static Handle<Value> ParamCountGetter(Local<String> property,
                                           const AccessorInfo &info);

    // Methods

    static Handle<Value> AffectedRowsSync(const Arguments& args);

    static Handle<Value> AttrGetSync(const Arguments& args);

    static Handle<Value> AttrSetSync(const Arguments& args);

    static Handle<Value> BindParamsSync(const Arguments& args);

    static Handle<Value> CloseSync(const Arguments& args);

    static Handle<Value> DataSeekSync(const Arguments& args);

    static Handle<Value> ErrnoSync(const Arguments& args);

    static Handle<Value> ErrorSync(const Arguments& args);

    struct execute_request {
      bool ok;

      Persistent<Function> callback;
      MysqlStatement* stmt;
    };

    static void EIO_After_Execute(uv_work_t* req);

    static void EIO_Execute(uv_work_t* req);

    static Handle<Value> Execute(const Arguments& args);

    static Handle<Value> ExecuteSync(const Arguments& args);

    struct fetchAll_request {
      bool ok;
      bool empty_resultset;

      Persistent<Function> callback;
      MysqlStatement* stmt;

      MYSQL_RES* meta;
      unsigned long field_count;
      my_bool* is_null;
      unsigned long* length;
      void** buffers;
    };

    static int BindResult(MYSQL_STMT* stmt, MYSQL_FIELD* fields, unsigned int field_count, unsigned long* length, my_bool* is_null, void** buffers);

    static Local<Value> GetFieldValue(void* ptr, unsigned long& length, MYSQL_FIELD& field);

    static void EIO_After_FetchAll(uv_work_t* req);

    static void EIO_FetchAll(uv_work_t* req);

    static Handle<Value> FetchAll(const Arguments& args);

    static Handle<Value> FetchAllSync(const Arguments& args);

    static Handle<Value> FieldCountSync(const Arguments& args);

    static Handle<Value> FreeResultSync(const Arguments& args);

    static Handle<Value> LastInsertIdSync(const Arguments& args);

    static Handle<Value> NextResultSync(const Arguments& args);

    static Handle<Value> NumRowsSync(const Arguments& args);

    static Handle<Value> PrepareSync(const Arguments& args);

    static Handle<Value> ResetSync(const Arguments& args);

    static Handle<Value> ResultMetadataSync(const Arguments& args);

    static Handle<Value> SendLongDataSync(const Arguments& args);

    static Handle<Value> StoreResultSync(const Arguments& args);

    static Handle<Value> SqlStateSync(const Arguments& args);
};

#endif  // SRC_MYSQL_BINDINGS_STATEMENT_H_

