#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <dbus/dbus.h>
#include "dbus/dbus-sysdeps.h"

int
main (int argc, char *argv[])
{
  DBusConnection *conn = NULL;
  DBusError error;

  _dbus_setenv ("DBUS_SESSION_BUS_ADDRESS", NULL);

  dbus_error_init (&error);

  conn = dbus_bus_get (DBUS_BUS_SESSION, &error);
  if (dbus_error_is_set (&error))
    {
      fprintf (stderr, "*** Failed to autolaunch session bus: %s\n",
               error.message);
      dbus_error_free (&error);
      return 1;
    }

  return 0;
}
