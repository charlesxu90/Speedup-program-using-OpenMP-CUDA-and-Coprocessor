// determine tomorrow's date

#include <stdio.h>
#include <stdbool.h>

struct date
{
  int day;
  int month;
  int year;
};

int main (void)
{
  struct date today, tomorrow;
  int DaysInMonth (struct date d);

  while (true) 
    {
      printf ("Enter the date (dd mm yyyy), or 0 0 0 to terminate:\n");
      fflush(stdout);
      scanf ("%i%i%i", &today.day, &today.month, &today.year);

      if (today.day == 0 && today.month == 0 && today.year == 0)
        return 0;
      if (today.day != DaysInMonth(today))
        {
          tomorrow   = today;
          tomorrow.day = today.day + 1;
        }
      else
        {
          if (today.month >= 12)
            {
              tomorrow.day = 1;
              tomorrow.month = 1;
              tomorrow.year = today.year + 1;
            }
          else
            {
              tomorrow.day = 1;
              tomorrow.month = today.month + 1;
              tomorrow.year = today.year;
            }
        }
      printf ("tomorrow's date is %i/%i/%i.\n", tomorrow.day, tomorrow.month,
              tomorrow.year);

    }
};


//
// Calculate the number of days in a month
//
int DaysInMonth (struct date d)
{
  int days;
  bool LeapYear (struct date d);
  const int MonthLength[12] =
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  if (d.month != 2)
    days = MonthLength[d.month-1];
  else
    {
      if (!LeapYear(d))
        days = MonthLength[1];
      else
        days = MonthLength[1] +   1;

    }

  return days;

}

//
// Check whether it is a leap year
//
bool LeapYear (struct date d)
{
  bool LeapLocal;

  if ( ((d.year % 4 == 0) && (d.year % 100 != 0)) || (d.year % 400 == 0))
    LeapLocal = true;
  else
    LeapLocal = false;

  return LeapLocal;

}
