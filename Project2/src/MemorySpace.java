import org.jetbrains.annotations.NotNull;

import java.util.LinkedList;
import java.util.List;
import java.util.Vector;

public class MemorySpace {
    private TimedProcess[] MyMemSpace;
    private List<TimedProcess> procQueue;
    private int Max;

    public TimedProcess[] GetMemorySpace()
    {
        return MyMemSpace;
    }

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
        List<Integer> ids = new LinkedList<>();

        for(int index = 0; index < Max; index++)
        {
            if(MyMemSpace[index] != null) {
                if(!ids.contains(MyMemSpace[index].ID))
                {
                    MyMemSpace[index].NewCycle();
                    ids.add(MyMemSpace[index].ID);
                }
                if (MyMemSpace[index].GetTimeToLive() <= 0)
                {
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
        var openings = GetOpenSpaces(0);
        Opening bestFit = new Opening(-1,Max + 1); //Size is larger than whole array to help logic later on

        // Find the best spot
        for (Opening spot: openings)
        {
            // Will the proc fit?
            if(spot.Size >= proc.Size)
            {
                if(spot.Size < bestFit.Size)
                {
                    bestFit = spot;
                }
            }
        }

        // See if we were able to find the best spot
        if(bestFit.Start == -1) return false;

        Fill(bestFit.Start, proc.Size + bestFit.Start -1, proc);
        return true;
    }

    private boolean AddWorstFit(TimedProcess proc)
    {
        //todo Write this method
        System.out.println("Not implemented\n");
        return false;
    }


    private @NotNull
    Vector<Opening> GetOpenSpaces(int start)
    {
        Vector<Opening> openings = new Vector<>();

        openings.add(new Opening(-1, 0)); //Add a starter tuple

        for(int index = start; index < Max; index++)
        {
            var currentOpening = openings.get(openings.size()-1);
            //Found an empty spot
            if(MyMemSpace[index] == null)
            {
                // Check if this is the start of a new opening
                if(currentOpening.Start == -1)
                {
                    currentOpening.Start = index;
                }
                    currentOpening.Size = currentOpening.Size + 1;
            }
            // Spot is occupied
            else
            {
                if(currentOpening.Size > 0) // See if the last opening was used
                {
                    openings.add(new Opening(-1, 0)); //add a new opening
                }
                //if it wasn't, then don't do anything
            }
        }
        return openings;
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
              spot = (MyMemSpace[(index)] != null) ? "" + MyMemSpace[index].ID : "_";
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

    private class Opening {
        public int Start;
        public int Size;
        public Opening(int start,int end) {
            this.Start = start;
            this.Size = end;
        }

        public int GetEnd()
        {
            return (Start + Size - 1);
        }
    }
}

