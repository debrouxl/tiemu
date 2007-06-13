#include <stdio.h>

int main(void)
{
    FILE *fi, *fo;
    char str[256];
    char *p;

    fi = fopen("input.txt", "rt");
    fo = fopen("output.txt", "wt");

    while(!feof(fi))
    {
        fgets(str, sizeof(str), fi);
        str[strlen(str) - 1] = '\0';

        p = strchr(str, ' ');
        if(p != NULL)
            *p = '\0';

        printf("<%s>\n", str);

        fprintf(fo, "\t{ %s, \"%s\" },\n", str, str);
    }


    fclose(fi);
    fclose(fo);
}

