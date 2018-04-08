#include <connection/DatabaseConnection.H>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>

namespace Algo
{

DatabaseConnection& DatabaseConnection::getInstance()
{
  static DatabaseConnection instance;
  return instance;
}

DatabaseConnection::DatabaseConnection()
{
}

bool DatabaseConnection::startConnection(const std::string& user,
                                         const std::string& passwd,
                                         const std::string& db,
                                         const std::string& host,
                                         unsigned int port)
{
  try
  {
    _db = new odb::mysql::database(user, passwd, db, host, port);
    // To test connection
    _db->connection();
    return true;
  }
  catch(const odb::exception& e)
  {
    std::cerr << "Failed to connect to DB: " << e.what() << std::endl;
    return false;
  }
}

bool DatabaseConnection::startTransaction()
{
  try
  {
    _transaction.reset(_db->begin());
  }
  catch(const odb::exception& e)
  {
    std::cerr << "DB error: " << e.what () << std::endl;
    return false;
  }

  return true;
}

bool DatabaseConnection::commitTransaction()
{
  assert(!_transaction.finalized());

  try
  {
    _transaction.commit();
  }
  catch(const odb::exception& e)
  {
    std::cerr << "DB error: " << e.what () << std::endl;
    return false;
  }

  return true;
}

} // namespace Algo
