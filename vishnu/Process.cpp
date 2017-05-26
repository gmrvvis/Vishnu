#include "Process.hpp"

namespace vishnu
{
    Process::Process(const std::string& shellCommand ,
      const QStringList& arguments)
    {
      _shellCommand = shellCommand;

      #ifdef defined(_WIN32) || defined(WIN32)
        _shellCommand += ".exe";
      #endif

      if( !arguments.empty( ) )
      {
          _arguments = arguments;
      }
    }

    Process::~Process( )
    {

    }

    std::string Process::getShellCommand( ) const
    {
      return _shellCommand;
    }

    QStringList Process::getArguments( ) const
    {
      return _arguments;
    }

    void Process::setArguments( const QStringList& arguments )
    {
      _arguments = arguments;
    }

    void Process::addArgument( const std::string& argument )
    {
        _arguments << QString::fromStdString( argument );
    }

}
