/* Gearman server and library
 * Copyright (C) 2008 Brian Aker, Eric Day
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license.  See
 * the COPYING file in the parent directory for full text.
 */

/**
 * @file
 * @brief Gearman core definitions
 */

#include "common.h"
#include "zlib.h"

/*
 * Private declarations
 */

/**
 * @addtogroup gearman_private Private Functions
 * @ingroup gearman
 * @{
 */

/**
 * Names of the verbose levels provided.
 */
static const char *_verbose_name[GEARMAN_VERBOSE_MAX]=
{
  "FATAL",
  "ERROR",
  "INFO",
  "DEBUG",
  "CRAZY"
};

/** @} */

/*
 * Public definitions
 */

const char *gearman_version(void)
{
    return PACKAGE_VERSION;
}

const char *gearman_bugreport(void)
{
    return PACKAGE_BUGREPORT;
}

const char *gearman_verbose_name(gearman_verbose_t verbose)
{
  if (verbose >= GEARMAN_VERBOSE_MAX)
    return "UNKNOWN";

  return _verbose_name[verbose];
}

gearman_st *gearman_create(gearman_st *gearman)
{
  if (gearman == NULL)
  {
    gearman= malloc(sizeof(gearman_st));
    if (gearman == NULL)
      return NULL;

    gearman->options= GEARMAN_ALLOCATED;
  }
  else
    gearman->options= 0;

  gearman->verbose= 0;
  gearman->con_count= 0;
  gearman->packet_count= 0;
  gearman->pfds_size= 0;
  gearman->sending= 0;
  gearman->last_errno= 0;
  gearman->timeout= -1;
  gearman->con_list= NULL;
  gearman->packet_list= NULL;
  gearman->pfds= NULL;
  gearman->log_fn= NULL;
  gearman->log_context= NULL;
  gearman->event_watch_fn= NULL;
  gearman->event_watch_context= NULL;
  gearman->workload_malloc_fn= NULL;
  gearman->workload_malloc_context= NULL;
  gearman->workload_free_fn= NULL;
  gearman->workload_free_context= NULL;
  gearman->last_error[0]= 0;
  gearman->reconnect_interval = GEARMAN_DEFAULT_RECONNECT_INTERVAL;
  gearman->socket_timeout = GEARMAN_DEFAULT_SOCKET_TIMEOUT;
  return gearman;
}

gearman_st *gearman_clone(gearman_st *gearman, const gearman_st *from)
{
  gearman_con_st *con;

  gearman= gearman_create(gearman);
  if (gearman == NULL)
    return NULL;

  gearman->options|= (from->options & (gearman_options_t)~GEARMAN_ALLOCATED);
  gearman->timeout= from->timeout;
  gearman->socket_timeout = from->socket_timeout;
  gearman->reconnect_interval = from->reconnect_interval;
  for (con= from->con_list; con != NULL; con= con->next)
  {
    if (gearman_con_clone(gearman, NULL, con) == NULL)
    {
      gearman_free(gearman);
      return NULL;
    }
  }

  /* Don't clone job or packet information, this is state information for
     old and active jobs/connections. */

  return gearman;
}

void gearman_free(gearman_st *gearman)
{
  gearman_con_free_all(gearman);
  gearman_packet_free_all(gearman);

  if (gearman->pfds != NULL)
    free(gearman->pfds);

  if (gearman->options & GEARMAN_ALLOCATED)
    free(gearman);
}

const char *gearman_error(const gearman_st *gearman)
{
  return (const char *)(gearman->last_error);
}

int gearman_errno(const gearman_st *gearman)
{
  return gearman->last_errno;
}

gearman_options_t gearman_options(const gearman_st *gearman)
{
  return gearman->options;
}

void gearman_set_options(gearman_st *gearman, gearman_options_t options)
{
  gearman->options= options;
}

void gearman_add_options(gearman_st *gearman, gearman_options_t options)
{
  gearman->options|= options;
}

void gearman_remove_options(gearman_st *gearman, gearman_options_t options)
{
  gearman->options&= ~options;
}

int gearman_timeout(gearman_st *gearman)
{
  return gearman->timeout;
}

void gearman_set_timeout(gearman_st *gearman, int timeout)
{
  gearman->timeout= timeout;
}

int gearman_socket_timeout(gearman_st *gearman)
{
    return gearman->socket_timeout;
}

void gearman_set_socket_timeout(gearman_st *gearman, int socket_timeout)
{
    gearman->socket_timeout= socket_timeout;
}

int gearman_reconnect_interval(gearman_st *gearman)
{
    return gearman->reconnect_interval;
}

void gearman_set_reconnect_interval(gearman_st *gearman, int reconnect_interval)
{
    gearman->reconnect_interval= reconnect_interval;
}

void gearman_set_log_fn(gearman_st *gearman, gearman_log_fn *function,
                        const void *context, gearman_verbose_t verbose)
{
  gearman->log_fn= function;
  gearman->log_context= context;
  gearman->verbose= verbose;
}

void gearman_set_event_watch_fn(gearman_st *gearman,
                                gearman_event_watch_fn *function,
                                const void *context)
{
  gearman->event_watch_fn= function;
  gearman->event_watch_context= context;
}

void gearman_set_workload_malloc_fn(gearman_st *gearman,
                                    gearman_malloc_fn *function,
                                    const void *context)
{
  gearman->workload_malloc_fn= function;
  gearman->workload_malloc_context= context;
}

void gearman_set_workload_free_fn(gearman_st *gearman,
                                  gearman_free_fn *function,
                                  const void *context)
{
  gearman->workload_free_fn= function;
  gearman->workload_free_context= context;
}

/*
 * Connection related functions.
 */

gearman_con_st *gearman_con_create(gearman_st *gearman, gearman_con_st *con)
{
  if (con == NULL)
  {
    con= malloc(sizeof(gearman_con_st));
    if (con == NULL)
    {
      gearman_error_set(gearman, "gearman_con_create", "malloc");
      return NULL;
    }

    con->options= GEARMAN_CON_ALLOCATED;
  }
  else
    con->options= 0;

  con->state= 0;
  con->send_state= 0;
  con->recv_state= 0;
  con->port= 0;
  con->events= 0;
  con->revents= 0;
  con->fd= -1;
  con->created_id= 0;
  con->created_id_next= 0;
  con->send_buffer_size= 0;
  con->send_data_size= 0;
  con->send_data_offset= 0;
  con->recv_buffer_size= 0;
  con->recv_data_size= 0;
  con->recv_data_offset= 0;
  con->gearman= gearman;

  if (gearman->con_list != NULL)
    gearman->con_list->prev= con;
  con->next= gearman->con_list;
  con->prev= NULL;
  gearman->con_list= con;
  gearman->con_count++;

  con->context= NULL;
  con->addrinfo= NULL;
  con->addrinfo_next= NULL;
  con->send_buffer_ptr= con->send_buffer;
  con->recv_packet= NULL;
  con->recv_buffer_ptr= con->recv_buffer;
  con->protocol_context= NULL;
  con->protocol_context_free_fn= NULL;
  con->packet_pack_fn= gearman_packet_pack;
  con->packet_unpack_fn= gearman_packet_unpack;
  con->host[0]= 0;
  con->reconnect_time = time(0);
  return con;
}

gearman_con_st *gearman_con_add(gearman_st *gearman, gearman_con_st *con,
                                const char *host, in_port_t port)
{
  con= gearman_con_create(gearman, con);
  if (con == NULL)
    return NULL;

  gearman_con_set_host(con, host);
  gearman_con_set_port(con, port);

  return con;
}

gearman_con_st *gearman_con_clone(gearman_st *gearman, gearman_con_st *con,
                                  const gearman_con_st *from)
{
  con= gearman_con_create(gearman, con);
  if (con == NULL)
    return NULL;

  con->options|= (from->options &
                  (gearman_con_options_t)~GEARMAN_CON_ALLOCATED);
  strcpy(con->host, from->host);
  con->port= from->port;

  return con;
}

void gearman_con_free(gearman_con_st *con)
{
  if (con->fd != -1)
    gearman_con_close(con, false);

  gearman_con_reset_addrinfo(con);

  if (con->protocol_context != NULL && con->protocol_context_free_fn != NULL)
    (*con->protocol_context_free_fn)(con, (void *)con->protocol_context);

  if (con->gearman->con_list == con)
    con->gearman->con_list= con->next;
  if (con->prev != NULL)
    con->prev->next= con->next;
  if (con->next != NULL)
    con->next->prev= con->prev;
  con->gearman->con_count--;

  if (con->options & GEARMAN_CON_PACKET_IN_USE)
    gearman_packet_free(&(con->packet));

  if (con->options & GEARMAN_CON_ALLOCATED)
    free(con);
}

void gearman_con_free_all(gearman_st *gearman)
{
  while (gearman->con_list != NULL)
    gearman_con_free(gearman->con_list);
}

gearman_return_t gearman_con_flush_all(gearman_st *gearman)
{
  gearman_con_st *con;
  gearman_return_t ret;

  for (con= gearman->con_list; con != NULL; con= con->next)
  {
    if (con->events & POLLOUT)
      continue;

    ret= gearman_con_flush(con);
    if (ret != GEARMAN_SUCCESS && ret != GEARMAN_IO_WAIT)
      return ret;
  }

  return GEARMAN_SUCCESS;
}

gearman_return_t gearman_con_send_all(gearman_st *gearman,
                                      const gearman_packet_st *packet)
{
  gearman_return_t ret;
  gearman_con_st *con;
  gearman_options_t options= gearman->options;

  gearman->options|= GEARMAN_NON_BLOCKING;

  if (gearman->sending == 0)
  {
    for (con= gearman->con_list; con != NULL; con= con->next)
    {
      ret= gearman_con_send(con, packet, true);
      if (ret != GEARMAN_SUCCESS)
      {
        if (ret != GEARMAN_IO_WAIT)
        {
          gearman->options= options;
          return ret;
        }

        gearman->sending++;
        break;
      }
    }
  }

  while (gearman->sending != 0)
  {
    while ((con= gearman_con_ready(gearman)) != NULL)
    {
      ret= gearman_con_send(con, packet, true);
      if (ret != GEARMAN_SUCCESS)
      {
        if (ret != GEARMAN_IO_WAIT)
        {
          gearman->options= options;
          return ret;
        }

        continue;
      }

      gearman->sending--;
    }

    if (gearman->sending == 0)
      break;

    if (options & GEARMAN_NON_BLOCKING)
    {
      gearman->options= options;
      return GEARMAN_IO_WAIT;
    }

    ret= gearman_con_wait(gearman);
    if (ret != GEARMAN_SUCCESS)
    {
      gearman->options= options;
      return ret;
    }
  }

  gearman->options= options;
  return GEARMAN_SUCCESS;
}

// 0 ~ return immediately
// positive x ~ wait of x milliseconds 
// negative x ~ as timeout
gearman_return_t gearman_con_single_wait(gearman_con_st *con, int x)
{
  struct pollfd pfds[1];
  int ret;
  gearman_return_t gret;

  if (con->events == 0)
  {
    return GEARMAN_NO_ACTIVE_FDS;
  }
  
  pfds[0].fd= con->fd;
  pfds[0].events= con->events;
  pfds[0].revents= 0;

  while (1)
  {
    //only wait 3 seconds for the response. Solve some sequence problem.
    int timeout = 0;
    if (x < 0)
    {
      timeout = con->gearman->timeout;
      if (timeout < 0)
        timeout = 3000;
    }
    else if (x > 0)
      timeout = x;
    ret= poll(pfds, 1,  timeout);
    if (ret == -1)
    {
      if (errno == EINTR)
        continue;

      gearman_error_set(con->gearman, "gearman_con_single_wait", "poll:%d", errno);
      con->gearman->last_errno= errno;
      return GEARMAN_ERRNO;
    }

    break;
  }

  if (ret == 0)
  {
    gearman_error_set(con->gearman, "gearman_con_wait", "timeout reached");
    return GEARMAN_TIMEOUT;
  }

  gret= gearman_con_set_revents(con, pfds[0].revents);
  if (gret != GEARMAN_SUCCESS)
    return gret;

  return GEARMAN_SUCCESS;
}

gearman_return_t gearman_con_wait(gearman_st *gearman)
{
  gearman_con_st *con;
  struct pollfd *pfds;
  nfds_t x;
  int ret;
  gearman_return_t gret;
  if (gearman->pfds_size < gearman->con_count)
  {
    pfds= realloc(gearman->pfds, gearman->con_count * sizeof(struct pollfd));
    if (pfds == NULL)
    {
      gearman_error_set(gearman, "gearman_con_wait", "realloc");
      return GEARMAN_MEMORY_ALLOCATION_FAILURE;
    }

    gearman->pfds= pfds;
    gearman->pfds_size= gearman->con_count;
  }
  else
    pfds= gearman->pfds;

  x= 0;
  for (con= gearman->con_list; con != NULL; con= con->next)
  {
    if (con->events == 0)
      continue;
    pfds[x].fd= con->fd;
    pfds[x].events= con->events;
    pfds[x].revents= 0;
    x++;
  }

  if (x == 0)
  {
    gearman_error_set(gearman, "gearman_con_wait",
                      "no active file descriptors");
    return GEARMAN_NO_ACTIVE_FDS;
  }

  while (1)
  {
    ret= poll(pfds, x, gearman->timeout);
    if (ret == -1)
    {
      if (errno == EINTR)
        continue;

      gearman_error_set(gearman, "gearman_con_wait", "poll:%d", errno);
      gearman->last_errno= errno;
      return GEARMAN_ERRNO;
    }

    break;
  }

  if (ret == 0)
  {
    gearman_error_set(gearman, "gearman_con_wait", "timeout reached");
    return GEARMAN_TIMEOUT;
  }

  x= 0;
  for (con= gearman->con_list; con != NULL; con= con->next)
  {
    if (con->events == 0)
      continue;

    gret= gearman_con_set_revents(con, pfds[x].revents);
    if (gret != GEARMAN_SUCCESS)
      return gret;

    x++;
  }

  return GEARMAN_SUCCESS;
}

gearman_con_st *gearman_con_ready(gearman_st *gearman)
{
  gearman_con_st *con;

  /* We can't keep state between calls since connections may be removed during
     processing. If this list ever gets big, we may want something faster. */
/*static uint32_t idx=0;
uint32_t x = 0;

if (gearman->con_count == 0)
  return NULL;
idx = (idx + 1) % gearman->con_count;

for (x=0, con= gearman->con_list; x < idx; x++)
{
  con= con->next;
}

if ( con ==NULL || (con->options & GEARMAN_CON_READY))
{
  for (con= gearman->con_list; con != NULL; con= con->next)
  {
    if (con->options & GEARMAN_CON_READY)
    {
      con->options&= (gearman_con_options_t)~GEARMAN_CON_READY;
      return con;
    }
  }
}

if (con == NULL)
{*/
  for (con= gearman->con_list; con != NULL; con= con->next)
  {
    if (con->options & GEARMAN_CON_READY)
    {
      con->options&= (gearman_con_options_t)~GEARMAN_CON_READY;
      return con;
    }
  }

//}
  return NULL;
}

gearman_return_t gearman_con_echo(gearman_st *gearman, const void *workload,
                                  size_t workload_size)
{
  gearman_con_st *con;
  gearman_options_t options= gearman->options;
  gearman_packet_st packet;
  gearman_return_t ret;

  ret= gearman_packet_add(gearman, &packet, GEARMAN_MAGIC_REQUEST,
                          GEARMAN_COMMAND_ECHO_REQ, workload, workload_size,
                          NULL);
  if (ret != GEARMAN_SUCCESS)
    return ret;

  gearman->options&= (gearman_con_options_t)~GEARMAN_NON_BLOCKING;

  for (con= gearman->con_list; con != NULL; con= con->next)
  {
    ret= gearman_con_send(con, &packet, true);
    if (ret != GEARMAN_SUCCESS)
    {
      gearman_packet_free(&packet);
      gearman->options= options;
      return ret;
    }

    (void)gearman_con_recv(con, &(con->packet), &ret, true);
    if (ret != GEARMAN_SUCCESS)
    {
      gearman_packet_free(&packet);
      gearman->options= options;
      return ret;
    }

    if (con->packet.data_size != workload_size ||
        memcmp(workload, con->packet.data, workload_size))
    {
      gearman_packet_free(&(con->packet));
      gearman_packet_free(&packet);
      gearman->options= options;
      gearman_error_set(gearman, "gearman_con_echo", "corruption during echo");
      return GEARMAN_ECHO_DATA_CORRUPTION;
    }

    gearman_packet_free(&(con->packet));
  }

  gearman_packet_free(&packet);
  gearman->options= options;
  return GEARMAN_SUCCESS;
}

/*
 * Packet related functions.
 */

gearman_packet_st *gearman_packet_create(gearman_st *gearman,
                                         gearman_packet_st *packet)
{
  if (packet == NULL)
  {
    packet= malloc(sizeof(gearman_packet_st));
    if (packet == NULL)
    {
      gearman_error_set(gearman, "gearman_packet_create", "malloc");
      return NULL;
    }

    packet->options= GEARMAN_PACKET_ALLOCATED;
  }
  else
    packet->options= 0;

  packet->magic= 0;
  packet->command= 0;
  packet->argc= 0;
  packet->args_size= 0;
  packet->data_size= 0;
  packet->gearman= gearman;

  if (!(gearman->options & GEARMAN_DONT_TRACK_PACKETS))
  {
    if (gearman->packet_list != NULL)
      gearman->packet_list->prev= packet;
    packet->next= gearman->packet_list;
    packet->prev= NULL;
    gearman->packet_list= packet;
    gearman->packet_count++;
  }

  packet->args= NULL;
  packet->data= NULL;

  return packet;
}

gearman_return_t gearman_packet_add(gearman_st *gearman,
                                    gearman_packet_st *packet,
                                    gearman_magic_t magic,
                                    gearman_command_t command,
                                    const void *arg, ...)
{
  va_list ap;
  size_t arg_size;
  gearman_return_t ret;

  packet= gearman_packet_create(gearman, packet);
  if (packet == NULL)
    return GEARMAN_MEMORY_ALLOCATION_FAILURE;

  packet->magic= magic;
  packet->command= command;

  va_start(ap, arg);

  while (arg != NULL)
  {
    arg_size = va_arg(ap, size_t);
    ret= gearman_packet_add_arg(packet, arg, arg_size);
    if (ret != GEARMAN_SUCCESS)
    {
      va_end(ap);
      gearman_packet_free(packet);
      return ret;
    }

    arg = va_arg(ap, void *);
  }

  va_end(ap);

  return gearman_packet_pack_header(packet);
}

void gearman_packet_free(gearman_packet_st *packet)
{
  if (packet->args != packet->args_buffer && packet->args != NULL)
  {
    free(packet->args);
    packet->args = NULL;
  }

  if (packet->options & GEARMAN_PACKET_FREE_DATA && packet->data != NULL)
  {
    if (packet->gearman->workload_free_fn == NULL)
      free((void *)(packet->data));
    else
    {
      packet->gearman->workload_free_fn((void *)(packet->data),
                              (void *)(packet->gearman->workload_free_context));
    }
    packet->data = NULL;
  }

  if (!(packet->gearman->options & GEARMAN_DONT_TRACK_PACKETS))
  {
    if (packet->gearman->packet_list == packet)
      packet->gearman->packet_list= packet->next;
    if (packet->prev != NULL)
      packet->prev->next= packet->next;
    if (packet->next != NULL)
    {
      packet->next->prev= packet->prev;
      //YRF: to solve a dead loop when multi-thread client delete gearman_client together,
      //     Also it could be solve by adding a client thread lock, but currently there is
      //     NO such utility in gearman.
      if (packet->next == packet)
        packet->next = NULL;
      if (packet->next != NULL && packet->next->next == packet)
        packet->next->next = NULL;
      //Solve over
    }
    packet->gearman->packet_count--;
  }

  if (packet->options & GEARMAN_PACKET_ALLOCATED)
  {
    free(packet);
    packet = NULL;
  }
}

void gearman_packet_free_all(gearman_st *gearman)
{
  while (gearman->packet_list != NULL)
    gearman_packet_free(gearman->packet_list);
}

/*
 * Local package functions.
 */
void gearman_error_set(gearman_st *gearman, const char *function,
                       const char *format, ...)
{
  size_t length;
  char *ptr;
  char log_buffer[GEARMAN_MAX_ERROR_SIZE];
  va_list arg;

  va_start(arg, format);

  length= strlen(function);

  /* Copy the function name and : before the format */
  ptr= memcpy(log_buffer, function, length);
  ptr+= length;
  ptr[0]= ':';
  length++;
  ptr++;

  length+= (size_t)vsnprintf(ptr, GEARMAN_MAX_ERROR_SIZE - length, format, arg);

  if (gearman->log_fn == NULL)
  {
    if (length >= GEARMAN_MAX_ERROR_SIZE)
      length= GEARMAN_MAX_ERROR_SIZE - 1;

    memcpy(gearman->last_error, log_buffer, length + 1);
  }
  else
  {
    (*(gearman->log_fn))(log_buffer, GEARMAN_VERBOSE_FATAL,
                         (void *)(gearman)->log_context);
  }

  va_end(arg);
}

gearman_return_t gearman_parse_servers(const char *servers,
                                       gearman_parse_server_fn *function,
                                       const void *context)
{
  const char *ptr= servers;
  size_t x;
  char host[NI_MAXHOST];
  char port[NI_MAXSERV];
  gearman_return_t ret;

  if (ptr == NULL)
    return (*function)(NULL, 0, (void *)context);

  while (1)
  {
    x= 0;

    while (*ptr != 0 && *ptr != ',' && *ptr != ':')
    {
      if (x < (NI_MAXHOST - 1))
        host[x++]= *ptr;

      ptr++;
    }

    host[x]= 0;

    if (*ptr == ':')
    {
      ptr++;
      x= 0;

      while (*ptr != 0 && *ptr != ',')
      {
        if (x < (NI_MAXSERV - 1))
          port[x++]= *ptr;

        ptr++;
      }

      port[x]= 0;
    }
    else
      port[0]= 0;

    ret= (*function)(host, (in_port_t)atoi(port), (void *)context);
    if (ret != GEARMAN_SUCCESS)
      return ret;

    if (*ptr == 0)
      break;

    ptr++;
  }

  return GEARMAN_SUCCESS;
}

#define COMPRESS_HEAD_LENGTH 5

compress_ret
q_compress(const char* source, long source_len,
    char* dest, long* dest_len,
    compress_type type)
{

  Bytef* z_dest = (Bytef*)dest + COMPRESS_HEAD_LENGTH;
  uLong z_dest_len = *dest_len - COMPRESS_HEAD_LENGTH;
  const Bytef* z_source = (const Bytef*)source;
  uLong z_source_len = (uLong)source_len;

  compress_ret ret = COMPRESS_OK;
  int z_ret;

  if (z_dest_len < compressBound(source_len))
    return COMPRESS_BUFFER_ERROR;
  if (!dest || !source || source_len < 0 ||
      *dest_len < 0)
    return COMPRESS_INVALID_PARAMETER;

  switch (type) {
    case Z_DEFLATE:
      z_ret = compress(z_dest, 
          &z_dest_len,z_source,z_source_len);
      if (z_ret == Z_OK)
      {
        dest[0] = (char)type;
        for (int i = 1; i < COMPRESS_HEAD_LENGTH; i++)
        {
          dest[i] = source_len % 256;
          source_len /= 256;
        }
        *dest_len = z_dest_len + COMPRESS_HEAD_LENGTH;
      }
      else if (z_ret == Z_BUF_ERROR)
      {
        ret = COMPRESS_BUFFER_ERROR;
      }
      else
        ret = COMPRESS_FAILED;
      break;
    default:
      ret = COMPRESS_INVALID_TYPE;
  }
  return ret;
}

compress_ret
q_uncompress(const char* source, long source_len,
    char* dest, long* dest_len)
{
  compress_ret ret = COMPRESS_OK;

  if (!source || source_len <= COMPRESS_HEAD_LENGTH)
    return COMPRESS_INVALID_PARAMETER;

  //validate the format
  char type = source[0];
  switch (type) {
    case (char)Z_DEFLATE: 
      if (source_len < 7 || source[5] != (char)0x78 || source[6] != (char)0x9c)
        return COMPRESS_INVALID_FORMAT;
      break;
    default:
      return COMPRESS_INVALID_FORMAT;
  }

  uLong z_dest_len = 0;
  for (int i = 1; i < COMPRESS_HEAD_LENGTH; i++)
  {
    z_dest_len += (uLong)((unsigned char)source[i])<<8*(i-1);
  }

  //Customer could use dest==NULL or dest_len<=0 to check 
  //how large buffer they need to provide for 
  if (!dest || *dest_len <=0)
  {
    *dest_len = z_dest_len;
    return COMPRESS_OK;
  }

  if (*dest_len < (long)z_dest_len)
    return COMPRESS_BUFFER_ERROR;

  z_dest_len = *dest_len;
  Bytef* z_dest = (Bytef*)dest;
  const Bytef* z_source = (const Bytef*)source + COMPRESS_HEAD_LENGTH;
  uLong z_source_len = (uLong)source_len - COMPRESS_HEAD_LENGTH;
  int z_ret;

  switch (type) {
    case (char)Z_DEFLATE:
      z_ret = uncompress(z_dest, &z_dest_len, 
          z_source, z_source_len);

      if (z_ret == Z_OK)
        break;
      else if (z_ret == Z_BUF_ERROR)
      {
        ret = COMPRESS_BUFFER_ERROR;
      }
      else if (z_ret == Z_MEM_ERROR)
      {
        ret = COMPRESS_FAILED;
      } 
      else 
      {
        ret = COMPRESS_INVALID_FORMAT;
      }
      break;
    default:
      ret = COMPRESS_INVALID_FORMAT;
  }
  return ret;
}


long
q_compressBound(long source_len)
{
  return compressBound(source_len) + COMPRESS_HEAD_LENGTH;
}


