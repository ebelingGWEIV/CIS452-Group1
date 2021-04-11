public class TimedProcess {

    private int TimeToLive;
    public final int Size;
    public int ID;

    TimedProcess(int life, int size) {
        this.TimeToLive = life;
        this.Size = size;
    }


    public int GetTimeToLive() {
        return TimeToLive;
    }

    /***
     * Run this upon a new cycle (1 second). Decreases the TimeToLive by 1.
     */
    public void NewCycle() {
        TimeToLive--;
    }


}


