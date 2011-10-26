/**

 Logger.h
 
 Created on: 	  02.10.2010
 Author: 			  Kay Wolter
 E-Mail:				wolter.kay@freenet.de

 @brief	Generic logger, provides different logging levels to adjust the verbosity of the logging output.

 */

#ifndef _G_IO_LOGGER_H_
#define _G_IO_LOGGER_H_

#include <string>
#include <iostream>

namespace g
{
  namespace io
  {

    /**
     * Generic logger, provides different logging levels to adjust the verbosity of the logging output.
     * The logging level is a global integer variable and used by all logger instances.
     * The output stream is also a global variable and used by all logger instances.
     */
    class Logger
    {
    public:

      /**
       * Definitions of the possible logging levels.
       */
      typedef enum { Error = 0 , Warning = 1 , Note = 2 , Info = 3 , Debug = 4 , Verbose = 5 } Level;

      /**
       * Constructor.
       * @param   id  String that describes the context of the logger instance.
       */
      Logger( const std::string &id );

      /**
       * Destructor.
       */
      virtual ~Logger();

      /**
       * Log error.
       * @param   message     Message to log.
       * @param   bLineBreak  Tell the logger if a line break should be added to the end of the message.
       *                      If this value is false, the message is flushed without line break and the cursor is sought back to beginning of the line.
       *                      The following messages with bLineBreak = false are all printed to the same line and will thus overwrite the previous message.
       *                      The first message with bLineBreak = true is printed on the next line with line break afterwards.
       */
      virtual void error( const std::string &message , bool bLineBreak = true ) const;

      /**
       * Log warning.
       * @param   message     Message to log.
       * @param   bLineBreak  Tell the logger if a line break should be added to the end of the message.
       *                      If this value is false, the message is flushed without line break and the cursor is sought back to beginning of the line.
       *                      The following messages with bLineBreak = false are all printed to the same line and will thus overwrite the previous message.
       *                      The first message with bLineBreak = true is printed on the next line with line break afterwards.
       */
      virtual void warning( const std::string &message , bool bLineBreak = true ) const;

      /**
       * Log notification.
       * @param   message     Message to log.
       * @param   bLineBreak  Tell the logger if a line break should be added to the end of the message.
       *                      If this value is false, the message is flushed without line break and the cursor is sought back to beginning of the line.
       *                      The following messages with bLineBreak = false are all printed to the same line and will thus overwrite the previous message.
       *                      The first message with bLineBreak = true is printed on the next line with line break afterwards.
       */
      virtual void note( const std::string &message , bool bLineBreak = true ) const;

      /**
       * Log information.
       * @param   message     Message to log.
       * @param   bLineBreak  Tell the logger if a line break should be added to the end of the message.
       *                      If this value is false, the message is flushed without line break and the cursor is sought back to beginning of the line.
       *                      The following messages with bLineBreak = false are all printed to the same line and will thus overwrite the previous message.
       *                      The first message with bLineBreak = true is printed on the next line with line break afterwards.
       */
      virtual void info( const std::string &message , bool bLineBreak = true ) const;

      /**
       * Log debugging information.
       * @param   message     Message to log.
       * @param   bLineBreak  Tell the logger if a line break should be added to the end of the message.
       *                      If this value is false, the message is flushed without line break and the cursor is sought back to beginning of the line.
       *                      The following messages with bLineBreak = false are all printed to the same line and will thus overwrite the previous message.
       *                      The first message with bLineBreak = true is printed on the next line with line break afterwards.
       */
      virtual void debug( const std::string &message , bool bLineBreak = true ) const;

      /**
       * Log everything.
       * @param   message     Message to log.
       * @param   bLineBreak  Tell the logger if a line break should be added to the end of the message.
       *                      If this value is false, the message is flushed without line break and the cursor is sought back to beginning of the line.
       *                      The following messages with bLineBreak = false are all printed to the same line and will thus overwrite the previous message.
       *                      The first message with bLineBreak = true is printed on the next line with line break afterwards.
       */
      virtual void verbose( const std::string &message , bool bLineBreak = true ) const;

      /**
       * Reset the output stream (default is cout).
       * If this function is called the output stream is redirected for ALL logger instances.
       * @param   os  Output stream to use.
       */
      static void setOutputStream( std::ostream &os );

      /**
       * Reset the logging level of ALL logger instances.
       * @param   level   Logging level to use.
       * @see     Level
       */
      static void setLevel( Level level );

      /**
       * Reset the logging level of ALL logger instances.
       * @param   iLevel  Logging level to use.
       */
      static void setLevel( int iLevel );

      /**
       * Get the currently used logging level.
       * @return  The currently used logging level.
       */
      static Level getLevel();

    protected:

      /**
       * Log a message.
       * @param   level       Level id.
       * @param   message     Message to log.
       * @param   bLineBreak  Tell the logger if a line break should be added to the end of the message.
       *                      If this value is false, the message is flushed without line break and the cursor is sought back to beginning of the line.
       *                      The following messages with bLineBreak = false are all printed to the same line and will thus overwrite the previous message.
       *                      The first message with bLineBreak = true is printed on the next line with line break afterwards.
       */
      virtual void print( const std::string &level , const std::string &message , bool bLineBreak ) const;

      /**
       * Print the logging prefix.
       * @param   os      Output stream.
       * @param   level   Level id.
       * @return  Number of characters written.
       */
      virtual int printPrefix( std::ostream &os , const std::string &level ) const;

      /**
       * Convert logging level to appropriate integer.
       * @param   level Logging level.
       * @return  Integer representing the queried logging level.
       */
      static int level2int( Level level );

      /**
       * Convert integer to appropriate logging level.
       * @param   iLevel  Logging level represented as integer.
       * @return  Logging level.
       */
      static Level int2level( int iLevel );

      /**
       * Logging level used by all logging instances.
       */
      static int m_iLoggingLevel;

      /**
       * Indicates whether the last log was printed without line break.
       */
      static bool m_bWasLastLineNoLineBreak;

      /**
       * Id of the logger instance.
       */
      std::string m_id;

      /**
       * Output stream used by all logger instances.
       */
      static std::ostream *m_pGlobalStream;

    };

  }
}

#endif /* _G_IO_LOGGER_H_ */
