#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Y = 1
#define y = 1

#define POINST_PER_MEMBER	60	// The amount of points that each members have to distribute amongst the other members
#define MAX_NUM_OF_MEMBERS	20 

typedef struct {
	int points, pointsLeft;
	char name[21];
	float percent;
	int* givenPoints;
}member;

void	giveInstructions();
member	createNewMember(int i, int noOfMembers);
int		getNoOfMembers();
void	printPoints(member *group, int noOfMembers);
member* getPercent(member *group, int noOfMembers, int maxPoints);
void	printPercentage(member *group, int noOfMembers, int maxPoints);
void	cls(); //clears screen.
int		promtPrint();
void	saveFile(member *group, int noOfMembers);

void main(void)
{
	giveInstructions();

	int noOfMembers = getNoOfMembers();					
	int maxPoints = noOfMembers * POINST_PER_MEMBER;	// Sum of all the members points
	int goAgain;										// Flag if the user wants to redo the distribution 

	printf("\nEnter the names of the members\n(Max 20 characters and no space in the names.)\n\n");
	
	member *group = malloc(sizeof(member)*noOfMembers);
	for (int i = 0; i < noOfMembers; i++)
		group[i] = createNewMember(i, noOfMembers);

	int dealer, reciever;	// dealer is the current member that distributes points
							// reciever is the current member that recieves the points
	

	for (dealer = 0; dealer < noOfMembers; dealer++)
	{
		do {
			cls();
			printf("%s's time to enter points. Get ready.\nDivide %d points based on contribution to the project.\n", group[dealer].name, POINST_PER_MEMBER);
			printf("For an even distribution enter %d points per member.\n-----------\n", POINST_PER_MEMBER/(noOfMembers - 1));
			
			goAgain = 1; // Resets flag
			for (reciever = 0; reciever < noOfMembers; reciever++) // delar ut poäng till varje gruppkamrat
			{
				int tempScore;
				if (reciever == dealer)	// Can't give points to your self
					continue;
				do {
					printf("You've got %d points left\n", group[dealer].pointsLeft);
					printf("Points to %s: ", group[reciever].name);
					scanf(" %d", &tempScore);
					printf("\n");

					if (group[dealer].pointsLeft < tempScore) 
						printf("Too many points. Try again\n");
				} while (tempScore > group[dealer].pointsLeft);

				group[dealer].givenPoints[reciever] = tempScore;
				group[dealer].pointsLeft -= tempScore;
			}
			
			// If dealer still have points left to spend
			if (group[dealer].pointsLeft > 0)
			{
				printf("You still got %d points left. Do it again please.\n", group[dealer].pointsLeft);
				system("pause");
				group[dealer].pointsLeft = POINST_PER_MEMBER;	// Resets the dealers points
			}													// goAgain is still set to true
			else 
			{ 	
				// Prints the current point distribution.
				printf("-----------\n");
				for (reciever = 0; reciever < noOfMembers; reciever++)
				{
					if (reciever == dealer)
						continue;
					printf("You gave %s %d points\n", group[reciever].name, group[dealer].givenPoints[reciever]);
					group[reciever].points += group[dealer].givenPoints[reciever];
				}

				printf("Are you happy with your distribution of points? (y/n): ");
				char c;
				scanf(" %c", &c);
							
				if (c == 'y' || c == 'Y')
					goAgain = 0;
				else
					group[dealer].pointsLeft = POINST_PER_MEMBER;
				
				cls();
			}
		} while (goAgain);
	}
	// All members has distributed all points.
	
	cls();
	group = getPercent(group, noOfMembers, maxPoints); 
	printPercentage(group, noOfMembers, maxPoints);	   

	// Save to file
	if (promtPrint())
		saveFile(group, noOfMembers);

	free(group);
	system("pause");
}


void giveInstructions()
{
	printf("This program let every member in a group evaluate the other members. \nThis is done by distribute points based on contribution to the project. \nEvery member gets %d points.\n\n", POINST_PER_MEMBER);
	printf("-----------\n");
}

member createNewMember(i, noOfMembers)
{
	member temp;	
	temp.givenPoints = malloc(sizeof(int)* noOfMembers);
	temp.points = 0; 
	temp.pointsLeft = POINST_PER_MEMBER;
	char tempName[21]; 
	printf("Name nr %d: ", i + 1);
		scanf("%s", tempName); 

	strcpy(temp.name, tempName);

	return temp;
}

int getNoOfMembers()
{
	int num;
	do {
		printf("Enter the number of members in your group (at least 3 and max %d): ", MAX_NUM_OF_MEMBERS);
		scanf(" %d", &num);
	} while (num < 3 || num > MAX_NUM_OF_MEMBERS);
	return num;
}

void printPoints(member *group, int noOfMembers)
{
	for (int i = 0; i < noOfMembers; i++)
		printf("%s has %d points\n", group[i].name, group[i].points);
}

member* getPercent(member *group, int noOfMembers, int maxPoints)
{
	for (int i = 0; i < noOfMembers; i++)
		group[i].percent = ((float)group[i].points / maxPoints) * 100;
	return group;
}

void printPercentage(member *group, int noOfMembers, int maxPoints)
{
	printf("Here is the final percentage:\n\n");
	for (int i = 0; i < noOfMembers; i++)
		printf("%s - %.1f %%\n", group[i].name, group[i].percent);

	printf("\n");
}

void cls()
{
	system("cls");
}

int promtPrint()
{
	printf("Would you like to print out a file with results? (y/n): ");
	fflush(stdin);	
	char c;
	scanf(" %c", &c);

	return (c == 'y' || c == 'Y');
}

void saveFile(member *group, int noOfMembers)
{
	FILE *outfile;
	outfile = fopen("Final scores.txt", "w");
	if (outfile == NULL)
	{
		printf("Error, can't open file.");
		return;
	}
	
	fprintf(outfile, "Here is the final percentage:\n\n");

	for (int i = 0; i < noOfMembers; i++)
		fprintf(outfile, "%s - %.1f %%\n", group[i].name, group[i].percent);

	fclose(outfile);
	printf("\nFile is created.\n\n");
}