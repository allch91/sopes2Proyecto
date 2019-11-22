#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <time.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

int main()
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    fd = inotify_init();

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    FILE *fp;

    if (fd < 0)
    {
        perror("inotify_init");
    }
    wd = inotify_add_watch(fd, "/", IN_CREATE | IN_DELETE | IN_MODIFY);

    while (1)
    {
        i = 0;
        length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0)
        {
            perror("read");
        }
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len)
            {
                if (event->mask & IN_CREATE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        fp = fopen("/home/abner/log.txt", "a");
                        fprintf(fp, "> Creación de Carpeta '%s' || %s", event->name, asctime(tm));
                        fclose(fp);

                        printf("New directory %s created.\n", event->name);
                        printf("%s", asctime(tm));
                    }
                    else
                    {
                        fp = fopen("/home/abner/log.txt", "a");
                        fprintf(fp, "> Creación de Archivo '%s' || %s", event->name, asctime(tm));
                        fclose(fp);

                        printf("New file %s created.\n", event->name);
                        printf("%s", asctime(tm));
                    }
                }
                else if (event->mask & IN_DELETE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        fp = fopen("/home/abner/log.txt", "a");
                        fprintf(fp, "> Eliminación de Carpeta '%s' || %s", event->name, asctime(tm));
                        fclose(fp);

                        printf("Directory %s deleted.\n", event->name);
                        printf("%s", asctime(tm));
                    }
                    else
                    {
                        fp = fopen("/home/abner/log", "a");
                        fprintf(fp, "> Eliminación de Archivo '%s' || %s", event->name, asctime(tm));
                        fclose(fp);

                        printf("File %s deleted.\n", event->name);
                        printf("%s", asctime(tm));
                    }
                }
                else if (event->mask & IN_MODIFY)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        fp = fopen("/home/abner/log.txt", "a");
                        fprintf(fp, "> Modificación de Carpeta '%s' || %s", event->name, asctime(tm));
                        fclose(fp);

                        printf("Directory %s modified.\n", event->name);
                        printf("%s", asctime(tm));
                    }
                    else
                    {
                        fp = fopen("/home/abner/log", "a");
                        fprintf(fp, "> Modificación de Archivo '%s' -> %s", event->name, asctime(tm));
                        fclose(fp);
                        printf("File %s modified.\n", event->name);
                        printf("%s", asctime(tm));
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}
