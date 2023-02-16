#ifndef CLEANUP
#define CLEANUP
#define ERROR_AND_EXIT()          \
	printf(GENERAL_ERROR_MSG);    \
	return 1
#define GENERAL_ERROR_MSG "An Error Has Occurred"
#endif
