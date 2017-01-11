/***************************************************************************
                          main.c  -  description
                             -------------------
    begin                : Mon Mar  6 17:54:27 EST 2000
    copyright            : (C) 2000 by Charles Leeds
                           (C) 2017 Muhammet Kara
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
#define GTK_ENABLE_BROKEN
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <gtk/gtk.h>
#include <gtk/gtktext.h>
#include <time.h>

#define DEBUG(x) printf(x)

typedef struct record {
	char *question;
	char *answer;
} record, *quesPtr;

typedef struct quiz {
	int nbQuestions;
	quesPtr question[500];
} Quiz, *quizPtr;

quizPtr theQuiz;

int curQuestion;  // The current question we are on
int curShowing;
int quiz_method = 0;   //  1 is edit, 2 is sequential, 3 is random
int quiz_open = 0; // 0 is no quiz open, 1 is a quiz open
int quiz_changed = 1; // 0 if there are no unsaved changes, 1 if there are

GString* quiz_filename;
GString* sq_description;

GtkWidget* frmMain;
GtkWidget* frmEdit;
GtkWidget* textbox;
GtkWidget* textbox_edit_question;
GtkWidget* textbox_edit_answer;
GtkWidget* dialog_open;
GtkWidget* label_mode;
GtkWidget* label_quizname;
GtkWidget* label_sq_name;
GtkWidget* label_num_of_questions;
GtkWidget* label_current_question;

GtkWidget* create_frmMain();
GtkWidget* create_frmEdit();
GtkWidget* dialog_save_quiz();

quizPtr xml_parse_quizfile(char *filename);
int get_random (int max_random);
void refresh_mode();
void save_question_changes();

void menu_clicked_close_quiz()
{
	GString* tempstring = g_string_new("");
  GtkWidget* dialogbox;
	int i;

	if (quiz_open == 1) {
		if (quiz_method == 1) {  // Save changes to current question if in edit mode
			save_question_changes();
		}
		if (quiz_changed == 1) {
	  		dialogbox = dialog_save_quiz ();
			gtk_widget_show(dialogbox);
		}


		for (i = 0; i < theQuiz->nbQuestions; i++) {
			free(theQuiz->question[i]->question);
			free(theQuiz->question[i]->answer);
			free(theQuiz->question[i]);
		}
		theQuiz->nbQuestions = 0;
		quiz_open = 0; // No quiz is now open
//		g_string_free (quiz_filename, FALSE);

		gtk_text_backward_delete( GTK_TEXT(textbox_edit_question), gtk_text_get_length(GTK_TEXT(textbox_edit_question)) );
		gtk_text_backward_delete( GTK_TEXT(textbox_edit_answer), gtk_text_get_length(GTK_TEXT(textbox_edit_answer)) );

		g_string_sprintf (tempstring, " ");
		gtk_label_set_text (GTK_LABEL (label_current_question), tempstring->str);
		g_string_sprintf (tempstring, " ");
		gtk_label_set_text (GTK_LABEL (label_num_of_questions), tempstring->str);
		g_string_sprintf (tempstring, " ");
		gtk_label_set_text (GTK_LABEL (label_sq_name), tempstring->str);

		gtk_widget_show (label_current_question);
		gtk_widget_show (label_num_of_questions);
		gtk_widget_show (textbox_edit_question);
		gtk_widget_show (textbox_edit_answer);
	}

}

void save_question_changes()
{
	GString* tempstring = g_string_new("");
	g_string_sprintf(tempstring, "%s", gtk_editable_get_chars( GTK_EDITABLE(textbox_edit_question), 0, -1) );

	if (strcmp (tempstring->str, theQuiz->question[curQuestion]->question) ) {  // Strings don't match
		free(theQuiz->question[curQuestion]->question);
		theQuiz->question[curQuestion]->question = malloc(tempstring->len);
		sprintf(theQuiz->question[curQuestion]->question, tempstring->str);
		quiz_changed = 1;  // There are unsaved changes
	}

	g_string_sprintf(tempstring, "%s", gtk_editable_get_chars( GTK_EDITABLE(textbox_edit_answer), 0, -1) );

	if (strcmp (tempstring->str, theQuiz->question[curQuestion]->answer) ) {  // Strings don't match
		free(theQuiz->question[curQuestion]->answer);
		theQuiz->question[curQuestion]->answer = malloc(tempstring->len);
		sprintf(theQuiz->question[curQuestion]->answer, tempstring->str);
		quiz_changed = 1;  // There are unsaved changes
	}

}

void button_clicked_next(GtkWidget *widget, gpointer data)
{
	GString* tempstring = g_string_new("");


	if (quiz_open == 1) {
		if (quiz_method == 1 && curQuestion > -1) { //If in edit mode and a legal question is open
			save_question_changes();
		}

		gtk_text_backward_delete( GTK_TEXT(textbox_edit_question), gtk_text_get_length(GTK_TEXT(textbox_edit_question)) );
		gtk_text_backward_delete( GTK_TEXT(textbox_edit_answer), gtk_text_get_length(GTK_TEXT(textbox_edit_answer)) );
		curShowing = 0;
		if (quiz_method == 1)
			curQuestion++;
		if (quiz_method == 2)
			curQuestion++;
		if (quiz_method == 3)
			curQuestion = get_random (theQuiz->nbQuestions);
		if (curQuestion == theQuiz->nbQuestions)
			curQuestion = 0;

		gtk_text_insert(GTK_TEXT(textbox_edit_question), NULL, NULL, NULL, theQuiz->question[curQuestion]->question, -1);

		if (quiz_method == 1) { //If in edit mode
			gtk_text_insert(GTK_TEXT(textbox_edit_answer), NULL, NULL, NULL, theQuiz->question[curQuestion]->answer, -1);
		}

		g_string_sprintf (tempstring, "%d", curQuestion + 1);
		gtk_label_set_text (GTK_LABEL (label_current_question), tempstring->str);
		g_string_sprintf (tempstring, "%d", theQuiz->nbQuestions);
		gtk_label_set_text (GTK_LABEL (label_num_of_questions), tempstring->str);

		gtk_widget_show (label_current_question);
		gtk_widget_show (label_num_of_questions);
		gtk_widget_show (textbox_edit_question);
		gtk_widget_show (textbox_edit_answer);
	}
}

void refresh_mode()
{
	GString* tempstring = g_string_new("");

	if (quiz_method == 0) {
		g_string_sprintf (tempstring, "No selection");
	} else if (quiz_method == 1) {
		g_string_sprintf (tempstring, "Edit");
		gtk_text_set_editable(GTK_TEXT(textbox_edit_question), TRUE);
		gtk_text_set_editable(GTK_TEXT(textbox_edit_answer), TRUE);
	} else if (quiz_method == 2) {
		g_string_sprintf (tempstring, "Sequential");
		gtk_text_set_editable(GTK_TEXT(textbox_edit_question), FALSE);
		gtk_text_set_editable(GTK_TEXT(textbox_edit_answer), FALSE);
	} else if (quiz_method == 3) {
		g_string_sprintf (tempstring, "Random");
		gtk_text_set_editable(GTK_TEXT(textbox_edit_question), FALSE);
		gtk_text_set_editable(GTK_TEXT(textbox_edit_answer), FALSE);
  }
	gtk_label_set_text (GTK_LABEL (label_mode), tempstring->str);
	gtk_widget_show (label_mode);

}

int get_random (int max_random)
{
	int i;

	i = (int) ( ( (float) lrand48() / RAND_MAX ) * max_random);

 	return i;
}

/* Get the selected filename and print it to the console */
void file_ok_sel( GtkWidget *w, GtkFileSelection *fs )
{
//	extern GString* quiz_filename;
//	extern quizPtr theQuiz;

	GString* tempstring = g_string_new("");
	GString* quiz_filename = g_string_new ("");

	g_string_sprintf (quiz_filename, "%s", gtk_file_selection_get_filename( GTK_FILE_SELECTION(fs) ) );
//	g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));

	theQuiz = xml_parse_quizfile (quiz_filename->str);
	quiz_open = 1; // Signify file is open

	gtk_text_backward_delete (GTK_TEXT (textbox), gtk_text_get_length(GTK_TEXT(textbox)) );
	curShowing = 0;     // We are now showing the question not answer
	curQuestion = 0;    // Set to first question
	gtk_widget_hide (dialog_open);

	g_string_sprintf (tempstring, "%d", theQuiz->nbQuestions);
	gtk_label_set_text (GTK_LABEL (label_quizname), tempstring->str);
	if (sq_description->len > 0){
		printf("%lu\n", sq_description->len);
		gtk_label_set_text (GTK_LABEL (label_sq_name), sq_description->str);
	}
	gtk_window_set_title (GTK_WINDOW (frmMain), quiz_filename->str);
//	gtk_widget_show (label_sq_name);

	if (quiz_method > 0) {
		curQuestion = -1; //Adjusts to first question once button_click_next ++'s it
		button_clicked_next(NULL, NULL);
  }

}

void file_cancel(gpointer data)
{
 	gtk_widget_hide(data);
}

quesPtr xml_parse_question(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur) {
	quesPtr ret = NULL;

DEBUG("parseQuestion\n");

	ret = (quesPtr) malloc(sizeof(record));
	printf ("%lu", sizeof(record) );
	if (ret == NULL) {
		fprintf(stderr, "out of memory\n");
		return(NULL);
	}

	memset(ret, 0, sizeof(record));

	cur = cur->children;
	while (cur != NULL) {
//		if ((!strcmp(cur->name, "Question")) && (cur->ns == ns))
		if (!strcmp(cur->name, "Question"))
			ret->question = xmlNodeListGetString(doc, cur->children, 1);
//		if ((!strcmp(cur->name, "Answer")) && (cur->ns == ns))
		if (!strcmp(cur->name, "Answer"))
			ret->answer = xmlNodeListGetString(doc, cur->children, 1);
		cur = cur->next;
	}

	return(ret);
}



quizPtr xml_parse_quizfile(char *filename) {
	xmlDocPtr doc;
	quizPtr ret;
	quesPtr question;
	xmlNsPtr ns;
	xmlNodePtr cur;
	xmlNodePtr prob;

	doc = xmlParseFile(filename);
	if (doc == NULL) return(NULL);

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return(NULL);
	}

	ret = (quizPtr) malloc(sizeof(quizPtr));

	if (ret == NULL) {
		fprintf(stderr,"out of memory\n");
		xmlFreeDoc(doc);
		return(NULL);
	}

	memset(ret, 0, sizeof(quizPtr));

	cur = cur->children;
	while (cur != NULL) { // Parse nodes on Description / Problems level
		if (!strcmp(cur->name, "Description")) {
			g_string_sprintf(sq_description, xmlNodeListGetString(doc, cur->children, 1) );
		} else if (!strcmp(cur->name, "Problems")) {
   		prob = cur->children;
   		while (prob != NULL) {
   			if (!strcmp(prob->name, "Problem")) {
   				question = xml_parse_question(doc, ns, prob);
   				if (question != NULL) {
   					ret->question[ret->nbQuestions++] = question;
   					printf ("%d\n", ret->nbQuestions);
   				} else {
   					printf ("Fraud!\n");
   				}

   				if (ret->nbQuestions >= 500) break;

   			} else {
   				printf ("Not in the right order!\n");
   			}
   			prob = prob->next;
   		}

   	}	else {
//   		xmlFreeDoc(doc);
//   		free(ret);
//   		return(NULL);
   	}
		cur = cur->next;

	}

	return(ret);
}

void button_clicked_edit(GtkWidget *widget, gpointer data)
{
	quiz_method = 1;  // Set mode to edit
	refresh_mode();
	curQuestion = curQuestion -1; //Adjusts to first question once button_click_next ++'s it
	button_clicked_next(NULL, NULL);
}

void button_clicked_show_answer(GtkWidget *widget, gpointer data)
{
	if (quiz_open == 1) { // If quiz is open
  	if (quiz_method == 2 || quiz_method == 3) { // If in random or sequential mode
  		gtk_text_backward_delete( GTK_TEXT(textbox_edit_answer), gtk_text_get_length(GTK_TEXT(textbox_edit_answer)) );
  		gtk_text_insert(GTK_TEXT(textbox_edit_answer), NULL, NULL, NULL, theQuiz->question[curQuestion]->answer, -1);
  		gtk_widget_show(textbox_edit_answer);
  	}
	}
}

void button_clicked_exit(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

void button_clicked_save(GtkWidget *widget, gpointer data)
{
	xmlDocPtr doc;
  xmlNodePtr tree, level1, level2;
	int i;

  doc = xmlNewDoc("1.0");
  //doc->root = xmlNewDocNode(doc, NULL, "quiz:Linux", NULL);
  xmlDocSetRootElement	(doc, xmlNewDocNode(doc, NULL, "quiz:Linux", NULL));

  tree = xmlNewChild(xmlDocGetRootElement(doc), NULL, "quiz:Problems", NULL);
	for (i=0 ; i < theQuiz->nbQuestions ; i++) {
	  level1 = xmlNewChild(tree, NULL, "quiz:Problem", NULL);
  	level2 = xmlNewChild(level1, NULL, "quiz:Question", theQuiz->question[i]->question);
	  level2 = xmlNewChild(level1, NULL, "quiz:Answer", theQuiz->question[i]->answer);
	}

	xmlSaveFile( (char*) quiz_filename->str, (xmlDocPtr) doc);

	xmlFreeDoc(doc);

	quiz_changed = 0; // There are now no unsaved changes
}

void menu_clicked_open (gpointer data)
{
// gtk_show_widget (data);
	gtk_widget_show (data) ;
}

void menu_clicked_sequential (gpointer data)
{
	quiz_method = 2; // Set mode to sequential
	refresh_mode();
	curQuestion = curQuestion -1; //Adjusts to first question once button_click_next ++'s it
	button_clicked_next(NULL, NULL);
}

void menu_clicked_random (gpointer data)
{
	quiz_method = 3;// Set mode to random
	refresh_mode();
	curQuestion = curQuestion -1; //Adjusts to first question once button_click_next ++'s it
	button_clicked_next(NULL, NULL);
}

int main(int argc, char **argv)
{
	// Declarations
	int i;
	time_t t1;
	curQuestion = 0;

  // Initializations
	quiz_filename = g_string_new ("");
	sq_description = g_string_new ("");

	// Initialize random seed
	(void) time (&t1);
	srand48 ( (long) t1); // Set random seed

	for (i = 1; i < argc ; i++) {
		theQuiz = xml_parse_quizfile(argv[i]);
	}

	gtk_init(&argc, &argv);  // No gtk objects can be initialized before this point

	dialog_open = gtk_file_selection_new ("File selection");
//	gtk_file_selection_set_filename(GTK_FILE_SELECTION(dialog_open), "/root/quizzes/");

	gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dialog_open)->ok_button),
		"clicked",
		(GtkSignalFunc) file_ok_sel,
		dialog_open );

	gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (dialog_open)->cancel_button),
		"clicked",
		(GtkSignalFunc) file_cancel ,
		(gpointer) dialog_open );

	frmEdit = create_frmEdit ();

	g_signal_connect(GTK_OBJECT(frmEdit), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();

	return 0;
}






