import java.lang.reflect.Method;
import java.util.LinkedList;
import java.util.Queue;

public class MemoryController {
    public static final int WaitTime = 1000; //mili

    private MemorySpace FirstFit;
    private MemorySpace BestFit;
    private MemorySpace WorstFit;

    private Queue<TimedProcess> ProcSequence;

    private Method UpdateGUI;
    private Method UpdateTime;
    private Method UpdateNext;

    MemoryController(int maxSpace, Method updateMethod, Method updateTime, Method upNext)
    {
        UpdateGUI = updateMethod;
        UpdateTime = updateTime;
        UpdateNext = upNext;

        ProcSequence = new LinkedList<>();
        FirstFit = new MemorySpace(maxSpace);
        BestFit = new MemorySpace(maxSpace);
        WorstFit = new MemorySpace(maxSpace);

        //Build the process sequence
        Add(3,2, 0);
        Add(3,4, 1);
        Add(5,3, 2);
        Add(3,3, 3);
        Add(3,2, 4);
        Add(2,2, 5);
        Add(6,3, 6);
        Add(4,4, 7);

        Add(3,6, 8);
        Add(4,3, 9);

        Add(3,4, 0);
        Add(3,2, 1);
        Add(2,3, 2);
        Add(3,6, 3);

        Add(4,2, 4);
        Add(2,2, 5);
        Add(4,3, 6);
        Add(2,4, 7);
        Add(1,6, 8);
        Add(3,3, 9);

        Add(3,4, 0);
        Add(3,2, 1);
        Add(5,4, 2);
        Add(3,6, 3);
        Add(4,2, 4);
        Add(2,2, 5);
        Add(4,3, 6);
        Add(2,4, 7);
        Add(3,6, 8);
        Add(3,3, 9);
    }

    private void Add(int size, int life, int id)
    {
        ProcSequence.add(new TimedProcess(life, size, id));
    }

    /**
     * Adds processes and waits and notifies the GUI
     */
    public void Run() {
        int index;
        for(index = 0; ProcSequence.size() > 0; index++)
        {
            var nextProc = ProcSequence.poll();
            if(nextProc != null)
            {
                // Start a new round
                FirstFit.NextCycle();
                BestFit.NextCycle();
                WorstFit.NextCycle();

                // Add the process
                FirstFit.AddFirstFit();
                BestFit.AddBestFit();
                WorstFit.AddWorstFit();

                // Submit a new process to the queue
                FirstFit.AddToQueue(nextProc);
                BestFit.AddToQueue(nextProc);
                WorstFit.AddToQueue(nextProc);
            }

            try {
                System.out.println("First: " + FirstFit + "Best:  " + BestFit + "Worst: " + WorstFit);
                UpdateGUI.invoke(null, 0, FirstFit.toString());
                UpdateNext.invoke(null, 0, FirstFit.GetNextProc());
//                System.out.println("Best:  " + BestFit);
                UpdateGUI.invoke(null, 1, BestFit.toString());
                UpdateNext.invoke(null, 1, BestFit.GetNextProc());
//                System.out.println("Worst: " + WorstFit);
                UpdateGUI.invoke(null, 2, WorstFit.toString());
                UpdateNext.invoke(null, 2, BestFit.GetNextProc());
                UpdateTime.invoke(null);

            }
            catch (Exception ex){
                System.out.println("Couldn't update GUI");
                System.out.println(ex);
            }
            Wait(WaitTime);
            if(index >= ProcSequence.size()) index = 0;
        }
    }


    private void Wait(int mili)
    {
        try {
            Thread.sleep(mili);
        }
        catch (Exception ex){
            System.out.println("Memory controller could not wait\n");
        }
    }
}