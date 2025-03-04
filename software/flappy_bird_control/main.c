/*
 * main.c
 *
 *  Created on: Nov 26, 2015
 *      Author: Zefan Xu
 */
 #include <stdio.h>
 #define true 1
 #define false 0

 volatile unsigned int * birdy = (unsigned int*) 0x20;
 volatile unsigned int * birdx = (unsigned int*) 0x30;
 volatile unsigned int * pipe_x = (unsigned int*) 0x40;
 volatile unsigned int * pipe3 = (unsigned int*) 0x70;
 volatile unsigned int * pipe2 = (unsigned int*) 0x60;
 volatile unsigned int * pipe1 = (unsigned int*) 0x50;
 volatile unsigned int * pipe4 = (unsigned int*) 0x90;
 volatile unsigned int * pipe5 = (unsigned int*) 0x80;
 volatile unsigned int * status = (unsigned int*) 0xA0;
 volatile unsigned int * keycode = (unsigned int*) 0xC0;
 volatile unsigned int * press = (unsigned int*) 0xB0;
 volatile unsigned int * TextX = (unsigned int*) 0xE0;
 volatile unsigned int * TextY = (unsigned int*) 0xD0;
 volatile unsigned int * ScoreY = (unsigned int*) 0xF0;
 volatile unsigned int * ScoreX = (unsigned int*) 0x100;
 volatile unsigned int * third = (unsigned int*) 0x110;
 volatile unsigned int * second = (unsigned int*) 0x120;
 volatile unsigned int * first = (unsigned int*) 0x130;
 volatile unsigned int * ScoreY2 = (unsigned int*) 0x150;
 volatile unsigned int * ScoreX2 = (unsigned int*) 0x160;
 volatile unsigned int * birdy2 = (unsigned int*) 0x140;
 volatile unsigned int * first2 = (unsigned int*) 0x190;
 volatile unsigned int * second2 = (unsigned int*) 0x180;
 volatile unsigned int * third2 = (unsigned int*) 0x170;


 static double timeCount, yPosition, pipeXposition, timeCount2, yPosition2;
 static int y_init, jump, gameover, gamestart, score, AIPlay, tempHeight, y_init2, jump2, dead, dead2,score2;
 static int pressed = false;
 static int too_high = false;
 static int two_players;
 static int bestscore = 0;

 int debouncer(){
	 pressed = pressed | (*press);
	 int released = !(*press);
	 if (pressed){
		 if (released){
			 pressed = false;
			 return 1;
		 }
	 }
	 return 0;
 }

 int randomHeight(){
 	int out = 120 + rand() % 180;
 	return out;
 }

 void initialize_game(){
 	y_init = 280;
 	*birdx = 0;
 	*birdy = y_init;
 	timeCount = 0;
 	yPosition = y_init;

 	y_init2 = 200;
 	*birdy2 = y_init2;
 	timeCount2 = 0;
 	yPosition2 = y_init2;

 	*pipe_x = 980;
 	*pipe1 = randomHeight();
 	*pipe2 = randomHeight();
 	*pipe3 = randomHeight();
 	*pipe4 = randomHeight();
 	*pipe5 = randomHeight();
 	pipeXposition = *pipe_x;

 	score = 0;
 	score2 = 0;
 	jump = false;
	gameover = false;
	gamestart = false;
	AIPlay = false;
	jump2 = false;
	dead = false;
	dead2 = false;

	tempHeight = 0;
	*ScoreX = 100;
	*ScoreY = 0;
	*ScoreX2 = 600;
	*ScoreY2 = 0;

	two_players = false;
 }

 int AIPlayer(int birdy, int pipex){
	int nextHeight;
 	if (pipex >= 450 ) nextHeight = *pipe5 + 90 - 36;
 	else if (pipex <= 180 && pipex >= 100) nextHeight = *pipe3 + 90 - 36;
 	else nextHeight = *pipe2 + 90 - 36;
 	if (!jump && (birdy >= (nextHeight - 10))) {
 		timeCount = 0;
 		y_init = yPosition;
 		jump = true;
 		tempHeight = birdy;
 	}
 	if (birdy < (nextHeight - 10)) {
 		jump = false;
 	}
 	else if (birdy < (tempHeight - 51)) {
 		jump = false;
 	}
 }

 int bird_alive(int birdy, int pipex){
	 //printf("pipex : %d, birdy: %d, pipe5: %d, pipe2: %d \n", pipex, birdy, *pipe5, *pipe2);
 	if (pipex <= 540 + 51 && pipex >= 450){
 		if ((birdy > *pipe5 - 90 + 33) && (birdy < *pipe5 + 90 - 36)) return 1;
 		else return 0;
 	}
 	else if ((pipex <= 270 + 51 && pipex >= 180)){
 		if ((birdy > *pipe2 - 90 + 33) && (birdy < *pipe2 + 90 - 36)) return 1;
 		else return 0;
 	}
 }

 int main(){
 	while (1){
 		while (!gamestart){
 			if (*keycode == 90 && debouncer()) {
 				initialize_game();
 				gamestart = true;
 				gameover = false;
 			}
 			else if ((*keycode == 30 || *keycode == 114) && debouncer()){
 				initialize_game();
 				gamestart = true;
 				gameover = false;
 				two_players = true;
 			}
 			else{
 				*ScoreX = 600;
 				*ScoreY = 0;
 				*TextX = 260;
 				*TextY = 200;
 				*status = 1;
 				if (yPosition < 280) timeCount = 0;
 				timeCount += 0.02;
 				yPosition = 0.5 * (-10) * timeCount * timeCount + timeCount * 45 + 280;
 				*birdy = 480 - yPosition;

 				*first = bestscore % 10;
 				*second = (bestscore / 10) % 10;
 				*third = (bestscore / 100) % 10;
 			}
 		}
 		while (gamestart && !gameover){

 			if (two_players == false) *status = 0;
 			else *status = 3;
 			if (*keycode == 27 && debouncer() && !AIPlay) {
 				AIPlay = true;
 			}
 			if (*keycode == 90 && debouncer() && AIPlay) {
 				AIPlay = false;
 			}
 			if (AIPlay) AIPlayer(*birdy, pipeXposition);
		//update bird position /////////////////////////////////////////////////////////////////////////
 			if (*keycode == 41 && debouncer()) {
 				jump = true;
 				timeCount = 0;
 				y_init = yPosition;
 			}

 				timeCount += 0.033;
 				yPosition = 0.5 * (-10) * timeCount * timeCount + timeCount * 36 + y_init;

 			if (yPosition < 80 || !bird_alive(*birdy, pipeXposition)) {
 				jump = false;
 				dead = true;
 			}
 			if (yPosition > 480) {
 				yPosition = 479;
 				timeCount = 3.6;
 				y_init = 415;
 			}
 			if (yPosition2 > 480) {
 				yPosition2 = 479;
 				timeCount2 = 3.6;
 				y_init2 = 415;
 			}
 			if (!dead)
 				*birdy = 480 - yPosition;
 			//update position for the second bird///////////////////////////////////////////////////////
 			if (*keycode == 117 && debouncer()) {
 				jump2 = true;
 				timeCount2 = 0;
 				y_init2 = yPosition2;
 			}

 				timeCount2 += 0.033;
 				yPosition2 = 0.5 * (-10) * timeCount2 * timeCount2 + timeCount2 * 36 + y_init2;

 			if (yPosition2 < 80 || !bird_alive(*birdy2, pipeXposition)) {
 				jump2 = false;
 				dead2 = true;
 			}
 			if (!dead2)
 				*birdy2 = 480 - yPosition2;

 			if (dead && two_players == false || dead && dead2 && two_players){
 				gameover = true;
 				break;
 			}
 		//track score
 			if (!dead){
 				if (pipeXposition <= 450 && pipeXposition > 449.7) score++;
 				if (pipeXposition <= 180 && pipeXposition > 179.7) score++;
 				*first = score % 10;
 				*second = (score / 10) % 10;
 				*third = (score / 100) % 10;
 			}
 			if (!dead2){
 				if (pipeXposition <= 450 && pipeXposition > 449.7) score2++;
 				if (pipeXposition <= 180 && pipeXposition > 179.7) score2++;
 				*first2 = score2 % 10;
 				*second2 = (score2 / 10) % 10;
 				*third2 = (score2 / 100) % 10;
 			}
 			if (dead && two_players){
 				*birdy = -50;
 			}
 			if (dead2 && two_players){
 				*birdy2 = -50;
 			}

		//update pipe x position ///////////////////////////////////////////////////////////////////////
 			pipeXposition -= 0.3;

 			if (pipeXposition <= 100){
 				*pipe5 = *pipe3;
 				*pipe2 = *pipe1;
 				pipeXposition = 640;
 				*pipe1 = *pipe4;
 				*pipe3 = randomHeight();
 				*pipe4 = randomHeight();
 			}
 			*pipe_x = pipeXposition;
 		}
 		if (gameover){
 			int i;
 			bestscore = (score > bestscore) ? score : bestscore;
 			bestscore = (score2 > bestscore) ? score2 : bestscore;
 			for (i = 0; i < 5000; i++) rand();
 			gamestart = false;
 		}
 	}

 }
