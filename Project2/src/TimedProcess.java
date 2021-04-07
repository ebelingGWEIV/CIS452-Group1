import java.lang.reflect.*;
import java.util.concurrent.*;

public class TimedProcess {
    private long MiliToLive;
    TimedProcess(long millis) {
        this.MiliToLive = millis;

        // As long as we're multithreaded, I think we can use Thread.sleep(millis) to do the timer part of this.

        }

    private void CloseProcess(String name)
    {
        System.out.println("Closing the processes (but not really)");
    }


}


