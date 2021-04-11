import java.sql.Array;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Vector;

public class MemorySpace {
    private TimedProcess[] MyMemSpace;
    private List<TimedProcess> procQueue;
    private int Max;

    public MemorySpace(int maxSize) {
        MyMemSpace = new TimedProcess[maxSize];
        procQueue = new Vector<>();

        Max = maxSize;
    }

    /***
     * Is run before at the start of every new cycle (1 second)
     */
    public void NextCycle(TimedProcess newProc)
    {
        procQueue.add(newProc);

        for(int index = 0; index < Max; index++)
        {
            if(MyMemSpace[index] != null) {
                MyMemSpace[index].NewCycle();
                if (MyMemSpace[index].GetTimeToLive() <= 0) {
                    MyMemSpace[index] = null;
                }
            }
        }
    }

    /***
     * Add a new process, if possible, to the memory space using the First Fit method.
     */
    public void AddFirstFit()
    {
        for(int index = 0; index < procQueue.size(); index++)
        {
            var proc = procQueue.get(index);
            if (proc != null) {
                if(AddFirstFit(proc)) //Try to add it to the memory space
                {
                    procQueue.remove(index); //If we do, remove the process from the queue
                    return;
                }
            }
        }
    }

    /***
     * Add a new process, if possible, to the memory space using the Best Fit method.
     */
    public void AddBestFit()
    {
        for(int index = 0; index < procQueue.size(); index++)
        {
            var proc = procQueue.get(index);
            if (proc != null) {
                if(AddBestFit(proc)) //Try to add it to the memory space
                {
                    procQueue.remove(index); //If we do, remove the process from the queue
                    return;
                }
            }
        }
    }

    /***
     * Add a new process, if possible, to the memory space using the Worst Fit method.
     */
    public void AddWorstFit()
    {
        for(int index = 0; index < procQueue.size(); index++)
        {
            var proc = procQueue.get(index);
            if (proc != null) {
                if(AddWorstFit(proc)) //Try to add it to the memory space
                {
                    procQueue.remove(index); //If we do, remove the process from the queue
                    return;
                }
            }
        }
    }

    /**
     * Add using the best fit method.
     * @param proc The timed process to add to the memory space.
     * @return true If the process was added.
     */
    private boolean AddFirstFit(TimedProcess proc) {
        int startIndex = 0;
        int index;
        int holeSize = 0;
        //Loop through the whole list
        for (index = 0; index < Max; index++) {
            // See if the spot is empty
                if (MyMemSpace[index] == null) {
                    holeSize++;
                    // If we found a hole large enough
                    if (holeSize == proc.Size) {
                        Fill(startIndex, index, proc);
                        return true;
                    }
                }
                // Spot was not empty
                else{
                    holeSize = 0;
                    startIndex = index + 1;

            }
        }
            return false;
    }

    private boolean AddBestFit(TimedProcess proc)
    {
        //todo Write this method
        System.out.println("Not implemented\n");
        return false;
    }

    private boolean AddWorstFit(TimedProcess proc)
    {
        //todo Write this method
        System.out.println("Not implemented\n");
        return false;
    }

    /***
     * Add a proc into each space. This allows us to keep track of how long the space should be filled
     * @param start Inclusive index of where to start filling memory.
     * @param end Inclusive index of the last spot to be filled.
     * @param proc The process being added.
     */
    private void Fill(int start, int end, TimedProcess proc)
    {
        while(start <= end)
        {
            MyMemSpace[start++] = proc;
        }
    }

    @Override
    public String toString()
    {
      StringBuilder out = new StringBuilder("[");

      for(int index = 0; index < Max; index++)
      {
          String spot;
          try{
              spot = (MyMemSpace[(index)] != null) ? "Y" : "_";
          }
          catch (Exception ex)
          {
              spot = "_";
          }
          out.append(spot);

      }
      out.append("]");
      return out.toString();
    }
}
