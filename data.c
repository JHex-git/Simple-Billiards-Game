#include "data.h"
#include <stdio.h>
#include <stdlib.h>
 
extern GameModeType gameMode;
extern PlayerBag playerBag[2];
extern Ball *B[];
extern Bag* plainBag, * stripBag, * List;
extern char prompt[80];
extern int foulCount[2];
extern int score[2];
extern int isPutBall, firstIn, startBall, player;

void TxtSave()
{
	FILE* fp;
	int i;
	Bag* bag;
	if (fp = fopen("txtdata.txt", "w")) {
		if (gameMode == poolEight) {
			fprintf(fp, "gameMode:%d\n", gameMode);
			fprintf(fp, "isPutBall:%d firstIn:%d startBall:%d player:%d\n",
				isPutBall, firstIn, startBall, player);
			fputs("Ball\n", fp);
			for (i = 0; i <= 15; i++) {
				fprintf(fp, "%f %f %d\n", B[i]->x, B[i]->y, B[i]->onTable);
			}
			fprintf(fp, "PlayerBag:%d %d %d %d\n", playerBag[0].flag, playerBag[0].n, playerBag[1].flag, playerBag[1].n);
			fputs("Bag\nPlainBag\n", fp);
			bag = plainBag;
			while (bag) {
				fprintf(fp, "%d ", bag->ballID);
				bag = bag->next;
			}
			fputs("\nStripBag\n", fp);
			bag = stripBag;
			while (bag) {
				fprintf(fp, "%d ", bag->ballID);
				bag = bag->next;
			}
			fputs("\nList\n", fp);
			bag = List->next;
			while (bag) {
				fprintf(fp, "%d ", bag->ballID);
				bag = bag->next;
			}
			fputs("\n", fp);
			fprintf(fp, "prompt:%s\n", prompt);
			fprintf(fp, "foulCount:%d %d\n", foulCount[0], foulCount[1]);
		}
		else if (gameMode == caromFour) {
			fprintf(fp, "gameMode:%d\n", gameMode);
			fprintf(fp, "player:%d\n", player);
			fputs("Ball\n", fp);
			for (i = 0; i <= 3; i++) {
				fprintf(fp, "%f %f\n", B[i]->x, B[i]->y);
			}
			fprintf(fp, "%d %d\n", score[0], score[1]);
			fprintf(fp, "prompt:%s\n", prompt);
			fprintf(fp, "foulCount:%d %d\n", foulCount[0], foulCount[1]);
		}
		else if (gameMode == nineBall) {
			fprintf(fp, "gameMode:%d\n", gameMode);
			fprintf(fp, "isPutBall:%d player:%d\n", isPutBall, player);
			fputs("Ball\n", fp);
			for (i = 0; i <= 9; i++) {
				fprintf(fp, "%f %f %d\n", B[i]->x, B[i]->y, B[i]->onTable);
			}
			fputs("List\n", fp);
			bag = List->next;
			while (bag) {
				fprintf(fp, "%d ", bag->ballID);
				bag = bag->next;
			}
			fputs("\n", fp);
			fprintf(fp, "prompt:%s\n", prompt);
			fprintf(fp, "foulCount:%d %d\n", foulCount[0], foulCount[1]);
		}

	}
	if (fclose(fp)) {
		Error("±£¥Ê ß∞‹");
	}
}

void TxtRead()
{
	FILE* fp;
	int i;
	char buffer[500];
	int num = 0;
	Bag* p, * tmp;
	if (fp = fopen("txtdata.txt", "r")) {
		fgets(buffer, 499, fp);
		sscanf(buffer, "%*[^0-9]%d", &gameMode);
		switch (gameMode) {
		case poolEight:
			fgets(buffer, 499, fp);
			sscanf(buffer, "%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d", &isPutBall, &firstIn, &startBall, &player);
			fgets(buffer, 499, fp);/* ∂¡µÙBall */
			for (i = 0; i <= 15; i++) {
				fgets(buffer, 499, fp);
				sscanf(buffer, "%lf%lf%d", &B[i]->x, &B[i]->y, &B[i]->onTable);
			}
			fgets(buffer, 499, fp);
			sscanf(buffer, "%*[^0-9-]%d%d%d%d", &playerBag[0].flag, &playerBag[0].n, &playerBag[1].flag, &playerBag[1].n);
			fgets(buffer, 499, fp);/* ∂¡µÙBag */
			fgets(buffer, 499, fp);/* ∂¡µÙplainBag*/
			fgets(buffer, 499, fp);
			p = plainBag;
			p->ballID = -(buffer[1] - '0');
			i = 3;
			while (buffer[i] != '\n') {
				if (buffer[i] != ' ')
					num = num * 10 + buffer[i] - '0';
				else {
					tmp = (Bag*)malloc(sizeof(Bag));
					tmp->ballID = num;
					num = 0;
					p->next = tmp;
					p = tmp;
				}
				i++;
			}
			p->next = NULL;
			fgets(buffer, 499, fp);/* ∂¡µÙstripBag */
			fgets(buffer, 499, fp);
			p = stripBag;
			num = 0;
			p->ballID = -(buffer[1] - '0');
			i = 3;
			while (buffer[i] != '\n') {
				if (buffer[i] != ' ')
					num = num * 10 + buffer[i] - '0';
				else {
					tmp = (Bag*)malloc(sizeof(Bag));
					tmp->ballID = num;
					num = 0;
					p->next = tmp;
					p = tmp;
				}
				i++;
			}
			p->next = NULL;
			if (plainBag->ballID == -1) {
				playerBag[0].bag = plainBag;
				playerBag[1].bag = stripBag;
			}
			else if (plainBag->ballID == -2) {
				playerBag[0].bag = stripBag;
				playerBag[1].bag = plainBag;
			}
			fgets(buffer, 499, fp);/* ∂¡µÙList */
			fgets(buffer, 499, fp);
			p = List;
			i = 0;
			while (buffer[i] != '\n') {
				if (buffer[i] != ' ')
					num = num * 10 + buffer[i] - '0';
				else {
					tmp = (Bag*)malloc(sizeof(Bag));
					tmp->ballID = num;
					num = 0;
					p->next = tmp;
					p = tmp;
				}
				i++;
			}
			p->next = NULL;
			fscanf(fp, "prompt:%s", prompt);
			fgets(buffer, 499, fp);/* ∂¡µÙ\n */
			fgets(buffer, 499, fp);
			sscanf(buffer, "%*[^0-9]%d%d", &foulCount[0], &foulCount[1]);
			break;
		case caromFour:
			fgets(buffer, 499, fp);
			sscanf(buffer, "%*[^0-9]%d", &player);
			fgets(buffer, 499, fp);/* ∂¡µÙBall */
			for (i = 0; i <= 3; i++) {
				fgets(buffer, 499, fp);
				sscanf(buffer, "%lf%lf", &B[i]->x, &B[i]->y);
			}
			fgets(buffer, 499, fp);
			sscanf(buffer, "%d%d", &score[0], &score[1]);
			fscanf(fp, "prompt:%s", prompt);
			fgets(buffer, 499, fp);/* ∂¡µÙ\n */
			fgets(buffer, 499, fp);
			sscanf(buffer, "%*[^0-9]%d%d", &foulCount[0], &foulCount[1]);
			break;
		case nineBall:
			fgets(buffer, 499, fp);
			sscanf(buffer, "%*[^0-9]%d%*[^0-9]%d", &isPutBall, &player);
			fgets(buffer, 499, fp);/* ∂¡µÙBall */
			for (i = 0; i <= 9; i++) {
				fgets(buffer, 499, fp);
				sscanf(buffer, "%lf%lf%d", &B[i]->x, &B[i]->y, &B[i]->onTable);
			}
			fgets(buffer, 499, fp);/* ∂¡µÙList */
			fgets(buffer, 499, fp);
			p = List;
			i = num = 0;
			while (buffer[i] != '\n') {
				if (buffer[i] != ' ')
					num = num * 10 + buffer[i] - '0';
				else {
					tmp = (Bag*)malloc(sizeof(Bag));
					tmp->ballID = num;
					num = 0;
					p->next = tmp;
					p = tmp;
				}
				i++;
			}
			p->next = NULL;
			fscanf(fp, "prompt:%s", prompt);
			fgets(buffer, 499, fp);/* ∂¡µÙ\n */
			fgets(buffer, 499, fp);
			sscanf(buffer, "%*[^0-9]%d%d", &foulCount[0], &foulCount[1]);
			break;

		}
		if (fclose(fp)) {
			Error("∂¡»° ß∞‹");
		}
	}
	else {
		Error("√ª”–¥Êµµ");
	}

}

void BiSave()
{
	FILE* fp;
	int num[6];
	int i;
	double data[2];
	Bag* p;
	if (fp = fopen("bidata.da", "wb")) {
		num[0] = gameMode;
		switch (gameMode) {
		case poolEight:
			num[1] = isPutBall, num[2] = firstIn;
			num[3] = startBall, num[4] = player;
			fwrite(num, sizeof(int), 5, fp);
			for (i = 0; i <= 15; i++) {
				data[0] = B[i]->x;
				data[1] = B[i]->y;
				num[0] = B[i]->onTable;
				fwrite(data, sizeof(double), 2, fp);
				fwrite(num, sizeof(int), 1, fp);
			}
			num[0] = playerBag[0].flag, num[1] = playerBag[0].n;
			num[2] = playerBag[1].flag, num[3] = playerBag[1].n;
			fwrite(num, sizeof(int), 4, fp);
			p = plainBag->next;
			i = 0;
			while (p) {
				i++;
				p = p->next;
			}
			fwrite(&i, sizeof(int), 1, fp);
			p = plainBag;
			while (p) {
				num[0] = p->ballID;
				fwrite(num, sizeof(int), 1, fp);
				p = p->next;
			}
			p = stripBag->next;
			i = 0;
			while (p) {
				i++;
				p = p->next;
			}
			fwrite(&i, sizeof(int), 1, fp);
			p = stripBag;
			while (p) {
				num[0] = p->ballID;
				fwrite(num, sizeof(int), 1, fp);
				p = p->next;
			}
			p = List->next;
			i = 0;
			while (p) {
				i++;
				p = p->next;
			}
			fwrite(&i, sizeof(int), 1, fp);
			p = List->next;
			while (p) {
				num[0] = p->ballID;
				fwrite(num, sizeof(int), 1, fp);
				p = p->next;
			}
			fwrite(prompt, sizeof(prompt), 1, fp);
			num[0] = foulCount[0], num[1] = foulCount[1];
			fwrite(num, sizeof(int), 2, fp);
			break;
		case caromFour:
			num[1] = player;
			fwrite(num, sizeof(int), 2, fp);
			for (i = 0; i <= 3; i++) {
				data[0] = B[i]->x;
				data[1] = B[i]->y;
				fwrite(data, sizeof(double), 2, fp);
			}
			num[0] = score[0], num[1] = score[1];
			fwrite(score, sizeof(int), 2, fp);
			fwrite(prompt, sizeof(prompt), 1, fp);
			num[0] = foulCount[0], num[1] = foulCount[1];
			fwrite(num, sizeof(int), 2, fp);
			break;
		case nineBall:
			num[1] = isPutBall, num[2] = player;
			fwrite(num, sizeof(int), 3, fp);
			for (i = 0; i <= 9; i++) {
				data[0] = B[i]->x;
				data[1] = B[i]->y;
				num[0] = B[i]->onTable;
				fwrite(data, sizeof(double), 2, fp);
				fwrite(num, sizeof(int), 1, fp);
			}
			p = List->next;
			i = 0;
			while (p) {
				i++;
				p = p->next;
			}
			fwrite(&i, sizeof(int), 1, fp);
			p = List->next;
			while (p) {
				num[0] = p->ballID;
				fwrite(num, sizeof(int), 1, fp);
				p = p->next;
			}
			fwrite(prompt, sizeof(prompt), 1, fp);
			num[0] = foulCount[0], num[1] = foulCount[1];
			fwrite(num, sizeof(int), 2, fp);
			break;
		}
		if (fclose(fp)) {
			Error("±£¥Ê ß∞‹");
		}
	}
}

void BiRead()
{
	FILE* fp;
	int i;
	Bag* p, * tmp;
	if (fp = fopen("bidata.da", "rb")) {
		fread(&gameMode, sizeof(int), 1, fp);
		switch (gameMode) {
		case poolEight:
			fread(&isPutBall, sizeof(int), 1, fp);
			fread(&firstIn, sizeof(int), 1, fp);
			fread(&startBall, sizeof(int), 1, fp);
			fread(&player, sizeof(int), 1, fp);
			for (i = 0; i <= 15; i++) {
				fread(&B[i]->x, sizeof(double), 1, fp);
				fread(&B[i]->y, sizeof(double), 1, fp);
				fread(&B[i]->onTable, sizeof(int), 1, fp);
			}
			fread(&playerBag[0].flag, sizeof(int), 1, fp);
			fread(&playerBag[0].n, sizeof(int), 1, fp);
			fread(&playerBag[1].flag, sizeof(int), 1, fp);
			fread(&playerBag[1].n, sizeof(int), 1, fp);

			fread(&i, sizeof(int), 1, fp);
			fread(&plainBag->ballID, sizeof(int), 1, fp);
			p = plainBag;
			while (i--) {
				tmp = (Bag*)malloc(sizeof(Bag));
				fread(&tmp->ballID, sizeof(int), 1, fp);
				p->next = tmp;
				p = tmp;
			}
			p->next = NULL;

			fread(&i, sizeof(int), 1, fp);
			fread(&stripBag->ballID, sizeof(int), 1, fp);
			p = stripBag;
			while (i--) {
				tmp = (Bag*)malloc(sizeof(Bag));
				fread(&tmp->ballID, sizeof(int), 1, fp);
				p->next = tmp;
				p = tmp;
			}
			p->next = NULL;

			fread(&i, sizeof(int), 1, fp);
			p = List;
			while (i--) {
				tmp = (Bag*)malloc(sizeof(Bag));
				fread(&tmp->ballID, sizeof(int), 1, fp);
				p->next = tmp;
				p = tmp;
			}
			p->next = NULL;
			fread(prompt, sizeof(prompt), 1, fp);
			fread(foulCount, sizeof(int), 2, fp);
			break;
		case caromFour:
			fread(&player, sizeof(int), 1, fp);
			for (i = 0; i <= 3; i++) {
				fread(&B[i]->x, sizeof(double), 1, fp);
				fread(&B[i]->y, sizeof(double), 1, fp);
			}
			fread(&score, sizeof(int), 2, fp);
			fread(prompt, sizeof(prompt), 1, fp);
			fread(foulCount, sizeof(int), 2, fp);
			break;
		case nineBall:
			fread(&isPutBall, sizeof(int), 1, fp);
			fread(&player, sizeof(int), 1, fp);
			for (i = 0; i <= 9; i++) {
				fread(&B[i]->x, sizeof(double), 1, fp);
				fread(&B[i]->y, sizeof(double), 1, fp);
				fread(&B[i]->onTable, sizeof(int), 1, fp);
			}
			fread(&i, sizeof(int), 1, fp);
			p = List;
			while (i--) {
				tmp = (Bag*)malloc(sizeof(Bag));
				fread(&tmp->ballID, sizeof(int), 1, fp);
				p->next = tmp;
				p = tmp;
			}
			p->next = NULL;
			fread(prompt, sizeof(prompt), 1, fp);
			fread(foulCount, sizeof(int), 2, fp);
			break;
		}
		if (fclose(fp)) {
			Error("∂¡»° ß∞‹");
		}
	}
	else {
		Error("√ª”–¥Êµµ");
	}
}

