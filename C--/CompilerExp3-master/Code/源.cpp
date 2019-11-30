#include "AbstractSyntaxTree.h"
#include "SemanticAnalysis.h"
#include <assert.h>
int main() {
	FILE* file = fopen("temp.txt", "r");
	Node node = getSyntaxTreeFromFile(file);
	semAnalysis(node);
	fclose(file);
	printTables();
	printInterCodeList(NULL, NULL);
	return 0;
}