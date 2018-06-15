//============================================================================
// Name        : Gcc.cpp
// Author      : TH
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <typeinfo>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>
#include "HtmlParser.h"

#include "getopt.h"

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

void processFile(struct dirent *dname, const string &dir, ostream &ros, bool verb)
{
	string ext = findExt(dname->d_name);
	if (ext == ".js") return;
	if (ext == ".css") return;
	if (ext == ".jpg") return;
	if (ext == ".png") return;
	ros << dir << '/' << dname->d_name << endl;
	if (ext == ".html" || ext == ".htm") {
		string ExtractedText;
		ifstream tempi(dir + "/" + dname->d_name);
		HtmlParser htp(tempi, ros, verb);
		if (verb) {
			ros << htp.readCharset();
			if (htp.convert2UTF_8()) ros << " converted to utf-8" << endl;
			else ros << " could not convert to utf-8" << endl;
		} else {
			htp.convert2UTF_8();
		}
		htp.quickParse();
		htp.structurize();

		htp.startExtracting();
		while (htp.getExtractedText(ExtractedText)) {
				ros << ExtractedText;
		}
	}
}

void traverse(DIR *p, const string &name, ostream &ros, bool verb)
{
 	struct dirent *pDirent;

 	do {
 		pDirent = readdir(p);
 		if (pDirent) {
			if (pDirent->d_type == DT_REG) {
				processFile(pDirent, name, ros, verb);
			} else if (pDirent->d_type == DT_DIR) {
				if (string(pDirent->d_name) == ".") continue;
				if (string(pDirent->d_name) == "..") continue;
				string path = name + '/' + pDirent->d_name;
				DIR *pD = opendir(path.c_str());
				if (pD) {
					traverse(pD, path, ros, verb);
					closedir(pD);
				}
			}
 		}
 	} while (pDirent);
}

bool traverseDirectory(char* dname, ostream &ros, bool verb) {
	DIR* pDir;
	pDir = opendir(dname);
	if (!pDir) {
		cerr << "Could not open directory " << dname[1] << endl;
		return false;
	}
	traverse(pDir, dname, ros, verb);
	closedir(pDir);
	return true;
}

void useSingleFile(istream &ris, ostream &ros, bool verb) {
	string ExtractedText;
	HtmlParser htp(ris, ros, verb);
	if (verb) {
		ros << htp.readCharset();
		if (htp.convert2UTF_8())
			ros << " converted to utf-8" << endl;
		else
			ros << " could not convert to utf-8" << endl;
	} else {
		htp.convert2UTF_8();
	}
	htp.quickParse();
	htp.structurize();
	if (verb) {
		htp.print();
		htp.printET();
	}
	htp.startExtracting();
	while (htp.getExtractedText(ExtractedText)) {
		ros << ExtractedText;
	}
}
void usage(char *pname)
{
	cout << "\nUse\t" << pname << "[-o <output file>] [-v]"  << endl;
	cout << "or\t" << pname << " -f <input file> [-o <output file>] [-v]" << endl;
	cout << "or\t" << pname << " -d <input directory> [-o <output file>] [-v]" << endl;
	cout << "\nThe first version reads a single html stream from stdin." << endl;
	cout << "The second version reads a single html stream from <input file>." << endl;
	cout << "The third version traverses <input directory> and reads every file with extension .html or .htm ." << endl;
	cout << "\nAll versions can optionally write to <output file> instead of stdout and add debug info to the output.\n" << endl;
}

int main(int argc, char *argv[]) {
	bool verbose = false;
	enum {file, directory} mode = file;
	int opt = PATH_MAX;
	char input_file[PATH_MAX] = "-";
	char output_file[PATH_MAX] = "-";
	istream *pis;
	ostream *pos;

	while ((opt = getopt(argc, argv, "vhf:d:o:")) != -1) {
		switch (opt) {
		case 'f':
			strcpy(input_file, optarg);
			break;
		case 'd':
			mode = directory;
			strcpy(input_file, optarg);
			break;
		case 'o':
			strcpy(output_file, optarg);
			break;
		case 'v':
			verbose = true;
			break;
		default:
		case 'h':
			usage(argv[0]);
			return 1;	// nothing to do
		}
	}
	if (optind < argc) {
		usage(argv[0]);
		return 2;	// too many arguments
	}
	if (strcmp(input_file, "-") == 0) {
		pis = &cin;
	} else {
		pis = new ifstream(input_file);
	}
	if (strcmp(output_file, "-") == 0) {
		pos = &cout;
	} else {
		pos = new ofstream(output_file);
	}
	switch (mode) {
	case directory:
		traverseDirectory(input_file, *pos, verbose);
		break;
	case file:
		useSingleFile(*pis, *pos, verbose);
		break;
	}
	if (typeid(*pis) == typeid(ifstream))
		delete pis;
	if (typeid(*pos) == typeid(ofstream))
		delete pos;
	return 0;
}
