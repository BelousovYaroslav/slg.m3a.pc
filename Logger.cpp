// Logger.cpp: implementation of the CLogger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logger.h"
#include "slg2.h"


extern CSlg2App theApp;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//0 - TRACE
//1 - DEBUG
//2 - INFO
//3 - WARNING
//4 - ERROR
//5 - FATAL
//6 - NONE

CLogger::CLogger()
{
  m_fh = NULL; 
}

CLogger::~CLogger()
{
  if( m_fh != NULL)
    fclose( m_fh);

  m_fh = NULL;
}



void CLogger::LogTrace( const char *strMessage, ...)
{
  if( theApp.GetLogLevel() != 0) return;
  
  SYSTEMTIME lt;
  GetLocalTime(&lt);
  if( m_fh != NULL)
    fprintf( m_fh, "%d.%02d.%02d %02d:%02d:%02d.%03d [TRACE] ", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

  va_list arglist;
  va_start( arglist, strMessage);
  
  if( m_fh != NULL)
    vfprintf( m_fh, strMessage, arglist);
  va_end( arglist);

  if( m_fh != NULL) {
    fprintf( m_fh, "\n");
    fflush( m_fh);
  }
}

void CLogger::LogDebug( const char *strMessage, ...)
{
  if( theApp.GetLogLevel() > 1) return;

  SYSTEMTIME lt;
  GetLocalTime(&lt);
  
  if( m_fh != NULL)
    fprintf( m_fh, "%d.%02d.%02d %02d:%02d:%02d.%03d [DEBUG] ", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

  va_list arglist;
  va_start( arglist, strMessage);
  if( m_fh != NULL)
    vfprintf( m_fh, strMessage, arglist);
  va_end( arglist);

  if( m_fh != NULL) {
    fprintf( m_fh, "\n");
    fflush( m_fh);
  }
}

void CLogger::LogInfo( const char *strMessage, ...)
{
  if( theApp.GetLogLevel() > 2) return;

  SYSTEMTIME lt;
  GetLocalTime(&lt);

  if( m_fh != NULL)
    fprintf( m_fh, "%d.%02d.%02d %02d:%02d:%02d.%03d [INFO ] ", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

  va_list arglist;
  va_start( arglist, strMessage);
  
  if( m_fh != NULL)
    vfprintf( m_fh, strMessage, arglist);
  va_end( arglist);

  if( m_fh != NULL) {
    fprintf( m_fh, "\n");
    fflush( m_fh);
  }
}

void CLogger::LogWarn( const char *strMessage, ...)
{
  if( theApp.GetLogLevel() > 3) return;

  SYSTEMTIME lt;
  GetLocalTime(&lt);
  
  if( m_fh != NULL)
    fprintf( m_fh, "%d.%02d.%02d %02d:%02d:%02d.%03d [WARN ] ", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

  va_list arglist;
  va_start( arglist, strMessage);
  if( m_fh != NULL)
    vfprintf( m_fh, strMessage, arglist);
  va_end( arglist);

  if( m_fh != NULL) {
    fprintf( m_fh, "\n");
    fflush( m_fh);
  }
}

void CLogger::LogError( const char *strMessage, ...)
{
  if( theApp.GetLogLevel() > 4) return;

  SYSTEMTIME lt;
  GetLocalTime(&lt);
  if( m_fh != NULL)
    fprintf( m_fh, "%d.%02d.%02d %02d:%02d:%02d.%03d [ERROR] ", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

  va_list arglist;
  va_start( arglist, strMessage);
  if( m_fh != NULL)
    vfprintf( m_fh, strMessage, arglist);
  va_end( arglist);

  if( m_fh != NULL) {
    fprintf( m_fh, "\n");
    fflush( m_fh);
  }
}

void CLogger::LogFatal( const char *strMessage, ...)
{
  if( theApp.GetLogLevel() > 5) return;

  SYSTEMTIME lt;
  GetLocalTime(&lt);
  if( m_fh != NULL)
    fprintf( m_fh, "%d.%02d.%02d %02d:%02d:%02d.%03d [FATAL] ", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

  va_list arglist;
  va_start( arglist, strMessage);
  if( m_fh != NULL)
    vfprintf( m_fh, strMessage, arglist);
  va_end( arglist);

  if( m_fh != NULL) {
    fprintf( m_fh, "\n");
    fflush( m_fh);
  }
}

/*
void CLogger::LogMsg( int nLevel, CString strMessage)
{
  switch( nLevel) {
    case 0: LogTrace( strMessage); break;
    case 1: LogDebug( strMessage); break;
    case 2: LogInfo(  strMessage); break;
    case 3: LogWarn(  strMessage); break;
    case 4: LogError( strMessage); break;
    case 5: LogFatal( strMessage); break;
  }
}
*/

void CLogger::Init( void)
{
  if( theApp.GetLogLevel() > -1 && theApp.GetLogLevel() < 6) {

    SYSTEMTIME lt;
    GetLocalTime(&lt);
    
    //printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
    //printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);

    CString m_strLogFileName;
    m_strLogFileName.Format( _T("%d_%02d_%02d.log"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);
  
    m_fh = fopen( m_strLogFileName, "a");
  
    if( m_fh != NULL) {
      fprintf( m_fh, "\n\n\n%d.%02d.%02d %02d:%02d:%02d [TRACE] Logging starts\n", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);
      fflush( m_fh);
    }
  }
  else {
    m_fh = NULL;
  }
}
