#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<math.h>
#include<time.h>

struct player
{
	char name[30];
	char team[30];
	int runs = 0, balls = 0;
	int wickets_taken = 0;
	int out_by = -1;
	int overs_bowled = 0;
};

struct match
{
	char team1[30], team2[30];
	int toss, firstBatting, currentBatting, innings;
	player team1_ar[11], team2_ar[11];
	int presentOver, nextBall, maxOvers;
	int stricker, nonStricker;
	int total1, total2, extra1, extra2;
	int bowler, matchOver;
};

struct node
{
	match *m;
	node *next;
};

struct liveMatches
{
	int count = 0;
	node *live = NULL;
};

int contains(char* s1, char* s2){
	if (s1 == NULL || s2 == NULL)return 1;
	int i = 0, j = 0;
	int flag = 0;
	int l1 = strlen(s1);
	int l2 = strlen(s2);
	while (i < l1&&j < l2&&s1[i] == s2[j]){
		i++;
		j++;
	}
	if (i<l1&&s2[j] == '\0'&&s1[i]==' ')return 0;
	return 1;
}
struct previous{
	char name[20];
	int seek_position;
};

struct previous* get_previous_matches(char* game, int *length){
	FILE *fp = fopen("index.txt", "r");
	if (fp == NULL){
		return NULL;
	}
	struct previous* matches;
	matches = (struct previous*)calloc(20, sizeof(struct previous));
	char name[100];
	int seek;
	//	char ch;
	int index = 0;
	while (!feof(fp)){
		int i = fscanf(fp, "%[^,]s,", name);
		fgetc(fp);
		i = fscanf(fp, "%d", &seek);
		fgetc(fp);
		fgetc(fp);
		if (i < 1)break;
		if (!contains(name, game)){
			strcpy(matches[index].name, name);
			matches[index].seek_position = seek;
			index++;
		}
		if (fgetc(fp) == EOF)break;
	}
	if (index == 0){
		return NULL;
	}
	*length = index;
	return matches;
}

struct records{
	char name[25];
	int views;
	int seek;
};
struct records** record;
int len = 0;

struct player* read_player_details(){
	struct player* temp = (struct player*)calloc(1, sizeof(struct player));
	temp->balls = 0;
	temp->out_by = 0;
	temp->wickets_taken = 0;
	printf("\nEnter name and team name\n");
	scanf("%s", temp->name);
	scanf("%s", temp->team);
	return temp;
}

void create_player(){
	struct player* temp = read_player_details();
	FILE *fp = fopen("player.txt", "a");
	if (fp == NULL){
		fclose(fp);
		fp = fopen("player.txt", "w");
	}
	//name,team,score,wickets_taken,
	fprintf(fp, "%-20s,%-10s,%-6d,%-4d,%-4d\n", temp->name, temp->team, 0, 0, 0);

	fclose(fp);
}
void insert_player(struct player temp){
	FILE *fp = fopen("player.txt", "a");
	int seek;
	if (fp == NULL){
		fclose(fp);
		fp = fopen("player.txt", "w");
		seek = 0;
	}
	else{
		fseek(fp, 0, SEEK_END);
		seek = ftell(fp);
		fclose(fp);
		fp = fopen("player.txt", "a");
	}
	fprintf(fp, "%-20s,%-10s,%-6d,%-4d,%-4d\n", temp.name, temp.team, temp.runs, temp.wickets_taken, 0);
	record[len] = (struct records*)calloc(1, sizeof(struct records));
	strcpy(record[len]->name, temp.name);
	record[len]->seek = seek;
	record[len]->views = 0;
	len++;
	fclose(fp);
}

int find_player(char* player_name){
	int index;
	for (index = 0; index < len; index++){
		if (!contains(record[index]->name, player_name)){
			//printf("\n%s and %s and length:%d", record[index]->name, player_name,len);
			record[index]->views += 1;
			return record[index]->seek;
		}
	}
	return -1;
}

struct player* get_player_info(char *player_name)
{
	int seek_position = find_player(player_name);
	if (seek_position == -1) return NULL;
	FILE *fp = fopen("player.txt", "r");
	fseek(fp, seek_position, SEEK_SET);
	struct player *p = (struct player*)calloc(1, sizeof(struct player));
	fscanf(fp, "%[^,]s", p->name);
	fgetc(fp);
	fscanf(fp, "%[^,]s", p->team);
	fgetc(fp);
	char temp[20];
	fscanf(fp, "%[^,]s", temp);
	p->runs = atoi(temp);
	fgetc(fp);
	fscanf(fp, "%[^,]s", temp);
	p->wickets_taken = atoi(temp);
	fscanf(fp, "%d", &p->out_by);
	fgetc(fp);
	fclose(fp);
	return p;
}

struct match* match_initialize()
{
	struct match *nn = (struct match*)calloc(1, sizeof(struct match));
	printf("Enter team 1 name:\n");
	scanf("%s", nn->team1);

	printf("Enter team 2 name:\n");
	scanf("%s", nn->team2);
	nn->firstBatting = 1;
	printf("Enter toss 1/0::\n");
	scanf("%d", &nn->toss);
	for (int i = 0; i < 11; i++){
		nn->team1_ar[i].name[0] = i + 65;
		nn->team1_ar[i].name[1] = '\0';
		nn->team1_ar[i].runs = rand() % 90;
		nn->team1_ar[i].overs_bowled = rand() % 4;
		nn->team1_ar[i].out_by = rand() % 11;
		nn->team1_ar[i].wickets_taken = rand() % 2;
		nn->team1_ar[i].team[0] = 66;
		nn->team1_ar[i].team[1] = '\0';
		nn->team2_ar[i].name[0] = i + 97;
		nn->team2_ar[i].name[1] = '\0';
		nn->team2_ar[i].runs = rand() % 90;
		nn->team2_ar[i].overs_bowled = rand() % 4;
		nn->team2_ar[i].out_by = rand() % 11;
		nn->team2_ar[i].wickets_taken = rand() % 2;
		nn->team2_ar[i].team[0] = 67;
		nn->team2_ar[i].team[1] = '\0';
	}
	return nn;
}


void save_match(match *m){
	//struct match* m = match_initialize();
	FILE *fp = fopen("match.txt", "a");
	int seek;
	if (fp == NULL){
		fclose(fp);
		fp = fopen("match.txt", "w");
		seek = 0;
	}
	else{
		fseek(fp, 0, SEEK_END);
		seek = ftell(fp);
		fclose(fp);
		fp = fopen("match.txt", "a");
	}
	fprintf(fp, "%-20s:%-20s\n","Title", strcat(strcat(m->team1, " vs "), m->team2));
	char *str;
	if (m->toss == 0){
		str = m->team1_ar[0].team;
	}
	else{
		str = m->team2_ar[0].team;
	}
	fprintf(fp, "%-20s:%-20s\n","Toss won by", str);
	char *str2;
	if (m->currentBatting == 0){
		str = m->team1_ar[0].team;
		str2 = m->team2_ar[0].team;
	}
	else{
		str = m->team2_ar[0].team;
		str2 = m->team1_ar[0].team;
	}
	int wick1 = 0, wick2 = 0;
	for (int index = 0; index < 11; index++){
		wick1 += m->team1_ar[index].wickets_taken;
	}
	for (int index = 0; index < 11; index++){
		wick2 += m->team2_ar[index].wickets_taken;
	}
	if (m->total1>m->total2){
		fprintf(fp, "%-20s:%-20s\n", "Win team", m->team1_ar[0].team);
	}
	else{
		fprintf(fp, "%-20s:%-20s\n", "Win team", m->team2_ar[0].team);
	}
	fprintf(fp, "%-20s:%-20s\n%-20s:%d/%d\n%-20s:%-20s\n%-20s:%d/%d\n","Innings1", str,"Score", m->total1, wick1,"Innings2", str2,"Score", m->total2, wick2);
	fprintf(fp, "\nInnings 1 Scorecard\n");
	fprintf(fp, "%-20s%-10s%-10s%-20s\n", "Name", "runs", "balls", "wick_taken_by");
	for (int index = 0; index < 11; index++){
		if(m->team1_ar[index].out_by!=-1)fprintf(fp, "%-20s%-10d%-10d%-20s\n", m->team1_ar[index].name, m->team1_ar[index].runs, m->team1_ar[index].balls, m->team2_ar[m->team1_ar[index].out_by].name);
		else{
			fprintf(fp, "%-20s%-10d%-10d%-20s\n", m->team1_ar[index].name, m->team1_ar[index].runs, m->team1_ar[index].balls, "Not_OUT");
		}
	}
	fprintf(fp, "%-20s%-11s\n", "Bowler_Name", "wick_taken");
	for (int index = 0; index < 11; index++){
		if (m->team2_ar[index].wickets_taken>0)fprintf(fp, "%-20s%-11d\n", m->team2_ar[index].name, m->team2_ar[index].wickets_taken);
	}


	fprintf(fp, "\nInnings 2 Scorecard\n");
	fprintf(fp, "%-20s%-10s%-10s%-20s\n", "Name", "runs", "balls", "wick_taken_by");
	for (int index = 0; index < 11; index++){
		if (m->team1_ar[index].out_by != -1)fprintf(fp, "%-20s%-10d%-10d%-20s\n", m->team2_ar[index].name, m->team2_ar[index].runs, m->team2_ar[index].balls, m->team1_ar[m->team2_ar[index].out_by].name);
		else{
			fprintf(fp, "%-20s%-10d%-10d%-20s\n", m->team2_ar[index].name, m->team2_ar[index].runs, m->team2_ar[index].balls, "Not_OUT");
		}
	}
	fprintf(fp, "%-20s%-11s\n", "Bowler_Name", "wick_taken");
	for (int index = 0; index < 11; index++){
		if (m->team1_ar[index].wickets_taken>0)fprintf(fp, "%-20s%-11d\n", m->team1_ar[index].name, m->team1_ar[index].wickets_taken);
	}
	fprintf(fp, "\n\n------------------------------------------------------------------------------\n\n");
	fclose(fp);
	fp = fopen("index.txt", "a");
	if (fp == NULL){
		fclose(fp);
		fp = fopen("index.txt", "w");
	}
	time_t timer;
	char buffer[26];
	struct tm* tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
	puts(buffer);
	fprintf(fp, "%s %s,%d\n", m->team1, buffer, seek);
	fclose(fp);
	fp = fopen("player.txt", "r+");
	for (int index = 0; index<11; index++){
		struct player* temp = get_player_info(m->team1_ar[index].name);
		int seek = find_player(m->team1_ar[index].name);
		if (temp == NULL){
			insert_player(m->team1_ar[index]);
		}
		else{
			fseek(fp, seek, SEEK_SET);
			fprintf(fp, "%-20s,%-10s,%-6d,%-4d,%-4d\n", m->team1_ar[index].name, m->team1_ar[index].team, (temp->runs+m->team1_ar[index].runs), (temp->wickets_taken+temp->wickets_taken), temp->out_by + 1);
		}
	}
	for (int index = 0; index<11; index++){
		struct player* temp = get_player_info(m->team2_ar[index].name);
		int seek = find_player(m->team2_ar[index].name);
		printf("\n%d", seek);
		if (temp == NULL){
			insert_player(m->team2_ar[index]);
		}
		else{
			fseek(fp, seek, SEEK_SET);
			fprintf(fp, "%-20s,%-10s,%-6d,%-4d,%-4d\n", m->team2_ar[index].name, m->team2_ar[index].team, (temp->runs + m->team2_ar[index].runs), (temp->wickets_taken + m->team2_ar[index].wickets_taken), temp->out_by + 1);
		}
	}
	fclose(fp);
}


void sort(){
	int index1, index2;
	struct records *temp;
	for (index1 = 0; index1 < 3; index1++){
		int min_index = index1;
		for (index2 = index1 + 1; index2 < len; index2++){
			if (record[index2]->views > record[min_index]->views){
				min_index = index2;
			}
		}
		temp = record[index1];
		record[index1] = record[min_index];
		record[min_index] = temp;
	}
	
}

void load_records(){
	FILE *fp = fopen("player.txt", "r");
	record = (struct records**)calloc(300, sizeof(struct records*));
	int index = 0;
	while (ftell(fp) != EOF){
		char temp[25];
		record[index] = (struct records*)calloc(1, sizeof(struct records));
		record[index]->seek = ftell(fp);
		fscanf(fp, "%[^,]s", record[index]->name);
		fgetc(fp);
		fscanf(fp, "%[^,]s", temp);
		fgetc(fp);

		fscanf(fp, "%[^,]s", temp);
		fgetc(fp);

		fscanf(fp, "%[^,]s", temp);
		fgetc(fp);

		fscanf(fp, "%d", &record[index]->views);
		fgetc(fp);
		fgetc(fp);
		fgetc(fp);
		if (fgetc(fp) == EOF)break;
		index++;
	}
	len = index;
	sort();
}



void add_player_list(){
	FILE *fp = fopen("playername.txt", "a");
	FILE *fp1 = fopen("playernameIndex.txt", "a");

	fseek(fp, 0, SEEK_END);
	char *teamname = (char*)malloc(sizeof(char) * 20);
	printf("enter team name:");
	scanf("%s", teamname);

	fprintf(fp1, "%s,%d\n", teamname, ftell(fp));

	fprintf(fp, "%s\n", teamname);
	char *playername = (char*)malloc(sizeof(char) * 25);


	for (int i = 0; i < 11; i++){
		printf("Enter player %d name:", i);
		scanf("%s", playername);
		fprintf(fp, "%s\n", playername);
	}
	fprintf(fp, "-----------------\n");
	fclose(fp);
	fclose(fp1);
}

void storeName(char *s, char *buff){
	int i;
	for (i = 0; buff[i] != '\0'&& buff[i] != '\n'; i++){
		s[i] = buff[i];
	}
	s[i] = '\0';
}


char** get_player_list(char *name){
	FILE *fp = fopen("playername.txt", "r");
	FILE *fp1 = fopen("playernameIndex.txt", "r");
	char *teamname = name;
	char *temp = (char*)malloc(sizeof(char) * 20);
	int x = 0;

	int index = 0;

	char **team = (char **)malloc(sizeof(char*) * 11);
	char ch = 'a';
	for (int i = 0; i < 11; i++){
		team[i] = (char*)malloc(sizeof(char) * 25);
	}

	while (ftell(fp1) != EOF){
		fscanf(fp1, "%[^,]s", temp);
		//printf("%s\n", temp);

		fgetc(fp1);

		fscanf(fp1, "%d", &x);
		//printf("%d\n", x);

		if (strcmp(teamname, temp) == 0){
			index = x;
			break;
		}

		ch = fgetc(fp1);

		if (ch == EOF)break;
	}

	int count = 0;
	char * buff = (char*)malloc(sizeof(char) * 255);

	fseek(fp, index, SEEK_SET);

	fgets(buff, 255, (FILE*)fp);
	while (count < 11){
		fgets(buff, 255, (FILE*)fp);

		storeName(team[count], buff);

		//printf("%s\n", team[count]);

		count++;

	}

	return team;
}

void show_match(int index){
	FILE *fp = fopen("match.txt", "r");
	fseek(fp, index, SEEK_SET);
	char * buff = (char*)malloc(sizeof(char) * 300);
	while (1){


		fgets(buff, 300, (FILE*)fp);

		if (buff[0] == '-')
			break;

		printf("%s", buff);
	}
	int x = 0;
	scanf("%d", &x);
}
