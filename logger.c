#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // date/time
#include <string.h>  // strcspn, remove new line character
#include <stdarg.h>  // format printf arguments

#define MAX_LOG_SIZE 200

// global
FILE* logger_fptr;

/**
 * @brief to be called before logger library use
 * 
 * @param filename 
 */
void logger_init(const char* filename){
   if (LOG_TO_FILE){
      logger_fptr = fopen(filename, "w+");
      fprintf(logger_fptr, "starting log\n");
   }
}

// to be called at end of program
void logger_finalize(){
   if (LOG_TO_FILE){
      fclose(logger_fptr);
   }
}


// actual logging thing
// assume new lines handled by user calling logger()
//    don't print new lines here
void logger(const char* tag, const char* format, ...) {
   // get string of current time
   time_t now;
   time(&now);
   char* timestamp = ctime(&now);

   // strip trailing new line character from timestamp
   timestamp[strcspn(timestamp, "\r\n")] = 0;

   // arbitrary character limit for log messages
   char log_message[MAX_LOG_SIZE]; 

   // add timestamp, tag, and format (aka the user message) to the log_message
   sprintf(log_message, "%s [%s]: %s", timestamp, tag, format);
   
   // some black magic to deal with printf arguments --> aka the (...) in the function arguments
   // https://stackoverflow.com/questions/1056411/how-to-pass-variable-number-of-arguments-to-printf-sprintf
   va_list args;
   if (LOG_TO_TERMINAL){
      va_start(args, format);
      vfprintf(stdout, log_message, args);   // send to terminal output
   }

   if (LOG_TO_FILE){
      // need to call va_start() 2x if logging both to terminal and file
      // I think it's cuz the print thing is destructive (I experimented)
      // cannot just move va_start() to outside of if statements
      va_start(args, format); // need to call va_start 2x if logging both to terminal and file, I think it's cuz the print thing is destructive
      vfprintf(logger_fptr, log_message, args);   // send to logfile
   }
   va_end(args);
}

void print_binary_32(uint32_t word) {
   logger("DEBUG", "bin = " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN ",\n",
          BYTE_TO_BINARY(word >> 24),
          BYTE_TO_BINARY(word >> 16),
          BYTE_TO_BINARY(word >> 8),
          BYTE_TO_BINARY(word));
}
