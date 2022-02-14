#ifndef DISPLAY_H  
#define DISPLAY_H  


void statusmessage(const char *msg);
void screenmessage(const char *msg);
void header(const char *string);

void TFTSetup();
void ShowAddress(int Channel);
void ShowSpeed(int Channel);

void DrawRosterList();
void DrawKeypad();
void DrawNumberPad();
void MainScreen();
void RosterScreen();
void KeypadScreen();
void CopyRosterItem(int b);
int CheckButtons();
int CheckRosterButtons();
bool CheckNumberPad();


#endif