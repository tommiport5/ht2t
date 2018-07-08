//============================================================================
// Name        : Gcc.cpp
// Author      : TH
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "config.h"
#include <iostream>
#include <typeinfo>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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

void convert2UTF_8(int verb, ostream& ros, HtmlParser& htp) {
#ifdef HAVE_ICONV_H
	if (verb) {
		ros << htp.readCharset();
		if (htp.convert2UTF_8())
			ros << " converted to utf-8" << endl;
		else
			ros << " could not be converted to utf-8" << endl;
	} else {
		htp.convert2UTF_8();
	}
#else
	ros << htp.readCharset() << " could not be converted to utf-8, because iconv is missing";
#endif
}

void processFile(struct dirent *dname, const string &dir, ostream &ros, int verb)
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
		HtmlReader tempr(tempi);
		HtmlParser htp(tempr, ros, verb);
		convert2UTF_8(verb, ros, htp);
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
}

void traverse(DIR *p, const string &name, ostream &ros, int verb)
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

bool traverseDirectory(char* dname, ostream &ros, int verb) {
	DIR* pDir;
	pDir = opendir(dname);
	if (!pDir) {
		cerr << "Could not open directory " << dname << endl;
		return false;
	}
	traverse(pDir, dname, ros, verb);
	closedir(pDir);
	return true;
}

void useSingleFile(istream &ris, ostream &ros, int verb) {
	string ExtractedText;
	try {
		HtmlReader ir(ris);
		while (ir.moreFollows()) {
			HtmlParser htp(ir, ros, verb);
			convert2UTF_8(verb, ros, htp);
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
	} catch (regex_error &e) {
		cerr << "ht2ts caught regex_error: 0x" << hex << e.code()<< endl;
		cerr << "This is most probably due to a too old g++ (<= 4.8) or libstdc++" << endl;
	} catch (exception &e) {
		cerr << "ht2t::useSingleFile caught exception: \"" << e.what() << "\"" << endl;
	}
}

void usage(char *pname)
{
	cout << "\n" << pname << "\tHypertext to text converter (extractor), version " << VERSION  << endl;
	cout << "\nUse\t" << pname << "[-o <output file>] [-v]"  << endl;
	cout << "or\t" << pname << " -f <input file> [-o <output file>] [-v]" << endl;
	cout << "or\t" << pname << " -d <input directory> [-o <output file>] [-v]" << endl;
	cout << "\nThe first version reads a html stream from stdin." << endl;
	cout << "The second version reads a html stream from <input file>." << endl;
	cout << "The third version traverses <input directory> and reads every file with extension .html or .htm ." << endl;
	cout << "\nAll versions can optionally write to <output file> instead of stdout and add debug info to the output (with -v, add v's for more detail)." << endl;
}

int main(int argc, char *argv[]) {
	int verbose = 0;
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
			++verbose;
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
