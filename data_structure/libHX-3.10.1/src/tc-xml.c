/*
 *	this program is released in the Public Domain
 */
#include <stdbool.h>
#include <stdio.h>
#include <libxml/parser.h>
#include <libHX/defs.h>
#include <libHX/libxml_helper.h>

int main(void)
{
	xmlDoc *doc;
	xmlNode *root, *etc, *node;
	char *result = NULL;
	int size = 0;

	doc = xmlNewDoc(NULL);
	root = xmlNewDocNode(doc, NULL, "root", NULL);
	xmlDocSetRootElement(doc, root);
	xml_newnode(root, "empty", NULL);
	etc = xml_newnode(root, "filled", NULL);
	xml_newnode(etc, "a", "1234 bytes");
	node = xml_newnode(etc, "b", "0 bytes");
	xml_newnode(node, "extra", NULL);
	xmlDocDumpFormatMemory(doc, reinterpret_cast(xmlChar **, &result),
		&size, true);
	xmlSaveFileEnc("test.xml", doc, "utf-8");
	if (result != NULL)
		printf("%.*s\n", size, result);
	return 0;
}
