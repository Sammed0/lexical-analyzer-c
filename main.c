#include<stdio.h>
#include<ctype.h>
#include<string.h>

int iskeyword(char *buffer)
{
    char *keyword[] = {
        "int","float","char","double","short","if","else","while","for",
        "return","break","continue","long","void","switch","case","default",
        "do","goto","const","signed","unsigned","static","struct","union",
        "typedef","enum","sizeof"
    };
    int len = sizeof(keyword) / sizeof(keyword[0]);

    for(int i = 0; i < len; i++)
    {
        if(strcmp(keyword[i], buffer) == 0)
            return 1;
    }
    return 0;
}

int isoperator(char buffer)
{
    char *operator[] = {"-","+","*","/","%","=","<",">","!","&","|","^","~"};
    int len = sizeof(operator) / sizeof(operator[0]);

    for(int i = 0; i < len; i++)
    {
        if(strchr(operator[i], buffer) != NULL)
            return 1;
    }
    return 0;
}

int isdelimiter(char buffer)
{
    char *delimiter[] = {";",",","{","}","[","]","(",")"};
    int len = sizeof(delimiter) / sizeof(delimiter[0]);

    for(int i = 0; i < len; i++)
    {
        if(strchr(delimiter[i], buffer) != NULL)
            return 1;
    }
    return 0;
}

int is_duplicate_string(char arr[][100], int count, char *value)
{
    for(int i = 0; i < count; i++)
    {
        if(strcmp(arr[i], value) == 0)
            return 1;
    }
    return 0;
}

int is_duplicate_operator(char arr[][10], int count, char *value)
{
    for(int i = 0; i < count; i++)
    {
        if(strcmp(arr[i], value) == 0)
            return 1;
    }
    return 0;
}

int is_duplicate_delimiter(char arr[], int count, char value)
{
    for(int i = 0; i < count; i++)
    {
        if(arr[i] == value)
            return 1;
    }
    return 0;
}

int main()
{
    FILE *fp = fopen("code.c", "r");
    if(fp == NULL)
    {
        printf("File not found\n");
        return 0;
    }

    int ch;
    int i = 0;
    int unterminated_comment = 0;
    char buffer[100];

    char keyword_buffer[100][100];
    char identifier_buffer[100][100];
    char operator_buffer[100][10];
    char delimiter_buffer[100];
    char string_buffer[100][100];
    char char_literal_buffer[100][100];
    char preprocessor_buffer[100][100];
    char constant_buffer[100][100];

    int key_count = 0, iden_count = 0, op_count = 0, delim_count = 0;
    int const_count = 0, string_count = 0, char_literal_count = 0, preprocessor_count = 0;

    while((ch = fgetc(fp)) != EOF)
    {
        if(isspace(ch))
            continue;

        if(ch == '#')
        {
            i = 0;
            buffer[i++] = ch;
            while((ch = fgetc(fp)) != EOF && ch != '\n')
            {
                if(i < 99)
                    buffer[i++] = ch;
            }
            buffer[i] = '\0';

            if(preprocessor_count < 100 && !is_duplicate_string(preprocessor_buffer, preprocessor_count, buffer))
                strcpy(preprocessor_buffer[preprocessor_count++], buffer);
        }
        else if(isalpha(ch) || ch == '_')
        {
            i = 0;
            buffer[i++] = ch;
            while((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_'))
            {
                if(i < 99)
                    buffer[i++] = ch;
            }
            buffer[i] = '\0';

            if(iskeyword(buffer))
            {
                if(key_count < 100 && !is_duplicate_string(keyword_buffer, key_count, buffer))
                    strcpy(keyword_buffer[key_count++], buffer);
            }
            else
            {
                if(iden_count < 100 && !is_duplicate_string(identifier_buffer, iden_count, buffer))
                    strcpy(identifier_buffer[iden_count++], buffer);
            }

            if(ch != EOF)
                fseek(fp, -1, SEEK_CUR);
        }
        else if(ch == '/')
        {
            int next = fgetc(fp);
            if(next == '/')
            {
                while((ch = fgetc(fp)) != EOF && ch != '\n');
            }
            else if(next == '*')
            {
                int prev = 0;
                while((ch = fgetc(fp)) != EOF)
                {
                    if(ch == '/' && prev == '*')
                        break;
                    prev = ch;
                }
                if(ch == EOF)
                    unterminated_comment = 1;
            }
            else
            {
                if(next != EOF)
                    fseek(fp, -1, SEEK_CUR);
                if(op_count < 100 && !is_duplicate_operator(operator_buffer, op_count, "/"))
                    strcpy(operator_buffer[op_count++], "/");
            }
        }
        else if(isdigit(ch))
        {
            int dot_flag = 0;
            i = 0;
            buffer[i++] = ch;

            while((ch = fgetc(fp)) != EOF)
            {
                if(isdigit(ch))
                {
                    if(i < 99)
                        buffer[i++] = ch;
                }
                else if(ch == '.' && !dot_flag)
                {
                    dot_flag = 1;
                    if(i < 99)
                        buffer[i++] = ch;
                }
                else
                {
                    break;
                }
            }

            buffer[i] = '\0';
            if(ch != EOF)
                fseek(fp, -1, SEEK_CUR);

            if(const_count < 100 && !is_duplicate_string(constant_buffer, const_count, buffer))
                strcpy(constant_buffer[const_count++], buffer);
        }
        else if(isoperator(ch))
        {
            int next = fgetc(fp);
            char op[10];

            op[0] = ch;
            op[1] = '\0';

            if(next != EOF)
            {
                if((ch == '+' && (next == '+' || next == '=')) ||
                   (ch == '-' && (next == '-' || next == '=' || next == '>')) ||
                   (ch == '=' && next == '=') ||
                   (ch == '<' && (next == '=' || next == '<')) ||
                   (ch == '>' && (next == '=' || next == '>')) ||
                   (ch == '!' && next == '=') ||
                   (ch == '&' && (next == '&' || next == '=')) ||
                   (ch == '|' && (next == '|' || next == '=')) ||
                   (ch == '*' && next == '=') ||
                   (ch == '%' && next == '=') ||
                   (ch == '^' && next == '='))
                {
                    op[0] = ch;
                    op[1] = next;
                    op[2] = '\0';

                    if((ch == '<' && next == '<') || (ch == '>' && next == '>'))
                    {
                        int third = fgetc(fp);
                        if(third == '=')
                        {
                            op[2] = third;
                            op[3] = '\0';
                        }
                        else if(third != EOF)
                        {
                            fseek(fp, -1, SEEK_CUR);
                        }
                    }
                }
                else
                {
                    fseek(fp, -1, SEEK_CUR);
                }
            }

            if(op_count < 100 && !is_duplicate_operator(operator_buffer, op_count, op))
                strcpy(operator_buffer[op_count++], op);
        }
        else if(isdelimiter(ch))
        {
            if(delim_count < 100 && !is_duplicate_delimiter(delimiter_buffer, delim_count, ch))
                delimiter_buffer[delim_count++] = ch;
        }
        else if(ch == '"')
        {
            i = 0;
            while((ch = fgetc(fp)) != EOF && ch != '"')
            {
                if(i < 99)
                    buffer[i++] = ch;
            }
            buffer[i] = '\0';

            if(ch == EOF)
                printf("Error: unterminated string literal\n");
            else if(string_count < 100 && !is_duplicate_string(string_buffer, string_count, buffer))
                strcpy(string_buffer[string_count++], buffer);
        }
        else if(ch == '\'')
        {
            i = 0;
            buffer[i++] = ch;
            while((ch = fgetc(fp)) != EOF)
            {
                if(i < 99)
                    buffer[i++] = ch;
                if(ch == '\'')
                    break;
            }
            buffer[i] = '\0';

            if(ch == EOF)
                printf("Error: unterminated character literal\n");
            else if(char_literal_count < 100 && !is_duplicate_string(char_literal_buffer, char_literal_count, buffer))
                strcpy(char_literal_buffer[char_literal_count++], buffer);
        }
    }

    if(unterminated_comment)
        printf("Error: unterminated comment\n");

    printf("KEYWORD : ");
    for(int i = 0; i < key_count; i++)
    {
        printf("%s ", keyword_buffer[i]);
    }
    printf("\n");

    printf("IDENTIFIERS : ");
    for(int i = 0; i < iden_count; i++)
    {
        printf("%s ", identifier_buffer[i]);
    }
    printf("\n");

    printf("CONSTANT : ");
    for(int i = 0; i < const_count; i++)
    {
        printf("%s ", constant_buffer[i]);
    }
    printf("\n");

    printf("OPERATOR : ");
    for(int i = 0; i < op_count; i++)
    {
        printf("%s ", operator_buffer[i]);
    }
    printf("\n");

    printf("DELIMITER : ");
    for(int i = 0; i < delim_count; i++)
    {
        printf("%c ", delimiter_buffer[i]);
    }
    printf("\n");

    printf("STRING LITERAL : ");
    for(int i = 0; i < string_count; i++)
    {
        printf("%s ", string_buffer[i]);
    }
    printf("\n");

    printf("CHAR LITERAL : ");
    for(int i = 0; i < char_literal_count; i++)
    {
        printf("%s ", char_literal_buffer[i]);
    }
    printf("\n");

    printf("PREPROCESSOR : ");
    for(int i = 0; i < preprocessor_count; i++)
    {
        printf("%s ", preprocessor_buffer[i]);
    }
    printf("\n");

    fclose(fp);
    return 0;
}
