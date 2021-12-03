/* Projeto2 IAED */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_VALUE 65535

typedef struct treenode {
	char *lastpart;
	char *path;
	char *value;
	struct treenode *child;
	struct treenode *brother;
} treenode;

treenode *createnode(char *parentpath, char *part){
	treenode* result = (treenode*)calloc(1, sizeof(treenode));
	if (!result)
		return NULL;
	if(!parentpath){
		result->path = (char*)malloc(2);
		strcpy(result->path, "/");
		return result;
	}
	result->path = (char*)malloc(strlen(parentpath)+1+strlen(part)+1);
	strcpy(result->path, parentpath);
	if (strcmp(parentpath, "/") != 0)
		strcat(result->path, "/");
	strcat(result->path, part);
	result->lastpart = (char*)malloc(strlen(part)+1);
	strcpy(result->lastpart, part);
	return result;
}

void freenode(treenode *node){
	treenode *child;

	for(child = node->child; child; child = child->brother)
		freenode(child);

	free(node->value);
	free(node->path);
	free(node->lastpart);
	free(node);
}


void printtree(treenode *root){
	if (root == NULL){
		printf("---<empty>---\n");
		return;
	}
	printf("value = %s\n", root->value);
	printf("child\n");
	printtree(root->child);
	printf("brother\n");
	printtree(root->brother);
	printf("done");

}

void help(){
	printf("help: Imprime os comandos disponíveis.\n");
	printf("quit: Termina o programa.\n");
	printf("set: Adiciona ou modifica o valor a armazenar.\n");
	printf("print: Imprime todos os caminhos e valores.\n");
	printf("find: Imprime o valor armazenado.\n");
	printf("list: Lista todos os componentes imediatos de um sub-caminho.\n");
	printf("search: Procura o caminho dado um valor.\n");
	printf("delete: Apaga um caminho e todos os subcaminhos.\n");
}


void quit(treenode *start){
	freenode(start);
	exit(0);
}


void set(treenode *start, char *path, char *value){
	char *part;
	treenode *node = start, *child, *brother, *parent = start;
	
	part = strtok(path, "/");

	while(part){
		brother = NULL;
		for(child = node->child; child; brother = child, child = child->brother){	
			if (strcmp(part, child->lastpart)==0){
				parent = node;
				node = child;
				break;
			}
		}
		/*if not found*/
		if(node != child){
			node = createnode(parent->path, part);
			if (brother) brother->brother = node;
			else if (parent) parent->child = node;
		}

		part = strtok(NULL, "/");

	}
	node->value = (char*)malloc(strlen(value)+1);
	strcpy(node->value, value);
}


void print(treenode *start){
	treenode *child;

	if(start->value) printf("%s %s\n", start->path, start->value);
	for(child = start->child; child; child = child->brother)
		print(child);
}


void find(treenode *start, char *path){
	char *part;
	treenode *node = start, *child;
	
	part = strtok(path, "/");

	while(part){
		for(child = node->child; child; child = child->brother){	
			if (strcmp(part, child->lastpart)==0){
				node = child;
				break;

			}
		}
		/*if not found*/
		if(node != child){
			printf("not found\n");
			return;
		}

		part = strtok(NULL, "/");
	}
	if(!node->value) {
		printf("no data\n");
	return;
	}
	printf("%s\n", node->value);
}

void list(treenode *start, char *path){
	int i = 1, x;
	char *part, **parts = NULL;
	treenode *node = start, *child;
	
	if (!path)
		part = NULL;
	else
		part = strtok(path, "/");

	while(part){
		for(child = node->child; child; child = child->brother){	
			if (strcmp(part, child->lastpart)==0){
				node = child;
				break;
			}
		}
		/*if not found*/
		if(node != child){
			printf("not found\n");
			return;
		}

		part = strtok(NULL, "/");
	}
	for(child = node->child; child; child = child->brother, i++){
		parts = realloc(parts, i * sizeof(char*));
		parts[i-1] = child->lastpart;
	}
	/*ordenar **parts with malloc*/
	for (x = 0; x < i-1; x++)
		printf("%s\n", parts[x]);
}

int search(treenode *start, char *value){
	treenode *child;

	if(start->value && strcmp(start->value, value) == 0) printf("%s\n", start->path);
	for(child = start->child; child; child = child->brother)
		if(search(child, value))
			return 1;
	if(strcmp(start->path, "/") == 0)
		printf("not found\n");
	return 0;
}

void delete(treenode *start, char *path){
	char *part;
	treenode *node = start, *child, *brother, *parent;
	
	if (!path)
		part = NULL;
	else
		part = strtok(path, "/");

	while(part){
		brother = NULL;
		for(child = node->child; child; brother = child, child = child->brother){	
			if (strcmp(part, child->lastpart)==0){
				parent = node;
				node = child;
				break;
			}
		}
		/*if not found*/
		if(node != child){
			printf("not found\n");
			return;
		}

		part = strtok(NULL, "/");
	}
	if (brother) brother->brother = node->brother;
	else parent->child = node->brother;
	freenode(node);
}

char *concatenate(char*a, char*b, char*c){
	int size = strlen(a)+strlen(b)+strlen(c)+1;
	char*str = malloc(size);
	strcpy(str, a);
	strcat(str, b);
	strcat(str,c);
	return str;
}

int main(){
	char line[MAX_STR_VALUE], *command, *path, *value, *h;
	treenode *start = createnode(NULL, NULL);

	while(1){
		fgets(line, MAX_STR_VALUE, stdin);
		line[strcspn(line, "\n")] = '\0';
		command = strtok(line, " ");
		path = strtok(NULL, " ");
		value = strtok(NULL, "");
		if (strcmp(command, "help") == 0) help();
		if (strcmp(command, "quit") == 0) quit(start);
		if (strcmp(command,  "set") == 0) set(start, path, value);
		if (strcmp(command,  "print") == 0) print(start);
		if (strcmp(command,  "find") == 0) find(start, path);
		if (strcmp(command,  "list") == 0) list(start, path);
		if (strcmp(command,  "search") == 0){

			h =concatenate(path, " ", value);
			printf("%s\n", h);
			search(start, h);
		}
		if (strcmp(command,  "delete") == 0) delete(start, path);
	}
	return 0;
}