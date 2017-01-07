/***************************************************************************
                          gui.c  -  description
                             -------------------
    begin                : Mon Mar 6 2000
    copyright            : (C) 2000 by Charles Leeds
    email                : clever@cdc.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

extern GtkWidget* dialog_open;
extern GtkWidget* textbox;
extern GtkWidget* label_mode;
extern GtkWidget* label_sq_name;
extern GtkWidget* label_quizname;
extern GtkWidget* label_num_of_questions;
extern GtkWidget* label_current_question;

extern GtkWidget* textbox_edit_question;
extern GtkWidget* textbox_edit_answer;

void file_ok_sel( GtkWidget *w, GtkFileSelection *fs );
void file_cancel(gpointer data);
void button_clicked_edit(GtkWidget *widget, gpointer data);
void button_clicked_exit(GtkWidget *widget, gpointer data);
void button_clicked_next(GtkWidget *widget, gpointer data);
void button_clicked_save(GtkWidget *widget, gpointer data);
void button_clicked_show_answer(GtkWidget *widget, gpointer data);
void menu_clicked_open(gpointer data);
void menu_clicked_sequential (gpointer data);
void menu_clicked_random (gpointer data);
void menu_clicked_close_quiz ();
void save_question_changes ();

GtkWidget* create_frmEdit()
{
	GtkWidget *window;
	GtkWidget *vbox_base;
	GtkWidget *hbox1;
	GtkWidget *hbox2;
	GtkWidget *hbox3;
	GtkWidget *vbox2;
	GtkWidget *vbox3;
	GtkWidget *vbox4;
	GtkWidget *label1, *label_sq_desc, *label3, *label4;

	GtkWidget *button_next;
	GtkWidget *button_edit;
	GtkWidget *button_add;
	GtkWidget *button_delete;
	GtkWidget *button_show_answer;
	GtkWidget *button_answer_0, *button_answer_1, *button_answer_2;
	GtkWidget *button_answer_3, *button_answer_4, *button_answer_5;

/*  Menu stuff    */
	GtkWidget* menu_bar;
	GtkWidget* menu_items;

	GtkWidget* submenu_file;
	GtkWidget* submenu_learn;
	GtkWidget* submenu_set;
	GtkWidget* menu_root_file;
	GtkWidget* menu_root_learn;
	GtkWidget* menu_root_set;
	char buf[128];
/* End of menu stuff */

// dialog_open = gtk_file_selection_new ("File selection");

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(GTK_WIDGET(window), 600, 400);
	gtk_window_set_title(GTK_WINDOW(window), "gMemorize Editor");

	vbox_base = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox_base), 0);
	gtk_container_add (GTK_CONTAINER (window), vbox_base);
	gtk_widget_show (vbox_base);

	submenu_file = gtk_menu_new ();
	submenu_learn = gtk_menu_new ();
	submenu_set = gtk_menu_new ();

	menu_items = gtk_menu_item_new_with_label ("New");
	gtk_menu_append (GTK_MENU (submenu_file), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_open),
		(gpointer) dialog_open);

	gtk_widget_show (menu_items);

	menu_items = gtk_menu_item_new_with_label ("Open");
	gtk_menu_append (GTK_MENU (submenu_file), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_open),
		(gpointer) dialog_open);

	gtk_widget_show (menu_items);

	menu_items = gtk_menu_item_new_with_label ("Close");
	gtk_menu_append (GTK_MENU (submenu_file), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_close_quiz),
		(gpointer) dialog_open);

	sprintf (buf, "Save");
	menu_items = gtk_menu_item_new_with_label (buf);
	gtk_menu_append (GTK_MENU (submenu_file), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (button_clicked_save),
		(gpointer) dialog_open);

	sprintf (buf, "Exit");
	menu_items = gtk_menu_item_new_with_label (buf);
	gtk_menu_append (GTK_MENU (submenu_file), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (button_clicked_exit),
		(gpointer) dialog_open);

	gtk_widget_show (menu_items);

	menu_items = gtk_menu_item_new_with_label ("New");
	gtk_menu_append (GTK_MENU (submenu_set), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_open),
		(gpointer) dialog_open);

	gtk_widget_show (menu_items);

	menu_items = gtk_menu_item_new_with_label ("Open");
	gtk_menu_append (GTK_MENU (submenu_set), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_open),
		(gpointer) dialog_open);

	gtk_widget_show (menu_items);

	menu_items = gtk_menu_item_new_with_label ("Edit");
	gtk_menu_append (GTK_MENU (submenu_set), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_open),
		(gpointer) dialog_open);

	gtk_widget_show (menu_items);

	menu_items = gtk_menu_item_new_with_label ("Close");
	gtk_menu_append (GTK_MENU (submenu_set), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_open),
		(gpointer) dialog_open);

  sprintf (buf, "Sequential");
	menu_items = gtk_menu_item_new_with_label (buf);
	gtk_menu_append (GTK_MENU (submenu_learn), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_sequential),
		(gpointer) dialog_open);

	gtk_widget_show (menu_items);

	sprintf (buf, "Random");
	menu_items = gtk_menu_item_new_with_label (buf);
	gtk_menu_append (GTK_MENU (submenu_learn), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_random),
		(gpointer) dialog_open);

	gtk_widget_show (menu_items);

	sprintf (buf, "Progressive");
	menu_items = gtk_menu_item_new_with_label (buf);
	gtk_menu_append (GTK_MENU (submenu_learn), menu_items);

	gtk_signal_connect_object (GTK_OBJECT (menu_items),
		"activate",
		GTK_SIGNAL_FUNC (menu_clicked_open),
		(gpointer) dialog_open);

	gtk_widget_show (menu_items);

	/* This is the root menu, and will be the label
    * displayed on the menu bar.  There won't be a signal handler attached,
    * as it only pops up the rest of the menu when pressed. */
	menu_root_file = gtk_menu_item_new_with_label ("File");
	menu_root_learn = gtk_menu_item_new_with_label ("Learn");
	menu_root_set = gtk_menu_item_new_with_label ("Set");

	gtk_widget_show (menu_root_file);
	gtk_widget_show (menu_root_learn);
	gtk_widget_show (menu_root_set);

	/* Now we specify that we want our newly created "menu" to be the menu
	 * for the "root menu" */
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_root_file), submenu_file);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_root_learn), submenu_learn);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_root_set), submenu_set);

	menu_bar = gtk_menu_bar_new ();
	gtk_box_pack_start (GTK_BOX (vbox_base), menu_bar, FALSE, FALSE, 0);
	gtk_widget_show (menu_bar);

   /* And finally we append the menu-item to the menu-bar -- this is the
         * "root" menu-item I have been raving about =) */
	gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), menu_root_file);
	gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), menu_root_set);
	gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), menu_root_learn);

	gtk_widget_show (menu_bar);

	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox1), 5);
	gtk_container_add (GTK_CONTAINER (vbox_base), hbox1);
	gtk_widget_show (hbox1);

	hbox2 = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox2), 5);
	gtk_container_add (GTK_CONTAINER (vbox_base), hbox2);
	gtk_widget_show (hbox2);

	hbox3 = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox3), 5);
	gtk_container_add (GTK_CONTAINER (vbox_base), hbox3);
	gtk_widget_show (hbox3);

	vbox2 = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox2), 0);
	gtk_container_add (GTK_CONTAINER (hbox1), vbox2);
	gtk_widget_show (vbox2);

	vbox3 = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox3), 0);
	gtk_container_add (GTK_CONTAINER (hbox1), vbox3);
	gtk_widget_show (vbox3);

	vbox4 = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox4), 0);
	gtk_container_add (GTK_CONTAINER (hbox1), vbox4);
	gtk_widget_show (vbox4);

	textbox_edit_question = gtk_text_new (0, 0);
	gtk_text_set_editable(GTK_TEXT(textbox_edit_question), TRUE);
	gtk_text_set_word_wrap(GTK_TEXT(textbox_edit_question), TRUE);
	gtk_text_insert(GTK_TEXT(textbox_edit_question), NULL, NULL, NULL, "", -1);
	gtk_box_pack_start (GTK_BOX (vbox2), textbox_edit_question, TRUE, TRUE, 0);
	gtk_widget_show (textbox_edit_question);

	textbox_edit_answer = gtk_text_new (0, 0);
	gtk_text_set_editable(GTK_TEXT(textbox_edit_answer), TRUE);
	gtk_text_set_word_wrap(GTK_TEXT(textbox_edit_answer), TRUE);
	gtk_text_insert(GTK_TEXT(textbox_edit_answer), NULL, NULL, NULL, "", -1);
	gtk_box_pack_start (GTK_BOX (vbox2), textbox_edit_answer, TRUE, TRUE, 0);
	gtk_widget_show (textbox_edit_answer);

	button_next     = gtk_button_new_with_label ("   next   ");
	gtk_box_pack_start (GTK_BOX (hbox2), button_next,    TRUE, TRUE, 30);
	gtk_widget_show (button_next);

	button_add     = gtk_button_new_with_label ("   add   ");
	gtk_box_pack_start (GTK_BOX (hbox2), button_add,    TRUE, TRUE, 30);
	gtk_widget_show (button_add);

	button_edit     = gtk_button_new_with_label ("   edit   ");
	gtk_box_pack_start (GTK_BOX (hbox2), button_edit,    TRUE, TRUE, 30);
	gtk_widget_show (button_add);

	button_delete     = gtk_button_new_with_label ("   delete   ");
	gtk_box_pack_start (GTK_BOX (hbox2), button_delete,    TRUE, TRUE, 30);
	gtk_widget_show (button_delete);

	button_show_answer     = gtk_button_new_with_label ("   show answer   ");
	gtk_box_pack_start (GTK_BOX (hbox3), button_show_answer,    TRUE, TRUE, 30);
	gtk_widget_show (button_show_answer);

	button_answer_0     = gtk_button_new_with_label ("   0   ");
	gtk_box_pack_start (GTK_BOX (hbox3), button_answer_0,    TRUE, TRUE, 30);
	gtk_widget_show (button_answer_0);

	button_answer_1     = gtk_button_new_with_label ("   1   ");
	gtk_box_pack_start (GTK_BOX (hbox3), button_answer_1,    TRUE, TRUE, 30);
	gtk_widget_show (button_answer_1);	

	button_answer_2     = gtk_button_new_with_label ("   2   ");
	gtk_box_pack_start (GTK_BOX (hbox3), button_answer_2,    TRUE, TRUE, 30);
	gtk_widget_show (button_answer_2);	

	button_answer_3     = gtk_button_new_with_label ("   3   ");
	gtk_box_pack_start (GTK_BOX (hbox3), button_answer_3,    TRUE, TRUE, 30);
	gtk_widget_show (button_answer_3);	

	button_answer_4     = gtk_button_new_with_label ("   4   ");
	gtk_box_pack_start (GTK_BOX (hbox3), button_answer_4,    TRUE, TRUE, 30);
	gtk_widget_show (button_answer_4);	

	button_answer_5     = gtk_button_new_with_label ("   5   ");
	gtk_box_pack_start (GTK_BOX (hbox3), button_answer_5,    TRUE, TRUE, 30);
	gtk_widget_show (button_answer_5);	

	label1 = gtk_label_new ("Mode");
	gtk_box_pack_start (GTK_BOX (vbox3), label1, TRUE, TRUE, 15);
	gtk_widget_show (label1);

	label_sq_desc = gtk_label_new ("Quiz");
	gtk_box_pack_start (GTK_BOX (vbox3), label_sq_desc, TRUE, TRUE, 15);
	gtk_widget_show (label_sq_desc);

	label3 = gtk_label_new ("Current Question");
	gtk_box_pack_start (GTK_BOX (vbox3), label3, TRUE, TRUE, 15);
	gtk_widget_show (label3);

	label4 = gtk_label_new ("Total Questions");
	gtk_box_pack_start (GTK_BOX (vbox3), label4, TRUE, TRUE, 15);
	gtk_widget_show (label4);

	label_mode = gtk_label_new ("None selected");
	gtk_box_pack_start (GTK_BOX (vbox4), label_mode, TRUE, TRUE, 15);
	gtk_widget_show (label_mode);

	label_sq_name = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (vbox4), label_sq_name, TRUE, TRUE, 15);
	gtk_widget_show (label_sq_name);

	label_current_question = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (vbox4), label_current_question, TRUE, TRUE, 15);
	gtk_widget_show (label_current_question);

	label_num_of_questions = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (vbox4), label_num_of_questions, TRUE, TRUE, 15);
	gtk_widget_show (label_num_of_questions);
	
	gtk_signal_connect(GTK_OBJECT(button_edit),
		"clicked",
		GTK_SIGNAL_FUNC(button_clicked_edit),
		NULL);

	gtk_signal_connect(GTK_OBJECT(button_next),
		"clicked",
		GTK_SIGNAL_FUNC(button_clicked_next),
		NULL);

	gtk_signal_connect(GTK_OBJECT(button_show_answer),
		"clicked",
		GTK_SIGNAL_FUNC(button_clicked_show_answer),
		NULL);

	gtk_signal_connect(GTK_OBJECT(button_answer_0),
		"clicked",
		GTK_SIGNAL_FUNC(save_question_changes),
		NULL);

	gtk_widget_show_all(window);


	return window;
}

GtkWidget* dialog_save_quiz()
{
	GtkWidget *window;
	GtkWidget *button_yes;
	GtkWidget *button_no;
	GtkWidget *label;

	window = gtk_dialog_new ();

	button_yes     = gtk_button_new_with_label ("   Yes   ");
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area),
		button_yes, TRUE, TRUE, 0);
	gtk_widget_show (button_yes);

	button_no     = gtk_button_new_with_label ("   No   ");
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area),
		button_no, TRUE, TRUE, 0);
	gtk_widget_show (button_no);

	label = gtk_label_new ("Unsaved changes have been made to some questions.  Do you want to save those changes?");
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->vbox),
		label, TRUE, TRUE, 0);
	gtk_widget_show (label);

	return window;
}