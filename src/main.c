#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <argp.h>

#include <persist-state.h>
#include <persistable-state-header.h>

#define MAX_CURSOR_LENGTH 1024

static const char systemd_journal[] = "systemd-journal";

struct journald_state {
	PersistableStateHeader header;
	char cursor[MAX_CURSOR_LENGTH];
};

struct arguments {
	char* filename;
	enum { NONE, LIST, SHOW_JOURNALD_CURSOR, SAVE_JOURNALD_CURSOR } action;
	union {
		char* journald_cursor;
	} action_arg;
};

void set_default_args(struct arguments* pargs) {
	pargs->filename = NULL;
	pargs->action = NONE;
}

static char argp_doc[] = "syslog-ng-persist - simple syslog-ng persist file tool";
static struct argp_option argp_options[] = {
	{ "list", 'l', 0,      0, "List existing entries" },
	{ "show-journald-cursor", 0x101, 0, 0, "Show saved journald cursor" },
	{ "save-journald-cursor", 0x102, "JOURNALD_CURSOR", 0, "Save journald cursor" },
	{ 0 }
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;

	switch (key) {
	case 'l':
		arguments->action = LIST;
	break;
	case 0x101:
		arguments->action = SHOW_JOURNALD_CURSOR;
	break;
	case 0x102:
		arguments->action = SAVE_JOURNALD_CURSOR;
		arguments->action_arg.journald_cursor = arg;
	break;
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

void handle_entry(gchar* name, gint entry_size, gpointer entry, gpointer userdata) {
	printf("%s\n", name);
}

int main(int argc, char** argv) {
	struct arguments args;

	set_default_args(&args);
	if (parse_args(argc, argv, &args)) {
		return 1;
	}

	PersistEntryHandle handle;
	struct journald_state* journald_state = NULL;
	PersistState* state = persist_state_new(args.filename);

	gsize state_size;
	guint8 persist_version;

	persist_state_start_dump(state);
	switch (args.action) {
	case LIST:
		persist_state_foreach_entry(state, &handle_entry, NULL);
	break;
	case SHOW_JOURNALD_CURSOR:
		handle = persist_state_lookup_entry(state, systemd_journal, &state_size, &persist_version);
		journald_state = persist_state_map_entry(state, handle);
		printf("journald cursor: %s\n", journald_state->cursor);
		persist_state_unmap_entry(state, handle);
	break;
	case SAVE_JOURNALD_CURSOR:
		handle = persist_state_alloc_entry(state, systemd_journal, sizeof(struct journald_state));
		journald_state = persist_state_map_entry(state, handle);
		journald_state->header.version = 0;
		journald_state->header.big_endian = (G_BYTE_ORDER == G_BIG_ENDIAN);
		strncpy(journald_state->cursor, args.action_arg.journald_cursor, MAX_CURSOR_LENGTH);
		persist_state_unmap_entry(state, handle);
		persist_state_commit(state);
	break;
	default:;
	}

	persist_state_free(state);

	return 0;
}
