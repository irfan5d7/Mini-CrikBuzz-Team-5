#pragma once
#include<stdlib.h>
#include<stdio.h>
#include"database.h"
int str_cmp(char *s1, char *s2)
{
	int i;
	for (i = 0; s1[i] && s2[i]; i++)
	{
		if (s1[i] != s2[i])
			return 0;
	}
	return 1;
}
int str_cpy(char *source, char *dest)
{
	int i;
	for (i = 0; source[i]; i++)
		dest[i] = source[i];
	dest[i] = '\0';
	return i;
}

void lower(char *str)
{
	int i = 0;
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] >= 'A'&&str[i] <= 'Z')
		{
			str[i] = str[i] - 'A' + 'a';
		}
	}
	return;
}

liveMatches LM;
void init()
{
	LM.count = 0;
	LM.live = NULL;
}

void copyTeamNames(char **teamPlayerNames, char *temamName, player *players)
{
	//FILE *f = fopen(teamPlayerNamefile, "r");
	for (int i = 0; i < 11; i++) {
		//fscanf(f, "%s", players[i].name);
		str_cpy(teamPlayerNames[i], players[i].name);
		str_cpy(temamName, players[i].team);
		players[i].out_by = -1;
		players[i].wickets_taken = players[i].balls = players[i].runs = players[i].overs_bowled = 0;
	}
	//fclose(f);
	free(teamPlayerNames);
}
node* getLiveMatches()
{
	return LM.live;
}

int search(player *p, char *name)
{
	for (int i = 0; i < 11; i++)
	{
		if (str_cmp(name, p[i].name))
			return i;
	}
	return -1;
}

void get_nextbowler(match *m)
{
	printf("enter next bowler name:");
	char b[100];
	scanf("%s", b);
	if (m->currentBatting == 1)
	{
		m->bowler = search(m->team2_ar, b);
	}
	else
		m->bowler = search(m->team1_ar, b);
	if (m->bowler == -1)
		get_nextbowler(m);
}



match * creatematch(char *team1, char *team2, int toss, int firstBatting, int maxOvers)
{
	match *m = (match *)malloc(sizeof(match));
	char **names;
	str_cpy(team1, m->team1);
	str_cpy(team2, m->team2);
	m->total1 = m->total2 = m->extra1 = m->extra2 = 0;
	m->toss = toss;
	m->currentBatting = m->firstBatting = firstBatting;
	m->maxOvers = maxOvers;
	m->stricker = 0;
	m->nonStricker = 1;
	m->presentOver = 0;
	m->nextBall = 1;
	//copyTeamNames("C:/Users/aslam/Desktop/team5/index1.txt", team1, m->team1_ar);//use database call later
	//copyTeamNames("C:\\Users\\aslam\\Desktop\\team5\\index2.txt", team2, m->team2_ar);//use database call later
	lower(m->team1);
	names = get_player_list(m->team1);///changed here 
	copyTeamNames(names, m->team1, m->team1_ar);
	lower(m->team2);
	names = get_player_list(m->team2);///changed here 
	copyTeamNames(names, m->team2, m->team2_ar);
	node *temp = LM.live;
	m->innings = 1;
	m->matchOver = 0;
	if (temp == 0) {
		LM.live = (node *)malloc(sizeof(node));
		LM.live->m = m;
		LM.live->next = NULL;
	}
	else
	{
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = (node *)malloc(sizeof(node));
		temp->m = m;
		temp->next = NULL;
	}
	LM.count++;
	m->currentBatting = firstBatting;
	get_nextbowler(m);
	return m;
}

int max(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

void out(struct match *mat, int out_batsman) {
	if (mat != NULL) {
		int i;
		if (out_batsman == 1) {
			if (mat->currentBatting == 1) {
				mat->team1_ar[mat->stricker].out_by = mat->bowler;
				i = max(mat->stricker, mat->nonStricker) + 1;
				if (mat->team1_ar[i].out_by == -1 && i != mat->nonStricker)
					mat->stricker = i;
				mat->team2_ar[mat->bowler].wickets_taken++;
				
			}
			else if (mat->currentBatting == 2) {
				mat->team2_ar[mat->stricker].out_by = mat->bowler;
				i = max(mat->stricker, mat->nonStricker) + 1;
				if (mat->team2_ar[i].out_by == -1 && i != mat->nonStricker)
					mat->stricker = i;
				mat->team1_ar[mat->bowler].wickets_taken++;
			}
		}
		else {
			if (mat->currentBatting == 1) {
				mat->team1_ar[mat->nonStricker].out_by = mat->bowler;
				i = max(mat->stricker, mat->nonStricker) + 1;
				if (mat->team1_ar[i].out_by == -1 && i != mat->stricker)
					mat->nonStricker = i;
				mat->team2_ar[mat->bowler].wickets_taken++;
			}
			else if (mat->currentBatting == 2) {
				mat->team2_ar[mat->nonStricker].out_by = mat->bowler;
				i = max(mat->stricker, mat->nonStricker) + 1;
				if (mat->team2_ar[i].out_by == -1 && i != mat->stricker)
					mat->nonStricker = i;
				mat->team1_ar[mat->bowler].wickets_taken++;
			}
		}
		if (i == 11)
		{
			if (mat->innings == 1)
			{
				mat->innings = 2;
				mat->presentOver = 0;
				mat->nextBall = 1;
				mat->currentBatting ^= 3;
				mat->stricker = 0;
				mat->nonStricker = 1;
				printf("Innings over all out");
				get_nextbowler(mat);
			}
			else
			{
				printf("match over\n");
				mat->currentBatting ^= 3;
				mat->matchOver = 1;
				save_match(mat);
			}
			return;
		}
	}
}




void update(struct match *mat, int no_of_runs, int legal)
{
	int temp;

	if (legal == 1)
	{
		if (mat->currentBatting == 1)
		{
			mat->total1 += no_of_runs;
			mat->team1_ar[mat->stricker].runs += no_of_runs;
			mat->team1_ar[mat->stricker].balls += 1;
		}
		else
		{
			mat->total2 += no_of_runs;
			mat->team2_ar[mat->stricker].runs += no_of_runs;
			mat->team2_ar[mat->stricker].balls += 1;
		}
		if (mat->nextBall == 2)
		{
			mat->presentOver++;
			mat->nextBall = 1;
			temp = mat->stricker;
			mat->stricker = mat->nonStricker;
			mat->nonStricker = temp;
			if (mat->currentBatting == 1)
			{
				mat->team2_ar[mat->bowler].overs_bowled++;
			}
			else {
				mat->team1_ar[mat->bowler].overs_bowled++;
			}
			if (mat->presentOver == mat->maxOvers)
			{
				if (mat->innings == 1)
				{
					mat->innings = 2;
					mat->presentOver = 0;
					mat->nextBall = 1;
					mat->currentBatting ^= 3;
					mat->stricker = 0;
					mat->nonStricker = 1;
					//printf("Innings over all out");
					printf("innings Over, overs complete\n");
					get_nextbowler(mat);
					return;
				}
				else
				{
					printf("match over\n");
					printf("Match completed\n");
					mat->matchOver = 1;
					save_match(mat);
					return;
				}
			}
			get_nextbowler(mat);
		}
		else
			mat->nextBall += 1;
	}
	else
	{
		if (mat->currentBatting == 1)
		{
			mat->total1 += no_of_runs;
			mat->extra1 += 1;
		}
		else
		{
			mat->total2 += no_of_runs;
			mat->extra2 += 1;
		}

	}
	if (no_of_runs % 2 != 0)
	{
		temp = mat->stricker;
		mat->stricker = mat->nonStricker;
		mat->nonStricker = temp;

	}
	if (mat->innings == 2)
	{

		if (mat->total1 < mat->total2&&mat->currentBatting == 2)
		{
			printf("match over\n");
			mat->matchOver = 1;
			save_match(mat);
		}
		else if (mat->total1 > mat->total2&&mat->currentBatting == 1)
		{
			printf("match over\n");
			mat->matchOver = 1;
			save_match(mat);
		}
	}
}