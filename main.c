#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define error_and_exit(str, ...) printf(str, __VA_ARGS__); \
        exit(1)

void transform_file(const char* filename){
    FILE *source_fp = fopen(filename, "r");
    if (source_fp == NULL)
    {
        error_and_exit("ERROR: Failed to open source file %s for reading!", filename);
    }

    const char* extension = "_CHTML";
    const int extension_len = strlen(extension);
    const int filename_len = strlen(filename);
    const int dot_pos =  abs(strrchr(filename, '.') - filename);

    char out_filename[filename_len + extension_len + 128];
    memcpy(out_filename, filename, dot_pos);
    memcpy(out_filename + dot_pos, extension, extension_len);
    memcpy(out_filename + dot_pos + extension_len, filename + dot_pos, filename_len - dot_pos);

    FILE *out_fp = fopen(out_filename, "w");
    if (out_fp == NULL){
        error_and_exit("ERROR: Failed to open file %s for writing!", filename);
    }

    size_t read_buffer_size = 0;
    char *line = NULL;
    
    bool first_line = false;
    bool parsing = false;
    bool in_c_tag = false;
    while(getline(&line, &read_buffer_size, source_fp) != -1)
    {
        if (strstr(line, "CHTML_START") != NULL)
        {
            parsing = true;
            first_line = true;
            fprintf(out_fp, "CHTML_EMIT(");
        }
        else if (strstr(line, "CHTML_END"))
        {
            parsing = false;
            fprintf(out_fp, ");\n");
        }
        else if (parsing && strstr(line, "<c>"))
        {
            in_c_tag = true;
            fprintf(out_fp, ");\n");
        }
        else if (parsing && strstr(line, "</c>"))
        {
            in_c_tag = false;
            first_line = true;
            fprintf(out_fp, "CHTML_EMIT(");
        }
        else if (parsing && !in_c_tag)
        {
            char* newline = strrchr(line, '\n');
            if (!first_line)
            {
                fprintf(out_fp, "\n");
            }
            first_line = false;
            if(newline != NULL) 
            {
                *newline = '\"';
                fprintf(out_fp, "\"%s", line);
            }
            else
            {
                fprintf(out_fp, "\"%s\"", line);
            }
        }
        else
        {
            fprintf(out_fp, "%s", line);
        }
    }

    free(line);
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        transform_file(argv[i]);
    }

    return 0;
}