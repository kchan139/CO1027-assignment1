#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include "main.h"

void display(int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue);
void adventureToKoopa(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue);

void readStats(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue);
void readEvent(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue);

void readMushGhost (string file_input, int & HP, int & phoenixdown, int eventID);
void readMerlinPack (string file_input, int & HP);
void readAsclepiusPack (string file_input, int & remedy, int & maidenkiss, int & phoenixdown);

void eventUpdate(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue, int eventID, int i);
string readNeededFile (string file_input, int eventID);

int mushGhost_type1 (int n2, int arr[]);
int mushGhost_type2 (int n2, int arr[]);
int mushGhost_type3 (int n2, int arr[]);
int mushGhost_type4 (int n2, int arrCopy[]);

int mushMario (int n1);
bool isPrime (int n);

#endif // __KNIGHT_H__