/* Generated file (by generator) */

/*
 * Copyright (C) 2015 William Yu <williamyu@gnome.org>
 *
 * This library is free software: you can redistribute it and/or modify it
 * under the terms of version 2.1. of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined (__LIBICAL_GLIB_H_INSIDE__) && !defined (LIBICAL_GLIB_COMPILATION)
#error "Only <libical-glib/libical-glib.h> can be included directly."
#endif

#ifndef I_CAL_REQSTAT_H
#define I_CAL_REQSTAT_H

#include <libical-glib/i-cal-forward-declarations.h>
#include <libical-glib/i-cal-object.h>
#include <libical-glib/i-cal-enums.h>

G_BEGIN_DECLS


#define I_CAL_TYPE_REQSTAT \
    (i_cal_reqstat_get_type ())
LIBICAL_ICAL_EXPORT
G_DECLARE_DERIVABLE_TYPE(ICalReqstat, i_cal_reqstat, I_CAL, REQSTAT, ICalObject)

/**
 * ICalReqstat:
 *
 * This is the ICalReqstat instance.
 */

/**
 * ICalReqstatClass:
 *
 * This is the ICalReqstat class.
 */
struct _ICalReqstatClass {
    /*< private >*/
    ICalObjectClass parent;
};

LIBICAL_ICAL_EXPORT
ICalReqstat *	i_cal_reqstat_new_from_string	(const gchar *str);

LIBICAL_ICAL_EXPORT
gchar *		i_cal_reqstat_to_string		(ICalReqstat *stat);

LIBICAL_ICAL_EXPORT
ICalRequestStatus 
		i_cal_reqstat_get_code		(ICalReqstat *reqstat);

LIBICAL_ICAL_EXPORT
void		i_cal_reqstat_set_code		(ICalReqstat *reqstat,
						 ICalRequestStatus code);

LIBICAL_ICAL_EXPORT
const gchar *	i_cal_reqstat_get_desc		(const ICalReqstat *reqstat);

LIBICAL_ICAL_EXPORT
const gchar *	i_cal_reqstat_get_debug		(const ICalReqstat *reqstat);

G_END_DECLS

#endif /* I_CAL_REQSTAT_H */
