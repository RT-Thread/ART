/**
 * initialization script
 */
#include <dfs_posix.h>
#include <finsh.h>
#include <shell.h>

static rt_uint32_t read_line(int fd, char* line, rt_uint32_t line_size)
{
	char ch, *ptr;
	int length;

	ptr = line;
	while (line_size)
	{
		length = read(fd, &ch, 1);
		if (length <= 0)
		{
			goto __exit;
		}

		if (ch == '\n')
		{
			*ptr = ch;
			ptr ++;
			goto __exit;
		}

		*ptr = ch;

		ptr ++;
		line_size --;
	}

__exit:
	return ptr - line;
}

void script_exec(const char* filename)
{
	int fd;
	char line[128];
	int line_len, line_no;
	struct finsh_parser parser;

	fd = open(filename, O_RDONLY, 0);
	if (fd >= 0)
	{
		/* create a finsh shell environment */
		finsh_init(&parser);
		line_no = 0;

		while (1)
		{
			/* read line and execute it */
			line_len = read_line(fd, line, sizeof(line) - 1);
			if (line_len > 0)
			{
				line[line_len] = '\0';
				line_no ++;

				{
					finsh_parser_run(&parser, (unsigned char*)line);

					/* compile node root */
					if (finsh_errno() == 0)
					{
						finsh_compiler_run(parser.root);
					}
					else
					{
						const char* err_str;
						err_str = finsh_error_string(finsh_errno());
						rt_kprintf("%s:%d: error %s\n", line_no, err_str);
					}

					/* run virtual machine */
					if (finsh_errno() == 0)
					{
						finsh_vm_run();
					}

				    finsh_flush(&parser);
				}
			}
			else break;
		}

		close(fd);
	}
}

/* do all modules on a directory */
#define FILENAME_MAX	256
void do_modules(const char *path)
{
	char *fn;
	DIR  *dir;

	fn = rt_malloc(FILENAME_MAX);
	if (fn == RT_NULL)
	{
		rt_kprintf("out of memory\n");
		return;
	}

	dir = opendir(path);
	if (dir != RT_NULL)
	{
		struct dirent* dirent;
		struct stat s;

		do
		{
			dirent = readdir(dir);
			if (dirent == RT_NULL) break;
			rt_memset(&s, 0, sizeof(struct stat));

			/* build full path for each file */
			if (path[strlen(path) - 1] != '/')
				rt_sprintf(fn, "%s/%s", path, dirent->d_name);
			else
				rt_sprintf(fn, "%s%s", path, dirent->d_name);

			if (strstr(fn, ".mo") != RT_NULL)
			{
				/* execute this module */
				rt_module_open(fn);
			}
		} while (dirent != RT_NULL);

		closedir(dir);
	}
	else rt_kprintf("open %s directory failed\n", path);

	rt_free(fn);
}
FINSH_FUNCTION_EXPORT_ALIAS(do_modules, exec_modules, executes all modules on a directory);

void do_init(void)
{
	script_exec("/init.rc");
}
FINSH_FUNCTION_EXPORT(do_init, do a init rc file);
FINSH_FUNCTION_EXPORT_ALIAS(rt_kprintf, printf, print formatted output);
