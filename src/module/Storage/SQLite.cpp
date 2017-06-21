#include "Record.h"
#include <Skuld/Exception.h>
#include <sqlite3.h>

namespace Skuld
{
	namespace Storage
	{
		class SQLite : public Database
		{
		protected:
			virtual ~SQLite();

			sqlite3* mDatabase;
		public:
			SQLite(sqlite3* sql) : mDatabase(sql) {}
		};

		SQLite::~SQLite()
		{
			sqlite3_close(mDatabase);
		}

		DataRecord * Database::_CreateDataRecord()
		{
			return new (std::nothrow) Record();
		}

		Database* Database::CreateSQLiteDatabase(const String& path)
		{
			sqlite3* sqlite;
			sqlite3_open(path.GetUTF8Str().c_str(), &sqlite);
			return new (std::nothrow) SQLite(sqlite);
		}
	}
}