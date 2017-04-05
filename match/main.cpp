#include "matchmrocessor.h"
#include <time.h>
#include <windows.h>

int main()
{
	MatchProcessor oProcessor;
	oProcessor.Initialize();

	oProcessor.AddMatching(1, 4, 10);
	oProcessor.AddMatching(2, 2, 50);
	oProcessor.AddMatching(3, 1, 9);
	oProcessor.AddMatching(4, 1, 16);
	oProcessor.AddMatching(5, 3, 17);
	oProcessor.AddMatching(6, 5, 20);
	oProcessor.AddMatching(7, 1, 30);
	oProcessor.AddMatching(8, 1, 50);
	oProcessor.AddMatching(9, 1, 70);
	oProcessor.AddMatching(31, 1, 21);
	oProcessor.AddMatching(32, 1, 22);
	oProcessor.AddMatching(33, 1, 35);

	while (true)
	{
		oProcessor.Update();
		static bool b = false;
		if (!b)
		{
			unsigned int arr[16] = {0};
			if (oProcessor.Matching(arr, 21, 5, 2))
			{
				b = true;
			}
		}
		Sleep(1);
	}
	return 0;
}