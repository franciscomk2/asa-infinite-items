

int counter = 0;
void TimerCallback()
{

    // Sample every 5 secs
    if (counter % 5 == 0)
    {
        // do something
    }

    counter++;

    // reset counter
    //if (counter == 1440) counter = 0;
}