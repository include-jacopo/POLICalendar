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

#ifndef I_CAL_RECUR_ITERATOR_H
#define I_CAL_RECUR_ITERATOR_H

#include <libical-glib/i-cal-forward-declarations.h>
#include <libical-glib/i-cal-object.h>

G_BEGIN_DECLS


#define I_CAL_TYPE_RECUR_ITERATOR \
    (i_cal_recur_iterator_get_type ())
LIBICAL_ICAL_EXPORT
G_DECLARE_DERIVABLE_TYPE(ICalRecurIterator, i_cal_recur_iterator, I_CAL, RECUR_ITERATOR, ICalObject)

/**
 * ICalRecurIterator:
 *
 * This is the ICalRecurIterator instance.
 */

/**
 * ICalRecurIteratorClass:
 *
 * This is the ICalRecurIterator class.
 */
struct _ICalRecurIteratorClass {
    /*< private >*/
    ICalObjectClass parent;
};

LIBICAL_ICAL_EXPORT
ICalRecurIterator *
		i_cal_recur_iterator_new	(ICalRecurrence *rule,
						 ICalTime *dtstart);

LIBICAL_ICAL_EXPORT
ICalTime *	i_cal_recur_iterator_next	(ICalRecurIterator *iterator);

LIBICAL_ICAL_EXPORT
gint 		i_cal_recur_iterator_set_start	(ICalRecurIterator *iterator,
						 ICalTime *start);

LIBICAL_ICAL_EXPORT
gint 		i_cal_recur_iterator_set_end	(ICalRecurIterator *iterator,
						 ICalTime *end);

LIBICAL_ICAL_EXPORT
void		i_cal_recur_iterator_free	(ICalRecurIterator *iterator);

G_END_DECLS

#endif /* I_CAL_RECUR_ITERATOR_H */
