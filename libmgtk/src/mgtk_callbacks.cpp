/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Mongoose
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the mlisp bridge to gtk+ events 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.27:
 * Mongoose - Major API clean up, documentation update
 *
 * 2004.04.11:
 * Mongoose - Removed GNOME support, lots of little clean ups
 * 
 * 2002.01.25:
 * Mongoose - Spin spin sugar... replaced a hundred spin functions
 *            with generic calls
 *
 * 2002.01.12:
 * Mongoose - Toolkit functions for mapping widgets and events
 *            to use for external get/set actions
 *
 * 2000.09.16:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <mstl/Map.h>
#include <mstl/Vector.h>

#include "mgtk_callbacks.h"
#include "mgtk_interface.h"
#include "mgtk_resource.h"
#include "mgtk_events.h"
#include "mgtk_keys.h"


float spinbutton_value_get_float(int event, bool *error);
int spinbutton_uint_set_range(GtkSpinButton *spin, 
							  unsigned int value,
							  unsigned int min, unsigned int max);

/* Mongoose, 2004.10.27,
 * FIXME: There is a better way to map this events/widgets,
 *        than these lame gobal maps */
/* Mongoose 2002.02.01,
 * Gobals only accessed in this file */
Map<int, Vector<GtkWidget *> *> gWidgetMap;


//////////////////////////////////////////////////////////////////////////
// Widget <-> Event mapping system
//////////////////////////////////////////////////////////////////////////

void mgtk_event_subscribe_gtk_widget(int index, GtkWidget *widget)
{
	Vector<GtkWidget *> *widgets = gWidgetMap[index];

	//printf("mgtk_event_subscribe_gtk_widget(%d, %p)\n", index, widget);

	if (!widgets)
	{
		widgets = new Vector<GtkWidget *>;
		gWidgetMap.Add(index, widgets);
	}

	widgets->pushBack(widget);

	mgtk_print("(mgtk_event_subscribe_gtk_widget %d %p)", 
				//gObserverGtkWigets.NumItems(), index, widget, widgets->size()
				index, widget);
}


void mgtk_event_notify_observer1f(unsigned int id, float r)
{
	Vector<GtkWidget *> *widgets;
	GtkWidget *widget;
	unsigned int i;


	widgets = gWidgetMap[id];

	if (!widgets)
	{
		mgtk_print("mgtk_event_notify_observer1f> ERROR, unknown id %d",
					id);
		return;
	}

	mgtk_print("(mgtk_event_notify_observer1f %d %f)", id, r);

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		widget = (*widgets)[i];

		if (!widget)
			continue;

		if (GTK_IS_SPIN_BUTTON(widget))
		{
			GtkAdjustment *adj = gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(widget));
			
			// gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), r);
			
			if (adj)
			{
				gtk_adjustment_set_value(adj, r);
				mgtk_print("(mgtk_event_notify_gtk_widget %d %p)", 
							id, widget);
			}
		}
		else
		{
			mgtk_print("mgtk_event_notify_observer1f> ERROR, unknown widget\n");
			//	return;
		}
	}
}


void mgtk_event_dialog_visible_set(int dialog, int visible)
{
	Vector<GtkWidget*> *widgets;
	GtkWidget *test;
	unsigned int i;


	widgets = gWidgetMap[dialog];

	if (!widgets)
		return;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		test = (*widgets)[i];

		if (test && GTK_IS_WINDOW(test))
		{
			if (visible)
			{
				gtk_widget_show_all(test);
			}
			else
			{
				gtk_widget_hide_all(test);
			}
		}
		else
		{
			mgtk_print("mgtk_dialog_visible_set> %i:%d failed", dialog, i);
		}
	}
}


void mgtk_option_menu_value_set(int event, int index)
{
	Vector<GtkWidget*> *widgets;
	GtkWidget *test;
	unsigned int i;


	widgets = gWidgetMap[event];

	if (!widgets)
		return;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		test = (*widgets)[i];

		if (test && GTK_IS_OPTION_MENU(test))
		{
			gtk_option_menu_set_history(GTK_OPTION_MENU(test), index);
		}
		else
		{
			mgtk_print("mgtk_option_menu_value_set> %i:%d failed", event, i);
		}
	}
}


void mgtk_textentry_value_set(int event, const char *s)
{
	Vector<GtkWidget*> *widgets;
	GtkWidget *test;
	unsigned int i;


	widgets = gWidgetMap[event];

	if (!widgets)
		return;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		test = (*widgets)[i];

		if (test)// && GTK_IS_ENTRY(test))
		{
			gtk_entry_set_text(GTK_ENTRY(test), s);
		}
		else
		{
			mgtk_print("mgtk_textentry_value_set> %i:%d failed", event, i);
		}
	}
}


void mgtk_togglebutton_value_set(int event, bool val)
{
	Vector<GtkWidget*> *widgets;
	GtkWidget *test;
	unsigned int i;


	widgets = gWidgetMap[event];

	if (!widgets)
		return;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		test = (*widgets)[i];

		if (test && GTK_IS_TOGGLE_BUTTON(test))
		{
			if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(test)) != val ? TRUE : FALSE)
			{
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(test),  
											 val ? TRUE : FALSE);
			}
		}
		else
		{
			mgtk_print("mgtk_togglebutton_value_set> %i:%d failed", event, i);
		}
	}
}


// Mongoose 2002.01.31, Hack for adjusting widget values by event id
void mgtk_spinbutton_value_set(int event, float val)
{
	Vector<GtkWidget*> *widgets;
	GtkWidget *test;
	unsigned int i;


	widgets = gWidgetMap[event];

	if (!widgets)
		return;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		test = (*widgets)[i];

		if (test)// && GTK_IS_SPIN_BUTTON(test))
		{
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(test), val);
		}
		else
		{
			mgtk_print("mgtk_spinbutton_value_set> %i:%d failed", event, i);
		}
	}
}


// Mongoose 2002.01.31, Hack for adjusting widget values by event id
float spinbutton_value_get_float(int event, bool *error)
{
	Vector<GtkWidget*> *widgets;
	GtkWidget *test;
	unsigned int i;


	widgets = gWidgetMap[event];

	if (!widgets)
		return 0.0f;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		test = (*widgets)[i];
		*error = true;

		if (test && GTK_IS_SPIN_BUTTON(test))
		{
			*error = false;
			return gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(test));
		}
		else
		{
			mgtk_print("spinbutton_value_get_float> %i:%d failed", event, i);
		}
	}

	return 0.0f;
}


int spinbutton_value_get_int(int event, bool *error)
{
	Vector<GtkWidget*> *widgets;
	GtkWidget *test;
	unsigned int i;


	widgets = gWidgetMap[event];

	if (!widgets)
		return 0;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		test = (*widgets)[i];
		*error = true;
		
		if (test && GTK_IS_SPIN_BUTTON(test))
		{
			*error = false;
			return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(test));
		}
		else
		{
			mgtk_print("spinbutton_value_get_int> %i:%d failed", event, i);
		}
	}

	return 0;
}


int mgtk_event_set_range(int event, unsigned int value,
						 unsigned int min, unsigned int max)
{
	Vector<GtkWidget*> *widgets;
	GtkWidget *test;
	unsigned int i;


	widgets = gWidgetMap[event];

	if (!widgets)
		return -1;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		test = (*widgets)[i];

		if (test && GTK_IS_SPIN_BUTTON(test))
		{
			return spinbutton_uint_set_range(GTK_SPIN_BUTTON(test),
											 value, min, max);
		}
#define ADJUSTMENT_RANGE_TEST
#ifdef ADJUSTMENT_RANGE_TEST
		else if (test && GTK_IS_ADJUSTMENT(test))
		{
			gdouble oldMax = GTK_ADJUSTMENT(test)->upper;

			GTK_ADJUSTMENT(test)->upper = max;
			GTK_ADJUSTMENT(test)->lower = min;

			//if (value > max)
			//	GTK_ADJUSTMENT(test)->value = max;

			if (oldMax < value)
				return 1;

			return 0;
		}
#endif
		else
		{
			mgtk_print("mgtk_event_set_range> %i:%d failed", event, i);
		}
	}

	return -1;
}


void mgtk_gtk_event_fileselection_pattern(GtkWidget *widget, gpointer data)
{
#ifdef USE_OLD_FILE_SELECTION_WIDGET
	mgtk_event_fileselection_pattern((char*)data);
#else
#endif
}


void mgtk_event_fileselection_pattern(char *pattern)
{
#ifdef USE_OLD_FILE_SELECTION_WIDGET
	GtkWidget *file = mgtk_get_fileselection_widget();

	gtk_file_selection_complete(GTK_FILE_SELECTION(file), pattern);
#else
	GtkWidget *file = mgtk_get_fileselection_widget();
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(file), 
								gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(file)));
#endif
}


void mgtk_event_file_dialog(char *title)
{
	GtkWidget *file = mgtk_get_fileselection_widget();
	
	gtk_window_set_title(GTK_WINDOW(file), title);
	gtk_widget_show(file);
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

/* Shutdown / exit handlers */
void mgtk_event_shutdown()
{
/* Mongoose 2004.10.29, 
 * FIXME clean up gWidgetMap here too! */


	mgtk_print("@Gtk+ shuting down...");
	gtk_exit(0);
}

void mgtk_application_window_title(char *title)
{
	gtk_window_set_title(GTK_WINDOW(mgtk_get_application_window()), title);
}

void mgtk_destroy_window(GtkWidget *widget)
{
	mgtk_event_shutdown();
}


float mgtk_event_get_float(int event)
{
	float value;
	bool error;

	value = spinbutton_value_get_float(event, &error);
	return value;
}


void mgtk_event_swap_gl_buffers()
{
	/* Hhhmm...  nothing really needed here, swapping is handled in 
	 * the gtk glarea interfacing code after draw requests */
	usleep(250); // Hhmm... avoid back to back redraws give time to finish
}


void mgtk_event_gl_refresh()
{
	if (mgtk_get_gl_widget())
	{
		mgtk_refresh_glarea(mgtk_get_gl_widget());
	}
}


// FIXME: factor out key mapping from GDK and mgtk!
void mgtk_event_button_press(GtkWidget *widget, GdkEventButton *event)
{
	GdkModifierType state;
	mgtk_glarea_window_state_t *gl_state;
	int x, y, mod, btn_state, button;


	gl_state = (mgtk_glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(widget),
														   "gl_window_state");
	
	/* Beginning of drag, reset mouse position */
	gdk_window_get_pointer(event->window, &x, &y, &state);
  
	gl_state->mouse_x = x;
	gl_state->mouse_y = y;

	btn_state = MOUSE_BTN_STATE_PRESSED;
	button = 0;
	mod = 0;

	if (event->state & GDK_CONTROL_MASK)
		mod |= KEY_LCTRL;

	if (event->state & GDK_SHIFT_MASK)
		mod |= KEY_LSHIFT;

	if (state & GDK_BUTTON1_MASK)
	{
		button |= MOUSE_BTN_LEFT;
	}

	if (state & GDK_BUTTON2_MASK)
	{
		button |= MOUSE_BTN_MIDDLE;
	}

	if (state & GDK_BUTTON3_MASK)
	{
		button |= MOUSE_BTN_RIGHT;
	}

	if (state & GDK_BUTTON4_MASK)
	{
		button |= MOUSE_BTN_UP;
	}

	if (state & GDK_BUTTON5_MASK)
	{
		button |= MOUSE_BTN_DOWN;
	}

#ifdef DEBUG_GTK_MOUSE
	char foo[32];
	sprintf(foo, "MOUSE_BTN_%i", button);
	mgtk_print("Sending mouse event to Freya; x: %i y: %i, %s, %s\n", 
					x, y, 
					(button == MOUSE_BTN_LEFT) ? "MOUSE_BTN_LEFT" :
					(button == MOUSE_BTN_RIGHT) ? "MOUSE_BTN_RIGHT" :
					(button == MOUSE_BTN_MIDDLE) ? "MOUSE_BTN_MIDDLE" :
					(button == MOUSE_BTN_4) ? "MOUSE_BTN_4" :
					(button == MOUSE_BTN_5) ? "MOUSE_BTN_5" :
					foo, 
					(btn_state==MOUSE_BTN_STATE_PRESSED)?"MOUSE_BTN_STATE_PRESSED":
					"MOUSE_BTN_STATE_NONE");
#endif

	mgtk_handle_mouse(button, btn_state, mod, x, y);
	mgtk_refresh_glarea(widget); // Seems a little much
}


void mgtk_event_button_release(GtkWidget *widget, GdkEventButton *event)
{
	//mgtk_callback_mouse_button_release(); /* assumes no chording */

	/* Mongoose 2004.10.28, 
	 * FIXME Why is there a still a mouse call here? */
	mgtk_handle_mouse(0, 0, 0, 0, 0);
}


void mgtk_event_key_press(GtkWidget *widget, GdkEventKey *event)
{
	unsigned int mod = 0;
	int key = -1;


	//gtk_widget_add_accelerator(item, "activate_item", accel, 
	//                           tmp_key, GDK_MOD1_MASK, (GtkAccelFlags)0);

	if (event->state & GDK_CONTROL_MASK)
	{
		mod |= KEY_RCTRL;
	}

	if (event->state & GDK_SHIFT_MASK)
	{
		mod |= KEY_RSHIFT;
	}

	switch (event->keyval)
	{
	case GDK_Left:
		key = KEY_LEFT;
		break;
	case GDK_Right:
		key = KEY_RIGHT;
		break;
	case GDK_Up:
		key = KEY_UP;
		break;
	case GDK_space:
		key = KEY_SPACE;
		break;
	case GDK_Down:
		key = KEY_DOWN;
		break;
	case GDK_F1:
		key = KEY_F1;
		break;		
	case GDK_F2:
		key = KEY_F2;
		break;		
	case GDK_F3:
		key = KEY_F3;
		break;		
	case GDK_F4:
		key = KEY_F4;
		break;
	case GDK_F5:
		key = KEY_F5;
		break;		
	case GDK_F6:
		key = KEY_F6;
		break;		
	case GDK_F7:
		key = KEY_F7;
		break;
	case GDK_F8:
		key = KEY_F8;
		break;		
	case GDK_F9:
		key = KEY_F9;
		break;
	case GDK_F10:
		key = KEY_F10;
		break;		
	case GDK_F11:
		key = KEY_F11;
		break;		
	case GDK_F12:
		key = KEY_F12;
		break;
	default:
		if ((unsigned char)event->keyval < 128)
		{
			key = (unsigned char)event->keyval;
		}
	}

	/* Prevent the default handler from being run */
	//gtk_signal_emit_stop_by_name(GTK_OBJECT(widget), "key_press_event");

	if (key < 0)
	{
		return;
	}

#ifdef DEBUG_GTK_KEYS
	mgtk_print("*** key_press_event> %i (%c) : %i\n", key, key, mod);
#endif

	//mgtk_handle_key_press(key, mod);
}


void mgtk_event_key_release(GtkWidget *widget, GdkEventKey *event)
{
#ifdef DEBUG_GTK_KEYS
	mgtk_print("*** key_release_event> %i : %i\n", key, mod);
#endif

	//mgtk_handle_key_release(key, mod);
}


void mgtk_event_mouse_motion(GtkWidget *widget, GdkEventMotion *event)
{
	int x, y;
	GdkModifierType state;
	mgtk_glarea_window_state_t *gl_state;


	gl_state = (mgtk_glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(widget),
																			 "gl_window_state");

	if (event->is_hint)
	{
		gdk_window_get_pointer(event->window, &x, &y, &state);
	} 
	else 
	{
		x = (int)event->x;
		y = (int)event->y;
		state = (GdkModifierType)event->state;
	}

	gl_state->mouse_x = x;
	gl_state->mouse_y = y;
	mgtk_handle_motion(x, y);
	mgtk_refresh_glarea(widget);
}


void mgtk_event_update_gtk_tree(mgtk_tree_t *tree,
								GtkTreeStore *store, GtkTreeIter root)
{
	GtkTreeIter child;
	unsigned int i;
	
	
	if (!tree)
		return;
	
#ifdef DEBUG_UPDATE_GTK_TREE
	mgtk_print("mgtk_event_update_gtk_tree %d::\n", tree->id);
#endif

	if (tree->id == 0) // Root bone
	{
		store = gtk_tree_store_new(N_COLUMNS,       /* Total number of cols */
								   G_TYPE_STRING,   /* Bone Name */
    	                           G_TYPE_INT);     /* Bone Id */

		gtk_tree_store_append(store, &root, NULL);
		gtk_tree_store_set(store, &root,
						   NAME_COLUMN, tree->label,
						   ID_COLUMN, tree->id,
						   -1);

		for (i = 0; i < tree->numChildren; ++i)
		{
			mgtk_event_update_gtk_tree(&tree->children[i], store, root);
		}

		mgtk_resource_rebuild_treeview(GTK_TREE_MODEL(store));
	}
	else if (!store)
	{
		mgtk_print("mgtk_event_update_gtk_tree> ERROR: Invalid tree model\n");
	}
	else
	{
		gtk_tree_store_append(store, &child, &root);
		gtk_tree_store_set(store, &child,
						   NAME_COLUMN, tree->label,
						   ID_COLUMN, tree->id,
						   -1);

		for (i = 0; i < tree->numChildren; ++i)
		{
			mgtk_event_update_gtk_tree(&tree->children[i], store, child);
		}
	}
}


void mgtk_event_update_gtk_tree_generic(GtkTreeView *gtktree, mgtk_tree_t *tree,
										GtkTreeStore *store, GtkTreeIter root)
{
	GtkTreeIter child;
	unsigned int i;
	
	
	if (!tree)
		return;
	
#ifdef DEBUG_UPDATE_GTK_TREE
	mgtk_print("mgtk_event_update_gtk_tree_generic %d::\n", tree->id);
#endif

	if (tree->id == 0) // Root bone
	{
		store = gtk_tree_store_new(N_COLUMNS,       /* Total number of cols */
								   G_TYPE_STRING,   /* Bone Name */
    	                           G_TYPE_INT);     /* Bone Id */

		gtk_tree_store_append(store, &root, NULL);
		gtk_tree_store_set(store, &root,
						   NAME_COLUMN, tree->label,
						   ID_COLUMN, tree->id,
						   -1);

		for (i = 0; i < tree->numChildren; ++i)
		{
			mgtk_event_update_gtk_tree_generic(gtktree,
											   &tree->children[i], store, root);
		}

		if (gtktree && store)
		{
			gtk_tree_view_set_model(gtktree, GTK_TREE_MODEL(store));
		}
	}
	else if (!store)
	{
		mgtk_print("mgtk_event_update_gtk_tree_generic> ERROR: Invalid tree model\n");
	}
	else
	{
		gtk_tree_store_append(store, &child, &root);
		gtk_tree_store_set(store, &child,
						   NAME_COLUMN, tree->label,
						   ID_COLUMN, tree->id,
						   -1);

		for (i = 0; i < tree->numChildren; ++i)
		{
			mgtk_event_update_gtk_tree_generic(gtktree,
											   &tree->children[i], store, child);
		}
	}
}


void mgtk_event_update_tree(unsigned int id, mgtk_tree_t *tree)
{
#ifdef GENERIC_TREE
	Vector<GtkWidget*> *widgets;
	GtkTreeIter root;
	GtkWidget *test;
	unsigned int i;


	widgets = gWidgetMap[dialog];

	if (!widgets)
		return;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		test = (*widgets)[i];

		if (test && GTK_IS_TREE_VIEW(test))
		{
			mgtk_event_update_gtk_tree_generic(test, tree, NULL, root);
		}
		else
		{
			mgtk_print("mgtk_event_update_tree> %i:%p failed", id, tree);
		}
	}
#else
	GtkTreeIter root;

	mgtk_event_update_gtk_tree(tree, NULL, root);
#endif
}


void mgtk_event_fileselection_homedir(GtkWidget *file, void *data)
{
	char *path = mgtk_rc_map("/");
	
	if (path)
	{
#ifdef USE_OLD_FILE_SELECTION_WIDGET
		gtk_file_selection_set_filename(GTK_FILE_SELECTION(data), path);
#else
		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(data), path);
#endif

		delete [] path;
	}
}


void mgtk_event_text(GtkWidget *widget, gpointer user_data)
{
	char *text;


	if (!widget)
	{
		return;
	}

	text = (char *)gtk_entry_get_text(GTK_ENTRY(widget));

	switch (GPOINTER_TO_INT(user_data))
	{
	default:
		mgtk_handle_text(GPOINTER_TO_INT(user_data), text);
	}
}


void mgtk_event_popup_menu_handler(GtkWidget *widget, GdkEventButton *event)
{
	if (event->button == 3) 
	{
		gtk_menu_popup(GTK_MENU(widget), NULL, NULL, NULL, NULL,
					   event->button, event->time);
		//return TRUE;
	}
	
	//return FALSE;
}


void mgtk_event_popup_menu_detacher(GtkWidget *attach_widget,GtkMenu *menu)
{
}


/* Mongoose 2004.10.29, 
 * FIXME: This is currently a noop, and doesn't generate a pair event
 *        with tree_id:event_id */
void mgtk_event_tree_item_selected(GtkWidget *item, gpointer event_id)
{
	mgtk_print("mgtk_event_tree_item_selected> FIXME event %i unprocessed, %s:%i",
			   GPOINTER_TO_INT(event_id),
			   __FILE__, __LINE__);
}


void mgtk_event_slider(GtkWidget *widget, gpointer id)
{
  GtkAdjustment *adj;


  if (widget)
  {
    adj = GTK_ADJUSTMENT(widget);

	// FIXME: Just generate uint event signal for now
	mgtk_handle_slider1u(GPOINTER_TO_INT(id), (unsigned int)adj->value);
  }
}


void mgtk_event_set_color(int id, float r, float g, float b, float a)
{
#ifdef DISABLE_GTK_COLORBUTTON
	return;
#else
	Vector<GtkWidget*> *widgets;
	GtkWidget *colorbutton;
	GdkColor color;
	guint16 alpha;
	unsigned int i;

	widgets = gWidgetMap[id];

	if (!widgets)
		return;

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		colorbutton = (*widgets)[i];

		if (colorbutton && GTK_IS_COLOR_BUTTON(colorbutton))
		{
			color.red = (unsigned short)(r * 65535.0);
			color.green = (unsigned short)(g * 65535.0);
			color.blue = (unsigned short)(b * 65535.0);
			alpha = (unsigned short)(a * 65535.0);
			
			gtk_color_button_set_color(GTK_COLOR_BUTTON(colorbutton), &color);
			gtk_color_button_set_alpha(GTK_COLOR_BUTTON(colorbutton), alpha);
		}
		else
		{
			mgtk_print("mgtk_event_set_color> %d:%d failed", id, i);
		}
	}
#endif
}


void mgtk_event_color(GtkWidget *colorbutton, gpointer id)
{
#ifdef DISABLE_GTK_COLORBUTTON
	return;
#else
	GdkColor color;
	guint16 alpha;
	float r, g, b, a;


	gtk_color_button_get_color(GTK_COLOR_BUTTON(colorbutton), &color);
	alpha = gtk_color_button_get_alpha(GTK_COLOR_BUTTON(colorbutton));


	r = color.red / 65535.0;
	g = color.green / 65535.0;
	b = color.blue / 65535.0;
	a = alpha / 65535.0;

	mgtk_handle_color(GPOINTER_TO_INT(id), r, g, b, a);
#endif
}


void mgtk_event_spinbutton_int(GtkSpinButton *spin, gpointer event_id)
{
	//	int old_value;
	int new_value;


	if (spin)
	{
		new_value = gtk_spin_button_get_value_as_int(spin);

		switch (GPOINTER_TO_INT(event_id))
		{
		default:
			mgtk_print("spinbutton_int_event> event %i not handled, email bug", 
							GPOINTER_TO_INT(event_id));
		}
	}
	else
	{
		mgtk_print("spinbutton_int_event> ERROR: Assertion '%s' failed",
						"spin != NULL");
	}
}


int spinbutton_uint_set_range(GtkSpinButton *spin, 
							  unsigned int value,
							  unsigned int min, unsigned int max)
{
	gtk_spin_button_set_range(spin, -1, 100);

	if (value > max)
	{
		gtk_spin_button_set_range(spin, min, max);
		return 1;
	}	

	return 0;
}


void mgtk_event_spinbutton_uint(GtkSpinButton *spin, gpointer event_id)
{
	unsigned int event;
	unsigned int value;


	if (!spin)
	{
		mgtk_print("ERROR: Assertion '%s' failed %s:%d",
					"spin != NULL", __FILE__, __LINE__);
		return;
	}

	value = gtk_spin_button_get_value_as_int(spin);
	event = GPOINTER_TO_INT(event_id);

	mgtk_handle_event1u(event, value);
}


void mgtk_event_spinbutton_float(GtkSpinButton *spin, gpointer event_id)
{
	int event = GPOINTER_TO_INT(event_id);
	float value;


	if (spin)
	{
		value = gtk_spin_button_get_value_as_float(spin);
	}
	else
	{
		mgtk_print("spinbutton_float_event> ERROR: Assertion '%s' failed",
					"spin != NULL");
		return;
	}

	mgtk_handle_event1f(event, value);
}


void mgtk_event_notebook_switch_page(GtkNotebook *notebook,
									 GtkNotebookPage *page,
									 gint page_num, gpointer event)
{
	mgtk_notebook_eventmap_t *emap;

	mgtk_print("notebook_switch_page_event> %i:%i", 
				page_num, GPOINTER_TO_INT(event));

	emap = (mgtk_notebook_eventmap_t*)gtk_object_get_data(GTK_OBJECT(notebook), 
														  "notebook_eventmap");

	if (emap && emap->count)
	{
		mgtk_handle_command2i(GPOINTER_TO_INT(event), emap->events[page_num]);
	}
}


void mgtk_event_notify_statusbar(const char *message)
{
	GtkWidget *widget = mgtk_get_statusbar_widget();


	if (widget)
	{
		if (GTK_IS_STATUSBAR(widget))
		{
			gtk_statusbar_push(GTK_STATUSBAR(widget), 0, message);
		}
		else if (GTK_IS_LABEL(widget))
		{
			gtk_label_set_text(GTK_LABEL(widget), message);
		}
	}
}


void mgtk_event_command(GtkWidget *widget, gpointer user_data)
{
	mgtk_handle_command(GPOINTER_TO_INT(user_data));
}



////////////////////////////////////////////////////////////////
// File dialog support func
////////////////////////////////////////////////////////////////

void mgtk_event_fileselection_append_pattern(char *label, char *pattern)
{
	GtkWidget *menu = mgtk_get_fileselection_pattern_widget();
	GtkWidget *item;

	if (!menu)
	{
		mgtk_print("mgtk_event_fileselection_append_pattern> failed to append '%s':'%s'", label, pattern);
		return;
	}

	item = gtk_image_menu_item_new_with_label(label);	
	gtk_menu_append(GTK_MENU(menu), item);
	gtk_widget_show(item);

	gtk_signal_connect(GTK_OBJECT(item), "activate",
					   GTK_SIGNAL_FUNC(mgtk_gtk_event_fileselection_pattern), 
					   (gpointer)pattern);

#ifdef USE_OLD_FILE_SELECTION_WIDGET
#else
	GtkWidget *file = mgtk_get_fileselection_widget();
	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter, (char*)pattern);
	gtk_file_filter_set_name(filter, (char*)label);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file), filter);
#endif
}


void mgtk_add_menu_item(char *text, long event)
{
	GtkWidget *item;
	extern GtkWidget *GTK_FILESELECTION_DROP_DOWN_MENU;
	extern void *rc_gtk_event_func(int event);
	void *agtk_event;

	//printf("%s %p\n", text, GTK_FILESELECTION_DROP_DOWN_MENU);

	item = gtk_image_menu_item_new_with_mnemonic(text);		
	gtk_menu_append(GTK_MENU(GTK_FILESELECTION_DROP_DOWN_MENU), item);
	gtk_widget_show(item);
		
	agtk_event = rc_gtk_event_func(event);

	if (agtk_event)
	{
		gtk_signal_connect(GTK_OBJECT(item), "activate",
						   GTK_SIGNAL_FUNC(agtk_event), 
						   GINT_TO_POINTER(event));
	}
}


void mgtk_event_fileselection_action()
{
	GtkWidget *file = mgtk_get_fileselection_widget();
	char *filename;

#ifdef USE_OLD_FILE_SELECTION_WIDGET
	filename = (char *)gtk_file_selection_get_filename(GTK_FILE_SELECTION(file));
#else
	filename = (char *)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
#endif

	mgtk_handle_file_dialog_selection(filename);
	gtk_widget_hide(file);
}


void mgtk_event_fileselection_cancel()
{
	GtkWidget *file = mgtk_get_fileselection_widget();
	gtk_widget_hide(file);
}


void mgtk_event_fileselection_set_dir(char *dir)
{
	GtkWidget *file = mgtk_get_fileselection_widget();
	
	if (!dir || !dir[0])
	{
		return;
	}

#ifdef USE_OLD_FILE_SELECTION_WIDGET
	gtk_file_selection_set_filename(GTK_FILE_SELECTION(file), dir);
#else
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file), dir);
#endif
}


int mgtk_init(int argc, char *argv[])
{
#ifdef ENABLE_NLS
	bindtextdomain(PACKAGE, PACKAGE_LOCALE_DIR);
	textdomain(PACKAGE);
#endif

	gtk_init(&argc, &argv);

	mgtk_print("@GTK+ interface started...");	
	mgtk_print("Email mgtk bug reports to %s", EMAIL_ADDRESS);

	/* Mongoose 2002.02.23, 
	 * Start event which builds the widgets from a script */
	mgtk_handle_resource_start();

	return 0;
}


void mgtk_start()
{
	//gdk_threads_enter();
	gtk_main();
	//gdk_threads_leave();
}
