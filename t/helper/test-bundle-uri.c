#include "test-tool.h"
#include "parse-options.h"
#include "bundle-uri.h"
#include "strbuf.h"
#include "string-list.h"

static int cmd__bundle_uri_parse(int argc, const char **argv)
{
	const char *key_value_usage[] = {
		"test-tool bundle-uri parse-key-values <input>",
		NULL
	};
	const char **usage = key_value_usage;
	struct option options[] = {
		OPT_END(),
	};
	struct strbuf sb = STRBUF_INIT;
	struct bundle_list list;
	int err = 0;
	FILE *fp;

	argc = parse_options(argc, argv, NULL, options, usage, 0);
	if (argc != 1)
		goto usage;

	init_bundle_list(&list);
	fp = fopen(argv[0], "r");
	if (!fp)
		die("failed to open '%s'", argv[0]);

	while (strbuf_getline(&sb, fp) != EOF) {
		if (bundle_uri_parse_line(&list, sb.buf))
			err = error("bad line: '%s'", sb.buf);
	}
	strbuf_release(&sb);
	fclose(fp);

	print_bundle_list(stdout, &list);

	clear_bundle_list(&list);

	return !!err;

usage:
	usage_with_options(usage, options);
}

int cmd__bundle_uri(int argc, const char **argv)
{
	const char *usage[] = {
		"test-tool bundle-uri <subcommand> [<options>]",
		NULL
	};
	struct option options[] = {
		OPT_END(),
	};

	argc = parse_options(argc, argv, NULL, options, usage,
			     PARSE_OPT_STOP_AT_NON_OPTION |
			     PARSE_OPT_KEEP_ARGV0);
	if (argc == 1)
		goto usage;

	if (!strcmp(argv[1], "parse-key-values"))
		return cmd__bundle_uri_parse(argc - 1, argv + 1);
	error("there is no test-tool bundle-uri tool '%s'", argv[1]);

usage:
	usage_with_options(usage, options);
}
