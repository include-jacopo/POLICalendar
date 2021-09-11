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

#ifndef I_CAL_COMP_ITER_H
#define I_CAL_COMP_ITER_H

#include <libical-glib/i-cal-forward-declarations.h>
#include <libical-glib/i-cal-object.h>

G_BEGIN_DECLS


#define I_CAL_TYPE_COMP_ITER \
    (i_cal_comp_iter_get_type ())
LIBICAL_ICAL_EXPORT
G_DECLARE_DERIVABLE_TYPE(ICalCompIter, i_cal_comp_iter, I_CAL, COMP_ITER, ICalObject)

/**
 * ICalCompIter:
 *
 * This is the ICalCompIter instance.
 */

/**
 * ICalCompIterClass:
 *
 * This is the ICalCompIter class.
 */
struct _ICalCompIterClass {
    /*< private >*/
    ICalObjectClass parent;
};

G_END_DECLS

#endif /* I_CAL_COMP_ITER_H */
