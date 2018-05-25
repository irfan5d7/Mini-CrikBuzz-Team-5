#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include "header1.h"
#include"api.h"
void live_match();
void stats();
void feeder();
void viewer();
int conn();
void match_list();
void create();
void feeder_query(match* c_mat);
char* csv(char *query, int* i);
int char_to_int(char* arr);
char* csv(char *query, int* i);
int str_len(char* str);
void display(int match_numb);
match* traverse(int numb);

void printTemplate()
{

	system("cls");
	printf("\n\n\n");
	printf("\t\t\t\t\t ____________________________________________________\n");
	printf("\t\t\t\t\t|____________________CRICKBUZZ_______________________|\n");
	printf("\t\t\t\t\t|____________________________________________________|\n");
	printf("\n\n\n\n\n\n");

}

void print_formatted(char **strings,int numberOfStrings)
{
	int i = 0;
	for (i = 0; i < numberOfStrings; i++)
	{
		printf("%30s\n", strings[i]);
	}
}

int main()
{
	int c = 1;
	//char *strs[] = { "main", "Matches" };
	load_records();
	while (c)
	{
		printTemplate();
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\n\t\t\t||\t\t\t******** MENU ********* \t\t\t\t\t ||\n\n\t\t\t||\t\t\t1.Live Matches\t\t\t\t\t\t\t ||\n\n\t\t\t||\t\t\t2.Stats\t\t\t\t\t\t\t\t ||\n\n\t\t\t||\t\t\t3.Exit\t\t\t\t\t\t\t\t ||\n\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t\t\t\tEnter your Choice : ");
		scanf("%d", &c);
		switch (c)
		{
		case 1: live_match();
			break;
		case 2: stats();
			break;
		case 3: break;
		}
	}
	
}
void live_match()
{
	int c = 1;
	while (1)
	{
		printTemplate();
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\n\t\t\t||\t\t\t******** MENU ********* \t\t\t\t\t ||\n\n\t\t\t||\t\t\t1.Feeder   \t\t\t\t\t\t\t ||\n\n\t\t\t||\t\t\t2.Viewer\t\t\t\t\t\t\t ||\n\n\t\t\t||\t\t\t3.Back\t\t\t\t\t\t\t\t ||\n\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t\t\t\tEnter your Choice : ");
		scanf("%d", &c);
		switch (c)
		{
		case 1: feeder();
			break;
		case 2: viewer();
			break;
		case 3: return;
		}
	}
}

void feeder()
{
	if (conn())
	{
		feeder_query(NULL);
	}
}


void viewer()
{
	int c = 0;
	while (1)
	{
		printTemplate();
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\n\t\t\t||\t\t\t******** MENU ********* \t\t\t\t\t ||\n\n\t\t\t||\t\t\t1.Ongoing Matches\t\t\t\t\t\t ||\n\n\t\t\t||\t\t\t2.Back\t\t\t\t\t\t\t\t ||\n\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t\t\t\tEnter your Choice : ");
		//printf("\t\t\t\t\t*****Menu*****\n\t\t\t\t\t1.Ongoing Matches\n\t\t\t\t\t2.Back\n");
		scanf("%d", &c);
		switch (c)
		{
		case 1: match_list();
			break;
		case 2: return;
		}
	}

}


int conn()
{
	char* uid = (char*)malloc(sizeof(char) * 32);
	char* pass = (char*)malloc(sizeof(char) * 32);
	int c = 0;
	while (1)
	{
		printTemplate();
		printf("\t\t\t\t\t");
		printf("Enter uid: ");
		scanf("%s", uid);
		printf("\t\t\t\t\t");
		printf("Enter Pass: ");
		scanf("%s", pass);
		if (str_cmp(uid, "feed") && str_cmp(pass, "123"))
		{
			free(uid);
			free(pass);
			return 1;
		}
		c++;
		if (c == 3)
		{
			printf("\n\n\n\n\t\t\t\t\t\t\t");
			printf("Entered wrong credentials 3 times \n\n");
			printf("\t\t\t\t\t\t1. continue\nt\t\t\t\t\t\t2. exit\n");
			printf("\t\t\t\t\t\t\tEnter Choice : ");
			int o;
			scanf("%d", &o);
			if (o == 2)
				exit(0);
			else if (o == 1)
				return 0;
		}
	}
}

void display_prev(struct previous* prv, int len)
{
	int c;
	for (int i = 0; i < len; i++)
	{
		printf("%d.%s\n", i + 1, prv[i].name);
	}
	printf("Which match number to view : ");
	scanf("%d", &c);
	show_match(prv[c - 1].seek_position);
}

void match_res()

{
	int len = 0;
	char* tname1 = (char*)malloc(sizeof(char) * 32);
	char* tname2 = (char*)malloc(sizeof(char) * 32);
	printf("Enter a teams name 1\t: ");
	scanf("%s", tname1);
	strcat(tname1, " Vs ");
	printf("Enter a teams name 2\t: ");
	scanf("%s", tname2);
	strcat(tname1, tname2);
	lower(tname1);
	struct previous* prv = get_previous_matches(tname1, &len);
	if (prv == NULL){
		printf("\nThere is no Match\n");
		match_res();

	}
	display_prev(prv, len);
}

void info()
{
	char* name = (char*)malloc(sizeof(char) * 32);
	printf("Enter Player Name  :  ");
	scanf("%s", name);
	lower(name);
	player* p = get_player_info(name);
	if (p == NULL)
		printf("\n No such Player\n");
	else
	{
		printf("\n%s\Team: %s\n", p->name, p->team);
		printf("Runs Scored\t %d\n", p->runs);
		printf("Wickets Taken\t %d\n",  p->wickets_taken);
	}
	printf("Press Enter to continue");
	getchar();
	getchar();
}


void stats()
{
	int c = 0;
	while (1)
	{
		printTemplate();
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\n\t\t\t||\t\t\t******** MENU ********* \t\t\t\t\t ||\n\n\t\t\t||\t\t\t1.Match Results\t\t\t\t\t\t ||\n\n\t\t\t||\t\t\t2.Player Info\t\t\t\t\t\t\t\t ||\n\n\t\t\t||\t\t\t3.Back\t\t\t\t\t\t\t\t \n\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t||-------------------------------------------------------------------------------||\n");
		printf("\t\t\t\t\t\tEnter your Choice : ");
		scanf("%d", &c);
		if (c == 1)
		{
			match_res();
		}
		else if (c == 2)
		{
			info();
		}
		else if (c == 3)
			return;
	}
}



void match_list()
{
	int m, c;
	m = 1;
	struct node* h = getLiveMatches();
	struct node* temp = h;

	printTemplate();
	printf("\t\t\t||-----------------------------------------------------------------||");
	printf("\n");
	while (temp != NULL)
	{
		printf("\t\t\t||%d | %s vs %s\t\t\t\t||\n", m++, temp->m->team1, temp->m->team2);
		temp = temp->next;
	}
	printf("\n");
	printf("\t\t\t||-----------------------------------------------------------------||");
	printf("\n\t\t\t||---------99.Back-------------------------------------------------||");
	scanf("%d", &c);
	if (c == 99)
		viewer();
	display(c);
}
void create()
{
	char* team1 = (char*)malloc(sizeof(char*) * 16);
	char* team2 = (char*)malloc(sizeof(char*) * 16);
	int toss;
	int bat_first;
	int overs;
	printTemplate();
	printf("\t\t\t\t Enter the Match Details ");
	printf("\n\n\t\t\t\t\t");
	printf("Team 1: ");
	scanf("%s", team1);
	printf("\n\t\t\t\t\t");
	printf("Team 2: ");
	scanf("%s", team2);
	printf("\n\t\t\t\t\t");
	printf("Toss won by : ");
	scanf("%d", &toss);
	printf("\n\t\t\t\t\t");
	printf("Bat first by : ");
	scanf("%d", &bat_first);
	printf("\n\t\t\t\t\t");
	printf("Overs : ");
	scanf("%d", &overs);
	match* c_mat = creatematch(team1, team2, toss, bat_first, overs);
}




void feeder_query(match* c_mat)
{
	int i, j, score, legal;
	i = j = 0;
	int* upd = (int*)malloc(sizeof(int) * 3);
	int* outr = (int*)malloc(sizeof(int) * 2);
	char* tmp = (char*)malloc(sizeof(char) * 32);
	char* cmd = (char*)malloc(sizeof(char) * 32);
	char* query = (char*)malloc(sizeof(char) * 128);
	while (1)
	{
		i = 0;
		printTemplate();
		printf("\t\t\t\t\t");
		printf("\n>");
		//printf("\n>");
		getchar();
		scanf("%[^\n]s", query);
		cmd = csv(query, &i);
		i++;
		if (str_cmp(cmd, "update"))
		{
			j = 0;
			while (query[i] != '\0')
			{
				tmp = csv(query, &i);
				if (query[i] != '\0')
					i++;
				upd[j++] = char_to_int(tmp);
			}

			match *m = traverse(upd[0]);
			m->matchOver;
			update(m, upd[1], upd[2]);
		}
		else if (str_cmp(cmd, "create")){
			create();
		}
		else if (str_cmp(cmd, "exit")){
			return;
		}
		else
		{
			j = 0;
			while (query[i] != '\0')
			{
				tmp = csv(query, &i);
				if (query[i] != '\0')
					i++;
				outr[j++] = char_to_int(tmp);
			}
			match *m = traverse(outr[0]);
			out(m, outr[1]);
		}
	}
}

char* csv(char *query, int* i)
{
	int j = 0;
	char* tmp = (char*)malloc(sizeof(char) * 32);
	while (query[*i] != ',' && query[*i] != ' ' && query[*i] != '\0')
	{
		tmp[j++] = query[(*i)++];
	}
	tmp[j] = '\0';
	return tmp;
}


int str_len(char* str)
{
	int len = 0;
	while (str[len] != '\0'){ len++; }
	return len;
}

int char_to_int(char* arr)
{
	int val, m,len;
	val = 0;
	m = 1;
	len = str_len(arr);
	for (int i = len - 1; i >= 0; i--)
	{
		val += (arr[i] - '0')*m;
		m *= 10;
	}
	return val;
}

float runrate(float runs, int balls)
{
	if (balls == 0)
		return 0;
	else
		return (runs / balls) * 6;
}
void display(int match_numb)
{
	printTemplate();
	match *m = traverse(match_numb-1);
	player stricker = (m->currentBatting == 1 ? m->team1_ar[m->stricker] : m->team2_ar[m->stricker]);
	player nonStricker = (m->currentBatting == 1 ? m->team1_ar[m->nonStricker] : m->team2_ar[m->nonStricker]);
	player bowler = (m->currentBatting == 2 ? m->team1_ar[m->bowler] : m->team2_ar[m->bowler]);
	printf("\n%-10svs%10s\n\n", m->team1, m->team2);
	printf("Batting : %-20s run rate :%.2f\n\n", (m->currentBatting == 1 ? m->team1 : m->team2), runrate(m->currentBatting == 1 ? m->total1 : m->total2, m->presentOver * 6 + m->nextBall - 1));
	printf("Score : %d-%d/%d.%d\n", (m->currentBatting == 1 ? m->total1 : m->total2), max(m->stricker, m->nonStricker) - 1, m->presentOver, m->nextBall - 1);
	printf("%-20s    R   B   SR\n", "Batsman");
	printf("*%-20s %3d  %3d  %.2f\n", stricker.name, stricker.runs, stricker.balls, strike_rate(stricker.runs, m->presentOver * 6 + m->nextBall - 1));
	printf("%-20s  %3d  %3d  %.2f\n\n", nonStricker.name, nonStricker.runs, nonStricker.balls, strike_rate(nonStricker.runs, m->presentOver * 6 + m->nextBall - 1));
	printf("bowler : %-20s overs: %d.%d\n", bowler.name, bowler.overs_bowled, m->nextBall - 1);
	while (_getch() != '\r');
	getchar();
	return;
}


match* traverse(int numb)
{
	int i = 0;
	struct node* h = getLiveMatches();
	while (h != NULL )
	{
		if (i == numb)
		{
			return h->m;
		}
		h = h->next;
		i++;
	}
}
