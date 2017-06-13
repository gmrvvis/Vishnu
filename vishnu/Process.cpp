#include "Process.hpp"

namespace vishnu
{
    Process::Process(const std::string& shellCommand ,
      const std::map<std::string, std::string>& arguments,
      const std::string& workingDirectory)
    {
      _shellCommand = shellCommand;

      /* Not needed
	  #if defined(_WIN32) || defined(WIN32)
        _shellCommand += ".exe";
      #endif
	  */

      _arguments = arguments;

      _workingDirectory = workingDirectory;
      if ( workingDirectory != "" )
      {          
        this->setWorkingDirectory( QString::fromStdString( workingDirectory ) );
      }
    }

    Process::~Process( )
    {

    }

    std::string Process::getShellCommand( ) const
    {
      return _shellCommand;
    }

    std::map<std::string, std::string> Process::getArguments( ) const
    {
      return _arguments;
    }

    void Process::setArguments( const std::map<std::string, std::string>& arguments )
    {
      _arguments = arguments;
    }

    void Process::setArgument( const std::string& type, const std::string& value)
    {
      _arguments[type] = value;
    }
}
