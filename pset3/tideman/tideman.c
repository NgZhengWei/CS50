#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool follow(int target, int from);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < (candidate_count - 1); i++)
    {
        int repeat = candidate_count - (i+1);
        for (int j = 0; j < repeat; j++)
        {
            preferences[ranks[i]][ranks[j+i+1]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < (candidate_count - 1); i++)
    {
        for (int j = 0, n = candidate_count - (i+1); j < n; j++)
        {
            if (preferences[i][j+i+1] > preferences[j+i+1][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j + i + 1;
                pair_count++;
            }
            else if (preferences[i][j+i+1] < preferences[j+i+1][i])
            {
                pairs[pair_count].loser = i;
                pairs[pair_count].winner = j + i + 1;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        int largest = i;
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            if (preferences[pairs[largest].winner][pairs[largest].loser] - preferences[pairs[largest].loser][pairs[largest].winner] < preferences[pairs[j+i+1].winner][pairs[j+i+1].loser] - preferences[pairs[j+i+1].loser][pairs[j+i+1].winner])
            {
                largest = j + i + 1;
            }
        }
        if (largest != i)
        {
            pair tmp = pairs[i];
            pairs[i] = pairs[largest];
            pairs[largest] = tmp;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        if (follow(pairs[i].winner, pairs[i].winner) == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int countFalse = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                countFalse++;
            }
        }
        if (countFalse == candidate_count)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}

bool follow(int target, int from)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[from][i] == true)
        {
            if (i == target)
            {
                return true;
            }
            else
            {
                if (follow(target, i) == true)
                {
                    return true;
                }
            }
        }
        else
        {
            continue;
        }
    }
    return false;
}