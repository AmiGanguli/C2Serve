/**

 DateTimeLogger.h
 
 Created on: 	  02.10.2010
 Author: 			  Kay Wolter
 E-Mail:				wolter.kay@freenet.de

 @brief Logger that prints time and date information in addition to the prefix created by g::io::Logger.

 */

#ifndef _G_IO_DATETIMELOGGER_H_
#define _G_IO_DATETIMELOGGER_H_

#include "Logger.h"

namespace g
{
  namespace io
  {

    /**
     * Logger that prints date and time information in addition to the prefix created by g::io::Logger.
     * @see g::io::Logger
     */
    class DateTimeLogger: public g::io::Logger
    {
    public:

      /**
       * Constructor.
       * @param   id  String that describes the context of the logger instance.
       */
      DateTimeLogger( const std::string &id );

      /**
       * Destructor.
       */
      virtual ~DateTimeLogger();

      /**
       * Enable/disable logging of milliseconds in the prefix for ALL date/time logger instances.
       * @param bShowMilliSeconds   If true, milliseconds will be displayed.
       */
      static void setShowMilliSeconds( bool bShowMilliSeconds );

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

    private:

      /**
       * Get the current system date and time represented as string.
       * @return  System time.
       */
      static std::string getCurrentDateAndTimeAsString();

      /**
       * Indicates if milliseconds will be displayed in the logging prefix.
       */
      static bool m_bShowMilliSec;

    };

  }
}

#endif /* _G_IO_DATETIMELOGGER_H_ */
