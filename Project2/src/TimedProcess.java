import java.lang.reflect.*;
import java.util.concurrent.*;

public class TimedProcess {
    TimedProcess(long millis) throws NoSuchMethodException {

        ScheduledExecutorService service = Executors.newScheduledThreadPool(1);
        String text = "tester";
        Object phoneNumbers;
        //This is supposed to start a timer that will call the CloseProcess() method when the timer finishes
        service.schedule(new TimerHealthSupport(text, TimedProcess.class.getDeclaredMethod("CloseProcess", String.class)), millis, TimeUnit.MILLISECONDS);
        }

    private void CloseProcess(String name)
    {
        System.out.println("Closing the processes (but not really)");
    }

    // I found this on StackOverflow. I haven't tested it yet.
    // https://stackoverflow.com/questions/43289895/how-to-trigger-events-in-java-at-specific-date-and-time
    private class TimerHealthSupport implements Runnable {
        private final String name;
        private final Method close;

        TimerHealthSupport(String name, Method closer)
        {
            this.name = name;
            this.close = closer;
        }

        // This gets run when the timer runs out
        public void run() {
            try {
                // This invoke is what should call CloseProcess
                this.close.invoke(this.name);
            } catch (IllegalAccessException | InvocationTargetException e) {
                //IntelliJ made this for me
                e.printStackTrace();
            }
        }
    }
}


