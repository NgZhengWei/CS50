#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int calcGrade(float L, float S);

int main(void)
{
    string userText = get_string("Text: ");

    int wordCount = 0;
    int sentenceCount = 0;
    int letterCount = 0;

    for (int i = 0, n = strlen(userText); i < n; i++)
    {
        if (userText[i] == ' ') {
            wordCount++;
        }else if (userText[i] == '!' || userText[i] == '.' || userText[i] == '?'){
            sentenceCount++;
        }else if (isalpha(userText[i])) {
            letterCount++;
        }
    }
    wordCount++;

    //printf("Word Count: %i\nSentence Count: %i\nLetter Count: %i\n", wordCount, sentenceCount, letterCount);

    float L = ((float)letterCount / (float) wordCount) * 100;
    float S = ((float) sentenceCount / (float) wordCount) * 100;

    int grade = calcGrade(L, S);

    if (grade == -1){
        printf("Grade 16+\n");
    } else if (grade == -2) {
        printf("Before Grade 1\n");
    } else {
        printf("Grade %i\n", grade);
    }

}

int calcGrade(float L, float S)
{
    float grade = 0.0588 * L - 0.296 * S - 15.8;
    grade = (int) round(grade);

    if (grade > 16){
        return -1;
    }else if (grade < 1){
        return -2;
    } else {
        return grade;
    }

}