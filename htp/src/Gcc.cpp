//============================================================================
// Name        : Gcc.cpp
// Author      : TH
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdio>
#include <dirent.h>
#include "HtmlParser.h"

using namespace std;

string stripPar(const string &name)
{
	size_t q = name.find_first_of("?");
	if (q != string::npos)
		return name.substr(0,q);
	else
		return name;
}

string findExt(const string name)
{
	string bare = stripPar(name);
	size_t dot = bare.find_last_of(".");
	if (dot != string::npos)
		return bare.substr(dot);
	else
		return bare;
}

void processFile(struct dirent *dname, const string &dir)
{
	string ext = findExt(dname->d_name);
	if (ext == ".js") return;
	if (ext == ".css") return;
	if (ext == ".jpg") return;
	if (ext == ".png") return;
	cout << dir << '/' << dname->d_name << endl;
}

void traverse(DIR *p, const string &name)
{
 	struct dirent *pDirent;

 	do {
 		pDirent = readdir(p);
 		if (pDirent) {
			if (pDirent->d_type == DT_REG) {
				processFile(pDirent, name);
			} else if (pDirent->d_type == DT_DIR) {
				if (string(pDirent->d_name) == ".") continue;
				if (string(pDirent->d_name) == "..") continue;
				string path = name + '/' + pDirent->d_name;
				DIR *pD = opendir(path.c_str());
				if (pD) {
					traverse(pD, path);
					closedir(pD);
				}
			}
 		}
 	} while (pDirent);
}

int main(int argc, char *argv[]) {
	string HeaderOrParagraph;
	//DIR *pDir;

/*
	pDir = opendir(argv[1]);
	if (!pDir) {
		cerr << "Could not open directory " << argv[1] << endl;
		return 1;
	}
	traverse(pDir, argv[1]);
	closedir(pDir);
*/
	HtmlParser htp("/home/dad/port5/port5.de/index.html");
	htp.quickParse();
	while (htp.getHeaderOrParagraph(HeaderOrParagraph)) {
		cout << HeaderOrParagraph << endl;
	}
	return 0;
}
