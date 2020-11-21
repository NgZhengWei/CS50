#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int checkAlpha(string key);
int checkRepetition(string key);

int main(int argc, string argv[])
{
    string key = argv[1];

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(key) != 26)
    {
        printf("Key must contain 26 letters.\n");
        return 1;
    } else if (checkAlpha(key) == 1)
    {
        printf("Key cannot contain numbers or special characters.\n");
        return 1;
    }
    else if (checkRepetition(key) == 1)
    {
        printf("Key must contain 26 unique letters.\n");
        return 1;
    }

    string plainText = get_string("plaintext:  ");

    int plainTextLength = strlen(plainText);
    char cipherSequence[26];
    char cipherText[plainTextLength + 1];

    for (int i = 0; i < 26; i++)
    {
        cipherSequence[i] = (char) key[i];
    }

    for (int i = 0; i < plainTextLength; i++)
    {
        int letterInt = (int) plainText[i];
        if (letterInt > 64 && letterInt < 91)
        {
            int letterPos = letterInt - 65;
            cipherText[i] = toupper(cipherSequence[letterPos]);
        }
        else if (letterInt > 96 && letterInt < 123)
        {
            int letterPos = letterInt - 97;
            cipherText[i] = cipherSequence[letterPos];
        }
        else
        {
            cipherText[i] = (char) plainText[i];
        }

    }
    cipherText[plainTextLength] = '\0';

    printf("ciphertext: %s" , (string)cipherText);

    return 0;
}

int checkAlpha(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (isalpha(key[i]))
        {
            continue;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

int checkRepetition(string key)
{
    for (int i = 0; key[i]; i++)
    {
        key[i] = tolower(key[i]);
    }

    for (int i = 0, n = strlen(key) - 1; i < n; i++)
    {
        int charCount = 0;

        for (int j = i + 1, k = strlen(key); j < k; j++)
        {
            if (key[i] == key[j])
            {
                charCount++;
            }
        }

        if (charCount > 0)
        {
            return 1;
        }

    }

    return 0;
}