#include <stdio.h>
#include <getopt.h>
#include <iostream>
#include "Diagnostics.h"

extern FILE* yyin;
extern int yydebug;
int yyparse();

int main(int argc,char **argv)
{
	int rc = 0;
	int opt;
	static const option options[] = {
		{"debug", required_argument, NULL, 'd'},
		{"warn", required_argument, NULL, 'w'},
	};

	Location::current.m_file = "<command line>";

	while ((opt = getopt_long(argc, argv, "d:w:", options, NULL)) != -1) {
		int level = 0;

		switch (opt)
		{
		case 'd':
			if (1 == sscanf(optarg, "%i", &level)) {
				getDiagnosticsSetup().setDebugLevel(level);
			} else {
				getDiagnostics().warn(0, Location::current)
					<< "Bad debug level: '" << optarg << "'" << std::endl;
			}
			break;

		case 'w':
			if (1 == sscanf(optarg, "%i", &level)) {
				getDiagnosticsSetup().setWarnLevel(level);
			} else {
				getDiagnostics().warn(0, Location::current)
					<< "Bad warn level: '" << optarg << "'" << std::endl;
			}
			break;
		}
	}

	if (optind < argc) {
		yyin = fopen(argv[optind], "r");
		Location::current.m_file = argv[optind];
	} else {
		yyin = stdin;
		Location::current.m_file = "<stdin>";
	}

	//  yydebug=1;

	try {
		yyparse();

		if (!getDiagnostics().isClear()) {
			throw "Parse errors detected, bailing out";
		}
	} catch(const char* m) {
		std::cerr << m << std::endl;
		rc = 1;
	}

	return rc;
}

int yyerror(const char *p)
{
	getDiagnostics().error(Location()) << p << std::endl;
	return 0;
}

extern "C" int yywrap(void)
{
	return 1;
}
