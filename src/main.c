#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

#include <persist-state.h>

struct arguments {
	char* filename;
};

static char argp_doc[] = "syslog-ng-persist - simple syslog-ng persist file tool";
static struct argp_option argp_options[] = {
        { 0 }
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;

	switch (key) {
	case ARGP_KEY_ARG:
		if (state->arg_num >= 1)
			argp_usage (state);
		arguments->filename = arg;
	break;
	case ARGP_KEY_END:
		if (state->arg_num < 1)
			argp_usage (state);
	break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static char args_doc[] = "FILENAME";
static struct argp argp = { argp_options, parse_opt, args_doc, argp_doc };

int parse_args(int argc, char** argv, struct arguments* parguments) {
	return argp_parse(&argp, argc, argv, 0, 0, parguments);
}

int main(int argc, char** argv) {
	struct arguments args;

	if (parse_args(argc, argv, &args)) {
		return 1;
	}

	PersistState* state = persist_state_new(args->filename);

	persist_state_free(state);

	return 0;
}
