import java.lang.Thread;
import java.lang.management.MemoryManagerMXBean;
import java.util.LinkedList;
import java.util.Queue;

public class MemoryController {
    public boolean Run = true;
    public static final int WaitTime = 1000; //mili

    private MemorySpace FirstFit;
    private MemorySpace BestFit;
    private MemorySpace WorstFit;

    private Queue<TimedProcess> ProcSequence;

    MemoryController(int maxSpace)
    {
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
        Add(6,2, 6);
        Add(4,4, 7);

    }

    private void Add(int size, int life, int id)
    {
        ProcSequence.add(new TimedProcess(life, size, id));
    }

    public void Run()
    {
        int head = 0;
        int index;
        for(index = 0; ProcSequence.size() > 0; index++)
        {
            var nextProc = ProcSequence.poll();
            if(nextProc != null)
            {
//                FirstFit.NextCycle(nextProc);
                BestFit.NextCycle(nextProc);
//                WorstFit.NextCycle(nextProc);

//                FirstFit.AddFirstFit();
                BestFit.AddBestFit();
//                WorstFit.AddWorstFit();
            }
//            System.out.println("First: " + FirstFit);
            System.out.println("Best:  " + BestFit);
//            System.out.println("Worst: " + WorstFit);
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