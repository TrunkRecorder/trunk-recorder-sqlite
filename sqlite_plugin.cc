
#define JSON_DIAGNOSTICS 1
#include <trunk-recorder/json.hpp>
#include <trunk-recorder/plugin_manager/plugin_api.h>
#include <trunk-recorder/systems/system.h>
#include <sqlite3.h> 
#include <boost/dll/alias.hpp> // for BOOST_DLL_ALIAS
#include <boost/foreach.hpp>
#include <boost/log/trivial.hpp>

class SQLite_Plugin : public Plugin_Api {
private:
    sqlite3 *db;

public:


int unit_registration(System *sys, long source_id) {
    std::string short_name = sys->get_short_name();
    //BOOST_LOG_TRIVIAL(info) << "EXAMPLE PLUGIN - unit_data_registration " << short_name;
             char *zErrMsg = 0;
   int rc;
char sql[1024];
snprintf(sql, sizeof(sql), "INSERT INTO UNITS (SOURCEID, DATETIME, REGISTER) VALUES (%ld, %ld, %ld);", 
    source_id, time(NULL), 1);



   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "[Unit Regirstration] SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } 
    return 0;
}

int unit_deregistration(System *sys, long source_id) { 
    std::string short_name = sys->get_short_name();
    //BOOST_LOG_TRIVIAL(info) << "EXAMPLE PLUGIN - unit_deregistration " << short_name;
             char *zErrMsg = 0;
   int rc;
char sql[1024];
snprintf(sql, sizeof(sql), "INSERT INTO UNITS (SOURCEID, DATETIME, REGISTER) VALUES (%ld, %ld, %ld);", 
    source_id, time(NULL), 0);



   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "[Unit Deregistration] SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } 
    return 0;
}  
int unit_acknowledge_response(System *sys, long source_id) { 
    std::string short_name = sys->get_short_name();
    //BOOST_LOG_TRIVIAL(info) << "EXAMPLE PLUGIN - unit_ack_response " << short_name;
    return 0;
}

int unit_group_affiliation(System *sys, long source_id, long talkgroup_num) {
    std::string short_name = sys->get_short_name();
    //BOOST_LOG_TRIVIAL(info) << "EXAMPLE PLUGIN - unit_group_affiliation " << short_name;
         char *zErrMsg = 0;
   int rc;
char sql[1024];
snprintf(sql, sizeof(sql), "INSERT INTO AFFILIATIONS (SOURCEID, DATETIME, TGNUM) VALUES (%ld, %ld, %ld);", 
    source_id, time(NULL), talkgroup_num);



   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "[Unit Affiliation] SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } 
    return 0;
}

int unit_data_grant(System *sys, long source_id) {
    std::string short_name = sys->get_short_name();
    //BOOST_LOG_TRIVIAL(info) << "EXAMPLE PLUGIN - unit_data_grant " << short_name;
    return 0;
}

int unit_answer_request(System *sys, long source_id, long talkgroup) {
    std::string short_name = sys->get_short_name();
    //BOOST_LOG_TRIVIAL(info) << "EXAMPLE PLUGIN - unit_answer_request " << short_name;
    return 0;
}

int unit_location(System *sys, long source_id, long talkgroup_num) {

    std::string short_name = sys->get_short_name();
    //BOOST_LOG_TRIVIAL(info) << "EXAMPLE PLUGIN - unit_location " << short_name;
    return 0;

}


int call_start(Call *call) {
     char *zErrMsg = 0;
   int rc;
char sql[1024];
snprintf(sql, sizeof(sql), "INSERT INTO CALLS (CALLNUM,DATETIME,TGNUM,ENCRYPTED,EMERGENCY,TDMA) VALUES (%ld, %ld, %ld, %d, %d, %d);", 
    call->get_call_num(), call->get_start_time(),  call->get_talkgroup(), call->get_encrypted(), call->get_emergency(), call->get_phase2_tdma());

//call->get_talkgroup_display()

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "[Call Start] SQL error: %s\n", zErrMsg);
      fprintf(stderr, "SQL: %s\n", sql);
      sqlite3_free(zErrMsg);
   } 
   return 0;
}


int call_end(Call_Data_t call_info) {
    /*
     char *zErrMsg = 0;
   int rc;
char sql[1024];
snprintf(sql, sizeof(sql), "INSERT INTO CALLS (CALLNUM,DATETIME,LENGTH,TGNUM,TGNAME,ENCRYPTED,EMERGENCY,ERRORCOUNT,SPIKECOUNT,TDMA) VALUES (%ld, %ld, %f, %ld, %s, %d, %d, %ld, %ld, %d);", 
    call_info.call_num, call_info.start_time, call_info.length, call_info.talkgroup, call_info.talkgroup_alpha_tag, call_info.encrypted, call_info.emergency, call_info.error_count, call_info.spike_count, call_info.phase2_tdma);

   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } */
   return 0;
}

int parse_config(boost::property_tree::ptree &cfg) {

    return 0;
  }


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

SQLite_Plugin() {
    char *zErrMsg = 0;
   int rc;
  char *sql;
   rc = sqlite3_open("test.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "CREATE TABLE IF NOT EXISTS UNITS("  \
      "SOURCEID             INTEGER  NOT NULL," \
      "DATETIME       INTEGER    NOT NULL," \
      "REGISTER         INTEGER    NOT NULL);";


   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "[Units Table] SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Units Table created successfully\n");
   }

   /* Create SQL statement */
   sql = "CREATE TABLE IF NOT EXISTS AFFILIATIONS("  \
      "SOURCEID             INTEGER  NOT NULL," \
      "DATETIME       INTEGER    NOT NULL," \
      "TGNUM          INTEGER    NOT NULL);";


   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "[Unit Affiliation] SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Unit Affiliations Table created successfully\n");
   }

   sql = "CREATE TABLE IF NOT EXISTS CALLS("  \
      "CALLNUM             INTEGER  NOT NULL," \
      "DATETIME       INTEGER    NOT NULL," \
      "TGNUM          INTEGER    NOT NULL," \
      "ENCRYPTED      NUMERIC    NOT NULL," \
        "EMERGENCY      NUMERIC    NOT NULL," \
        "TDMA           NUMERIC    NOT NULL);";


   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "[Calls] SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } 
}
  static boost::shared_ptr<SQLite_Plugin> create() {
    return boost::shared_ptr<SQLite_Plugin>(
        new SQLite_Plugin());
  }
};

BOOST_DLL_ALIAS(
    SQLite_Plugin::create, // <-- this function is exported with...
    create_plugin             // <-- ...this alias name
)
