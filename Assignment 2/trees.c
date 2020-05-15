/*
    Author: Mark Douglas Ray Robinson
    Date: November 16, 2018
    Title: Assignment 2 ---- CORPUS
    Description:
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "queue.h"
#include "cda.h"
#include "bst.h"
#include "rbt.h"
#include "gst.h"
#include "tnode.h"
#include "scanner.h"


/*  Options */
static char *clean(char *str);
static void getOPS(void *t, char *w, char *c);
static void REDops(void *t, char *word, char *command);
static void GREENops(void *t, char *word, char *command);

static int compareStrings(void *str1, void *str2);
static RBT *createREDtree();
static GST *createGREENtree();
static void displayStrings(void *value, FILE *fp);
static void freeStrings(void *value);
static void readCommands(void *t);


static void ProcessOptions(int, char **);

char *corpus = 0, *commFile = 0;
char tree;

int main(int argc, char **argv) {
  ProcessOptions(argc, argv);

  /*  FUN STUFF STARTS HERE */
  if (tree == 'R') {
    RBT *t = createREDtree();
    readCommands((void *)t);
    freeRBT(t);
  }
  else if (tree == 'G') {
    GST *t = createGREENtree();
    readCommands((void *)t);
    freeGST(t);
  }
  return 0;
}

void ProcessOptions(int argc, char **argv) {
  // -v option -> Print Name And Exit
  if (argv[1][1] == 'v' || argc == 2) {
    printf("Mark Douglas Ray Robinson\n");
    exit(0);
  }
  // Four arguments
  else if (argc == 4) {
    tree = toupper(argv[1][1]);
    corpus = argv[2];
    commFile = argv[3];
  }
  // Three arguments (Red Black Tree By Default)
  else if (argc == 3) {
    tree = 'R';
    corpus = argv[1];
    commFile = argv[2];
  }
}

RBT *createREDtree() {
  RBT *t = newRBT(compareStrings);
  setRBTdisplay(t, displayStrings);
  setRBTfree(t, freeStrings);
  FILE *fp = fopen(corpus, "r");
  char *s;
  while (!feof(fp)) {
    if (stringPending(fp))
      s = readString(fp);
    else
      s = readToken(fp);
    if (s == 0)
      break;
    s = clean(s);
    if (s != NULL) {
      insertRBT(t, s);
    }
  }
  fclose(fp);
  return t;
}

void readCommands(void *t) {
  FILE *fp = fopen(commFile, "r");
  char *s = NULL, *c = NULL, *w = NULL;
  while (!feof(fp)) {
    if (stringPending(fp)) {
      s = readString(fp);
      if (s == 0)   // End of file
        break;
      w = clean(s);
      if (w != NULL)  // word exists
        getOPS(t, w, c);
      free(c);
      c = NULL;
      w = NULL;
    }
    else {
      s = readToken(fp);
      if (s == 0)
        break;
      if (c != NULL) {    // command stored
        // read in word
        w = clean(s);
        if (w != NULL)    // word exists
          getOPS(t, w, c);
        if (c)
          free(c);
        c = NULL;
        w = NULL;
      }
      else if (c == NULL ) {    // no command stored
        c = s;
        if (c[0] == 's' || c[0] == 'r') {   // show or report
          getOPS(t, w, c);
          if (c)
            free(c);
          c = NULL;
          w = NULL;
        }
        // delete insert or frequency
        else
          w = NULL;
      }
    }
  }
  fclose(fp);
}

void getOPS(void *t, char *w, char *c) {
  if (tree == 'R')
    REDops(t, w, c);
  else if (tree == 'G')
    GREENops(t, w, c);
}

void REDops(void *t, char *word, char *command) {
  RBT *r = t;
  if (command[0] == 's') {
    if (sizeRBT(r) > 0)
      displayRBT(r, stdout);
    else
      fprintf(stdout, "EMPTY\n");
  }
  else if (command[0] == 'r')
    statisticsRBT(r, stdout);
  else if (command[0] == 'i')
    insertRBT(r, word);
  else if (command[0] == 'd') {
    if (freqRBT(r, word) > 1)
      deleteRBT(r, word);
    else if (freqRBT(r, word) == 1) {
      char *str = findRBT(r, word);
      setRBTfree(r, 0);
      deleteRBT(r, word);
      setRBTfree(r, freeStrings);
      freeStrings(str);
    }
    else {
      printf("Value %s not found.\n", word);
    }
    free(word);
  }
  else if (command[0] == 'f') {
    fprintf(stdout, "Frequency of %s: %d\n", word, freqRBT(r, word));
    free(word);
  }
}

GST *createGREENtree() {
  GST *t = newGST(compareStrings);
  setGSTdisplay(t, displayStrings);
  setGSTfree(t, freeStrings);
  FILE *fp = fopen(corpus, "r");
  char *s;
  while (!feof(fp)) {
    if (stringPending(fp))
      s = readString(fp);
    else
      s = readToken(fp);
    if (s == 0)
      break;
    s = clean(s);
    if (s != NULL) {
      insertGST(t, s);
    }
  }
  fclose(fp);
  return t;
}

void GREENops(void *t, char *word, char *command) {
  GST *r = t;
  if (command[0] == 's') {
    if (sizeGST(r) > 0)
      displayGST(r, stdout);
    else
      fprintf(stdout, "EMPTY\n");
  }
  else if (command[0] == 'r')
    statisticsGST(r, stdout);
  else if (command[0] == 'i')
    insertGST(r, word);
  else if (command[0] == 'd') {
    if (freqGST(r, word) > 1)
      deleteGST(r, word);
    else if (freqGST(r, word) == 1) {
      char *str = findGST(r, word);
      deleteGST(r, word);
      freeStrings(str);
    }
    else {
      printf("Value %s not found.\n", word);
    }
    free(word);
  }
  else if (command[0] == 'f') {
    fprintf(stdout, "Frequency of %s: %d\n", word, freqGST(r, word));
    free(word);
  }
}

char *clean(char *str) {
  int i = 0, j = 0, size = 0;
  while (str[i] != '\0') {
    str[i] = tolower(str[i]);
    if ( (!(isalpha(str[i])) && !(isspace(str[i]))) ||  (isspace(str[i]) && size == 0)) {
      str[i] = '6';
    }
    else if ((isspace(str[i]) && size > 0) && (str[i + 1] == '\0' || !isalpha(str[i + 1]))) {
      str[i] = '6';
    }
    else
      size += 1;
    i += 1;
  }
  // the entire text is illegal
  if (size == 0) {
    free(str);
    return NULL;
  }
  char *newStr = (char *)malloc(size + 1);
  for (i = 0; i < (int)strlen(str); i++) {
    if (str[i] != '6') {
      newStr[j++] = str[i];
    }
  }
  newStr[j] = '\0';
  free(str);
  return newStr;
}

int compareStrings(void *str1, void *str2) {
  char *s1 = str1, *s2 = str2;
  return strcmp(s1, s2);
}

void displayStrings(void *value, FILE *fp) {
  char *val = value;
  fprintf(fp, "%s", val);
}

void freeStrings(void *value) {
  free(value);
}
