/*-------------------------------------------------------------------------
 *
 * syslogger.h
 *	  Exports from postmaster/syslogger.c.
 *
 * Copyright (c) 2004-2021, PostgreSQL Global Development Group
 *
 * src/include/postmaster/syslogger.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef _SYSLOGGER_H
#define _SYSLOGGER_H

#include <limits.h>				/* for PIPE_BUF */


/*
 * Primitive protocol structure for writing to syslogger pipe(s).  The idea
 * here is to divide long messages into chunks that are not more than
 * PIPE_BUF bytes long, which according to POSIX spec must be written into
 * the pipe atomically.  The pipe reader then uses the protocol headers to
 * reassemble the parts of a message into a single string.  The reader can
 * also cope with non-protocol data coming down the pipe, though we cannot
 * guarantee long strings won't get split apart.
 *
 * We use non-nul bytes in is_last to make the protocol a tiny bit
 * more robust against finding a false double nul byte prologue. But
 * we still might find it in the len and/or pid bytes unless we're careful.
 */

#ifdef PIPE_BUF
/* Are there any systems with PIPE_BUF > 64K?  Unlikely, but ... */
#if PIPE_BUF > 65536
#define PIPE_CHUNK_SIZE  65536
#else
#define PIPE_CHUNK_SIZE  ((int) PIPE_BUF)
#endif
#else							/* not defined */
/* POSIX says the value of PIPE_BUF must be at least 512, so use that */
#define PIPE_CHUNK_SIZE  512
#endif

typedef struct
{
	char		nuls[2];		/* always \0\0 */
	uint16		len;			/* size of this chunk (counts data only) */
	int32		pid;			/* writer's pid */
	char		is_last;		/* last chunk of message? 't' or 'f' ('T' or
								 * 'F' for CSV case) */
	char		data[FLEXIBLE_ARRAY_MEMBER];	/* data payload starts here */
} PipeProtoHeader;

typedef union
{
	PipeProtoHeader proto;
	char		filler[PIPE_CHUNK_SIZE];
} PipeProtoChunk;

#define PIPE_HEADER_SIZE  offsetof(PipeProtoHeader, data)
#define PIPE_MAX_PAYLOAD  ((int) (PIPE_CHUNK_SIZE - PIPE_HEADER_SIZE))


/* GUC options */
extern __thread bool Logging_collector;
extern __thread int	Log_RotationAge;
extern __thread int	Log_RotationSize;
extern __thread PGDLLIMPORT char *Log_directory;
extern __thread PGDLLIMPORT char *Log_filename;
extern __thread bool Log_truncate_on_rotation;
extern __thread int	Log_file_mode;

#ifndef WIN32
extern __thread int	syslogPipe[2];
#else
extern __thread HANDLE syslogPipe[2];
#endif


extern int	SysLogger_Start(void);

extern void write_syslogger_file(const char *buffer, int count, int dest);

#ifdef EXEC_BACKEND
extern void SysLoggerMain(int argc, char *argv[]) pg_attribute_noreturn();
#endif

extern bool CheckLogrotateSignal(void);
extern void RemoveLogrotateSignalFiles(void);

/*
 * Name of files saving meta-data information about the log
 * files currently in use by the syslogger
 */
#define LOG_METAINFO_DATAFILE  "current_logfiles"
#define LOG_METAINFO_DATAFILE_TMP  LOG_METAINFO_DATAFILE ".tmp"

#endif							/* _SYSLOGGER_H */
